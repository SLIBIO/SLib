/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/math/matrix2.h"

namespace slib
{

	template <class T>
	SLIB_ALIGN(8) T Matrix2T<T>::_zero[4] = {
		0, 0
		, 0, 0 };

	template <class T>
	SLIB_ALIGN(8) T Matrix2T<T>::_one[4] = {
		1, 1
		, 1, 1 };

	template <class T>
	SLIB_ALIGN(8) T Matrix2T<T>::_identity[4] = {
		1, 0
		, 0, 1 };

	template <class T>
	Vector2T<T> Matrix2T<T>::getRow0() const
	{
		return {m00, m01};
	}

	template <class T>
	void Matrix2T<T>::setRow0(const Vector2T<T>& v)
	{
		m00 = v.x;
		m01 = v.y;
	}

	template <class T>
	Vector2T<T> Matrix2T<T>::getRow1() const
	{
		return {m10, m11};
	}

	template <class T>
	void Matrix2T<T>::setRow1(const Vector2T<T>& v)
	{
		m10 = v.x;
		m11 = v.y;
	}

	template <class T>
	Vector2T<T> Matrix2T<T>::getRow(sl_uint32 index) const
	{
		const T* t = &m00 + (index << 1);
		return {t[0], t[1]};
	}

	template <class T>
	void Matrix2T<T>::setRow(sl_uint32 index, const Vector2T<T>& v)
	{
		T* t = &m00 + (index << 1);
		t[0] = v.x;
		t[1] = v.y;
	}

	template <class T>
	Vector2T<T> Matrix2T<T>::getColumn0() const
	{
		return {m00, m10};
	}

	template <class T>
	void Matrix2T<T>::setColumn0(const Vector2T<T>& v)
	{
		m00 = v.x;
		m10 = v.y;
	}

	template <class T>
	Vector2T<T> Matrix2T<T>::getColumn1() const
	{
		return {m01, m11};
	}

	template <class T>
	void Matrix2T<T>::setColumn1(const Vector2T<T>& v)
	{
		m01 = v.x;
		m11 = v.y;
	}

	template <class T>
	Vector2T<T> Matrix2T<T>::getColumn(sl_uint32 index) const
	{
		const T* t = &m00 + index;
		return {t[0], t[2]};
	}

	template <class T>
	void Matrix2T<T>::setColumn(sl_uint32 index, const Vector2T<T>& v)
	{
		T* t = &m00 + index;
		t[0] = v.x;
		t[2] = v.y;
	}

	template <class T>
	T Matrix2T<T>::getElement(sl_uint32 row, sl_uint32 column) const
	{
		return (&m00)[(row << 1) + column];
	}

	template <class T>
	void Matrix2T<T>::setElement(sl_uint32 row, sl_uint32 column, const T& v)
	{
		(&m00)[(row << 1) + column] = v;
	}

	template <class T>
	void Matrix2T<T>::add(const Matrix2T<T>& other)
	{
		m00 += other.m00; m01 += other.m01;
		m10 += other.m10; m11 += other.m11;
	}

	template <class T>
	void Matrix2T<T>::subtract(const Matrix2T<T>& other)
	{
		m00 -= other.m00; m01 -= other.m01;
		m10 -= other.m10; m11 -= other.m11;
	}

	template <class T>
	void Matrix2T<T>::multiply(T value)
	{
		m00 *= value; m01 *= value;
		m10 *= value; m11 *= value;
	}

	template <class T>
	void Matrix2T<T>::divide(T value)
	{
		m00 /= value; m01 /= value;
		m10 /= value; m11 /= value;
	}

	template <class T>
	Vector2T<T> Matrix2T<T>::multiplyLeft(const Vector2T<T>& v) const
	{
		T _x = v.x * m00 + v.y * m10;
		T _y = v.x * m01 + v.y * m11;
		return {_x, _y};
	}

	template <class T>
	Vector2T<T> Matrix2T<T>::multiplyRight(const Vector2T<T>& v) const
	{
		T _x = m00 * v.x + m01 * v.y;
		T _y = m10 * v.x + m11 * v.y;
		return {_x, _y};
	}

	template <class T>
	void Matrix2T<T>::multiply(const Matrix2T<T>& m)
	{
		T v0, v1;
		v0 = m00 * m.m00 + m01 * m.m10;
		v1 = m00 * m.m01 + m01 * m.m11;
		m00 = v0; m01 = v1;
		v0 = m10 * m.m00 + m11 * m.m10;
		v1 = m10 * m.m01 + m11 * m.m11;
		m10 = v0; m11 = v1;
	}

	template <class T>
	T Matrix2T<T>::getDeterminant() const
	{
		return SLIB_MATH_MATRIX_DETERMINANT2(m00, m01, m10, m11);
	}

	template <class T>
	void Matrix2T<T>::makeInverse()
	{
		T A00 = m11;
		T A01 = -m10;
		T A10 = -m01;
		T A11 = m00;
		T D = 1 / SLIB_MATH_MATRIX_DETERMINANT2(m00, m01, m10, m11);
		m00 = D * A00; m01 = D * A10;
		m10 = D * A01; m11 = D * A11;
	}

	template <class T>
	Matrix2T<T> Matrix2T<T>::inverse() const
	{
		Matrix2T<T> ret(*this);
		ret.makeInverse();
		return ret;
	}

	template <class T>
	void Matrix2T<T>::makeTranspose()
	{
		T	f;
		f = m01; m01 = m10; m10 = f;
	}

	template <class T>
	Matrix2T<T> Matrix2T<T>::transpose() const
	{
		Matrix2T<T> ret(*this);
		ret.makeTranspose();
		return ret;
	}

	template <class T>
	void Matrix2T<T>::makeInverseTranspose()
	{
		makeInverse();
		makeTranspose();
	}

	template <class T>
	Matrix2T<T> Matrix2T<T>::inverseTranspose() const
	{
		Matrix2T<T> ret(*this);
		ret.makeInverse();
		ret.makeTranspose();
		return ret;
	}

	template <class T>
	Matrix2T<T> Matrix2T<T>::lerp(const Matrix2T<T>& target, float factor) const
	{
		return {
			SLIB_LERP(m00, target.m00, factor), SLIB_LERP(m01, target.m01, factor),
			SLIB_LERP(m10, target.m10, factor), SLIB_LERP(m11, target.m11, factor)};
	}

	template <class T>
	Matrix2T<T> Matrix2T<T>::operator+(const Matrix2T<T>& other) const
	{
		Matrix2T<T> ret(*this);
		ret.add(other);
		return ret;
	}

	template <class T>
	Matrix2T<T>& Matrix2T<T>::operator+=(const Matrix2T<T>& other)
	{
		add(other);
		return *this;
	}

	template <class T>
	Matrix2T<T> Matrix2T<T>::operator-(const Matrix2T<T>& other) const
	{
		Matrix2T<T> ret(*this);
		ret.subtract(other);
		return ret;
	}

	template <class T>
	Matrix2T<T>& Matrix2T<T>::operator-=(const Matrix2T<T>& other)
	{
		subtract(other);
		return *this;
	}

	template <class T>
	Matrix2T<T> Matrix2T<T>::operator-() const
	{
		Matrix2T<T> ret(Matrix2T<T>::zero());
		ret.subtract(*this);
		return ret;
	}

	template <class T>
	Matrix2T<T> Matrix2T<T>::operator*(T value) const
	{
		Matrix2T<T> ret(*this);
		ret.multiply(value);
		return ret;
	}

	template <class T>
	Matrix2T<T>& Matrix2T<T>::operator*=(T value)
	{
		multiply(value);
		return *this;
	}

	template <class T>
	Matrix2T<T> Matrix2T<T>::operator/(T value) const
	{
		Matrix2T<T> ret(*this);
		ret.divide(value);
		return ret;
	}

	template <class T>
	Matrix2T<T>& Matrix2T<T>::operator/=(T value)
	{
		divide(value);
		return *this;
	}

	template <class T>
	Vector2T<T> Matrix2T<T>::operator*(const Vector2T<T>& v) const
	{
		return multiplyRight(v);
	}

	template <class T>
	Matrix2T<T> Matrix2T<T>::operator*(const Matrix2T<T>& other) const
	{
		Matrix2T<T> ret(*this);
		ret.multiply(other);
		return ret;
	}

	template <class T>
	Matrix2T<T>& Matrix2T<T>::operator*=(const Matrix2T<T>& other)
	{
		multiply(other);
		return *this;
	}

	template <class T>
	sl_bool Matrix2T<T>::operator==(const Matrix2T<T>& other) const
	{
		return m00 == other.m00 && m01 == other.m01 &&
			m10 == other.m10 && m11 == other.m11;
	}

	template <class T>
	sl_bool Matrix2T<T>::operator!=(const Matrix2T<T>& other) const
	{
		return m00 != other.m00 || m01 != other.m01 ||
			m10 != other.m10 || m11 != other.m11;
	}

	SLIB_DEFINE_GEOMETRY_TYPE(Matrix2)

}
