/////////////////////////////
// Author: Liam Bansal
// Date Created: 26/05/2020
/////////////////////////////

// Includes
#include "SpiderManager.h" // File's header.
#include "GameplayState.h"
#include "Grid.h"
#include "Spider.h"
#include "Vector2D.h"

// Respawn time & max is 20
SpiderManager::SpiderManager() : mc_fMaxRespawnTime(20.0f),
	m_fRespawnTime(20.0f),
	m_pSpider(nullptr)
{}

SpiderManager::~SpiderManager()
{
	if (m_pSpider)
	{
		delete m_pSpider;
		m_pSpider = nullptr;
	}
}

// Updates the manager's spider and handles when to spawn it.
void SpiderManager::Update(GameplayState* a_pGameplayState,
	Grid* a_pGrid,
	float* a_pDeltaTime)
{
	if (a_pDeltaTime)
	{
		// Update the spider if one has been created.
		if (m_pSpider)
		{
			m_pSpider->Update(a_pGameplayState,
				a_pGrid,
				a_pDeltaTime);
		}
		else // Start respawning the spider.
		{
			// Reduce the time to respawn with each update.
			m_fRespawnTime -= (*a_pDeltaTime);

			// Spawn a spider once the respawn time has reached zero.
			if (m_fRespawnTime <= 0.0f)
			{
				SpawnSpider(a_pGrid);
				// Get semi-random float up to the max respawn time and assign it 
				// as a new respawn time.
				m_fRespawnTime = randGenerator.GetRandomFloat(mc_fMaxRespawnTime);
			}
		}
	}
}

// Draws the manager's spider onto the screen.
void SpiderManager::Draw(GameplayState* a_pGameplayState)
{
	if (m_pSpider)
	{
		m_pSpider->Draw(a_pGameplayState);
	}
}

// Spawns a new spider to fight.
// Only one spider may exist at any given time.
void SpiderManager::SpawnSpider(Grid* a_pGrid)
{
	if (a_pGrid && m_pSpider == nullptr)
	{
		m_pSpider = new Spider();

		if (m_pSpider->GetSprite())
		{
			m_pSpider->SetManager(this);
			// Sets the spiders position to be somewhere along the grid's right 
			// boundary, between the lower boundary and the start of the no movement 
			// zone.
			Vector2D spawn((float)a_pGrid->GetRightBoundary(m_pSpider->GetSprite()->width * 0.5f),
				// Pad top and bottom of blaster zone with half the spiders height.
				(float)a_pGrid->GetLowerBoundary(m_pSpider->GetSprite()->height * 0.5f) - randGenerator.GetRandomFloat((float)a_pGrid->GetBlasterZoneHeight() - m_pSpider->GetSprite()->height * 0.5f));
			m_pSpider->SetPosition(a_pGrid->GetCell(&spawn).GetPosition());
		}
	}
}

// Deallocates the memory for the spider and removes cell references to it, 
// destroying it in the process.
void SpiderManager::DestroySpider(Grid* a_pGrid)
{
	if (a_pGrid && m_pSpider)
	{
		// Clear cell references to this spider.
		a_pGrid->GetCell(m_pSpider->GetPreviousPosition()).ClearSpider();
		a_pGrid->GetCell(m_pSpider->GetCurrentPosition()).ClearSpider();
		delete m_pSpider;
		m_pSpider = nullptr;
	}
}