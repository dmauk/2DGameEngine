#include "Bullet.h"
#include <GameEngine2D\ResourceManager.h>
#include "Level.h"
#include "Agent.h"
#include <algorithm>
#include <iostream>

using namespace std;

Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed, float duration) : _position(position), _direction(direction), _damage(damage), _speed(speed), _duration(duration)
{

}


Bullet::~Bullet()
{
}

bool Bullet::update(vector<Human*>& humans, vector<Zombie*>& zombies, const vector<string>& levelData)
{
	_duration-= 1.0f;
	if (_duration == 0)
	{
		return true; //Delete bullet
	}
	_position += _direction * _speed;

	return (collideWithLevel(levelData));
	
}

void Bullet::draw(GameEngine2D::SpriteBatch& spriteBatch)
{
	GameEngine2D::Color color;
	color.r = 0;
	color.g = 0;
	color.b = 0;
	color.a = 255;

	static GameEngine2D::GLTexture texture = GameEngine2D::ResourceManager::getTexture("Textures/circle.png");
	glm::vec4 destRect(_position.x + BULLET_RADIUS, _position.y + BULLET_RADIUS, BULLET_RADIUS * 2, BULLET_RADIUS * 2);
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	spriteBatch.draw(destRect, uvRect, texture.id, 0.0f, color);
}

void Bullet::collideWithTile(glm::vec2 tilePosition)
{
	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE = BULLET_RADIUS + TILE_RADIUS;
	glm::vec2 centerPlayerPosition = _position + glm::vec2(BULLET_RADIUS);
	glm::vec2 distVec = centerPlayerPosition - tilePosition;
	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	if (xDepth > 0 || yDepth > 0) //Collision
	{
		if (max(xDepth, 0.0f)  < max(yDepth, 0.0f))
		{
			if (distVec.x < 0)
			{
				_position.x -= xDepth;
			}
			else
			{
				_position.x += xDepth;
			}

		}
		else
		{
			if (distVec.y < 0)
			{
				_position.y -= yDepth;
			}
			else
			{
				_position.y += yDepth;
			}

		}
	}
}

bool Bullet::collideWithLevel(const vector<string>& levelData)
{
	glm::ivec2 gridPosition;
	gridPosition.x = floor(_position.x / TILE_WIDTH);
	gridPosition.y = floor(_position.y / TILE_WIDTH);
	return (levelData[gridPosition.y][gridPosition.x] != '.');
}

bool Bullet::collidewWithAgent(Agent* agent)
{
	//Check with circular collision
	const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;
	glm::vec2 centerPosA = _position + glm::vec2(BULLET_RADIUS);
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);
	glm::vec2 distVec = centerPosA - centerPosB;
	float distance = glm::length(distVec);
	float collisionDepth = MIN_DISTANCE - distance;
	if (collisionDepth > 0)
	{
		glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;
		_position += collisionDepthVec / 2.0f;
		agent->getPosition() -= collisionDepthVec / 2.0f;
		return true;
	}
	return false;
}