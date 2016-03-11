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
	float _speed;
	glm::vec2 _direction;
	glm::vec2 _position;
	int _lifeTime;

};

