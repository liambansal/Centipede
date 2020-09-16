/////////////////////////////
// Author: Liam Bansal
// Date Created: 19/05/2020
/////////////////////////////

// Includes.
#include "MenuState.h" // File's header.
#include "Clamp.h"
#include "GameplayState.h"
#include "GameStateManager.h"
#include "Vector2D.h"

// Namespaces.
using namespace olc;

MenuState::MenuState() : GameState(),
	m_uiSelectedOption(0),
	m_options()
{
	m_bIsActive = true;
}

MenuState::~MenuState()
{}

// Sets up the menu wit hbuttons and a manager.
void MenuState::Initialize()
{
	if (m_pManager)
	{
		m_score.SetStateManager(m_pManager);
		Vector2D drawPosition((float)m_pManager->GetDrawTargetWidth() - 100.0f,
			0.0f);
		m_score.SetPosition(&drawPosition);
		m_options[0].SetText("Play");
		m_options[0].SetPosition(&Vector2D((float)m_pManager->GetDrawTargetWidth() / 2,
			(float)m_pManager->GetDrawTargetHeight() / 2));
		m_options[1].SetText("Quit");
		unsigned int buttonHeight = 10;
		m_options[1].SetPosition(&Vector2D((float)m_pManager->GetDrawTargetWidth() / 2,
			(float)m_pManager->GetDrawTargetHeight() / 2 + buttonHeight));
	}
}

// Checks if any buttons are pressed and executes their commands.
void MenuState::Update(float a_deltaTime)
{
	if (m_bIsActive && m_pManager)
	{
		m_fTimeInState += a_deltaTime;

		// Should we quit?
		if (m_pManager->GetKey(olc::Key::ESCAPE).bPressed)
		{
			Quit();
		}

		if (m_pManager->GetKey(Key::W).bPressed) // Move up.
		{
			--m_uiSelectedOption;
		}

		if (m_pManager->GetKey(Key::S).bPressed) // Move down.
		{
			++m_uiSelectedOption;
		}

		// Clamp the value used for selection menu options so players can only 
		// select what's there.
		Clamp clamp;
		unsigned int arrayLength = sizeof(m_options) / sizeof(*m_options);
		clamp.ClampInt((unsigned int*)&m_uiSelectedOption, 0, --arrayLength);

		if (m_pManager->GetKey(Key::ENTER).bPressed) // Select option.
		{
			if (m_uiSelectedOption == BUTTONS_PLAY)
			{
				ExitState();
			}
			else if (m_uiSelectedOption == BUTTONS_QUIT)
			{
				Quit();
			}
		}
	}
}

// Draws the menus buttons and score.
void MenuState::Draw()
{
	if (m_pManager)
	{
		m_pManager->Clear(olc::BLACK);
		unsigned int arrayLength = sizeof(m_options) / sizeof(*m_options);

		// Draw each menu button.
		for (unsigned int option = 0; option < arrayLength; ++option)
		{
			if (&m_options[option] == &m_options[m_uiSelectedOption])
			{
				m_options[m_uiSelectedOption].Draw(this, olc::YELLOW);
			}
			else
			{
				m_options[option].Draw(this, olc::WHITE);
			}
		}

		m_score.Draw();
	}
}

// Exits this state and enters a gameplay state.
void MenuState::ExitState()
{
	if (m_pManager)
	{
		GameplayState* gameplayState = new GameplayState();
		gameplayState->SetManager(m_pManager);
		gameplayState->Initialize(this);
		m_pManager->PushState(gameplayState);
		m_fTimeInState = 0.0f;
		m_pManager->StatesChanged(true);
	}
}

// Calling will close the application.
void MenuState::Quit()
{
	if (m_pManager)
	{
		m_fTimeInState = 0.0f;
		m_pManager->Quit();
	}
}

Score* MenuState::GetScore()
{
	return &m_score;
}