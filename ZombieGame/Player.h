#pragma once

#include "Human.h"
#include <GameEngine2D/InputManager.h>

class Player : public Human
{
public:
	Player();
	~Player();
	void init(float speed, glm::vec2 position, GameEngine2D::InputManager* inputManager);
	//void update(GameEngine2D::InputManager& inputManager);
	void update(const vector<string>& _levelData, vector<Human*>& _humans, vector<Zombie*>& _zombies);
private:
	GameEngine2D::InputManager * _inputManager;
};

