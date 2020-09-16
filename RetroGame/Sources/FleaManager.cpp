/////////////////////////////
// Author: Liam Bansal
// Date Created: 26/05/2020
/////////////////////////////

// Includes.
#include "FleaManager.h" // File's header.
#include "Flea.h"
#include "GameplayState.h"
#include "GameStateManager.h"
#include "Grid.h"
#include "RandGenerator.h"
#include "Vector2D.h"

FleaManager::FleaManager() : m_uiMaxDestroyCount(10),
	m_uiDestroyCount(10),
	m_uiMushroomsDestroyed(0),
	m_fMaxRespawnTime(60.0f),
	m_fRespawnTime(40.0f),
	m_pFlea(nullptr)
{}

FleaManager::~FleaManager()
{
	if (m_pFlea)
	{
		delete m_pFlea;
		m_pFlea = nullptr;
	}
}

// Updates the manager's flea and handles when to spawn it.
void FleaManager::Update(GameplayState* a_pGameplayState,
	Grid* a_pGrid,
	float* a_pDeltaTime)
{
	if (a_pDeltaTime)
	{
		// Update the flea if it exists...
		if (m_pFlea)
		{
			m_pFlea->Update(a_pGameplayState,
				a_pGrid,
				*a_pDeltaTime);
		}
		// ...otherwise start respawning it.
		else if (m_pFlea == nullptr)
		{
			m_fRespawnTime -= (*a_pDeltaTime);

			// Check if the player has destroyed enough mushrooms to spawn a 
			// flea or if the respawn time hes reached zero.
			if (m_uiMushroomsDestroyed >= m_uiDestroyCount ||
				m_fRespawnTime <= 0.0f)
			{
				SpawnFlea(a_pGrid);
				m_fRespawnTime = m_randGenerator.GetRandomFloat(m_fMaxRespawnTime);
				// Reset the mushroom counter.
				m_uiMushroomsDestroyed = 0;
				// Get a new semi-random number for how many mushrooms need to 
				// be destroyed and the next respawn time.
				RandGenerator randGenerator;
				m_uiDestroyCount = randGenerator.GetRandomInteger(m_uiMaxDestroyCount);
			}
		}
	}
}

// Draws the manager's flea.
void FleaManager::Draw(GameplayState* a_pGameplayState)
{
	if (m_pFlea)
	{
		m_pFlea->Draw(a_pGameplayState);
	}
}

// Spawns a flea along the grid's top boundary somehwat randomly between the 
// left and right boundaries.
void FleaManager::SpawnFlea(Grid* a_pGrid)
{
	if (m_pFlea == nullptr)
	{
		m_pFlea = new Flea();
		m_pFlea->SetManager(this);
	}

	if (a_pGrid && m_pFlea->GetSprite())
	{
		Vector2D spawnposition(m_randGenerator.GetRandomFloat((float)a_pGrid->
			GetRightBoundary(m_pFlea->GetSprite()->height)),
			(float)a_pGrid->GetUpperBoundary(m_pFlea->GetSprite()->height));
		m_pFlea->SetPosition(a_pGrid->GetCell(&spawnposition).GetPosition());
	}
}

void FleaManager::DestroyFlea(Grid* a_pGrid)
{
	if (a_pGrid && m_pFlea)
	{
		// Clear cell references to the flea.
		a_pGrid->GetCell(m_pFlea->GetPreviousPosition()).ClearFlea();
		a_pGrid->GetCell(m_pFlea->GetCurrentPosition()).ClearFlea();
		delete m_pFlea;
		m_pFlea = nullptr;
	}
}

// Increments the counter tracking the number of mushrooms destroyed by 
// the player but only if no fleas exist.
void FleaManager::MushroomDestroyed()
{
	if (m_pFlea == nullptr)
	{
		++m_uiMushroomsDestroyed;
	}
}