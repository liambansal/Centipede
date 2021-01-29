/////////////////////////////
// Author: Liam Bansal
// Date Created: 04/03/2020
/////////////////////////////

// Incldues.
#include "Cell.h" // File's header.
#include "BugBlaster.h"
#include "Centipede.h"
#include "CentipedeHead.h"
#include "Grid.h"
#include "GameplayState.h"
#include "GameStateManager.h"
#include "Mushroom.h"

// Namespaces.
using namespace olc;

Cell::Cell() : mc_uiWidth(8),
	mc_uiHeight(8),
	m_sTag("Empty Cell"),
	m_bOccupied(false),
	m_position(),
	m_pMushroom(nullptr),
	m_pCentipedeHead(nullptr),
	m_pCentipede(nullptr),
	m_pSpider(nullptr),
	m_pFlea(nullptr)
{}

Cell::~Cell()
{
	if (m_pMushroom)
	{
		delete m_pMushroom;
		m_pMushroom = nullptr;
	}
}

// Updates the mushroom occupying the cell.
void Cell::Update(GameplayState* a_pState,
	Grid* a_pGrid,
	BugBlaster& a_rBugBlaster)
{
	if (m_bOccupied && m_pMushroom)
	{
		m_pMushroom->Update(a_pState, a_pGrid, *this, a_rBugBlaster);
	}
}

// Draws the cell's mushroom if present otherwise fills the cell with black
// pixels.
void Cell::Draw(GameplayState* a_pState, Grid* grid)
{
	if (a_pState && a_pState->GetManager())
	{
		for (unsigned int x = 0; x < mc_uiWidth; ++x)
		{
			for (unsigned int y = 0; y < mc_uiHeight; ++y)
			{
				if (m_bOccupied && m_pMushroom && m_pMushroom->GetSprite())
				{
					// If there's no pixel at (x, y) in the sprite then we need
					// to draw a black pixel onto the screen to simulate empty
					// space.
					if (m_pMushroom->GetSprite()->GetPixel(x, y) == olc::BLANK)
					{
						// Draws a black pixel at (x, y) from the cell's
						// position starting at the top left corner of the cell.
						a_pState->GetManager()->
							olc::PixelGameEngine::Draw((int32_t)((((int)m_position.GetX()) - mc_uiWidth * 0.5f) + x),
								(int32_t)((((int)m_position.GetY()) - mc_uiHeight * 0.5f) + y),
								olc::BLACK);
					}
					// If there's a pixel in the sprite and it doesn't match 
					// the pixel on the grid at the cell's position then we need to redraw the sprite in that cell.
					else if (GetPosition() &&
						m_pMushroom->GetSprite()->GetPixel(x, y) != GetPixel(nullptr,
						((int)GetPosition()->GetX() - mc_uiWidth * 0.5f) + x,
						((int)GetPosition()->GetY() - mc_uiHeight * 0.5f) + y))
					{
						// Draws the mushroom's sprite at the mushroom's position starting from the top left corner.
						a_pState->GetManager()->olc::PixelGameEngine::Draw((int32_t)((((int)m_pMushroom->GetPosition().GetX()) - mc_uiWidth * 0.5f) + x),
							(int32_t)((((int)m_pMushroom->GetPosition().GetY()) - mc_uiHeight * 0.5f) + y),
							m_pMushroom->GetSprite()->GetPixel(x, y));
					}
				}
				else
				{
					// Draws a black pixel at (x, y) from the cell's position starting at the top left corner of the cell.
					a_pState->GetManager()->olc::PixelGameEngine::Draw((int32_t)((((int)m_position.GetX()) - mc_uiWidth * 0.5f) + x),
						(int32_t)((((int)m_position.GetY()) - mc_uiHeight * 0.5f) + y), olc::BLACK);
				}
			}
		}
	}
}

void Cell::SpawnMushroom()
{
	if (!m_bOccupied && !m_pMushroom)
	{
		m_bOccupied = true;
		m_sTag = "Mushroom";
		m_pMushroom = new Mushroom();
		m_pMushroom->SetPosition(&m_position);
	}
}

void Cell::ClearMushroom()
{
	if (m_bOccupied && m_pMushroom)
	{
		m_bOccupied = false;
		m_sTag = "Empty Cell";
		delete m_pMushroom;
		m_pMushroom = nullptr;
	}
}

// Clears the cell's pointer to a centipede.
void Cell::ClearCentipede()
{
	m_pCentipedeHead = nullptr;
	m_pCentipede = nullptr;
	ClearTag();
}

// Clears the cell's pointer to a spider.
void Cell::ClearSpider()
{
	m_pSpider = nullptr;
	ClearTag();
}

// Clears the cell's pointer to a flea.
void Cell::ClearFlea()
{
	m_pFlea = nullptr;
	ClearTag();
}

// Tags the cell as "Mushroom" if one is present otherwise clears the tag for
// what was occupying the cell.
void Cell::ClearTag()
{
	if (m_bOccupied && m_pMushroom)
	{
		m_sTag = "Mushroom";
	}
	else
	{
		m_sTag = "Empty Cell";
	}
}

// Compares a centipede head with the one occupying this cell.
bool Cell::CompareCentipede(CentipedeHead* a_pCentipede)
{
	if (m_pCentipedeHead == a_pCentipede)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Cell::SetPosition(float x, float y)
{
	m_position = Vector2D(x, y);
}

// Sets a tag for what's occupying the cell.
void Cell::SetTag(std::string a_tag)
{
	m_sTag = a_tag;
}

// Sets a pointer to the centipede occupying the cell.
void Cell::SetCentipede(CentipedeHead* a_pCentipedeHead, 
	Centipede* a_pCentipede)
{
	m_pCentipedeHead = a_pCentipedeHead;
	m_pCentipede = a_pCentipede;
}

// Sets a pointer to the spider occupying the cell.
void Cell::SetSpider(Spider* a_pSpider)
{
	m_pSpider = a_pSpider;
}

// Sets a pointer to the flea occupying the cell.
void Cell::SetFlea(Flea* a_pFlea)
{
	m_pFlea = a_pFlea;
}

const unsigned int Cell::GetWidth() const
{
	return mc_uiWidth;
}

const unsigned int Cell::GetHeight() const
{
	return mc_uiHeight;
}

const Vector2D* Cell::GetPosition() const
{
	return &m_position;
}

// Gets the tag for what's occupying the cell.
std::string Cell::GetTag() const
{
	return m_sTag;
}

// Returns the cell's mushroom if one is present.
Mushroom* Cell::GetMushroom()
{
	return m_pMushroom;
}

// Returns the cell's centipede head if one is present.
CentipedeHead* Cell::GetCentipedeHead() const
{
	return m_pCentipedeHead;
}

// Returns the cell's centipede if one is present.
Centipede* Cell::GetCentipede() const
{
	return m_pCentipede;
}

// Returns the cell's spider if one is present.
Spider* Cell::GetSpider() const
{
	return m_pSpider;
}

// Returns the cell's flea if one is present.
Flea* Cell::GetFlea() const
{
	return m_pFlea;
}