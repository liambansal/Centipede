/////////////////////////////
// Author: Will Masek
// Date Created: 06/02/2020
/////////////////////////////

#ifndef GAME_STATE_H
#define GAME_STATE_H

// Forward Declarations.
class GameStateManager;
class PixelGameEngine;

class GameState
{
public:
	// Constructor.
	GameState();
	// Destructor.
	virtual ~GameState();

	// Functions.
	// Virtual functions implemented by inheriting classes.
	virtual void Update(float fDeltaTime) = 0;
	virtual void Draw() = 0;
	virtual void ExitState() = 0;
	// Returns if the state it active.
	bool StateOver() const;
	void SetManager(GameStateManager* a_pManager);
	// Sets if the state is active or inactive.
	void SetState(bool a_state);
	GameStateManager* GetManager();
	const GameState* GetGameState() const;

protected:
	// Variables.
	float m_fTimeInState;
	bool m_bIsActive;
	GameStateManager* m_pManager;
};

#endif // !GAME_STATE_H.