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

#include "slib/math/line.h"

#include "slib/core/math.h"

namespace slib
{

	template <class T>
	LineT<T>::LineT(const PointT<T>& point, const Vector2T<T>& dir) noexcept
	{
		setFromPointAndDirection(point, dir);
	}

	template <class T>
	Vector2T<T> LineT<T>::getDirection() const noexcept
	{
		return {b, -a};
	}

	template <class T>
	Vector2T<T> LineT<T>::getNormal() const noexcept
	{
		return {a, b};
	}

	template <class T>
	Vector2T<T> LineT<T>::projectOriginOnNormalized() const noexcept
	{
		return {-a*c, -b*c};
	}

	template <class T>
	Vector2T<T> LineT<T>::projectOrigin() const noexcept
	{
		T L = a * a + b * b;
		if (L > 0) {
			return {-a*c / L, -b*c / L};
		} else {
			return {0, 0};
		}
	}

	template <class T>
	T LineT<T>::getDistanceFromPointOnNormalized(const PointT<T>& pos) const noexcept
	{
		return a * pos.x + b * pos.y + c;
	}

	template <class T>
	T LineT<T>::getDistanceFromPoint(const PointT<T>& pos) const noexcept
	{
		T L = a * a + b * b;
		if (L > 0) {
			L = Math::sqrt(L);
			return (a * pos.x + b * pos.y + c) / L;
		} else {
			return c;
		}
	}

	template <class T>
	Vector2T<T> LineT<T>::projectPointOnNormalized(const Vector2T<T>& pos) const noexcept
	{
		T D = a * pos.x + b * pos.y + c;
		return {pos.x - D * a, pos.y - D * b};
	}

	template <class T>
	Vector2T<T> LineT<T>::projectPoint(const Vector2T<T>& pos) const noexcept
	{
		T L = a * a + b * b;
		if (L > 0) {
			T D = a * pos.x + b * pos.y + c;
			return {pos.x - D * a / L, pos.y - D * b / L};
		} else {
			return pos;
		}
	}

	template <class T>
	void LineT<T>::setFromPointAndDirection(const PointT<T>& point, const Vector2T<T>& dir) noexcept
	{
		a = dir.y;
		b = -dir.x;
		c = -(point.x * a + point.y * b);
	}

	template <class T>
	void LineT<T>::setFromPointAndNormal(const PointT<T>& point, const Vector2T<T>& normal) noexcept
	{
		a = normal.x;
		b = normal.y;
		c = -point.dot(normal);
	}

	template <class T>
	void LineT<T>::normalize() noexcept
	{
		T l = Math::sqrt(a * a + b * b);
		a /= l;
		b /= l;
		c /= l;
	}

	template <class T>
	void LineT<T>::transform(const Matrix3T<T>& mat) noexcept
	{
		T _a = a * mat.m00 + b * mat.m10;
		T _b = a * mat.m01 + b * mat.m11;
		T L = a * a + b * b;
		if (L > 0) {
			T k = c / L;
			c = (k * _a + mat.m20) * _a + (k * _b + mat.m21) * _b;
			a = _a;
			b = _b;
		} else {
			c = 0;
		}
	}

	SLIB_DEFINE_GEOMETRY_TYPE(Line)

}
