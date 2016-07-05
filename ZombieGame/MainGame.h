#pragma once

#include <GameEngine2D\GLSLProgram.h>
#include <GameEngine2D\Window.h>
#include <GameEngine2D\InputManager.h>
#include <GameEngine2D\Camera2D.h>
#include <GameEngine2D\Timing.h>
#include <GameEngine2D\SpriteBatch.h>
#include <GameEngine2D\SpriteFont.h>
#include <GameEngine2D\AudioEngine.h>
#include <GameEngine2D\ParticleEngine2D.h>
#include <GameEngine2D\ParticleBatch2D.h>
#include "Level.h"
#include "Player.h" //Will also include human and agent
#include "Bullet.h"

using namespace std;

class Zombie;

enum class GameState { PLAY, EXIT, PAUSE };

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
	void drawHud();
	void processInput();
	void updateAgents(float deltaTime); //Updates all agents
	void updateBullets(float deltaTime);
	void addBlood(const glm::vec2& position, int numParticles);
	GameState m_gameState;
	GameEngine2D::GLSLProgram m_textureProgram;
	GameEngine2D::Window m_window;
	GameEngine2D::InputManager m_inputManager;
	GameEngine2D::Camera2D m_camera;
	GameEngine2D::Camera2D m_hudCamera;
	GameEngine2D::SpriteBatch m_agentSpriteBatch;
	GameEngine2D::SpriteBatch m_hudSpriteBatch;
	GameEngine2D::SpriteFont* m_spriteFont;
	GameEngine2D::AudioEngine m_audioEngine;
	GameEngine2D::ParticleEngine2D m_particleEngine;
	GameEngine2D::ParticleBatch2D* m_bloodParticleBatch;
	int m_screenWidth;
	int m_screenHeight;
	vector<Level*> m_levels;
	GameEngine2D::FpsLimiter m_fpsLimiter;
	float m_fps;
	int m_currentLevel;
	Player* m_player;
	vector<Human*> m_humans;
	vector<Zombie*> m_zombies;
	vector<Bullet> m_bullets;
	int m_numHumansKilled;
	int m_numZombiesKilled;
};