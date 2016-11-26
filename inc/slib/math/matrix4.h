#ifndef CHECKHEADER_SLIB_MATH_MATRIX4
#define CHECKHEADER_SLIB_MATH_MATRIX4

#include "definition.h"

#include "matrix3.h"
#include "vector4.h"

#include "../core/interpolation.h"

SLIB_MATH_NAMESPACE_BEGIN

#define SLIB_MATH_MATRIX_DETERMINANT4(m00,m01,m02,m03,m10,m11,m12,m13,m20,m21,m22,m23,m30,m31,m32,m33) ((m00)*SLIB_MATH_MATRIX_DETERMINANT3(m11,m12,m13,m21,m22,m23,m31,m32,m33)-(m01)*SLIB_MATH_MATRIX_DETERMINANT3(m10,m12,m13,m20,m22,m23,m30,m32,m33)+(m02)*SLIB_MATH_MATRIX_DETERMINANT3(m10,m11,m13,m20,m21,m23,m30,m31,m33)-(m03)*SLIB_MATH_MATRIX_DETERMINANT3(m10,m11,m12,m20,m21,m22,m30,m31,m32))

template <class T>
class SLIB_EXPORT Matrix4T
{
public:
	T m00; T m01; T m02; T m03;
	T m10; T m11; T m12; T m13;
	T m20; T m21; T m22; T m23;
	T m30; T m31; T m32; T m33;

public:
	Matrix4T() = default;
	
	Matrix4T(const Matrix4T<T>& other) = default;

	template <class O>
	Matrix4T(const Matrix4T<O>& other);

	Matrix4T(T m00, T m01, T m02, T m03,
			 T m10, T m11, T m12, T m13,
			 T m20, T m21, T m22, T m23,
			 T m30, T m31, T m32, T m33);

	Matrix4T(const Vector4T<T>& row0, const Vector4T<T>& row1, const Vector4T<T>& row2, const Vector4T<T>& row3);
	
public:
	static const Matrix4T<T>& zero();

	static const Matrix4T<T>& one();

	static const Matrix4T<T>& identity();
	
	static const Matrix4T<T>& fromArray(const T arr[16]);
	
	static Matrix4T<T>& fromArray(T arr[16]);

	
	Vector4T<T> getRow0() const;
	
	void setRow0(const Vector4T<T>& v);
	
	Vector4T<T> getRow1() const;
	
	void setRow1(const Vector4T<T>& v);
	
	Vector4T<T> getRow2() const;
	
	void setRow2(const Vector4T<T>& v);
	
	Vector4T<T> getRow3() const;
	
	void setRow3(const Vector4T<T>& v);

	Vector4T<T> getRow(sl_uint32 index) const;
	
	void setRow(sl_uint32 index, const Vector4T<T>& v);

	Vector4T<T> getColumn0() const;
	
	void setColumn0(const Vector4T<T>& v);
	
	Vector4T<T> getColumn1() const;
	
	void setColumn1(const Vector4T<T>& v);
	
	Vector4T<T> getColumn2() const;
	
	void setColumn2(const Vector4T<T>& v);
	
	Vector4T<T> getColumn3() const;
	
	void setColumn3(const Vector4T<T>& v);

	Vector4T<T> getColumn(sl_uint32 index) const;
	
	void setColumn(sl_uint32 index, const Vector4T<T>& v);

	T getElement(sl_uint32 row, sl_uint32 column) const;
	
	void setElement(sl_uint32 row, sl_uint32 column, const T& v);

	
	void add(const Matrix4T<T>& other);
	
	void subtract(const Matrix4T<T>& other);
	
	void multiply(T value);
	
	void divide(T value);
	
	Vector4T<T> multiplyLeft(const Vector4T<T>& v) const;
	
	Vector4T<T> multiplyRight(const Vector4T<T>& v) const;
	
	Vector3T<T> transformPosition(T x, T y, T z) const;
	
	Vector3T<T> transformPosition(const Vector3T<T>& v) const;
	
	Vector3T<T> transformDirection(T x, T y, T z) const;
	
	Vector3T<T> transformDirection(const Vector3T<T>& v) const;
	
	void multiply(const Matrix4T<T>& m);
	
	T getDeterminant() const;
	
	void makeInverse();
	
	Matrix4T<T> inverse() const;
	
	void makeTranspose();
	
	Matrix4T<T> transpose() const;
	
	void makeInverseTranspose();
	
	Matrix4T<T> inverseTranspose() const;
	
	Matrix4T<T> lerp(const Matrix4T<T>& target, float factor) const;

public:
	Matrix4T<T>& operator=(const Matrix4T<T>& other) = default;
	
	template <class O>
	Matrix4T<T>& operator=(const Matrix4T<O>& other);

	Matrix4T<T> operator+(const Matrix4T<T>& other) const;

	Matrix4T<T>& operator+=(const Matrix4T<T>& other);

	Matrix4T<T> operator-(const Matrix4T<T>& other) const;

	Matrix4T<T>& operator-=(const Matrix4T<T>& other);
	
	Matrix4T<T> operator-() const;

	Matrix4T<T> operator*(T value) const;

	Matrix4T<T>& operator*=(T value);
	
	Matrix4T<T> operator/(T value) const;

	Matrix4T<T>& operator/=(T value);

	Vector4T<T> operator*(const Vector4T<T>& v) const;

	Matrix4T<T> operator*(const Matrix4T<T>& other) const;

	Matrix4T<T>& operator*=(const Matrix4T<T>& other);
	
	sl_bool operator==(const Matrix4T<T>& other) const;
	
	sl_bool operator!=(const Matrix4T<T>& other) const;
	
private:
	static T _zero[16];
	static T _one[16];
	static T _identity[16];
	
};

template <class T>
Matrix4T<T> operator*(T value, const Matrix4T<T>& m);

template <class T>
Vector4T<T> operator*(const Vector4T<T>& v, const Matrix4T<T>& m);

template <class T>
class Interpolation< Matrix4T<T> >
{
public:
	static Matrix4T<T> interpolate(const Matrix4T<T>& a, const Matrix4T<T>& b, float factor);
};

SLIB_DECLARE_GEOMETRY_TYPE(Matrix4)

SLIB_MATH_NAMESPACE_END


SLIB_MATH_NAMESPACE_BEGIN

template <class T>
template <class O>
Matrix4T<T>::Matrix4T(const Matrix4T<O>& other)
: m00((T)(other.m00)), m01((T)(other.m01)), m02((T)(other.m02)), m03((T)(other.m03)),
m10((T)(other.m10)), m11((T)(other.m11)), m12((T)(other.m12)), m13((T)(other.m13)),
m20((T)(other.m20)), m21((T)(other.m21)), m22((T)(other.m22)), m23((T)(other.m23)),
m30((T)(other.m30)), m31((T)(other.m31)), m32((T)(other.m32)), m33((T)(other.m33))
{
}

template <class T>
SLIB_INLINE const Matrix4T<T>& Matrix4T<T>::zero()
{
	return *((Matrix4T<T>*)((void*)(_zero)));
}

template <class T>
SLIB_INLINE const Matrix4T<T>& Matrix4T<T>::one()
{
	return *((Matrix4T<T>*)((void*)(_one)));
}

template <class T>
SLIB_INLINE const Matrix4T<T>& Matrix4T<T>::identity()
{
	return *((Matrix4T<T>*)((void*)(_identity)));
}

template <class T>
SLIB_INLINE const Matrix4T<T>& Matrix4T<T>::fromArray(const T *arr)
{
	return *((Matrix4T<T>*)((void*)(arr)));
}

template <class T>
SLIB_INLINE Matrix4T<T>& Matrix4T<T>::fromArray(T *arr)
{
	return *((Matrix4T<T>*)((void*)(arr)));
}

template <class T>
template <class O>
Matrix4T<T>& Matrix4T<T>::operator=(const Matrix4T<O>& other)
{
	m00 = (T)(other.m00); m01 = (T)(other.m01); m02 = (T)(other.m02); m03 = (T)(other.m03);
	m10 = (T)(other.m10); m11 = (T)(other.m11); m12 = (T)(other.m12); m13 = (T)(other.m13);
	m20 = (T)(other.m20); m21 = (T)(other.m21); m22 = (T)(other.m22); m23 = (T)(other.m23);
	m30 = (T)(other.m30); m31 = (T)(other.m31); m32 = (T)(other.m32); m33 = (T)(other.m33);
	return *this;
}

template <class T>
Matrix4T<T> operator*(T value, const Matrix4T<T>& m)
{
	Matrix4T<T> ret(m);
	ret.multiply(value);
	return ret;
}

template <class T>
Vector4T<T> operator*(const Vector4T<T>& v, const Matrix4T<T>& m)
{
	return m.multiplyLeft(v);
}


template <class T>
SLIB_INLINE Matrix4T<T> Interpolation< Matrix4T<T> >::interpolate(const Matrix4T<T>& a, const Matrix4T<T>& b, float factor)
{
	return a.lerp(b, factor);
}

SLIB_MATH_NAMESPACE_END

#endif
