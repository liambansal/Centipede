/////////////////////////////
// Author: Liam Bansal
// Date Created: 04/04/2020
/////////////////////////////

#ifndef RANDGENERATOR_H
#define RANDGENERATOR_H

class RandGenerator
{
public:
	// Constructor.
	RandGenerator();
	// Destructor.
	~RandGenerator();

	// Returns a semi-random number between 0 and the argument integer.
	// Use srand() to create different sequences of numbers.
	unsigned int GetRandomInteger(const unsigned int maxInteger) const;
	// Returns a semi-random number between 0 and the argument float.
	// Use srand() to create different sequences of numbers.
	float GetRandomFloat(const float maxFloat) const;
};

#endif // RANDGENERATOR_H.