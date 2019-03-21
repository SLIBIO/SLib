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

#ifndef CHECKHEADER_SLIB_MATH_TRANSFORM2D
#define CHECKHEADER_SLIB_MATH_TRANSFORM2D

#include "definition.h"

#include "vector2.h"
#include "matrix3.h"

namespace slib
{
	
	template <class T>
	class SLIB_EXPORT Transform2T
	{
	public:
		static void setTranslation(Matrix3T<T>& _out, T x, T y) noexcept;

		static void setTranslation(Matrix3T<T>& _out, const Vector2T<T>& v) noexcept;

		static Matrix3T<T> getTranslationMatrix(T x, T y) noexcept;

		static Matrix3T<T> getTranslationMatrix(const Vector2T<T>& v) noexcept;

		static void translate(Matrix3T<T>& mat, T x, T y) noexcept;

		static void translate(Matrix3T<T>& mat, const Vector2T<T>& v) noexcept;
		
		static void preTranslate(Matrix3T<T>& mat, T x, T y) noexcept;
		
		static void preTranslate(Matrix3T<T>& mat, const Vector2T<T>& v) noexcept;
		
		static sl_bool isTranslation(const Matrix3T<T>& mat) noexcept;


		static void setScaling(Matrix3T<T>& _out, T sx, T sy) noexcept;

		static void setScaling(Matrix3T<T>& _out, const Vector2T<T>& v) noexcept;

		static Matrix3T<T> getScalingMatrix(T x, T y) noexcept;

		static Matrix3T<T> getScalingMatrix(const Vector2T<T>& v) noexcept;

		static void scale(Matrix3T<T>& mat, T sx, T sy) noexcept;

		static void scale(Matrix3T<T>& mat, const Vector2T<T>& v) noexcept;
	

		static void setRotation(Matrix3T<T>& _out, T radians) noexcept;

		static void setRotation(Matrix3T<T>& _out, T cx, T cy, T radians) noexcept;

		static void setRotation(Matrix3T<T>& _out, const Vector2T<T>& pt, T radians) noexcept;

		static Matrix3T<T> getRotationMatrix(T radians) noexcept;

		static Matrix3T<T> getRotationMatrix(T cx, T cy, T radians) noexcept;

		static Matrix3T<T> getRotationMatrix(const Vector2T<T>& pt, T radians) noexcept;

		static void rotate(Matrix3T<T>& mat, T radians) noexcept;

		static void rotate(Matrix3T<T>& mat, T cx, T cy, T radians) noexcept;

		static void rotate(Matrix3T<T>& mat, const Vector2T<T>& pt, T radians) noexcept;
	

		static T getRotationAngleFromDirToDir(const Vector2T<T>& from, const Vector2T<T>& to) noexcept;

		static void setTransformFromDirToDir(Matrix3T<T>& _out, const Vector2T<T>& from, const Vector2T<T>& to) noexcept;

		static Matrix3T<T> getTransformMatrixFromDirToDir(const Vector2T<T>& from, const Vector2T<T>& to) noexcept;
		
		
		static Vector2T<T> getScaleFromMatrix(const Matrix3T<T>& mat);
		
		static T getRotationAngleFromMatrix(const Matrix3T<T>& mat);
		
		static Vector2T<T> getTranslationFromMatrix(const Matrix3T<T>& mat);
		
	};
	
	extern template class Transform2T<float>;
	extern template class Transform2T<double>;
	typedef Transform2T<sl_real> Transform2;
	typedef Transform2T<float> Transform2f;
	typedef Transform2T<double> Transform2lf;

}

#endif
