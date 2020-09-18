/////////////////////////////
// Author: Liam Bansal
// Date Created: 26/05/2020
/////////////////////////////

// Includes
#include "Spider.h" // File's header.
#include "BugBlaster.h"
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
	}
{
	m_uiSpeed = 85;
	m_pSprite = new olc::Sprite("Resources/Sprites/Spider/Spider (1).png");
	m_position = Vector2D(120.0f, 100.0f);
	m_moveDirection = m_moveDirection.Left();

	for (unsigned int i = 1; i <= mc_uiMoveSprites; ++i)
	{
		m_animator.AddFrame("Move",
			new olc::Sprite("Resources/Sprites/Spider/Spider (" +
				std::to_string(i) +
				").png"));
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
		// TODO: turn into function. start...
		unsigned int arrayLength = sizeof(m_fDistances) / sizeof(float);

		for (unsigned int i = 0; i < arrayLength; ++i)
		{
			if (GetDistance(a_pGameplayState->GetBugBlaster()) >=
				m_fDistances[i])
			{
				a_pGameplayState->AddScore(PopUpText(std::to_string(m_uiScores[i]), m_position));
			}
		}
		//...end

		Destroy(a_pGameplayState,
			a_pGrid,
			true);
	}
	else
	{
		Move(a_pGrid, a_pDeltaTime);
		m_animator.Update(a_pDeltaTime);
		m_pSprite = m_animator.GetCurrentFrame();
		// Call this function last because it may delete this spider.
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
			m_position += m_moveDirection.normalised() *
				(const float)m_uiSpeed *
				(*a_pDeltaTime);
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
		// Check if x position is greater than the grid's right boundary.
		if (m_position.GetX() >
			a_pGrid->GetRightBoundary(m_pSprite->height / 2))
		{
			Destroy(a_pGameplayState, a_pGrid, false);
			return;
		}
		// Check if x position is lower than the grid's left boundary.
		else if (m_position.GetX() <
			a_pGrid->GetLeftBoundary(m_pSprite->height / 2))
		{
			Destroy(a_pGameplayState, a_pGrid, false);
			return;
		}

		if (m_pTargetCell &&
			m_position.distance(*m_pTargetCell->GetPosition()) < 1.0f)
		{
			SetTargetCell(a_pGrid);
		}
	}
}

// Updates the spider's currently and previosuly occupied cell positions.
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
		m_pPreviousCellsPosition =
			(Vector2D*)a_pGrid->GetCell(&m_position).GetPosition();
	}
}

// Destroys the spider and rewards the player with points if they killed it.
void Spider::Destroy(GameplayState* a_pGameplayState,
	Grid* a_pGrid,
	bool a_bool)
{
	if (a_pGameplayState &&
		a_pGameplayState->GetScore())
	{
		// Only increase the players score if a_bool is true.
		// a_bool is true for spiders killed by the bug blaster.
		if (a_bool)
		{
			unsigned int arrayLength = sizeof(m_fDistances) / sizeof(float);

			for (unsigned int i = 0; i < arrayLength; ++i)
			{
				if (GetDistance(a_pGameplayState->GetBugBlaster()) >=
					m_fDistances[i])
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

// Gets the distance to the bug blaster.
float Spider::GetDistance(BugBlaster* a_pBugBlaster)
{
	if (a_pBugBlaster)
	{
		return m_position.distance(a_pBugBlaster->GetPosition());
	}
}