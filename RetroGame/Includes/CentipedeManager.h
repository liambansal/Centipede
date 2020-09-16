/////////////////////////////
// Author: Liam Bansal
// Date Created: 14/05/2020
/////////////////////////////

#ifndef CENTIPEDE_MANAGER_H
#define CENTIPEDE_MANAGER_H

// Includes.
#include "CentipedeHead.h"
#include <list>
#include "RandGenerator.h"

// Forward Declarations.
class CentipedeBody;
class GameplayState;
class Grid;
class Vector2D;

class CentipedeManager
{
public:
	// Constructor.
	CentipedeManager();
	// Destructor.
	~CentipedeManager();

	// Functions.
	// Updates the manager's centipedes and when to spawn them.
	void Update(GameplayState* a_pState, 
		Grid* a_pGrid,
		float* a_pDeltaTime);
	// Draws the manager's centipedes onto the screen.
	void Draw(GameplayState* a_pScene);
	// Spawn's the level's main centipede.
	// Spawns additional heads when body centipede length decreases.
	void SpawnCentipede(Grid* a_pGrid);
	// Creates a new centipede and assigns it the split body of another
	// centipede.
	void CreateCentipede(Grid* a_pGrid,
		std::list<CentipedeBody*>* a_pBodyList);
	// Removes a centipede head from the manager's list of centipedes.
	void RemoveCentipede(CentipedeHead* a_pHead);
	// Clears all of the manager's centipedes.
	void ClearCentipedes(Grid* a_pGrid);
	// Loops through a centipede's bodies and removes any target cells whose 
	// positions are outisde of the centipede.
	void EraseTargetCells(CentipedeHead* a_pHead);

private:
	// Variables.
	const unsigned int m_uiMaxCentipedeLength;
	unsigned int m_uiCentipedeLength;
	// Have all of the state's centipedes been slain?
	bool m_bCentipedesSlain;
	// Have either of the centipede lists been modified?
	bool m_bListChanged;

	// A list containing all of the centipede's in the scene.
	std::list<CentipedeHead*> m_centipedes;
	std::list<CentipedeHead> m_centipedeHeads;
	RandGenerator m_numberGenerator;
};

#endif // !CENTIPEDE_MANAGER_H