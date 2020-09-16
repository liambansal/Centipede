/////////////////////////////
// Author: Liam Bansal
// Date Created: 14/05/2020
/////////////////////////////

#ifndef CENTIPEDE_H
#define CENTIPEDE_H

// Includes.
#include "Enemy.h"

// Forward Declarations.
class CentipedeHead;
class GameplayState;
class Grid;
class Vector2D;

// Enum for switching between the different directions in which the 
// centiupede can move.
enum class MOVE_DIRECTIONS
{
	MOVE_DIRECTIONS_UP,
	MOVE_DIRECTIONS_LEFT,
	MOVE_DIRECTIONS_DOWN,
	MOVE_DIRECTIONS_RIGHT,

	MOVE_DIRECTIONS_FOUR // Total number of enum states.
};

// Contains the generic behaviour and properties for the centipede enemy.
// Inhereted by the head and body.
class Centipede : public Enemy
{
public:
	// Constructor.
	Centipede();
	// Destructor.
	virtual ~Centipede();
	// Assignment Operator.
	void operator=(const Centipede* a_rCentipede);

	// Functions.
	// Draws a centipedes part onto the screen.
	virtual void Draw(GameplayState* a_pScene);
	// Updates the centipede part's currently & previously occupied cell positions.
	void UpdateOccupiedCell(Grid* a_pGrid) override;
	// Sets the head which this centipede will follow.
	void SetHead(CentipedeHead* a_head);
	// Returns the centipede part's current movement direction.
	Vector2D* GetMoveDirection();
	// Returns the direction in which the centipede checks for collisions.
	MOVE_DIRECTIONS* GetForwardDirection();
	// Returns the position of the centipede part's previously occupied cell.
	Vector2D* GetLastCellPosition();
	// Returns a pointer to the centipede's head.
	CentipedeHead* GetHead();
	// Returns the centipede part's parent class.
	Centipede* GetParent();

protected:
	// Variables.
	Centipede& m_pParent;
	// Reference to the head of the centipede.
	CentipedeHead* m_pHead;
	MOVE_DIRECTIONS m_currentDirection;
};

#endif // !CENTIPEDE_H.