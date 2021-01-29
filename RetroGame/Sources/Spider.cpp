/////////////////////////////
// Author: Liam Bansal
// Date Created: 26/05/2020
/////////////////////////////

// Includes
#include "Spider.h" // File's header.
#include "BugBlaster.h"
#include "Clamp.h"
#include "GameplayState.h"
#include "Grid.h"
#include <list>
#include "PopUpText.h"
#include "Score.h"
#include "Vector2D.h"

Spider::Spider() : Enemy(),
	mc_uiMoveSprites(7),
	m_pSpiderManager(nullptr),
	m_uiScores {
		300,
		600,
		900
	},
	m_fDistances {
		24.0f,
		16.0f,
		0.0f
	},
	m_verticalDirection(VERTICAL_DIRECTION::UP)
{
	m_uiSpeed = 85;
	m_pSprite = new olc::Sprite("Resources/Sprites/Spider/Spider (1).png");
	m_position = Vector2D(120.0f, 100.0f);
	m_moveDirection = Vector2D(-1.0f, -1.0f);

	for (unsigned int i = 1; i <= mc_uiMoveSprites; ++i)
	{
		m_animator.AddFrame("Move",
			new olc::Sprite("Resources/Sprites/Spider/Spider (" + std::to_string(i) + ").png"));
	}

	m_animator.SetAnimation("Move");
	const float frameLength = 0.06f;
	m_animator.SetFrameLength(frameLength);
}

Spider::~Spider()
{}

// Updates the spider's movement, collisions, animation and death.
void Spider::Update(GameplayState* a_pGameplayState,
	Grid* a_pGrid,
	float* a_pDeltaTime)
{
	if (m_uiHealth <= 0)
	{
		unsigned int arrayLength = sizeof(m_fDistances) / sizeof(float);

		for (unsigned int i = 0; i < arrayLength; ++i)
		{
			if (GetDistance(a_pGameplayState->GetBugBlaster()) >= m_fDistances[i])
			{
				a_pGameplayState->AddScore(PopUpText(std::to_string(m_uiScores[i]), m_position));
			}
		}

		Destroy(a_pGameplayState, a_pGrid, true);
	}
	else
	{
		Move(a_pGrid, a_pDeltaTime);
		m_animator.Update(a_pDeltaTime);
		m_pSprite = m_animator.GetCurrentFrame();
		// Call this function last because it may delete the spider.
		CheckCollisions(a_pGameplayState, a_pGrid);
	}
}

// Moves the spider towards it's target cell.
void Spider::Move(Grid* a_pGrid, float* a_pDeltaTime)
{
	if (a_pGrid)
	{
		// If we have a target to move towards then do it.
		if (m_pTargetCell)
		{
			m_position += m_moveDirection.normalised() * (const float)m_uiSpeed * (*a_pDeltaTime);
		}
		else
		{
			SetTargetCell(a_pGrid);
		}

		// Check if the spider has moved into a new cell.
		if (a_pGrid->GetCell(&m_position).GetPosition() !=
			m_pPreviousCellsPosition)
		{
			UpdateOccupiedCell(a_pGrid);
		}
	}
}

// Checks for collisions with the target cell and grid's boundaries.
void Spider::CheckCollisions(GameplayState* a_pGameplayState,
	Grid* a_pGrid)
{
	if (a_pGrid && m_pSprite)
	{
		// Screen origin starts at top left corner. Y axis increases downwards. X 
		// axis increases to the right.
		// Passed left or right play area boundaries.
		if (m_position.GetX() > a_pGrid->GetRightBoundary((unsigned int)(m_pSprite->height * 0.5f)) ||
			m_position.GetX() < a_pGrid->GetLeftBoundary((unsigned int)(m_pSprite->height * 0.5f)))
		{
			Destroy(a_pGameplayState, a_pGrid, false);
			return;
		}

		// Destroy if reached the play area's left boundary.
		if (m_position.GetX() < a_pGrid->GetLeftBoundary(m_pSprite->width * 2))
		{
			Destroy(a_pGameplayState, a_pGrid, false);
			return;
		}

		const float touchDistance = 1.0f;
		if (m_pTargetCell && m_position.distance(*m_pTargetCell->GetPosition()) < touchDistance)
		{
			SetTargetCell(a_pGrid);
		}
	}
}

// Updates the spider's currently and previously occupied cell positions.
// Clears mushrooms in the spider's current cell.
void Spider::UpdateOccupiedCell(Grid* a_pGrid)
{
	if (a_pGrid)
	{
		// Check if this spider has occupied a cell before its current one.
		// This will be false for newly spawned spiders.
		if (m_pPreviousCellsPosition)
		{
			// Clear the previously occupied cell of its reference to a 
			// spider.
			a_pGrid->GetCell(m_pPreviousCellsPosition).ClearSpider();
			m_pLastPosition = m_pPreviousCellsPosition;
		}

		// Destroy any mushrooms that the spider walks over.
		a_pGrid->GetCell(&m_position).ClearMushroom();
		// Set a reference to this centipede's head for its currently 
		// occupied cell. So centipede parts don't collide with parts from 
		// their centipede.
		a_pGrid->GetCell(&m_position).SetSpider(this);
		a_pGrid->GetCell(&m_position).SetTag("Spider");
		// Update this centipede part's previous occupied cell's position with 
		// the position of the one its currently occupying.
		m_pPreviousCellsPosition = (Vector2D*)a_pGrid->GetCell(&m_position).GetPosition();
	}
}

// Destroys the spider and rewards the player with points if they killed it.
void Spider::Destroy(GameplayState* a_pGameplayState,
	Grid* a_pGrid,
	bool a_killedByPlayer)
{
	if (a_pGameplayState &&
		a_pGameplayState->GetScore())
	{
		// Only increase the players score if a_killedByPlayer is true.
		// a_killedByPlayer is true for spiders killed by the bug blaster.
		if (a_killedByPlayer)
		{
			unsigned int arrayLength = sizeof(m_fDistances) / sizeof(float);

			for (unsigned int i = 0; i < arrayLength; ++i)
			{
				if (GetDistance(a_pGameplayState->GetBugBlaster()) >= m_fDistances[i])
				{
					a_pGameplayState->GetScore()->Update(m_uiScores[i]);
					break;
				}
			}
		}

		m_pSpiderManager->DestroySpider(a_pGrid);
	}
}

void Spider::SetManager(SpiderManager* a_pSpiderManager)
{
	m_pSpiderManager = a_pSpiderManager;
}

// Sets a target cell to move towards.
void Spider::SetTargetCell(Grid* a_pGrid)
{
	if (m_pSprite && a_pGrid)
	{
		unsigned int tilesToMoveAcross = 4;

		if (m_verticalDirection == VERTICAL_DIRECTION::UP)
		{
			m_verticalDirection = VERTICAL_DIRECTION::DOWN;
			m_moveDirection = Vector2D(-1.0f, 1.0f);
			// Get the number of cells between the spider and top & bottom blaster zone boundaries.
			const unsigned int lowerCellLength = (unsigned int)(m_position.distance(Vector2D(m_position.GetX(), (float)(a_pGrid->GetLowerBoundary(0)))) / a_pGrid->GetCellHeight());

			if (lowerCellLength < tilesToMoveAcross)
			{
				tilesToMoveAcross = lowerCellLength;
			}
		}
		else
		{
			m_verticalDirection = VERTICAL_DIRECTION::UP;
			m_moveDirection = Vector2D(-1.0f, -1.0f);
			const unsigned int upperCellLength = (unsigned int)(m_position.distance(Vector2D(m_position.GetX(), (float)(a_pGrid->GetLowerBoundary(0) - a_pGrid->GetBlasterZoneHeight()))) / a_pGrid->GetCellHeight());

			if (upperCellLength < tilesToMoveAcross)
			{
				tilesToMoveAcross = upperCellLength;
			}
		}

		// The position that we want to move to.
		Vector2D targetPosition = *a_pGrid->GetCell(&m_position).GetPosition() + m_moveDirection * ((const float)m_pSprite->height * tilesToMoveAcross);
		m_pTargetCell = &a_pGrid->GetCell(&targetPosition);
		// Get direction towards target cell position.
		m_moveDirection = *m_pTargetCell->GetPosition() - m_position;
	}
}

// Gets the distance to the bug blaster.
float Spider::GetDistance(BugBlaster* a_pBugBlaster)
{
	if (a_pBugBlaster)
	{
		return m_position.distance(a_pBugBlaster->GetPosition());
	}
}