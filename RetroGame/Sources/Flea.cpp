/////////////////////////////
// Author: Liam Bansal
// Date Created: 26/05/2020
/////////////////////////////

// Includes.
#include "Flea.h" // File's header.
#include "FleaManager.h"
#include "GameplayState.h"
#include "Grid.h"
#include "RandGenerator.h"
#include "PopUpText.h"
#include "Score.h"

Flea::Flea() : Enemy(),
	mc_uiMoveSprites(2),
	mc_fMushroomSpawnChance(4.0f),
	m_pFleaManager(nullptr)
{
	m_uiScoreValue = 50;
	m_pSprite = new olc::Sprite("Resources/Sprites/Flea/Flea (1).png");
	m_moveDirection = m_moveDirection.Up();
	m_uiSpeed = 80;

	for (unsigned int i = 1; i <= mc_uiMoveSprites; ++i)
	{
		m_animator.AddFrame("Move",
			new olc::Sprite("Resources/Sprites/Flea/Flea (" +
				std::to_string(i) +
				").png"));
	}

	m_animator.SetAnimation("Move");
	const float frameLength = 0.1f;
	m_animator.SetFrameLength(frameLength);
}

Flea::~Flea()
{}

// Updates the fleas movement, animation, collisions and death.
void Flea::Update(GameplayState* a_pGameplayState,
	Grid* a_pGrid,
	float a_deltaTime)
{
	if (a_pGameplayState)
	{
		if (m_uiHealth <= 0)
		{
			if (a_pGameplayState->GetScore() && m_pFleaManager)
			{
				a_pGameplayState->AddScore(PopUpText(std::to_string(m_uiScoreValue), m_position));
				a_pGameplayState->GetScore()->Update(m_uiScoreValue);
				m_pFleaManager->SpawnFlea(a_pGrid);
				m_uiHealth = mc_uiMaxHealth;
				return;
			}
		}
		else
		{
			Move(a_pGrid, &a_deltaTime);
			m_animator.Update(&a_deltaTime);
			m_pSprite = m_animator.GetCurrentFrame();
			UpdateOccupiedCell(a_pGrid);
			CheckCollisions(a_pGameplayState, a_pGrid);
		}
	}
}

// Moves the flea towards it's target cell.
void Flea::Move(Grid* a_pGrid, float* a_pDeltaTime)
{
	if (a_pDeltaTime)
	{
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
	}
}

// Checks for collisions with a grid's uppder & lower boundaries.
void Flea::CheckCollisions(GameplayState* a_pGameplayState,
	Grid* a_pGrid)
{
	if (m_pSprite && a_pGrid)
	{
		// Screen origin starts at top left corner. Y axis increases downwards. X 
		// axis increases to the right.
		// Check if y position is greater than the grid's bottom boundary.
		if (m_position.GetY() > a_pGrid->GetLowerBoundary(m_pSprite->height / 2))
		{
			Destroy(a_pGrid);
		}
		// Check if y position is lower than the grid's top boundary.
		else if (m_position.GetY() < a_pGrid->GetUpperBoundary(m_pSprite->height / 2))
		{
			Destroy(a_pGrid);
		}
	}
}

// Updates the flea's currently and previously occupied cell positions.
// Chance to spawn mushrooms in new cells.
void Flea::UpdateOccupiedCell(Grid* a_pGrid)
{
	if (a_pGrid)
	{
		// Check if the flea has occupied a cell before its current one.
		// This will be false for newly spawned fleas.
		if (m_pPreviousCellsPosition)
		{
			// Clear the previously occupied cell of its reference to a 
			// flea.
			a_pGrid->GetCell(m_pPreviousCellsPosition).ClearFlea();
			m_pLastPosition = m_pPreviousCellsPosition;
		}

		RandGenerator randGenerator;
		const float maximumReturnValue = 100.0f;

		// For each new cell that the flea moves into; spawn a mushroom if the 
		// semi-random float is less than the spawn chance.
		if (randGenerator.GetRandomFloat(maximumReturnValue) <
			mc_fMushroomSpawnChance)
		{
			a_pGrid->GetCell(&m_position).SpawnMushroom();
		}

		// Set a reference to this flea for its currently occupied cell.
		a_pGrid->GetCell(&m_position).SetFlea(this);
		a_pGrid->GetCell(&m_position).SetTag("Flea");
		// Update the flea previous occupied cell's position with 
		// the position of the one its currently occupying.
		m_pPreviousCellsPosition =
			(Vector2D*)a_pGrid->GetCell(&m_position).GetPosition();
	}
}

void Flea::Destroy(Grid* a_pGrid)
{
	if (m_pFleaManager)
	{
		m_pFleaManager->DestroyFlea(a_pGrid);
	}
}

void Flea::SetManager(FleaManager* a_pFleaManager)
{
	m_pFleaManager = a_pFleaManager;
}