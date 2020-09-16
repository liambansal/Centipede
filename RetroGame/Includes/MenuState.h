/////////////////////////////
// Author: Liam Bansal
// Date Created: 19/05/2020
/////////////////////////////

#ifndef MENU_STATE_H
#define MENU_STATE_H

// Includes.
#include "Button.h"
#include "GameState.h"
#include "Score.h"

class MenuState : public GameState
{
public:
	// Constructor.
	MenuState();
	// Destructor.
	virtual ~MenuState();

	// Functions.
	// Sets up the menu wit hbuttons and a manager.
	void Initialize();
	// Checks if any buttons are pressed and executes their commands.
	void Update(float fDeltaTime) override;
	// Draws the menus buttons and score.
	void Draw();
	// Exits this state and enters a gameplay state.
	void ExitState();
	// Calling will close the application.
	void Quit();
	Score* GetScore();

private:
	//Variables
	unsigned int m_uiSelectedOption;
	Button m_options[2];
	Score m_score;

	enum BUTTONS
	{
		BUTTONS_PLAY = 0,
		BUTTONS_QUIT = 1,

		BUTTONS_TWO // Total states in the enum.
	};
};

#endif // !MENU_STATE_H.