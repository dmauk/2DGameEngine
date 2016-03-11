#include "MainGame.h"
#include <GameEngine2D/ErrorHandling.h>
#include <GameEngine2D\ResourceManager.h>

MainGame::MainGame() : _screenWidth(1024), _screenHeight(768), _gameState(GameState::PLAY), _time(0), _maxFPS(60.0f) //Initialization list
{
	_camera.init(_screenWidth, _screenHeight);
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

	_window.create("Game Engine", _screenWidth, _screenHeight, 0);

	initShaders();
	_spriteBatch.init();
	_fpsLimiter.init(_maxFPS);
}

void MainGame::initShaders()
{
	_colorProgram.compileShaders("Shaders/colorShading.vert","Shaders/colorShading.frag");
	_colorProgram.addAttribute("vertexPosition");
	_colorProgram.addAttribute("vertexColor");
	_colorProgram.addAttribute("vertexUV");
	_colorProgram.linkShaders();
}

void MainGame::gameLoop() { //FPS is number of times going through this loop per second essentially
	while (_gameState != GameState::EXIT) {
		_fpsLimiter.begin(); //Begin frame
		processInput();
		_camera.update();
		_time += 0.015;
		drawGame();
		_fps = _fpsLimiter.end();

		//Print every 10 frames
		static int count = 0;
		if (count == 999)
		{
			cout << _fps << endl; 
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

	//Now we will move continuously rather than just on an event. NOT BOUND BY EVENTS

	if (_inputManager.isKeyPressed(SDLK_w))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
	}
	if (_inputManager.isKeyPressed(SDLK_s))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
	}
	if (_inputManager.isKeyPressed(SDLK_a))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
	}
	if (_inputManager.isKeyPressed(SDLK_d))
	{
		_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
	}
	if (_inputManager.isKeyPressed(SDLK_q))
	{
		_camera.setScale(_camera.getScale() * SCALE_SPEED);
	}
	if (_inputManager.isKeyPressed(SDLK_e))
	{
		_camera.setScale(_camera.getScale() / SCALE_SPEED);
	}
	if (_inputManager.isKeyPressed(SDL_BUTTON_LEFT))
	{
		glm::vec2 mouseCoords = _inputManager.getMouseCoords();
		mouseCoords = _camera.convertScreenToWorldCoords(mouseCoords);
		cout << mouseCoords.x << " " << mouseCoords.y << endl;
	}

}

void MainGame::drawGame() {
	//Set base depth to 1.0
	glClearDepth(1.0);

	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	_colorProgram.use();
	//Texture
	glActiveTexture(GL_TEXTURE0); //Avoid multi texturing and use only first one. Look up multitexturing
	GLint textureLocation = _colorProgram.getUniformLocation("mySampler");
	glUniform1i(textureLocation, 0); //User gl active texture 1;

	//Uniform time
	GLint timeLocation = _colorProgram.getUniformLocation("time"); //Will give error if time variable doesn't exist
	glUniform1f(timeLocation, _time);//Handler to timeLocation in shader 

	//Set the cameraMatrix
	GLint orthographicProjectionMatrixLocation = _colorProgram.getUniformLocation("orthographicProjectionMatrix");
	glm::mat4 cameraMatrix = _camera.getCameraMatrix();

	glUniformMatrix4fv(orthographicProjectionMatrixLocation, 1, GL_FALSE, &(cameraMatrix[0][0])); //Need address of first element of 2d array

	_spriteBatch.begin();

	glm::vec4 pos(0.0f, 0.0f, 50.0f, 50.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static GameEngine2D::GLTexture texture = GameEngine2D::ResourceManager::getTexture("Textures/PNG/CharacterRight_Standing.png");
	GameEngine2D::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	_spriteBatch.draw(pos, uv, texture.id, 0.0f, color);
	

	_spriteBatch.end();

	_spriteBatch.renderBatch();


	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram.unuse();

	//Swap our buffer.
	_window.swapBuffer();
}
