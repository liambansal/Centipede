/////////////////////////////
// Author: Liam Bansal
// Date Created: 03/05/2020
/////////////////////////////

// Includes.
#include "CentipedeBody.h" // File's header.
#include "BugBlaster.h"
#include "CentipedeHead.h"
#include "Clamp.h"
#include "GameplayState.h"
#include "GameState.h"
#include "Grid.h"
#include "PopUpText.h"
#include "Score.h"

CentipedeBody::CentipedeBody() : Centipede(),
	mc_uiMoveSprites(8),
	mc_fMargin(1.0f),
	m_targetCells()
{
	m_uiScoreValue = 10;
	m_moveDirection = m_moveDirection.Up();
	m_pSprite = new olc::Sprite("Resources/Sprites/Centipede Body/Centipede Body_0.png");
	m_pHead = nullptr;
	// Set to down because centipede body always spawns in a column.
	m_currentDirection = MOVE_DIRECTIONS::MOVE_DIRECTIONS_DOWN;

	for (unsigned int spriteCount = 1;
		spriteCount <= mc_uiMoveSprites;
		++spriteCount)
	{
		m_animator.AddFrame("Move",
			new olc::Sprite("Resources/Sprites/Centipede Body/Centipede Body (" +
				std::to_string(spriteCount) +
				").png"));
	}

	m_animator.SetAnimation("Move");
	const float frameLength = 0.06f;
	m_animator.SetFrameLength(frameLength);
}

CentipedeBody::~CentipedeBody()
{}

// Updates the centipede's position, collisions, animation and spawning.
void CentipedeBody::Update(GameplayState* a_pGameplayState,
	Grid* a_pGrid,
	CentipedeBody* a_pForwardBody,
	float* a_pDeltaTime)
{
	if (GetHealth() <= 0)
	{
		a_pGameplayState->AddScore(PopUpText(std::to_string(m_uiScoreValue), m_position));
		Split(a_pGameplayState, a_pGrid);
	}
	else // Continue updating while health is above 0.
	{
		if (a_pGameplayState &&
			m_targetCells.size() > 0)
		{
			Move(a_pGrid, a_pForwardBody, a_pDeltaTime);
			m_animator.Update(a_pDeltaTime);
			m_pSprite = m_animator.GetCurrentFrame();
			CheckTargetCollision(a_pGrid);
		}
	}
}

// Moves the body towards it's first target cell.
void CentipedeBody::Move(Grid* a_pGrid,
	CentipedeBody* a_pForwardBody,
	float* a_pDeltaTime)
{
	m_position += m_moveDirection.normalised() *
		(const float)m_uiSpeed *
		(*a_pDeltaTime);
	CloseDistance(a_pForwardBody, a_pDeltaTime);
	UpdateOccupiedCell(a_pGrid);
}

// Checks for collisions between the body and it's first target cell.
void CentipedeBody::CheckTargetCollision(Grid* a_pGrid)
{
	if (a_pGrid &&
		m_targetCells.size() > 0 &&
		m_targetCells.front()->GetPosition())
	{
		// ...check for collisions along the current movement direction.
		// Register a 'collision' when our x or y position is greater/less than 
		// or equal to the target cell's x or y position.
		switch (m_currentDirection)
		{
		case MOVE_DIRECTIONS::MOVE_DIRECTIONS_UP:
			{
				if (m_position.GetY() <=
					m_targetCells.front()->GetPosition()->GetY())
				{
					SetMoveDirection(a_pGrid);
				}

				break;
			}
		case MOVE_DIRECTIONS::MOVE_DIRECTIONS_LEFT:
			{
				if (m_position.GetX() <=
					m_targetCells.front()->GetPosition()->GetX())
				{
					SetMoveDirection(a_pGrid);
				}
				
				break;
			}
		case MOVE_DIRECTIONS::MOVE_DIRECTIONS_DOWN:
			{
				if (m_position.GetY() >=
					m_targetCells.front()->GetPosition()->GetY())
				{
					SetMoveDirection(a_pGrid);
				}

				break;
			}
		case MOVE_DIRECTIONS::MOVE_DIRECTIONS_RIGHT:
			{
				if (m_position.GetX() >=
					m_targetCells.front()->GetPosition()->GetX())
				{
					SetMoveDirection(a_pGrid);
				}

				break;
			}
		default:
			{
				break;
			}
		}
	}
}

// Makes sure there's always a near constant distance between this 
// and the previous body part.
void CentipedeBody::CloseDistance(CentipedeBody* a_pForwardBody,
	float* a_pDeltaTime)
{
	// Only effect the distancing of centiepde parts if they're moving 
	// horizontally. Distances between parts lengthen as they bend around 
	// corners.
	if (m_pSprite != nullptr &&
		(m_currentDirection == MOVE_DIRECTIONS::MOVE_DIRECTIONS_LEFT ||
		m_currentDirection == MOVE_DIRECTIONS::MOVE_DIRECTIONS_RIGHT))
	{
		// Is there a body part prior to this one in the head's body parts list.
		if (a_pForwardBody != nullptr)
		{
			if (m_position.distance(*a_pForwardBody->GetCurrentPosition()) >
				m_pSprite->height + mc_fMargin)
			{
				// Route to the previous body.
				Vector2D direction = m_position -
					(*a_pForwardBody->GetCurrentPosition());
				// Moves the body back along the route to open the distance.
				m_position -= (direction.normalised() *
					direction.magnitude()) *
					(*a_pDeltaTime);
			}
			else if (m_position.distance(*a_pForwardBody->GetCurrentPosition()) <
				m_pSprite->height - mc_fMargin)
			{
				// Route to the previous body.
				Vector2D direction = m_position -
					(*a_pForwardBody->GetCurrentPosition());
				// Moves the body forward along the route to close the distance.
				m_position += (direction.normalised() *
					direction.magnitude()) *
					(*a_pDeltaTime);
			}
		}
		else if (m_pHead != nullptr) // The head must precede this body in the 
		// centipede's chain.
		{
			if (m_position.distance(*m_pHead->GetCurrentPosition()) >
				m_pSprite->height + mc_fMargin)
			{
				// Route to the head.
				Vector2D direction = m_position - (*m_pHead->GetCurrentPosition());
				// Moves the body back along the route to open the distance.
				m_position -= (direction.normalised() *
					direction.magnitude()) *
					(*a_pDeltaTime);
			}
			else if (m_position.distance(*m_pHead->GetCurrentPosition()) <
				m_pSprite->height - mc_fMargin)
			{
				// Route to the head.
				Vector2D direction = m_position - *m_pHead->GetCurrentPosition();
				// Moves the body forward along the route to close the distance.
				m_position += (direction.normalised() *
					direction.magnitude()) *
					(*a_pDeltaTime);
			}
		}
	}
}

// Calls to split the centipede at this bodies position.
void CentipedeBody::Split(GameplayState* a_pState, Grid* a_pGrid)
{
	if (a_pState && m_pHead && a_pState->GetScore())
	{
		// Increase the player's score.
		a_pState->GetScore()->Update(m_uiScoreValue);
		m_pHead->Split(a_pGrid, this);
	}
}

// Sets a new movement direction and target movement destination.
void CentipedeBody::SetMoveDirection(Grid* a_pGrid)
{
	// We need to calculate a direction from the current target to the next 
	// target, so we need at least two cells.
	const unsigned int minimumCellCount = 2;

	if (m_targetCells.front() &&
		m_targetCells.size() >= minimumCellCount)
	{
		auto listIterator = m_targetCells.begin();
		listIterator++;

		if (listIterator != m_targetCells.end() &&
			m_targetCells.front()->GetPosition() &&
			(*listIterator))
		{
			// Get direction between first two target cells.
			m_moveDirection = (*((*listIterator)->GetPosition()) -
				(*m_targetCells.front()->GetPosition()));
			m_moveDirection.normalise();

			if (m_moveDirection == m_moveDirection.Up())
			{
				m_currentDirection = MOVE_DIRECTIONS::MOVE_DIRECTIONS_DOWN;
			}
			else if (m_moveDirection == m_moveDirection.Left())
			{
				m_currentDirection = MOVE_DIRECTIONS::MOVE_DIRECTIONS_LEFT;
			}
			else if (m_moveDirection == m_moveDirection.Down())
			{
				m_currentDirection = MOVE_DIRECTIONS::MOVE_DIRECTIONS_UP;
			}
			else if (m_moveDirection == m_moveDirection.Right())
			{
				m_currentDirection = MOVE_DIRECTIONS::MOVE_DIRECTIONS_RIGHT;
			}

			// Allign the centipede's y position with it's target cell's row.
			m_position.SetY(m_targetCells.front()->GetPosition()->GetY());
			m_targetCells.pop_front();
		}
	}
}

// Returns a list of cells that the body will move towards.
std::list<Cell*>* CentipedeBody::GetTargetCells()
{
	return &m_targetCells;
}