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

#include "slib/math/line_segment.h"

namespace slib
{

	template <class T>
	LineSegmentT<T>::LineSegmentT(const PointT<T>& _point1, const PointT<T>& _point2) noexcept
	 : point1(_point1), point2(_point2)
	{
		point1 = _point1;
		point2 = _point2;
	}

	template <class T>
	LineSegmentT<T>::LineSegmentT(T x1, T y1, T x2, T y2) noexcept
	 : point1(x1, y1), point2(x2, y2)
	{
	}

	template <class T>
	Vector2T<T> LineSegmentT<T>::getDirection() const noexcept
	{
		return (point2 - point1);
	}

	template <class T>
	T LineSegmentT<T>::getLength2p() const noexcept
	{
		return point1.getLength2p(point2);
	}

	template <class T>
	T LineSegmentT<T>::getLength() const noexcept
	{
		return point1.getLength(point2);
	}

	template <class T>
	void LineSegmentT<T>::transform(const Matrix3T<T>& mat) noexcept
	{
		point1 = mat.transformPosition(point1);
		point2 = mat.transformPosition(point2);
	}

	template <class T>
	PointT<T> LineSegmentT<T>::projectPoint(const PointT<T>& point) const noexcept
	{
		Vector2T<T> dir = point2 - point1;
		PointT<T> ret = point1 + (point - point1).dot(dir) * dir;
		return ret;
	}

	template <class T>
	T LineSegmentT<T>::getDistanceFromPoint(const PointT<T>& point) const noexcept
	{
		Vector2T<T> dir = point2 - point1;
		T f = (point - point1).dot(dir);
		Vector2T<T> proj = point1 + f * dir;
		if (f < 0) {
			return point1.getLength(point);
		} else {
			if (f > getLength()) {
				return point2.getLength(point);
			} else {
				return proj.getLength(point);
			}
		}
	}

	template <class T>
	T LineSegmentT<T>::getDistanceFromPointOnInfiniteLine(const PointT<T>& point) const noexcept
	{
		Vector2T<T> dir = point2 - point1;
		T f = (point - point1).dot(dir);
		Vector2T<T> proj = point1 + f * dir;
		return proj.getLength(point);
	}

	SLIB_DEFINE_GEOMETRY_TYPE(LineSegment)

}
