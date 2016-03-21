#include "Bullet.h"
#include <GameEngine2D\ResourceManager.h>
#include "Level.h"
#include "Agent.h"
#include <algorithm>
#include <iostream>

using namespace std;

Bullet::Bullet(glm::vec2 position, glm::vec2 direction, float damage, float speed, float duration) : m_position(position), m_direction(direction), m_damage(damage), m_speed(speed), m_duration(duration)
{

}


Bullet::~Bullet()
{
}

bool Bullet::update(vector<Human*>& humans, vector<Zombie*>& zombies, const vector<string>& levelData)
{
	m_duration-= 1.0f;
	if (m_duration == 0)
	{
		return true; //Delete bullet
	}
	m_position += m_direction * m_speed;

	return (collideWithLevel(levelData));
	
}

void Bullet::draw(GameEngine2D::SpriteBatch& spriteBatch)
{
	GameEngine2D::ColorRGBA8 color = GameEngine2D::ColorRGBA8(0,0,0,255);

	static GameEngine2D::GLTexture texture = GameEngine2D::ResourceManager::getTexture("Textures/circle.png");
	glm::vec4 destRect(m_position.x + BULLET_RADIUS, m_position.y + BULLET_RADIUS, BULLET_RADIUS * 2, BULLET_RADIUS * 2);
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	spriteBatch.draw(destRect, uvRect, texture.id, 0.0f, color);
}

void Bullet::collideWithTile(glm::vec2 tilePosition)
{
	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE = BULLET_RADIUS + TILE_RADIUS;
	glm::vec2 centerPlayerPosition = m_position + glm::vec2(BULLET_RADIUS);
	glm::vec2 distVec = centerPlayerPosition - tilePosition;
	float xDepth = MIN_DISTANCE - abs(distVec.x);
	float yDepth = MIN_DISTANCE - abs(distVec.y);

	if (xDepth > 0 || yDepth > 0) //Collision
	{
		if (max(xDepth, 0.0f)  < max(yDepth, 0.0f))
		{
			if (distVec.x < 0)
			{
				m_position.x -= xDepth;
			}
			else
			{
				m_position.x += xDepth;
			}

		}
		else
		{
			if (distVec.y < 0)
			{
				m_position.y -= yDepth;
			}
			else
			{
				m_position.y += yDepth;
			}

		}
	}
}

bool Bullet::collideWithLevel(const vector<string>& levelData)
{
	glm::ivec2 gridPosition;
	gridPosition.x = floor(m_position.x / TILE_WIDTH);
	gridPosition.y = floor(m_position.y / TILE_WIDTH);
	return (levelData[gridPosition.y][gridPosition.x] != '.');
}

bool Bullet::collidewWithAgent(Agent* agent)
{
	//Check with circular collision
	const float MIN_DISTANCE = AGENT_RADIUS + BULLET_RADIUS;
	glm::vec2 centerPosA = m_position + glm::vec2(BULLET_RADIUS);
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);
	glm::vec2 distVec = centerPosA - centerPosB;
	float distance = glm::length(distVec);
	float collisionDepth = MIN_DISTANCE - distance;
	if (collisionDepth > 0)
	{
		glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;
		m_position += collisionDepthVec / 2.0f;
		agent->getPosition() -= collisionDepthVec / 2.0f;
		return true;
	}
	return false;
}