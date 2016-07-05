#include "MainGame.h"
#include <GameEngine2D\GameEngine2D.h>
#include <GameEngine2D\ErrorHandling.h>
#include <GameEngine2D\ResourceManager.h>
#include <GLM/gtx/rotate_vector.hpp>
#include "Zombie.h"
#include <random>
#include <ctime>
#include <iostream>
#include "Gun.h"
#include <algorithm>

using namespace std;

const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.3f;

MainGame::MainGame() : m_screenWidth(1024), m_screenHeight(768), m_gameState(GameState::PLAY), m_fps(0.0f), m_player(nullptr)
{

}


MainGame::~MainGame()
{
	for (int i = 0; i < m_levels.size(); i++)
	{
		delete m_levels[i];
	}

	for (int i = 0; i < m_zombies.size(); i++)
	{
		delete m_zombies[i];
	}

	for (int i = 0; i < m_humans.size(); i++)
	{
		delete m_humans[i];
	}
}


void MainGame::run() 
{
	initSystems();
	initLevel();
	GameEngine2D::Music music = m_audioEngine.loadMusic("Sound/Music/XYZ.ogg");
	music.play(-1); //loop forever
	gameLoop();
}

//Use in case you forget about lambdas. Ayyy 
//void updateBloodParticle(GameEngine2D::Particle2D& particle, float deltaTime)
//{
//	particle.position += particle.velocity * deltaTime;
//	particle.color.a = GLubyte(particle.life*255.0f);
//}

void MainGame::initSystems()
{
	//Initialize Game Engine
	GameEngine2D::init(); //Initialize sdl and set to double buffer.

	//Initialize Audio Engine.
	m_audioEngine.init();

	//Create Window
	m_window.create("Zombie Game", m_screenWidth, m_screenHeight, 0);

	//Set up shaders
	initShaders();

	//Initialize sprite batches
	m_agentSpriteBatch.init();
	m_hudSpriteBatch.init();

	//Initialize sprite font
	m_spriteFont = new GameEngine2D::SpriteFont("Fonts/arial.ttf", 32);

	//Initialize camera
	m_camera.init(m_screenWidth, m_screenHeight);
	m_hudCamera.init(m_screenWidth, m_screenHeight);
	m_hudCamera.setPosition(glm::vec2(m_screenWidth / 2, m_screenHeight / 2));


	//Options for lambda brackets [] nothing [=] pass in a local variable from this function [&] pass in a reference
	//Initialize particles
	m_bloodParticleBatch = new GameEngine2D::ParticleBatch2D;
	m_bloodParticleBatch->init(1000, 0.01f, GameEngine2D::ResourceManager::getTexture("Textures/particle.png"), [](GameEngine2D::Particle2D& particle, float deltaTime) {
		particle.position += particle.velocity * deltaTime;
		particle.color.a = GLubyte(particle.life * 255.0f);
	});
	m_particleEngine.addParticleBatch(m_bloodParticleBatch);

}

void MainGame::initLevel() //Change to take a string of the filePath for the level;
{

	const int PLAYER_HEALTH = 100;
	const int HUMAN_HEALTH = 100;
	const int ZOMBIE_HEALTH = 60;

	m_levels.push_back(new Level("Levels/level1.txt"));
	m_currentLevel = 0;
	m_player = new Player();
	m_player->init(PLAYER_HEALTH,10.0f, m_levels[m_currentLevel]->getStartPlayerPos(), &m_inputManager, &m_camera, &m_bullets);
	m_humans.push_back(m_player);

	mt19937 randomEngine(time(nullptr));
	uniform_int_distribution<int> randX(2, m_levels[m_currentLevel]->getWidth()-2);
	uniform_int_distribution<int> randY(2, m_levels[m_currentLevel]->getHeight()-2);



	//Add humans
	for (int i = 0; i < m_levels[m_currentLevel]->getNumHumans(); i++)
	{
		m_humans.push_back(new Human);
		glm::vec2 pos(randX(randomEngine)* TILE_WIDTH, randY(randomEngine)*TILE_WIDTH);
		m_humans.back()->init(HUMAN_HEALTH,HUMAN_SPEED, pos);
	}

	//Add the zombies
	const vector<glm::vec2>& zombiePositions = m_levels[m_currentLevel]->getStartZombiePoistions();
	for (int i = 0; i < zombiePositions.size(); i++)
	{
		m_zombies.push_back(new Zombie);
		m_zombies.back()->init(ZOMBIE_HEALTH,ZOMBIE_SPEED, zombiePositions[i]);
	}

	//Set up player's guns
	m_player->addGun(new Gun("Shotgun", 30, 6, 20.0f, 5.0f, 10.0f, 360.0f, m_audioEngine.loadSoundEffect("Sound/Shots/shotgun.wav")));
	m_player->addGun(new Gun("Rifle", 1, 1, 20.0f, 5.0f, 4.0f, 360.0f, m_audioEngine.loadSoundEffect("Sound/Shots/rifle.wav")));
	m_player->addGun(new Gun("Pistol", 20, 1, 20.0f, 1.0f, 1.0f, 360.0f, m_audioEngine.loadSoundEffect("Sound/Shots/pistol.wav")));
	m_numHumansKilled = 0;
	m_numZombiesKilled = 0;
}


void MainGame::initShaders()
{
	//Compile Shaders
	m_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	m_textureProgram.addAttribute("vertexPosition");
	m_textureProgram.addAttribute("vertexColor");
	m_textureProgram.addAttribute("vertexUV");
	m_textureProgram.linkShaders();
}


void MainGame::gameLoop()
{
	m_fpsLimiter.setMaxFPS(60.0f);

	const float DESIRED_FPS = 60;
	const int MAX_PHYSICS_STEPS = 6;
	const float MS_PER_SECOND = 1000;
	const float DESIRED_FRAME_TIME = MS_PER_SECOND / DESIRED_FPS;
	const float MAX_DELTA_TIME = 1.0f;

	float previousTicks = SDL_GetTicks(); //number of ticks since sdl was initialized


	while (m_gameState != GameState::EXIT)
	{
		float newTicks = SDL_GetTicks();
		float frameTime = newTicks - previousTicks; //Frame time is inversely proportional to fps
		previousTicks = newTicks;
		float totalDeltaTime = frameTime / DESIRED_FRAME_TIME;

		m_fpsLimiter.begin();

		m_inputManager.update();

		processInput();

		int i = 0;
		while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS) //Risk spiral of death is cpu takes too long
		{
			float deltaTime = min(totalDeltaTime, MAX_DELTA_TIME);
			updateAgents(deltaTime);
			updateBullets(deltaTime);
			m_particleEngine.update(deltaTime);
			totalDeltaTime -= deltaTime;
			i++;
		}

		

		m_camera.setPosition(m_player->getPosition());

		m_camera.update();
		m_hudCamera.update();

		drawGame();

		m_fps = m_fpsLimiter.end();
	}
}


void MainGame::drawGame()
{
	//Set base depth to 1.0
	glClearDepth(1.0);
	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	m_textureProgram.use();
	//Set active texture to 0
	glActiveTexture(GL_TEXTURE0);

	GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	glm::mat4 projectionMatrix = m_camera.getCameraMatrix();
	GLint projectionUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//Draw level
	m_levels[m_currentLevel]->draw();

	//Draw the humans

	glm::vec2 agentDims(AGENT_RADIUS * 2.0f);

	m_agentSpriteBatch.begin();
	for (int i = 0; i < m_humans.size(); i++)
	{
		if(m_camera.isBoxInView(m_humans[i]->getPosition(), agentDims))
		{
			m_humans[i]->draw(m_agentSpriteBatch);
		}
	}

	for (int i = 0; i < m_zombies.size(); i++)
	{
		if (m_camera.isBoxInView(m_zombies[i]->getPosition(), agentDims))
		{
			m_zombies[i]->draw(m_agentSpriteBatch);
		}
	}

	for (int i = 0; i < m_bullets.size(); i++)
	{
		m_bullets[i].draw(m_agentSpriteBatch);
	}

	m_agentSpriteBatch.end();
	m_agentSpriteBatch.renderBatch();

	//Render the particles
	m_particleEngine.draw(&m_agentSpriteBatch);

	drawHud();

	//Swap our buffer and draw everything to the screen
	m_textureProgram.unuse();
	m_window.swapBuffer();
}


void MainGame::processInput()
{
	SDL_Event evnt;
	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type) {
			case SDL_QUIT:
				m_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION:
				m_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
				break;
			case SDL_KEYDOWN:
				m_inputManager.pressKey(evnt.key.keysym.sym);
				break;
			case SDL_KEYUP:
				m_inputManager.releaseKey(evnt.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				m_inputManager.pressKey(evnt.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				m_inputManager.releaseKey(evnt.button.button);
				break;
		}
	}
}

void MainGame::updateAgents(float deltaTime)
{
	const int ZOMBIE_HEALTH = 60;
	//Update humans
	for (int i = 0; i < m_humans.size(); i++)
	{
		m_humans[i]->update(m_levels[m_currentLevel]->getLevelData(),m_humans,m_zombies, deltaTime);
	}
	
	//Update all zombies
	for (int i = 0; i < m_zombies.size(); i++)
	{
		m_zombies[i]->update(m_levels[m_currentLevel]->getLevelData(), m_humans, m_zombies, deltaTime);
	}

	for (int i = 0; i < m_zombies.size(); i++)
	{
		for (int j = i + 1; j < m_zombies.size(); j++)
		{
			m_zombies[i]->collidewWithAgent(m_zombies[j]);
		}
		for (int j = 1; j < m_humans.size(); j++)
		{
			if (m_zombies[i]->collidewWithAgent(m_humans[j]))
			{
				//Add the zombie
				m_zombies.push_back(new Zombie);
				m_zombies.back()->init(ZOMBIE_HEALTH,ZOMBIE_SPEED, m_humans[j]->getPosition());
				//Delete the human
				delete m_humans[j];
				m_humans[j] = m_humans.back();
				m_humans.pop_back();
				m_numHumansKilled++;
			}
		}
		//Collide with player
		if (m_zombies[i]->collidewWithAgent(m_player))
		{
			GameEngine2D::fatalError("YOU LOSE!");
		}
	}

	//Update Human collisions
	for (int i = 0; i < m_humans.size(); i++) 
	{
		for (int j = i + 1; j < m_humans.size(); j++)
		{
			m_humans[i]->collidewWithAgent(m_humans[j]);
		}
	}
}

void MainGame::updateBullets(float deltaTime)
{
	for (int i = 0; i < m_bullets.size();)
	{
		if (m_bullets[i].update(m_humans, m_zombies, m_levels[m_currentLevel]->getLevelData(), deltaTime)) //if bullet should dissapear, pop it 
		{
			m_bullets[i] = m_bullets.back();
			m_bullets.pop_back();
		}
		else i++;

	}

	bool bulletDeleted = false;

	for (int i = 0; i < m_bullets.size(); i++)
	{
		for (int j = 0; j < m_zombies.size();)
		{
			if (m_bullets[i].collidewWithAgent(m_zombies[j]))
			{
				//Add blood
				addBlood(m_bullets[i].getPosition(), 5);

				if (m_zombies[j]->applyDamage(m_bullets[i].getDamage()))
				{
					delete m_zombies[j];
					m_zombies[j] = m_zombies.back();
					m_zombies.pop_back();
					m_numZombiesKilled++;
				}

				m_bullets[i] = m_bullets.back();
				m_bullets.pop_back();
				i--;//Don't skip bullet
				//Don't need to check for an already deleted bullet
				bulletDeleted = true;
				break;
				
			}
			else
			{
				j++;
			}
		}
		if(bulletDeleted == false)
		{
			for (int j = 1; j < m_humans.size();)
			{
				if (m_bullets[i].collidewWithAgent(m_humans[j]))
				{
					//Add blood
					addBlood(m_bullets[i].getPosition(), 5);

					if (m_humans[j]->applyDamage(m_bullets[i].getDamage()))
					{
						delete m_humans[j];
						m_humans[j] = m_humans.back();
						m_humans.pop_back();
					}

					m_bullets[i] = m_bullets.back();
					m_bullets.pop_back();
					i--;
					break;
				}
				else
				{
					j++;
				}

			}
		}
		
		bulletDeleted = false;
		
	}
}

void MainGame::drawHud()
{
	char buffer[256];

	glm::mat4 projectionMatrix = m_hudCamera.getCameraMatrix();
	GLint projectionUniform = m_textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	m_hudSpriteBatch.begin();
	
	sprintf_s(buffer, "Num Humans %d", m_humans.size());
	m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0, 0), glm::vec2(0.5), 0.0f, GameEngine2D::ColorRGBA8(255, 255, 255, 255), GameEngine2D::Justification::LEFT);

	sprintf_s(buffer, "Num Zombies %d", m_zombies.size());
	m_spriteFont->draw(m_hudSpriteBatch, buffer, glm::vec2(0, 32), glm::vec2(0.5), 0.0f, GameEngine2D::ColorRGBA8(255, 255, 255, 255), GameEngine2D::Justification::LEFT);
	
	m_hudSpriteBatch.end();
	m_hudSpriteBatch.renderBatch();
}

void MainGame::addBlood(const glm::vec2& position, int numParticles)
{
	static mt19937 randEngine(time(nullptr));
	static uniform_real_distribution<float> randAngle(0.0f, 2.0f * M_PI);
	glm::vec2 velocity(2.0f, 0.0f);
	GameEngine2D::ColorRGBA8 color(255, 0, 0, 255);

	for (int i = 0; i < numParticles; i++)
	{
		m_bloodParticleBatch->addParticle(position, glm::rotate(velocity, randAngle(randEngine)), color, 10.0f);
	}
	
}