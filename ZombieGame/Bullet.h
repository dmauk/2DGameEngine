#pragma once
#include <GLM\glm.hpp>
#include <vector>
#include <GameEngine2D\SpriteBatch.h>

using namespace std;

class Human;
class Zombie;

const float BULLET_RADIUS = 5.0f;


class Bullet
{
public:
	Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed);
	~Bullet();
	void update(vector<Human*>& humans, vector<Zombie*>& zombies);
	void draw(GameEngine2D::SpriteBatch& spriteBatch); 
private:
	float _damage;
	glm::vec2 _position;
	glm::vec2 _direction;
	float _speed;
};

