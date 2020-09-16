/////////////////////////////
// Author: Liam Bansal
// Date Created: 20/05/2020
/////////////////////////////

#ifndef TIMER_H
#define TIMER_H

class Timer
{
public:
	// Constructor.
	Timer();
	// Destuctor.
	~Timer();

	// Functions.
	// Updates the timer's delta time and total run length.
	void Update(float* a_pDeltaTime);
	// Resets the timer's total run length to zero.
	void Reset();
	// Returns the length of time (in seconds) between the two most recent 
	// Update calls.
	float GetDeltaTime() const;
	// Returns the total length of time the timer has been running.
	double GetRunLength() const;

private:
	// Variables.
	// Length of time (in seconds) between the two latest Update calls.
	float* m_pDeltaTime;
	// Total amount of time the timer has been running for.
	double m_runLength;
};

#endif // !TIMER_H.