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
	m_speed = speed;
	m_position = position;
	m_direction = glm::vec2(randDir(randomEngine), randDir(randomEngine));
	if (m_direction.length() == 0)
		m_direction = glm::vec2(1.0f, 0.0f);

	m_direction = glm::normalize(m_direction);
	m_color = GameEngine2D::ColorRGBA8(200, 0, 200, 255);
	m_health = health;
}

void Human::update(const vector<string>& levelData, vector<Human*>& humans, vector<Zombie*>& zombies)
{
	static mt19937 randomEngine;
	static uniform_real_distribution<float> randRotate(-5.0f, 5.0f);
	const float DEG_TO_RAD = M_PI / 180.0f;

	m_position += m_direction * m_speed;
	m_direction = glm::rotate(m_direction, randRotate(randomEngine) * DEG_TO_RAD);

	if (collideWithLevel(levelData))
	{
		m_direction = glm::rotate(m_direction, 90.0f * DEG_TO_RAD);
	
	}
}

