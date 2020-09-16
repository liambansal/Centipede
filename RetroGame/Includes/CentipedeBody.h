/////////////////////////////
// Author: Liam Bansal
// Date Created: 03/05/2020
/////////////////////////////

#ifndef CENTIPEDE_BODY_H
#define CENTIPEDE_BODY_H

// Includes.
#include "Animator.h"
#include "Centipede.h"

// Forward Declarations.
class CentipedeHead;
class GameplayState;

class CentipedeBody : public Centipede
{
public:
	// Constructor.
	CentipedeBody();
	// Destructor.
	virtual ~CentipedeBody();

	// Functions.
	// Updates the centipede's position, collisions, animation and spawning.
	void Update(GameplayState* a_pState, 
		Grid* a_pGrid,
		CentipedeBody* m_pBodyIterator,
		float* a_pDeltaTime);
	// Moves the body towards it's first target cell.
	void Move(Grid* a_pGrid, 
		CentipedeBody* a_pForwardBody, 
		float* a_pDeltaTime);
	// Checks for collisions between the body and it's first target 
	// cell.
	void CheckTargetCollision(Grid* a_pGrid);
	// Makes sure there's always a near constant distance between this 
	// and the previous body part.
	void CloseDistance(CentipedeBody* a_pForwardBody,
		float* a_pDeltaTime);
	// Calls to split the centipede at this bodies position.
	void Split(GameplayState* a_pState, Grid* a_pGrid);
	// Sets a new movement direction and target movement destination.
	void SetMoveDirection(Grid* a_pGrid);
	// Returns a list of cells that the body will move towards.
	std::list<Cell*>* GetTargetCells();

private:
	// Variables
	const unsigned int mc_uiMoveSprites;
	const float mc_fMargin;
	Animator m_animator;
	// The cells that we want to move towards.
	std::list<Cell*> m_targetCells;
};

#endif // !CENTIPEDE_BODY_H.