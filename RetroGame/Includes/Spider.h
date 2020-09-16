/////////////////////////////
// Author: Liam Bansal
// Date Created: 26/05/2020
/////////////////////////////

#ifndef SPIDER_H
#define SPIDER_H

// Includes
#include "Animator.h"
#include "Enemy.h"

// Forward Declarations.
class BugBlaster;
class GameplayState;
class Grid;
class SpiderManager;

class Spider : public Enemy
{
public:
	// Constructor.
	Spider();
	// Destructor.
	virtual ~Spider();

	// Functions.
	// Updates the spider's movement, collisions, animation and death.
	void Update(GameplayState* a_pGameplayState,
		Grid* a_pGrid,
		float* a_pDeltaTime);
	// Moves the spider towards it's target cell.
	void Move(Grid* a_pGrid, float* a_pDeltaTime);
	// Checks for collisions with the target cell and grid's boundaries.
	void CheckCollisions(GameplayState* a_pGameplayState,
		Grid* a_pGrid);
	// Updates the spider's currently and previosuly occupied cell positions.
	// Clears mushrooms in the spider's current cell.
	void UpdateOccupiedCell(Grid* a_pGrid) override;
	// Destroys the spider and rewards the player with points if they killed it.
	void Destroy(GameplayState* a_pGameplayState,
		Grid* a_pGrid,
		bool a_bool);
	void SetManager(SpiderManager* a_pManager);
	// Gets the distance to the bug blaster.
	float GetDistance(BugBlaster* a_pBugBlaster);

private:
	// Variables.
	const unsigned int mc_uiMoveSprites;
	unsigned int m_uiScores[3];
	float m_fDistances[3];
	Animator m_animator;
	SpiderManager* m_pSpiderManager;
};

#endif // !SPIDER_H.