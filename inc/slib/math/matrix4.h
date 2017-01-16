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
	SLIB_INLINE Matrix4T() = default;
	
	constexpr Matrix4T(const Matrix4T<T>& other):
	 m00(other.m00), m01(other.m01), m02(other.m02), m03(other.m03),
	 m10(other.m10), m11(other.m11), m12(other.m12), m13(other.m13),
	 m20(other.m20), m21(other.m21), m22(other.m22), m23(other.m23),
	 m30(other.m30), m31(other.m31), m32(other.m32), m33(other.m33)
	{}

	template <class O>
	constexpr Matrix4T(const Matrix4T<O>& other):
	 m00((T)(other.m00)), m01((T)(other.m01)), m02((T)(other.m02)), m03((T)(other.m03)),
	 m10((T)(other.m10)), m11((T)(other.m11)), m12((T)(other.m12)), m13((T)(other.m13)),
	 m20((T)(other.m20)), m21((T)(other.m21)), m22((T)(other.m22)), m23((T)(other.m23)),
	 m30((T)(other.m30)), m31((T)(other.m31)), m32((T)(other.m32)), m33((T)(other.m33))
	{}

	Matrix4T(T _m00, T _m01, T _m02, T _m03,
			 T _m10, T _m11, T _m12, T _m13,
			 T _m20, T _m21, T _m22, T _m23,
			 T _m30, T _m31, T _m32, T _m33):
	 m00(_m00), m01(_m01), m02(_m02), m03(_m03),
	 m10(_m10), m11(_m11), m12(_m12), m13(_m13),
	 m20(_m20), m21(_m21), m22(_m22), m23(_m23),
	 m30(_m30), m31(_m31), m32(_m32), m33(_m33)
	{}

	Matrix4T(const Vector4T<T>& row0, const Vector4T<T>& row1, const Vector4T<T>& row2, const Vector4T<T>& row3):
	 m00(row0.x), m01(row0.y), m02(row0.z), m03(row0.w),
	 m10(row1.x), m11(row1.y), m12(row1.z), m13(row1.w),
	 m20(row2.x), m21(row2.y), m22(row2.z), m23(row2.w),
	 m30(row3.x), m31(row3.y), m32(row3.z), m33(row3.w)
	{}
	
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
SLIB_INLINE const Matrix4T<T>& Matrix4T<T>::zero()
{
	return *(reinterpret_cast<Matrix4T<T> const*>(&_zero));
}

template <class T>
SLIB_INLINE const Matrix4T<T>& Matrix4T<T>::one()
{
	return *(reinterpret_cast<Matrix4T<T> const*>(&_one));
}

template <class T>
SLIB_INLINE const Matrix4T<T>& Matrix4T<T>::identity()
{
	return *(reinterpret_cast<Matrix4T<T> const*>(&_identity));
}

template <class T>
SLIB_INLINE const Matrix4T<T>& Matrix4T<T>::fromArray(const T arr[16])
{
	return *(reinterpret_cast<Matrix4T<T> const*>(arr));
}

template <class T>
SLIB_INLINE Matrix4T<T>& Matrix4T<T>::fromArray(T arr[16])
{
	return *(reinterpret_cast<Matrix4T<T>*>(arr));
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
