#pragma once
#include "Agent.h" //Forward declares human
class Zombie : public Agent
{
public:
	Zombie();
	~Zombie();
	void init(float health, float speed, glm::vec2 position);
	void update(const vector<string>& levelData, vector<Human*>& _humans, vector<Zombie*>& _zombies);
private:
	Human* getNearestHuman(vector<Human*>& humans);
};

