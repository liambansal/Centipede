/////////////////////////////
// Author: Liam Bansal
// Date Created: 26/05/2020
/////////////////////////////

#ifndef FLEA_H
#define FLEA_H

// Includes.
#include "Animator.h"
#include "Enemy.h"

// Forward Declarations.
class FleaManager;
class GameplayState;

class Flea : public Enemy
{
public:
	// Constructor.
	Flea();
	// Destructor.
	virtual ~Flea();

	// Functions.
	// Updates the fleas movement, animation, collisions and death.
	void Update(GameplayState* a_pGameplayState,
		Grid* a_pGrid,
		float a_deltaTime);
	// Moves the flea towards it's target cell.
	void Move(Grid* a_pGrid, float* a_pDeltaTime);
	// Checks for collisions with a grid's uppder & lower boundaries.
	void CheckCollisions(GameplayState* a_pGameplayState,
		Grid* a_pGrid);
	// Updates the flea's currently and previously occupied cell positions.
	// Chance to spawn mushrooms in new cells.
	void UpdateOccupiedCell(Grid* a_pGrid) override;
	void Destroy(Grid* a_pGrid);
	void SetManager(FleaManager* a_pFleaManager);

private:
	// Variables.
	const unsigned int mc_uiMoveSprites;
	const float mc_fMushroomSpawnChance;
	Animator m_animator;
	FleaManager* m_pFleaManager;
};

#endif // !FLEA_H.