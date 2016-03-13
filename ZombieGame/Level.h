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
	int getWidth() { return _levelData[0].length(); } //Assuming level is a rectangle
	int getHeight() { return _levelData.size(); }
	const vector<string>& getLevelData() const { return _levelData; }
	glm::vec2 getStartPlayerPos() const { return _startPlayerPos; }
	const vector<glm::vec2>& getStartZombiePoistions() const { return _zombiePositions;  }
	int getNumHumans() const { return _numHumans; }
private:
	bool _playerSet;
	vector<string> _levelData;
	int _numHumans;
	GameEngine2D::SpriteBatch _spriteBatch;
	//GameEngine2D::ResourceManager * _resourceManager ///<This is a pointer to manager in main game. Is a static class CHANGE THIS
	glm::vec2 _startPlayerPos;
	vector<glm::vec2> _zombiePositions;

};

