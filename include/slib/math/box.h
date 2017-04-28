/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MATH_BOX
#define CHECKHEADER_SLIB_MATH_BOX

#include "definition.h"

#include "vector3.h"
#include "matrix4.h"

#include "../core/list.h"

namespace slib
{
	
	template <class T>
	class SLIB_EXPORT BoxT
	{
	public:
		T x1, y1, z1;
		T x2, y2, z2;

	public:
		BoxT() = default;

		BoxT(const BoxT<T>& other) = default;

		template <class O>
		BoxT(const BoxT<O>& other);

		BoxT(T x1, T y1, T z1,
			 T x2, T y2, T z2);

	public:
		static const BoxT<T>& zero();

		Vector3T<T> getStart() const;

		void setStart(T x, T y, T z);

		void setStart(const Vector3T<T>& v);

		Vector3T<T> getEnd() const;

		void setEnd(T x, T y, T z);

		void setEnd(const Vector3T<T>& v);

		Vector3T<T> getSize() const;

		Vector3T<T> getCenter() const;

		void setZero();

		sl_bool containsPoint(T x, T y, T z) const;

		sl_bool containsPoint(const Vector3T<T>& pt) const;

		sl_bool containsBox(const BoxT<T>& other) const;

		void setFromPoint(T x, T y, T z);

		void setFromPoint(const Vector3T<T>& pt);

		void mergePoint(T x, T y, T z);

		void mergePoint(const Vector3T<T>& pt);

		void mergePoints(const Vector3T<T>* points, sl_size count);

		void mergePoints(const List< Vector3T<T> >& points);

		void setFromPoints(const Vector3T<T>* points, sl_size count);

		void setFromPoints(const List< Vector3T<T> >& points);

		void setFromPoints(const Vector3T<T>& pt1, const Vector3T<T>& pt2);

		void mergeBox(const BoxT<T>& other);

		// 8 points
		void getCornerPoints(Vector3T<T>* _out) const;

	public:
		BoxT<T>& operator=(const BoxT<T>& other) = default;

		template <class O>
		BoxT<T>& operator=(const BoxT<O>& other);

	private:
		static T _zero[6];
	
	};
	
	extern template class BoxT<float>;
	extern template class BoxT<double>;
	typedef BoxT<sl_real> Box;
	typedef BoxT<float> Boxf;
	typedef BoxT<double> Boxlf;

}

#include "detail/box.inc"

#endif
