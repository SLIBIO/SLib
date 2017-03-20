/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/math/triangle.h"

namespace slib
{

	template <class T>
	TriangleT<T>::TriangleT(const PointT<T>& _point1, const PointT<T>& _point2, const PointT<T>& _point3)
	: point1(_point1), point2(_point2), point3(_point3)
	{
	}

	template <class T>
	T TriangleT<T>::getCross(const PointT<T>& _point1, const PointT<T>& _point2, const PointT<T>& _point3)
	{
		return (_point1.x - _point2.x) * (_point2.y - _point3.y) - (_point2.x - _point3.x) * (_point1.y - _point2.y);
	}

	template <class T>
	T TriangleT<T>::getCross() const
	{
		return getCross(point1, point2, point3);
	}

	template <class T>
	T TriangleT<T>::getSize() const
	{
		return getCross(point1, point2, point3) / 2;
	}

	template <class T>
	void TriangleT<T>::transform(Matrix3T<T>& mat)
	{
		point1 = mat.transformPosition(point1);
		point2 = mat.transformPosition(point2);
		point3 = mat.transformPosition(point3);
	}

	SLIB_DEFINE_GEOMETRY_TYPE(Triangle)

}
