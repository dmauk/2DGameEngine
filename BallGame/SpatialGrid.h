#pragma once

#include "Ball.h"
#include <vector>

using namespace std;

struct Cell
{
	vector<Ball*> balls;
};

class SpatialGrid
{
	friend class BallController;
public:
	SpatialGrid(int width, int height, int cellSize);
	~SpatialGrid();

	void addBall(Ball* ball);

	//add ball and determine the cell it belongs to
	void addBall(Ball* ball, Cell* cell);

	//Get cell based on cell coord
	Cell* getCell(int x, int y);

	//Get cell based on window coords
	Cell* getCell(const glm::vec2 position);

	void removeBallFromCell(Ball* ball);

private:
	//Using a 1 dimensional vector to implement a 2D grid for cache friendliness.
	vector<Cell> m_cells;
	int m_cellSize;
	int m_width;
	int m_height;
	int m_numXCells;
	int m_numYCells;

};

