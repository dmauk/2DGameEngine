#include "MainGame.h"
#include "ErrorHandling.h"

MainGame::MainGame() : _screenWidth(1024), _screenHeight(768), _gameState(GameState::PLAY), _time(0) //Initialization list
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

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	glClearColor(0.0f,0.0f,1.0f,1.0f);

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

void MainGame::gameLoop() {
	while (_gameState != GameState::EXIT) {
		processInput();
		_time += 0.015;
		drawGame();
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
