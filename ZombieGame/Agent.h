#pragma once
#include<GLM\glm.hpp>
#include<GameEngine2D\SpriteBatch.h>

const float AGENT_WIDTH = 60;
const float AGENT_RADIUS = AGENT_WIDTH / 2.0f;


class Zombie;
class Human;



class Agent
{
public:
	Agent();
	virtual ~Agent();
	virtual void update(const vector<string>& levelData, vector<Human*>& humans, vector<Zombie*>& zombies) = 0;
	bool collideWithLevel(const vector<string>& levelData);
	bool collidewWithAgent(Agent* agent); //maybe store a vector of agents instead?
	void draw(GameEngine2D::SpriteBatch& _spriteBatch);
	bool applyDamage(int damage);
	glm::vec2 getPosition() const { return m_position; }
protected:
	void checkTilePosition(const vector<string>& levelData, vector<glm::vec2>& collideTilePositions, float x, float y);
	void collideWithTile(glm::vec2 tilePosition);
	glm::vec2 m_position;
	float m_speed;
	float m_health;
	glm::vec2 m_direction;
	GameEngine2D::ColorRGBA8 m_color;
	//GLTexture _texture; ///> Not used because agent will use the same texture so there's no point in making copies. 
};

