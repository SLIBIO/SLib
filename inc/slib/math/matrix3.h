#ifndef CHECKHEADER_SLIB_MATH_MATRIX3
#define CHECKHEADER_SLIB_MATH_MATRIX3

#include "definition.h"

#include "matrix2.h"
#include "vector3.h"

#include "../core/math.h"

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
	SLIB_INLINE Matrix3T()
	{
	}

	template <class O>
	SLIB_INLINE Matrix3T(const Matrix3T<O>& other)
	{
		m00 = (T)(other.m00); m01 = (T)(other.m01); m02 = (T)(other.m02);
		m10 = (T)(other.m10); m11 = (T)(other.m11); m12 = (T)(other.m12);
		m20 = (T)(other.m20); m21 = (T)(other.m21); m22 = (T)(other.m22);
	}

	SLIB_INLINE Matrix3T(T m00, T m01, T m02
		, T m10, T m11, T m12
		, T m20, T m21, T m22)
	{
		this->m00 = m00; this->m01 = m01; this->m02 = m02;
		this->m10 = m10; this->m11 = m11; this->m12 = m12;
		this->m20 = m20; this->m21 = m21; this->m22 = m22;
	}

	SLIB_INLINE Matrix3T(const Vector3T<T>& row0, const Vector3T<T>& row1, const Vector3T<T>& row2)
	{
		this->m00 = row0.x; this->m01 = row0.y; this->m02 = row0.z;
		this->m10 = row1.x; this->m11 = row1.y; this->m12 = row1.z;
		this->m20 = row2.x; this->m21 = row2.y; this->m22 = row2.z;
	}

private:
	static T _zero[9];
	static T _one[9];
	static T _identity[9];
	
public:
	SLIB_INLINE static const Matrix3T<T>& zero()
	{
		return *((Matrix3T<T>*)((void*)(_zero)));
	}

	SLIB_INLINE static const Matrix3T<T>& one()
	{
		return *((Matrix3T<T>*)((void*)(_one)));
	}

	SLIB_INLINE static const Matrix3T<T>& identity()
	{
		return *((Matrix3T<T>*)((void*)(_identity)));
	}

public:
	SLIB_INLINE Vector3T<T> row0() const
	{
		return Vector3T<T>(m00, m01, m02);
	}
	
	SLIB_INLINE Vector3T<T> getRow0() const
	{
		return Vector3T<T>(m00, m01, m02);
	}
	
	SLIB_INLINE void setRow0(const Vector3T<T>& v)
	{
		m00 = v.x;
		m01 = v.y;
		m02 = v.z;
	}

	SLIB_INLINE Vector3T<T> row1() const
	{
		return Vector3T<T>(m10, m11, m12);
	}
	
	SLIB_INLINE Vector3T<T> getRow1() const
	{
		return Vector3T<T>(m10, m11, m12);
	}
	
	SLIB_INLINE void setRow1(const Vector3T<T>& v)
	{
		m10 = v.x;
		m11 = v.y;
		m12 = v.z;
	}

	SLIB_INLINE Vector3T<T> row2() const
	{
		return Vector3T<T>(m20, m21, m22);
	}
	
	SLIB_INLINE Vector3T<T> getRow2() const
	{
		return Vector3T<T>(m20, m21, m22);
	}
	
	SLIB_INLINE void setRow2(const Vector3T<T>& v)
	{
		m20 = v.x;
		m21 = v.y;
		m22 = v.z;
	}

	SLIB_INLINE Vector3T<T> getRow(sl_uint32 index) const
	{
		T* t = &m00 + (index + (index << 1));
		return Vector3T<T>(t[0], t[1], t[2]);
	}
	
	SLIB_INLINE void setRow(sl_uint32 index, const Vector3T<T>& v)
	{
		T* t = &m00 + (index + (index << 1));
		t[0] = v.x;
		t[1] = v.y;
		t[2] = v.z;
	}

	SLIB_INLINE Vector3T<T> column0() const
	{
		return Vector3T<T>(m00, m10, m20);
	}
	
	SLIB_INLINE Vector3T<T> getColumn0() const
	{
		return Vector3T<T>(m00, m10, m20);
	}
	
	SLIB_INLINE void setColumn0(const Vector3T<T>& v)
	{
		m00 = v.x;
		m10 = v.y;
		m20 = v.z;
	}

	SLIB_INLINE Vector3T<T> column1() const
	{
		return Vector3T<T>(m01, m11, m21);
	}
	
	SLIB_INLINE Vector3T<T> getColumn1() const
	{
		return Vector3T<T>(m01, m11, m21);
	}
	
	SLIB_INLINE void setColumn1(const Vector3T<T>& v)
	{
		m01 = v.x;
		m11 = v.y;
		m21 = v.z;
	}

	SLIB_INLINE Vector3T<T> column2() const
	{
		return Vector3T<T>(m02, m12, m22);
	}
	
	SLIB_INLINE Vector3T<T> getColumn2() const
	{
		return Vector3T<T>(m02, m12, m22);
	}
	
	SLIB_INLINE void setColumn2(const Vector3T<T>& v)
	{
		m02 = v.x;
		m12 = v.y;
		m22 = v.z;
	}

	SLIB_INLINE Vector3T<T> getColumn(sl_uint32 index) const
	{
		T* t = &m00 + index;
		return Vector3T<T>(t[0], t[3], t[6]);
	}
	
	SLIB_INLINE void setColumn(sl_uint32 index, const Vector3T<T>& v)
	{
		T* t = &m00 + index;
		t[0] = v.x;
		t[3] = v.y;
		t[6] = v.z;
	}

	SLIB_INLINE T getElement(sl_uint32 row, sl_uint32 column) const
	{
		return (&m00)[row + (row << 1) + column];
	}
	
	SLIB_INLINE void setElement(sl_uint32 row, sl_uint32 column, const T& v)
	{
		(&m00)[row + (row << 1) + column] = v;
	}

public:
	template <class O>
	SLIB_INLINE Matrix3T<T>& operator=(const Matrix3T<O>& other)
	{
		m00 = (T)(other.m00); m01 = (T)(other.m01); m02 = (T)(other.m02);
		m10 = (T)(other.m10); m11 = (T)(other.m11); m12 = (T)(other.m12);
		m20 = (T)(other.m20); m21 = (T)(other.m21); m22 = (T)(other.m22);
		return *this;
	}
	
	SLIB_INLINE sl_bool operator==(const Matrix3T<T>& other) const
	{
		return m00 == other.m00 && m01 == other.m01 && m02 == other.m02
			&& m10 == other.m10 && m11 == other.m11 && m12 == other.m12
			&& m20 == other.m20 && m21 == other.m21 && m22 == other.m22;
	}

	SLIB_INLINE sl_bool operator!=(const Matrix3T<T>& other) const
	{
		return !(*this == other);
	}

	SLIB_INLINE Matrix3T<T> operator+(const Matrix3T<T>& other) const
	{
		Matrix3T<T> ret(*this);
		ret.add(other);
		return ret;
	}
	
	SLIB_INLINE Matrix3T<T>& operator+=(const Matrix3T<T>& other)
	{
		add(other);
		return *this;
	}
	
	SLIB_INLINE Matrix3T<T> operator-(const Matrix3T<T>& other) const
	{
		Matrix3T<T> ret(*this);
		ret.subtract(other);
		return ret;
	}
	
	SLIB_INLINE Matrix3T<T>& operator-=(const Matrix3T<T>& other)
	{
		subtract(other);
		return *this;
	}
	
	SLIB_INLINE Matrix3T<T> operator-() const
	{
		Matrix3T<T> ret(Matrix3T<T>::zero());
		ret.subtract(*this);
		return ret;
	}

	SLIB_INLINE Matrix3T<T> operator*(T value) const
	{
		Matrix3T<T> ret(*this);
		ret.multiply(value);
		return ret;
	}
	
	SLIB_INLINE Matrix3T<T>& operator*=(T value)
	{
		multiply(value);
		return *this;
	}
	
	SLIB_INLINE friend Matrix3T<T> operator*(T value, const Matrix3T<T>& m)
	{
		Matrix3T<T> ret(m);
		ret.multiply(value);
		return ret;
	}
	
	SLIB_INLINE Matrix3T<T> operator/(T value) const
	{
		Matrix3T<T> ret(*this);
		ret.divide(value);
		return ret;
	}
	
	SLIB_INLINE Matrix3T<T>& operator/=(T value)
	{
		divide(value);
		return *this;
	}

	SLIB_INLINE Vector3T<T> operator*(const Vector3T<T>& v) const
	{
		return multiplyRight(v);
	}
	
	SLIB_INLINE friend Vector3T<T> operator*(const Vector3T<T>& v, const Matrix3T<T>& m)
	{
		return m.multiplyLeft(v);
	}

	SLIB_INLINE Matrix3T<T> operator*(const Matrix3T<T>& other) const
	{
		Matrix3T<T> ret(*this);
		ret.multiply(other);
		return ret;
	}
	
	SLIB_INLINE Matrix3T<T>& operator*=(const Matrix3T<T>& other)
	{
		multiply(other);
		return *this;
	}

public:
	void add(const Matrix3T<T>& other)
	{
		m00 += other.m00; m01 += other.m01; m02 += other.m02;
		m10 += other.m10; m11 += other.m11; m12 += other.m12;
		m20 += other.m20; m21 += other.m21; m22 += other.m22;
	}

	void subtract(const Matrix3T<T>& other)
	{
		m00 -= other.m00; m01 -= other.m01; m02 -= other.m02;
		m10 -= other.m10; m11 -= other.m11; m12 -= other.m12;
		m20 -= other.m20; m21 -= other.m21; m22 -= other.m22;
	}

	void multiply(T value)
	{
		m00 *= value; m01 *= value; m02 *= value;
		m10 *= value; m11 *= value; m12 *= value;
		m20 *= value; m21 *= value; m22 *= value;
	}

	void divide(T value)
	{
		m00 /= value; m01 /= value; m02 /= value;
		m10 /= value; m11 /= value; m12 /= value;
		m20 /= value; m21 /= value; m22 /= value;
	}

	Vector3T<T> multiplyLeft(const Vector3T<T>& v) const
	{
		Vector3T<T> ret;
		ret.x = v.x * m00 + v.y * m10 + v.z * m20;
		ret.y = v.x * m01 + v.y * m11 + v.z * m21;
		ret.z = v.x * m02 + v.y * m12 + v.z * m22;
		return ret;
	}

	Vector3T<T> multiplyRight(const Vector3T<T>& v) const
	{
		Vector3T<T> ret;
		ret.x = m00 * v.x + m01 * v.y + m02 * v.z;
		ret.y = m10 * v.x + m11 * v.y + m12 * v.z;
		ret.z = m20 * v.x + m21 * v.y + m22 * v.z;
		return ret;
	}

	Vector2T<T> transformPosition(T x, T y) const
	{
		Vector2T<T> ret;
		ret.x = x * m00 + y * m10 + m20;
		ret.y = x * m01 + y * m11 + m21;
		return ret;
	}

	Vector2T<T> transformPosition(const Vector2T<T>& v) const
	{
		Vector2T<T> ret;
		ret.x = v.x * m00 + v.y * m10 + m20;
		ret.y = v.x * m01 + v.y * m11 + m21;
		return ret;
	}

	Vector2T<T> transformDirection(T x, T y) const
	{
		Vector2T<T> ret;
		ret.x = x * m00 + y * m10;
		ret.y = x * m01 + y * m11;
		return ret;
	}

	Vector2T<T> transformDirection(const Vector2T<T>& v) const
	{
		Vector2T<T> ret;
		ret.x = v.x * m00 + v.y * m10;
		ret.y = v.x * m01 + v.y * m11;
		return ret;
	}

	void multiply(const Matrix3T<T>& m)
	{
		T v0, v1, v2;
		v0 = m00 * m.m00 + m01 * m.m10 + m02 * m.m20;
		v1 = m00 * m.m01 + m01 * m.m11 + m02 * m.m21;
		v2 = m00 * m.m02 + m01 * m.m12 + m02 * m.m22;
		m00 = v0; m01 = v1; m02 = v2;
		v0 = m10 * m.m00 + m11 * m.m10 + m12 * m.m20;
		v1 = m10 * m.m01 + m11 * m.m11 + m12 * m.m21;
		v2 = m10 * m.m02 + m11 * m.m12 + m12 * m.m22;
		m10 = v0; m11 = v1; m12 = v2;
		v0 = m20 * m.m00 + m21 * m.m10 + m22 * m.m20;
		v1 = m20 * m.m01 + m21 * m.m11 + m22 * m.m21;
		v2 = m20 * m.m02 + m21 * m.m12 + m22 * m.m22;
		m20 = v0; m21 = v1; m22 = v2;
	}

	T getDeterminant() const
	{
		return SLIB_MATH_MATRIX_DETERMINANT3(m00, m01, m02
			, m10, m11, m12
			, m20, m21, m22);
	}

	SLIB_INLINE T determinant() const
	{
		return getDeterminant();
	}

	void makeInverse()
	{
		T A00 = SLIB_MATH_MATRIX_DETERMINANT2(m11, m12, m21, m22);
		T A01 = -SLIB_MATH_MATRIX_DETERMINANT2(m10, m12, m20, m22);
		T A02 = SLIB_MATH_MATRIX_DETERMINANT2(m10, m11, m20, m21);
		T A10 = -SLIB_MATH_MATRIX_DETERMINANT2(m01, m02, m21, m22);
		T A11 = SLIB_MATH_MATRIX_DETERMINANT2(m00, m02, m20, m22);
		T A12 = -SLIB_MATH_MATRIX_DETERMINANT2(m00, m01, m20, m21);
		T A20 = SLIB_MATH_MATRIX_DETERMINANT2(m01, m02, m11, m12);
		T A21 = -SLIB_MATH_MATRIX_DETERMINANT2(m00, m02, m10, m12);
		T A22 = SLIB_MATH_MATRIX_DETERMINANT2(m00, m01, m10, m11);
		T D = 1 / (m00*A00 + m01*A01 + m02*A02);
		m00 = D * A00; m01 = D * A10; m02 = D * A20;
		m10 = D * A01; m11 = D * A11; m12 = D * A21;
		m20 = D * A02; m21 = D * A12; m22 = D * A22;
	}

	SLIB_INLINE Matrix3T<T> inverse() const
	{
		Matrix3T<T> ret(*this);
		ret.makeInverse();
		return ret;
	}

	void makeTranspose()
	{
		T	f;
		f = m01; m01 = m10; m10 = f;
		f = m02; m02 = m20; m20 = f;
		f = m12; m12 = m21; m21 = f;
	}

	SLIB_INLINE Matrix3T<T> transpose() const
	{
		Matrix3T<T> ret(*this);
		ret.makeTranspose();
		return ret;
	}

	SLIB_INLINE void makeInverseTranspose()
	{
		makeInverse();
		makeTranspose();
	}
	
	SLIB_INLINE Matrix3T<T> inverseTranspose() const
	{
		Matrix3T<T> ret(*this);
		ret.makeInverse();
		ret.makeTranspose();
		return ret;
	}
};

template <class T>
T Matrix3T<T>::_zero[9] = {
		0, 0, 0
		, 0, 0, 0
		, 0, 0, 0 };

template <class T>
T Matrix3T<T>::_one[9] = {
		1, 1, 1
		, 1, 1, 1
		, 1, 1, 1 };

template <class T>
T Matrix3T<T>::_identity[9] = {
		1, 0, 0
		, 0, 1, 0
		, 0, 0, 1 };

typedef Matrix3T<sl_real> Matrix3;
typedef Matrix3T<float> Matrix3f;
typedef Matrix3T<double> Matrix3lf;

SLIB_MATH_NAMESPACE_END

#endif
