#pragma once

#include <GameEngine2D\GLSLProgram.h>
#include <GameEngine2D\Window.h>
#include <GameEngine2D\InputManager.h>

enum class GameState { PLAY, EXIT };

class MainGame {
public:
	MainGame();
	~MainGame();
	void run();

private:
	void initSystems();
	void initShaders();
	void gameLoop();
	void drawGame();
	void processInput();
	GameState _gameState;
	GameEngine2D::GLSLProgram _textureProgram;
	GameEngine2D::Window _window;
	GameEngine2D::InputManager _inputManager;
};