/////////////////////////////
// Author: Liam Bansal
// Date Created: 14/05/2020
/////////////////////////////

// Includes.
#include "CentipedeManager.h" // File's header.
#include "CentipedeBody.h"
#include "CentipedeHead.h"
#include "GameplayState.h"
#include "Grid.h"
#include "RandGenerator.h"
#include "Vector2D.h"

CentipedeManager::CentipedeManager() : m_uiCentipedeLength(11),
	m_uiMaxCentipedeLength(11),
	m_bCentipedesSlain(false),
	m_bListChanged(false),
	m_centipedes(),
	m_centipedeHeads(),
	m_numberGenerator()
{}

CentipedeManager::~CentipedeManager()
{
	for (auto listIterator = m_centipedes.begin();
		listIterator != m_centipedes.end();
		++listIterator)
	{
		if (*listIterator)
		{
			delete (*listIterator);
			(*listIterator) = nullptr;
		}
	}
}

// Updates the manager's centipedes and when to spawn them.
void CentipedeManager::Update(GameplayState* a_pState,
	Grid* a_pGrid,
	float* a_pDeltaTime)
{
	if (a_pGrid && a_pGrid->IsDrawn())
	{
		// Spawns a new centipede (plus indiviudal heads) every time the level
		// is cleared of them.
		if (m_bCentipedesSlain)
		{
			if (m_uiCentipedeLength == 0)
			{
				m_uiCentipedeLength = m_uiMaxCentipedeLength;
			}

			a_pGrid->CountMushrooms(a_pState);
			SpawnCentipede(a_pGrid);
			m_bCentipedesSlain = false;
		}
		else
		{
			// Use for loops to update all our centipedes and their bodies.
			if (m_centipedes.size() > 0)
			{
				// Update the centipedes.
				for (auto listIterator = m_centipedes.begin();
					listIterator != m_centipedes.end();
					++listIterator)
				{
					if (*listIterator)
					{
						(*listIterator)->Update(a_pState,
							a_pGrid,
							a_pDeltaTime);
					}

					if (m_bListChanged)
					{
						m_bListChanged = false;
						break;
					}
				}
			}

			if (m_centipedeHeads.size() > 0)
			{
				// Update the cenitpede heads (centipedes spawned without bodies).
				for (auto listIterator = m_centipedeHeads.begin();
					listIterator != m_centipedeHeads.end();
					++listIterator)
				{
					(*listIterator).Update(a_pState,
						a_pGrid,
						a_pDeltaTime);

					if (m_bListChanged)
					{
						m_bListChanged = false;
						break;
					}
				}
			}

			// Check if all the centipedes have been slain.
			if (m_centipedes.size() == 0 &&
				m_centipedeHeads.size() == 0)
			{
				// There are no more centipedes in the level.
				m_bCentipedesSlain = true;
			}
		}
	}
}

// Draws the manager's centipedes onto the screen.
void CentipedeManager::Draw(GameplayState* a_pScene)
{
	// Draw the centipedes if they have not been modified so we don't 
	// dereference invalid data.
	if (!m_bListChanged)
	{
		if (m_centipedes.size() > 0)
		{
			// Draw the cenitpedes.
			for (auto listIterator = m_centipedes.begin();
				listIterator != m_centipedes.end();
				++listIterator)
			{
				if (*listIterator)
				{
					(*listIterator)->Draw(a_pScene);
				}
			}
		}

		if (m_centipedeHeads.size() > 0)
		{
			// Draw the cenitpede heads (centipedes spawned without bodies).
			for (auto listIterator = m_centipedeHeads.begin();
				listIterator != m_centipedeHeads.end();
				++listIterator)
			{
				(*listIterator).Draw(a_pScene);
			}
		}
	}
	else
	{
		m_bListChanged = false;
	}
}

// Spawn's the level's main centipede.
// Spawns additional heads when body centipede length decreases.
void CentipedeManager::SpawnCentipede(Grid* a_pGrid)
{
	if (a_pGrid)
	{
		// Spawns one head for each time the centipede body length has been 
		// decremented.
		for (unsigned int headCount = m_uiCentipedeLength;
			headCount < m_uiMaxCentipedeLength;
			++headCount)
		{
			CentipedeHead* pHead = new CentipedeHead();

			if (pHead->GetSprite())
			{
				// Generate a semi-random spawn position using the grid's top and right 
				// boundary positions, using the centipede sprite as a margin.
				Vector2D spawnPosition = Vector2D(m_numberGenerator.
					GetRandomFloat((float)a_pGrid->
						GetRightBoundary(pHead->GetSprite()->height * 0.5f)),
					(float)a_pGrid->
					GetUpperBoundary(pHead->GetSprite()->height * 0.5f));
				pHead->SetPosition(a_pGrid->GetCell(&spawnPosition).GetPosition());
				// Set how many bodies will follow after the head.
				pHead->SetManager(this);
				// Do not give this centipede a body.
				unsigned int bodyLength = 0;
				pHead->SetBodyLength(bodyLength);
				m_centipedes.push_back(pHead);
			}
		}

		CentipedeHead* pCentipedeHead = new CentipedeHead();

		if (pCentipedeHead->GetSprite())
		{
			// Generate a semi-random spawn position using the grid's top and right 
			// boundary positions, using the centipede sprite as a margin.
			Vector2D spawnPosition(m_numberGenerator.GetRandomFloat((float)a_pGrid->
				GetRightBoundary(pCentipedeHead->GetSprite()->height * 0.5f)),
				(float)a_pGrid->GetUpperBoundary(pCentipedeHead->GetSprite()->height * 0.5f));
			pCentipedeHead->SetPosition(a_pGrid->GetCell(&spawnPosition).GetPosition());
			// Set how many bodies will follow after the head.
			pCentipedeHead->CreateBody(a_pGrid, m_uiCentipedeLength);
			pCentipedeHead->SetManager(this);
			// Decrease the length each time we spawn a centipede.
			m_uiCentipedeLength--;
			m_centipedes.push_back(pCentipedeHead);
			m_bListChanged = true;
		}
	}
}

// Creates a new centipede and assigns it the split body of another
// centipede.
void CentipedeManager::CreateCentipede(Grid* a_pGrid,
	std::list<CentipedeBody*>* a_pBodyList)
{
	if (a_pGrid && a_pBodyList && a_pBodyList->front())
	{
		// Create a new dynamically allocated centipede head.
		CentipedeHead* pNewHead = new CentipedeHead();
		// Set the new head's position.
		pNewHead->SetPosition(a_pBodyList->front()->GetCurrentPosition());
		// Copy the head parameter's members to the new head so it acts the same.
		(*pNewHead->GetParent()) = (a_pBodyList->front()->GetParent());
		// Get rid of the first body part so we can replace it with the 
		// new centipede head. There may only be one body in the list, 
		// still do this and replace it with a head.
		a_pGrid->GetCell(a_pBodyList->front()->GetPreviousPosition()).ClearCentipede();
		a_pGrid->GetCell(a_pBodyList->front()->GetCurrentPosition()).ClearCentipede();
		// Deallocate the memory for the body list's first body part.
		delete a_pBodyList->front();
		a_pBodyList->front() = nullptr;
		// Remove the first body part from the body list.
		a_pBodyList->pop_front();

		// Check if the body list has any bodies.
		if (a_pBodyList->size() > 0)
		{
			// Assign the body list to our new head.
			// Replace each body part's head with the new head.
			for (auto listIterator = a_pBodyList->rbegin();
				listIterator != a_pBodyList->rend();
				++listIterator)
			{
				pNewHead->GetBodyList()->push_front(*listIterator);
				(*listIterator)->SetHead(pNewHead);
			}

			pNewHead->SetBodyLength((unsigned int)a_pBodyList->size());

			// Nullify the body list's pointers.
			for (auto listIterator = a_pBodyList->rbegin();
				listIterator != a_pBodyList->rend();
				++listIterator)
			{
				(*listIterator) = nullptr;
			}

			a_pBodyList = nullptr;
			pNewHead->SetTargetCell(a_pGrid);
			EraseTargetCells(pNewHead);
		}

		// Assign a manager to update and draw the new centipede.
		pNewHead->SetManager(this);
		// Add the new centipede to the manager's list of centipedes.
		m_centipedes.push_back(pNewHead);
		m_bListChanged = true;
	}
}

// Removes a centipede head from the manager's list of centipedes.
void CentipedeManager::RemoveCentipede(CentipedeHead* a_pHead)
{
	for (auto listIterator = m_centipedes.begin();
		listIterator != m_centipedes.end();
		++listIterator)
	{
		if (a_pHead == *listIterator)
		{
			m_centipedes.remove(a_pHead);
			// Notify the manager that a list has been modified.
			m_bListChanged = true;
			delete a_pHead;
			a_pHead = nullptr;
			break;
		}
	}
}

// Clears all of the manager's centipedes.
void CentipedeManager::ClearCentipedes(Grid* a_pGrid)
{
	if (a_pGrid)
	{
		// Did we spawn any centipedes with bodies.
		if (m_centipedes.size() > 0)
		{
			// Loops through the state's centipedes.
			for (auto headIterator = m_centipedes.begin();
				headIterator != m_centipedes.end();
				++headIterator)
			{
				// Check if the head has a body so we delete that as well.
				if ((*headIterator))
				{
					// Erase references to the cenitpede head's body parts.
					for (auto bodyIterator = (*headIterator)->GetBodyList()->begin();
						bodyIterator != (*headIterator)->GetBodyList()->end();
						++bodyIterator)
					{
						if ((*bodyIterator))
						{
							a_pGrid->GetCell((*bodyIterator)->GetPreviousPosition()).ClearCentipede();
							a_pGrid->GetCell((*bodyIterator)->GetCurrentPosition()).ClearCentipede();
						}

						// Deallocate the body part's memory because it was allocated dynamically.
						delete (*bodyIterator);
						(*bodyIterator) = nullptr;
					}

					(*headIterator)->GetBodyList()->clear();
					// Clear cell references to the centipede head.
					a_pGrid->GetCell((*headIterator)->GetPreviousPosition()).ClearCentipede();
					a_pGrid->GetCell((*headIterator)->GetCurrentPosition()).ClearCentipede();
				}

				// Deallocate the head's memory because it was allocated dynamically.
				delete (*headIterator);
				(*headIterator) = nullptr;
			}

			m_centipedes.clear();
			m_bListChanged = true;
		}

		// Did we spawn any centipedes without bodies.
		if (m_centipedeHeads.size() > 0)
		{
			// Loops through the states stand-alone centipede heads.
			for (auto headIterator = m_centipedeHeads.begin();
				headIterator != m_centipedeHeads.end();
				++headIterator)
			{
				// Clear cell references to the centipede head.
				a_pGrid->GetCell((headIterator)->GetPreviousPosition()).ClearCentipede();
				a_pGrid->GetCell((headIterator)->GetCurrentPosition()).ClearCentipede();
			}

			m_centipedeHeads.clear();
			m_bListChanged = true;
		}
	}
}

// Loops through a centipede's bodies and removes any target cells whose 
// positions are outisde of the centipede.
void CentipedeManager::EraseTargetCells(CentipedeHead* a_pHead)
{
	if (a_pHead)
	{
		// Erases all target cells up to the heads position.
		// Loop through each body part belonging to the new head.
		// Starts iterating from the body closest to the new head.
		for (auto bodyIterator = a_pHead->GetBodyList()->rbegin();
			bodyIterator != a_pHead->GetBodyList()->rend();
			++bodyIterator)
		{
			if ((*bodyIterator))
			{
				// List to hold values to remove.
				std::list<Cell*> cellsToRemove;

				// Loop through each target cell for current centipede body, 
				// starting from the beginning, and remove it until we reach the 
				// new head's position.
				for (auto cellIterator = (*bodyIterator)->GetTargetCells()->rbegin();
					cellIterator != (*bodyIterator)->GetTargetCells()->rend();
					++cellIterator)
				{
					if (*cellIterator)
					{
						if ((*cellIterator)->GetPosition() !=
							a_pHead->GetTargetCell()->GetPosition())
						{
							cellsToRemove.push_back(*cellIterator);
						}
						// Remember to add the head target cell so we delete up to 
						// (including) that target cell.
						else if ((*cellIterator)->GetPosition() ==
							a_pHead->GetTargetCell()->GetPosition())
						{
							cellsToRemove.push_back(*cellIterator);
							break;
						}
					}
				}

				std::list<Cell*>::iterator listIterator = cellsToRemove.begin();

				// Clear the list of unnecessary target cells. 
				while (listIterator != cellsToRemove.end())
				{
					// The back of the list is the front of the centipede.
					(*bodyIterator)->GetTargetCells()->pop_back();
					listIterator++;
				}

				// The centipede head will only have one target cell, which we 
				// have already deleted. Check if any taregt cells still exist 
				// before trying to delete another one. 
				if ((*bodyIterator)->GetTargetCells()->size() > 0)
				{
					// Removes the heads target cell position from its bodies' target 
					// cells lists, so they only know to move until they reach the 
					// head's position.
					(*bodyIterator)->GetTargetCells()->pop_back();
				}
			}
		}
	}
}