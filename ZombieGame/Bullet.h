#pragma once
#include <GLM\glm.hpp>
#include <vector>
#include <GameEngine2D\SpriteBatch.h>

using namespace std;

class Human;
class Zombie;
class Agent;

const float BULLET_RADIUS = 5.0f;


class Bullet
{
public:
	Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed, float duration);
	~Bullet();
	bool update(vector<Human*>& humans, vector<Zombie*>& zombies, const vector<string>& levelData, float deltaTime);
	glm::vec2 getPosition() { return m_position;  }
	void draw(GameEngine2D::SpriteBatch& spriteBatch); 
	float getDamage() const { return m_damage; };
	bool collidewWithAgent(Agent* agent);

private:
	bool collideWithLevel(const vector<string>& levelData);
	void collideWithTile(glm::vec2 tilePosition);
	float m_damage;
	glm::vec2 m_position;
	glm::vec2 m_direction;
	float m_speed;
	float m_duration; ///< Bullet life time in frames
};

