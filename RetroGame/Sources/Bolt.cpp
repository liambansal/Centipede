/////////////////////////////
// Author: Liam Bansal
// Date Created: 01/03/2020
/////////////////////////////

// Includes
#include "Bolt.h" // File's header.
#include "BugBlaster.h"
#include "Centipede.h"
#include "GameplayState.h"
#include "GameStateManager.h"
#include "Grid.h"
#include "Mushroom.h"
#include "Spider.h"
#include "Flea.h"

Bolt::Bolt()
{
	m_pSprite = new olc::Sprite("Resources/Sprites/bolt.png");
	m_spawn = Vector2D(0.0f, 0.0f);
	m_position = m_spawn;
	m_bCanFire = false;
	m_fVelocity = 480.0f;
}

Bolt::~Bolt()
{
	if (m_pSprite)
	{
		delete m_pSprite;
		m_pSprite = nullptr;
	}
}
 
// Moves the bolt and updates its collisions.
void Bolt::Update(Grid* a_pGrid, 
	float* a_pDeltaTime,
	int32_t a_screenWidth, 
	int32_t a_screenHeight)
{
	if (m_bCanFire)
	{
		// We want the bolt to travel up the screen.
		// Our coordinate system measures moving down the screen as increasing 
		// along the y axis.
		m_position -= m_position.Up() * m_fVelocity * (*a_pDeltaTime);
		ClampPosition(a_pGrid, a_screenWidth, a_screenHeight);
		CheckCollisions(a_pGrid);
	}
}

// Draws the bolt onto the screen.
void Bolt::Draw(GameplayState* a_pState) const
{
	if (a_pState && a_pState->GetManager() && m_bCanFire)
	{
		a_pState->GetManager()->DrawSprite((int32_t)m_position.GetX(), 
			(int32_t)m_position.GetY(),
			m_pSprite);
	}
}

// Sets the bolt as ready to fire.
void Bolt::Fire(BugBlaster* a_pBugBlaster)
{
	if (a_pBugBlaster && !m_bCanFire)
	{
		m_position = Vector2D(a_pBugBlaster->GetPosition().GetX(), 
			a_pBugBlaster->GetPosition().GetY() - (m_pSprite->height * 0.5f));
		m_bCanFire = true;
	}
}

// Checks for collisions with objects occupying the cell which the bolt is 
// traveling through.
void Bolt::CheckCollisions(Grid* a_pGrid)
{
	if (a_pGrid)
	{
		// Gets the cell at the bolt's position.
		Cell* cell = nullptr;
		cell = &a_pGrid->GetCell(&m_position);

		if (cell != nullptr)
		{
			// Check what's occupying the cell.
			if (cell->GetFlea())
			{
				cell->GetFlea()->DecreaseHealth();
				m_position = m_spawn;
				m_bCanFire = false;
			}
			else if (cell->GetSpider())
			{
				cell->GetSpider()->DecreaseHealth();
				m_position = m_spawn;
				m_bCanFire = false;
			}
			else if (cell->GetCentipede())
			{
				cell->GetCentipede()->DecreaseHealth();
				m_position = m_spawn;
				m_bCanFire = false;
			}
			else if (cell->GetTag() == "Mushroom")
			{
				cell->GetMushroom()->DecreaseHealth();
				m_position = m_spawn;
				m_bCanFire = false;
			}
		}
	}
}

// Respawns the bolt when its position passes one of the grid's boundaries.
void Bolt::ClampPosition(Grid* a_pGrid,
	int32_t a_screenWidth,
	int32_t a_screenHeight)
{
	if (a_pGrid)
	{
		// Screen origin starts at top left corner. Y axis increases downwards. X 
		// axis increases to the right.
		// Check if x position is greater than the grid's right boundary.
		if (m_position.GetX() > a_pGrid->GetRightBoundary(m_pSprite->height * 0.5f))
		{
			m_position = m_spawn;
			m_bCanFire = false;
		}
		// Check if x position is lower than the grid's left boundary.
		else if (m_position.GetX() < a_pGrid->GetLeftBoundary(m_pSprite->height * 0.5f))
		{
			m_position = m_spawn;
			m_bCanFire = false;
		}

		// Check if y position is greater than the grid's top boundary plus the no 
		// movement zone's height.
		if (m_position.GetY() < a_pGrid->GetUpperBoundary(m_pSprite->height * 0.5f))
		{
			m_position = m_spawn;
			m_bCanFire = false;
		}
		// Check if y position is lower than the grid's bottom boundary.
		else if (m_position.GetY() > a_pGrid->GetLowerBoundary(m_pSprite->height * 0.5f))
		{
			m_position = m_spawn;
			m_bCanFire = false;
		}
	}
}

void Bolt::SetSprite(olc::Sprite* a_pSprite)
{
	m_pSprite = a_pSprite;
}

void Bolt::SetPosition(float a_x, float a_y)
{
	m_position = Vector2D(a_x, a_y);
}

olc::Sprite* Bolt::GetSprite() const
{
	return m_pSprite;
}

Vector2D Bolt::GetPosition() const
{
	return m_position;
}