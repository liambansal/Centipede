/////////////////////////////
// Author: Will Masek
// Date Created: 07/02/2020
/////////////////////////////

// Includes.
#include "BugBlaster.h" // File's header.
#include "Clamp.h"
#include "GameplayState.h"
#include "GameStateManager.h"
#include "Grid.h"
#include <string>
#include "Vector2D.h"

// Namespaces.
using namespace olc;

BugBlaster::BugBlaster(Grid* a_pGrid) : mc_uiMoveSprites(1),
	mc_uiFireSprites(1),
	mc_uiDeathSprites(4),
	mc_uiMaxLives(6),
	m_uiLives(3),
	m_uiBoltCounter(0),
	m_fMovementSpeed(110.0f),
	m_fMaxReloadTime(0.45f),
	m_fReloadTime(0.0f),
	m_bCanFire(true),
	m_bIsAlive(true),
	mc_cpMoveSprite("Move"),
	mc_cpFiredSprite("Fired"),
	mc_cpDeathAnimation("Death"),
	m_livesPrefix("Lives: "),
	m_spawnPosition(128.0f, 216.0f),
	m_position(m_spawnPosition),
	m_pSprite(new Sprite("Resources/Sprites/Bug Blaster/Bug Blaster (1).png"))
{
	m_animator.AddFrame(mc_cpMoveSprite,
		new olc::Sprite("Resources/Sprites/Bug Blaster/Bug Blaster (1).png"));
	m_animator.AddFrame(mc_cpFiredSprite,
		new olc::Sprite("Resources/Sprites/Bug Blaster/Bug Blaster (2).png"));

	// Create an animation from each bug blaster death sprite.
	for (unsigned int i = 1; i <= mc_uiDeathSprites; ++i)
	{
		m_animator.AddFrame(mc_cpDeathAnimation,
			new olc::Sprite("Resources/Sprites/Bug Blaster/Bug Blaster Death (" +
				std::to_string(i) +
				").png"));
	}

	m_animator.SetAnimation(mc_cpMoveSprite);
	const float frameLength = 0.1f;
	m_animator.SetFrameLength(frameLength);
}

BugBlaster::~BugBlaster()
{}

// Updates the bug blasters behaviour and how its displayed.
void BugBlaster::Update(GameplayState* a_pState,
	float* a_pDeltaTime,
	Grid* a_pGrid)
{	
	if (a_pState && a_pState->GetManager()) // Null check pointers.
	{
		if (m_bIsAlive)
		{
			HandleInput(a_pState, a_pDeltaTime);
			CalculateFireRate(a_pGrid);
			ClampPosition(a_pGrid);
			CheckForCollisions(a_pGrid);
			
			// Update our bolts.
			for (int i = 0; i < sizeof(m_bolts) / sizeof(Bolt); ++i)
			{
				m_bolts[i].Update(a_pGrid,
					a_pDeltaTime,
					a_pState->GetManager()->olc::PixelGameEngine::ScreenWidth(),
					a_pState->GetManager()->olc::PixelGameEngine::ScreenHeight());
			}

			// Checks if we should enable firing or continue reloading based on a 
			// reload timer being less than zero i.e. complete.
			if (m_fReloadTime <= 0.0f)
			{
				m_animator.SetAnimation(mc_cpMoveSprite);
				m_bCanFire = true;
			}
			else
			{
				m_animator.SetAnimation(mc_cpFiredSprite);
				m_fReloadTime -= (*a_pDeltaTime);
			}
		}
		else
		{
			PlayerDied(a_pState, a_pGrid);
		}

		m_animator.Update(a_pDeltaTime);
		m_pSprite = m_animator.GetCurrentFrame();
	}
}

// Listens for and reacts to input for moving and firing the bug blaster.
void BugBlaster::HandleInput(GameplayState* a_pState, float* a_pDeltaTime)
{
	if (a_pState && a_pState->GetManager())
	{
		// Screen origin is in the top left corner. Y axis increases downwards and 
		// x axis increases towards the right.
		if (a_pState->GetManager()->GetKey(Key::W).bHeld) // Move up.
		{
			m_position -= m_position.Up() * m_fMovementSpeed * (*a_pDeltaTime);
		}

		if (a_pState->GetManager()->GetKey(Key::A).bHeld) // Move left.
		{
			m_position += m_position.Left() * m_fMovementSpeed * (*a_pDeltaTime);
		}

		if (a_pState->GetManager()->GetKey(Key::S).bHeld) // Move down.
		{
			m_position -= m_position.Down() * m_fMovementSpeed * (*a_pDeltaTime);
		}

		if (a_pState->GetManager()->GetKey(Key::D).bHeld) // Move right.
		{
			m_position += m_position.Right() * m_fMovementSpeed * (*a_pDeltaTime);
		}

		// Check input for firing bolts.
		if (m_bCanFire &&
			(a_pState->GetManager()->GetKey(Key::ENTER).bHeld ||
			a_pState->GetManager()->GetKey(Key::ENTER).bPressed))
		{
			Fire(a_pState, a_pDeltaTime);
			m_bCanFire = false;
			m_fReloadTime = m_fMaxReloadTime;
		}
	}
}

void BugBlaster::DrawSprite(GameplayState* a_pState) const
{
	// Null check pointers.
	if (a_pState && a_pState->GetManager() && m_pSprite)
	{
		// Draws the bug blaster sprite and offsets the sprite from the 
		// position, so the position is at the center of the sprite.
		a_pState->GetManager()->DrawSprite((int32_t)(m_position.GetX() -
			(m_pSprite->width / 2)),
			(int32_t)(m_position.GetY() - (m_pSprite->height / 2)), 
			m_pSprite);
	}
}

void BugBlaster::DrawBolts(GameplayState* a_pState) const
{
	for (int i = 0; i < sizeof(m_bolts) / sizeof(Bolt); ++i)
	{
		m_bolts[i].Draw(a_pState);
	}
}

// Draws the player's remaining lives onto the screen.
void BugBlaster::DrawLives(GameplayState* pScene) const
{
	if (pScene && pScene->GetManager() && m_pSprite) // Null check pointers.
	{
		unsigned int xPosition = 0;
		unsigned int yPosition = 0;
		// Draws the lives prefix text to explain what the proceeding values 
		// are.
		pScene->GetManager()->DrawString(xPosition,
			yPosition,
			m_livesPrefix,
			olc::WHITE);

		// Lengths are in pixels.
		unsigned int letterLength = 8;
		unsigned int prefixLength = (unsigned int)m_livesPrefix.length() *
			letterLength;

		// Draws a bug blaster sprite for each of the player's lives.
		// Offset their x position by the prefix length plus sprites width 
		// multiplied with the amount of lives drawn.
		for (unsigned int i = 0; i < m_uiLives; ++i)
		{
			pScene->GetManager()->DrawSprite(prefixLength +
				m_pSprite->width *
				i,
				yPosition,
				m_pSprite);
		}
	}
}

// Keeps the bug blasters position inside the grid's boundaries.
void BugBlaster::ClampPosition(Grid* a_pGrid)
{
	if (a_pGrid)
	{
		// Screen origin starts at top left corner. Y axis increases downwards. X 
		// axis increases to the right.
		// Check if x position is greater than the grid's right boundary.
		if (m_position.GetX() > a_pGrid->GetRightBoundary(m_pSprite->height / 2))
		{
			m_position.SetX((const float)a_pGrid->GetRightBoundary(m_pSprite->height / 2));
		}
		// Check if x position is lower than the grid's left boundary.
		else if (m_position.GetX() < a_pGrid->GetLeftBoundary(m_pSprite->height / 2))
		{
			m_position.SetX((const float)a_pGrid->GetLeftBoundary(m_pSprite->height / 2));
		}

		float noMovementZoneHeight = a_pGrid->GetNoMovementZone()->GetY();

		// Check if y position is greater than the grid's top boundary plus the no 
		// movement zone's height.
		if (m_position.GetY() < a_pGrid->GetUpperBoundary(m_pSprite->height / 2) +
			noMovementZoneHeight)
		{
			m_position.SetY((const float)a_pGrid->GetUpperBoundary(m_pSprite->height / 2) +
				noMovementZoneHeight);
		}
		// Check if y position is lower than the grid's bottom boundary.
		else if (m_position.GetY() > a_pGrid->GetLowerBoundary(m_pSprite->height / 2))
		{
			m_position.SetY((const float)a_pGrid->GetLowerBoundary(m_pSprite->height / 2));
		}
	}
}

// Fires a singular bolt from the bug blasters position.
void BugBlaster::Fire(GameplayState* a_pState, float* a_pDeltaTime)
{
	if (a_pState) // Null check pointer.
	{
		// Gets an existing bolt in the scene.
		Bolt& bolt = GetBolt();
		bolt.Load(this);
	}
}

// Checks for collisions with objects in cell's adjacent the bug blaster.
// and handles different collision events.
void BugBlaster::CheckForCollisions(Grid* a_pGrid)
{
	if (a_pGrid)
	{
		const char* emptyCellTag = "Empty Cell";
		const char* mushroomTag = "Mushroom";

		// Check for collisions with enemies first.
		if (a_pGrid->GetCell(&m_position).GetTag() != emptyCellTag &&
			a_pGrid->GetCell(&m_position).GetTag() != mushroomTag)
		{
			--m_uiLives;
			// Destroy the player after they collide with something hostile.
			m_bIsAlive = false;
			return;
		}
		else
		{
			const unsigned int cellDistance = 1;
			// Gets each cell around the Bug Blaster.
			m_pAdjacentCells[0] =
				&a_pGrid->GetCell((unsigned int)m_position.GetX() / 8 - cellDistance,
					(unsigned int)m_position.GetY() / 8 - cellDistance); // Up-left.
			m_pAdjacentCells[1] =
				&a_pGrid->GetCell((unsigned int)m_position.GetX() / 8,
					(unsigned int)m_position.GetY() / 8 - cellDistance); // Up.
			m_pAdjacentCells[2] =
				&a_pGrid->GetCell((unsigned int)m_position.GetX() / 8 + cellDistance,
					(unsigned int)m_position.GetY() / 8 - cellDistance); // Up-right.
			m_pAdjacentCells[3] =
				&a_pGrid->GetCell((unsigned int)m_position.GetX() / 8 - cellDistance,
					(unsigned int)m_position.GetY() / 8); // Left.
			m_pAdjacentCells[4] =
				&a_pGrid->GetCell((unsigned int)m_position.GetX() / 8 + cellDistance,
					(unsigned int)m_position.GetY() / 8); // Right.
			m_pAdjacentCells[5] =
				&a_pGrid->GetCell((unsigned int)m_position.GetX() / 8 - cellDistance,
					(unsigned int)m_position.GetY() / 8 + cellDistance); // Down-left.
			m_pAdjacentCells[6] =
				&a_pGrid->GetCell((unsigned int)m_position.GetX() / 8,
					(unsigned int)m_position.GetY() / 8 + cellDistance); // Down.
			m_pAdjacentCells[7] =
				&a_pGrid->GetCell((unsigned int)m_position.GetX() / 8 + cellDistance,
					(unsigned int)m_position.GetY() / 8 + cellDistance); // Down-right.

			if (m_pAdjacentCells && m_pSprite) // Null check pointers.
			{
				const char* mushroomTag = "Mushroom";

				// Loops through each adjacent cell.
				for (unsigned int i = 0;
					i < sizeof(m_pAdjacentCells) / sizeof(*m_pAdjacentCells);
					++i)
				{
					// Now check for collisions with mushrooms.
					if (m_pAdjacentCells[i]->GetTag() == mushroomTag)
					{
						// If the distance to the object is less than our sprite's 
						// width then we're colliding.
						if ((m_position.distance(*m_pAdjacentCells[i]->GetPosition())) <
							(float)m_pSprite->width)
						{
							// Vector2D for storing the route to the object.
							Vector2D direction =
								m_position - (*m_pAdjacentCells[i]->GetPosition());
							// Moves the bug blaster back along the route so we don't
							// collide.
							m_position += (direction.normalised() *
								direction.magnitudeSq());
						}
					}
				}
			}
		}
	}
}

// Calculates a reload time based on the distance of the closest object 
// along the bug blaster's y axis.
void BugBlaster::CalculateFireRate(Grid* a_pGrid)
{
	if (a_pGrid)
	{
		// Loops through the vertical line of the cells between the bug blaster
		// and the top of the screen.
		for (int y = (int)a_pGrid->GetCell(&m_position).GetPosition()->GetY() /
			a_pGrid->GetCellHeight();
			y > 1;
			--y)
		{
			const Cell& examinedCell =
				a_pGrid->GetCell((int)m_position.GetX() / a_pGrid->GetCellHeight(), y);

			// Stops when its finds an occupied cell.
			if (a_pGrid->GetCell(&m_position).GetPosition() &&
				examinedCell.GetTag() != "Empty Cell")
			{
				if (examinedCell.GetTag() == "Centipede")
				{
					float centipedeReloadTime = 0.16f;
					m_fMaxReloadTime = centipedeReloadTime;
					return;
				}

				// Calculates a reload time based on the distance between the bug 
				// blaster and the occupied cell.
				// A shorter distance leads to a shorter reload time.
				m_fMaxReloadTime =
					((a_pGrid->GetCell(&m_position).GetPosition()->GetY() -
						examinedCell.GetPosition()->GetY()) /
						a_pGrid->GetCellHeight() /
						a_pGrid->GetHeight());
				m_fMaxReloadTime /= 2;
				return;
			}
		}

		// Sets the maximum possible reload time if no occupied cells were 
		// found.
		m_fMaxReloadTime =
			a_pGrid->GetCell(&m_position).GetPosition()->GetY() /
			a_pGrid->GetCellHeight() /
			a_pGrid->GetHeight();
		m_fMaxReloadTime /= 2;
	}
}

// Gives the player an extra life.
void BugBlaster::AddLife()
{
	++m_uiLives;

	if (m_uiLives > mc_uiMaxLives)
	{
		m_uiLives = mc_uiMaxLives;
	}
}

// Checks if the game should end or if the bug blaster should be respawned.
void BugBlaster::PlayerDied(GameplayState* a_pState, Grid* a_pGrid)
{
	if (a_pState && a_pGrid)
	{
		if (m_animator.GetName() != mc_cpDeathAnimation)
		{
			m_animator.SetAnimation(mc_cpDeathAnimation);
			const float frameLength = 0.5f;
			m_animator.SetFrameLength(frameLength);
		}

		if (m_animator.AnimationOver())
		{
			if (m_uiLives == 0)
			{
				a_pState->SetState(false);
				return;
			}

			// Count any new mushrooms to increase the players score.
			a_pGrid->CountMushrooms(a_pState);
			// Respawn the player after they've died.
			Respawn(a_pState, a_pGrid);
			// Clear all enemies.
			a_pState->GetCentipedeManager()->ClearCentipedes(a_pGrid);
			a_pState->GetSpiderManager()->DestroySpider(a_pGrid);
			a_pState->GetFleaManager()->DestroyFlea(a_pGrid);
			// Spawn a new centipede to fight against.
			a_pState->GetCentipedeManager()->SpawnCentipede(a_pGrid);
		}
	}
}

void BugBlaster::Respawn(GameplayState* a_pState, Grid* a_pGrid)
{
	m_position = m_spawnPosition;
	m_bIsAlive = true;
}

void BugBlaster::SetSprite(olc::Sprite* a_sprite)
{
	m_pSprite = a_sprite;
}

void BugBlaster::SetPosition(Vector2D a_position)
{
	m_position = a_position;
}

olc::Sprite* BugBlaster::GetSprite() const
{
	return m_pSprite;
}

const Vector2D BugBlaster::GetPosition() const
{
	return m_position;
}

Bolt& BugBlaster::GetBolt()
{
	// Resets the bolt array's counter when it reaches the array's last 
	// element.
	if (m_uiBoltCounter == sizeof(m_bolts) / sizeof(Bolt))
	{
		m_uiBoltCounter = 0;
	}

	// Increments the array's counter, after returning a bolt, to get the 
	// next bolt for when the function is called again.
	return m_bolts[m_uiBoltCounter++];
}