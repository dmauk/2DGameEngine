#include "Level.h"
#include <fstream>
#include <iostream>
#include <GameEngine2D\ErrorHandling.h>
#include <GameEngine2D\ResourceManager.h>

using namespace std;

Level::Level(const string& filePath) : _playerSet(false)//ResourceManager dependency injection
{

	ifstream file;
	file.open(filePath);

	//Error Checking
	if (file.fail())
	{
		GameEngine2D::fatalError("Failed to open file: " + filePath);
	}

	std::string tmp;
	//Throw away first string in temp
	file >> tmp >> _numHumans;


	getline(file, tmp); //Throw away rest of line

	while (getline(file, tmp))
	{
		_levelData.push_back(tmp);
	}

	//Testing
	for (int i = 0; i < _levelData.size(); i++)
	{
		cout << _levelData[i] << endl;
	}


	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	GameEngine2D::Color whiteColor; //Create class and constructor for this for simplicity;
	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;
	_spriteBatch.init();
	_spriteBatch.begin();
	for (int y = 0; y < _levelData.size(); y++)
	{
		for (int x = 0; x < _levelData[y].size(); x++)
		{
			if (y == 20)
			{
				//
			}
			//Grab the tile
			char tile = _levelData[y][x];

			//Get dest rect
			glm::vec4 destRect(x*TILE_WIDTH, y*TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			//Process the tile
			switch (tile) {
			case 'B':
				_spriteBatch.draw(destRect, uvRect, GameEngine2D::ResourceManager::getTexture("Textures/red_bricks.png").id, 0.0f, whiteColor);
				break;
			case 'R':
				_spriteBatch.draw(destRect, uvRect, GameEngine2D::ResourceManager::getTexture("Textures/red_bricks.png").id, 0.0f, whiteColor);
				break;
			case 'G':
				_spriteBatch.draw(destRect, uvRect, GameEngine2D::ResourceManager::getTexture("Textures/glass.png").id, 0.0f, whiteColor);
				break;
			case 'L':
				_spriteBatch.draw(destRect, uvRect, GameEngine2D::ResourceManager::getTexture("Textures/light_bricks.png").id, 0.0f, whiteColor);
				break;
			case '@':
				_levelData[y][x] = '.';
				_startPlayerPos.x = x * TILE_WIDTH;
				_startPlayerPos.y = y * TILE_WIDTH;
				break;
			case 'Z':
				_levelData[y][x] = '.';
				_zombiePositions.emplace_back(x*TILE_WIDTH, y*TILE_WIDTH);
				break;
			case '.':

				break;
			default:
				cout << "UNEXPECTED SYMBOL" << tile << "@" << x << " " << y << endl;
			}
		}
	}


	_spriteBatch.end();
}


Level::~Level()
{
}

void Level::draw()
{
	_spriteBatch.renderBatch();
}
