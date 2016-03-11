#pragma once
#include<iostream>
#include<string>
#include<GameEngine2D/Sprite.h>
#include<SDL\SDL.h>
#include<GL\glew.h>
#include<GameEngine2D/GLSLProgram.h>
#include<GameEngine2D/GLTexture.h>
#include<GameEngine2D/Window.h>
#include <vector>
#include<GameEngine2D/GameEngine2D.h>
#include<GameEngine2D/Camera2D.h>
#include<GameEngine2D/SpriteBatch.h>
#include<GameEngine2D\InputManager.h>
#include<GameEngine2D\Timing.h>
#include "Projectile.h"

using namespace std;

enum class GameState{PLAY,EXIT};

class MainGame
{
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
	GameEngine2D::Window _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;
	GameEngine2D::Camera2D _camera;
	GameEngine2D::GLSLProgram _colorProgram;
	float _time;
	float _maxFPS;
	float _fps;
	GameEngine2D::SpriteBatch _spriteBatch;
	GameEngine2D::InputManager _inputManager;
	GameEngine2D::FpsLimiter _fpsLimiter; 
	vector<Projectile> _projectiles;
};

