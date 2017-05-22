/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MATH_MATRIX3
#define CHECKHEADER_SLIB_MATH_MATRIX3

#include "definition.h"

#include "matrix2.h"
#include "vector3.h"

#include "../core/interpolation.h"

#define SLIB_MATH_MATRIX_DETERMINANT3(m00,m01,m02,m10,m11,m12,m20,m21,m22) ((m00)*SLIB_MATH_MATRIX_DETERMINANT2(m11,m12,m21,m22)-(m01)*SLIB_MATH_MATRIX_DETERMINANT2(m10,m12,m20,m22)+(m02)*SLIB_MATH_MATRIX_DETERMINANT2(m10,m11,m20,m21))

namespace slib
{
	
	template <class T>
	class SLIB_EXPORT Matrix3T
	{
	public:
		T m00; T m01; T m02;
		T m10; T m11; T m12;
		T m20; T m21; T m22;
	
	public:
		SLIB_INLINE Matrix3T() noexcept = default;
	
		SLIB_INLINE constexpr Matrix3T(const Matrix3T<T>& other) noexcept
		:m00(other.m00), m01(other.m01), m02(other.m02),
		 m10(other.m10), m11(other.m11), m12(other.m12),
		 m20(other.m20), m21(other.m21), m22(other.m22)
		{}

		template <class O>
		SLIB_INLINE constexpr Matrix3T(const Matrix3T<O>& other) noexcept
		:m00((T)(other.m00)), m01((T)(other.m01)), m02((T)(other.m02)),
		 m10((T)(other.m10)), m11((T)(other.m11)), m12((T)(other.m12)),
		 m20((T)(other.m20)), m21((T)(other.m21)), m22((T)(other.m22))
		{}
	
		SLIB_INLINE constexpr Matrix3T(
			T _m00, T _m01, T _m02,
			T _m10, T _m11, T _m12,
			T _m20, T _m21, T _m22
		) noexcept
		:m00(_m00), m01(_m01), m02(_m02),
		 m10(_m10), m11(_m11), m12(_m12),
		 m20(_m20), m21(_m21), m22(_m22)
		{}
	
		SLIB_INLINE constexpr Matrix3T(const Vector3T<T>& row0, const Vector3T<T>& row1, const Vector3T<T>& row2)
		:m00(row0.x), m01(row0.y), m02(row0.z),
		 m10(row1.x), m11(row1.y), m12(row1.z),
		 m20(row2.x), m21(row2.y), m22(row2.z)
		{}

	public:
		static const Matrix3T<T>& zero() noexcept;

		static const Matrix3T<T>& one() noexcept;

		static const Matrix3T<T>& identity() noexcept;

		static const Matrix3T<T>& fromArray(const T arr[9]) noexcept;

		static Matrix3T<T>& fromArray(T arr[9]) noexcept;

	public:
		Vector3T<T> getRow0() const noexcept;

		void setRow0(const Vector3T<T>& v) noexcept;
	
		Vector3T<T> getRow1() const noexcept;

		void setRow1(const Vector3T<T>& v) noexcept;

		Vector3T<T> getRow2() const noexcept;

		void setRow2(const Vector3T<T>& v) noexcept;
	
		Vector3T<T> getRow(sl_uint32 index) const noexcept;

		void setRow(sl_uint32 index, const Vector3T<T>& v) noexcept;

		Vector3T<T> getColumn0() const noexcept;

		void setColumn0(const Vector3T<T>& v) noexcept;

		Vector3T<T> getColumn1() const noexcept;

		void setColumn1(const Vector3T<T>& v) noexcept;

		Vector3T<T> getColumn2() const noexcept;

		void setColumn2(const Vector3T<T>& v) noexcept;
	
		Vector3T<T> getColumn(sl_uint32 index) const noexcept;

		void setColumn(sl_uint32 index, const Vector3T<T>& v) noexcept;
	
		T getElement(sl_uint32 row, sl_uint32 column) const noexcept;

		void setElement(sl_uint32 row, sl_uint32 column, const T& v) noexcept;


		void add(const Matrix3T<T>& other) noexcept;

		void subtract(const Matrix3T<T>& other) noexcept;

		void multiply(T value) noexcept;

		void divide(T value) noexcept;

		Vector3T<T> multiplyLeft(const Vector3T<T>& v) const noexcept;

		Vector3T<T> multiplyRight(const Vector3T<T>& v) const noexcept;

		Vector2T<T> transformPosition(T x, T y) const noexcept;

		Vector2T<T> transformPosition(const Vector2T<T>& v) const noexcept;

		Vector2T<T> transformDirection(T x, T y) const noexcept;

		Vector2T<T> transformDirection(const Vector2T<T>& v) const noexcept;

		void multiply(const Matrix3T<T>& m) noexcept;

		T getDeterminant() const noexcept;

		void makeInverse() noexcept;

		Matrix3T<T> inverse() const noexcept;

		void makeTranspose() noexcept;

		Matrix3T<T> transpose() const noexcept;

		void makeInverseTranspose() noexcept;

		Matrix3T<T> inverseTranspose() const noexcept;
	
		Matrix3T<T> lerp(const Matrix3T<T>& target, float factor) const noexcept;
	
	public:
		Matrix3T<T>& operator=(const Matrix3T<T>& other) noexcept = default;

		template <class O>
		Matrix3T<T>& operator=(const Matrix3T<O>& other) noexcept;

		Matrix3T<T> operator+(const Matrix3T<T>& other) const noexcept;

		Matrix3T<T>& operator+=(const Matrix3T<T>& other) noexcept;

		Matrix3T<T> operator-(const Matrix3T<T>& other) const noexcept;

		Matrix3T<T>& operator-=(const Matrix3T<T>& other) noexcept;

		Matrix3T<T> operator-() const noexcept;
	
		Matrix3T<T> operator*(T value) const noexcept;

		Matrix3T<T>& operator*=(T value) noexcept;

		Matrix3T<T> operator/(T value) const noexcept;

		Matrix3T<T>& operator/=(T value) noexcept;
	
		Vector3T<T> operator*(const Vector3T<T>& v) const noexcept;

		Matrix3T<T> operator*(const Matrix3T<T>& other) const noexcept;

		Matrix3T<T>& operator*=(const Matrix3T<T>& other) noexcept;

		sl_bool operator==(const Matrix3T<T>& other) const noexcept;

		sl_bool operator!=(const Matrix3T<T>& other) const noexcept;

	private:
		static T _zero[9];
		static T _one[9];
		static T _identity[9];
	
	};
	
	extern template class Matrix3T<float>;
	extern template class Matrix3T<double>;
	typedef Matrix3T<sl_real> Matrix3;
	typedef Matrix3T<float> Matrix3f;
	typedef Matrix3T<double> Matrix3lf;
	
	template <class T>
	Matrix3T<T> operator*(T value, const Matrix3T<T>& m) noexcept;
	
	template <class T>
	Vector3T<T> operator*(const Vector3T<T>& v, const Matrix3T<T>& m) noexcept;
	
	template <class T>
	class Interpolation< Matrix3T<T> >
	{
	public:
		static Matrix3T<T> interpolate(const Matrix3T<T>& a, const Matrix3T<T>& b, float factor) noexcept;
	};

}

#include "detail/matrix3.inc"

#endif
