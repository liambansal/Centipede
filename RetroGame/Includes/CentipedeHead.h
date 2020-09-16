/////////////////////////////
// Author: Liam Bansal
// Date Created: 03/05/2020
/////////////////////////////

#ifndef CENTIPEDE_HEAD_H
#define CENTIPEDE_HEAD_H

// Includes.
#include "Animator.h"
#include "Centipede.h"

// Forward Declaration.
class CentipedeBody;
class CentipedeManager;
class GameplayState;

class CentipedeHead : public Centipede
{
public:
	// Constructor.
	CentipedeHead();
	// Destructor.
	virtual ~CentipedeHead();
	// Assignment Operator.
	void operator=(const CentipedeHead* a_rCentipede);

	// Functions.
	// Updates the centipede head & bodies' positions, collisions, animations
	// and deaths.
	void Update(GameplayState* a_pState, 
		Grid* a_pGrid,
		float* a_pDeltaTime);
	// Moves the head towards its target cell and adds it to its bodies' list of
	// destinations.
	void Move(Grid* a_pGrid, float* a_pDeltaTime);
	// Checks if the head has crossed any of the grid's boundaries and 
	// moves it back onto the grid. Also sets a new move direction & target 
	// destination after colliding.
	void CheckBoundaryCollisions(Grid* a_pGrid);
	// Checks for collisions with the target cell and mushrooms.
	void CheckObjectCollision(Grid* a_pGrid);
	// Checks the distance between the head and it's target cell.
	// Sets a new movement course upon colliding.
	void CheckDistance(Grid* a_pGrid);
	// Draws the head onto the screen.
	void Draw(GameplayState* a_pScene) override;
	// Splits the centipede somehwere along it's body creating a new,
	// independant, centipede.
	void Split(Grid* a_pGrid, 
		CentipedeBody* a_pSplitPoint);
	void CreateBody(Grid* a_pGrid,
		unsigned int a_length);
	// Destroys the head and replaces its first body with a new head.
	void DestroyThis(GameplayState* a_pState, 
		Grid* a_pGrid);
	// Handles collisions with the top and bottom grid boundaries.
	void VerticalWallCollision(Grid* a_pGrid);
	// Sets the head's movement course after colliding with a grid's top boundary.
	void TopBoundaryCollide(Grid* a_pGrid);
	// Sets the head's movement course after colliding with a grid's bottom
	// boundary.
	void BottomBoundaryCollide(Grid* a_pGrid);
	void SetSprite(olc::Sprite* a_pSprite);
	// Sets the manager that will update this head.
	void SetManager(CentipedeManager* a_pManager);
	void SetBodyLength(unsigned int a_length);
	// Sets a new movement direction and target movement destination based on 
	// the centipede's previous movement direction.
	void SetMoveDirection(Grid* a_pGrid);
	// Returns a pointer to the list of centipede bodies belonging to this 
	// head.
	std::list<CentipedeBody*>* GetBodyList();

private:
	// Variables.
	const unsigned int mc_uiMoveSprites;
	// Number of centipede bodies spawned behind the head.
	unsigned int m_uiBodyLength;
	// The number of cells collisions to ignore.
	unsigned int m_uiIgnoreCount;

	// Do we want to ignore cell collisions.
	bool m_bIgnoreCollisions;
	bool m_bHasBody;
	// Tells us if the body list been modified.
	bool m_bListChanged;

	// Are we moving up or down the screen.
	Vector2D m_verticalMoveDirection;
	// The previous horizontal direction in which we were moving.
	MOVE_DIRECTIONS m_lastHorizontalDirection;
	// Tells us if we're moving up or down the screen.
	MOVE_DIRECTIONS m_verticalDirection;

	// A list of centipede bodies that follow this head.
	std::list<CentipedeBody*> m_bodyParts;
	// The class that controls the updating and drawing of this head and its 
	// bodies.
	CentipedeManager* m_pManager;
	Animator m_animator;
};

#endif // !CENTIPEDE_HEAD_H.