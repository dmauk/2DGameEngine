#include "MainGame.h"
#include <GameEngine2D/ErrorHandling.h>
#include <GameEngine2D\ResourceManager.h>

MainGame::MainGame() : m_screenWidth(1024), m_screenHeight(768), m_gameState(GameState::PLAY), m_time(0), m_maxFPS(60.0f) //Initialization list
{
	m_camera.init(m_screenWidth, m_screenHeight);
}


MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();
	//_playerTexture = ImageLoader::loadPNG("Textures/PNG/CharacterRight_Standing.png");


	gameLoop();
}

void MainGame::initSystems()
{
	//initialize sdl
	GameEngine2D::init();

	m_window.create("Game Engine", m_screenWidth, m_screenHeight, 0);

	initShaders();
	m_spriteBatch.init();
	m_fpsLimiter.init(m_maxFPS);
}

void MainGame::initShaders()
{
	m_colorProgram.compileShaders("Shaders/colorShading.vert","Shaders/colorShading.frag");
	m_colorProgram.addAttribute("vertexPosition");
	m_colorProgram.addAttribute("vertexColor");
	m_colorProgram.addAttribute("vertexUV");
	m_colorProgram.linkShaders();
}

void MainGame::gameLoop() { //FPS is number of times going through this loop per second essentially
	while (m_gameState != GameState::EXIT) {
		m_fpsLimiter.begin(); //Begin frame
		processInput();
		m_camera.update();

		for (int i = 0; i < m_projectiles.size();) //moved i++ down
		{
			m_projectiles[i].update();
			if (m_projectiles[i].update() == true)
			{
				m_projectiles[i] = m_projectiles.back(); //Remove element when order doesn't matter
				m_projectiles.pop_back();
			}
			else
			{
				i++;
			}
		}
		m_time += 0.015;
		drawGame();
		m_fps = m_fpsLimiter.end();

		//Print every 10 frames
		static int count = 0;
		if (count == 999)
		{
			cout << m_fps << endl; 
			count = 0;
		}
		count++;
	}
}

void MainGame::processInput() {
	SDL_Event evnt;
	const float CAMERA_SPEED = 2.0f;
	const float SCALE_SPEED = 0.95f;
	while (SDL_PollEvent(&evnt)) {
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

	//Now we will move continuously rather than just on an event. NOT BOUND BY EVENTS

	if (m_inputManager.isKeyDown(SDLK_w))
	{
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	}
	if (m_inputManager.isKeyDown(SDLK_s))
	{
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}
	if (m_inputManager.isKeyDown(SDLK_a))
	{
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	}
	if (m_inputManager.isKeyDown(SDLK_d))
	{
		m_camera.setPosition(m_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	}
	if (m_inputManager.isKeyDown(SDLK_q))
	{
		m_camera.setScale(m_camera.getScale() * SCALE_SPEED);
	}
	if (m_inputManager.isKeyDown(SDLK_e))
	{
		m_camera.setScale(m_camera.getScale() / SCALE_SPEED);
	}
	if (m_inputManager.isKeyDown(SDL_BUTTON_LEFT))
	{
		glm::vec2 mouseCoords = m_inputManager.getMouseCoords();
		mouseCoords = m_camera.convertScreenToWorldCoords(mouseCoords);
		glm::vec2 playerPosition(0.0f);
		glm::vec2 direction = mouseCoords - playerPosition;
		direction = glm::normalize(direction);
		m_projectiles.emplace_back(playerPosition, direction, 1.0f, 100);
	}

}

void MainGame::drawGame() {
	//Set base depth to 1.0
	glClearDepth(1.0);

	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_colorProgram.use();
	//Texture
	glActiveTexture(GL_TEXTURE0); //Avoid multi texturing and use only first one. Look up multitexturing
	GLint textureLocation = m_colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0); //User gl active texture 1;

	//Uniform time
	GLint timeLocation = m_colorProgram.getUniformLocation("time"); //Will give error if time variable doesn't exist
	glUniform1f(timeLocation, m_time);//Handler to timeLocation in shader 

	//Set the cameraMatrix
	GLint orthographicProjectionMatrixLocation = m_colorProgram.getUniformLocation("orthographicProjectionMatrix");
	glm::mat4 cameraMatrix = m_camera.getCameraMatrix();

	glUniformMatrix4fv(orthographicProjectionMatrixLocation, 1, GL_FALSE, &(cameraMatrix[0][0])); //Need address of first element of 2d array

	m_spriteBatch.begin();

	glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static GameEngine2D::GLTexture texture = GameEngine2D::ResourceManager::getTexture("Textures/PNG/CharacterRight_Standing.png");
	GameEngine2D::ColorRGBA8 color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	m_spriteBatch.draw(pos, uv, texture.id, 0.0f, color);

	for (int i = 0; i < m_projectiles.size(); i++)
	{
		m_projectiles[i].draw(m_spriteBatch);

	}

	m_spriteBatch.end();

	m_spriteBatch.renderBatch();


	glBindTexture(GL_TEXTURE_2D, 0);
	m_colorProgram.unuse();

	//Swap our buffer.
	m_window.swapBuffer();
}
