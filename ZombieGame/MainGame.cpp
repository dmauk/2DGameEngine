#include "MainGame.h"
#include <GameEngine2D\GameEngine2D.h>
#include <GameEngine2D\ErrorHandling.h>
#include "Zombie.h"
#include <random>
#include <ctime>
#include <iostream>
#include "Gun.h"

using namespace std;

const float HUMAN_SPEED = 1.0f;
const float ZOMBIE_SPEED = 1.3f;

MainGame::MainGame() : _screenWidth(1024), _screenHeight(768), _gameState(GameState::PLAY), _fps(0.0f), _player(nullptr)
{
	
}


MainGame::~MainGame()
{
	for (int i = 0; i < _levels.size(); i++)
	{
		delete _levels[i];
	}
}


void MainGame::run() 
{
	initSystems();
	initLevel();
	gameLoop();
}


void MainGame::initSystems()
{
	GameEngine2D::init(); //Initialize sdl and set to double buffer.
	_window.create("Zombie Game", _screenWidth, _screenHeight, 0);
	initShaders();
	_agentSpriteBatch.init();
	_camera.init(_screenWidth, _screenHeight);

}

void MainGame::initLevel() //Change to take a string of the filePath for the level;
{
	_levels.push_back(new Level("Levels/level1.txt"));
	_currentLevel = 0;
	_player = new Player();
	_player->init(10.0f, _levels[_currentLevel]->getStartPlayerPos(), &_inputManager, &_camera, &_bullets);
	_humans.push_back(_player);

	mt19937 randomEngine(time(nullptr));
	uniform_int_distribution<int> randX(2, _levels[_currentLevel]->getWidth()-2);
	uniform_int_distribution<int> randY(2, _levels[_currentLevel]->getHeight()-2);



	//Add humans
	for (int i = 0; i < _levels[_currentLevel]->getNumHumans(); i++)
	{
		_humans.push_back(new Human);
		glm::vec2 pos(randX(randomEngine)* TILE_WIDTH, randY(randomEngine)*TILE_WIDTH);
		_humans.back()->init(HUMAN_SPEED, pos);
	}

	//Add the zombies
	const vector<glm::vec2>& zombiePositions = _levels[_currentLevel]->getStartZombiePoistions();
	for (int i = 0; i < zombiePositions.size(); i++)
	{
		_zombies.push_back(new Zombie);
		_zombies.back()->init(ZOMBIE_SPEED, zombiePositions[i]);
	}

	//Set up player's guns
	_player->addGun(new Gun("Shotgun", 60, 6, 20.0f, 5.0f, 10.0f));
	_player->addGun(new Gun("Rifle", 10, 1, 20.0f, 5.0f, 2.0f));
	_player->addGun(new Gun("Pistol", 60, 1, 20.0f, 1.0f, 1.0f));
}


void MainGame::initShaders()
{
	//Compile Shaders
	_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
	_textureProgram.linkShaders();
}


void MainGame::gameLoop()
{
	_fpsLimiter.setMaxFPS(60.0f);

	while (_gameState != GameState::EXIT)
	{
		_fpsLimiter.begin();
		processInput();
		updateAgents();
		updateBullets();
		_camera.setPosition(_player->getPosition());
		_camera.update();
		drawGame();
		_fps = _fpsLimiter.end();
	}
}


void MainGame::drawGame()
{
	//Set base depth to 1.0
	glClearDepth(1.0);
	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	_textureProgram.use();
	//Set active texture to 0
	glActiveTexture(GL_TEXTURE0);

	GLint textureUniform = _textureProgram.getUniformLocation("mySampler");
	glUniform1i(textureUniform, 0);

	glm::mat4 projectionMatrix = _camera.getCameraMatrix();
	GLint projectionUniform = _textureProgram.getUniformLocation("P");
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

	//Draw level
	_levels[_currentLevel]->draw();

	//Draw the humans

	_agentSpriteBatch.begin();
	for (int i = 0; i < _humans.size(); i++)
	{
		_humans[i]->draw(_agentSpriteBatch);
	}

	for (int i = 0; i < _zombies.size(); i++)
	{
		_zombies[i]->draw(_agentSpriteBatch);
	}

	for (int i = 0; i < _bullets.size(); i++)
	{
		_bullets[i].draw(_agentSpriteBatch);
	}

	_agentSpriteBatch.end();
	_agentSpriteBatch.renderBatch();

	//Swap our buffer and draw everything to the screen
	_textureProgram.unuse();
	_window.swapBuffer();
}


void MainGame::processInput()
{
	SDL_Event evnt;
	//Will keep looping until there are no more events to process
	while (SDL_PollEvent(&evnt))
	{
		switch (evnt.type) {
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION:
				_inputManager.setMouseCoords(evnt.motion.x, evnt.motion.y);
				break;
			case SDL_KEYDOWN:
				_inputManager.pressKey(evnt.key.keysym.sym);
				break;
			case SDL_KEYUP:
				_inputManager.releaseKey(evnt.key.keysym.sym);
				break;
			case SDL_MOUSEBUTTONDOWN:
				_inputManager.pressKey(evnt.button.button);
				break;
			case SDL_MOUSEBUTTONUP:
				_inputManager.releaseKey(evnt.button.button);
				break;
		}
	}
}

void MainGame::updateAgents()
{
	//Update humans
	for (int i = 0; i < _humans.size(); i++)
	{
		_humans[i]->update(_levels[_currentLevel]->getLevelData(),_humans,_zombies);
	}
	
	//Update all zombies
	for (int i = 0; i < _zombies.size(); i++)
	{
		_zombies[i]->update(_levels[_currentLevel]->getLevelData(), _humans, _zombies);
	}

	for (int i = 0; i < _zombies.size(); i++)
	{
		for (int j = i + 1; j < _zombies.size(); j++)
		{
			_zombies[i]->collidewWithAgent(_zombies[j]);
		}
		for (int j = 1; j < _humans.size(); j++)
		{
			if (_zombies[i]->collidewWithAgent(_humans[j]))
			{
				//Add the zombie
				_zombies.push_back(new Zombie);
				_zombies.back()->init(ZOMBIE_SPEED, _humans[j]->getPosition());
				//Delete the human
				delete _humans[j];
				_humans[j] = _humans.back();
				_humans.pop_back();
			}
		}
		//Collide with player
		if (_zombies[i]->collidewWithAgent(_player))
		{
			GameEngine2D::fatalError("YOU LOSE!");
		}
	}

	//Update Human collisions
	for (int i = 0; i < _humans.size(); i++) 
	{
		for (int j = i + 1; j < _humans.size(); j++)
		{
			_humans[i]->collidewWithAgent(_humans[j]);
		}
	}
}

void MainGame::updateBullets()
{
	for (int i = 0; i < _bullets.size(); i++)
	{
		_bullets[i].update(_humans, _zombies);
	}
}