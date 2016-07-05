#include "Player.h"
#include <SDL\SDL.h>
#include "Gun.h"
#include <iostream>
#include <GameEngine2D\ResourceManager.h>

using namespace std;

Player::Player() : m_currentGun(-1)
{
}


Player::~Player()
{
}

void Player::init(float health, float speed, glm::vec2 position, GameEngine2D::InputManager* inputManager, GameEngine2D::Camera2D * camera, vector<Bullet>* bullets) //Dependency injection
{
	m_speed = speed;
	m_position = position;
	m_inputManager = inputManager;
	m_color = GameEngine2D::ColorRGBA8(255, 255, 255, 255);
	m_camera = camera;
	m_bullets = bullets;
	m_health = health;
	m_textureID = GameEngine2D::ResourceManager::getTexture("Textures/player.png").id;
}

void Player::update(const vector<string>& levelData, vector<Human*>& humans, vector<Zombie*>& zombies, float deltaTime)
{
	if (m_inputManager->isKeyDown(SDLK_w)) {
		m_position.y += m_speed * deltaTime;
	}
	if (m_inputManager->isKeyDown(SDLK_s)) 
	{
		m_position.y -= m_speed * deltaTime;
	}
	if ((m_inputManager->isKeyDown(SDLK_a)))
	{
		m_position.x -= m_speed * deltaTime;
	}
	if (m_inputManager->isKeyDown(SDLK_d))
	{
		m_position.x += m_speed * deltaTime;
	}
	if (m_inputManager->isKeyPressed(SDLK_1)&&m_guns.size()>0)
	{
		m_currentGun = 0;
	}
	else if (m_inputManager->isKeyPressed(SDLK_2) && m_guns.size()>1)
	{
		cout << "Equip GUN 2 " << endl;
		m_currentGun = 1;
	}
	else if (m_inputManager->isKeyPressed(SDLK_3) && m_guns.size()>2)
	{
		cout << "Equip GUN 3 " << endl;
		m_currentGun = 2;
	}
	else if (m_inputManager->isKeyPressed(SDLK_4) && m_guns.size()>3)
	{
		cout << "Equip GUN 4 " << endl;
		m_currentGun = 3;
	}
	if (m_inputManager->isKeyPressed(SDLK_5) && m_guns.size()>4)
	{
		m_currentGun = 4;
	}

	glm::vec2 mouseCoords = m_camera->convertScreenToWorldCoords(m_inputManager->getMouseCoords());
	glm::vec2 centerPosition = m_position + glm::vec2(AGENT_RADIUS);
	m_direction = glm::normalize(mouseCoords - centerPosition);
	if (m_currentGun > -1 && m_currentGun < m_guns.size() + 1)
	{
		m_guns[m_currentGun]->update(m_inputManager->isKeyDown(SDL_BUTTON_LEFT), m_direction, *m_bullets, centerPosition, deltaTime);
	}
	if(m_inputManager)
	collideWithLevel(levelData);
}

void Player::addGun(Gun* gun)
{
	m_guns.push_back(gun);

	//If no gun equipped, equip gun
	m_currentGun = 0;
}