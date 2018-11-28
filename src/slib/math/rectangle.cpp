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

#include "slib/math/rectangle.h"
#include "slib/core/math.h"

namespace slib
{

	template <class T, class FT>
	SLIB_ALIGN(8) T RectangleT<T, FT>::_zero[4] = {0, 0, 0, 0};

	template <class T, class FT>
	void RectangleT<T, FT>::setWidth(T width) noexcept
	{
		right = left + width;
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setHeight(T height) noexcept
	{
		bottom = top + height;
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setSize(T width, T height) noexcept
	{
		right = left + width;
		bottom = top + height;
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setSize(const SizeT<T, FT>& size) noexcept
	{
		right = left + size.x;
		bottom = top + size.y;
	}

	template <class T, class FT>
	PointT<T, FT> RectangleT<T, FT>::getLocation() const noexcept
	{
		return {left, top};
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setLocation(T _x, T _y) noexcept
	{
		T w = right - left;
		T h = bottom - top;
		left = _x;
		top = _y;
		right = _x + w;
		bottom = _y + h;
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setLocation(const PointT<T, FT>& location) noexcept
	{
		T w = right - left;
		T h = bottom - top;
		left = location.x;
		top = location.y;
		right = location.x + w;
		bottom = location.y + h;
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setLocationLeft(T _x) noexcept
	{
		T w = right - left;
		left = _x;
		right = _x + w;
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setLocationTop(T _y) noexcept
	{
		T h = bottom - top;
		top = _y;
		bottom = _y + h;
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setLocationRight(T _x) noexcept
	{
		T w = right - left;
		left = _x - w;
		right = _x;
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setLocationBottom(T _y) noexcept
	{
		T h = bottom - top;
		top = _y - h;
		bottom = _y;
	}

	template <class T, class FT>
	void RectangleT<T, FT>::translate(T tx, T ty) noexcept
	{
		left += tx;
		top += ty;
		right += tx;
		bottom += ty;
	}

	template <class T, class FT>
	void RectangleT<T, FT>::translate(const PointT<T, FT>& t) noexcept
	{
		left += t.x;
		top += t.y;
		right += t.x;
		bottom += t.y;
	}

	template <class T, class FT>
	PointT<T, FT> RectangleT<T, FT>::getLeftTop() const noexcept
	{
		return {left, top};
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setLeftTop(T _x, T _y) noexcept
	{
		left = _x;
		top = _y;
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setLeftTop(const PointT<T, FT>& pt) noexcept
	{
		left = pt.x;
		top = pt.y;
	}

	template <class T, class FT>
	PointT<T, FT> RectangleT<T, FT>::getLeftBottom() const noexcept
	{
		return {left, bottom};
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setLeftBottom(T _x, T _y) noexcept
	{
		left = _x;
		bottom = _y;
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setLeftBottom(const PointT<T, FT>& pt) noexcept
	{
		left = pt.x;
		bottom = pt.y;
	}

	template <class T, class FT>
	PointT<T, FT> RectangleT<T, FT>::getRightTop() const noexcept
	{
		return {right, top};
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setRightTop(T _x, T _y) noexcept
	{
		right = _x;
		top = _y;
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setRightTop(const PointT<T, FT>& pt) noexcept
	{
		right = pt.x;
		top = pt.y;
	}

	template <class T, class FT>
	PointT<T, FT> RectangleT<T, FT>::getRightBottom() const noexcept
	{
		return {right, bottom};
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setRightBottom(T _x, T _y) noexcept
	{
		right = _x;
		bottom = _y;
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setRightBottom(const PointT<T, FT>& pt) noexcept
	{
		right = pt.x;
		bottom = pt.y;
	}

	template <class T, class FT>
	PointT<T, FT> RectangleT<T, FT>::getCenter() const noexcept
	{
		return {(left + right) / 2, (top + bottom) / 2};
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setCenter(T _x, T _y) noexcept
	{
		T w = (right - left) / 2;
		T h = (bottom - top) / 2;
		left = _x - w;
		top = _y - h;
		right = _x + w;
		bottom = _y + h;
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setCenter(const PointT<T, FT>& pt) noexcept
	{
		T w = (right - left) / 2;
		T h = (bottom - top) / 2;
		left = pt.x - w;
		top = pt.y - h;
		right = pt.x + w;
		bottom = pt.y + h;
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setZero() noexcept
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}

	template <class T, class FT>
	sl_bool RectangleT<T, FT>::containsPoint(T x, T y) const noexcept
	{
		return x >= left && x <= right && y >= top && y <= bottom;
	}

	template <class T, class FT>
	sl_bool RectangleT<T, FT>::containsPoint(const PointT<T, FT>& pt) const noexcept
	{
		return pt.x >= left && pt.x <= right && pt.y >= top && pt.y <= bottom;
	}

	template <class T, class FT>
	sl_bool RectangleT<T, FT>::containsRectangle(const RectangleT<T, FT>& other) const noexcept
	{
		return left <= other.left && right >= other.right && top <= other.top && bottom >= other.bottom;
	}

	template <class T, class FT>
	sl_bool RectangleT<T, FT>::intersectRectangle(const RectangleT<T, FT>& other, RectangleT<T, FT>* outIntersect) const noexcept
	{
		if (outIntersect) {
			T _left = SLIB_MAX(left, other.left);
			T _right = SLIB_MIN(right, other.right);
			T _top = SLIB_MAX(top, other.top);
			T _bottom = SLIB_MIN(bottom, other.bottom);
			outIntersect->left = _left;
			outIntersect->right = _right;
			outIntersect->top = _top;
			outIntersect->bottom = _bottom;
			return _left <= _right && _top <= _bottom;
		} else {
			return left <= other.right && right >= other.left && top <= other.bottom && bottom >= other.top;
		}
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setFromPoint(T x, T y) noexcept
	{
		left = right = x;
		top = bottom = y;
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setFromPoint(const PointT<T, FT>& pt) noexcept
	{
		left = right = pt.x;
		top = bottom = pt.y;
	}

	template <class T, class FT>
	void RectangleT<T, FT>::mergePoint(T x, T y) noexcept
	{
		if (left > x) {
			left = x;
		}
		if (right < x) {
			right = x;
		}
		if (top > y) {
			top = y;
		}
		if (bottom < y) {
			bottom = y;
		}
	}

	template <class T, class FT>
	void RectangleT<T, FT>::mergePoint(const PointT<T, FT>& pt) noexcept
	{
		if (left > pt.x) {
			left = pt.x;
		}
		if (right < pt.x) {
			right = pt.x;
		}
		if (top > pt.y) {
			top = pt.y;
		}
		if (bottom < pt.y) {
			bottom = pt.y;
		}
	}

	template <class T, class FT>
	void RectangleT<T, FT>::mergePoints(const PointT<T, FT>* points, sl_size count) noexcept
	{
		for (sl_size i = 0; i < count; i++) {
			const PointT<T, FT>& v = points[i];
			if (left > v.x) {
				left = v.x;
			}
			if (right < v.x) {
				right = v.x;
			}
			if (top > v.y) {
				top = v.y;
			}
			if (bottom < v.y) {
				bottom = v.y;
			}
		}
	}

	template <class T, class FT>
	void RectangleT<T, FT>::mergePoints(const List< PointT<T, FT> >& points) noexcept
	{
		ListLocker< PointT<T, FT> > list(points);
		mergePoints(list.data, list.count);
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setFromPoints(const PointT<T, FT>* points, sl_size count) noexcept
	{
		if (count > 0) {
			setFromPoint(points[0]);
			if (count > 1) {
				mergePoints(points + 1, count - 1);
			}
		}
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setFromPoints(const List< PointT<T, FT> >& points) noexcept
	{
		ListLocker< PointT<T, FT> > list(points);
		setFromPoints(list.data, list.count);
	}

	template <class T, class FT>
	void RectangleT<T, FT>::setFromPoints(const PointT<T, FT>& pt1, const PointT<T, FT>& pt2) noexcept
	{
		setFromPoint(pt1);
		mergePoint(pt2);
	}

	template <class T, class FT>
	void RectangleT<T, FT>::mergeRectangle(const RectangleT<T, FT>& rect) noexcept
	{
		if (left > rect.left) {
			left = rect.left;
		}
		if (right < rect.right) {
			right = rect.right;
		}
		if (top > rect.top) {
			top = rect.top;
		}
		if (bottom < rect.bottom) {
			bottom = rect.bottom;
		}
	}

	template <class T, class FT>
	void RectangleT<T, FT>::getCornerPoints(PointT<T, FT>* _out) const noexcept
	{
		_out[0].x = left; _out[0].y = top;
		_out[1].x = right; _out[1].y = top;
		_out[2].x = left; _out[2].y = bottom;
		_out[3].x = right; _out[3].y = bottom;
	}

	template <class T, class FT>
	void RectangleT<T, FT>::transform(const Matrix3T<FT>& mat) noexcept
	{
		Vector2T<T, FT> pts[4];
		getCornerPoints(pts);
		for (int i = 0; i < 4; i++) {
			pts[i] = mat.transformPosition(pts[i]);
		}
		setFromPoints(pts, 4);
	}

	template <class T, class FT>
	sl_bool RectangleT<T, FT>::equals(const RectangleT<T, FT>& other) const noexcept
	{
		return left == other.left && top == other.top && right == other.right && bottom == other.bottom;
	}

	template <class T, class FT>
	sl_bool RectangleT<T, FT>::isAlmostEqual(const RectangleT<T, FT>& other) const noexcept
	{
		return Math::isAlmostZero(left - other.left) &&
			Math::isAlmostZero(top - other.top) &&
			Math::isAlmostZero(right - other.right) &&
			Math::isAlmostZero(bottom - other.bottom);
	}

	template <class T, class FT>
	sl_bool RectangleT<T, FT>::isValidSize() noexcept
	{
		return right > left && bottom > top;
	}

	template <class T, class FT>
	sl_bool RectangleT<T, FT>::fixSizeError() noexcept
	{
		sl_bool flagFixed = sl_false;
		if (right < left) {
			flagFixed = sl_true;
			right = left;
		}
		if (bottom < top) {
			flagFixed = sl_true;
			bottom = top;
		}
		return flagFixed;
	}

	template <class T, class FT>
	RectangleT<T, FT> RectangleT<T, FT>::lerp(const RectangleT<T, FT>& target, float factor) const noexcept
	{
		return {(T)SLIB_LERP(left, target.left, factor), (T)SLIB_LERP(top, target.top, factor), (T)SLIB_LERP(right, target.right, factor), (T)SLIB_LERP(bottom, target.bottom, factor)};
	}

	SLIB_DEFINE_GEOMETRY_TYPE_EX(Rectangle)

}
