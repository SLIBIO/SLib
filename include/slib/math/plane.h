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

#ifndef CHECKHEADER_SLIB_MATH_PLANE
#define CHECKHEADER_SLIB_MATH_PLANE

#include "definition.h"

#include "vector3.h"
#include "line3.h"
#include "matrix4.h"

/*
	ax + by + cz + d = 0
 */

namespace slib
{

	template <class T>
	class SLIB_EXPORT PlaneT
	{
	public:
		T a;
		T b;
		T c;
		T d;

	public:
		SLIB_DEFINE_CLASS_DEFAULT_MEMBERS_INLINE(PlaneT)
		
		SLIB_INLINE PlaneT() noexcept = default;

		template <class O>
		SLIB_INLINE constexpr PlaneT(const PlaneT<O>& other)
		 : a((T)(other.a)), b((T)(other.b)), c((T)(other.c)), d((T)(other.d))
		 {}

		SLIB_INLINE constexpr PlaneT(T _a, T _b, T _c, T _d)
		 : a(_a), b(_b), c(_c), d(_d)
		 {}

		PlaneT(const Vector3T<T>& point, const Vector3T<T>& normal) noexcept;

	public:
		Vector3T<T> getNormal() const noexcept;

		Vector3T<T> projectOriginOnNormalized() const noexcept;

		Vector3T<T> projectOrigin() const noexcept;

		T getDistanceFromPointOnNormalized(const Vector3T<T>& pos) const noexcept;

		T getDistanceFromPoint(const Vector3T<T>& pos) const noexcept;

		Vector3T<T> projectPointOnNormalized(const Vector3T<T>& pos) const noexcept;

		Vector3T<T> projectPoint(const Vector3T<T>& pos) const noexcept;

		void setFromPointAndNormal(const Vector3T<T>& point, const Vector3T<T>& normal) noexcept;

		void normalize() noexcept;

		void transform(const Matrix4T<T>& mat) noexcept;

		// return sl_true when the plane intersects to the line segment
		sl_bool intersectLine(
			const Line3T<T>& line,
			Vector3T<T>* outIntersectPoint = sl_null,
			sl_bool* pFlagParallel = sl_null,
			sl_bool* pFlagExtendPoint1 = sl_null,
			sl_bool* pFlagExtendPoint2 = sl_null) const noexcept;

		sl_bool intersectPlane(const PlaneT<T>& plane, Line3T<T>* outIntersectLine = sl_null, sl_bool* pFlagParallel = sl_null) const noexcept;

	public:
		template <class O>
		PlaneT<T>& operator=(const PlaneT<O>& other) noexcept;
	
	};
	
	extern template class PlaneT<float>;
	extern template class PlaneT<double>;
	typedef PlaneT<sl_real> Plane;
	typedef PlaneT<float> Planef;
	typedef PlaneT<double> Planelf;

}

#include "detail/plane.inc"

#endif
