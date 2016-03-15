#include "Bullet.h"
#include <GameEngine2D\ResourceManager.h>


Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed) : _position(position), _direction(direction), _damage(damage), _speed(speed)
{

}


Bullet::~Bullet()
{
}

void Bullet::update(vector<Human*>& humans, vector<Zombie*>& zombies)
{
	_position += _direction * _speed;
}

void Bullet::draw(GameEngine2D::SpriteBatch& spriteBatch)
{
	GameEngine2D::Color color;
	color.r = 0;
	color.g = 0;
	color.b = 0;
	color.a = 255;

	glm::vec4 destRect(_position.x + BULLET_RADIUS, _position.y + BULLET_RADIUS, BULLET_RADIUS * 2, BULLET_RADIUS * 2);
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	spriteBatch.draw(destRect, uvRect, GameEngine2D::ResourceManager::getTexture("Textures/circle.png").id, 0.0f, color);
}