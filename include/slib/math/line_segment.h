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

#ifndef CHECKHEADER_SLIB_MATH_LINE_SEGMENT
#define CHECKHEADER_SLIB_MATH_LINE_SEGMENT

#include "definition.h"

#include "point.h"
#include "matrix3.h"

namespace slib
{
	
	template <class T>
	class SLIB_EXPORT LineSegmentT
	{
	public:
		PointT<T> point1;
		PointT<T> point2;

	public:
		LineSegmentT() noexcept = default;

		LineSegmentT(const LineSegmentT<T>& other) noexcept = default;

		template <class O>
		LineSegmentT(const LineSegmentT<O>& other) noexcept;

		LineSegmentT(const PointT<T>& point1, const PointT<T>& point2) noexcept;

		LineSegmentT(T x1, T y1, T x2, T y2) noexcept;

	public:
		Vector2T<T> getDirection() const noexcept;

		T getLength2p() const noexcept;

		T getLength() const noexcept;

		void transform(const Matrix3T<T>& mat) noexcept;

		PointT<T> projectPoint(const PointT<T>& point) const noexcept;

		T getDistanceFromPoint(const PointT<T>& point) const noexcept;

		T getDistanceFromPointOnInfiniteLine(const PointT<T>& point) const noexcept;

	public:
		LineSegmentT<T>& operator=(const LineSegmentT<T>& other) noexcept = default;

		template <class O>
		LineSegmentT<T>& operator=(const LineSegmentT<O>& other) noexcept;

	};
	
	extern template class LineSegmentT<float>;
	extern template class LineSegmentT<double>;
	typedef LineSegmentT<sl_real> LineSegment;
	typedef LineSegmentT<float> LineSegmentf;
	typedef LineSegmentT<double> LineSegmentlf;

}

#include "detail/line_segment.inc"

#endif
