#include "Agent.h"
#include <GameEngine2D/ResourceManager.h> //Change from static class
#include <GameEngine2D\GLTexture.h>
#include "Level.h"
#include <iostream>
#include <algorithm>


Agent::Agent()
{
}


Agent::~Agent()
{
}

bool Agent::collideWithLevel(const vector<string>& levelData)
{
	vector<glm::vec2> collideTilePositions;

	//Check the four corners
	//First corner
	checkTilePosition(levelData, collideTilePositions, m_position.x, m_position.y);
	//Second corner
	checkTilePosition(levelData, collideTilePositions, m_position.x+AGENT_WIDTH, m_position.y);
	//Third corner
	checkTilePosition(levelData, collideTilePositions, m_position.x, m_position.y + AGENT_WIDTH);
	//Fourth corner
	checkTilePosition(levelData, collideTilePositions, m_position.x + AGENT_WIDTH, m_position.y + AGENT_WIDTH);

	for (int i = 0; i < collideTilePositions.size(); i++)
	{
		collideWithTile(collideTilePositions[i]);
	}

	if (collideTilePositions.size() > 0)
	{
		return true;
	}
	return false;
}

void Agent::checkTilePosition(const vector<string>& levelData, vector<glm::vec2>& collideTilePositions, float x, float y)
{
	glm::vec2 cornerPos = glm::vec2(floor(x / (float)TILE_WIDTH), floor(y / (float)TILE_WIDTH)); //Floor floating point


	if (cornerPos.x < 0 || cornerPos.x >= levelData[0].length() || cornerPos.y < 0 || cornerPos.y >= levelData.size())
	{
		//if outside world just return
		return;
	}

	if (levelData[cornerPos.y][cornerPos.x] != '.')
	{
		collideTilePositions.push_back(cornerPos * (float)TILE_WIDTH + glm::vec2(TILE_WIDTH/2.0f)); //store center position of tile
	}
}


//AABB collision

//Need to sort tiles to prevent agents from getting stuck when moving down and to the left in a while
void Agent::collideWithTile(glm::vec2 tilePosition)
{
	const float TILE_RADIUS = (float)TILE_WIDTH / 2.0f;
	const float MIN_DISTANCE = AGENT_RADIUS + TILE_RADIUS;
	glm::vec2 centerPlayerPosition = m_position + glm::vec2(AGENT_RADIUS);
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

void Agent::draw(GameEngine2D::SpriteBatch& spriteBatch)
{
	glm::vec4 destRect(m_position.x, m_position.y, AGENT_WIDTH, AGENT_WIDTH);
	const glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	spriteBatch.draw(destRect, uvRect, m_textureID, 0.0f, m_color, m_direction);
}

bool Agent::collidewWithAgent(Agent* agent)
{
	//Check with circular collision
	const float MIN_DISTANCE = AGENT_RADIUS * 2;
	glm::vec2 centerPosA = m_position + glm::vec2(AGENT_RADIUS);
	glm::vec2 centerPosB = agent->getPosition() + glm::vec2(AGENT_RADIUS);
	glm::vec2 distVec = centerPosA - centerPosB; 
	float distance = glm::length(distVec);
	float collisionDepth = MIN_DISTANCE - distance;
	if (collisionDepth > 0)
	{
		glm::vec2 collisionDepthVec = glm::normalize(distVec) * collisionDepth;
		m_position += collisionDepthVec / 2.0f;
		agent->m_position -= collisionDepthVec / 2.0f;
		return true;
	}
	return false;
}

bool Agent::applyDamage(int damage)
{
	m_health -= damage;
	if (m_health <= 0)
	{
		return true;
	}

	return false;
}