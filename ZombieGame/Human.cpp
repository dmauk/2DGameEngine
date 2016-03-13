#include "Human.h"
#include <random>
#include <ctime>


Human::Human()
{
}


Human::~Human()
{
}

void Human::init(float speed, glm::vec2 position)
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
}

void Human::update(const vector<string>& levelData, vector<Human*>& _humans, vector<Zombie*>& _zombies)
{
	

	_position += _direction * _speed;

	collideWithLevel(levelData);
}

