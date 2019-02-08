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

#ifndef CHECKHEADER_SLIB_MATH_TRIANGLE
#define CHECKHEADER_SLIB_MATH_TRIANGLE

#include "definition.h"

#include "point.h"
#include "matrix3.h"

namespace slib
{
	
	template <class T>
	class SLIB_EXPORT TriangleT
	{
	public:
		PointT<T> point1;
		PointT<T> point2;
		PointT<T> point3;

	public:
		SLIB_DEFINE_CLASS_DEFAULT_MEMBERS_INLINE(TriangleT)
		
		TriangleT() noexcept = default;

		template <class O>
		TriangleT(const TriangleT<O>& other) noexcept;

		TriangleT(const PointT<T>& point1, const PointT<T>& point2, const PointT<T>& point3) noexcept;

	public:
		static T getCross(const PointT<T>& point1, const PointT<T>& point2, const PointT<T>& point3) noexcept;

		T getCross() const noexcept;

		T getSize() const noexcept;

		void transform(Matrix3T<T>& mat) noexcept;

	public:
		template <class O>
		TriangleT<T>& operator=(const TriangleT<O>& other) noexcept;
	
	};
	
	extern template class TriangleT<float>;
	extern template class TriangleT<double>;
	typedef TriangleT<sl_real> Triangle;
	typedef TriangleT<float> Trianglef;
	typedef TriangleT<double> Trianglelf;

}

#include "detail/triangle.inc"

#endif
