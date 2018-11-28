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
