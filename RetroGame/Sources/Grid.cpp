/////////////////////////////
// Author: Liam Bansal
// Date Created: 04/03/2020
/////////////////////////////

// Includes
#include "Grid.h" // File's header.
#include "Cell.h"
#include "Clamp.h"
#include "olcPixelGameEngine.h"
#include "RandGenerator.h"
#include "GameplayState.h"
#include "Mushroom.h"
#include "Score.h"
#include "Vector2D.h"

// Namespaces.
using namespace olc;

Grid::Grid() : mc_uiCellWidth(8),
	mc_uiCellHeight(8),
	mc_uiWidthInCells(32),
	mc_uiHeightInCells(28),
	mc_uiPixelWidth(mc_uiWidthInCells* mc_uiCellWidth),
	mc_uiPixelHeight(mc_uiHeightInCells* mc_uiCellWidth),
	mc_uiXOrigin(4),
	mc_uiYOrigin(20),
	mc_uiBlasterZoneHeight(10),
	mc_fMushroomSpawnChance(4.3f),
	m_bDrawn(false)
{
	if (m_cells) // Null check pointer.
	{
		for (unsigned int row = 0; row < mc_uiWidthInCells; row++)
		{
			for (unsigned int column = 0; column < mc_uiHeightInCells; column++)
			{
				// Add 2 because we want that distance between the cells and 
				// the top of the screen.
				m_cells[row][column]->SetPosition((float)row *
					m_cells[row][column]->GetHeight() +
					mc_uiXOrigin, (float)(column) *
					m_cells[row][column]->GetWidth() +
					mc_uiYOrigin);
			}
		}
	}

	m_noMovementZone = Vector2D((float)mc_uiWidthInCells * mc_uiCellWidth,
		(float)(mc_uiHeightInCells - mc_uiBlasterZoneHeight) * mc_uiCellHeight);
}

Grid::~Grid()
{}

// Updates all of the grid's cells if they have a mushroom.
void Grid::Update(GameplayState* a_pScene, BugBlaster& a_rBugBlaster)
{
	if (m_cells)
	{
		for (unsigned int x = 0; x < mc_uiWidthInCells; ++x)
		{
			for (unsigned int y = 0; y < mc_uiHeightInCells; ++y)
			{
				if (m_cells[x][y]->GetTag() == "Mushroom")
				{
					m_cells[x][y]->Update(a_pScene, this, a_rBugBlaster);
				}
			}
		}
	}
}

// Draws the grid's cells onto the screen.
void Grid::Draw(GameplayState* pScene)
{
	if (m_cells) // Null check pointer.
	{
		for (unsigned int x = 0; x < mc_uiWidthInCells; ++x)
		{
			for (unsigned int y = 0; y < mc_uiHeightInCells; ++y)
			{
				m_cells[x][y]->Draw(pScene, this);
			}
		}
		
		m_bDrawn = true;
	}
}

// Spawns mushrooms across the grid.
void Grid::SpawnMushrooms() const
{
	if (m_cells) // Null check pointer.
	{
		RandGenerator randGenerator;

		// Two loops for checking every grid cell where mushrooms can spawn.
		for (unsigned int x = 0; x < mc_uiWidthInCells; ++x)
		{
			for (unsigned int y = 0; y < mc_uiHeightInCells; ++y)
			{
				const float maxReturnValue = 100.0f;

				// Gets a random float between 0 and 100 to check if a 
				// mushroom should spawn.
				if (randGenerator.GetRandomFloat(maxReturnValue) <
					mc_fMushroomSpawnChance)
				{
					// Gets a set of random coordinates to spawn a mushroom.
					unsigned int xPosition =
						randGenerator.GetRandomInteger(mc_uiWidthInCells);
					unsigned int yPosition =
						randGenerator.GetRandomInteger(mc_uiHeightInCells);
					m_cells[xPosition][yPosition]->SpawnMushroom();
				}
			}
		}
	}
}

// Counts the number of mushrooms occupying cells and gives the player points
// for pairs.
void Grid::CountMushrooms(GameplayState* a_pState) const
{
	if (m_cells && a_pState && a_pState->GetScore())
	{
		unsigned int mushroomCount = 0;
		const unsigned int pairValue = 5;

		// Loops through cells containing a mushroom and add it to the tally.
		for (unsigned int x = 0; x < mc_uiWidthInCells; ++x)
		{
			for (unsigned int y = 0; y < mc_uiHeightInCells; ++y)
			{
				if (m_cells[x][y]->GetTag() == "Mushroom" &&
					m_cells[x][y]->GetMushroom() &&
					!m_cells[x][y]->GetMushroom()->GetCountState())
				{
					++mushroomCount;
					m_cells[x][y]->GetMushroom()->SetCountState(true);

					// If our tally is an even number then we have a pair of 
					// mushrooms; increase the score.
					if (mushroomCount % 2 == 0)
					{
						a_pState->GetScore()->Update(pairValue);
					}
				}
			}
		}
	}
}

const bool Grid::IsDrawn() const
{
	return m_bDrawn;
}

const unsigned int Grid::GetCellWidth() const
{
	return mc_uiCellWidth;
}

const unsigned int Grid::GetCellHeight() const
{
	return mc_uiCellHeight;
}

// Returns the grid's width in terms of cell count.
const unsigned int Grid::GetWidth() const
{ 
	return mc_uiWidthInCells; 
}

// Returns the grid's height in terms of cell count.
const unsigned int Grid::GetHeight() const
{ 
	return mc_uiHeightInCells; 
}

const unsigned int Grid::GetPixelWidth() const
{
	return mc_uiPixelWidth;
}

const unsigned int Grid::GetPixelHeight() const
{
	return mc_uiPixelHeight;
}

const unsigned int Grid::GetXOrigin() const
{
	return mc_uiXOrigin;
}
const unsigned int Grid::GetYOrigin() const
{
	return mc_uiYOrigin;
}

// The y position of the grid's top 'wall'.
const unsigned int Grid::GetUpperBoundary(unsigned int a_margin) const
{
	unsigned int UIHeight = (Grid::GetYOrigin() / mc_uiCellHeight) * 
		mc_uiCellHeight;
	unsigned int yPosition = UIHeight + a_margin;
	return yPosition;
}

// The y position of the grid's bottom 'wall'.
const unsigned int Grid::GetLowerBoundary(unsigned int a_margin) const
{
	unsigned int UIHeight = (Grid::GetYOrigin() / mc_uiCellHeight) *
		mc_uiCellHeight;
	unsigned int yPosition = Grid::GetHeight() *
		mc_uiCellHeight + UIHeight - a_margin;
	return yPosition;
}

// The x position of the grid's left 'wall'.
const unsigned int Grid::GetLeftBoundary(unsigned int a_margin) const
{
	return a_margin;
}

// The x position of the grid's right 'wall'.
const unsigned int Grid::GetRightBoundary(unsigned int a_margin) const
{
	return mc_uiPixelWidth - a_margin;
}

// Returns the dimensions of the zone in which the bug blaster cannot move
// over.
const Vector2D* Grid::GetNoMovementZone() const
{
	return &m_noMovementZone;
}

// Returns the height of the area in which the bug blaster is allowed to move.
const unsigned int Grid::GetBlasterZoneHeight() const
{
	return mc_uiBlasterZoneHeight * mc_uiCellHeight;
}

// Returns a reference to a cell from grid's cell array. X and y should be 
// measured in cells.
Cell& Grid::GetCell(unsigned int a_x, unsigned int a_y) const
{
	if (m_cells)
	{
		Clamp clamp;
		// Deduct one because we're accessing an array whose max index is one less
		// than the grid width in cells.
		unsigned int cellsToDeduct = 1;
		return *m_cells[clamp.ClampInt(a_x, 0, mc_uiWidthInCells - cellsToDeduct)]
			[clamp.ClampInt(a_y - (mc_uiYOrigin / mc_uiCellHeight),
				0,
				mc_uiHeightInCells - cellsToDeduct)];
	}
}

// Returns a reference to a cell from grid's cell array. The position should 
// not be measured in cells.
Cell& Grid::GetCell(float a_x, float a_y) const
{
	if (m_cells)
	{
		Clamp clamp;
		// Deduct one because we're accessing an array whose max index is one less
		// than the grid width in cells.
		unsigned int cellsToDeduct = 1;
		return *m_cells[clamp.ClampInt((int)a_x / mc_uiCellWidth,
			0,
			mc_uiWidthInCells - cellsToDeduct)]
			[clamp.ClampInt((int)a_y / mc_uiCellHeight - (mc_uiYOrigin / mc_uiCellHeight),
				0,
				mc_uiHeightInCells - cellsToDeduct)];
	}
}

// Converts a 2D vector into a cell position on the grid and returns a reference
// to that cell.
Cell& Grid::GetCell(Vector2D* a_pPosition) const
{
	if (m_cells && a_pPosition)
	{
		Clamp clamp;
		// Deduct one because we're accessing an array whose max index is one less
		// than the grid width in cells.
		unsigned int cellsToDeduct = 1;
		return *m_cells[clamp.ClampInt((unsigned int)a_pPosition->GetX() / mc_uiCellWidth,
			0,
			mc_uiWidthInCells - cellsToDeduct)]
			[clamp.ClampInt((unsigned int)a_pPosition->GetY() / mc_uiCellHeight -
				(mc_uiYOrigin / mc_uiCellHeight),
				0,
				mc_uiHeightInCells - cellsToDeduct)];
	}
}