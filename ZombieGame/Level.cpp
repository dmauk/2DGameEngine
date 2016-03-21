#include "Level.h"
#include <fstream>
#include <iostream>
#include <GameEngine2D\ErrorHandling.h>
#include <GameEngine2D\ResourceManager.h>

using namespace std;

Level::Level(const string& filePath) : m_playerSet(false)//ResourceManager dependency injection
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
	file >> tmp >> m_numHumans;


	getline(file, tmp); //Throw away rest of line

	while (getline(file, tmp))
	{
		m_levelData.push_back(tmp);
	}

	//Testing
	for (int i = 0; i < m_levelData.size(); i++)
	{
		cout << m_levelData[i] << endl;
	}


	glm::vec4 uvRect(0.0f, 0.0f, 1.0f, 1.0f);
	GameEngine2D::ColorRGBA8 whiteColor; //Create class and constructor for this for simplicity;
	whiteColor.r = 255;
	whiteColor.g = 255;
	whiteColor.b = 255;
	whiteColor.a = 255;
	m_spriteBatch.init();
	m_spriteBatch.begin();
	for (int y = 0; y < m_levelData.size(); y++)
	{
		for (int x = 0; x < m_levelData[y].size(); x++)
		{
			if (y == 20)
			{
				//
			}
			//Grab the tile
			char tile = m_levelData[y][x];

			//Get dest rect
			glm::vec4 destRect(x*TILE_WIDTH, y*TILE_WIDTH, TILE_WIDTH, TILE_WIDTH);

			//Process the tile
			switch (tile) {
			case 'B':
				m_spriteBatch.draw(destRect, uvRect, GameEngine2D::ResourceManager::getTexture("Textures/red_bricks.png").id, 0.0f, whiteColor);
				break;
			case 'R':
				m_spriteBatch.draw(destRect, uvRect, GameEngine2D::ResourceManager::getTexture("Textures/red_bricks.png").id, 0.0f, whiteColor);
				break;
			case 'G':
				m_spriteBatch.draw(destRect, uvRect, GameEngine2D::ResourceManager::getTexture("Textures/glass.png").id, 0.0f, whiteColor);
				break;
			case 'L':
				m_spriteBatch.draw(destRect, uvRect, GameEngine2D::ResourceManager::getTexture("Textures/light_bricks.png").id, 0.0f, whiteColor);
				break;
			case '@':
				m_levelData[y][x] = '.';
				m_startPlayerPos.x = x * TILE_WIDTH;
				m_startPlayerPos.y = y * TILE_WIDTH;
				break;
			case 'Z':
				m_levelData[y][x] = '.';
				m_zombiePositions.emplace_back(x*TILE_WIDTH, y*TILE_WIDTH);
				break;
			case '.':

				break;
			default:
				cout << "UNEXPECTED SYMBOL" << tile << "@" << x << " " << y << endl;
			}
		}
	}


	m_spriteBatch.end();
}


Level::~Level()
{
}

void Level::draw()
{
	m_spriteBatch.renderBatch();
}
