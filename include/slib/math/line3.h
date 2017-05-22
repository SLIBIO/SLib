/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MATH_LINE3
#define CHECKHEADER_SLIB_MATH_LINE3

#include "definition.h"

#include "vector3.h"
#include "matrix4.h"

namespace slib
{
	
	template <class T>
	class SLIB_EXPORT Line3T
	{
	public:
		Vector3T<T> point1;
		Vector3T<T> point2;

	public:
		Line3T() noexcept = default;

		Line3T(const Line3T<T>& other) noexcept = default;

		template <class O>
		Line3T(const Line3T<O>& other) noexcept;

		Line3T(const Vector3T<T>& point1, const Vector3T<T>& point2) noexcept;

		Line3T(T x1, T y1, T z1, T x2, T y2, T z2) noexcept;

	public:
		Vector3T<T> getDirection() const noexcept;

		T getLength2p() const noexcept;

		T getLength() const noexcept;

		Vector3T<T> projectPoint(const Vector3T<T>& point) const noexcept;

		T getDistanceFromPoint(const Vector3T<T>& point) const noexcept;

		T getDistanceFromPointOnInfiniteLine(const Vector3T<T>& point) const noexcept;

		void transform(const Matrix4T<T>& mat) noexcept;

	public:
		Line3T<T>& operator=(const Line3T<T>& other) noexcept = default;

		template <class O>
		Line3T<T>& operator=(const Line3T<O>& other) noexcept;

	};
	
	extern template class Line3T<float>;
	extern template class Line3T<double>;
	typedef Line3T<sl_real> Line3;
	typedef Line3T<float> Line3f;
	typedef Line3T<double> Line3lf;

}

#include "detail/line3.inc"

#endif
