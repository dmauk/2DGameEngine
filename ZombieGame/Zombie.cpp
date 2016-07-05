#include "Zombie.h"
#include "Human.h"
#include <GameEngine2D\ResourceManager.h>

Zombie::Zombie()
{
}


Zombie::~Zombie()
{
}

void Zombie::update(const vector<string>& levelData, vector<Human*>& humans, vector<Zombie*>& zombies, float deltaTime)
{
	Human* closestHuman = getNearestHuman(humans);

	if (closestHuman != nullptr)
	{
		m_direction = glm::normalize(closestHuman->getPosition() - m_position);
		m_position += m_direction * m_speed * deltaTime;
	}

	collideWithLevel(levelData);
}

void Zombie::init(float health, float speed, glm::vec2 position)
{
	m_speed = speed;
	m_position = position;
	m_color = GameEngine2D::ColorRGBA8(255, 255, 255, 255);
	m_health = health;
	m_textureID = GameEngine2D::ResourceManager::getTexture("Textures/zombie.png").id;
}

Human* Zombie::getNearestHuman(vector<Human*>& humans)
{
	Human* closestHuman = nullptr;
	float smallestDistance = 9999999.0f;
	for (int i = 0; i < humans.size(); i++)
	{
		glm::vec2 distVec = humans[i]->getPosition() - m_position;
		float distance = glm::length(distVec);

		if (distance < smallestDistance)
		{
			smallestDistance = distance;
			closestHuman = humans[i];
		}
	}



	return closestHuman;
}