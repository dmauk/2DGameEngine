#pragma once
#include <GLM\glm.hpp>
#include <GameEngine2D\SpriteBatch.h>

class Projectile
{
public:
	Projectile(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime);
	~Projectile();
	void draw(GameEngine2D::SpriteBatch& spriteBatch);  //pass a reference not a copy to sprite batch
	bool update();

private:
	float m_speed;
	glm::vec2 m_direction;
	glm::vec2 m_position;
	int m_lifeTime;

};

