/////////////////////////////
// Author: Liam Bansal
// Date Created: 26/05/2020
/////////////////////////////

#ifndef PAUSE_STATE_H
#define PAUSE_STATE_H

// Includes.
#include "GameState.h"
#include <string>

class PauseState : public GameState
{
public: 
	// Constructor.
	PauseState();
	// Destructor.
	virtual ~PauseState();

	// Listens for user input to unpause and exit the state.
	void Update(float a_deltaTime);
	// Draws the pause text in the center of the screen.
	void Draw();
	// Removes the state from its manager and deallocates its memory.
	void ExitState();

private:
	// Variables.
	bool m_bIsPaused;
	// Text to display while game is paused.
	std::string m_pauseText;
};

#endif // !PAUSE_STATE_H.