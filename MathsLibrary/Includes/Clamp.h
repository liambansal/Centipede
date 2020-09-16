/////////////////////////////
// Author: Liam Bansal
// Date Created: 20/05/2020
/////////////////////////////

#ifndef CLAMP_H
#define CLAMP_H

class Clamp
{
public:
	// Constructor.
	Clamp() {};
	// Destructor.
	~Clamp() {};

	// Clamps an unsigned integer between a min and max value.
	unsigned int ClampInt(unsigned int a_value, unsigned int a_min, unsigned int a_max);
	// Clamps a pointer to an unsigned integer between a min and max value.
	void ClampInt(unsigned int* a_pValue, unsigned int a_min, unsigned int a_max);
	// Clamps a float between a min and max value.
	float ClampFloat(float a_value, float a_min, float a_max);
};

#endif // !CLAMP_H
