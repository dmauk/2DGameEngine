#pragma once
#include "Agent.h"
class Human : public Agent
{
public:
	Human();
	virtual ~Human();
	virtual void update(const vector<string>& levelData, vector<Human*>& _humans, vector<Zombie*>& _zombies);
	void init(float health, float speed, glm::vec2 position);
private:
	glm::vec2 m_direction;

};

