#pragma once
#include "Agent.h"
class Human : public Agent
{
public:
	Human();
	virtual ~Human();
	virtual void update(const vector<string>& levelData, vector<Human*>& _humans, vector<Zombie*>& _zombies);
	void init(float speed, glm::vec2 position);
private:
	glm::vec2 _direction;

};

