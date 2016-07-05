#include "SpatialGrid.h"
#include <iostream>
using namespace std;


SpatialGrid::SpatialGrid(int width, int height, int cellSize) : m_width(width), m_height(height), m_cellSize(cellSize)
{
	m_numXCells = ceil(float(m_width / m_cellSize));
	m_numYCells = ceil(float(m_height / m_cellSize));

	//Allocate all of the cells
	const int BALLS_TO_RESERVE = 20;
	m_cells.resize(m_numYCells * m_numXCells);
	for (int i = 0; i < m_cells.size(); i++)
	{
		m_cells[i].balls.reserve(BALLS_TO_RESERVE);
	}

}


SpatialGrid::~SpatialGrid()
{
}

Cell* SpatialGrid::getCell(int x, int y) //Works when cells start at 0 in both direction. I.E. Cell 1 would be [0,0]
{
	//Make sure we are within grid dimensions
	if (x < 0) x = 0;
	if (x >= m_numXCells) x = m_numXCells - 1;
	if (y < 0) y = 0;
	if (y >= m_numYCells) y = m_numYCells - 1;

	return &m_cells[y * m_numXCells + x]; //y * m_numXCells + x
}

void SpatialGrid::addBall(Ball* ball)
{
	Cell* cell = getCell(ball->position);
	cell->balls.push_back(ball);
	ball->ownerCell = cell;
	ball->cellVectorIndex = cell->balls.size() - 1;
}

void SpatialGrid::addBall(Ball* ball, Cell* cell)
{
	cell->balls.push_back(ball);
	ball->ownerCell = cell;
	ball->cellVectorIndex = cell->balls.size() - 1;
}

Cell* SpatialGrid::getCell(const glm::vec2 position) //Works when cells start at 0 in both direction. I.E. Cell 1 would be [0,0]
{
	int cellX = (int)(position.x / m_cellSize);
	int cellY = (int)(position.y / m_cellSize);

	return getCell(cellX, cellY);
}

void SpatialGrid::removeBallFromCell(Ball* ball)
{
	std::vector<Ball*>& balls = ball->ownerCell->balls;
	//Ya generic vector swap
	balls[ball->cellVectorIndex] = balls.back();
	balls.pop_back();
	//Update vector index
	if (ball->cellVectorIndex < balls.size())
	{
		balls[ball->cellVectorIndex]->cellVectorIndex = ball->cellVectorIndex;
	}
	ball->cellVectorIndex = -1;
	ball->ownerCell = nullptr;
}

