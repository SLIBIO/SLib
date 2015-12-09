#ifndef CHECKHEADER_SLIB_MATH_MATRIX2
#define CHECKHEADER_SLIB_MATH_MATRIX2

#include "definition.h"

#include "vector2.h"

#include "../core/math.h"

SLIB_MATH_NAMESPACE_BEGIN

#define SLIB_MATH_MATRIX_DETERMINANT2(m00,m01,m10,m11) ((m00)*(m11)-(m01)*(m10))

template <class T>
class SLIB_EXPORT Matrix2T
{
public:
	T m00; T m01;
	T m10; T m11;

public:
	SLIB_INLINE Matrix2T()
	{
	}

	template <class O>
	SLIB_INLINE Matrix2T(const Matrix2T<O>& other)
	{
		m00 = (T)(other.m00); m01 = (T)(other.m01);
		m10 = (T)(other.m10); m11 = (T)(other.m11);
	}

	SLIB_INLINE Matrix2T(
		T m00, T m01
		, T m10, T m11)
	{
		this->m00 = m00; this->m01 = m01;
		this->m10 = m10; this->m11 = m11;
	}

private:
	static T _zero[4];
	static T _one[4];
	static T _identity[4];
	
public:
	SLIB_INLINE static const Matrix2T<T>& zero()
	{
		return *((Matrix2T<T>*)((void*)(_zero)));
	}

	SLIB_INLINE static const Matrix2T<T>& one()
	{
		return *((Matrix2T<T>*)((void*)(_one)));
	}

	SLIB_INLINE static const Matrix2T<T>& identity()
	{
		return *((Matrix2T<T>*)((void*)(_identity)));
	}

public:
	SLIB_INLINE Vector2T<T> row0() const
	{
		return Vector2T<T>(m00, m01);
	}
	
	SLIB_INLINE Vector2T<T> getRow0() const
	{
		return Vector2T<T>(m00, m01);
	}
	
	SLIB_INLINE void setRow0(const Vector2T<T>& v)
	{
		m00 = v.x;
		m01 = v.y;
	}

	SLIB_INLINE Vector2T<T> row1() const
	{
		return Vector2T<T>(m10, m11);
	}
	
	SLIB_INLINE Vector2T<T> getRow1() const
	{
		return Vector2T<T>(m10, m11);
	}
	
	SLIB_INLINE void setRow1(const Vector2T<T>& v)
	{
		m10 = v.x;
		m11 = v.y;
	}

	SLIB_INLINE Vector2T<T> getRow(sl_uint32 index) const
	{
		T* t = &m00 + (index << 1);
		return Vector2T<T>(t[0], t[1]);
	}
	
	SLIB_INLINE void setRow(sl_uint32 index, const Vector2T<T>& v)
	{
		T* t = &m00 + (index << 1);
		t[0] = v.x;
		t[1] = v.y;
	}

	SLIB_INLINE Vector2T<T> column0() const
	{
		return Vector2T<T>(m00, m10);
	}
	
	SLIB_INLINE Vector2T<T> getColumn0() const
	{
		return Vector2T<T>(m00, m10);
	}
	
	SLIB_INLINE void setColumn0(const Vector2T<T>& v)
	{
		m00 = v.x;
		m10 = v.y;
	}

	SLIB_INLINE Vector2T<T> column1() const
	{
		return Vector2T<T>(m01, m11);
	}
	
	SLIB_INLINE Vector2T<T> getColumn1() const
	{
		return Vector2T<T>(m01, m11);
	}
	
	SLIB_INLINE void setColumn1(const Vector2T<T>& v)
	{
		m01 = v.x;
		m11 = v.y;
	}

	SLIB_INLINE Vector2T<T> getColumn(sl_uint32 index) const
	{
		T* t = &m00 + index;
		return Vector2T<T>(t[0], t[2]);
	}
	
	SLIB_INLINE void setColumn(sl_uint32 index, const Vector2T<T>& v)
	{
		T* t = &m00 + index;
		t[0] = v.x;
		t[2] = v.y;
	}

	SLIB_INLINE T getElement(sl_uint32 row, sl_uint32 column) const
	{
		return (&m00)[(row << 1) + column];
	}
	
	SLIB_INLINE void setElement(sl_uint32 row, sl_uint32 column, const T& v)
	{
		(&m00)[(row << 1) + column] = v;
	}

public:
	template <class O>
	SLIB_INLINE Matrix2T<T>& operator=(const Matrix2T<O>& other)
	{
		m00 = (T)(other.m00); m01 = (T)(other.m01);
		m10 = (T)(other.m10); m11 = (T)(other.m11);
		return *this;
	}
	
	SLIB_INLINE sl_bool operator==(const Matrix2T<T>& other) const
	{
		return m00 == other.m00 && m01 == other.m01
			&& m10 == other.m10 && m11 == other.m11;
	}
	
	SLIB_INLINE sl_bool operator!=(const Matrix2T<T>& other) const
	{
		return !(*this == other);
	}

	SLIB_INLINE Matrix2T<T> operator+(const Matrix2T<T>& other) const
	{
		Matrix2T<T> ret(*this);
		ret.add(other);
		return ret;
	}
	
	SLIB_INLINE Matrix2T<T>& operator+=(const Matrix2T<T>& other)
	{
		add(other);
		return *this;
	}
	
	SLIB_INLINE Matrix2T<T> operator-(const Matrix2T<T>& other) const
	{
		Matrix2T<T> ret(*this);
		ret.subtract(other);
		return ret;
	}
	
	SLIB_INLINE Matrix2T<T>& operator-=(const Matrix2T<T>& other)
	{
		subtract(other);
		return *this;
	}
	
	SLIB_INLINE Matrix2T<T> operator-() const
	{
		Matrix2T<T> ret(Matrix2T<T>::zero());
		ret.subtract(*this);
		return ret;
	}

	SLIB_INLINE Matrix2T<T> operator*(T value) const
	{
		Matrix2T<T> ret(*this);
		ret.multiply(value);
		return ret;
	}
	
	SLIB_INLINE Matrix2T<T>& operator*=(T value)
	{
		multiply(value);
		return *this;
	}
	
	SLIB_INLINE friend Matrix2T<T> operator*(T value, const Matrix2T<T>& m)
	{
		Matrix2T<T> ret(m);
		ret.multiply(value);
		return ret;
	}
	
	SLIB_INLINE Matrix2T<T> operator/(T value) const
	{
		Matrix2T<T> ret(*this);
		ret.divide(value);
		return ret;
	}
	
	SLIB_INLINE Matrix2T<T>& operator/=(T value)
	{
		divide(value);
		return *this;
	}

	SLIB_INLINE Vector2T<T> operator*(const Vector2T<T>& v) const
	{
		return multiplyRight(v);
	}
	
	SLIB_INLINE friend Vector2T<T> operator*(const Vector2T<T>& v, const Matrix2T<T>& m)
	{
		return m.multiplyLeft(v);
	}

	SLIB_INLINE Matrix2T<T> operator*(const Matrix2T<T>& other) const
	{
		Matrix2T<T> ret(*this);
		ret.multiply(other);
		return ret;
	}
	
	SLIB_INLINE Matrix2T<T>& operator*=(const Matrix2T<T>& other)
	{
		multiply(other);
		return *this;
	}

public:
	void add(const Matrix2T<T>& other)
	{
		m00 += other.m00; m01 += other.m01;
		m10 += other.m10; m11 += other.m11;
	}

	void subtract(const Matrix2T<T>& other)
	{
		m00 -= other.m00; m01 -= other.m01;
		m10 -= other.m10; m11 -= other.m11;
	}

	void multiply(T value)
	{
		m00 *= value; m01 *= value;
		m10 *= value; m11 *= value;
	}

	void divide(T value)
	{
		m00 /= value; m01 /= value;
		m10 /= value; m11 /= value;
	}

	Vector2T<T> multiplyLeft(const Vector2T<T>& v) const
	{
		Vector2T<T> ret;
		ret.x = v.x * m00 + v.y * m10;
		ret.y = v.x * m01 + v.y * m11;
		return ret;
	}

	Vector2T<T> multiplyRight(const Vector2T<T>& v) const
	{
		Vector2T<T> ret;
		ret.x = m00 * v.x + m01 * v.y;
		ret.y = m10 * v.x + m11 * v.y;
		return ret;
	}

	void multiply(const Matrix2T<T>& m)
	{
		T v0, v1;
		v0 = m00 * m.m00 + m01 * m.m10;
		v1 = m00 * m.m01 + m01 * m.m11;
		m00 = v0; m01 = v1;
		v0 = m10 * m.m00 + m11 * m.m10;
		v1 = m10 * m.m01 + m11 * m.m11;
		m10 = v0; m11 = v1;
	}

	T getDeterminant() const
	{
		return SLIB_MATH_MATRIX_DETERMINANT2(m00, m01, m10, m11);
	}

	SLIB_INLINE T determinant() const
	{
		return getDeterminant();
	}

	void makeInverse()
	{
		T A00 = m11;
		T A01 = -m10;
		T A10 = -m01;
		T A11 = m00;
		T D = 1 / SLIB_MATH_MATRIX_DETERMINANT2(m00, m01, m10, m11);
		m00 = D * A00; m01 = D * A10;
		m10 = D * A01; m11 = D * A11;
	}

	SLIB_INLINE Matrix2T<T> inverse() const
	{
		Matrix2T<T> ret(*this);
		ret.makeInverse();
		return ret;
	}

	void makeTranspose()
	{
		T	f;
		f = m01; m01 = m10; m10 = f;
	}

	SLIB_INLINE Matrix2T<T> transpose() const
	{
		Matrix2T<T> ret(*this);
		ret.makeTranspose();
		return ret;
	}

	SLIB_INLINE void makeInverseTranspose()
	{
		makeInverse();
		makeTranspose();
	}

	SLIB_INLINE Matrix2T<T> inverseTranspose() const
	{
		Matrix2T<T> ret(*this);
		ret.makeInverse();
		ret.makeTranspose();
		return ret;
	}

};

template <class T>
T Matrix2T<T>::_zero[4] = {
		0, 0
		, 0, 0 };

template <class T>
T Matrix2T<T>::_one[4] = {
		1, 1
		, 1, 1 };

template <class T>
T Matrix2T<T>::_identity[4] = {
		1, 0
		, 0, 1 };

typedef Matrix2T<sl_real> Matrix2;
typedef Matrix2T<float> Matrix2f;
typedef Matrix2T<double> Matrix2lf;

SLIB_MATH_NAMESPACE_END

#endif
