/////////////////////////////
// Author: Liam Bansal
// Date Created: 19/05/2020
/////////////////////////////

// Includes
#include "Button.h" // File's header.'
#include "GameState.h"
#include "GameStateManager.h"
#include "MenuState.h"

Button::Button() : m_position(),
	m_text()
{}

Button::~Button()
{}

// Draws the buttons text onto the screen.
void Button::Draw(GameState* a_pState, olc::Pixel a_colour) const
{
	if (a_pState && a_pState->GetManager())
	{
		a_pState->GetManager()->DrawString((int32_t)m_position.GetX(),
			(int32_t)m_position.GetY(),
			m_text,
			a_colour);
	}
}

void Button::SetPosition(Vector2D* a_pPosition)
{
	if (a_pPosition)
	{
		m_position = (*a_pPosition);
	}
}

// Sets the text that the button will display.
void Button::SetText(std::string a_text)
{
	m_text = a_text;
}