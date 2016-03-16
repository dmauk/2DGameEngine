#include "Player.h"
#include <SDL\SDL.h>
#include "Gun.h"
#include <iostream>

using namespace std;

Player::Player() : _currentGun(-1)
{
}


Player::~Player()
{
}

void Player::init(float health, float speed, glm::vec2 position, GameEngine2D::InputManager* inputManager, GameEngine2D::Camera2D * camera, vector<Bullet>* bullets) //Dependency injection
{
	_speed = speed;
	_position = position;
	_inputManager = inputManager;
	_color.r = 0;
	_color.g = 0;
	_color.b = 100;
	_color.a = 255;
	_camera = camera;
	_bullets = bullets;
	_health = health;
}

void Player::update(const vector<string>& levelData, vector<Human*>& humans, vector<Zombie*>& zombies)
{
	if (_inputManager->isKeyPressed(SDLK_w)) {
		_position.y += _speed;
	}
	if (_inputManager->isKeyPressed(SDLK_s)) 
	{
		_position.y -= _speed;
	}
	if ((_inputManager->isKeyPressed(SDLK_a)))
	{
		_position.x -= _speed;
	}
	if (_inputManager->isKeyPressed(SDLK_d))
	{
		_position.x += _speed;
	}
	if (_inputManager->isKeyPressed(SDLK_1)&&_guns.size()>0)
	{
		_currentGun = 0;
	}
	if (_inputManager->isKeyPressed(SDLK_2) && _guns.size()>1)
	{
		cout << "Equip GUN 2 " << endl;
		_currentGun = 1;
	}
	if (_inputManager->isKeyPressed(SDLK_3) && _guns.size()>2)
	{
		cout << "Equip GUN 3 " << endl;
		_currentGun = 2;
	}
	if (_inputManager->isKeyPressed(SDLK_4) && _guns.size()>3)
	{
		cout << "Equip GUN 4 " << endl;
		_currentGun = 3;
	}
	if (_inputManager->isKeyPressed(SDLK_5) && _guns.size()>4)
	{
		_currentGun = 4;
	}
	if (_currentGun > -1 && _currentGun < _guns.size() + 1)
	{
		glm::vec2 mouseCoords = _camera->convertScreenToWorldCoords(_inputManager->getMouseCoords());
		glm::vec2 centerPosition = _position + glm::vec2(AGENT_RADIUS);
		glm::vec2 direction = glm::normalize(mouseCoords - centerPosition);
		_guns[_currentGun]->update(_inputManager->isKeyPressed(SDL_BUTTON_LEFT), direction, *_bullets, centerPosition);
	}
	if(_inputManager)
	collideWithLevel(levelData);
}

void Player::addGun(Gun* gun)
{
	_guns.push_back(gun);

	//If no gun equipped, equip gun
	_currentGun = 0;
}