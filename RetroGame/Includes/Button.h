/////////////////////////////
// Author: Liam Bansal
// Date Created: 19/05/2020
/////////////////////////////

#ifndef BUTTON_H
#define BUTTON_H

// Includes.
#include "olcPixelGameEngine.h"
#include <string>
#include "Vector2D.h"

// Forward Declarations.
class GameState;
class MenuState;

class Button
{
public:
	// Constructor.
	Button();
	// Destructor.
	~Button();

	// Functions.
	// Draws the buttons text onto the screen.
	void Draw(GameState* a_pState, olc::Pixel a_colour) const;
	void SetPosition(Vector2D* a_pPosition);
	// Sets the text that the button will display.
	void SetText(std::string a_text);

private:
	// Variables.
	Vector2D m_position;
	// The button text to display on screen.
	std::string m_text;
};

#endif // !BUTTON_H.