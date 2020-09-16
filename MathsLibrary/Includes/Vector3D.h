/////////////////////////////
// Author: Will Masek
// Date Created: 10/02/2020
/////////////////////////////

#ifndef VECTOR_3D
#define VECTOR_3D

class Vector2D;
class Matrix3x3;

class Vector3D
{
public:
	// Constructors.
	Vector3D(float a_x, float a_y, float a_z);
	Vector3D(Vector2D other);
	Vector3D();
	// Destructor.
	~Vector3D();

	// Operator Overloads.
	const float operator[](const int a_i) const;
	const Vector3D operator*(const float a_other) const;
	void operator*=(const float a_other);
	const Vector3D operator*(const Vector3D &a_other) const;
	void operator*=(const Vector3D &a_other);
	const Vector3D operator/(const Vector3D &a_other) const;
	void operator/=(const Vector3D &a_other);
	const Vector3D operator/(const float a_other) const;
	void operator/=(const float a_other);
	const Vector3D operator*(const Matrix3x3 &a_other) const;
	const Vector3D operator+(const Vector3D &a_other) const;
	void operator+=(const Vector3D &a_other);
	const Vector3D operator-(const Vector3D &a_other) const;
	void operator-=(const Vector3D &a_other);

	// Functions
	// Returns a float containing the vectors direction (length).
	const float magnitude() const;
	// Returns the squared length of this vector.
	const float magnitudeSq() const;
	// Returns the distance between this vector and another vector.
	const float distance(const Vector3D &a_other) const;
	// Returns the squared value of the distance between this vector and 
	// another vector.
	const float distanceSq(const Vector3D &a_other) const;
	// Returns a scalar value telling us the angle between two vectors but 
	// represented between -1 and 0.
	const float dotProduct(const Vector3D &a_other) const;
	// Returns a vector prependicular to this vector and the argument vector.
	const Vector3D crossProduct(const Vector3D & a_other) const;
	// Shortens the vectors length to a value between -1 and 1. A normalised 
	// vector with a magnitude of 1.
	void normalise();
	// Returns a vector with the value of this vector's length being shortend 
	// to a value between -1 and 1. A normalised vector is a vector with a 
	// magnitude of 1.
	const Vector3D normalised() const;
	void SetX(const float a_x);
	void SetY(const float a_y);
	void SetZ(const float a_z);
	// Gets a pointer to the vector's coordinates.
	const float* GetPointer() const;
	const float GetX() const;
	const float GetY() const;
	const float GetZ() const;

private:
	// Variables
	float m_coordinates[3];
};

#endif //!VECTOR_3D
