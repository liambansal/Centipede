/////////////////////////////
// Author: Will Masek
// Date Created: 10/02/2020
/////////////////////////////

#include "Matrix3x3.h" // File's header.
#include <cmath>
#include "Vector3D.h"

#pragma region Constructors/destructors.
Matrix3x3::Matrix3x3()
{
	m_fMatrixValues[0] = 0.0f;
	m_fMatrixValues[1] = 0.0f;
	m_fMatrixValues[2] = 0.0f;
	m_fMatrixValues[3] = 0.0f;
	m_fMatrixValues[4] = 0.0f;
	m_fMatrixValues[5] = 0.0f;
	m_fMatrixValues[6] = 0.0f;
	m_fMatrixValues[7] = 0.0f;
	m_fMatrixValues[8] = 0.0f;
}

Matrix3x3::Matrix3x3(const float a_fm11,
	const float a_fm12,
	const float a_fm13,
	const float a_fm21,
	const float a_fm22,
	const float a_fm23,
	const float a_fm31,
	const float a_fm32,
	const float a_fm33)
{
	m_fMatrixValues[0] = a_fm11;
	m_fMatrixValues[1] = a_fm12;
	m_fMatrixValues[2] = a_fm13;
	m_fMatrixValues[3] = a_fm21;
	m_fMatrixValues[4] = a_fm22;
	m_fMatrixValues[5] = a_fm23;
	m_fMatrixValues[6] = a_fm31;
	m_fMatrixValues[7] = a_fm32;
	m_fMatrixValues[8] = a_fm33;
}

Matrix3x3::Matrix3x3(const float* a_fp3x3MatrixValues)
{
	m_fMatrixValues[0] = a_fp3x3MatrixValues[0];
	m_fMatrixValues[1] = a_fp3x3MatrixValues[1];
	m_fMatrixValues[2] = a_fp3x3MatrixValues[2];
	m_fMatrixValues[3] = a_fp3x3MatrixValues[3];
	m_fMatrixValues[4] = a_fp3x3MatrixValues[4];
	m_fMatrixValues[5] = a_fp3x3MatrixValues[5];
	m_fMatrixValues[6] = a_fp3x3MatrixValues[6];
	m_fMatrixValues[7] = a_fp3x3MatrixValues[7];
	m_fMatrixValues[8] = a_fp3x3MatrixValues[8];
}

Matrix3x3::~Matrix3x3()
{}
#pragma endregion

#pragma region Functions Definitions.
// Static funtions.
const Matrix3x3 Matrix3x3::Identity()
{
	return Matrix3x3(
		1, 0, 0,
		0, 1, 0, 
		0, 0, 1
	);
}

// Flips the matrix's values along its main diagonal.
void Matrix3x3::Transpose()
{
	Matrix3x3 newMatrix;

	unsigned int previousFloat = sizeof(m_fMatrixValues) / sizeof(float) - 1;
	unsigned int mainDiagonalElement = 4;

	for (unsigned int arrayElement = 0;
		arrayElement < sizeof(m_fMatrixValues) / sizeof(float);
		++arrayElement, --previousFloat)
	{
		// Don't change the matrix's main diagonal line.
		// Every fourth element will be a part of this diagonal line becuase 
		// its a 3x3 matrix. Modulus of i will confirm when where examining a 
		// value along this line.
		if (arrayElement % mainDiagonalElement != 0)
		{
			newMatrix.m_fMatrixValues[previousFloat] = m_fMatrixValues[arrayElement];
		}
	}

	m_fMatrixValues[1] = newMatrix.m_fMatrixValues[5];
	m_fMatrixValues[2] = newMatrix.m_fMatrixValues[2];
	m_fMatrixValues[3] = newMatrix.m_fMatrixValues[7];
	m_fMatrixValues[5] = newMatrix.m_fMatrixValues[1];
	m_fMatrixValues[6] = newMatrix.m_fMatrixValues[6];
	m_fMatrixValues[7] = newMatrix.m_fMatrixValues[3];
}

Matrix3x3 Matrix3x3::MinorMatrix()
{
	Matrix3x3 minorMatrix(
		m_fMatrixValues[4] * m_fMatrixValues[8] -
		m_fMatrixValues[7] * m_fMatrixValues[5],
		m_fMatrixValues[3] * m_fMatrixValues[8] -
		m_fMatrixValues[6] * m_fMatrixValues[5],
		m_fMatrixValues[3] * m_fMatrixValues[7] -
		m_fMatrixValues[6] * m_fMatrixValues[4],

		m_fMatrixValues[1] * m_fMatrixValues[8] -
		m_fMatrixValues[7] * m_fMatrixValues[2],
		m_fMatrixValues[0] * m_fMatrixValues[8] -
		m_fMatrixValues[6] * m_fMatrixValues[2],
		m_fMatrixValues[0] * m_fMatrixValues[7] -
		m_fMatrixValues[6] * m_fMatrixValues[1],

		m_fMatrixValues[1] * m_fMatrixValues[5] -
		m_fMatrixValues[4] * m_fMatrixValues[2],
		m_fMatrixValues[0] * m_fMatrixValues[5] -
		m_fMatrixValues[3] * m_fMatrixValues[2],
		m_fMatrixValues[0] * m_fMatrixValues[4] -
		m_fMatrixValues[3] * m_fMatrixValues[1]
	);

	return minorMatrix;
}

Matrix3x3 Matrix3x3::CofactorMatrix(Matrix3x3& a_minorMatrix)
{
	Matrix3x3 cofactorMatrix(
		a_minorMatrix[0] > 0.0f ? a_minorMatrix[0] :
		a_minorMatrix[0] < 0.0f ? a_minorMatrix[0] : a_minorMatrix[0],
		a_minorMatrix[1] < 0.0f ? a_minorMatrix[1] - a_minorMatrix[1] * 2 :
		a_minorMatrix[1] > 0.0f ? -a_minorMatrix[1] : a_minorMatrix[1] -
		a_minorMatrix[1] * 2,
		a_minorMatrix[2] > 0.0f ? a_minorMatrix[2] :
		a_minorMatrix[2] < 0.0f ? a_minorMatrix[2] : a_minorMatrix[2],

		a_minorMatrix[3] < 0.0f ? a_minorMatrix[3] - a_minorMatrix[3] * 2 :
		a_minorMatrix[3] > 0.0f ? -a_minorMatrix[3] : a_minorMatrix[3] -
		a_minorMatrix[3] * 2,
		a_minorMatrix[4] > 0.0f ? a_minorMatrix[4] :
		a_minorMatrix[4] < 0.0f ? a_minorMatrix[4] : a_minorMatrix[4],
		a_minorMatrix[5] < 0.0f ? a_minorMatrix[5] - a_minorMatrix[5] * 2 :
		a_minorMatrix[5] > 0.0f ? -a_minorMatrix[5] : a_minorMatrix[5] -
		a_minorMatrix[5] * 2,

		a_minorMatrix[6] > 0.0f ? a_minorMatrix[6] :
		a_minorMatrix[6] < 0.0f ? a_minorMatrix[6] : a_minorMatrix[6],
		a_minorMatrix[7] < 0.0f ? a_minorMatrix[7] - a_minorMatrix[7] * 2 :
		a_minorMatrix[7] > 0.0f ? -a_minorMatrix[7] : a_minorMatrix[7] -
		a_minorMatrix[7] * 2,
		a_minorMatrix[8] > 0.0f ? a_minorMatrix[8] :
		a_minorMatrix[8] < 0.0f ? a_minorMatrix[8] : a_minorMatrix[8]
	);

	return cofactorMatrix;
}

// Multiplying a matrix by its inverse evaluates to the matrix's identity.
Matrix3x3 Matrix3x3::Inverse(Matrix3x3& a_cofactorMatrix)
{
	float determinent = (m_fMatrixValues[0] * a_cofactorMatrix[0]) +
		(m_fMatrixValues[1] * a_cofactorMatrix[1]) +
		(m_fMatrixValues[2] * a_cofactorMatrix[2]);

	Matrix3x3 adjugateMatrix = a_cofactorMatrix;
	adjugateMatrix.Transpose();

	Matrix3x3 inverseMatrix(
		adjugateMatrix[0] / determinent,
		adjugateMatrix[1] / determinent,
		adjugateMatrix[2] / determinent,
		adjugateMatrix[3] / determinent,
		adjugateMatrix[4] / determinent,
		adjugateMatrix[5] / determinent,
		adjugateMatrix[6] / determinent,
		adjugateMatrix[7] / determinent,
		adjugateMatrix[8] / determinent
	);

	return inverseMatrix;
}

void Matrix3x3::SetRow(const int a_irowNumber, const Vector3D& a_rowValues)
{
	if (a_irowNumber == 1)
	{
		m_fMatrixValues[0] = a_rowValues[0];
		m_fMatrixValues[1] = a_rowValues[1];
		m_fMatrixValues[2] = a_rowValues[2];
	}
	else if (a_irowNumber == 2)
	{
		m_fMatrixValues[3] = a_rowValues[0];
		m_fMatrixValues[4] = a_rowValues[1];
		m_fMatrixValues[5] = a_rowValues[2];
	}
	else if (a_irowNumber == 3)
	{
		m_fMatrixValues[6] = a_rowValues[0];
		m_fMatrixValues[7] = a_rowValues[1];
		m_fMatrixValues[8] = a_rowValues[2];
	}
}

const Vector3D Matrix3x3::GetRow(const int a_irowNumber) const
{
	if (a_irowNumber == 1)
	{
		Vector3D tempVector3D(m_fMatrixValues[0],
			m_fMatrixValues[1],
			m_fMatrixValues[2]);
		return tempVector3D;
	}
	else if (a_irowNumber == 2)
	{
		Vector3D tempVector3D(m_fMatrixValues[3],
			m_fMatrixValues[4],
			m_fMatrixValues[5]);
		return tempVector3D;
	}
	else if (a_irowNumber == 3)
	{
		Vector3D tempVector3D(m_fMatrixValues[6],
			m_fMatrixValues[7],
			m_fMatrixValues[8]);
		return tempVector3D;
	}
	else
	{
		return Vector3D(0, 0, 0);
	}
}

// Rotations.
const Matrix3x3 Matrix3x3::RotateX(const float a_ftheta) const
{
	return Matrix3x3(
		1, 0, 0,
		0, cosf(a_ftheta), -sinf(a_ftheta),
		0, sinf(a_ftheta), cosf(a_ftheta));
}

const Matrix3x3 Matrix3x3::RotateY(const float a_ftheta) const
{
	return Matrix3x3(
		cosf(a_ftheta), 0, sinf(a_ftheta),
		0, 1, 0,
		-sinf(a_ftheta), 0, cosf(a_ftheta));
}

const Matrix3x3 Matrix3x3::RotateZ(const float a_ftheta) const
{
	return Matrix3x3(
		cosf(a_ftheta), -sinf(a_ftheta), 0,
		sinf(a_ftheta), cosf(a_ftheta), 0,
		2, 1, 1);
}
#pragma endregion

#pragma region Operator Overload Definitions.
const Matrix3x3 Matrix3x3::operator+(const Matrix3x3& a_other) const
{
	Matrix3x3 matrix3x3;

	for (int i = 0; i < (smc_uiRows * smc_uiColumns); ++i)
	{
		matrix3x3.m_fMatrixValues[i] = m_fMatrixValues[i] + a_other[i];
	}

	return matrix3x3;
}

void Matrix3x3::operator+=(const Matrix3x3& a_other)
{
	for (int i = 0; i < (smc_uiRows * smc_uiColumns); ++i)
	{
		m_fMatrixValues[i] += a_other[i];
	}
}

const Matrix3x3 Matrix3x3::operator-(const Matrix3x3& a_other) const
{
	Matrix3x3 matrix3x3;

	for (int i = 0; i < (smc_uiRows * smc_uiColumns); ++i)
	{
		matrix3x3.m_fMatrixValues[i] = m_fMatrixValues[i] - a_other[i];
	}

	return matrix3x3;
}

void Matrix3x3::operator-=(const Matrix3x3& a_other)
{
	for (int i = 0; i < (smc_uiRows * smc_uiColumns); ++i)
	{
		m_fMatrixValues[i] -= a_other[i];
	}
}

const Matrix3x3 Matrix3x3::operator*(const Matrix3x3& a_other) const
{
	Matrix3x3 matrix3x3;
	Vector3D matrixRow;
	Vector3D matrixColumn;
	int i = 0;

	for (int x = 1; x <= smc_uiRows; ++x)
	{
		matrixRow = GetRow(x);

		for (int y = 0; y < smc_uiColumns; ++y)
		{
			matrixColumn.SetX(a_other.m_fMatrixValues[y]);
			matrixColumn.SetY(a_other.m_fMatrixValues[y + 3]);
			matrixColumn.SetZ(a_other.m_fMatrixValues[y + 6]);
				
			matrix3x3.m_fMatrixValues[i] = ((matrixRow.GetX() * matrixColumn.GetX()) + (matrixRow.GetY() * matrixColumn.GetY()) + (matrixRow.GetZ() * matrixColumn.GetZ()));
			++i;
		}
	}

	return matrix3x3;
}

void Matrix3x3::operator*=(const Matrix3x3& a_other)
{
	Vector3D matrixRow;
	Vector3D matrixColumn;
	int i = 0;

	for (int x = 1; x <= smc_uiRows; ++x)
	{
		matrixRow = GetRow(x);

		for (int y = 0; y < smc_uiColumns; ++y)
		{
			matrixColumn.SetX(a_other.m_fMatrixValues[y]);
			matrixColumn.SetY(a_other.m_fMatrixValues[y + 3]);
			matrixColumn.SetZ(a_other.m_fMatrixValues[y + 6]);

			m_fMatrixValues[i] = ((matrixRow.GetX() * matrixColumn.GetX()) + (matrixRow.GetY() * matrixColumn.GetY()) + (matrixRow.GetZ() * matrixColumn.GetZ()));
			++i;
		}
	}
}

const Matrix3x3 Matrix3x3::operator*(const float& a_other) const
{
	Matrix3x3 matrix3x3;
	Vector3D matrixRow;
	int i = 0;

	for (int x = 1; x <= smc_uiRows; ++x)
	{
		matrixRow = GetRow(x);

		for (int y = 0; y < smc_uiColumns; ++y)
		{
			matrix3x3.m_fMatrixValues[i] = ((matrixRow.GetX() * a_other) + (matrixRow.GetY() * a_other) + (matrixRow.GetZ() * a_other));
			++i;
		}
	}

	return matrix3x3;
}

void Matrix3x3::operator*=(const float& a_other)
{
	Vector3D matrixRow;
	int i = 0;

	for (int x = 1; x <= smc_uiRows; ++x)
	{
		matrixRow = GetRow(x);

		for (int y = 0; y < smc_uiColumns; ++y)
		{
			m_fMatrixValues[i] = ((matrixRow.GetX() * a_other) + (matrixRow.GetY() * a_other) + (matrixRow.GetZ() * a_other));
			++i;
		}
	}
}

const bool Matrix3x3::operator==(const Matrix3x3& a_other) const
{
	if (m_fMatrixValues == a_other.m_fMatrixValues)
	{
		return true;
	}
	else
	{
		return false;
	}
}

const bool Matrix3x3::operator!=(const Matrix3x3& a_other) const
{
	if (m_fMatrixValues != a_other.m_fMatrixValues)
	{
		return true;
	}
	else
	{
		return false;
	}
}

const float Matrix3x3::operator[](const int i) const
{
	return m_fMatrixValues[i];
}
#pragma endregion