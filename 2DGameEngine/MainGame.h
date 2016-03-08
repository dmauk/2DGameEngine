#pragma once
#include<iostream>
#include<string>
#include"Sprite.h"
#include<SDL\SDL.h>
#include<GL\glew.h>
#include"GLSLProgram.h"
#include"GLTexture.h"
#include <vector>

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
	SDL_Window* _window;
	int _screenWidth;
	int _screenHeight;
	GameState _gameState;
	GLSLProgram _colorProgram;
	float _time;
	vector<Sprite> _sprites;

};

