/////////////////////////////
// Author: Liam Bansal
// Date Created: 19/05/2020
/////////////////////////////

#ifndef GAMEPLAY_STATE_H
#define GAMEPLAY_STATE_H

// Includes.
#include "CentipedeManager.h"
#include "FleaManager.h"
#include "GameState.h"
#include "SpiderManager.h"

// Forward declarations
class BugBlaster;
class Grid;
class MenuState;
class PopUpText;
class Score;

class GameplayState : public GameState
{
public:
	// Constructor.
	GameplayState();
	// Destructor.
	virtual ~GameplayState();

	// Functions.
	// Sets up the gameplay state with its grid, bug blaster and enemy managers.
	void Initialize(MenuState* a_pMenuState);
	// Updates the grid, bug blaster, enemy managers and hanldes input for
	// pausing. Call once per frame.
	void Update(float fDeltaTime);
	// Draws the state's enemies, bug blaster, score and grid.
	void Draw();
	void ExitState();
	void AddScore(PopUpText a_text);
	Grid* GetGrid() const;
	Score* GetScore() const;
	BugBlaster* GetBugBlaster() const;
	CentipedeManager* GetCentipedeManager();
	SpiderManager* GetSpiderManager();
	FleaManager* GetFleaManager();

private:
	// Variables.
	BugBlaster* m_pBugBlaster;
	Grid* m_pGrid;
	Score* m_pScore;
	CentipedeManager m_centipedeManager;
	SpiderManager m_spiderManager;
	FleaManager m_fleaManager;
	std::list<PopUpText> m_scores;
};

#endif // !GAMEPLAY_STATE_H.