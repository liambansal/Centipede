/////////////////////////////
// Author: Liam Bansal
// Date Created: 04/04/2020
/////////////////////////////

// Includes.
#include "RandGenerator.h" // File's header.
#include <cmath>

RandGenerator::RandGenerator()
{}

RandGenerator::~RandGenerator()
{}

// Returns a semi-random number between 0 and the argument integer.
// Use srand() to create different sequences of numbers.
unsigned int RandGenerator::GetRandomInteger(const unsigned int maxInteger) const
{
	// Gets a semi-random double between 0 and 1.
	double randomDouble = (double)rand() / RAND_MAX;
	// Scales the double by the max integer to get a number between 0 and the 
	// max integer that has a difference between itself, 0 and the max integer 
	// roughly equal to the difference between the double, 0 and 1.
	return (unsigned int)(randomDouble *= maxInteger);
}

// Returns a semi-random number between 0 and the argument float.
// Use srand() to create different sequences of numbers.
float RandGenerator::GetRandomFloat(const float maxFloat) const
{
	// Gets a semi-random double between 0 and 1.
	double randomDouble = (double)rand() / RAND_MAX;
	// Scales the double by the max float to get a number between 0 and the 
	// max float that has a difference between itself, 0 and the max float 
	// roughly equal to the difference between the double, 0 and 1.
	return (float)(randomDouble *= maxFloat);
}
