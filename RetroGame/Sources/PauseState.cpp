/////////////////////////////
// Author: Liam Bansal
// Date Created: 26/05/2020
/////////////////////////////

// Includes.
#include "PauseState.h" // File's header.
#include "GameStatemanager.h"
#include "Vector2D.h"

PauseState::PauseState() : GameState(),
	m_pauseText("Paused"),
	m_bIsPaused(true)
{
	m_bIsActive = true;
}

PauseState::~PauseState()
{}

// Listens for user input to unpause and exit the state.
void PauseState::Update(float a_deltaTime)
{
	if (m_bIsActive)
	{
		m_fTimeInState += a_deltaTime;

		if (m_bIsPaused)
		{
			if (m_pManager && m_pManager->GetKey(olc::ESCAPE).bPressed)
			{
				m_bIsPaused = false;
			}
		}
		else
		{
			ExitState();
		}
	}
}

// Draws the pause text in the center of the screen.
void PauseState::Draw()
{
	if (m_bIsActive && m_pManager)
	{
		int32_t xPosition = m_pManager->GetDrawTargetWidth() / 2;
		int32_t yPosition = m_pManager->GetDrawTargetHeight() / 2;
		// Draws the pause text at the center of the screen.
		m_pManager->DrawString(xPosition,
			yPosition,
			m_pauseText,
			olc::WHITE);
	}
}

// Removes the state from its manager and deallocates its memory.
void PauseState::ExitState()
{
	if (m_pManager)
	{
		m_bIsActive = false;
		m_pManager->StatesChanged(true);
		m_pManager->RemoveState(this);
	}
}