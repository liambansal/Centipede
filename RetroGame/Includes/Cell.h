/////////////////////////////
// Author: Liam Bansal
// Date Created: 04/03/2020
/////////////////////////////

#ifndef CELL_H
#define CELL_H

// Includes.
#include "olcPixelGameEngine.h"
#include <string>
#include "Vector2D.h"

// Forward Declarations.
class BugBlaster;
class Centipede;
class CentipedeHead;
class Flea;
class GameplayState;
class Grid;
class Mushroom;
class Spider;

class Cell
{
public:
	// Constructor.
	Cell();
	// Destructor.
	~Cell();

	// Functions
	// Updates the mushroom occupying the cell.
	void Update(GameplayState* a_pState,
		Grid* a_pGrid,
		BugBlaster& a_rBugBlaster);
	// Draws the cell's mushroom if present otherwise fills the cell with black
	// pixels.
	void Draw(GameplayState* a_pState, Grid* a_pGrid);
	void SpawnMushroom();
	void ClearMushroom();
	// Clears the cell's pointer to a centipede.
	void ClearCentipede();
	// Clears the cell's pointer to a spider.
	void ClearSpider();
	// Clears the cell's pointer to a flea.
	void ClearFlea();
	// Tags the cell as "Mushroom" if one is present otherwise clears the tag 
	// for what was occupying the cell.
	void ClearTag();
	// Compares a centipede head with the one occupying this cell.
	bool CompareCentipede(CentipedeHead* a_pEnemy);
	void SetPosition(float x, float y);
	// Sets a tag for what's occupying the cell.
	void SetTag(std::string a_tag);
	// Sets a pointer to the centipede occupying the cell.
	void SetCentipede(CentipedeHead* a_pCentipedeHead, 
		Centipede* a_pCentipede);
	// Sets a pointer to the spider occupying the cell.
	void SetSpider(Spider* a_pSpider);
	// Sets a pointer to the flea occupying the cell.
	void SetFlea(Flea* a_pFlea);
	const unsigned int GetWidth() const;
	const unsigned int GetHeight() const;
	const Vector2D* GetPosition() const;
	// Gets the tag for what's occupying the cell.
	std::string GetTag() const;
	// Returns the cell's mushroom if one is present.
	Mushroom* GetMushroom();
	// Returns the cell's centipede head if one is present.
	CentipedeHead* GetCentipedeHead() const;
	// Returns the cell's centipede if one is present.
	Centipede* GetCentipede() const;
	// Returns the cell's spider if one is present.
	Spider* GetSpider() const;
	// Returns the cell's flea if one is present.
	Flea* GetFlea() const;

private:
	// Variables.
	const unsigned int mc_uiWidth;
	const unsigned int mc_uiHeight;
	bool m_bOccupied;

	std::string m_sTag;
	Vector2D m_position;
	Mushroom* m_pMushroom;
	// Points to the centipede occupying this cell.
	Centipede* m_pCentipede;
	// Points to the centipede head occupying this cell.
	CentipedeHead* m_pCentipedeHead;
	Spider* m_pSpider;
	Flea* m_pFlea;
};

#endif // CELL_H.