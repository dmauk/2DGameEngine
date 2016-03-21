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
	GameEngine2D::Window m_window;
	int m_screenWidth;
	int m_screenHeight;
	GameState m_gameState;
	GameEngine2D::Camera2D m_camera;
	GameEngine2D::GLSLProgram m_colorProgram;
	float m_time;
	float m_maxFPS;
	float m_fps;
	GameEngine2D::SpriteBatch m_spriteBatch;
	GameEngine2D::InputManager m_inputManager;
	GameEngine2D::FpsLimiter m_fpsLimiter; 
	vector<Projectile> m_projectiles;
};

