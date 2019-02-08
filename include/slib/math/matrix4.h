/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#ifndef CHECKHEADER_SLIB_MATH_MATRIX4
#define CHECKHEADER_SLIB_MATH_MATRIX4

#include "definition.h"

#include "matrix3.h"
#include "vector4.h"

#include "../core/interpolation.h"

#define SLIB_MATH_MATRIX_DETERMINANT4(m00,m01,m02,m03,m10,m11,m12,m13,m20,m21,m22,m23,m30,m31,m32,m33) ((m00)*SLIB_MATH_MATRIX_DETERMINANT3(m11,m12,m13,m21,m22,m23,m31,m32,m33)-(m01)*SLIB_MATH_MATRIX_DETERMINANT3(m10,m12,m13,m20,m22,m23,m30,m32,m33)+(m02)*SLIB_MATH_MATRIX_DETERMINANT3(m10,m11,m13,m20,m21,m23,m30,m31,m33)-(m03)*SLIB_MATH_MATRIX_DETERMINANT3(m10,m11,m12,m20,m21,m22,m30,m31,m32))

namespace slib
{
	
	template <class T>
	class SLIB_EXPORT Matrix4T
	{
	public:
		T m00; T m01; T m02; T m03;
		T m10; T m11; T m12; T m13;
		T m20; T m21; T m22; T m23;
		T m30; T m31; T m32; T m33;
	
	public:
		SLIB_DEFINE_CLASS_DEFAULT_MEMBERS_INLINE(Matrix4T)
		
		SLIB_INLINE Matrix4T() noexcept = default;

		template <class O>
		SLIB_INLINE constexpr Matrix4T(const Matrix4T<O>& other) noexcept
		 : m00((T)(other.m00)), m01((T)(other.m01)), m02((T)(other.m02)), m03((T)(other.m03)),
		   m10((T)(other.m10)), m11((T)(other.m11)), m12((T)(other.m12)), m13((T)(other.m13)),
		   m20((T)(other.m20)), m21((T)(other.m21)), m22((T)(other.m22)), m23((T)(other.m23)),
		   m30((T)(other.m30)), m31((T)(other.m31)), m32((T)(other.m32)), m33((T)(other.m33))
		{}
	
		SLIB_INLINE Matrix4T(
			T _m00, T _m01, T _m02, T _m03,
			T _m10, T _m11, T _m12, T _m13,
			T _m20, T _m21, T _m22, T _m23,
			T _m30, T _m31, T _m32, T _m33
		) noexcept
		 : m00(_m00), m01(_m01), m02(_m02), m03(_m03),
		   m10(_m10), m11(_m11), m12(_m12), m13(_m13),
		   m20(_m20), m21(_m21), m22(_m22), m23(_m23),
		   m30(_m30), m31(_m31), m32(_m32), m33(_m33)
		{}
	
		SLIB_INLINE Matrix4T(const Vector4T<T>& row0, const Vector4T<T>& row1, const Vector4T<T>& row2, const Vector4T<T>& row3) noexcept
		 : m00(row0.x), m01(row0.y), m02(row0.z), m03(row0.w),
		   m10(row1.x), m11(row1.y), m12(row1.z), m13(row1.w),
		   m20(row2.x), m21(row2.y), m22(row2.z), m23(row2.w),
		   m30(row3.x), m31(row3.y), m32(row3.z), m33(row3.w)
		{}

	public:
		static const Matrix4T<T>& zero() noexcept;

		static const Matrix4T<T>& one() noexcept;

		static const Matrix4T<T>& identity() noexcept;

		static const Matrix4T<T>& fromArray(const T arr[16]) noexcept;

		static Matrix4T<T>& fromArray(T arr[16]) noexcept;

		Vector4T<T> getRow0() const noexcept;

		void setRow0(const Vector4T<T>& v) noexcept;

		Vector4T<T> getRow1() const noexcept;

		void setRow1(const Vector4T<T>& v) noexcept;

		Vector4T<T> getRow2() const noexcept;

		void setRow2(const Vector4T<T>& v) noexcept;

		Vector4T<T> getRow3() const noexcept;

		void setRow3(const Vector4T<T>& v) noexcept;
	
		Vector4T<T> getRow(sl_uint32 index) const noexcept;

		void setRow(sl_uint32 index, const Vector4T<T>& v) noexcept;
	
		Vector4T<T> getColumn0() const noexcept;

		void setColumn0(const Vector4T<T>& v) noexcept;

		Vector4T<T> getColumn1() const noexcept;

		void setColumn1(const Vector4T<T>& v) noexcept;

		Vector4T<T> getColumn2() const noexcept;

		void setColumn2(const Vector4T<T>& v) noexcept;

		Vector4T<T> getColumn3() const noexcept;

		void setColumn3(const Vector4T<T>& v) noexcept;
	
		Vector4T<T> getColumn(sl_uint32 index) const noexcept;

		void setColumn(sl_uint32 index, const Vector4T<T>& v) noexcept;
	
		T getElement(sl_uint32 row, sl_uint32 column) const noexcept;

		void setElement(sl_uint32 row, sl_uint32 column, const T& v) noexcept;
	

		void add(const Matrix4T<T>& other) noexcept;

		void subtract(const Matrix4T<T>& other) noexcept;

		void multiply(T value) noexcept;

		void divide(T value) noexcept;

		Vector4T<T> multiplyLeft(const Vector4T<T>& v) const noexcept;

		Vector4T<T> multiplyRight(const Vector4T<T>& v) const noexcept;

		Vector3T<T> transformPosition(T x, T y, T z) const noexcept;

		Vector3T<T> transformPosition(const Vector3T<T>& v) const noexcept;

		Vector3T<T> transformDirection(T x, T y, T z) const noexcept;

		Vector3T<T> transformDirection(const Vector3T<T>& v) const noexcept;

		void multiply(const Matrix4T<T>& m) noexcept;

		T getDeterminant() const noexcept;

		void makeInverse() noexcept;

		Matrix4T<T> inverse() const noexcept;

		void makeTranspose() noexcept;

		Matrix4T<T> transpose() const noexcept;

		void makeInverseTranspose() noexcept;

		Matrix4T<T> inverseTranspose() const noexcept;

		Matrix4T<T> lerp(const Matrix4T<T>& target, float factor) const noexcept;
	
	public:
		template <class O>
		Matrix4T<T>& operator=(const Matrix4T<O>& other) noexcept;
	
		Matrix4T<T> operator+(const Matrix4T<T>& other) const noexcept;
	
		Matrix4T<T>& operator+=(const Matrix4T<T>& other) noexcept;
	
		Matrix4T<T> operator-(const Matrix4T<T>& other) const noexcept;
	
		Matrix4T<T>& operator-=(const Matrix4T<T>& other) noexcept;

		Matrix4T<T> operator-() const noexcept;
	
		Matrix4T<T> operator*(T value) const noexcept;
	
		Matrix4T<T>& operator*=(T value) noexcept;

		Matrix4T<T> operator/(T value) const noexcept;
	
		Matrix4T<T>& operator/=(T value) noexcept;
	
		Vector4T<T> operator*(const Vector4T<T>& v) const noexcept;
	
		Matrix4T<T> operator*(const Matrix4T<T>& other) const noexcept;
	
		Matrix4T<T>& operator*=(const Matrix4T<T>& other) noexcept;

		sl_bool operator==(const Matrix4T<T>& other) const noexcept;

		sl_bool operator!=(const Matrix4T<T>& other) const noexcept;

	private:
		static T _zero[16];
		static T _one[16];
		static T _identity[16];
	
	};
	
	template <class T>
	Matrix4T<T> operator*(T value, const Matrix4T<T>& m) noexcept;
	
	template <class T>
	Vector4T<T> operator*(const Vector4T<T>& v, const Matrix4T<T>& m) noexcept;
	
	template <class T>
	class Interpolation< Matrix4T<T> >
	{
	public:
		static Matrix4T<T> interpolate(const Matrix4T<T>& a, const Matrix4T<T>& b, float factor) noexcept;
	};
	
	extern template class Matrix4T<float>;
	extern template class Matrix4T<double>;
	typedef Matrix4T<sl_real> Matrix4;
	typedef Matrix4T<float> Matrix4f;
	typedef Matrix4T<double> Matrix4lf;

}

#include "detail/matrix4.inc"

#endif
