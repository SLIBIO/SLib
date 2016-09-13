#include "../../../inc/slib/math/rectangle.h"
#include "../../../inc/slib/core/math.h"

SLIB_MATH_NAMESPACE_BEGIN

template <class T, class FT>
T RectangleT<T, FT>::_zero[4] = {0, 0, 0, 0};

template <class T, class FT>
void RectangleT<T, FT>::setWidth(T width)
{
	right = left + width;
}

template <class T, class FT>
void RectangleT<T, FT>::setHeight(T height)
{
	bottom = top + height;
}

template <class T, class FT>
void RectangleT<T, FT>::setSize(T width, T height)
{
	right = left + width;
	bottom = top + height;
}

template <class T, class FT>
void RectangleT<T, FT>::setSize(const SizeT<T, FT>& size)
{
	right = left + size.x;
	bottom = top + size.y;
}

template <class T, class FT>
PointT<T, FT> RectangleT<T, FT>::getLocation() const
{
	return {left, top};
}

template <class T, class FT>
void RectangleT<T, FT>::setLocation(T _x, T _y)
{
	T w = right - left;
	T h = bottom - top;
	left = _x;
	top = _y;
	right = left + w;
	bottom = top + h;
}

template <class T, class FT>
void RectangleT<T, FT>::setLocation(const PointT<T, FT>& location)
{
	T w = right - left;
	T h = bottom - top;
	left = location.x;
	top = location.y;
	right = left + w;
	bottom = top + h;
}

template <class T, class FT>
void RectangleT<T, FT>::translate(T tx, T ty)
{
	left += tx;
	top += ty;
	right += tx;
	bottom += ty;
}

template <class T, class FT>
void RectangleT<T, FT>::translate(const PointT<T, FT>& t)
{
	left += t.x;
	top += t.y;
	right += t.x;
	bottom += t.y;
}

template <class T, class FT>
PointT<T, FT> RectangleT<T, FT>::getLeftTop() const
{
	return {left, top};
}

template <class T, class FT>
void RectangleT<T, FT>::setLeftTop(T _x, T _y)
{
	left = _x;
	top = _y;
}

template <class T, class FT>
void RectangleT<T, FT>::setLeftTop(const PointT<T, FT>& pt)
{
	left = pt.x;
	top = pt.y;
}

template <class T, class FT>
PointT<T, FT> RectangleT<T, FT>::getLeftBottom() const
{
	return {left, bottom};
}

template <class T, class FT>
void RectangleT<T, FT>::setLeftBottom(T _x, T _y)
{
	left = _x;
	bottom = _y;
}

template <class T, class FT>
void RectangleT<T, FT>::setLeftBottom(const PointT<T, FT>& pt)
{
	left = pt.x;
	bottom = pt.y;
}

template <class T, class FT>
PointT<T, FT> RectangleT<T, FT>::getRightTop() const
{
	return {right, top};
}

template <class T, class FT>
void RectangleT<T, FT>::setRightTop(T _x, T _y)
{
	right = _x;
	top = _y;
}

template <class T, class FT>
void RectangleT<T, FT>::setRightTop(const PointT<T, FT>& pt)
{
	right = pt.x;
	top = pt.y;
}

template <class T, class FT>
PointT<T, FT> RectangleT<T, FT>::getRightBottom() const
{
	return {right, bottom};
}

template <class T, class FT>
void RectangleT<T, FT>::setRightBottom(T _x, T _y)
{
	right = _x;
	bottom = _y;
}

template <class T, class FT>
void RectangleT<T, FT>::setRightBottom(const PointT<T, FT>& pt)
{
	right = pt.x;
	bottom = pt.y;
}

template <class T, class FT>
PointT<T, FT> RectangleT<T, FT>::getCenter() const
{
	return {(left + right) / 2, (top + bottom) / 2};
}

template <class T, class FT>
void RectangleT<T, FT>::setCenter(T _x, T _y)
{
	T w = (right - left) / 2;
	T h = (bottom - top) / 2;
	left = _x - w;
	top = _y - h;
	right = _x + w;
	bottom = _y + h;
}

template <class T, class FT>
void RectangleT<T, FT>::setCenter(const PointT<T, FT>& pt)
{
	T w = (right - left) / 2;
	T h = (bottom - top) / 2;
	left = pt.x - w;
	top = pt.y - h;
	right = pt.x + w;
	bottom = pt.y + h;
}

template <class T, class FT>
void RectangleT<T, FT>::setZero()
{
	left = 0;
	top = 0;
	right = 0;
	bottom = 0;
}

template <class T, class FT>
sl_bool RectangleT<T, FT>::containsPoint(T x, T y) const
{
	return x >= left && x <= right && y >= top && y <= bottom;
}

template <class T, class FT>
sl_bool RectangleT<T, FT>::containsPoint(const PointT<T, FT>& pt) const
{
	return pt.x >= left && pt.x <= right && pt.y >= top && pt.y <= bottom;
}

template <class T, class FT>
sl_bool RectangleT<T, FT>::containsRectangle(const RectangleT<T, FT>& other) const
{
	return left <= other.right && right >= other.left && top <= other.bottom && bottom >= other.top;
}

template <class T, class FT>
sl_bool RectangleT<T, FT>::intersectRectangle(const RectangleT<T, FT>& other, RectangleT<T, FT>* outIntersect) const
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
		T _left = SLIB_MAX(left, other.left);
		T _right = SLIB_MIN(right, other.right);
		if (_left > _right) {
			return sl_false;
		}
		T _top = SLIB_MAX(top, other.top);
		T _bottom = SLIB_MIN(bottom, other.bottom);
		if (_top > _bottom) {
			return sl_false;
		}
		return sl_true;
	}
}

template <class T, class FT>
void RectangleT<T, FT>::setFromPoint(T x, T y)
{
	left = right = x;
	top = bottom = y;
}

template <class T, class FT>
void RectangleT<T, FT>::setFromPoint(const PointT<T, FT>& pt)
{
	left = right = pt.x;
	top = bottom = pt.y;
}

template <class T, class FT>
void RectangleT<T, FT>::mergePoint(T x, T y)
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
void RectangleT<T, FT>::mergePoint(const PointT<T, FT>& pt)
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
void RectangleT<T, FT>::mergePoints(const PointT<T, FT>* points, sl_size count)
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
void RectangleT<T, FT>::mergePoints(const List< PointT<T, FT> >& points)
{
	ListLocker< PointT<T, FT> > list(points);
	mergePoints(list.data, list.count);
}

template <class T, class FT>
void RectangleT<T, FT>::setFromPoints(const PointT<T, FT>* points, sl_size count)
{
	if (count > 0) {
		setFromPoint(points[0]);
		if (count > 1) {
			mergePoints(points + 1, count - 1);
		}
	}
}

template <class T, class FT>
void RectangleT<T, FT>::setFromPoints(const List< PointT<T, FT> >& points)
{
	ListLocker< PointT<T, FT> > list(points);
	setFromPoints(list.data, list.count);
}

template <class T, class FT>
void RectangleT<T, FT>::setFromPoints(const PointT<T, FT>& pt1, const PointT<T, FT>& pt2)
{
	setFromPoint(pt1);
	mergePoint(pt2);
}

template <class T, class FT>
void RectangleT<T, FT>::mergeRectangle(const RectangleT<T, FT>& rect)
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
void RectangleT<T, FT>::getCornerPoints(PointT<T, FT>* _out) const
{
	_out[0].x = left; _out[0].y = top;
	_out[1].x = right; _out[1].y = top;
	_out[2].x = left; _out[2].y = bottom;
	_out[3].x = right; _out[3].y = bottom;
}

template <class T, class FT>
sl_bool RectangleT<T, FT>::equals(const RectangleT<T, FT>& other) const
{
	return left == other.left && top == other.top && right == other.right && bottom == other.bottom;
}

template <class T, class FT>
sl_bool RectangleT<T, FT>::isAlmostEqual(const RectangleT<T, FT>& other) const
{
	return Math::isAlmostZero(left - other.left) &&
		Math::isAlmostZero(top - other.top) &&
		Math::isAlmostZero(right - other.right) &&
		Math::isAlmostZero(bottom - other.bottom);
}

template <class T, class FT>
sl_bool RectangleT<T, FT>::fixSizeError()
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

SLIB_DEFINE_GEOMETRY_TYPE_EX(Rectangle)

SLIB_MATH_NAMESPACE_END
