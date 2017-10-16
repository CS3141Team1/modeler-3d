#pragma once

#include <iostream>
#include <cmath>

#include "Types.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"

namespace Core
{

namespace Math
{

/**
 * Matrix4 representation using 3 Vector4 as columns.
 *
 * @tparam Type the type for the values in the matrix
 */
template <typename Type>
struct Matrix4
{
	//Columns variable
	Math::Vector4<Type> mMatrix[4];

	//Declarations for constant matrices
	static const Matrix4<Type> Identity;
	static const Matrix4<Type> Zero;

	//Constructors
	Matrix4()
	{
		mMatrix[0] = Math::Vector4<Type>(1,0,0,0);
		mMatrix[1] = Math::Vector4<Type>(0,1,0,0);
		mMatrix[2] = Math::Vector4<Type>(0,0,1,0);
		mMatrix[2] = Math::Vector4<Type>(0,0,0,1);
	}
	Matrix4(const Type& s)
	{
		 mMatrix[0] = Math::Vector4<Type>(s);
		 mMatrix[1] = Math::Vector4<Type>(s);
		 mMatrix[2] = Math::Vector4<Type>(s);
		 mMatrix[3] = Math::Vector4<Type>(s);
	}
	Matrix4(const Type& x, const Type& y, const Type& z, const Type& w)
	{
		 mMatrix[0] = Math::Vector4<Type>(x);
		 mMatrix[1] = Math::Vector4<Type>(y);
		 mMatrix[2] = Math::Vector4<Type>(z);
		 mMatrix[3] = Math::Vector4<Type>(w);
	}
	template <typename Type2, typename Type3, typename Type4, typename Type5>
	Matrix4(const Type2& x, const Type3& y, const Type4& z, const Type5& w)
	{
		mMatrix[0] = Math::Vector4<Type>(x);
		mMatrix[1] = Math::Vector4<Type>(y);
		mMatrix[2] = Math::Vector4<Type>(z);
		mMatrix[3] = Math::Vector4<Type>(w);
	}
	Matrix4(const Math::Vector4<Type>& x, const Math::Vector4<Type>& y, const Math::Vector4<Type>& z, const Math::Vector4<Type>& w)
	{
		mMatrix[0] = x;
		mMatrix[1] = y;
		mMatrix[2] = z;
		mMatrix[3] = w;
	}
	template <typename Type2, typename Type3, typename Type4, typename Type5>
	Matrix4(const Math::Vector4<Type2>& x, const Math::Vector4<Type3>& y, const Math::Vector4<Type4>& z, const Math::Vector4<Type5>& w)
	{
		mMatrix[0] = x;
		mMatrix[1] = y;
		mMatrix[2] = z;
		mMatrix[3] = w;
	}

	//Array access overload
	const Math::Vector4<Type>& operator[](uint i) const { return mMatrix[i]; }
	Math::Vector4<Type>& operator[](uint i) { return mMatrix[i]; }

    //Assignment overload
    Matrix4<Type>& operator=(const Matrix4& v)
    {
    	for(int32 i = 0; i < 4; ++i)
    	{
    		mMatrix[i] = v.mMatrix[i];
    	}
    	return *this;
    }

    //Negation overload
    Matrix4<Type> operator-() const { return Matrix4<Type>(-mMatrix[0], -mMatrix[1], -mMatrix[2], -mMatrix[3]); }

    //Equality overloads
    bool operator==(const Matrix4<Type>& v) { return mMatrix[0] == v.mMatrix[0] && mMatrix[1] == v.mMatrix[1] && mMatrix[2] == v.mMatrix[2] && mMatrix[3] == v.mMatrix[3]; }
    bool operator!=(const Matrix4<Type>& v) { return mMatrix[0] != v.mMatrix[0] || mMatrix[1] != v.mMatrix[1] || mMatrix[2] != v.mMatrix[2] || mMatrix[3] != v.mMatrix[3]; }

    //Math with assignment overloads
    Matrix4<Type>& operator+=(const Matrix4<Type>& v) { mMatrix[0] += v.mMatrix[0]; mMatrix[1] += v.mMatrix[1]; mMatrix[2] += v.mMatrix[2]; mMatrix[3] += v.mMatrix[3]; return *this; }
    Matrix4<Type>& operator-=(const Matrix4<Type>& v) { mMatrix[0] -= v.mMatrix[0]; mMatrix[1] -= v.mMatrix[1]; mMatrix[2] -= v.mMatrix[2]; mMatrix[3] -= v.mMatrix[3]; return *this; }
    Matrix4<Type>& operator*=(const Matrix4<Type>& m)
    {
    	Matrix4<Type> copy = *this;
    	for(int32 i = 0; i < 4; ++i)
    	{
    		for(int32 j = 0; j < 4; ++j)
    		{
    			copy.mMatrix[i][j] = mMatrix[0][j] * m.mMatrix[i][0] + mMatrix[1][j] * m.mMatrix[i][1] + mMatrix[2][j] * m.mMatrix[i][2] + mMatrix[3][j] * m.mMatrix[i][3];
    		}
    	}
    	*this = copy;
    	return *this;
    }
    Matrix4<Type>& operator*=(const Type& s)
    {
    	for(int32 i = 0; i < 4; ++i)
    	{
    		for(int32 j = 0; j < 4; ++j)
    		{
    			mMatrix[i][j] *= s;
    		}
    	}
    	return *this;
    }
    Matrix4<Type>& operator/=(Matrix4<Type> m)
    {
    	Matrix4<Type> inv = Inverse(m);
    	(*this) *= inv;
    	return *this;
    }
    Matrix4<Type>& operator/=(const Type& s)
    {
    	for(int32 i = 0; i < 4; ++i)
    	{
    		for(int32 j = 0; j < 4; ++j)
    		{
    			mMatrix[i][j] /= s;
    		}
    	}
    	return *this;
    }
};

//Macro to define all standard math for the matrix
CORE_MATH_GEN_MATRIX_OPERATORS(Matrix4);

//Multiplying with vector
template <typename Type, typename Type2>
Vector4<Type> operator*(const Matrix4<Type>& m, const Vector4<Type2>& v)
{
	Math::Vector4<Type> ret(0);
	for(int32 i = 0; i < 4; ++i)
	{
		Type sum = 0;
		for(int32 j = 0; j < 4; ++j)
		{
			sum += m[j][i] * v[j];
		}
		ret[i] = sum;
	}
	return ret;
}

//Inverse
template <typename Type>
Matrix4<Type> Inverse(Matrix4<Type> m)
{
	Type determinant = Determinant(m);
	if(determinant == 0)
	{
		Matrix4<Type> ret(0); //return zero matrix
		return ret;
	}
	Matrix4<Type> inverse(0); // inverse of matrix m

	for(int32 i = 0; i < 4; ++i)
	{
		for(int32 j = 0; j < 4; ++j)
		{
			Math::Matrix3<Type> m3(0);
			int32 xx = 0;
			for(int32 x = 0; x < 4; ++x)
			{
				int32 yy = 0;
				for(int32 y = 0; y < 4; ++y)
				{
					if(y != j) {
						m3[xx][yy] = m[x][y];
						yy += 1;
					}
				}
				if(x != i)
					xx += 1;
			}
			inverse[i][j] = std::pow(-1,i+j) * Math::Determinant(m3);
		}
	}

	inverse = Transpose(inverse);
	inverse /= determinant;

	return inverse;
}

//Transpose
template <typename Type>
Matrix4<Type> Transpose(Matrix4<Type>& m)
{
	Matrix4<Type> transpose(0);
	for(int32 i = 0; i < 4; ++i)
	{
		for(int32 j = 0; j < 4; ++j)
		{
			transpose.mMatrix[i][j] = m.mMatrix[j][i];
		}
	}
	return transpose;
}
//Determinant
template <typename Type>
Type Determinant(Matrix4<Type>& m)
{
	Vector3<Type> v1(m[0][1],m[0][2],m[0][3]);
	Vector3<Type> v2(m[1][1],m[1][2],m[1][3]);
	Vector3<Type> v3(m[2][1],m[2][2],m[2][3]);
	Vector3<Type> v4(m[3][1],m[3][2],m[3][3]);

	Matrix3<Type> m1(v2,v3,v4);
	Matrix3<Type> m2(v1,v3,v4);
	Matrix3<Type> m3(v1,v2,v4);
	Matrix3<Type> m4(v1,v2,v3);
	Type det = m[0][0] * Math::Determinant(m1) - m[1][0] * Math::Determinant(m2) +
			m[2][0] * Math::Determinant(m3) - m[3][0] * Math::Determinant(m4);
	return det;
}

//To string
template <typename Type>
std::ostream& operator<<(std::ostream& out, const Matrix4<Type>& m)
{
    return out << "[" << m.mMatrix[0][0] << ", " << m.mMatrix[1][0] << ", " << m.mMatrix[2][0] << ", " << m.mMatrix[3][0] << "]" << std::endl
    		<< "[" << m.mMatrix[0][1] << ", " << m.mMatrix[1][1] << ", " << m.mMatrix[2][1] << ", " << m.mMatrix[3][1] << "]" << std::endl
			<< "[" << m.mMatrix[0][2] << ", " << m.mMatrix[1][2] << ", " << m.mMatrix[2][2] << ", " << m.mMatrix[3][2] << "]" << std::endl
			<< "[" << m.mMatrix[0][3] << ", " << m.mMatrix[1][3] << ", " << m.mMatrix[2][3] << ", " << m.mMatrix[3][3] << "]";
}

template <typename Type>
std::string toString(Matrix4<Type> m) {
	std::string ret = "";
	ret += "[" + m.mMatrix[0][0] + ", " + m.mMatrix[1][0] + ", " + m.mMatrix[2][0] + ", " + m.mMatrix[3][0] + "]" + std::endl
    		+ "[" + m.mMatrix[0][1] + ", " + m.mMatrix[1][1] + ", " + m.mMatrix[2][1] + ", " + m.mMatrix[3][1] + "]" + std::endl
			+ "[" + m.mMatrix[0][2] + ", " + m.mMatrix[1][2] + ", " + m.mMatrix[2][2] + ", " + m.mMatrix[3][2] + "]" + std::endl
			+ "[" + m.mMatrix[0][3] + ", " + m.mMatrix[1][3] + ", " + m.mMatrix[2][3] + ", " + m.mMatrix[3][3] + "]";
	return ret;
}

//Identity and zero matrices
template <typename Type> const Matrix4<Type> Matrix4<Type>::Identity(Math::Vector4<Type>(1,0,0,0),Math::Vector4<Type>(0,1,0,0),Math::Vector4<Type>(0,0,1,0),Math::Vector4<Type>(0,0,0,1));
template <typename Type> const Matrix4<Type> Matrix4<Type>::Zero(0);

//Type definitions for prettier code and less typing
typedef Matrix4<float32> Matrix4f;
typedef Matrix4<float64> Matrix4d;
typedef Matrix4<int32> Matrix4i;
typedef Matrix4<uint32> Matrix4ui;
typedef Matrix4<int64> Matrix4l;
typedef Matrix4<uint64> Matrix4ul;

}

}
