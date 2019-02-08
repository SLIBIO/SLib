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

#ifndef CHECKHEADER_SLIB_MATH_LINE3
#define CHECKHEADER_SLIB_MATH_LINE3

#include "definition.h"

#include "vector3.h"
#include "matrix4.h"

namespace slib
{
	
	template <class T>
	class SLIB_EXPORT Line3T
	{
	public:
		Vector3T<T> point1;
		Vector3T<T> point2;

	public:
		SLIB_DEFINE_CLASS_DEFAULT_MEMBERS_INLINE(Line3T)
		
		Line3T() noexcept = default;

		template <class O>
		Line3T(const Line3T<O>& other) noexcept;

		Line3T(const Vector3T<T>& point1, const Vector3T<T>& point2) noexcept;

		Line3T(T x1, T y1, T z1, T x2, T y2, T z2) noexcept;

	public:
		Vector3T<T> getDirection() const noexcept;

		T getLength2p() const noexcept;

		T getLength() const noexcept;

		Vector3T<T> projectPoint(const Vector3T<T>& point) const noexcept;

		T getDistanceFromPoint(const Vector3T<T>& point) const noexcept;

		T getDistanceFromPointOnInfiniteLine(const Vector3T<T>& point) const noexcept;

		void transform(const Matrix4T<T>& mat) noexcept;

	public:
		template <class O>
		Line3T<T>& operator=(const Line3T<O>& other) noexcept;

	};
	
	extern template class Line3T<float>;
	extern template class Line3T<double>;
	typedef Line3T<sl_real> Line3;
	typedef Line3T<float> Line3f;
	typedef Line3T<double> Line3lf;

}

#include "detail/line3.inc"

#endif
