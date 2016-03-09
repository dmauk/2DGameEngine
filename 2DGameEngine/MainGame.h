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
	void calculateFPS();
	void processInput();
	GameEngine2D::Window _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;
	GameEngine2D::Camera2D _camera;
	GameEngine2D::GLSLProgram _colorProgram;
	float _time;
	vector<GameEngine2D::Sprite> _sprites;
	float _fps;
	float _frameTime;
	float _maxFPS;
};

