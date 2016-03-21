#pragma once
#include <string>
#include <vector>
#include <GameEngine2D/SpriteBatch.h>

using namespace std;

const int TILE_WIDTH = 64;

class Level
{
public:
	//Load the level
	Level(const string& filePath); //Alternatively, a level init function could load all of the information
	~Level();
	void draw();
	int getWidth() { return m_levelData[0].length(); } //Assuming level is a rectangle
	int getHeight() { return m_levelData.size(); }
	const vector<string>& getLevelData() const { return m_levelData; }
	glm::vec2 getStartPlayerPos() const { return m_startPlayerPos; }
	const vector<glm::vec2>& getStartZombiePoistions() const { return m_zombiePositions;  }
	int getNumHumans() const { return m_numHumans; }
private:
	bool m_playerSet;
	vector<string> m_levelData;
	int m_numHumans;
	GameEngine2D::SpriteBatch m_spriteBatch;
	//GameEngine2D::ResourceManager * _resourceManager ///<This is a pointer to manager in main game. Is a static class CHANGE THIS
	glm::vec2 m_startPlayerPos;
	vector<glm::vec2> m_zombiePositions;

};

