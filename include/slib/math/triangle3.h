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
		Triangle3T() noexcept = default;

		Triangle3T(const Triangle3T<T>& other) noexcept = default;

		template <class O>
		Triangle3T(const Triangle3T<O>& other) noexcept;

		Triangle3T(const Vector3T<T>& point1, const Vector3T<T>& point2, const Vector3T<T>& point3) noexcept;

	public:
		static Vector3T<T> getNormal(const Vector3T<T>& point1, const Vector3T<T>& point2, const Vector3T<T>& point3) noexcept;

		Vector3T<T> getNormal() const noexcept;

		T getSize() const noexcept;

		PlaneT<T> getPlane() const noexcept;

		void transform(const Matrix4T<T>& mat) noexcept;

		Vector3T<T> projectPoint(const Vector3T<T>& point, T* pDist = sl_null, T* pU = sl_null, T* pV = sl_null) const noexcept;

		sl_bool intersectLine(
			Line3T<T>& line,
			Vector3T<T>* outIntersectPoint = sl_null,
			T* pDist = sl_null,
			T* pU = sl_null,
			T* pV = sl_null,
			sl_bool* pFlagParallel = sl_null,
			sl_bool* pFlagExtendPoint1 = sl_null,
			sl_bool* pFlagExtendPoint2 = sl_null,
			sl_bool* pFlagExtendTriangle = sl_null) const noexcept;

		sl_bool intersectPlane(
			const PlaneT<T>& plane,
			Line3T<T>* outLine = sl_null,
			sl_uint32* outLineNo1 = sl_null,
			sl_uint32* outLineNo2 = sl_null) const noexcept;

		sl_bool intersectTriangle(const Triangle3T<T>& triangle, Line3T<T>* outLine) const noexcept;

	public:
		Triangle3T<T>& operator=(const Triangle3T<T>& other) noexcept = default;

		template <class O>
		Triangle3T<T>& operator=(const Triangle3T<O>& other) noexcept;
	
	};
	
	extern template class Triangle3T<float>;
	extern template class Triangle3T<double>;
	typedef Triangle3T<sl_real> Triangle3;
	typedef Triangle3T<float> Triangle3f;
	typedef Triangle3T<double> Triangle3lf;

}

#include "detail/triangle3.inc"

#endif
