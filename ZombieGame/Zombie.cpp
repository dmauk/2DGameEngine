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
		glm::vec2 direction = glm::normalize(closestHuman->getPosition() - _position);
		_position += direction * _speed;
	}

	collideWithLevel(levelData);
}

void Zombie::init(float speed, glm::vec2 position)
{
	_speed = speed;
	_position = position;
	_color.r = 0;
	_color.g = 160;
	_color.b = 0;
	_color.a = 255;
}

Human* Zombie::getNearestHuman(vector<Human*>& humans)
{
	Human* closestHuman = nullptr;
	float smallestDistance = 9999999.0f;
	for (int i = 0; i < humans.size(); i++)
	{
		glm::vec2 distVec = humans[i]->getPosition() - _position;
		float distance = glm::length(distVec);

		if (distance < smallestDistance)
		{
			smallestDistance = distance;
			closestHuman = humans[i];
		}
	}



	return closestHuman;
}