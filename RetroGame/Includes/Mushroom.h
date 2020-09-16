/////////////////////////////
// Author: Liam Bansal
// Date Created: 07/04/2020
/////////////////////////////

#ifndef MUSHROOM_H
#define MUSHROOM_H

// Includes.
#include "olcPixelGameEngine.h"
#include "Vector2D.h"

// Forward Declarations.
class BugBlaster;
class Cell;
class Grid;
class GameplayState;

class Mushroom
{
public:
	// Constructor.
	Mushroom();
	// Destructor.
	~Mushroom();

	// Functions.
	// Updates the mushroom based on its health.
	void Update(GameplayState* a_pState,
		Grid* a_pGrid,
		Cell& cell,
		BugBlaster& bugBlaster);
	void DecreaseHealth();
	void Destroy(GameplayState* a_pState,
		Grid* a_pGrid,
		Cell& cell,
		BugBlaster& bugBlaster) const;
	void SetPosition(float x, float y);
	void SetPosition(const Vector2D* newPosition);
	void SetSprite(olc::Sprite* newSprite);
	// Sets whether or not the mushroom has been counted for score.
	void SetCountState(bool a_bool);
	Vector2D GetPosition() const;
	olc::Sprite* GetSprite() const;
	// Returns whether or not the mushroom has been counted for score.
	bool GetCountState() const;

private:
	const unsigned int mc_uiWidth;
	const unsigned int mc_uiHeight;
	const unsigned int mc_uiScoreValue;
	unsigned int m_uiMaxHealth;
	unsigned int m_uiHealth; // Start counting from 0. Used to access sprite 
	// array. So 0 = 1st sprite / 1 hp.
	// Has this mushroom already been counted for score.
	bool m_bCounted;

	Vector2D m_position;
	olc::Sprite* m_currentSprite;
	olc::Sprite* m_sprites[4];
};

#endif // MUSHROOM_H.