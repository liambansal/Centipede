/////////////////////////////
// Author: Liam Bansal
// Date Created: 20/05/2020
/////////////////////////////

// Includes.
#include "Clamp.h"

// Clamps an unsigned integer between a min and max value.
unsigned int Clamp::ClampInt(unsigned int value, unsigned int min, unsigned int max)
{
	unsigned int temp = 0;
	temp = value > max ? max : value;
	temp = value < min ? min : temp;
	return temp;
}

// Clamps a pointer to an unsigned integer between a min and max value.
void Clamp::ClampInt(unsigned int* a_pValue, unsigned int a_min, unsigned int a_max)
{
	if (a_pValue)
	{
		unsigned int temp = 0;
		temp = *a_pValue > a_max ? a_max : *a_pValue;
		temp = *a_pValue < a_min ? a_min : temp;
		*a_pValue = temp;
	}
}

// Clamps a float between a min and max value.
float Clamp::ClampFloat(float value, float min, float max)
{
	float temp = 0;
	temp = value > max ? max : value;
	temp = value < min ? min : temp;
	return temp;
}