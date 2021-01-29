/////////////////////////////
// Author: Liam Bansal
// Date Created: 26/05/2020
/////////////////////////////

#ifndef SPIDER_H
#define SPIDER_H

#include "Animator.h"
#include "Enemy.h"

class BugBlaster;
class GameplayState;
class Grid;
class SpiderManager;

class Spider : public Enemy
{
public:
	Spider();
	virtual ~Spider();

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
		bool a_killedByPlayer);
	void SetManager(SpiderManager* a_pManager);
	void SetTargetCell(Grid* a_pGrid) override;
	// Gets the distance to the bug blaster.
	float GetDistance(BugBlaster* a_pBugBlaster);

private:
	enum VERTICAL_DIRECTION
	{
		UP,
		DOWN,
		COUNT
	};

	const unsigned int mc_uiMoveSprites;
	unsigned int m_uiScores[3];
	float m_fDistances[3];
	Animator m_animator;
	SpiderManager* m_pSpiderManager;
	VERTICAL_DIRECTION m_verticalDirection;
};

#endif // !SPIDER_H.