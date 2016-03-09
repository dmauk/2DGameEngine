#include "MainGame.h"
#include <GameEngine2D/ErrorHandling.h>

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
	_sprites.push_back(GameEngine2D::Sprite());
	_sprites.back().init(0.0f, 0.0f, _screenWidth/2, _screenHeight/2, "Textures/PNG/CharacterRight_Standing.png");


	gameLoop();
}

void MainGame::initSystems()
{
	//initialize sdl
	GameEngine2D::init();

	_window.create("Game Engine", _screenWidth, _screenHeight, 0);

	initShaders();
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

		float startTicks = SDL_GetTicks();
		processInput();
		_camera.update();
		_time += 0.015;
		drawGame();
		calculateFPS(); 
		static int count = 0;
		if (count == 9)
		{
			cout << _fps << endl; //Will slow down frame rate //Print every 10
			count = 0;
		}
		count++;

		//Limit FPS //Already handled by vsync however
		float frameTicks = SDL_GetTicks() - startTicks;
		if (1000.0f / _maxFPS > frameTicks)
		{													
			SDL_Delay(1000.0f / _maxFPS - frameTicks); // 1s / 60 = every 0.0166 second we have a frame. So every 1000*0.0166 = 16.66ms we have one frame. 
		}											   // if we have more frames in a shorter ms interval then we want to delay to the standard 60s interval. 
	}
}

void MainGame::processInput() {
	SDL_Event evnt;
	const float CAMERA_SPEED = 10.0f;
	const float SCALE_SPEED = 0.95f;
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
			case SDL_MOUSEMOTION:
				//cout << evnt.motion.x << " "<<evnt.motion.y<<endl;
				break;
			case SDL_KEYDOWN:
				switch (evnt.key.keysym.sym) {
					//Get the input and use it to move the camera
					//THIS IS TEMPORARY
				case SDLK_w:
					_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));
					break;
				case SDLK_s:
					_camera.setPosition(_camera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
					break;
				case SDLK_a:
					_camera.setPosition(_camera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));
					break;
				case SDLK_d:
					_camera.setPosition(_camera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));
					break;
				case SDLK_q:
					_camera.setScale(_camera.getScale() * SCALE_SPEED);
					break;
				case SDLK_e:
					_camera.setScale(_camera.getScale() / SCALE_SPEED);
					break;
				}
				break;
		}
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

	//Draw sprite
	for (int i = 0; i < _sprites.size(); i++)
	{
		_sprites[i].draw();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram.unuse();

	//Swap our buffer.
	_window.swapBuffer();
}

void MainGame::calculateFPS()
{
	static const int NUM_SAMPLES = 10; //Used to average last 10 frames for smooth frame rate measurment
	static float frameTimes[NUM_SAMPLES];
	static int currentBufferLocation;
	static float prevTicks = SDL_GetTicks();
	float currentTicks;
	currentTicks = SDL_GetTicks(); 
	_frameTime = currentTicks - prevTicks; //Will be incorrect on the first reading because it will be 0
	frameTimes[currentBufferLocation%NUM_SAMPLES] = _frameTime; //Use modulus to do a circular buffer.

	prevTicks = currentTicks;//Update previous ticks.
	int count;
	currentBufferLocation++; //Avoid divide by zero by iterating here
	if (currentBufferLocation < NUM_SAMPLES)//In the first few moments, the averages will based off numbers 1- 10 rather than 10 for accuracy.
	{
		count = currentBufferLocation;
	}
	else
	{
		count = NUM_SAMPLES;//Eventually everything will be averaged by NUM_SAMPLES or 10
	}
	
	float frameAverage = 0;
	for(int i = 0; i < count; i++)
	{
		frameAverage += frameTimes[i];
	}
	frameAverage /= count;

	if (frameAverage > 0)
	{
		_fps = 1000.0f / frameAverage; //frameAverage = FPMS -> How many frames is that a second? 1 second = 1000 ms so 1000 / FPMS = FPS
	}
	else
	{
		_fps = 60.0f; //This will only occur in the first case when the frame calculation will divide by zero
	}
	
}