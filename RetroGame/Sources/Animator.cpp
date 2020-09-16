/////////////////////////////
// Author: Liam Bansal
// Date Created: 27/05/2020
/////////////////////////////

// Includes.
#include "Animator.h"

Animator::Animator() : m_fFrameLength(0.1f),
	m_bListChanged(false),
	m_pPreviousAnimation(nullptr),
	m_animations(),
	m_currentAnimation()
{}

Animator::~Animator()
{
	// If any memory for sprites has been allocated dynamically...
	if (m_animations.size() > 0)
	{
		// ...loop through the animation(s)...
		for (auto animationIterator = m_animations.begin();
			animationIterator != m_animations.end();
			++animationIterator)
		{
			// ...and deallocate all of the memory for the sprites.
			for (auto spriteIterator = (*animationIterator)->frames.begin();
				spriteIterator != (*animationIterator)->frames.end();
				++spriteIterator)
			{
				if (*spriteIterator)
				{
					delete (*spriteIterator);
					(*spriteIterator) = nullptr;
				}
			}

			if (*animationIterator)
			{
				delete *(*animationIterator)->currentFrame;
				*(*animationIterator)->currentFrame = nullptr;
			}

			if (*animationIterator)
			{
				delete (*animationIterator);
				(*animationIterator) = nullptr;
			}
		}

		(*m_currentAnimation) = nullptr;
	}
}

// Steps through an animations frames.
void Animator::Update(float* a_pDeltaTime)
{
	if (*m_currentAnimation)
	{
		if (!m_bListChanged &&
			!(*m_currentAnimation)->bListChanged)
		{
			// Get a list iterator to animate.
			m_timer.Update(a_pDeltaTime);

			// If time passed is greater than frame length...
			if (m_timer.GetRunLength() > m_fFrameLength)
			{
				// ...increment the current animation's frame to the next one 
				// in the list.
				++(*m_currentAnimation)->currentFrame;
				m_timer.Reset();

				if ((*m_currentAnimation)->currentFrame ==
					(*m_currentAnimation)->frames.end())
				{
					(*m_currentAnimation)->currentFrame =
						(*m_currentAnimation)->frames.begin();
				}
			}
		}
		else
		{
			m_bListChanged = false;
			(*m_currentAnimation)->bListChanged = false;
		}
	}
}

// Adds an frame to an existing animation with the argument name, otherwise 
// creates a new animation for that frame.
void Animator::AddFrame(const char* a_pAnimationName,
	olc::Sprite* a_sprite)
{
	// Tracks whether or not an animation with the parameter name already
	// exists.
	bool animationExists = false;

	for (auto listIterator = m_animations.begin();
		listIterator != m_animations.end();
		++listIterator)
	{
		if (a_pAnimationName == (*listIterator)->pName)
		{
			animationExists = true;
			// Add the sprite to the existing animation.
			(*listIterator)->frames.push_back(a_sprite);
			(*listIterator)->bListChanged = true;
			return;
		}
	}

	if (!animationExists)
	{
		// Create a new animation for this sprite.
		Animation* animation = new Animation();

		if (animation)
		{
			// Set its name so we can identify it in the future.
			animation->pName = a_pAnimationName;
			// Add the parameter sprite to its list of sprites.
			animation->frames.push_back(a_sprite);
			animation->bListChanged = true;
			animation->currentFrame = animation->frames.begin();
			m_animations.push_back(animation);
			m_bListChanged = true;
		}
	}
}

// Removes an animation from the animators list.
void Animator::RemoveAnimation(const char* a_pAnimationName)
{
	if (m_animations.size() > 0)
	{
		for (auto listIterator = m_animations.begin();
			listIterator != m_animations.end();
			++listIterator)
		{
			if (a_pAnimationName == (*listIterator)->pName)
			{
				m_animations.remove(*listIterator);
				m_bListChanged = true;
				return;
			}
		}
	}
}

// Checks if an animation has reached its final frame.
bool Animator::AnimationOver()
{
	if (m_pPreviousAnimation)
	{
		if ((*m_pPreviousAnimation->currentFrame) ==
			m_pPreviousAnimation->frames.back())
		{
			m_pPreviousAnimation->currentFrame =
				m_pPreviousAnimation->frames.begin();
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return true;
	}
}

// Sets the animation to be played.
void Animator::SetAnimation(const char* a_pAnimationName)
{
	// Disable any previous animations.
	if (m_pPreviousAnimation)
	{
		m_pPreviousAnimation->bIsActive = false;
	}

	for (auto listIterator = m_animations.begin();
		listIterator != m_animations.end();
		++listIterator)
	{
		// Check if the parameter name is the same as the currently examined 
		// animation.
		if (a_pAnimationName == (*listIterator)->pName)
		{
			if (!(*listIterator)->bIsActive)
			{
				// Set the animation to be active and save it's name so we know 
				// which one to update.
				(*listIterator)->bIsActive = true;
				m_currentAnimation = listIterator;
				m_pPreviousAnimation = (*m_currentAnimation);
				(*m_currentAnimation)->currentFrame =
					(*m_currentAnimation)->frames.begin();
				return;
			}
		}
	}
}

// Sets the length of time each frame is displayed.
void Animator::SetFrameLength(const float a_frameLength)
{
	m_fFrameLength = a_frameLength;
}

olc::Sprite* Animator::GetCurrentFrame() const
{
	if (m_currentAnimation != m_animations.end())
	{
		return (*(*m_currentAnimation)->currentFrame);
	}
}

// Gets the name of the currently playing animation.
const char* Animator::GetName() const
{
	if (m_currentAnimation != m_animations.end())
	{
		return (*m_currentAnimation)->pName;
	}
}

// Gets the state of the currently playing animation.
bool Animator::GetState() const
{
	if (m_currentAnimation != m_animations.end())
	{
		return (*m_currentAnimation)->bIsActive;
	}
}