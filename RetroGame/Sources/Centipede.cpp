/////////////////////////////
// Author: Liam Bansal
// Date Created: 14/05/2020
/////////////////////////////

// Includes.
#include "Centipede.h" // File's header.
#include "Cell.h"
#include "CentipedeBody.h"
#include "Clamp.h"
#include "Enemy.h"
#include "GameplayState.h"
#include "GameStateManager.h"
#include "Grid.h"

Centipede::Centipede() : Enemy(),
	m_pParent(*this),
	m_pHead(nullptr),
	// Set to same direction as movement direction.
	m_currentDirection(MOVE_DIRECTIONS::MOVE_DIRECTIONS_LEFT)
{
	// The default starting direction to move.
	m_moveDirection = m_moveDirection.Left();
	// Default centipede movement speed.
	m_uiSpeed = 90;
	m_pLastPosition = &m_position;
}

Centipede::~Centipede()
{}

void Centipede::operator=(const Centipede* a_rCentipede)
{
	if (a_rCentipede)
	{
		m_uiHealth = a_rCentipede->m_uiHealth;
		m_uiSpeed = a_rCentipede->m_uiSpeed;
		m_moveDirection = a_rCentipede->m_moveDirection;
		m_pLastPosition = a_rCentipede->m_pLastPosition;
		m_currentDirection = a_rCentipede->m_currentDirection;
		m_pPreviousCellsPosition = a_rCentipede->m_pPreviousCellsPosition;
	}
}

// Draws a centipedes part onto the screen.
void Centipede::Draw(GameplayState* a_pScene)
{
	if (a_pScene && a_pScene->GetManager() && m_pSprite)
	{
		a_pScene->GetManager()->DrawSprite((int32_t)m_position.GetX() -
			m_pSprite->width * 0.5f,
			(int32_t)m_position.GetY() -
			m_pSprite->height * 0.5f,
			m_pSprite);
	}
}

// Updates the centipede part's currently & previously occupied cell positions.
void Centipede::UpdateOccupiedCell(Grid* a_pGrid)
{
	if (a_pGrid)
	{
		// Check if this centipede part has occupied a cell before its 
		// current one.
		// This will be false for freshly spawned centipede parts.
		if (m_pPreviousCellsPosition)
		{
			// Clear the previously occupied cell of its reference to a 
			// centipede.
			a_pGrid->GetCell(m_pPreviousCellsPosition).ClearCentipede();
		}

		// Set a reference to this centipede's head for its currently 
		// occupied cell. So centipede parts don't collide with parts from 
		// their centipede.
		a_pGrid->GetCell(&m_position).SetCentipede(GetHead(), this);
		a_pGrid->GetCell(&m_position).SetTag("Centipede");
		// Update this centipede part's previous occupied cell's position with 
		// the position of the one its currently occupying.
		m_pPreviousCellsPosition =
			(Vector2D*)a_pGrid->GetCell(&m_position).GetPosition();
	}
}

// Sets the head which this centipede will follow.
void Centipede::SetHead(CentipedeHead* a_head)
{
	m_pHead = a_head;
}

// Returns the centipede part's current movement direction.
Vector2D* Centipede::GetMoveDirection()
{
	return &m_moveDirection;
}

// Returns the direction in which the centipede checks for collisions.
MOVE_DIRECTIONS* Centipede::GetForwardDirection()
{
	return &m_currentDirection;
}

// Returns the position of the centipede part's previously occupied cell.
Vector2D* Centipede::GetLastCellPosition()
{
	return m_pPreviousCellsPosition;
}

// Returns a pointer to the centipede's head.
CentipedeHead* Centipede::GetHead()
{
	return m_pHead;
}

// Returns the centipede part's parent class.
Centipede* Centipede::GetParent()
{
	return &m_pParent;
}