/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MATH_LINE
#define CHECKHEADER_SLIB_MATH_LINE

#include "definition.h"

#include "point.h"
#include "matrix3.h"

/*
	ax + by + c = 0
*/

namespace slib
{
	
	template <class T>
	class SLIB_EXPORT LineT
	{
	public:
		T a;
		T b;
		T c;

	public:
		SLIB_INLINE LineT() noexcept = default;

		SLIB_INLINE constexpr LineT(const LineT<T>& other) noexcept
		 : a((T)(other.a)), b((T)(other.b)), c((T)(other.c))
		{}

		template <class O>
		SLIB_INLINE constexpr LineT(const LineT<O>& other) noexcept
		 : a((T)(other.a)), b((T)(other.b)), c((T)(other.c))
		{}

		SLIB_INLINE constexpr LineT(T _a, T _b, T _c) noexcept
		 : a(_a), b(_b), c(_c)
		{}

		LineT(const PointT<T>& point, const Vector2T<T>& dir) noexcept;

	public:
		Vector2T<T> getDirection() const noexcept;

		Vector2T<T> getNormal() const noexcept;

		Vector2T<T> projectOriginOnNormalized() const noexcept;

		Vector2T<T> projectOrigin() const noexcept;

		T getDistanceFromPointOnNormalized(const PointT<T>& pos) const noexcept;

		T getDistanceFromPoint(const PointT<T>& pos) const noexcept;

		Vector2T<T> projectPointOnNormalized(const PointT<T>& pos) const noexcept;

		Vector2T<T> projectPoint(const PointT<T>& pos) const noexcept;

		void setFromPointAndDirection(const PointT<T>& point, const Vector2T<T>& dir) noexcept;

		void setFromPointAndNormal(const PointT<T>& point, const Vector2T<T>& normal) noexcept;

		void normalize() noexcept;

		void transform(const Matrix3T<T>& mat) noexcept;

	public:
		LineT<T>& operator=(const LineT<T>& other) noexcept = default;

		template <class O>
		LineT<T>& operator=(const LineT<O>& other) noexcept;

	};
	
	extern template class LineT<float>;
	extern template class LineT<double>;
	typedef LineT<sl_real> Line;
	typedef LineT<float> Linef;
	typedef LineT<double> Linelf;

}

#include "detail/line.inc"

#endif
