#include "MainGame.h"
#include <GameEngine2D\GameEngine2D.h>
MainGame::MainGame()
{
	
}


MainGame::~MainGame()
{

}


void MainGame::run() 
{
	initSystems();
	gameLoop();
}


void MainGame::initSystems()
{
	GameEngine2D::init(); //Initialize sdl and set to double buffer.
	_window.create("Zombie Game", 1024, 768, 0);
	initShaders();
	_gameState = GameState::PLAY;
}


void MainGame::initShaders()
{
	//Compile Shaders
	_textureProgram.compileShaders("Shaders/textureShading.vert", "Shaders/textureShading.frag");
	_textureProgram.addAttribute("vertexPosition");
	_textureProgram.addAttribute("vertexColor");
	_textureProgram.addAttribute("vertexUV");
}


void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
	}
}


void MainGame::drawGame()
{
	//Set base depth to 1.0
	glClearDepth(1.0);
	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Swap our buffer and draw everything to the screen
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
