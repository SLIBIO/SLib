/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/math/line3.h"

namespace slib
{

	template <class T>
	Line3T<T>::Line3T(const Vector3T<T>& _point1, const Vector3T<T>& _point2)
	: point1(_point1), point2(_point2)
	{
	}

	template <class T>
	Line3T<T>::Line3T(T x1, T y1, T z1, T x2, T y2, T z2)
	: point1(x1, y1, z1), point2(x2, y2, z2)
	{
	}

	template <class T>
	Vector3T<T> Line3T<T>::getDirection() const
	{
		return point2 - point1;
	}

	template <class T>
	T Line3T<T>::getLength2p() const
	{
		return point1.getLength2p(point2);
	}

	template <class T>
	T Line3T<T>::getLength() const
	{
		return point1.getLength(point2);
	}

	template <class T>
	Vector3T<T> Line3T<T>::projectPoint(const Vector3T<T>& point) const
	{
		Vector3T<T> dir = point2 - point1;
		Vector3T<T> ret = point1 + (point - point1).dot(dir) * dir;
		return ret;
	}

	template <class T>
	T Line3T<T>::getDistanceFromPoint(const Vector3T<T>& point) const
	{
		Vector3T<T> dir = point2 - point1;
		T f = (point - point1).dot(dir);
		Vector3T<T> proj = point1 + f * dir;
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
	T Line3T<T>::getDistanceFromPointOnInfiniteLine(const Vector3T<T>& point) const
	{
		Vector3T<T> dir = point2 - point1;
		T f = (point - point1).dot(dir);
		Vector3T<T> proj = point1 + f * dir;
		return proj.getLength(point);
	}

	template <class T>
	void Line3T<T>::transform(const Matrix4T<T>& mat)
	{
		point1 = mat.transformPosition(point1);
		point2 = mat.transformPosition(point2);
	}

	SLIB_DEFINE_GEOMETRY_TYPE(Line3)

}
