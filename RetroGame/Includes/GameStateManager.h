/////////////////////////////
// Author: Liam Bansal
// Date Created: 19/05/2020
/////////////////////////////

#ifndef GAME_STATE_MANAGER_H
#define GAME_STATE_MANAGER_H

// Includes.
#include <list>
#include "olcPixelGameEngine.h"
#include "Timer.h"

// Forward Declaration.
class GameState;

class GameStateManager : public olc::PixelGameEngine
{
public:
	// Constructor.
	GameStateManager();
	// Destructor.
	virtual ~GameStateManager();

	// Functions.
	// Constructs the window application.
	void Initialize();
	// Called once at the start of the application.
	// Creates a splash screen state.
	bool OnUserCreate() override;
	// Called once per frame.
	// Override updates and draws the content of the state at the
	// back of the state list.
	bool OnUserUpdate(float fDeltaTime) override;
	// Draws the contents of the state at the back of the states list.
	void Draw() const;
	void StatesChanged(bool a_bool);
	// Will close the application.
	void Quit();
	// Pushes a state onto the back of the state list.
	void PushState(GameState* a_pState);
	// Remove the state at the back of the state list.
	void PopState();
	// Removes a state from anywhere within the state list.
	void RemoveState(GameState* a_pState);
	const std::list<GameState*>* GetStates() const;
	const Timer* GetTimer() const;

private:
	// Variables.
	bool m_bStatesChanged;
	bool m_bExitGame;
	std::list<GameState*> m_states;
	Timer m_timer;
};

#endif // !GAME_STATE_MANAGER_H.