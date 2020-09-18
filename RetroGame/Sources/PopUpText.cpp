/////////////////////////////
// Author: Liam Bansal
// Date Created: 16/09/2020
/////////////////////////////

// Includes.
#include "PopUptext.h" // File's header.
#include "GameStateManager.h"

// Constructor
PopUpText::PopUpText() : m_uiDriftSpeed(4),
m_fDisplayLength(1.5f),
m_bDestroy(false),
m_position()
{}

PopUpText::PopUpText(std::string a_text, Vector2D a_enemyPosition) : m_uiDriftSpeed(4),
m_fDisplayLength(1.5f),
m_bDestroy(false)
{
	m_text = a_text;
	m_position = a_enemyPosition;
}

// Destructor
PopUpText::~PopUpText()
{}

void PopUpText::Draw(GameState* a_pGameState)
{
	if (a_pGameState)
	{
		a_pGameState->GetManager()->DrawString(m_position.GetX(),
			m_position.GetY(),
			m_text,
			olc::WHITE);
	}
}

void PopUpText::Update(float a_fDeltaTime)
{
	if (m_fDisplayLength <= 0)
	{
		m_bDestroy = true;
	}
	else
	{
		m_fDisplayLength -= a_fDeltaTime;
		m_position -= m_position.Up() * m_uiDriftSpeed * a_fDeltaTime;
	}
}

bool PopUpText::IsActive()
{
	if (m_bDestroy)
	{
		return false;
	}
	else
	{
		return true;
	}
}