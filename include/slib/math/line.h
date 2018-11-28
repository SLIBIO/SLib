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

#ifndef CHECKHEADER_SLIB_MATH_LINE
#define CHECKHEADER_SLIB_MATH_LINE

#include "definition.h"

#include "point.h"
#include "matrix3.h"

/*
	ax + by + c = 0
*/

namespace slib
{
	
	template <class T>
	class SLIB_EXPORT LineT
	{
	public:
		T a;
		T b;
		T c;

	public:
		SLIB_INLINE LineT() noexcept = default;

		SLIB_INLINE constexpr LineT(const LineT<T>& other) noexcept
		 : a((T)(other.a)), b((T)(other.b)), c((T)(other.c))
		{}

		template <class O>
		SLIB_INLINE constexpr LineT(const LineT<O>& other) noexcept
		 : a((T)(other.a)), b((T)(other.b)), c((T)(other.c))
		{}

		SLIB_INLINE constexpr LineT(T _a, T _b, T _c) noexcept
		 : a(_a), b(_b), c(_c)
		{}

		LineT(const PointT<T>& point, const Vector2T<T>& dir) noexcept;

	public:
		Vector2T<T> getDirection() const noexcept;

		Vector2T<T> getNormal() const noexcept;

		Vector2T<T> projectOriginOnNormalized() const noexcept;

		Vector2T<T> projectOrigin() const noexcept;

		T getDistanceFromPointOnNormalized(const PointT<T>& pos) const noexcept;

		T getDistanceFromPoint(const PointT<T>& pos) const noexcept;

		Vector2T<T> projectPointOnNormalized(const PointT<T>& pos) const noexcept;

		Vector2T<T> projectPoint(const PointT<T>& pos) const noexcept;

		void setFromPointAndDirection(const PointT<T>& point, const Vector2T<T>& dir) noexcept;

		void setFromPointAndNormal(const PointT<T>& point, const Vector2T<T>& normal) noexcept;

		void normalize() noexcept;

		void transform(const Matrix3T<T>& mat) noexcept;

	public:
		LineT<T>& operator=(const LineT<T>& other) noexcept = default;

		template <class O>
		LineT<T>& operator=(const LineT<O>& other) noexcept;

	};
	
	extern template class LineT<float>;
	extern template class LineT<double>;
	typedef LineT<sl_real> Line;
	typedef LineT<float> Linef;
	typedef LineT<double> Linelf;

}

#include "detail/line.inc"

#endif
