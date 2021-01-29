/////////////////////////////
// Author: Liam Bansal
// Date Created: 19/05/2020
/////////////////////////////

// Includes.
#include "GameStateManager.h" // File's header.
#include "SplashState.h"

GameStateManager::GameStateManager() : m_states(),
	m_bStatesChanged(false),
	m_bExitGame(false),
	m_timer()
{
	sAppName = "Centipede";
}

GameStateManager::~GameStateManager()
{
	for (auto listIterator = m_states.begin();
		listIterator != m_states.end();
		++listIterator)
	{
		if (*listIterator)
		{
			delete (*listIterator);
			(*listIterator) = nullptr;
		}
	}
}

// Constructs the window application.
void GameStateManager::Initialize()
{
	unsigned int screenWidth = 256;
	unsigned int screenHeight = 240;
	unsigned int pixelWidth = 4;
	unsigned int pixelHeight = 4;
	bool isFullscreen = true;

	if (Construct(screenWidth,
		screenHeight,
		pixelWidth,
		pixelHeight,
		isFullscreen))
	{
		Start();
	}
}

// Called once at the start of the application.
// Creates a splash screen state.
bool GameStateManager::OnUserCreate()
{
	SplashState* splashState = new SplashState();
	splashState->SetManager(this);
	PushState(splashState);
	return true;
}

// Called once per frame.
// Override updates and draws the content of the state at the 
// back of the state list.
bool GameStateManager::OnUserUpdate(float a_deltaTime)
{
	if (m_states.size() > 0)
	{
		m_timer.Update(&a_deltaTime);

		if (!m_bStatesChanged &&
			m_states.back())
		{
			m_states.back()->Update(a_deltaTime);
		}

		// Check if the player has requested to exit the game.
		if (m_bExitGame)
		{
			return false;
		}

		Draw();
		m_bStatesChanged = false;
		return true;
	}
	else
	{
		return false;
	}
}

// Draws the contents of the state at the back of the states list.
void GameStateManager::Draw() const
{
	if (!m_bStatesChanged &&
		m_states.back())
	{
		m_states.back()->Draw();
	}
}

void GameStateManager::StatesChanged(bool a_bool)
{
	m_bStatesChanged = a_bool;
}

// Closes the application.
void GameStateManager::Quit()
{
	m_bExitGame = true;
	m_states.clear();
}

// Pushes a state onto the back of the state list.
void GameStateManager::PushState(GameState* a_pState)
{
	m_states.push_back(a_pState);
}

// Remove the state at the back of the state list.
void GameStateManager::PopState()
{
	// Check a state to pop exists.
	if (m_states.size() > 0)
	{
		// Remove the last state from the stack.
		m_states.pop_back();
	}
}

// Removes a state from anywhere within the state list.
void GameStateManager::RemoveState(GameState* a_pState)
{
	for (auto listIterator = m_states.begin();
		listIterator != m_states.end();
		++listIterator)
	{
		if (*listIterator == a_pState)
		{
			m_states.erase(listIterator);
			delete a_pState;
			a_pState = nullptr;
			break;
		}
	}
}

const std::list<GameState*>* GameStateManager::GetStates() const
{
	return &m_states;
}

const Timer* GameStateManager::GetTimer() const
{
	return &m_timer;
}