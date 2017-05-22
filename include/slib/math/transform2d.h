/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

	};
	
	extern template class Transform2T<float>;
	extern template class Transform2T<double>;
	typedef Transform2T<sl_real> Transform2;
	typedef Transform2T<float> Transform2f;
	typedef Transform2T<double> Transform2lf;

}

#endif
