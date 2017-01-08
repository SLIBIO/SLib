#include "../../../inc/slib/math/matrix4.h"

SLIB_MATH_NAMESPACE_BEGIN

template <class T>
SLIB_ALIGN(8) T Matrix4T<T>::_zero[16] = {
	0, 0, 0, 0
	, 0, 0, 0, 0
	, 0, 0, 0, 0
	, 0, 0, 0, 0 };

template <class T>
SLIB_ALIGN(8) T Matrix4T<T>::_one[16] = {
	1, 1, 1, 1
	, 1, 1, 1, 1
	, 1, 1, 1, 1
	, 1, 1, 1, 1 };

template <class T>
SLIB_ALIGN(8) T Matrix4T<T>::_identity[16] = {
	1, 0, 0, 0
	, 0, 1, 0, 0
	, 0, 0, 1, 0
	, 0, 0, 0, 1 };

template <class T>
Vector4T<T> Matrix4T<T>::getRow0() const
{
	return {m00, m01, m02, m03};
}

template <class T>
void Matrix4T<T>::setRow0(const Vector4T<T>& v)
{
	m00 = v.x;
	m01 = v.y;
	m02 = v.z;
	m03 = v.w;
}

template <class T>
Vector4T<T> Matrix4T<T>::getRow1() const
{
	return {m10, m11, m12, m13};
}

template <class T>
void Matrix4T<T>::setRow1(const Vector4T<T>& v)
{
	m10 = v.x;
	m11 = v.y;
	m12 = v.z;
	m13 = v.w;
}

template <class T>
Vector4T<T> Matrix4T<T>::getRow2() const
{
	return {m20, m21, m22, m23};
}

template <class T>
void Matrix4T<T>::setRow2(const Vector4T<T>& v)
{
	m20 = v.x;
	m21 = v.y;
	m22 = v.z;
	m23 = v.w;
}

template <class T>
Vector4T<T> Matrix4T<T>::getRow3() const
{
	return {m30, m31, m32, m33};
}

template <class T>
void Matrix4T<T>::setRow3(const Vector4T<T>& v)
{
	m30 = v.x;
	m31 = v.y;
	m32 = v.z;
	m33 = v.w;
}

template <class T>
Vector4T<T> Matrix4T<T>::getRow(sl_uint32 index) const
{
	const T* t = &m00 + (index << 2);
	return {t[0], t[1], t[2], t[3]};
}

template <class T>
void Matrix4T<T>::setRow(sl_uint32 index, const Vector4T<T>& v)
{
	T* t = &m00 + (index << 2);
	t[0] = v.x;
	t[1] = v.y;
	t[2] = v.z;
	t[3] = v.w;
}

template <class T>
Vector4T<T> Matrix4T<T>::getColumn0() const
{
	return {m00, m10, m20, m30};
}

template <class T>
void Matrix4T<T>::setColumn0(const Vector4T<T>& v)
{
	m00 = v.x;
	m10 = v.y;
	m20 = v.z;
	m30 = v.w;
}

template <class T>
Vector4T<T> Matrix4T<T>::getColumn1() const
{
	return {m01, m11, m21, m31};
}

template <class T>
void Matrix4T<T>::setColumn1(const Vector4T<T>& v)
{
	m01 = v.x;
	m11 = v.y;
	m21 = v.z;
	m31 = v.w;
}

template <class T>
Vector4T<T> Matrix4T<T>::getColumn2() const
{
	return {m02, m12, m22, m32};
}

template <class T>
void Matrix4T<T>::setColumn2(const Vector4T<T>& v)
{
	m02 = v.x;
	m12 = v.y;
	m22 = v.z;
	m32 = v.w;
}

template <class T>
Vector4T<T> Matrix4T<T>::getColumn3() const
{
	return {m03, m13, m23, m33};
}

template <class T>
void Matrix4T<T>::setColumn3(const Vector4T<T>& v)
{
	m03 = v.x;
	m13 = v.y;
	m23 = v.z;
	m33 = v.w;
}

template <class T>
Vector4T<T> Matrix4T<T>::getColumn(sl_uint32 index) const
{
	const T* t = &m00 + index;
	return {t[0], t[4], t[8], t[12]};
}

template <class T>
void Matrix4T<T>::setColumn(sl_uint32 index, const Vector4T<T>& v)
{
	T* t = &m00 + index;
	t[0] = v.x;
	t[4] = v.y;
	t[8] = v.z;
	t[12] = v.w;
}

template <class T>
T Matrix4T<T>::getElement(sl_uint32 row, sl_uint32 column) const
{
	return (&m00)[(row << 2) + column];
}

template <class T>
void Matrix4T<T>::setElement(sl_uint32 row, sl_uint32 column, const T& v)
{
	(&m00)[(row << 2) + column] = v;
}

template <class T>
void Matrix4T<T>::add(const Matrix4T<T>& other)
{
	m00 += other.m00; m01 += other.m01; m02 += other.m02; m03 += other.m03;
	m10 += other.m10; m11 += other.m11; m12 += other.m12; m13 += other.m13;
	m20 += other.m20; m21 += other.m21; m22 += other.m22; m23 += other.m23;
	m30 += other.m30; m31 += other.m31; m32 += other.m32; m33 += other.m33;
}

template <class T>
void Matrix4T<T>::subtract(const Matrix4T<T>& other)
{
	m00 -= other.m00; m01 -= other.m01; m02 -= other.m02; m03 -= other.m03;
	m10 -= other.m10; m11 -= other.m11; m12 -= other.m12; m13 -= other.m13;
	m20 -= other.m20; m21 -= other.m21; m22 -= other.m22; m23 -= other.m23;
	m30 -= other.m30; m31 -= other.m31; m32 -= other.m32; m33 -= other.m33;
}

template <class T>
void Matrix4T<T>::multiply(T value)
{
	m00 *= value; m01 *= value; m02 *= value; m03 *= value;
	m10 *= value; m11 *= value; m12 *= value; m13 *= value;
	m20 *= value; m21 *= value; m22 *= value; m23 *= value;
	m30 *= value; m31 *= value; m32 *= value; m33 *= value;
}

template <class T>
void Matrix4T<T>::divide(T value)
{
	m00 /= value; m01 /= value; m02 /= value; m03 /= value;
	m10 /= value; m11 /= value; m12 /= value; m13 /= value;
	m20 /= value; m21 /= value; m22 /= value; m23 /= value;
	m30 /= value; m31 /= value; m32 /= value; m33 /= value;
}

template <class T>
Vector4T<T> Matrix4T<T>::multiplyLeft(const Vector4T<T>& v) const
{
	T _x = v.x * m00 + v.y * m10 + v.z * m20 + v.w * m30;
	T _y = v.x * m01 + v.y * m11 + v.z * m21 + v.w * m31;
	T _z = v.x * m02 + v.y * m12 + v.z * m22 + v.w * m32;
	T _w = v.x * m03 + v.y * m13 + v.z * m23 + v.w * m33;
	return {_x, _y, _z, _w};
}

template <class T>
Vector4T<T> Matrix4T<T>::multiplyRight(const Vector4T<T>& v) const
{
	T _x = m00 * v.x + m01 * v.y + m02 * v.z + m03 * v.w;
	T _y = m10 * v.x + m11 * v.y + m12 * v.z + m13 * v.w;
	T _z = m20 * v.x + m21 * v.y + m22 * v.z + m23 * v.w;
	T _w = m30 * v.x + m31 * v.y + m32 * v.z + m33 * v.w;
	return {_x, _y, _z, _w};
}

template <class T>
Vector3T<T> Matrix4T<T>::transformPosition(T x, T y, T z) const
{
	T _x = x * m00 + y * m10 + z * m20 + m30;
	T _y = x * m01 + y * m11 + z * m21 + m31;
	T _z = x * m02 + y * m12 + z * m22 + m32;
	return {_x, _y, _z};
}

template <class T>
Vector3T<T> Matrix4T<T>::transformPosition(const Vector3T<T>& v) const
{
	T _x = v.x * m00 + v.y * m10 + v.z * m20 + m30;
	T _y = v.x * m01 + v.y * m11 + v.z * m21 + m31;
	T _z = v.x * m02 + v.y * m12 + v.z * m22 + m32;
	return {_x, _y, _z};
}

template <class T>
Vector3T<T> Matrix4T<T>::transformDirection(T x, T y, T z) const
{
	T _x = x * m00 + y * m10 + z * m20;
	T _y = x * m01 + y * m11 + z * m21;
	T _z = x * m02 + y * m12 + z * m22;
	return {_x, _y, _z};
}

template <class T>
Vector3T<T> Matrix4T<T>::transformDirection(const Vector3T<T>& v) const
{
	T _x = v.x * m00 + v.y * m10 + v.z * m20;
	T _y = v.x * m01 + v.y * m11 + v.z * m21;
	T _z = v.x * m02 + v.y * m12 + v.z * m22;
	return {_x, _y, _z};
}

template <class T>
void Matrix4T<T>::multiply(const Matrix4T<T>& m)
{
	T v0, v1, v2, v3;
	v0 = m00 * m.m00 + m01 * m.m10 + m02 * m.m20 + m03 * m.m30;
	v1 = m00 * m.m01 + m01 * m.m11 + m02 * m.m21 + m03 * m.m31;
	v2 = m00 * m.m02 + m01 * m.m12 + m02 * m.m22 + m03 * m.m32;
	v3 = m00 * m.m03 + m01 * m.m13 + m02 * m.m23 + m03 * m.m33;
	m00 = v0; m01 = v1; m02 = v2; m03 = v3;
	v0 = m10 * m.m00 + m11 * m.m10 + m12 * m.m20 + m13 * m.m30;
	v1 = m10 * m.m01 + m11 * m.m11 + m12 * m.m21 + m13 * m.m31;
	v2 = m10 * m.m02 + m11 * m.m12 + m12 * m.m22 + m13 * m.m32;
	v3 = m10 * m.m03 + m11 * m.m13 + m12 * m.m23 + m13 * m.m33;
	m10 = v0; m11 = v1; m12 = v2; m13 = v3;
	v0 = m20 * m.m00 + m21 * m.m10 + m22 * m.m20 + m23 * m.m30;
	v1 = m20 * m.m01 + m21 * m.m11 + m22 * m.m21 + m23 * m.m31;
	v2 = m20 * m.m02 + m21 * m.m12 + m22 * m.m22 + m23 * m.m32;
	v3 = m20 * m.m03 + m21 * m.m13 + m22 * m.m23 + m23 * m.m33;
	m20 = v0; m21 = v1; m22 = v2; m23 = v3;
	v0 = m30 * m.m00 + m31 * m.m10 + m32 * m.m20 + m33 * m.m30;
	v1 = m30 * m.m01 + m31 * m.m11 + m32 * m.m21 + m33 * m.m31;
	v2 = m30 * m.m02 + m31 * m.m12 + m32 * m.m22 + m33 * m.m32;
	v3 = m30 * m.m03 + m31 * m.m13 + m32 * m.m23 + m33 * m.m33;
	m30 = v0; m31 = v1; m32 = v2; m33 = v3;
}

template <class T>
T Matrix4T<T>::getDeterminant() const
{
	return SLIB_MATH_MATRIX_DETERMINANT4(m00, m01, m02, m03,
										 m10, m11, m12, m13,
										 m20, m21, m22, m23,
										 m30, m31, m32, m33);
}

template <class T>
void Matrix4T<T>::makeInverse()
{
	T A00 = SLIB_MATH_MATRIX_DETERMINANT3(m11, m12, m13, m21, m22, m23, m31, m32, m33);
	T A01 = -SLIB_MATH_MATRIX_DETERMINANT3(m10, m12, m13, m20, m22, m23, m30, m32, m33);
	T A02 = SLIB_MATH_MATRIX_DETERMINANT3(m10, m11, m13, m20, m21, m23, m30, m31, m33);
	T A03 = -SLIB_MATH_MATRIX_DETERMINANT3(m10, m11, m12, m20, m21, m22, m30, m31, m32);
	T A10 = -SLIB_MATH_MATRIX_DETERMINANT3(m01, m02, m03, m21, m22, m23, m31, m32, m33);
	T A11 = SLIB_MATH_MATRIX_DETERMINANT3(m00, m02, m03, m20, m22, m23, m30, m32, m33);
	T A12 = -SLIB_MATH_MATRIX_DETERMINANT3(m00, m01, m03, m20, m21, m23, m30, m31, m33);
	T A13 = SLIB_MATH_MATRIX_DETERMINANT3(m00, m01, m02, m20, m21, m22, m30, m31, m32);
	T A20 = SLIB_MATH_MATRIX_DETERMINANT3(m01, m02, m03, m11, m12, m13, m31, m32, m33);
	T A21 = -SLIB_MATH_MATRIX_DETERMINANT3(m00, m02, m03, m10, m12, m13, m30, m32, m33);
	T A22 = SLIB_MATH_MATRIX_DETERMINANT3(m00, m01, m03, m10, m11, m13, m30, m31, m33);
	T A23 = -SLIB_MATH_MATRIX_DETERMINANT3(m00, m01, m02, m10, m11, m12, m30, m31, m32);
	T A30 = -SLIB_MATH_MATRIX_DETERMINANT3(m01, m02, m03, m11, m12, m13, m21, m22, m23);
	T A31 = SLIB_MATH_MATRIX_DETERMINANT3(m00, m02, m03, m10, m12, m13, m20, m22, m23);
	T A32 = -SLIB_MATH_MATRIX_DETERMINANT3(m00, m01, m03, m10, m11, m13, m20, m21, m23);
	T A33 = SLIB_MATH_MATRIX_DETERMINANT3(m00, m01, m02, m10, m11, m12, m20, m21, m22);
	T D = 1 / (m00*A00 + m01*A01 + m02*A02 + m03*A03);
	
	m00 = A00*D; m10 = A01*D; m20 = A02*D; m30 = A03*D;
	m01 = A10*D; m11 = A11*D; m21 = A12*D; m31 = A13*D;
	m02 = A20*D; m12 = A21*D; m22 = A22*D; m32 = A23*D;
	m03 = A30*D; m13 = A31*D; m23 = A32*D; m33 = A33*D;
}

template <class T>
Matrix4T<T> Matrix4T<T>::inverse() const
{
	Matrix4T<T> ret(*this);
	ret.makeInverse();
	return ret;
}

template <class T>
void Matrix4T<T>::makeTranspose()
{
	T	f;
	f = m01; m01 = m10; m10 = f;
	f = m02; m02 = m20; m20 = f;
	f = m03; m03 = m30; m30 = f;
	f = m12; m12 = m21; m21 = f;
	f = m13; m13 = m31; m31 = f;
	f = m23; m23 = m32; m32 = f;
}

template <class T>
Matrix4T<T> Matrix4T<T>::transpose() const
{
	Matrix4T<T> ret(*this);
	ret.makeTranspose();
	return ret;
}

template <class T>
void Matrix4T<T>::makeInverseTranspose()
{
	makeInverse();
	makeTranspose();
}

template <class T>
Matrix4T<T> Matrix4T<T>::inverseTranspose() const
{
	Matrix4T<T> ret(*this);
	ret.makeInverse();
	ret.makeTranspose();
	return ret;
}

template <class T>
Matrix4T<T> Matrix4T<T>::lerp(const Matrix4T<T>& target, float factor) const
{
	return {
		SLIB_LERP(m00, target.m00, factor), SLIB_LERP(m01, target.m01, factor), SLIB_LERP(m02, target.m02, factor), SLIB_LERP(m03, target.m03, factor),
		SLIB_LERP(m10, target.m10, factor), SLIB_LERP(m11, target.m11, factor), SLIB_LERP(m12, target.m12, factor), SLIB_LERP(m13, target.m13, factor),
		SLIB_LERP(m20, target.m20, factor), SLIB_LERP(m21, target.m21, factor), SLIB_LERP(m22, target.m22, factor), SLIB_LERP(m23, target.m23, factor),
		SLIB_LERP(m30, target.m30, factor), SLIB_LERP(m31, target.m31, factor), SLIB_LERP(m32, target.m32, factor), SLIB_LERP(m33, target.m33, factor)};
}

template <class T>
Matrix4T<T> Matrix4T<T>::operator+(const Matrix4T<T>& other) const
{
	Matrix4T<T> ret(*this);
	ret.add(other);
	return ret;
}

template <class T>
Matrix4T<T>& Matrix4T<T>::operator+=(const Matrix4T<T>& other)
{
	add(other);
	return *this;
}

template <class T>
Matrix4T<T> Matrix4T<T>::operator-(const Matrix4T<T>& other) const
{
	Matrix4T<T> ret(*this);
	ret.subtract(other);
	return ret;
}

template <class T>
Matrix4T<T>& Matrix4T<T>::operator-=(const Matrix4T<T>& other)
{
	subtract(other);
	return *this;
}

template <class T>
Matrix4T<T> Matrix4T<T>::operator-() const
{
	Matrix4T<T> ret(Matrix4T<T>::zero());
	ret.subtract(*this);
	return ret;
}

template <class T>
Matrix4T<T> Matrix4T<T>::operator*(T value) const
{
	Matrix4T<T> ret(*this);
	ret.multiply(value);
	return ret;
}

template <class T>
Matrix4T<T>& Matrix4T<T>::operator*=(T value)
{
	multiply(value);
	return *this;
}

template <class T>
Matrix4T<T> Matrix4T<T>::operator/(T value) const
{
	Matrix4T<T> ret(*this);
	ret.divide(value);
	return ret;
}

template <class T>
Matrix4T<T>& Matrix4T<T>::operator/=(T value)
{
	divide(value);
	return *this;
}

template <class T>
Vector4T<T> Matrix4T<T>::operator*(const Vector4T<T>& v) const
{
	return multiplyRight(v);
}

template <class T>
Matrix4T<T> Matrix4T<T>::operator*(const Matrix4T<T>& other) const
{
	Matrix4T<T> ret(*this);
	ret.multiply(other);
	return ret;
}

template <class T>
Matrix4T<T>& Matrix4T<T>::operator*=(const Matrix4T<T>& other)
{
	multiply(other);
	return *this;
}

template <class T>
sl_bool Matrix4T<T>::operator==(const Matrix4T<T>& other) const
{
	return m00 == other.m00 && m01 == other.m01 && m02 == other.m02 && m03 == other.m03 &&
		m10 == other.m10 && m11 == other.m11 && m12 == other.m12 && m13 == other.m13 &&
		m20 == other.m20 && m21 == other.m21 && m22 == other.m22 && m23 == other.m23 &&
		m30 == other.m30 && m31 == other.m31 && m32 == other.m32 && m33 == other.m33;
}

template <class T>
sl_bool Matrix4T<T>::operator!=(const Matrix4T<T>& other) const
{
	return m00 != other.m00 || m01 != other.m01 || m02 != other.m02 || m03 != other.m03 ||
		m10 != other.m10 || m11 != other.m11 || m12 != other.m12 || m13 != other.m13 ||
		m20 != other.m20 || m21 != other.m21 || m22 != other.m22 || m23 != other.m23 ||
		m30 != other.m30 || m31 != other.m31 || m32 != other.m32 || m33 != other.m33;
}


SLIB_DEFINE_GEOMETRY_TYPE(Matrix4)

SLIB_MATH_NAMESPACE_END
