/////////////////////////////
// Author: Liam Bansal
// Date Created: 19/05/2020
/////////////////////////////

// Includes.
#include "SplashState.h" // File's header.
#include "MenuState.h"
#include "GameState.h"
#include "GameStateManager.h"

SplashState::SplashState() : GameState(),
	m_xPosition(60),
	m_yPosition(120),
	m_fSplashLength(2.0f),
	m_splashText("Made By Liam Bansal")
{
	m_bIsActive = true;
}

SplashState::~SplashState()
{}

// Increases the time spent in this state and exits after a set time.
void SplashState::Update(float a_deltaTime)
{
	if (m_bIsActive)
	{
		m_fTimeInState += a_deltaTime;
		
		if (m_fTimeInState >= m_fSplashLength)
		{
			m_bIsActive = false;
		}
	}
	else
	{
		ExitState();
	}
}

// Draws the splash text to the center of the screen.
void SplashState::Draw()
{
	if (m_pManager)
	{
		// Draw the splash screen text.
		m_pManager->DrawString((int32_t)m_xPosition,
			(int32_t)m_yPosition,
			m_splashText,
			olc::WHITE);
	}
}

// Removes the state from its manager, exiting it, and enters the menu
// state.
void SplashState::ExitState()
{
	if (m_pManager)
	{
		MenuState* pMenuState = new MenuState();
		pMenuState->SetManager(m_pManager);
		pMenuState->Initialize();
		m_pManager->PushState(pMenuState);
		m_fTimeInState = 0.0f;
		m_bIsActive = false;
		m_pManager->StatesChanged(true);
		m_pManager->RemoveState(this);
	}
}