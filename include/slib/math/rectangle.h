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

#ifndef CHECKHEADER_SLIB_MATH_RECTANGLE
#define CHECKHEADER_SLIB_MATH_RECTANGLE

#include "definition.h"

#include "point.h"
#include "size.h"
#include "matrix3.h"

#include "../core/list.h"

namespace slib
{
	
	template <class T, class FT = T>
	class SLIB_EXPORT RectangleT
	{
	public:
		T left;
		T top;
		T right;
		T bottom;

	public:
		SLIB_DEFINE_CLASS_DEFAULT_MEMBERS_INLINE(RectangleT)
		
		SLIB_INLINE RectangleT() noexcept = default;

		template <class O, class FO>
		SLIB_INLINE constexpr RectangleT(const RectangleT<O, FO>& other) noexcept
		 : left((T)(other.left)), top((T)(other.top)), right((T)(other.right)), bottom((T)(other.bottom))
		 {}

		SLIB_INLINE constexpr RectangleT(T _left, T _top, T _right, T _bottom)
		 : left(_left), top(_top), right(_right), bottom(_bottom)
		 {}

		SLIB_INLINE constexpr RectangleT(const PointT<T, FT>& leftTop, const SizeT<T, FT>& rightBottom)
		 : left(leftTop.x), top(leftTop.y), right(rightBottom.x), bottom(rightBottom.y)
		 {}

	public:
		static const RectangleT<T, FT>& zero() noexcept;

		T getWidth() const noexcept;

		T getHeight() const noexcept;

		void setWidth(T width) noexcept;

		void setHeight(T height) noexcept;

		SizeT<T, FT> getSize() const noexcept;

		void setSize(T width, T height) noexcept;

		void setSize(const SizeT<T, FT>& size) noexcept;

		PointT<T, FT> getLocation() const noexcept;

		void setLocation(T _x, T _y) noexcept;

		void setLocation(const PointT<T, FT>& location) noexcept;
		
		void setLocationLeft(T _x) noexcept;
		
		void setLocationTop(T _y) noexcept;

		void setLocationRight(T _x) noexcept;
		
		void setLocationBottom(T _y) noexcept;
		
		void translate(T tx, T ty) noexcept;

		void translate(const PointT<T, FT>& t) noexcept;


		PointT<T, FT> getLeftTop() const noexcept;

		void setLeftTop(T _x, T _y) noexcept;

		void setLeftTop(const PointT<T, FT>& pt) noexcept;

		PointT<T, FT> getLeftBottom() const noexcept;

		void setLeftBottom(T _x, T _y) noexcept;

		void setLeftBottom(const PointT<T, FT>& pt) noexcept;

		PointT<T, FT> getRightTop() const noexcept;

		void setRightTop(T _x, T _y) noexcept;

		void setRightTop(const PointT<T, FT>& pt) noexcept;

		PointT<T, FT> getRightBottom() const noexcept;

		void setRightBottom(T _x, T _y) noexcept;

		void setRightBottom(const PointT<T, FT>& pt) noexcept;


		PointT<T, FT> getCenter() const noexcept;

		void setCenter(T _x, T _y) noexcept;

		void setCenter(const PointT<T, FT>& pt) noexcept;

		void setZero() noexcept;

		sl_bool containsPoint(T x, T y) const noexcept;

		sl_bool containsPoint(const PointT<T, FT>& pt) const noexcept;

		sl_bool containsRectangle(const RectangleT<T, FT>& other) const noexcept;

		sl_bool intersectRectangle(const RectangleT<T, FT>& other, RectangleT<T, FT>* outIntersect = sl_null) const noexcept;

		void setFromPoint(T x, T y) noexcept;

		void setFromPoint(const PointT<T, FT>& pt) noexcept;

		void mergePoint(T x, T y) noexcept;

		void mergePoint(const PointT<T, FT>& pt) noexcept;

		void mergePoints(const PointT<T, FT>* points, sl_size count) noexcept;

		void mergePoints(const List< PointT<T, FT> >& points) noexcept;

		void setFromPoints(const PointT<T, FT>* points, sl_size count) noexcept;

		void setFromPoints(const List< PointT<T, FT> >& points) noexcept;

		void setFromPoints(const PointT<T, FT>& pt1, const PointT<T, FT>& pt2) noexcept;

		void mergeRectangle(const RectangleT<T, FT>& rect) noexcept;

		// 4 points
		void getCornerPoints(PointT<T, FT>* _out) const noexcept;

		void transform(const Matrix3T<FT>& mat) noexcept;

		sl_bool equals(const RectangleT<T, FT>& other) const noexcept;

		sl_bool isAlmostEqual(const RectangleT<T, FT>& other) const noexcept;

		sl_bool isValidSize() noexcept;

		sl_bool fixSizeError() noexcept;

		RectangleT<T, FT> lerp(const RectangleT<T, FT>& target, float factor) const noexcept;
	
	public:
		template <class O, class FO>
		RectangleT<T, FT>& operator=(const RectangleT<O, FO>& other) noexcept;

	private:
		static T _zero[4];
	
	};
	
	extern template class RectangleT<float>;
	extern template class RectangleT<double>;
	extern template class RectangleT<sl_int32, float>;
	extern template class RectangleT<sl_int64, double>;
	typedef RectangleT<sl_real> Rectangle;
	typedef RectangleT<float> Rectanglef;
	typedef RectangleT<double> Rectanglelf;
	typedef RectangleT<sl_int32, float> Rectanglei;
	typedef RectangleT<sl_int64, double> Rectangleli;
	
	template <class T, class FT>
	class Interpolation< RectangleT<T, FT> >
	{
	public:
		static RectangleT<T, FT> interpolate(const RectangleT<T, FT>& a, const RectangleT<T, FT>& b, float factor) noexcept;
	};

}

#include "detail/rectangle.inc"

#endif
