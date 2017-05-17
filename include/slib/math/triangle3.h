/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MATH_TRIANGLE3
#define CHECKHEADER_SLIB_MATH_TRIANGLE3

#include "definition.h"

#include "vector3.h"
#include "line3.h"
#include "plane.h"
#include "matrix4.h"

namespace slib
{
	
	template <class T>
	class SLIB_EXPORT Triangle3T
	{
	public:
		Vector3T<T> point1;
		Vector3T<T> point2;
		Vector3T<T> point3;

	public:
		Triangle3T() = default;

		Triangle3T(const Triangle3T<T>& other) = default;

		template <class O>
		Triangle3T(const Triangle3T<O>& other);

		Triangle3T(const Vector3T<T>& point1, const Vector3T<T>& point2, const Vector3T<T>& point3);

	public:
		static Vector3T<T> getNormal(const Vector3T<T>& point1, const Vector3T<T>& point2, const Vector3T<T>& point3);

		Vector3T<T> getNormal() const;

		T getSize() const;

		PlaneT<T> getPlane() const;

		void transform(const Matrix4T<T>& mat);

		Vector3T<T> projectPoint(const Vector3T<T>& point, T* pDist = sl_null, T* pU = sl_null, T* pV = sl_null) const;

		sl_bool intersectLine(
			Line3T<T>& line,
			Vector3T<T>* outIntersectPoint = sl_null,
			T* pDist = sl_null,
			T* pU = sl_null,
			T* pV = sl_null,
			sl_bool* pFlagParallel = sl_null,
			sl_bool* pFlagExtendPoint1 = sl_null,
			sl_bool* pFlagExtendPoint2 = sl_null,
			sl_bool* pFlagExtendTriangle = sl_null) const;

		sl_bool intersectPlane(
			const PlaneT<T>& plane,
			Line3T<T>* outLine = sl_null,
			sl_uint32* outLineNo1 = sl_null,
			sl_uint32* outLineNo2 = sl_null) const;

		sl_bool intersectTriangle(const Triangle3T<T>& triangle, Line3T<T>* outLine) const;

	public:
		Triangle3T<T>& operator=(const Triangle3T<T>& other) = default;

		template <class O>
		Triangle3T<T>& operator=(const Triangle3T<O>& other);
	
	};
	
	extern template class Triangle3T<float>;
	extern template class Triangle3T<double>;
	typedef Triangle3T<sl_real> Triangle3;
	typedef Triangle3T<float> Triangle3f;
	typedef Triangle3T<double> Triangle3lf;

}

#include "detail/triangle3.inc"

#endif
