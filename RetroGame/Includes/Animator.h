/////////////////////////////
// Author: Liam Bansal
// Date Created: 27/05/2020
/////////////////////////////

#ifndef ANIMATOR_H
#define ANIMATOR_H

// Includes.
#include "olcPixelGameEngine.h"
#include <list>
#include "Timer.h"

struct Animation {
	const char* pName = "NoName";
	bool bIsActive = false;
	bool bListChanged = false;
	std::list<olc::Sprite*> frames;
	std::list<olc::Sprite*>::iterator currentFrame;
};

class Animator
{
public:
	// Constructor.
	Animator();
	// Destructor.
	~Animator();

	// Functions.
	// Steps through an animations frames.
	void Update(float* a_pDeltaTime);
	// Adds an frame to an existing animation with the argument name,
	// otherwise creates a new animation for that frame.
	void AddFrame(const char* a_pAnimationName,
		olc::Sprite* a_sprite);
	// Removes an animation from the animators list.
	void RemoveAnimation(const char* a_pAnimationName);
	// Checks if an animation has reached its final frame.
	bool AnimationOver();
	// Sets the animation to be played.
	void SetAnimation(const char* a_pAnimationName);
	// Sets the length of time each frame is displayed.
	void SetFrameLength(const float a_frameLength);
	olc::Sprite* GetCurrentFrame() const;
	// Gets the name of the currently playing animation.
	const char* GetName() const;
	// Gets the state of the currently playing animation.
	bool GetState() const;

private:
	// Variables.
	float m_fFrameLength;
	bool m_bListChanged;
	Animation* m_pPreviousAnimation;
	std::list<Animation*> m_animations;
	std::list<Animation*>::iterator m_currentAnimation;
	Timer m_timer;
};

#endif // !ANIMATOR_H.