#include "Player.h"
#include <SDL\SDL.h>


Player::Player()
{
}


Player::~Player()
{
}

void Player::init(float speed, glm::vec2 position, GameEngine2D::InputManager* inputManager) //Dependency injection
{
	_speed = speed;
	_position = position;
	_inputManager = inputManager;
	_color.r = 0;
	_color.g = 0;
	_color.b = 100;
	_color.a = 255;
}

void Player::update(const vector<string>& levelData, vector<Human*>& _humans, vector<Zombie*>& _zombies)
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
	collideWithLevel(levelData);
}
