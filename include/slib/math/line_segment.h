/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
