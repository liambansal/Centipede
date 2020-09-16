/////////////////////////////
// Author: Will Masek
// Date Created: 10/02/2020
/////////////////////////////

#include "Vector3D.h" // File's header.
#include <cmath>
#include "Matrix3x3.h"
#include "Vector2D.h"

#pragma region Constructors/Destructors
Vector3D::Vector3D(float a_x, float a_y, float a_z)
{
	m_coordinates[0] = a_x;
	m_coordinates[1] = a_y;
	m_coordinates[2] = a_z;
}

Vector3D::Vector3D(Vector2D other)
{
	m_coordinates[0] = other.GetX();
	m_coordinates[1] = other.GetY();
	m_coordinates[2] = 0;
}

Vector3D::Vector3D()
{
	m_coordinates[0] = 0.f;
	m_coordinates[1] = 0.f;
	m_coordinates[2] = 0.f;
}

Vector3D::~Vector3D()
{}
#pragma endregion

#pragma region Functions Definitions
// Returns a float containing the vectors direction (length).
const float Vector3D::magnitude() const
{
	return sqrtf(powf(m_coordinates[0], 2) + powf(m_coordinates[1], 2) + powf(m_coordinates[2], 2));
}

// Returns the squared length of this vector.
const float Vector3D::magnitudeSq() const
{
	return sqrtf(magnitude());
}

// Returns the distance between this vector and another vector.
const float Vector3D::distance(const Vector3D& a_other) const
{
	return sqrtf((powf(m_coordinates[0] - a_other.m_coordinates[0], 2)) + (powf(m_coordinates[1] - a_other.m_coordinates[1], 2)) + (powf(m_coordinates[2] - a_other[2], 2)));
}

// Returns the squared value of the distance between this vector and 
// another vector.
const float Vector3D::distanceSq(const Vector3D& a_other) const
{
	return sqrtf(distance(a_other));
}

// Returns a scalar value telling us the angle between two vectors but 
// represented between -1 and 0.
const float Vector3D::dotProduct(const Vector3D& a_other) const
{
	return ((m_coordinates[0] * a_other.m_coordinates[0]) + (m_coordinates[1] * a_other.m_coordinates[1]) + (m_coordinates[2] * a_other.m_coordinates[2]));
}

// Returns a vector prependicular to this vector and the argument vector.
const Vector3D Vector3D::crossProduct(const Vector3D& a_other) const
{
	Vector3D crossProduct;
	crossProduct.m_coordinates[0] = m_coordinates[1] * a_other[2] - m_coordinates[2] * a_other[1];
	crossProduct.m_coordinates[1] = m_coordinates[2] * a_other[0] - m_coordinates[0] * a_other[2];
	crossProduct.m_coordinates[2] = m_coordinates[0] * a_other[1] - m_coordinates[1] * a_other[0];
	return crossProduct;
}

// Shortens the vectors length to a value between -1 and 1. A normalised 
// vector with a magnitude of 1.
void Vector3D::normalise()
{
	m_coordinates[0] /= magnitude();
	m_coordinates[1] /= magnitude();
	m_coordinates[2] /= magnitude();
}

// Returns a vector with the value of this vector's length being shortend 
// to a value between -1 and 1. A normalised vector is a vector with a 
// magnitude of 1.
const Vector3D Vector3D::normalised() const
{
	Vector3D normalisedVector(m_coordinates[0] / magnitude(), m_coordinates[1] / magnitude(), m_coordinates[2] / magnitude());
	return normalisedVector;
}

void Vector3D::SetX(const float a_x)
{
	m_coordinates[0] = a_x;
}

void Vector3D::SetY(const float a_y)
{
	m_coordinates[1] = a_y;
}

void Vector3D::SetZ(const float a_z)
{
	m_coordinates[2] = a_z;
}

// Gets a pointer to the vector's coordinates.
const float* Vector3D::GetPointer() const
{
	return m_coordinates;
}

const float Vector3D::GetX() const
{
	return m_coordinates[0];
}

const float Vector3D::GetY() const
{
	return m_coordinates[1];
}

const float Vector3D::GetZ() const
{
	return m_coordinates[2];
}
#pragma endregion

#pragma region Operator Overload Functions
const float Vector3D::operator[](const int a_i) const
{
	return m_coordinates[a_i];
}

const Vector3D Vector3D::operator*(const float a_other) const
{
	Vector3D vector3D;
	vector3D.m_coordinates[0] = m_coordinates[0] * a_other;
	vector3D.m_coordinates[1] = m_coordinates[1] * a_other;
	vector3D.m_coordinates[2] = m_coordinates[2] * a_other;
	return vector3D;
}

void Vector3D::operator*=(const float a_other)
{
	m_coordinates[0] *= a_other;
	m_coordinates[1] *= a_other;
	m_coordinates[2] *= a_other;
}

const Vector3D Vector3D::operator*(const Vector3D& a_other) const
{
	Vector3D vector3D;
	vector3D.m_coordinates[0] = m_coordinates[0] * a_other.m_coordinates[0];
	vector3D.m_coordinates[1] = m_coordinates[1] * a_other.m_coordinates[1];
	vector3D.m_coordinates[2] = m_coordinates[2] * a_other.m_coordinates[2];
	return vector3D;
}

void Vector3D::operator*=(const Vector3D& a_other)
{
	m_coordinates[0] *= a_other.m_coordinates[0];
	m_coordinates[1] *= a_other.m_coordinates[1];
	m_coordinates[2] *= a_other.m_coordinates[2];
}

const Vector3D Vector3D::operator/(const Vector3D& a_other) const
{
	Vector3D vector3D;
	vector3D.m_coordinates[0] = m_coordinates[0] / a_other.m_coordinates[0];
	vector3D.m_coordinates[1] = m_coordinates[1] / a_other.m_coordinates[1];
	vector3D.m_coordinates[2] = m_coordinates[2] / a_other.m_coordinates[2];
	return vector3D;
}

void Vector3D::operator/=(const Vector3D& a_other)
{
	m_coordinates[0] /= a_other.m_coordinates[0];
	m_coordinates[1] /= a_other.m_coordinates[1];
	m_coordinates[2] /= a_other.m_coordinates[2];
}

const Vector3D Vector3D::operator/(const float a_other) const
{
	Vector3D vector3D;
	vector3D.m_coordinates[0] = m_coordinates[0] / a_other;
	vector3D.m_coordinates[1] = m_coordinates[1] / a_other;
	vector3D.m_coordinates[2] = m_coordinates[2] / a_other;
	return vector3D;
}

void Vector3D::operator/=(const float a_other)
{
	m_coordinates[0] /= a_other;
	m_coordinates[1] /= a_other;
	m_coordinates[2] /= a_other;
}

const Vector3D Vector3D::operator*(const Matrix3x3& a_other) const
{
	Vector3D vector3D;
	vector3D.SetX((m_coordinates[0] * a_other[0]) + (m_coordinates[1] * a_other[3]) + (m_coordinates[2] * a_other[6]));
	vector3D.SetY((m_coordinates[0] * a_other[1]) + (m_coordinates[1] * a_other[4]) + (m_coordinates[2] * a_other[7]));
	vector3D.SetZ((m_coordinates[0] * a_other[2]) + (m_coordinates[1] * a_other[5]) + (m_coordinates[2] * a_other[8]));
	return vector3D;
}

const Vector3D Vector3D::operator+(const Vector3D& a_other) const
{
	Vector3D vector3D;
	vector3D.m_coordinates[0] = m_coordinates[0] + a_other.m_coordinates[0];
	vector3D.m_coordinates[1] = m_coordinates[1] + a_other.m_coordinates[1];
	vector3D.m_coordinates[2] = m_coordinates[2] + a_other.m_coordinates[2];
	return vector3D;
}

void Vector3D::operator+=(const Vector3D& a_other)
{
	m_coordinates[0] += a_other.m_coordinates[0];
	m_coordinates[1] += a_other.m_coordinates[1];
	m_coordinates[2] += a_other.m_coordinates[2];
}

const Vector3D Vector3D::operator-(const Vector3D& a_other) const
{
	Vector3D vector3D;
	vector3D.m_coordinates[0] = m_coordinates[0] - a_other.m_coordinates[0];
	vector3D.m_coordinates[1] = m_coordinates[1] - a_other.m_coordinates[1];
	vector3D.m_coordinates[2] = m_coordinates[2] - a_other.m_coordinates[2];
	return vector3D;
}

void Vector3D::operator-=(const Vector3D& a_other)
{
	m_coordinates[0] -= a_other.m_coordinates[0];
	m_coordinates[1] -= a_other.m_coordinates[1];
	m_coordinates[2] -= a_other.m_coordinates[2];
}
#pragma endregion