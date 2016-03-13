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
	glm::vec2 getPosition() const { return _position; }
protected:
	void checkTilePosition(const vector<string>& levelData, vector<glm::vec2>& collideTilePositions, float x, float y);
	void collideWithTile(glm::vec2 tilePosition);
	glm::vec2 _position;
	float _speed;
	glm::vec2 _direction;
	GameEngine2D::Color _color;
	//GLTexture _texture; ///> Not used because agent will use the same texture so there's no point in making copies. 
};

