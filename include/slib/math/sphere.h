/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MATH_SPHERE
#define CHECKHEADER_SLIB_MATH_SPHERE

#include "definition.h"

#include "vector3.h"
#include "line3.h"

namespace slib
{
	
	template <class T>
	class SLIB_EXPORT SphereT
	{
	public:
		Vector3T<T> center;
		T radius;

	public:
		SphereT() noexcept = default;

		SphereT(const SphereT<T>& other) noexcept = default;

		template <class O>
		SphereT(const SphereT<O>& other) noexcept;

		SphereT(const Vector3T<T>& center, T radius) noexcept;

		SphereT(T xc, T yc, T zc, T radius) noexcept;

	public:
		sl_bool containsPoint(const Vector3T<T>& point) const noexcept;

		// returns the count of intersected points
		sl_uint32 intersectLine(const Line3T<T>& line, Vector3T<T>* pOut1, Vector3T<T>* pOut2) const noexcept;

	public:
		SphereT<T>& operator=(const SphereT<T>& other) noexcept = default;

		template <class O>
		SphereT<T>& operator=(const SphereT<O>& other) noexcept;
	
	};
	
	extern template class SphereT<float>;
	extern template class SphereT<double>;
	typedef SphereT<sl_real> Sphere;
	typedef SphereT<float> Spheref;
	typedef SphereT<double> Spherelf;

}

#include "detail/sphere.inc"

#endif
