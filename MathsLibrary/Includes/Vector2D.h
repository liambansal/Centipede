/////////////////////////////
// Author: Will Masek
// Date Created: 10/02/2020
/////////////////////////////

#ifndef VECTOR_2D
#define VECTOR_2D

// Uses two floating points as coordinates that can be manipulated in 
// many different ways.
// Provides all the standard functionality expected from a traditional 
// Vector2D class.
class Vector2D
{
public:
	// Constructors.
	Vector2D();
	Vector2D(float a_x, float a_y);
	Vector2D(const Vector2D& other);
	// Destructor.
	~Vector2D();
	
	// Operator overloads.
	// Used to access the x or y coordinate of a vector.
	const float operator[](const int a_i) const;
	// Compares the values of two vector's coordinates.
	const bool operator==(Vector2D a_vector) const;
	// Returns a vector whose coordinates are the values of one vector's 
	// coordinates being multiplied by a scalar value.
	const Vector2D operator*(const float a_other) const;
	// Multiplies one vector's coordinates by a scalar value.
	void operator*=(const float a_other);
	// Returns a vector whose coordinates are the values of two vectors 
	// being multiplied.
	const Vector2D operator*(const Vector2D& a_other) const;
	// Multiplies a vector's coordinates by another vector's 
	// coordinates.
	void operator*=(const Vector2D& a_other);
	// Returns a vector whose coordinates are the values of two vectors 
	// being divided.
	const Vector2D operator/(const Vector2D& a_other) const;
	// Divides a vector's coordinates by another vector's 
	// coordinates.
	void operator/=(const Vector2D& a_other);
	// Returns a vector whose coordinates are the values of one vector's 
	// coordinates being divided by a scalar value.
	const Vector2D operator/(const float a_other) const;
	// Divides a vector's coordinates by a scalar value.
	void operator/=(const float a_other);
	// Returns a vector whose coordinates are the values of two vectors 
	// being added together.
	const Vector2D operator+(const Vector2D& a_other) const;
	// Increases a vector's coordinates by adding another vector's 
	// coordinates to them.
	void operator+=(const Vector2D& a_other);
	// Returns a vector whose coordinates are the values of two vectors 
	// being subtracted from one another.
	const Vector2D operator-(const Vector2D& a_other) const;
	// Decreases a vector's coordinates by subtracting another vector's 
	// coordinates from them.
	void operator-=(const Vector2D& a_other);

	// Functions
	// Returns a float containing the vectors direction (length).
	const float magnitude() const;
	// Returns the squared length of this vector.
	const float magnitudeSq() const;
	// Returns the distance between this vector and another vector.
	const float distance(const Vector2D& a_other) const;
	// Returns the squared value of the distance between this vector and 
	// another vector.
	const float distanceSq(const Vector2D& a_other) const;
	// Returns a float telling us how much this vector projects onto 
	// another, like getting the angle between them. If the vectors are 
	// normalised then the return value will be between -1 and 1.
	const float dotProduct(const Vector2D& a_other) const;
	// Shortens the vectors length to a value between -1 and 1. A normalised 
	// vector with a magnitude of 1.
	void normalise();
	// Returns a vector with the value of this vector's length being shortend 
	// to a value between -1 and 1. A normalised vector is a vector with a 
	// magnitude of 1.
	const Vector2D normalised() const;
	// Shorthand for writing a vector with an up direction.
	const Vector2D Up()	const;
	// Shorthand for writing a vector with a left direction.
	const Vector2D Left() const;
	// Shorthand for writing a vector with a down direction.
	const Vector2D Down() const;
	// Shorthand for writing a vector with a right direction.
	const Vector2D Right() const;
	void SetX(const float a_x);
	void SetY(const float a_y);
	// Gets a pointer to the vector's coordinates.
	const float* GetPointer() const;
	const float GetX() const;
	const float GetY() const;

private:
	// Variables
	// Stores the vector's x and y coordinate's in one place.
	float m_coordinates[2];
};

#endif //!VECTOR_2D