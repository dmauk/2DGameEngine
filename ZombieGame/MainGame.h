#pragma once

#include <GameEngine2D\GLSLProgram.h>
#include <GameEngine2D\Window.h>
#include <GameEngine2D\InputManager.h>
#include <GameEngine2D\Camera2D.h>
#include <GameEngine2D\Timing.h>
#include <GameEngine2D\SpriteBatch.h>
#include "Level.h"
#include "Player.h" //Will also include human and agent
#include "Bullet.h"

using namespace std;

class Zombie;

enum class GameState { PLAY, EXIT };

class MainGame {
public:
	MainGame();
	~MainGame();
	void run();

private:

	//Initializes SDL
	void initSystems();

	//Initializes shaders
	void initShaders();

	//Initialize levels
	void initLevel();

	//Main game loop
	void gameLoop();
	void drawGame();
	void processInput();
	void updateAgents(); //Updates all agents
	void updateBullets();
	GameState _gameState;
	GameEngine2D::GLSLProgram _textureProgram;
	GameEngine2D::Window _window;
	GameEngine2D::InputManager _inputManager;
	GameEngine2D::Camera2D _camera;
	GameEngine2D::SpriteBatch _agentSpriteBatch;
	int _screenWidth;
	int _screenHeight;
	vector<Level*> _levels;
	GameEngine2D::FpsLimiter _fpsLimiter;
	float _fps;
	int _currentLevel;
	Player* _player;
	vector<Human*> _humans;
	vector<Zombie*> _zombies;
	vector<Bullet> _bullets;
};