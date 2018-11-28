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

#ifndef CHECKHEADER_SLIB_MATH_BOX
#define CHECKHEADER_SLIB_MATH_BOX

#include "definition.h"

#include "vector3.h"
#include "matrix4.h"

#include "../core/list.h"

namespace slib
{
	
	template <class T>
	class SLIB_EXPORT BoxT
	{
	public:
		T x1, y1, z1;
		T x2, y2, z2;

	public:
		BoxT() noexcept = default;

		BoxT(const BoxT<T>& other) noexcept = default;

		template <class O>
		BoxT(const BoxT<O>& other) noexcept;

		BoxT(T x1, T y1, T z1,
			 T x2, T y2, T z2) noexcept;

	public:
		static const BoxT<T>& zero() noexcept;

		Vector3T<T> getStart() const noexcept;

		void setStart(T x, T y, T z) noexcept;

		void setStart(const Vector3T<T>& v) noexcept;

		Vector3T<T> getEnd() const noexcept;

		void setEnd(T x, T y, T z) noexcept;

		void setEnd(const Vector3T<T>& v) noexcept;

		Vector3T<T> getSize() const noexcept;

		Vector3T<T> getCenter() const noexcept;

		void setZero() noexcept;

		sl_bool containsPoint(T x, T y, T z) const noexcept;

		sl_bool containsPoint(const Vector3T<T>& pt) const noexcept;

		sl_bool containsBox(const BoxT<T>& other) const noexcept;

		void setFromPoint(T x, T y, T z) noexcept;

		void setFromPoint(const Vector3T<T>& pt) noexcept;

		void mergePoint(T x, T y, T z) noexcept;

		void mergePoint(const Vector3T<T>& pt) noexcept;

		void mergePoints(const Vector3T<T>* points, sl_size count) noexcept;

		void mergePoints(const List< Vector3T<T> >& points) noexcept;

		void setFromPoints(const Vector3T<T>* points, sl_size count) noexcept;

		void setFromPoints(const List< Vector3T<T> >& points) noexcept;

		void setFromPoints(const Vector3T<T>& pt1, const Vector3T<T>& pt2) noexcept;

		void mergeBox(const BoxT<T>& other) noexcept;

		// 8 points
		void getCornerPoints(Vector3T<T>* _out) const noexcept;

	public:
		BoxT<T>& operator=(const BoxT<T>& other) noexcept = default;

		template <class O>
		BoxT<T>& operator=(const BoxT<O>& other) noexcept;

	private:
		static T _zero[6];
	
	};
	
	extern template class BoxT<float>;
	extern template class BoxT<double>;
	typedef BoxT<sl_real> Box;
	typedef BoxT<float> Boxf;
	typedef BoxT<double> Boxlf;

}

#include "detail/box.inc"

#endif
