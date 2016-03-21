#include "Zombie.h"
#include "Human.h"


Zombie::Zombie()
{
}


Zombie::~Zombie()
{
}

void Zombie::update(const vector<string>& levelData, vector<Human*>& humans, vector<Zombie*>& zombies)
{
	Human* closestHuman = getNearestHuman(humans);

	if (closestHuman != nullptr)
	{
		glm::vec2 direction = glm::normalize(closestHuman->getPosition() - m_position);
		m_position += direction * m_speed;
	}

	collideWithLevel(levelData);
}

void Zombie::init(float health, float speed, glm::vec2 position)
{
	m_speed = speed;
	m_position = position;
	m_color = GameEngine2D::ColorRGBA8(0, 160, 0, 255);
	m_health = health;
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