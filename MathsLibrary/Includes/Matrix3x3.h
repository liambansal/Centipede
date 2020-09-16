/////////////////////////////
// Author: Will Masek
// Date Created: 10/02/2020
/////////////////////////////

#ifndef MATRIX_3X3
#define MATRIX_3X3

class Vector3D;

class Matrix3x3
{
public:
	// Constructors.
	Matrix3x3();
	Matrix3x3(const float a_fm11, const float a_fm12, const float a_fm13,
		const float a_fm21, const float a_fm22, const float a_fm23,
		const float a_fm31, const float a_fm32, const float a_fm33);
	Matrix3x3(const float* a_fpm33Values);
	// Destructor.
	~Matrix3x3();

	// Overloaded Operators.
	const Matrix3x3 operator+(const Matrix3x3 &a_other) const;
	void operator+=(const Matrix3x3 &a_other);
	const Matrix3x3 operator-(const Matrix3x3 &a_other) const;
	void operator-=(const Matrix3x3 &a_other);
	const Matrix3x3 operator*(const Matrix3x3 &a_other) const;
	void operator*=(const Matrix3x3 &a_other);
	const Matrix3x3 operator*(const float &a_other) const;
	void operator*=(const float &a_other);
	const bool operator==(const Matrix3x3 &a_other) const;
	const bool operator!=(const Matrix3x3 &a_other) const;
	const float operator[](const int i) const;

	// Funtions.
	static const Matrix3x3 Identity();
	// Flips the matrix's values along its main diagonal.
	void Transpose();
	Matrix3x3 MinorMatrix();
	Matrix3x3 CofactorMatrix(Matrix3x3& a_minorMatrix);
	// Multiplying a matrix by its inverse evaluates to the matrix's identity.
	Matrix3x3 Inverse(Matrix3x3& a_cofactorMatrix);
	// Rotations.
	const Matrix3x3 RotateX(const float a_ftheta) const;
	const Matrix3x3 RotateY(const float a_ftheta) const;
	const Matrix3x3 RotateZ(const float a_ftheta) const;
	void SetRow(const int a_irowNumber, const Vector3D& a_rowValues);
	const Vector3D GetRow(const int a_irowNumber) const;

private:
	// Variables
	static const unsigned int smc_uiColumns = 3;
	static const unsigned int smc_uiRows = 3;
	float m_fMatrixValues[9];
};

#endif //!MATRIX_3X3