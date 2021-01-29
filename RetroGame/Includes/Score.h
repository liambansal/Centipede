/////////////////////////////
// Author: Liam Bansal
// Date Created: 03/05/2020
/////////////////////////////

#ifndef SCORE_H
#define SCORE_H

#include <string>
#include "Vector2D.h"

class BugBlaster;
class GameStateManager;
class Vector2D;

class Score
{
public:
	Score();
	~Score();

	// Increases the players score by a set amount and checks if they should 
	// recieve extra lives.
	void Update(const unsigned int a_addition);
	// Draws the score onto the screen.
	void Draw() const;
	// Resets the score to 0.
	void ResetScore();
	// Sets the position to draw the score.
	void SetPosition(Vector2D* a_pPosition);
	// Assigns the state manager responsible for drawing the score.
	void SetStateManager(GameStateManager* a_pStateManager);
	// Sets the bug blaster responsible for increasing the score.
	void SetBugBlaster(BugBlaster* a_pBugBlaster);
	unsigned int GetScore() const;

private:
	// Score needed to gain extra life.
	const unsigned int mc_uiLifeMilestone;
	unsigned int m_uiScore;
	std::string m_scorePrefix;
	Vector2D m_position;
	GameStateManager* m_pStateManager;
	BugBlaster* m_pBugBlaster;
};

#endif // !SCORE_H.