/////////////////////////////
// Author: Liam Bansal
// Date Created: 19/05/2020
/////////////////////////////

// Includes.
#include "GameplayState.h" // File's header.
#include "BugBlaster.h"
#include "FleaManager.h"
#include "GameState.h"
#include "GameStateManager.h"
#include "Grid.h"
#include "MenuState.h"
#include "PauseState.h"
#include "PopUpText.h"

GameplayState::GameplayState() : GameState(),
	m_pBugBlaster(nullptr),
	m_pGrid(nullptr),
	m_pScore(nullptr)
{}

GameplayState::~GameplayState()
{
	if (m_pGrid)
	{
		delete m_pGrid;
		m_pGrid = nullptr;
	}

	if (m_pBugBlaster)
	{
		delete m_pBugBlaster;
		m_pBugBlaster = nullptr;
	}
}

// Sets up the gameplay state with its grid, bug blaster and enemy managers.
void GameplayState::Initialize(MenuState* a_pMenuState)
{
	if (m_pManager && a_pMenuState)
	{
		m_bIsActive = true;
		// Seed random with the length of time the application has been 
		// running.
		srand((unsigned int)m_pManager->GetTimer()->GetRunLength());
		// Create a playing grid and spawn some mushrooms.
		m_pGrid = new Grid();
		m_pGrid->SpawnMushrooms();
		// Spawn a centipede for the player to fight.
		m_centipedeManager.SpawnCentipede(m_pGrid);
		// Create a new instance of the ship.
		m_pBugBlaster = new BugBlaster(m_pGrid);
		m_pScore = a_pMenuState->GetScore();

		if (m_pScore)
		{
			m_pScore->ResetScore();
			m_pScore->SetBugBlaster(m_pBugBlaster);
		}
	}
}

// Updates the grid, bug blaster, enemy managers and hanldes input for pausing.
// Call once per frame.
void GameplayState::Update(float a_deltaTime)
{
	if (m_pManager)
	{
		if (m_bIsActive)
		{
			m_fTimeInState += a_deltaTime;

			// Should we quit?
			if (m_pManager->GetKey(olc::Key::ESCAPE).bPressed)
			{
				PauseState* pauseState = new PauseState();
				pauseState->SetManager(m_pManager);
				m_pManager->PushState(pauseState);
				m_pManager->StatesChanged(true);
			}

			// Update our grid.
			if (m_pGrid && m_pBugBlaster)
			{
				m_pGrid->Update(this, *m_pBugBlaster);
			}

			// Update the enemies.
			m_centipedeManager.Update(this,	m_pGrid, &a_deltaTime);
			m_spiderManager.Update(this, m_pGrid, &a_deltaTime);
			m_fleaManager.Update(this, m_pGrid, &a_deltaTime);

			// Update our player ship.
			if (m_pBugBlaster)
			{
				m_pBugBlaster->Update(this,	&a_deltaTime, m_pGrid);
			}

			// Updates pop-up scores.
			for (auto listIterator = m_scores.begin();
				listIterator != m_scores.end();
				++listIterator)
			{
				if (!listIterator->IsActive())
				{
					m_scores.erase(listIterator);
					break;
				}
				else
				{
					listIterator->Update(GetManager()->GetTimer()->GetDeltaTime());
				}
			}
		}
		else
		{
			ExitState();
		}
	}
}

// Draws the state's enemies, bug blaster, score and grid.
void GameplayState::Draw()
{
	if (m_pManager && m_pGrid)
	{
		// Clears previously drawn sprites.
		m_pManager->Clear(olc::BLACK); // TODO: do I need this
		// Enables alpha blending.
		m_pManager->SetPixelMode(olc::Pixel::ALPHA);

		// Update the grid.
		m_pGrid->Draw(this);

		// Draw the enemies.
		m_centipedeManager.Draw(this);
		m_spiderManager.Draw(this);
		m_fleaManager.Draw(this);

		// Draw our player ship.
		if (m_pBugBlaster) // Null check pointer.
		{
			m_pBugBlaster->DrawSprite(this);
			m_pBugBlaster->DrawBolts(this);
			m_pBugBlaster->DrawLives(this);
		}

		// Draw the pop-up scores.
		for (auto listIterator = m_scores.begin();
			listIterator != m_scores.end();
			++listIterator)
		{
			listIterator->Draw(this);
		}

		if (m_pScore)
		{
			m_pScore->Draw();
		}
	}
}

void GameplayState::ExitState()
{
	if (m_pManager)
	{
		m_bIsActive = false;
		m_fTimeInState = 0.0f;
		m_pManager->StatesChanged(true);
		m_pManager->RemoveState(this);
	}
}

void GameplayState::AddScore(PopUpText a_text)
{
	m_scores.push_back(a_text);
}

Grid* GameplayState::GetGrid() const
{
	return m_pGrid;
}

Score* GameplayState::GetScore() const
{
	return m_pScore;
}

BugBlaster* GameplayState::GetBugBlaster() const
{
	return m_pBugBlaster;
}

CentipedeManager* GameplayState::GetCentipedeManager()
{
	return &m_centipedeManager;
}

SpiderManager* GameplayState::GetSpiderManager()
{
	return &m_spiderManager;
}

FleaManager* GameplayState::GetFleaManager()
{
	return &m_fleaManager;
}