/////////////////////////////
// Author: Liam Bansal
// Date Created: 26/05/2020
/////////////////////////////

#ifndef SPIDER_MANAGER_H
#define SPIDER_MANAGER_H

// Includes.
#include "RandGenerator.h"

// Forward Declarations.
class GameplayState;
class Grid;
class Spider;

class SpiderManager
{
public:
	// Constructor.
	SpiderManager();
	// Destructor.
	~SpiderManager();

	// Functions.
	// Updates the manager's spider and handles when to spawn it.
	void Update(GameplayState* a_pGameplayState,
		Grid* a_pGrid,
		float* a_pDeltaTime);
	// Draws the manager's spider onto the screen.
	void Draw(GameplayState* a_pGameplayState);
	// Spawns a new spider to fight.
	// Only one spider may exist at any given time.
	void SpawnSpider(Grid* a_pGrid);
	// Deallocates the memory for the spider and removes cell references to it, 
	// destroying it in the process.
	void DestroySpider(Grid* a_pGrid);

private:
	// Variables.
	const float mc_fMaxRespawnTime;
	float m_fRespawnTime;
	Spider* m_pSpider;
	RandGenerator randGenerator;
};

#endif // !SPIDER_MANAGER_H.