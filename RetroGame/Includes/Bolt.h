/////////////////////////////
// Author: Liam Bansal
// Date Created: 01/03/2020
/////////////////////////////

#ifndef BOLT_H
#define BOLT_H

// Includes.
#include "olcPixelGameEngine.h"
#include "Vector2D.h"

// Forward declarations.
class GameplayState;
class BugBlaster;
class Grid;

class Bolt
{
public:
	// Constructor.
	Bolt();
	// Destructor.
	~Bolt();

	// Functions.
	// Moves the bolt and updates its collisions.
	void Update(Grid* grid,
		float* a_pDeltaTime,
		int32_t a_screenWidth,
		int32_t a_screenHeight);
	// Draws the bolt onto the screen.
	void Draw(GameplayState* pScene) const;
	// Sets the bolt as ready to fire.
	void Load(BugBlaster* ship);
	// Checks for collisions with objects occupying the cell which the bolt is 
	// traveling through.
	void CheckCollisions(Grid* grid);
	// Respawns the bolt when its position passes one of the grid's boundaries.
	void ClampPosition(Grid* a_pGrid,
		int32_t a_screenWidth,
		int32_t a_screenHeight);
	void SetSprite(olc::Sprite* a_sprite);
	void SetPosition(float a_x, float a_y);
	olc::Sprite* GetSprite() const;
	Vector2D GetPosition() const;

private:
	// Variables.
	float m_fVelocity;
	bool m_bCanFire;
	Vector2D m_spawn;
	Vector2D m_position;
	olc::Sprite* m_pSprite;
};

#endif // BOLT_H.