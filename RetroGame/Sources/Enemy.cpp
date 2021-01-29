/////////////////////////////
// Author: Liam Bansal
// Date Created: 03/05/2020
/////////////////////////////

// Includes.
#include "Enemy.h" // File's header.
#include "Clamp.h"
#include "GameplayState.h"
#include "GameStateManager.h"
#include "Grid.h"

Enemy::Enemy() : mc_uiMaxHealth(1),
	m_uiHealth(1),
	m_uiSpeed(60),
	m_uiScoreValue(10),
	m_pSprite(nullptr),
	m_pPreviousCellsPosition(nullptr),
	m_moveDirection(),
	m_position(),
	m_pLastPosition(),
	m_pAnimationSprites(),
	m_pTargetCell()
{}

Enemy::~Enemy()
{}

// Draws the enemy's sprite onto the screen.
void Enemy::Draw(GameplayState* a_pGameplayState)
{
	if (m_pSprite && a_pGameplayState)
	{
		a_pGameplayState->GetManager()->DrawSprite((int32_t)m_position.GetX() - m_pSprite->width * 0.5f,
			(int32_t)m_position.GetY() - m_pSprite->height * 0.5f,
			m_pSprite);
	}
}

// Implemented by inheriting class.
void Enemy::UpdateOccupiedCell(Grid* a_pGrid)
{}

// Decrements the enemies health by one.
void Enemy::DecreaseHealth()
{
	--m_uiHealth;
}

void Enemy::SetPosition(const Vector2D* a_position)
{
	if (a_position)
	{
		m_position = *a_position;
	}
}

// Set a new target cell to move towards.
void Enemy::SetTargetCell(Grid* a_pGrid)
{
	if (m_pSprite && a_pGrid)
	{
		// The position that we want to move to.
		Vector2D targetPosition = m_position + (m_moveDirection * (const float)m_pSprite->height);
		Clamp clamp;
		// Clamp the target position's coordinates so they lay within the 
		// grid's boundaries. Coordinates outside the grid's boundaries may 
		// retrieve the wrong cell.
		// Always use unsigned coordinate values when setting target cell.
		targetPosition.SetX(clamp.ClampFloat(targetPosition.GetX(),
			(float)a_pGrid->GetXOrigin(),
			(float)a_pGrid->GetXOrigin() + a_pGrid->GetPixelWidth()));
		targetPosition.SetY(clamp.ClampFloat(targetPosition.GetY(),
			(float)a_pGrid->GetYOrigin(),
			(float)a_pGrid->GetYOrigin() + a_pGrid->GetPixelHeight()));
		m_pTargetCell = &a_pGrid->GetCell(&targetPosition);
	}
}

Vector2D* Enemy::GetCurrentPosition()
{
	return &m_position;
}

Vector2D* Enemy::GetPreviousPosition()
{
	return m_pLastPosition;
}

olc::Sprite* Enemy::GetSprite() const
{
	return m_pSprite;
}

unsigned int Enemy::GetHealth() const
{
	return m_uiHealth;
}

// Gets the cell this enemy is moving towards.
Cell* Enemy::GetTargetCell() const
{
	return m_pTargetCell;
}