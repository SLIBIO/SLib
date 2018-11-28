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

#include "slib/math/sphere.h"

#include "slib/core/math.h"

namespace slib
{

	template <class T>
	SphereT<T>::SphereT(const Vector3T<T>& _center, T _radius) noexcept
	 : center(_center), radius(_radius)
	 {}

	template <class T>
	SphereT<T>::SphereT(T xc, T yc, T zc, T _radius) noexcept
	 : center(xc, yc, zc), radius(_radius)
	 {}

	template <class T>
	sl_bool SphereT<T>::containsPoint(const Vector3T<T>& point) const noexcept
	{
		T dx = point.x - center.x;
		T dy = point.y - center.y;
		T dz = point.z - center.z;
		return dx * dx + dy * dy + dz * dz <= radius * radius;
	}

	template <class T>
	sl_uint32 SphereT<T>::intersectLine(const Line3T<T>& line, Vector3T<T>* pOut1, Vector3T<T>* pOut2) const noexcept
	{
		const SphereT<T>& sphere = *this;
		Vector3T<T> l = line.getDirection();
		if (Math::isLessThanEpsilon(l.getLength2p())) {
			return 0;
		}
		l.normalize();
		Vector3T<T> o = line.point1;
		Vector3T<T> c = sphere.center;
		T r = sphere.radius;
		Vector3T<T> o_c = o - c;
		T s1 = l.dot(o_c);
		T s = s1 * s1 - o_c.getLength2p() + r * r;
		if (s < 0) {
			return 0;
		}
		s = Math::sqrt(s);
		T d0 = -l.dot(o_c);
		if (pOut1) {
			*pOut1 = o + l * (d0 - s);
		}
		if (pOut2) {
			*pOut2 = o + l * (d0 + s);
		}
		if (Math::isLessThanEpsilon(s)) {
			return 1;
		} else {
			return 2;
		}
	}

	SLIB_DEFINE_GEOMETRY_TYPE(Sphere)

}
