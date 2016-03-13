#pragma once
#include "Agent.h"
class Zombie : public Agent
{
public:
	Zombie();
	~Zombie();
	void update(const vector<string>& _levelData, vector<Human*>& _humans, vector<Zombie*>& _zombies);
};

