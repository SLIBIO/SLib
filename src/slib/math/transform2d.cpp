/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/math/transform2d.h"

#include "slib/core/math.h"

namespace slib
{

	template <class T>
	void Transform2T<T>::setTranslation(Matrix3T<T>& _out, T x, T y) noexcept
	{
		_out.m00 = 1; _out.m01 = 0; _out.m02 = 0;
		_out.m10 = 0; _out.m11 = 1; _out.m12 = 0;
		_out.m20 = x; _out.m21 = y; _out.m22 = 1;
	}

	template <class T>
	void Transform2T<T>::setTranslation(Matrix3T<T>& _out, const Vector2T<T>& v) noexcept
	{
		_out.m00 = 1; _out.m01 = 0; _out.m02 = 0;
		_out.m10 = 0; _out.m11 = 1; _out.m12 = 0;
		_out.m20 = v.x; _out.m21 = v.y; _out.m22 = 1;
	}

	template <class T>
	Matrix3T<T> Transform2T<T>::getTranslationMatrix(T x, T y) noexcept
	{
		return {1, 0, 0,
				0, 1, 0,
				x, y, 1};
	}

	template <class T>
	Matrix3T<T> Transform2T<T>::getTranslationMatrix(const Vector2T<T>& v) noexcept
	{
		return {1, 0, 0,
				0, 1, 0,
				v.x, v.y, 1};
	}

	template <class T>
	void Transform2T<T>::translate(Matrix3T<T>& mat, T x, T y) noexcept
	{
		mat.m20 += x;
		mat.m21 += y;
	}

	template <class T>
	void Transform2T<T>::translate(Matrix3T<T>& mat, const Vector2T<T>& v) noexcept
	{
		mat.m20 += v.x;
		mat.m21 += v.y;
	}
	
	template <class T>
	void Transform2T<T>::preTranslate(Matrix3T<T>& mat, T x, T y) noexcept
	{
		mat.m20 += (x * mat.m00 + y * mat.m10);
		mat.m21 += (x * mat.m01 + y * mat.m11);
	}
	
	template <class T>
	void Transform2T<T>::preTranslate(Matrix3T<T>& mat, const Vector2T<T>& v) noexcept
	{
		mat.m20 += (v.x * mat.m00 + v.y * mat.m10);
		mat.m21 += (v.x * mat.m01 + v.y * mat.m11);
	}
	
	template <class T>
	sl_bool Transform2T<T>::isTranslation(const Matrix3T<T>& mat) noexcept
	{
		return Math::isAlmostZero(mat.m00 - 1) && Math::isAlmostZero(mat.m11 - 1) && Math::isAlmostZero(mat.m01) && Math::isAlmostZero(mat.m10);
	}
	
	template <class T>
	void Transform2T<T>::setScaling(Matrix3T<T>& _out, T sx, T sy) noexcept
	{
		_out.m00 = sx; _out.m01 = 0; _out.m02 = 0;
		_out.m10 = 0; _out.m11 = sy; _out.m12 = 0;
		_out.m20 = 0; _out.m21 = 0; _out.m22 = 1;
	}

	template <class T>
	void Transform2T<T>::setScaling(Matrix3T<T>& _out, const Vector2T<T>& v) noexcept
	{
		_out.m00 = v.x; _out.m01 = 0; _out.m02 = 0;
		_out.m10 = 0; _out.m11 = v.y; _out.m12 = 0;
		_out.m20 = 0; _out.m21 = 0; _out.m22 = 1;
	}

	template <class T>
	Matrix3T<T> Transform2T<T>::getScalingMatrix(T x, T y) noexcept
	{
		return {x, 0, 0,
				0, y, 0,
				0, 0, 1};
	}

	template <class T>
	Matrix3T<T> Transform2T<T>::getScalingMatrix(const Vector2T<T>& v) noexcept
	{
		return {v.x, 0, 0,
				0, v.y, 0,
				0, 0, 1};
	}

	template <class T>
	void Transform2T<T>::scale(Matrix3T<T>& mat, T sx, T sy) noexcept
	{
		mat.m00 *= sx;
		mat.m10 *= sx;
		mat.m20 *= sx;
		mat.m01 *= sy;
		mat.m11 *= sy;
		mat.m21 *= sy;
	}

	template <class T>
	void Transform2T<T>::scale(Matrix3T<T>& mat, const Vector2T<T>& v) noexcept
	{
		scale(mat, v.x, v.y);
	}

	template <class T>
	void Transform2T<T>::setRotation(Matrix3T<T>& _out, T radians) noexcept
	{
		T c = Math::cos(radians);
		T s = Math::sin(radians);
		_out.m00 = c; _out.m01 = s; _out.m02 = 0;
		_out.m10 = -s; _out.m11 = c; _out.m12 = 0;
		_out.m20 = 0; _out.m21 = 0; _out.m22 = 1;
	}

	template <class T>
	void Transform2T<T>::setRotation(Matrix3T<T>& _out, T cx, T cy, T radians) noexcept
	{
		setTranslation(_out, -cx, -cy);
		rotate(_out, radians);
		translate(_out, cx, cy);
	}

	template <class T>
	void Transform2T<T>::setRotation(Matrix3T<T>& _out, const Vector2T<T>& pt, T radians) noexcept
	{
		setRotation(_out, pt.x, pt.y, radians);
	}

	template <class T>
	Matrix3T<T> Transform2T<T>::getRotationMatrix(T radians) noexcept
	{
		T c = Math::cos(radians);
		T s = Math::sin(radians);
		return {c, s, 0,
			-s, c, 0,
			0, 0, 1};
	}

	template <class T>
	Matrix3T<T> Transform2T<T>::getRotationMatrix(T cx, T cy, T radians) noexcept
	{
		Matrix3T<T> ret;
		setRotation(ret, cx, cy, radians);
		return ret;
	}

	template <class T>
	Matrix3T<T> Transform2T<T>::getRotationMatrix(const Vector2T<T>& pt, T radians) noexcept
	{
		Matrix3T<T> ret;
		setRotation(ret, pt, radians);
		return ret;
	}

	template <class T>
	void Transform2T<T>::rotate(Matrix3T<T>& mat, T radians) noexcept
	{
		T c = Math::cos(radians);
		T s = Math::sin(radians);
		T _m00 = mat.m00 * c - mat.m01 * s;
		T _m01 = mat.m00 * s + mat.m01 * c;
		T _m10 = mat.m10 * c - mat.m11 * s;
		T _m11 = mat.m10 * s + mat.m11 * c;
		T _m20 = mat.m20 * c - mat.m21 * s;
		T _m21 = mat.m20 * s + mat.m21 * c;
		mat.m00 = _m00;
		mat.m01 = _m01;
		mat.m10 = _m10;
		mat.m11 = _m11;
		mat.m20 = _m20;
		mat.m21 = _m21;
	}

	template <class T>
	void Transform2T<T>::rotate(Matrix3T<T>& mat, T cx, T cy, T radians) noexcept
	{
		translate(mat, -cx, -cy);
		rotate(mat, radians);
		translate(mat, cx, cy);
	}

	template <class T>
	void Transform2T<T>::rotate(Matrix3T<T>& mat, const Vector2T<T>& pt, T radians) noexcept
	{
		rotate(mat, pt.x, pt.y, radians);
	}

	template <class T>
	T Transform2T<T>::getRotationAngleFromDirToDir(const Vector2T<T>& from, const Vector2T<T>& to) noexcept
	{
		return to.getAngleBetween(from);
	}

	template <class T>
	void Transform2T<T>::setTransformFromDirToDir(Matrix3T<T>& _out, const Vector2T<T>& from, const Vector2T<T>& to) noexcept
	{
		setRotation(_out, getRotationAngleFromDirToDir(from, to));
	}

	template <class T>
	Matrix3T<T> Transform2T<T>::getTransformMatrixFromDirToDir(const Vector2T<T>& from, const Vector2T<T>& to) noexcept
	{
		Matrix3T<T> ret;
		setTransformFromDirToDir(ret, from, to);
		return ret;
	}

	SLIB_DEFINE_GEOMETRY_TYPE(Transform2)

}
