/////////////////////////////
// Author: Will Masek
// Date Created: 10/02/2020
/////////////////////////////

#include "Vector2D.h" // File's header.
#include <cmath>

#pragma region Constructors/Destructors
Vector2D::Vector2D()
{
	m_coordinates[0] = 0.0f;
	m_coordinates[1] = 0.0f;
}

Vector2D::Vector2D(float a_x, float a_y)
{
	m_coordinates[0] = a_x;
	m_coordinates[1] = a_y;
}

Vector2D::Vector2D(const Vector2D& other)
{
	m_coordinates[0] = other.m_coordinates[0];
	m_coordinates[1] = other.m_coordinates[1];
}

Vector2D::~Vector2D()
{}
#pragma endregion

#pragma region Functions Definitions
// Returns a float containing the vectors direction (length).
const float Vector2D::magnitude() const
{
	return sqrtf(powf(m_coordinates[0], 2.0f) + powf(m_coordinates[1], 2.0f));
}

// Returns the squared length of this vector.
const float Vector2D::magnitudeSq() const
{
	return sqrtf(magnitude());
}

// Returns the distance between this vector and another vector.
const float Vector2D::distance(const Vector2D& a_other) const
{
	return sqrtf(powf(m_coordinates[0] - a_other.m_coordinates[0], 2.0f) + powf(m_coordinates[1] - a_other.m_coordinates[1], 2.0f));
}

// Returns the squared value of the distance between this vector and 
// another vector.
const float Vector2D::distanceSq(const Vector2D& a_other) const
{
	return sqrtf(sqrtf(powf(m_coordinates[0] - a_other.m_coordinates[0], 2.0f) + powf(m_coordinates[1] - a_other.m_coordinates[1], 2.0f)));
}

// Returns a float telling us how much this vector projects onto 
// another, like getting the angle between them. If the vectors are 
// normalised then the return value will be between -1 and 1.
const float Vector2D::dotProduct(const Vector2D& a_other) const
{
	return ((m_coordinates[0] * a_other.m_coordinates[0]) + (m_coordinates[1] * a_other.m_coordinates[1]));
}

// Shortens the vectors length to a value between -1 and 1.
void Vector2D::normalise()
{
	m_coordinates[0] /= magnitude();
	m_coordinates[1] /= magnitude();
}

// Returns a vector with the value of this vector's length being shortend 
// to a value between -1 and 1. A normalised vector is a vector with a 
// magnitude of 1.
const Vector2D Vector2D::normalised() const
{
	Vector2D vector2D(m_coordinates[0] / magnitude(), m_coordinates[1] / magnitude());
	return vector2D;
}

// Shorthand for writing a vector with an up direction.
const Vector2D Vector2D::Up()	const
{
	Vector2D vector(0.0f, 1.0f); return vector;
}

// Shorthand for writing a vector with a left direction.
const Vector2D Vector2D::Left() const
{
	Vector2D vector(-1.0f, 0.0f); return vector;
}

// Shorthand for writing a vector with a down direction.
const Vector2D Vector2D::Down() const
{
	Vector2D vector(0.0f, -1.0f); return vector;
}

// Shorthand for writing a vector with a right direction.
const Vector2D Vector2D::Right() const
{
	Vector2D vector(1.0f, 0.0f); return vector;
}

void Vector2D::SetX(const float a_x)
{
	m_coordinates[0] = a_x;
}

void Vector2D::SetY(const float a_y)
{
	m_coordinates[1] = a_y;
}

// Gets a pointer to the vector's coordinates.
const float* Vector2D::GetPointer() const
{
	return m_coordinates;
}

const float Vector2D::GetX() const
{
	return m_coordinates[0];
}

const float Vector2D::GetY() const
{
	return m_coordinates[1];
}
#pragma endregion

#pragma region Operator Overload Functions
// Used to access the x or y coordinate of a vector.
const float Vector2D::operator[](const int a_i) const
{
	return m_coordinates[a_i];
}

// Compares the values of two vector's coordinates.
const bool Vector2D::operator==(Vector2D a_vector) const
{
	if (GetX() == a_vector.GetX() &&
		GetY() == a_vector.GetY())
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Returns a vector whose coordinates are the values of one vector's 
// coordinates being multiplied by a scalar value.
const Vector2D Vector2D::operator*(const float a_other) const
{
	Vector2D vector2D;
	vector2D.m_coordinates[0] = m_coordinates[0] * a_other;
	vector2D.m_coordinates[1] = m_coordinates[1] * a_other;
	return vector2D;
}

// Multiplies one vector's coordinates by a scalar value.
void Vector2D::operator*=(const float a_other)
{
	m_coordinates[0] *= a_other;
	m_coordinates[1] *= a_other;
}

// Returns a vector whose coordinates are the values of two vectors 
// being multiplied.
const Vector2D Vector2D::operator*(const Vector2D& a_other) const
{
	Vector2D vector2D;
	vector2D.m_coordinates[0] = m_coordinates[0] * a_other.m_coordinates[0];
	vector2D.m_coordinates[1] = m_coordinates[1] * a_other.m_coordinates[1];
	return vector2D;
}

// Multiplies a vector's coordinates by another vector's 
// coordinates.
void Vector2D::operator*=(const Vector2D& a_other)
{
	m_coordinates[0] *= a_other.m_coordinates[0];
	m_coordinates[1] *= a_other.m_coordinates[1];
}

// Returns a vector whose coordinates are the values of two vectors 
// being divided.
const Vector2D Vector2D::operator/(const Vector2D& a_other) const
{
	Vector2D vector2D;
	vector2D.m_coordinates[0] = m_coordinates[0] / a_other.m_coordinates[0];
	vector2D.m_coordinates[1] = m_coordinates[1] / a_other.m_coordinates[1];
	return vector2D;
}

// Divides a vector's coordinates by another vector's 
// coordinates.
void Vector2D::operator/=(const Vector2D& a_other)
{
	m_coordinates[0] /= a_other.m_coordinates[0];
	m_coordinates[1] /= a_other.m_coordinates[1];
}

// Returns a vector whose coordinates are the values of one vector's 
// coordinates being divided by a scalar value.
const Vector2D Vector2D::operator/(const float a_other) const
{
	Vector2D vector2D;
	vector2D.m_coordinates[0] = m_coordinates[0] / a_other;
	vector2D.m_coordinates[1] = m_coordinates[1] / a_other;
	return vector2D;
}

// Divides a vector's coordinates by a scalar value.
void Vector2D::operator/=(const float a_other)
{
	m_coordinates[0] /= a_other;
	m_coordinates[1] /= a_other;
}

// Returns a vector whose coordinates are the values of two vectors 
// being added together.
const Vector2D Vector2D::operator+(const Vector2D& a_other) const
{
	Vector2D vector2D;
	vector2D.m_coordinates[0] = m_coordinates[0] + a_other.m_coordinates[0];
	vector2D.m_coordinates[1] = m_coordinates[1] + a_other.m_coordinates[1];
	return vector2D;
}

// Increases a vector's coordinates by adding another vector's 
// coordinates to them.
void Vector2D::operator+=(const Vector2D& a_other)
{
	m_coordinates[0] += a_other.m_coordinates[0];
	m_coordinates[1] += a_other.m_coordinates[1];
}

// Returns a vector whose coordinates are the values of two vectors 
// being subtracted from one another.
const Vector2D Vector2D::operator-(const Vector2D& a_other) const
{
	Vector2D vector2D;
	vector2D.m_coordinates[0] = m_coordinates[0] - a_other.m_coordinates[0];
	vector2D.m_coordinates[1] = m_coordinates[1] - a_other.m_coordinates[1];
	return vector2D;
}

// Decreases a vector's coordinates by subtracting another vector's 
// coordinates from them.
void Vector2D::operator-=(const Vector2D& a_other)
{
	m_coordinates[0] -= a_other.m_coordinates[0];
	m_coordinates[1] -= a_other.m_coordinates[1];
}
#pragma endregion