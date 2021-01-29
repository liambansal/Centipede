/////////////////////////////
// Author: Liam Bansal
// Date Created: 03/05/2020
/////////////////////////////

// Includes.
#include "CentipedeHead.h" // File's header.
#include "BugBlaster.h"
#include "CentipedeBody.h"
#include "CentipedeManager.h"
#include "GameplayState.h"
#include "GameStateManager.h"
#include "Grid.h"
#include "PopUpText.h"
#include "Score.h"

CentipedeHead::CentipedeHead() : Centipede(),
	mc_uiMoveSprites(8),
	// Sets how many bodies are behind the centipede head.
	m_uiBodyLength(0),
	// Ignore first two collisions to make moving easier.
	m_uiIgnoreCount(0),
	m_bHasBody(false),
	m_bIgnoreCollisions(false),
	m_bListChanged(false),
	m_bodyParts(),
	// With our coordinates system's up and down axes are reversed.
	m_verticalMoveDirection(m_verticalMoveDirection.Up()),
	m_lastHorizontalDirection(m_currentDirection),
	// Set to same direction as vertical movement.
	m_verticalDirection(MOVE_DIRECTIONS::MOVE_DIRECTIONS_DOWN),
	m_pManager(nullptr)
{
	m_pSprite = new olc::Sprite("Resources/Sprites/Centipede Head/Centipede Head (1).png");
	m_pHead = this;
	m_uiScoreValue = 100;

	for (unsigned int spriteCount = 1;
		spriteCount <= mc_uiMoveSprites;
		++spriteCount)
	{
		m_animator.AddFrame("Move",
			new olc::Sprite("Resources/Sprites/Centipede Head/Centipede Head (" +
				std::to_string(spriteCount) +
				").png"));
	}

	m_animator.SetAnimation("Move");
	const float frameLength = 0.06f;
	m_animator.SetFrameLength(frameLength);
}

CentipedeHead::~CentipedeHead()
{
	for (auto listIterator = m_bodyParts.begin();
		listIterator != m_bodyParts.end();
		++listIterator)
	{
		if (*listIterator)
		{
			delete (*listIterator);
			(*listIterator) = nullptr;
		}
	}
}

void CentipedeHead::operator=(const CentipedeHead* a_rCentipede)
{
	if (a_rCentipede)
	{
		m_uiHealth = a_rCentipede->m_uiHealth;
		m_uiSpeed = a_rCentipede->m_uiSpeed;
		m_uiIgnoreCount = a_rCentipede->m_uiIgnoreCount;
		m_bIgnoreCollisions = a_rCentipede->m_bIgnoreCollisions;
		m_moveDirection = a_rCentipede->m_moveDirection;
		m_verticalMoveDirection = a_rCentipede->m_verticalMoveDirection;
		m_pLastPosition = a_rCentipede->m_pLastPosition;
		m_pTargetCell = a_rCentipede->m_pTargetCell;
		m_currentDirection = a_rCentipede->m_currentDirection;
		m_lastHorizontalDirection = a_rCentipede->m_lastHorizontalDirection;
		m_verticalDirection = a_rCentipede->m_verticalDirection;
		m_pPreviousCellsPosition = a_rCentipede->m_pPreviousCellsPosition;
	}
}

// Updates the centipede head & bodies' positions, collisions, animations and
// deaths.
void CentipedeHead::Update(GameplayState* a_pGameplayState,
	Grid* a_pGrid,
	float* a_pDeltaTime)
{
	if (a_pGameplayState && a_pGameplayState->GetBugBlaster())
	{
		if (m_uiHealth <= 0)
		{
			a_pGameplayState->AddScore(PopUpText(std::to_string(m_uiScoreValue), m_position));
			DestroyThis(a_pGameplayState, a_pGrid);
		}
		else
		{
			Move(a_pGrid, a_pDeltaTime);
			m_animator.Update(a_pDeltaTime);
			m_pSprite = m_animator.GetCurrentFrame();
			CheckBoundaryCollisions(a_pGrid);
			CheckObjectCollision(a_pGrid);

			if (m_bHasBody)
			{
				// Additional iterator for looping through the body parts list so
				// we can keep a reference to the previously examined part.
				auto m_pPreviousBody = m_bodyParts.begin();

				// Calls update on all the centipede body parts using our primary 
				// list iterator.
				for (auto listIterator = m_bodyParts.begin();
					listIterator != m_bodyParts.end();
					listIterator++)
				{
					if (*listIterator)
					{
						// Must isolate and dereference the iterator before we can 
						// access it's value.
						(*listIterator)->Update(a_pGameplayState,
							a_pGrid,
							// Using ternary operator sends a pointer to the previous 
							// body part if we're not examining the first body part.
							listIterator != m_bodyParts.begin() ?
							(*m_pPreviousBody) : nullptr,
							a_pDeltaTime);
					}

					// Check for changes with our body parts list while we were 
					// updating it.
					if (!m_bListChanged)
					{
						// Don't update secondary list iterator if our primary one
						// is examining the beginning element.
						if (listIterator != m_bodyParts.begin())
						{
							// Update local iterator so we point to the body list's 
							// previous part in the next loop iteration.
							m_pPreviousBody++;
						}
					}
					else
					{
						// We've acted to this bool being true so now we can negate
						// it and update as normal in the function's proceeding
						// calls.
						m_bListChanged = false;
						break;
					}
				}
			}
		}
	}
}

// Moves the head towards its target cell and adds it to its bodies' list of
// destinations.
void CentipedeHead::Move(Grid* a_pGrid, float* a_pDeltaTime)
{
	if (a_pGrid)
	{
		// If we have a target to move towards then do it.
		if (m_pTargetCell)
		{
			m_position += m_moveDirection.normalised() * (const float)m_uiSpeed * (*a_pDeltaTime);
		}
		else
		{
			SetTargetCell(a_pGrid);
		}

		// Check if this centipede part has moved into a new cell.
		if (a_pGrid->GetCell(&m_position).GetPosition() != m_pPreviousCellsPosition)
		{
			UpdateOccupiedCell(a_pGrid);

			// Add the new cell to each of the body's target cell lists.
			for (auto listIterator = m_bodyParts.begin();
				listIterator != m_bodyParts.end();
				++listIterator)
			{
				if (*listIterator)
				{
					(*listIterator)->GetTargetCells()->push_back(&a_pGrid->GetCell(&m_position));
				}
			}
		}
	}
}

// Checks if the head has crossed any of the grid's boundaries and 
// moves it back onto the grid. Also sets a new move direction & target 
// destination after colliding.
void CentipedeHead::CheckBoundaryCollisions(Grid* a_pGrid)
{
	if (m_pSprite && a_pGrid)
	{
		bool collided = false;

		// Checks if the centipede's x position is less or greater than the left 
		// and right grid boundaries.
		if (m_position.GetX() < ((float)m_pSprite->width * 0.5f))
		{
			m_position.SetX((float)m_pSprite->width * 0.5f);
			collided = true;
		}
		else if (m_position.GetX() > ((float)a_pGrid->GetRightBoundary((unsigned int)((float)m_pSprite->width * 0.5f))))
		{
			m_position.SetX((float)a_pGrid->GetRightBoundary((unsigned int)((float)m_pSprite->width * 0.5f)));
			collided = true;
		}

		if (collided)
		{
			// We only need to check for vertical boundary collisions when at the 
			// left/right side of the grid because that's the only place where we 
			// will surpass the top/bottom grid boundaries.
			// Checks if we've crossed the top or bottom boundary of the grid.
			if ((m_verticalDirection == MOVE_DIRECTIONS::MOVE_DIRECTIONS_UP &&
				(m_position.GetY() <= (float)a_pGrid->GetUpperBoundary((unsigned int)((float)m_pSprite->height * 0.5f)) + a_pGrid->GetNoMovementZone()->GetY())) ||
				((m_verticalDirection == MOVE_DIRECTIONS::MOVE_DIRECTIONS_DOWN &&
					m_position.GetY() >= (float)a_pGrid->GetLowerBoundary((unsigned int)((float)m_pSprite->height * 0.5f)))))
			{
				VerticalWallCollision(a_pGrid);
				return;
			}

			// Set a new movement direction after we collide.
			SetMoveDirection(a_pGrid);
		}
	}
}

// Checks for collisions with the target cell and mushrooms.
void CentipedeHead::CheckObjectCollision(Grid* a_pGrid)
{
	if (a_pGrid && m_pSprite)
	{
		// We want to ignore collisions while the ignore count is greater than 
		// 0.
		if (m_uiIgnoreCount <= 0)
		{
			m_bIgnoreCollisions = false;
		}

		// Checks if we're ignoring collisions and have collided with a new 
		// cell...
		if (m_bIgnoreCollisions &&
			a_pGrid->GetCell(&m_position).GetPosition() != m_pLastPosition)
		{
			// ...so we can decrease the number of cells to ignore each 
			// time we collide with a cell.
			--m_uiIgnoreCount;
			// Record our current cell's position so we don't register a
			// collision with the same cell more than once.
			m_pLastPosition =
				(Vector2D*)a_pGrid->GetCell(&m_position).GetPosition();
		}

		// If we don't want to ignore collisions...
		if (!m_bIgnoreCollisions)
		{
			Cell* forwardCell = nullptr;

			// ...check for collisions along the current movement direction.
			switch (m_currentDirection)
			{
			case MOVE_DIRECTIONS::MOVE_DIRECTIONS_UP:
				{
					// Register a 'collision' when our y position is less than or 
					// equal to the target cell's y position because we only want 
					// to move up one row.
					if (m_pTargetCell &&
						m_position.GetY() <= m_pTargetCell->GetPosition()->GetY())
					{
						SetMoveDirection(a_pGrid);
					}

					break;
				}
			case MOVE_DIRECTIONS::MOVE_DIRECTIONS_LEFT:
				{
					Vector2D forwardPosition(m_position.GetX() - (const float)m_pSprite->height,
						m_position.GetY());
					forwardCell = &a_pGrid->GetCell(&forwardPosition);

					// Check the cell is occupied before attempting to 
					// register a collision.
					if (forwardCell != nullptr &&
						forwardCell->GetTag() != "Empty Cell")
					{
						if (forwardCell->GetTag() == "Centipede" &&
							!forwardCell->CompareCentipede(GetHead()))
						{
							if (!forwardCell->CompareCentipede(GetHead()))
							{
								SetTargetCell(a_pGrid);
								CheckDistance(a_pGrid);
								break;
							}
						}

						if (forwardCell->GetTag() != "Centipede")
						{
							SetTargetCell(a_pGrid);
							CheckDistance(a_pGrid);
							break;
						}
					}

					break;
				}
			case MOVE_DIRECTIONS::MOVE_DIRECTIONS_DOWN:
				{
					// Register a 'collision' when our y position is greater than 
					// or equal to the target cell's y position because we only 
					// want to move down one row.
					if (m_pTargetCell &&
						m_position.GetY() >= m_pTargetCell->GetPosition()->GetY())
					{
						SetMoveDirection(a_pGrid);
					}

					break;
				}
			case MOVE_DIRECTIONS::MOVE_DIRECTIONS_RIGHT:
				{
					Vector2D forwardPosition(m_position.GetX() +
						(const float)m_pSprite->height,
						m_position.GetY());
					forwardCell = &a_pGrid->GetCell(&forwardPosition);

					// Check the cell is occupied before attempting to 
					// register a collision.
					if (forwardCell != nullptr &&
						forwardCell->GetTag() != "Empty Cell")
					{
						if (forwardCell->GetTag() == "Centipede" &&
							!forwardCell->CompareCentipede(GetHead()))
						{
							if (!forwardCell->CompareCentipede(GetHead()))
							{
								SetTargetCell(a_pGrid);
								CheckDistance(a_pGrid);
								break;
							}
						}

						if (forwardCell->GetTag() != "Centipede")
						{
							SetTargetCell(a_pGrid);
							CheckDistance(a_pGrid);
							break;
						}
					}

					break;
				}
			default:
				{
					break;
				}
			}
		}
	}
}

// Checks the distance between the head and it's target cell.
// Sets a new movement course upon colliding.
void CentipedeHead::CheckDistance(Grid* a_pGrid)
{
	if (m_pTargetCell && m_pSprite && a_pGrid)
	{
		// Check for collisions once we're within the sprites height range of the 
		// cell.
		if (m_position.distance(*m_pTargetCell->GetPosition()) < m_pSprite->height)
		{
			// Checks if we've crossed the top or bottom boundary 
			// of the grid. We want to reverse our vertical 
			// direction when colliding with objects along the top 
			// or bottom rows of the grid.
			if ((m_verticalDirection == MOVE_DIRECTIONS::MOVE_DIRECTIONS_UP &&
				(m_position.GetY() <= (const float)a_pGrid->
					GetUpperBoundary(m_pSprite->height * 0.5f) +
					a_pGrid->GetNoMovementZone()->GetY())) ||
				((m_verticalDirection == MOVE_DIRECTIONS::MOVE_DIRECTIONS_DOWN &&
					m_position.GetY() >= (const float)a_pGrid->
					GetLowerBoundary(m_pSprite->height * 0.5f))))
			{
				VerticalWallCollision(a_pGrid);
				return;
			}

			SetMoveDirection(a_pGrid);
		}
	}
}

// Draws the head onto the screen.
void CentipedeHead::Draw(GameplayState* a_pScene)
{
	if (m_pSprite && a_pScene)
	{
		a_pScene->GetManager()->DrawSprite((int32_t)m_position.GetX() -
			m_pSprite->width * 0.5f,
			(int32_t)m_position.GetY() -
			m_pSprite->height * 0.5f,
			m_pSprite);
	}

	if (m_bHasBody)
	{
		// Calls Draw on all centipede body parts.
		for (auto listIterator = m_bodyParts.begin();
			listIterator != m_bodyParts.end();
			++listIterator)
		{
			if (*listIterator)
			{
				(*listIterator)->Draw(a_pScene);
			}
		}
	}
}

// Splits the centipede somewhere along it's body creating a new,
// independent, centipede.
void CentipedeHead::Split(Grid* a_pGrid, CentipedeBody* a_pSplitPoint)
{
	if (m_pManager && a_pGrid)
	{
		std::list<CentipedeBody*> pBody;

		// Starts iterating from the tail of the centipede so we can remove 
		// the bodies that come after the one that was hit and leave the
		// proceeding ones alone.
		for (auto listIterator = m_bodyParts.rbegin();
			listIterator != m_bodyParts.rend();
			listIterator++)
		{
			if (*listIterator == a_pSplitPoint)
			{
				break;
			}
			else
			{
				// Pushes the bodies into a new list in their natural order.
				pBody.push_front(*listIterator);
			}
		}

		// Start iterating through the centipede's body from the last part.
		auto listIterator = pBody.rbegin();

		// Removes all body parts until we reach the one that was hit.
		while (listIterator != pBody.rend() &&
			(*listIterator) &&
			m_bodyParts.back())
		{
			// Clear cell references to the iterating body.
			a_pGrid->GetCell((*listIterator)->
				GetPreviousPosition()).ClearCentipede();
			a_pGrid->GetCell((*listIterator)->
				GetCurrentPosition()).ClearCentipede();
			// Clear cell references to the body at the end of this head's 
			// body list.
			a_pGrid->GetCell(m_bodyParts.back()->
				GetPreviousPosition()).ClearCentipede();
			a_pGrid->GetCell(m_bodyParts.back()->
				GetCurrentPosition()).ClearCentipede();
			// Don't deallocate the memory because we're pointing to it in 
			// another list.
			m_bodyParts.back() = nullptr;
			// Remove the last body from this centipede head.
			m_bodyParts.pop_back();
			listIterator++;
		}

		if (m_bodyParts.back() && m_bodyParts.back()->GetSprite())
		{
			Vector2D forwardPosition =
				(*m_bodyParts.back()->GetCurrentPosition()) +
				(*m_bodyParts.back()->GetMoveDirection()) *
				(const float)m_bodyParts.back()->GetSprite()->height;
			// One cell forward of the last body part in the list, along its 
			// movement direction.
			// Clear cell references to this centipede's last body part.
			a_pGrid->GetCell(m_bodyParts.back()->
				GetPreviousPosition()).ClearCentipede();
			a_pGrid->GetCell(m_bodyParts.back()->
				GetCurrentPosition()).ClearCentipede();

			if (*m_bodyParts.back()->GetMoveDirection() == m_moveDirection.Left() ||
				*m_bodyParts.back()->GetMoveDirection() == m_moveDirection.Right())
			{
				Vector2D forwardPosition(*m_bodyParts.back()->GetCurrentPosition() +
					(*m_bodyParts.back()->GetMoveDirection()) *
					(float)m_bodyParts.back()->GetSprite()->height);
				Cell* forwardCell = nullptr;
				forwardCell = &a_pGrid->GetCell(&forwardPosition);

				// Create a mushroom forward of the centipede part that was hit
				// if the cell isn't already occupied by a mushroom.
				if (!forwardCell->GetMushroom())
				{
					forwardCell->SpawnMushroom();
				}
			}
			else if (*m_bodyParts.back()->GetMoveDirection() == m_moveDirection.Up() ||
				*m_bodyParts.back()->GetMoveDirection() == m_moveDirection.Down())
			{
				// TODO: centipede bodies may need their own 'last horizontal direction' variable.
				if (m_lastHorizontalDirection == MOVE_DIRECTIONS::MOVE_DIRECTIONS_LEFT)
				{
					Vector2D left(*m_bodyParts.back()->GetCurrentPosition() +
						m_bodyParts.back()->GetMoveDirection()->Left() *
						(const float)m_bodyParts.back()->GetSprite()->height);
					// Spawn a mushroom left of this head's position.
					a_pGrid->GetCell(&left).SpawnMushroom();
				}
				else if (m_lastHorizontalDirection == MOVE_DIRECTIONS::MOVE_DIRECTIONS_RIGHT)
				{
					Vector2D right(*m_bodyParts.back()->GetCurrentPosition() +
						m_bodyParts.back()->GetMoveDirection()->Right() *
						(const float)m_bodyParts.back()->GetSprite()->height);
					// Spawn a mushroom left of this head's position.
					a_pGrid->GetCell(&right).SpawnMushroom();
				}
			}

			// Destroys the body part that was hit by a bolt.
			delete m_bodyParts.back();
			m_bodyParts.back() = nullptr;
			m_bodyParts.pop_back();
		}

		// Only spawn a new centipede if there are any bodies behind the 
		// one that was hit by a bolt.
		if (pBody.size() > 0)
		{
			m_pManager->CreateCentipede(a_pGrid, &pBody);
		}

		// Re-count this heads body length, after .
		m_uiBodyLength = (unsigned int)m_bodyParts.size();
		m_bListChanged = true;
	}
}

void CentipedeHead::CreateBody(Grid* a_pGrid, unsigned int a_length)
{
	m_uiBodyLength = a_length;

	if (m_pSprite && m_uiBodyLength > 0)
	{
		// Spawns the centipede's body parts.
		for (unsigned int bodiesSpawned = 0; bodiesSpawned < m_uiBodyLength;)
		{
			CentipedeBody* body = new CentipedeBody();

			if (body && m_pSprite)
			{
				// Prefix increment operator is evaluated before its operand is 
				// used in the expression evaluation.
				Vector2D bodyEnd(m_position.GetX(), m_position.GetY() - ++bodiesSpawned * m_pSprite->height);
				// Spawns the bodies in a column, one behind the other. Some 
				// bodies may spawn off screen and have negative position values.
				body->SetPosition(&bodyEnd);
				body->SetHead(this);
				// Add the newly created body to the centipede's parts list to 
				// keep track of it.
				m_bodyParts.push_back(body);
			}
		}

		// Check body has initialized properly.
		if (m_bodyParts.size() == m_uiBodyLength)
		{
			m_bHasBody = true;
		}
		else
		{
			// Clear the body parts data because we did not initialize to 
			// right length, then 'nullify' the head's body variables.
			m_bodyParts.clear();
			m_uiBodyLength = 0;
			m_bHasBody = false;
		}
	}
	else
	{
		m_bHasBody = false;
	}
}

// Destroys the head and replaces its first body with a new head.
void CentipedeHead::DestroyThis(GameplayState* a_pState,
	Grid* a_pGrid)
{
	if (m_pManager && a_pState && a_pState->GetScore())
	{
		if (m_bHasBody && m_bodyParts.size() > 0)
		{
			// This head has a body. We'll need to assign it a new head to follow.
			m_pManager->CreateCentipede(a_pGrid, &m_bodyParts);
			// This head's body list changed when assigning a new head.
			m_bListChanged = true;
		}

		// Clear cell references to this head.
		a_pGrid->GetCell(m_pLastPosition).ClearCentipede();
		a_pGrid->GetCell(&m_position).ClearCentipede();
		// Increase the player's score.
		a_pState->GetScore()->Update(m_uiScoreValue);

		if (m_pSprite)
		{
			if (m_moveDirection == m_moveDirection.Left() ||
				m_moveDirection == m_moveDirection.Right())
			{
				Vector2D forward(m_position + m_moveDirection * (const float)m_pSprite->height);
				// Spawn a mushroom forward of this head's position.
				a_pGrid->GetCell(&forward).SpawnMushroom();
			}
			else if (m_moveDirection == m_moveDirection.Up() || m_moveDirection == m_moveDirection.Down())
			{
				if (m_lastHorizontalDirection == MOVE_DIRECTIONS::MOVE_DIRECTIONS_LEFT)
				{
					Vector2D left(m_position + m_moveDirection.Left() * (const float)m_pSprite->height);
					// Spawn a mushroom left of this head's position.
					a_pGrid->GetCell(&left).SpawnMushroom();
				}
				else if (m_lastHorizontalDirection == MOVE_DIRECTIONS::MOVE_DIRECTIONS_RIGHT)
				{
					Vector2D right(m_position + m_moveDirection.Right() * (const float)m_pSprite->height);
					// Spawn a mushroom left of this head's position.
					a_pGrid->GetCell(&right).SpawnMushroom();
				}
			}
		}

		// Remove this head from its manager's centipede list before 
		// deallocating its memory.
		m_pManager->RemoveCentipede(this);
	}
}

// Handles collisions with the top and bottom grid boundaries.
void CentipedeHead::VerticalWallCollision(Grid* a_pGrid)
{
	switch (m_verticalDirection)
	{
	case MOVE_DIRECTIONS::MOVE_DIRECTIONS_UP:
		{
			BottomBoundaryCollide(a_pGrid);
			break;
		}
	case MOVE_DIRECTIONS::MOVE_DIRECTIONS_DOWN:
		{
			TopBoundaryCollide(a_pGrid);
			break;
		}
	default:
		{
			break;
		}
	}

	if (m_lastHorizontalDirection == MOVE_DIRECTIONS::MOVE_DIRECTIONS_LEFT)
	{
		m_lastHorizontalDirection = MOVE_DIRECTIONS::MOVE_DIRECTIONS_RIGHT;
	}
	else if (m_lastHorizontalDirection == MOVE_DIRECTIONS::MOVE_DIRECTIONS_RIGHT)
	{
		m_lastHorizontalDirection = MOVE_DIRECTIONS::MOVE_DIRECTIONS_LEFT;
	}

	m_bIgnoreCollisions = true;
	// Sets the number of objects to ignore collisions with.
	m_uiIgnoreCount = 2;
	SetTargetCell(a_pGrid);
}

// Sets the head's movement course after colliding with a grid's top boundary.
void CentipedeHead::TopBoundaryCollide(Grid* a_pGrid)
{
	// Now we want to move up. Our coordinate system's axes measure 
	// moving down the screen as increasing along the y axis.
	m_moveDirection = m_moveDirection.Down();
	m_verticalMoveDirection = m_verticalMoveDirection.Down();
	m_currentDirection = MOVE_DIRECTIONS::MOVE_DIRECTIONS_UP;
	m_verticalDirection = MOVE_DIRECTIONS::MOVE_DIRECTIONS_UP;
}

// Sets the head's movement course after colliding with a grid's bottom
// boundary.
void CentipedeHead::BottomBoundaryCollide(Grid* a_pGrid)
{
	// Now we want to move down. Our coordinate system's axes measure 
	// moving up the screen as decreasing along the y axis.
	m_moveDirection = m_moveDirection.Up();
	m_verticalMoveDirection = m_verticalMoveDirection.Up();
	m_currentDirection = MOVE_DIRECTIONS::MOVE_DIRECTIONS_DOWN;
	m_verticalDirection = MOVE_DIRECTIONS::MOVE_DIRECTIONS_DOWN;
}

void CentipedeHead::SetSprite(olc::Sprite* a_pSprite)
{
	m_pSprite = a_pSprite;
}

// Sets the manager that will update this head.
void CentipedeHead::SetManager(CentipedeManager* a_pManager)
{
	m_pManager = a_pManager;
}

void CentipedeHead::SetBodyLength(unsigned int a_length)
{
	// Check the body length is a valid length.
	if (a_length > 0)
	{
		m_uiBodyLength = a_length;
		m_bHasBody = true;
	}
	else
	{
		m_uiBodyLength = 0;
		m_bHasBody = false;
	}
}

// Sets a new movement direction and target movement destination based on the 
// centipede's previous movement direction.
void CentipedeHead::SetMoveDirection(Grid* a_pGrid)
{
	switch (m_currentDirection)
	{
	case MOVE_DIRECTIONS::MOVE_DIRECTIONS_UP:
		{
			m_bIgnoreCollisions = true;
			// Sets the number of objects to ignore collisions with.
			// 2 yields results similar to the original game.
			m_uiIgnoreCount = 2;

			// Reverse the centipede's movement direction based on its 
			// previous horizontal movement direction.
			if (m_lastHorizontalDirection == MOVE_DIRECTIONS::MOVE_DIRECTIONS_LEFT)
			{
				// Now move right.
				m_moveDirection = m_moveDirection.Right();
				m_currentDirection = MOVE_DIRECTIONS::MOVE_DIRECTIONS_RIGHT;
			}
			else if (m_lastHorizontalDirection == MOVE_DIRECTIONS::MOVE_DIRECTIONS_RIGHT)
			{
				// Now move left.
				m_moveDirection = m_moveDirection.Left();
				m_currentDirection = MOVE_DIRECTIONS::MOVE_DIRECTIONS_LEFT;
			}

			if (m_pTargetCell)
			{
				// Align the centipede's y position with it's target cell's row.
				m_position.SetY(m_pTargetCell->GetPosition()->GetY());
			}

			SetTargetCell(a_pGrid);
			break;
		}
	case MOVE_DIRECTIONS::MOVE_DIRECTIONS_LEFT:
		{
			// Save our current movement direction so we can reverse our 
			// movement later. (This should be a horizontal direction now).
			m_lastHorizontalDirection = m_currentDirection;
			// Now we want to move down. Our coordinate system's axes measure 
			// moving down the screen as increasing along the y axis.
			m_moveDirection = m_verticalMoveDirection;
			SetTargetCell(a_pGrid);
			m_currentDirection = m_verticalDirection;
			break;
		}
	case MOVE_DIRECTIONS::MOVE_DIRECTIONS_DOWN:
		{
			m_bIgnoreCollisions = true;
			// Sets the number of objects to ignore collisions with.
			// 2 yields results similar to the original game.
			m_uiIgnoreCount = 2;

			// Reverse the centipede's movement direction based on its 
			// previous horizontal movement direction.
			if (m_lastHorizontalDirection == MOVE_DIRECTIONS::MOVE_DIRECTIONS_LEFT)
			{
				// Now move right.
				m_moveDirection = m_moveDirection.Right();
				m_currentDirection = MOVE_DIRECTIONS::MOVE_DIRECTIONS_RIGHT;
			}
			else if (m_lastHorizontalDirection == MOVE_DIRECTIONS::MOVE_DIRECTIONS_RIGHT)
			{
				// Now move left.
				m_moveDirection = m_moveDirection.Left();
				m_currentDirection = MOVE_DIRECTIONS::MOVE_DIRECTIONS_LEFT;
			}

			if (m_pTargetCell)
			{
				// Align the centipede's y position with it's target cell's row.
				m_position.SetY(m_pTargetCell->GetPosition()->GetY());
			}

			SetTargetCell(a_pGrid);
			break;
		}
	case MOVE_DIRECTIONS::MOVE_DIRECTIONS_RIGHT:
		{
			// Save our current movement direction so we can reverse our 
			// movement later. (This should be a horizontal direction now).
			m_lastHorizontalDirection = m_currentDirection;
			m_currentDirection = m_verticalDirection;
			// Now we want to move down. Our coordinate system's axes measure 
			// moving down the screen as increasing along the y axis.
			m_moveDirection = m_verticalMoveDirection;
			SetTargetCell(a_pGrid);
			break;
		}
	default:
		{
			break;
		}
	}
}

// Returns a pointer to the list of centipede bodies belonging to this head.
std::list<CentipedeBody*>* CentipedeHead::GetBodyList()
{
	return &m_bodyParts;
}