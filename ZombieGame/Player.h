#pragma once

#include "Human.h"
#include <GameEngine2D/InputManager.h>
#include <GameEngine2D/Camera2D.h>
#include "Bullet.h"
class Gun;

class Player : public Human
{
public:
	Player();
	~Player();
	void addGun(Gun* gun);
	void init(float health, float speed, glm::vec2 position, GameEngine2D::InputManager* inputManager, GameEngine2D::Camera2D * camera, vector<Bullet>* bullets);
	//void update(GameEngine2D::InputManager& inputManager);
	void update(const vector<string>& _levelData, vector<Human*>& _humans, vector<Zombie*>& _zombies, float deltaTime) override; ///> LOOK UP C++ 11 OVERRIDE
private:
	GameEngine2D::InputManager * m_inputManager;
	vector<Gun*> m_guns;
	int m_currentGun;
	GameEngine2D::Camera2D* m_camera;
	vector<Bullet>* m_bullets;
};

