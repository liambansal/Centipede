/////////////////////////////
// Author: Liam Bansal
// Date Created: 20/05/2020
/////////////////////////////

// Includes.
#include "Timer.h"

Timer::Timer() : m_fDeltaTime(0.0f),
	m_runLength(0.0)
{}

Timer::~Timer()
{}

// Updates the timer's delta time and total run length.
void Timer::Update(float* a_pDeltaTime)
{
	m_fDeltaTime = *a_pDeltaTime;
	m_runLength += *a_pDeltaTime;
}

// Resets the timer's total run length to zero.
void Timer::Reset()
{
	m_runLength = 0.0f;
}

// Returns the length of time (in seconds) between the two most recent 
// Update calls.
float Timer::GetDeltaTime() const
{
	return m_fDeltaTime;
}

// Returns the total length of time the timer has been running.
double Timer::GetRunLength() const
{
	return m_runLength;
}