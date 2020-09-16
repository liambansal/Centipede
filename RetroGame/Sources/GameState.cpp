/////////////////////////////
// Author: Will Masek
// Date Created: 06/02/2020
/////////////////////////////

// Includes.
#include "GameState.h" // File's Header.
#include "olcPixelGameEngine.h"

GameState::GameState() : m_fTimeInState(0.0f),
	m_bIsActive(false),
	m_pManager(nullptr)
{}

GameState::~GameState()
{}

// Returns if the state it active.
bool GameState::StateOver() const
{
	return m_bIsActive;
}

void GameState::SetManager(GameStateManager* a_pManager)
{
	m_pManager = a_pManager;
}

// Sets if the state is active or inactive.
void GameState::SetState(bool a_state)
{
	m_bIsActive = a_state;
}

GameStateManager* GameState::GetManager()
{
	return m_pManager;
}

const GameState* GameState::GetGameState() const
{
	return this;
}