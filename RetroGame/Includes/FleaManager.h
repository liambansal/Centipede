/////////////////////////////
// Author: Liam Bansal
// Date Created: 26/05/2020
/////////////////////////////

#ifndef FLEA_MANAGER_H
#define FLEA_MANAGER_H

// Includes.
#include "RandGenerator.h"

// Forward Declarations.
class Flea;
class GameplayState;
class Grid;

class FleaManager
{
public:
	// Constructor.
	FleaManager();
	// Destructor.
	~FleaManager();

	// Functions.
	// Updates the manager's flea and handles when to spawn it.
	void Update(GameplayState* a_pGameplayState,
		Grid* a_pGrid,
		float* a_pDeltaTime);
	// Draws the manager's flea.
	void Draw(GameplayState* a_pGameplayState);
	// Spawns a flea along the grid's top boundary somehwat randomly between 
	// the left and right boundaries.
	void SpawnFlea(Grid* a_pGrid);
	void DestroyFlea(Grid* a_pGrid);
	// Increments the counter tracking the number of mushrooms destroyed by 
	// the player but only if no fleas exist.
	void MushroomDestroyed();

private:
	// Variables.
	// The maximum possible number of mushrooms needed to be destroyed before 
	// a flea is spawned.
	const unsigned int m_uiMaxDestroyCount;
	// The current number of mushrooms needed to be destroyed before a flea is
	// spawned.
	unsigned int m_uiDestroyCount;
	unsigned int m_uiMushroomsDestroyed;
	const float m_fMaxRespawnTime;
	float m_fRespawnTime;
	Flea* m_pFlea;
	RandGenerator m_randGenerator;
};

#endif // !FLEA_MANAGER_H.