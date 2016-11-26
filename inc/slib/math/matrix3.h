#ifndef CHECKHEADER_SLIB_MATH_MATRIX3
#define CHECKHEADER_SLIB_MATH_MATRIX3

#include "definition.h"

#include "matrix2.h"
#include "vector3.h"

#include "../core/interpolation.h"

SLIB_MATH_NAMESPACE_BEGIN

#define SLIB_MATH_MATRIX_DETERMINANT3(m00,m01,m02,m10,m11,m12,m20,m21,m22) ((m00)*SLIB_MATH_MATRIX_DETERMINANT2(m11,m12,m21,m22)-(m01)*SLIB_MATH_MATRIX_DETERMINANT2(m10,m12,m20,m22)+(m02)*SLIB_MATH_MATRIX_DETERMINANT2(m10,m11,m20,m21))

template <class T>
class SLIB_EXPORT Matrix3T
{
public:
	T m00; T m01; T m02;
	T m10; T m11; T m12;
	T m20; T m21; T m22;

public:
	Matrix3T() = default;

	Matrix3T(const Matrix3T<T>& other) = default;
	
	template <class O>
	Matrix3T(const Matrix3T<O>& other);

	Matrix3T(T m00, T m01, T m02,
			 T m10, T m11, T m12,
			 T m20, T m21, T m22);

	Matrix3T(const Vector3T<T>& row0, const Vector3T<T>& row1, const Vector3T<T>& row2);
	
public:
	static const Matrix3T<T>& zero();

	static const Matrix3T<T>& one();

	static const Matrix3T<T>& identity();
	
	static const Matrix3T<T>& fromArray(const T arr[9]);
	
	static Matrix3T<T>& fromArray(T arr[9]);

	
	Vector3T<T> getRow0() const;
	
	void setRow0(const Vector3T<T>& v);

	Vector3T<T> getRow1() const;
	
	void setRow1(const Vector3T<T>& v);
	
	Vector3T<T> getRow2() const;
	
	void setRow2(const Vector3T<T>& v);

	Vector3T<T> getRow(sl_uint32 index) const;
	
	void setRow(sl_uint32 index, const Vector3T<T>& v);
	
	Vector3T<T> getColumn0() const;
	
	void setColumn0(const Vector3T<T>& v);
	
	Vector3T<T> getColumn1() const;
	
	void setColumn1(const Vector3T<T>& v);
	
	Vector3T<T> getColumn2() const;
	
	void setColumn2(const Vector3T<T>& v);

	Vector3T<T> getColumn(sl_uint32 index) const;
	
	void setColumn(sl_uint32 index, const Vector3T<T>& v);

	T getElement(sl_uint32 row, sl_uint32 column) const;
	
	void setElement(sl_uint32 row, sl_uint32 column, const T& v);
	
	
	void add(const Matrix3T<T>& other);
	
	void subtract(const Matrix3T<T>& other);
	
	void multiply(T value);
	
	void divide(T value);
	
	Vector3T<T> multiplyLeft(const Vector3T<T>& v) const;
	
	Vector3T<T> multiplyRight(const Vector3T<T>& v) const;
	
	Vector2T<T> transformPosition(T x, T y) const;
	
	Vector2T<T> transformPosition(const Vector2T<T>& v) const;
	
	Vector2T<T> transformDirection(T x, T y) const;
	
	Vector2T<T> transformDirection(const Vector2T<T>& v) const;
	
	void multiply(const Matrix3T<T>& m);
	
	T getDeterminant() const;
	
	void makeInverse();
	
	Matrix3T<T> inverse() const;
	
	void makeTranspose();
	
	Matrix3T<T> transpose() const;
	
	void makeInverseTranspose();
	
	Matrix3T<T> inverseTranspose() const;

	Matrix3T<T> lerp(const Matrix3T<T>& target, float factor) const;

public:
	Matrix3T<T>& operator=(const Matrix3T<T>& other) = default;
	
	template <class O>
	Matrix3T<T>& operator=(const Matrix3T<O>& other);
	
	Matrix3T<T> operator+(const Matrix3T<T>& other) const;
	
	Matrix3T<T>& operator+=(const Matrix3T<T>& other);
	
	Matrix3T<T> operator-(const Matrix3T<T>& other) const;
	
	Matrix3T<T>& operator-=(const Matrix3T<T>& other);
	
	Matrix3T<T> operator-() const;

	Matrix3T<T> operator*(T value) const;
	
	Matrix3T<T>& operator*=(T value);
	
	Matrix3T<T> operator/(T value) const;
	
	Matrix3T<T>& operator/=(T value);

	Vector3T<T> operator*(const Vector3T<T>& v) const;
	
	Matrix3T<T> operator*(const Matrix3T<T>& other) const;
	
	Matrix3T<T>& operator*=(const Matrix3T<T>& other);
	
	sl_bool operator==(const Matrix3T<T>& other) const;
	
	sl_bool operator!=(const Matrix3T<T>& other) const;

private:
	static T _zero[9];
	static T _one[9];
	static T _identity[9];

};

SLIB_DECLARE_GEOMETRY_TYPE(Matrix3)

template <class T>
Matrix3T<T> operator*(T value, const Matrix3T<T>& m);

template <class T>
Vector3T<T> operator*(const Vector3T<T>& v, const Matrix3T<T>& m);

template <class T>
class Interpolation< Matrix3T<T> >
{
public:
	static Matrix3T<T> interpolate(const Matrix3T<T>& a, const Matrix3T<T>& b, float factor);
};

SLIB_MATH_NAMESPACE_END


SLIB_MATH_NAMESPACE_BEGIN

template <class T>
template <class O>
Matrix3T<T>::Matrix3T(const Matrix3T<O>& other)
: m00((T)(other.m00)), m01((T)(other.m01)), m02((T)(other.m02)),
m10((T)(other.m10)), m11((T)(other.m11)), m12((T)(other.m12)),
m20((T)(other.m20)), m21((T)(other.m21)), m22((T)(other.m22))
{
}

template <class T>
SLIB_INLINE const Matrix3T<T>& Matrix3T<T>::zero()
{
	return *((Matrix3T<T>*)((void*)(_zero)));
}

template <class T>
SLIB_INLINE const Matrix3T<T>& Matrix3T<T>::one()
{
	return *((Matrix3T<T>*)((void*)(_one)));
}

template <class T>
SLIB_INLINE const Matrix3T<T>& Matrix3T<T>::identity()
{
	return *((Matrix3T<T>*)((void*)(_identity)));
}

template <class T>
SLIB_INLINE const Matrix3T<T>& Matrix3T<T>::fromArray(const T* arr)
{
	return *((Matrix3T<T>*)((void*)(arr)));
}

template <class T>
SLIB_INLINE Matrix3T<T>& Matrix3T<T>::fromArray(T* arr)
{
	return *((Matrix3T<T>*)((void*)(arr)));
}

template <class T>
template <class O>
Matrix3T<T>& Matrix3T<T>::operator=(const Matrix3T<O>& other)
{
	m00 = (T)(other.m00); m01 = (T)(other.m01); m02 = (T)(other.m02);
	m10 = (T)(other.m10); m11 = (T)(other.m11); m12 = (T)(other.m12);
	m20 = (T)(other.m20); m21 = (T)(other.m21); m22 = (T)(other.m22);
	return *this;
}


template <class T>
Matrix3T<T> operator*(T value, const Matrix3T<T>& m)
{
	Matrix3T<T> ret(m);
	ret.multiply(value);
	return ret;
}

template <class T>
Vector3T<T> operator*(const Vector3T<T>& v, const Matrix3T<T>& m)
{
	return m.multiplyLeft(v);
}


template <class T>
SLIB_INLINE Matrix3T<T> Interpolation< Matrix3T<T> >::interpolate(const Matrix3T<T>& a, const Matrix3T<T>& b, float factor)
{
	return a.lerp(b, factor);
}

SLIB_MATH_NAMESPACE_END

#endif
