/////////////////////////////
// Author: Liam Bansal
// Date Created: 19/05/2020
/////////////////////////////

#ifndef SPLASH_STATE_H
#define SPLASH_STATE_H

// Includes.
#include "GameState.h"
#include <string>

class SplashState : public GameState
{
public:
	// Constructor.
	SplashState();
	// Destructor.
	virtual ~SplashState();

	// Functions.
	// Increases the time spent in this state and exits after a set time.
	void Update(float fDeltaTime);
	// Draws the splash text to the center of the screen.
	void Draw();
	// Removes the state from its manager, exiting it, and enters the menu
	// state.
	void ExitState();

private:
	// Variables.
	unsigned int m_xPosition;
	unsigned int m_yPosition;
	float m_fSplashLength;
	std::string m_splashText;
};

#endif // !SPLASH_STATE_H.