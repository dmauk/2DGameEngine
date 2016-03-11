#include "Projectile.h"
#include <GameEngine2D\ResourceManager.h>
#include <GameEngine2D\SpriteBatch.h>


Projectile::Projectile(glm::vec2 pos, glm::vec2 dir, float speed, int lifeTime)
{
	_position = pos;
	_direction = dir;
	_speed = speed;
	_lifeTime = lifeTime;
}


Projectile::~Projectile()
{
}

void Projectile::draw(GameEngine2D::SpriteBatch& spriteBatch)
{
	glm::vec4 pos(_position.x,_position.y,30.0f, 30.0f);
	glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	static GameEngine2D::GLTexture texture = GameEngine2D::ResourceManager::getTexture("Textures/PNG/CharacterRight_Standing.png");
	GameEngine2D::Color color;
	color.r = 255;
	color.g = 255;
	color.b = 255;
	color.a = 255;

	spriteBatch.draw(pos, uv, texture.id, 0.0f, color);
}

bool Projectile::update() //Return true when projectile needs to be destroyed.
{
	_position += _direction * _speed;
	_lifeTime--;
	if (_lifeTime == 0)
	{
		return true;
	}
	return false;
}