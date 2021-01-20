/////////////////////////////
// Author: Will Masek
// Date Created: 07/02/2020
/////////////////////////////

#ifndef BUG_BLASTER_H
#define BUG_BLASTER_H

// Includes.
#include "Animator.h"
#include "Bolt.h"
#include "Cell.h"
#include "olcPixelGameEngine.h"
#include <string>
#include "Vector2D.h"

// Forward declarations.
class Grid;
class GameplayState;

class BugBlaster
{
public:
	// Constructor.
	BugBlaster(Grid* a_pGrid);
	// Destructor.
	~BugBlaster();

	// Functions.
	// Updates the bug blasters behaviour and how its displayed.
	void Update(GameplayState* a_pState,
		float* a_pDeltaTime,
		Grid* a_pGrid);
	// Listens for and reacts to input for moving and firing the bug blaster.
	void HandleInput(GameplayState* a_pState, float* a_pDeltaTime);
	void DrawSprite(GameplayState* a_pState) const;
	void DrawBolts(GameplayState* a_pState) const;
	// Draws the player's remaining lives onto the screen.
	void DrawLives(GameplayState* a_pState) const;
	// Keeps the bug blasters position inside the grid's boundaries.
	void ClampPosition(Grid* a_pGrid);
	// Fires a singular bolt from the bug blasters position.
	void Fire(GameplayState* a_pState, float* a_pDeltaTime);
	// Checks for collisions with objects in cell's adjacent the bug blaster.
	// and handles different collision events.
	void CheckForCollisions(Grid* a_pGrid);
	// Calculates a reload time based on the distance of the closest object 
	// along the bug blaster's y axis.
	void CalculateFireRate(Grid* a_pGrid);
	// Gives the player an extra life.
	void AddLife();
	// Respawns the bug blaster and resets the scene.
	void Respawn(GameplayState* a_pState, Grid* a_pGrid);
	void SetSprite(olc::Sprite* a_sprite);
	void SetPosition(Vector2D a_position);
	olc::Sprite* GetSprite() const;
	const Vector2D GetPosition() const;
	Bolt& GetBolt();

private:
	// Variables.
	const unsigned int mc_uiMoveSprites;
	const unsigned int mc_uiFireSprites;
	const unsigned int mc_uiDeathSprites;
	const unsigned int mc_uiMaxLives;
	unsigned int m_uiLives;
	unsigned int m_uiBoltCounter;

	float m_fMovementSpeed;
	// The maximum time remaining before the player can shoot.
	float m_fMaxReloadTime;
	// The current time remaining before the player can shoot.
	float m_fReloadTime;

	bool m_bCanFire;
	bool m_bIsAlive;

	const char* mc_cpMoveSprite;
	const char* mc_cpFiredSprite;
	const char* mc_cpDeathAnimation;
	std::string m_livesPrefix;

	Vector2D m_spawnPosition;
	Vector2D m_position;
	olc::Sprite* m_pSprite;
	Animator m_animator;
	const Cell* m_pAdjacentCells[8] = { new Cell(), new Cell(), new Cell(), new Cell(), new Cell(), new Cell(), new Cell(), new Cell() };
	Bolt m_bolts[20] = { Bolt(), Bolt(), Bolt(), Bolt(), Bolt(),
	Bolt(), Bolt(), Bolt(), Bolt(), Bolt(),
	Bolt(), Bolt(), Bolt(), Bolt(), Bolt(),
	Bolt(), Bolt(), Bolt(), Bolt(), Bolt() };
};

#endif //!BUG_BLASTER_H.