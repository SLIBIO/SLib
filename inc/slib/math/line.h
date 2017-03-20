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
		LineT() = default;

		constexpr LineT(const LineT<T>& other)
		 : a((T)(other.a)), b((T)(other.b)), c((T)(other.c))
		{}

		template <class O>
		constexpr LineT(const LineT<O>& other)
		 : a((T)(other.a)), b((T)(other.b)), c((T)(other.c))
		{}

		constexpr LineT(T _a, T _b, T _c)
		 : a(_a), b(_b), c(_c)
		{}

		LineT(const PointT<T>& point, const Vector2T<T>& dir);

	public:
		Vector2T<T> getDirection() const;

		Vector2T<T> getNormal() const;

		Vector2T<T> projectOriginOnNormalized() const;

		Vector2T<T> projectOrigin() const;

		T getDistanceFromPointOnNormalized(const PointT<T>& pos) const;

		T getDistanceFromPoint(const PointT<T>& pos) const;

		Vector2T<T> projectPointOnNormalized(const PointT<T>& pos) const;

		Vector2T<T> projectPoint(const PointT<T>& pos) const;

		void setFromPointAndDirection(const PointT<T>& point, const Vector2T<T>& dir);

		void setFromPointAndNormal(const PointT<T>& point, const Vector2T<T>& normal);

		void normalize();

		void transform(const Matrix3T<T>& mat);

	public:
		LineT<T>& operator=(const LineT<T>& other) = default;

		template <class O>
		LineT<T>& operator=(const LineT<O>& other);

	};
	
	extern template class LineT<float>;
	extern template class LineT<double>;
	typedef LineT<sl_real> Line;
	typedef LineT<float> Linef;
	typedef LineT<double> Linelf;

}

#include "detail/line.h"

#endif
