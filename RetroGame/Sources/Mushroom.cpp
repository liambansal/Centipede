/////////////////////////////
// Author: Liam Bansal
// Date Created: 07/04/2020
/////////////////////////////

// Includes.
#include "Mushroom.h" // File's header.
#include "BugBlaster.h"
#include "Cell.h"
#include "Grid.h"
#include "GameplayState.h"
#include "Score.h"

Mushroom::Mushroom() : mc_uiWidth(8),
	mc_uiHeight(8),
	mc_uiScoreValue(1),
	m_uiMaxHealth(3),
	m_uiHealth(m_uiMaxHealth),
	m_bCounted(false),
	m_position(),
	m_sprites()
{
	unsigned int spriteCount = 4;

	for (unsigned int i = 0; i < spriteCount; ++i)
	{
		std::string filePath =
			"Resources/Sprites/Mushroom_" + std::to_string(i) + ".png";
		m_sprites[i] = new olc::Sprite(filePath);
	}

	m_currentSprite = m_sprites[m_uiHealth];
}

Mushroom::~Mushroom()
{
	unsigned int spriteCount = 4;

	for (unsigned int i = 0; i < spriteCount; ++i)
	{
		if (m_sprites[i])
		{
			delete m_sprites[i];
			m_sprites[i] = nullptr;
		}
	}

	m_currentSprite = nullptr;
}

// Updates the mushroom based on its health.
void Mushroom::Update(GameplayState* a_pState,
	Grid* a_pGrid,
	Cell& cell,
	BugBlaster& bugBlaster)
{
	// Update the mushroom's sprite to represent its remaining health while 
	// its health is <= its max health.
	if (m_uiHealth <= m_uiMaxHealth)
	{
		m_currentSprite = m_sprites[m_uiHealth];
	}
	else // Destroy the mushroom when its health's value raps round.
	{
		Destroy(a_pState, a_pGrid, cell, bugBlaster);
	}
}

void Mushroom::DecreaseHealth()
{
	--m_uiHealth;
}

void Mushroom::Destroy(GameplayState* a_pGameplayState,
	Grid* a_pGrid,
	Cell& cell,
	BugBlaster& bugBlaster) const
{
	if (a_pGameplayState && a_pGameplayState->GetScore())
	{
		// Increase the player's score because they destroyed this mushroom.
		a_pGameplayState->GetScore()->Update(mc_uiScoreValue);
		// Notify the flea manager that a mushroom has been destroyed.
		a_pGameplayState->GetFleaManager()->MushroomDestroyed();
	}

	// Clear this mushrooms cell.
	cell.ClearMushroom();
}

void Mushroom::SetPosition(float x, float y)
{
	m_position.SetX(x);
	m_position.SetY(y);
}

void Mushroom::SetPosition(const Vector2D* newPosition)
{
	if (newPosition)
	{
		m_position = *newPosition;
	}
}

void Mushroom::SetSprite(olc::Sprite* newSprite)
{
	m_currentSprite = newSprite;
}

// Sets whether or not the mushroom has been counted for score.
void Mushroom::SetCountState(bool a_bool)
{
	m_bCounted = a_bool;
}

Vector2D Mushroom::GetPosition() const
{
	return m_position;
}

olc::Sprite* Mushroom::GetSprite() const
{
	return m_currentSprite;
}

// Returns whether or not the mushroom has been counted for score.
bool Mushroom::GetCountState() const
{
	return m_bCounted;
}