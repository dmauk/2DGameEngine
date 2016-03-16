#include "Human.h"
#include <random>
#include <ctime>
#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>
#include <GLM/gtx/rotate_vector.hpp>


Human::Human()
{
}


Human::~Human()
{
}

void Human::init(float health, float speed, glm::vec2 position)
{
	static mt19937 randomEngine;
	static uniform_real_distribution<float> randDir(-1.0f, 1.0f);
	_speed = speed;
	_position = position;
	_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	if (_direction.length() == 0)
		_direction = glm::vec2(1.0f, 0.0f);

	_direction = glm::normalize(_direction);

	_color.r = 200;
	_color.g = 0;
	_color.b = 200;
	_color.a = 255;
	_health = health;
}

void Human::update(const vector<string>& levelData, vector<Human*>& humans, vector<Zombie*>& zombies)
{
	static mt19937 randomEngine;
	static uniform_real_distribution<float> randRotate(-5.0f, 5.0f);
	const float DEG_TO_RAD = M_PI / 180.0f;

	_position += _direction * _speed;
	_direction = glm::rotate(_direction, randRotate(randomEngine) * DEG_TO_RAD);

	if (collideWithLevel(levelData))
	{
		_direction = glm::rotate(_direction, 90.0f * DEG_TO_RAD);
	
	}
}

