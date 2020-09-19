/////////////////////////////
// Author: Liam Bansal
// Date Created: 16/09/2020
/////////////////////////////

#ifndef POPUPTEXT_H
#define POPUPTEXT_H

// Includes.
#include "GameState.h"
#include "olcPixelGameEngine.h"
#include "Vector2D.h"

class PopUpText
{
public:
	// Constructor
	PopUpText(std::string a_text, Vector2D a_enemyPosition);
	// Destructor
	~PopUpText();

	// Functions
	void Draw(GameState* a_pGameState);
	void Update(float a_fDeltaTime);
	bool IsActive();

private:
	unsigned int m_uiDriftSpeed;
	float m_fDisplayLength;
	bool m_bDestroy;
	std::string m_text;
	Vector2D m_position;
};

#endif POPUPTEXT_H