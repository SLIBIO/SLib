/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
		SLIB_INLINE RectangleT() = default;

		constexpr RectangleT(const RectangleT<T, FT>& other):
		 left(other.left), top(other.top), right(other.right), bottom(other.bottom)
		{}

		template <class O, class FO>
		constexpr RectangleT(const RectangleT<O, FO>& other):
		 left((T)(other.left)), top((T)(other.top)), right((T)(other.right)), bottom((T)(other.bottom))
		{}

		constexpr RectangleT(T _left, T _top, T _right, T _bottom):
		 left(_left), top(_top), right(_right), bottom(_bottom)
		{}

		constexpr RectangleT(const PointT<T, FT>& leftTop, const SizeT<T, FT>& rightBottom):
		 left(leftTop.x), top(leftTop.y), right(rightBottom.x), bottom(rightBottom.y)
		{}

	public:
		static const RectangleT<T, FT>& zero();

		T getWidth() const;

		T getHeight() const;

		void setWidth(T width);

		void setHeight(T height);

		SizeT<T, FT> getSize() const;

		void setSize(T width, T height);

		void setSize(const SizeT<T, FT>& size);

		PointT<T, FT> getLocation() const;

		void setLocation(T _x, T _y);

		void setLocation(const PointT<T, FT>& location);

		void translate(T tx, T ty);

		void translate(const PointT<T, FT>& t);


		PointT<T, FT> getLeftTop() const;

		void setLeftTop(T _x, T _y);

		void setLeftTop(const PointT<T, FT>& pt);

		PointT<T, FT> getLeftBottom() const;

		void setLeftBottom(T _x, T _y);

		void setLeftBottom(const PointT<T, FT>& pt);

		PointT<T, FT> getRightTop() const;

		void setRightTop(T _x, T _y);

		void setRightTop(const PointT<T, FT>& pt);

		PointT<T, FT> getRightBottom() const;

		void setRightBottom(T _x, T _y);

		void setRightBottom(const PointT<T, FT>& pt);


		PointT<T, FT> getCenter() const;

		void setCenter(T _x, T _y);

		void setCenter(const PointT<T, FT>& pt);

		void setZero();

		sl_bool containsPoint(T x, T y) const;

		sl_bool containsPoint(const PointT<T, FT>& pt) const;

		sl_bool containsRectangle(const RectangleT<T, FT>& other) const;

		sl_bool intersectRectangle(const RectangleT<T, FT>& other, RectangleT<T, FT>* outIntersect = sl_null) const;

		void setFromPoint(T x, T y);

		void setFromPoint(const PointT<T, FT>& pt);

		void mergePoint(T x, T y);

		void mergePoint(const PointT<T, FT>& pt);

		void mergePoints(const PointT<T, FT>* points, sl_size count);

		void mergePoints(const List< PointT<T, FT> >& points);

		void setFromPoints(const PointT<T, FT>* points, sl_size count);

		void setFromPoints(const List< PointT<T, FT> >& points);

		void setFromPoints(const PointT<T, FT>& pt1, const PointT<T, FT>& pt2);

		void mergeRectangle(const RectangleT<T, FT>& rect);

		// 4 points
		void getCornerPoints(PointT<T, FT>* _out) const;

		void transform(const Matrix3T<FT>& mat);

		sl_bool equals(const RectangleT<T, FT>& other) const;

		sl_bool isAlmostEqual(const RectangleT<T, FT>& other) const;

		sl_bool isValidSize();

		sl_bool fixSizeError();

		RectangleT<T, FT> lerp(const RectangleT<T, FT>& target, float factor) const;
	
	public:
		RectangleT<T, FT>& operator=(const RectangleT<T, FT>& other) = default;

		template <class O, class FO>
		RectangleT<T, FT>& operator=(const RectangleT<O, FO>& other);

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
		static RectangleT<T, FT> interpolate(const RectangleT<T, FT>& a, const RectangleT<T, FT>& b, float factor);
	};

}

#include "detail/rectangle.inc"

#endif
