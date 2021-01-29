/////////////////////////////
// Author: Liam Bansal
// Date Created: 03/05/2020
/////////////////////////////

#ifndef ENEMY_H
#define ENEMY_H

// Includes.
#include "olcPixelGameEngine.h"
#include "Vector2D.h"

// Forward Declaration.
class Cell;
class Grid;
class GameplayState;

class Enemy
{
public:
	// Constructor.
	Enemy();
	// Destructor.
	virtual ~Enemy();

	// Functions.
	// Draws the enemy's sprite onto the screen.
	virtual void Draw(GameplayState* a_pGameplayState);
	// Implemented by inheriting class.
	virtual void UpdateOccupiedCell(Grid* a_pGrid);
	// Decrements the enemies health by one.
	void DecreaseHealth();
	void SetPosition(const Vector2D* a_position);
	// Set a new target cell to move towards.
	virtual void SetTargetCell(Grid* a_pGrid);
	Vector2D* GetCurrentPosition();
	Vector2D* GetPreviousPosition();
	olc::Sprite* GetSprite() const;
	unsigned int GetHealth() const;
	// Gets the cell this enemy is moving towards.
	Cell* GetTargetCell() const;

protected:
	// Variables.
	const unsigned int mc_uiMaxHealth;
	unsigned int m_uiHealth;
	unsigned int m_uiSpeed;
	unsigned int m_uiScoreValue;

	// The enemies travel direction.
	Vector2D m_moveDirection;
	Vector2D m_position;
	// Position of the enemy before it moved out of its occupied cell.
	Vector2D* m_pLastPosition;
	// Position of the previous cell occupied by the enemy.
	Vector2D* m_pPreviousCellsPosition;

	olc::Sprite* m_pSprite;
	olc::Sprite* m_pAnimationSprites[4];

	// The cell that we want to move towards.
	Cell* m_pTargetCell;
};

#endif // !ENEMY_H.