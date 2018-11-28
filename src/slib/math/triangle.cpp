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

#include "slib/math/triangle.h"

namespace slib
{

	template <class T>
	TriangleT<T>::TriangleT(const PointT<T>& _point1, const PointT<T>& _point2, const PointT<T>& _point3) noexcept
	 : point1(_point1), point2(_point2), point3(_point3)
	 {}

	template <class T>
	T TriangleT<T>::getCross(const PointT<T>& _point1, const PointT<T>& _point2, const PointT<T>& _point3) noexcept
	{
		return (_point1.x - _point2.x) * (_point2.y - _point3.y) - (_point2.x - _point3.x) * (_point1.y - _point2.y);
	}

	template <class T>
	T TriangleT<T>::getCross() const noexcept
	{
		return getCross(point1, point2, point3);
	}

	template <class T>
	T TriangleT<T>::getSize() const noexcept
	{
		return getCross(point1, point2, point3) / 2;
	}

	template <class T>
	void TriangleT<T>::transform(Matrix3T<T>& mat) noexcept
	{
		point1 = mat.transformPosition(point1);
		point2 = mat.transformPosition(point2);
		point3 = mat.transformPosition(point3);
	}

	SLIB_DEFINE_GEOMETRY_TYPE(Triangle)

}
