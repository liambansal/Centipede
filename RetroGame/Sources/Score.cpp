/////////////////////////////
// Author: Liam Bansal
// Date Created: 03/05/2020
/////////////////////////////

// Includes.
#include "Score.h" // File's header.
#include "BugBlaster.h"
#include "GameStateManager.h"

Score::Score() : m_uiScore(0),
	m_siScoreToNextMilestone(12000),
	m_uiLifeMilestone(12000),
	m_scorePrefix("Score: "),
	m_position(),
	m_pStateManager(nullptr),
	m_pBugBlaster(nullptr)
{}

Score::~Score() 
{}

// Increases the players score by a set amount and checks if they should 
// recieve extra lives.
void Score::Update(const unsigned int a_addition)
{
	if (m_pBugBlaster)
	{
		m_uiScore += a_addition;
		m_siScoreToNextMilestone -= a_addition;

		if (m_siScoreToNextMilestone <= 0)
		{
			m_pBugBlaster->AddLife();
			m_siScoreToNextMilestone = m_uiLifeMilestone;
		}
	}
}

// Draws the score onto the screen.
void Score::Draw() const
{
	if (m_pStateManager)
	{
		m_pStateManager->DrawString((int32_t)m_position.GetX(),
			(int32_t)m_position.GetY(), 
			m_scorePrefix + std::to_string(m_uiScore), 
			olc::WHITE);
	}
}

// Resets the score to 0.
void Score::ResetScore()
{
	m_uiScore = 0;
}

// Sets the position to draw the score.
void Score::SetPosition(Vector2D* a_pPosition)
{
	m_position = (*a_pPosition);
}

// Assigns the state manager responsible for drawing the score.
void Score::SetStateManager(GameStateManager* a_pStateManager)
{
	m_pStateManager = a_pStateManager;
}

// Sets the bug blaster responsible for increasing the score.
void Score::SetBugBlaster(BugBlaster* a_pBugBlaster)
{
	m_pBugBlaster = a_pBugBlaster;
}

unsigned int Score::GetScore() const
{
	return m_uiScore;
}