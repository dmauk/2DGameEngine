#include "MainGame.h"
#include "ErrorHandling.h"

MainGame::MainGame() : _screenWidth(1024), _screenHeight(768), _gameState(GameState::PLAY), _time(0), _maxFPS(60.0f) //Initialization list
{
}


MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems();
	//_playerTexture = ImageLoader::loadPNG("Textures/PNG/CharacterRight_Standing.png");
	_sprites.push_back(Sprite());
	_sprites.back().init(-1.0f, -1.0f, 2.0f, 2.0f, "Textures/PNG/CharacterRight_Standing.png");

	_sprites.push_back(Sprite());
	_sprites.back().init(0.0f, -1.0f, 2.0f, 2.0f, "Textures/PNG/CharacterRight_Standing.png");

	gameLoop();
}

void MainGame::initSystems()
{
	//initialize sdl
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); //Set state before creating window

	_window = SDL_CreateWindow("Game Engine v0.01", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, _screenWidth, _screenHeight, SDL_WINDOW_OPENGL);
	if (_window == nullptr) {
		fatalError("SDL Window could not be created!");
	}
	SDL_GLContext glContext = SDL_GL_CreateContext(_window);
	if (glContext == nullptr) {
		fatalError("SDL_GL context could not be created!");
	}
	GLenum error = glewInit();
	if (error != GLEW_OK) {
		fatalError("Could not initialize glew");
	};
	//Display OPENGL VERSION FOR USER
	cout << "OPENGL VERSION: " << glGetString(GL_VERSION) << endl; //GL_VERSION MACRO IS SET TO WHATEVER OPENGL VERSION YOU ARE RUNNING
	glClearColor(0.0f,0.0f,1.0f,1.0f);
	SDL_GL_SetSwapInterval(1); //Enable/Disables V-Sync
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
	while (SDL_PollEvent(&evnt)) {
		switch (evnt.type) {
		case SDL_QUIT:
			_gameState = GameState::EXIT;
			break;
		case SDL_MOUSEMOTION:
			//cout << evnt.motion.x << " "<<evnt.motion.y<<endl;
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

	//Draw sprite
	for (int i = 0; i < _sprites.size(); i++)
	{
		_sprites[i].draw();
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	_colorProgram.unuse();

	//Swap our buffer.
	
	SDL_GL_SwapWindow(_window);
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