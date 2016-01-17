#ifndef CHECKHEADER_SLIB_MATH_RECTANGLE
#define CHECKHEADER_SLIB_MATH_RECTANGLE

#include "definition.h"

#include "point.h"
#include "size.h"

#include "../core/list.h"

SLIB_MATH_NAMESPACE_BEGIN

template <class T>
class SLIB_EXPORT RectangleT
{
public:
	T left;
	T top;
	T right;
	T bottom;
	
public:
	RectangleT()
	{
	}
	
	template <class O>
	SLIB_INLINE RectangleT(const RectangleT<O>& other)
	{
		left = (T)(other.left);
		top = (T)(other.top);
		right = (T)(other.right);
		bottom = (T)(other.bottom);
	}
	
	SLIB_INLINE RectangleT(T left, T top, T right, T bottom)
	{
		this->left = left;
		this->top = top;
		this->right = right;
		this->bottom = bottom;
	}
	
	SLIB_INLINE RectangleT(const PointT<T>& pt, const SizeT<T>& size)
	{
		left = pt.x;
		top = pt.y;
		right = pt.x + size.x;
		bottom = pt.y + size.y;
	}
	
public:
	template <class O>
	SLIB_INLINE RectangleT<T>& operator=(const RectangleT<O>& other)
	{
		left = (T)(other.left);
		top = (T)(other.top);
		right = (T)(other.right);
		bottom = (T)(other.bottom);
		return *this;
	}
	
private:
	static T _zero[4];
	
public:
	SLIB_INLINE static const RectangleT<T>& zero()
	{
		return *((RectangleT<T>*)((void*)_zero));
	}
	
	SLIB_INLINE T getWidth() const
	{
		return right - left;
	}
	
	SLIB_INLINE T getHeight() const
	{
		return bottom - top;
	}
	
	SLIB_INLINE void setWidth(T width)
	{
		right = left + width;
	}
	
	SLIB_INLINE void setHeight(T height)
	{
		bottom = top + height;
	}
	
	SLIB_INLINE SizeT<T> getSize() const
	{
		return SizeT<T>(right - left, bottom - top);
	}
	
	SLIB_INLINE void setSize(T width, T height)
	{
		right = left + width;
		bottom = top + height;
	}
	
	SLIB_INLINE void setSize(const SizeT<T>& size)
	{
		setSize(size.x, size.y);
	}
	
	SLIB_INLINE PointT<T> getLocation() const
	{
		return PointT<T>(left, top);
	}
	
	SLIB_INLINE void setLocation(T _x, T _y)
	{
		T w = right - left;
		T h = bottom - top;
		left = _x;
		top = _y;
		right = left + w;
		bottom = top + h;
	}
	
	SLIB_INLINE void setLocation(const PointT<T>& location)
	{
		setLocation(location.x, location.y);
	}
	
	SLIB_INLINE void translate(T tx, T ty)
	{
		left += tx;
		top += ty;
		right += tx;
		bottom += ty;
	}
	
	SLIB_INLINE void translate(const PointT<T>& t)
	{
		translate(t.x, t.y);
	}
	
	SLIB_INLINE PointT<T> getLeftTop() const
	{
		return PointT<T>(left, top);
	}
	
	SLIB_INLINE void setLeftTop(T _x, T _y)
	{
		left = _x;
		top = _y;
	}
	
	SLIB_INLINE void setLeftTop(const PointT<T>& pt)
	{
		left = pt.x;
		top = pt.y;
	}
	
	SLIB_INLINE PointT<T> getLeftBottom() const
	{
		return PointT<T>(left, bottom);
	}
	
	SLIB_INLINE void setLeftBottom(T _x, T _y)
	{
		left = _x;
		bottom = _y;
	}
	
	SLIB_INLINE void setLeftBottom(const PointT<T>& pt)
	{
		left = pt.x;
		bottom = pt.y;
	}
	
	SLIB_INLINE PointT<T> getRightTop() const
	{
		return PointT<T>(right, top);
	}
	
	SLIB_INLINE void setRightTop(T _x, T _y)
	{
		right = _x;
		top = _y;
	}
	
	SLIB_INLINE void setRightTop(const PointT<T>& pt)
	{
		right = pt.x;
		top = pt.y;
	}
	
	SLIB_INLINE PointT<T> getRightBottom() const
	{
		return PointT<T>(right, bottom);
	}
	
	SLIB_INLINE void setRightBottom(T _x, T _y)
	{
		right = _x;
		bottom = _y;
	}
	
	SLIB_INLINE void setRightBottom(const PointT<T>& pt)
	{
		right = pt.x;
		bottom = pt.y;
	}
	
	SLIB_INLINE PointT<T> getCenter() const
	{
		return PointT<T>((left + right) / 2, (top + bottom) / 2);
	}
	
	SLIB_INLINE PointT<T> center() const
	{
		return getCenter();
	}
	
	SLIB_INLINE void setCenter(T _x, T _y)
	{
		T w = (right - left) / 2;
		T h = (bottom - top) / 2;
		left = _x - w;
		top = _y - h;
		right = _x + w;
		bottom = _y + h;
	}
	
	SLIB_INLINE void setCenter(const PointT<T>& pt)
	{
		setCenter(pt.x, pt.y);
	}
	
	SLIB_INLINE void setZero()
	{
		left = 0;
		top = 0;
		right = 0;
		bottom = 0;
	}
	
	SLIB_INLINE sl_bool containsPoint(T x, T y) const
	{
		return x >= left && x <= right && y >= top && y <= bottom;
	}
	
	SLIB_INLINE sl_bool containsPoint(const PointT<T>& pt) const
	{
		return pt.x >= left && pt.x <= right && pt.y >= top && pt.y <= bottom;
	}
	
	SLIB_INLINE sl_bool containsRectangle(const RectangleT<T>& other) const
	{
		return left <= other.right && right >= other.left && top <= other.bottom && bottom >= other.top;
	}
	
	SLIB_INLINE void setFromPoint(T x, T y)
	{
		left = right = x;
		top = bottom = y;
	}
	
	SLIB_INLINE void setFromPoint(const PointT<T>& pt)
	{
		left = right = pt.x;
		top = bottom = pt.y;
	}
	
	SLIB_INLINE void mergePoint(T x, T y)
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
	
	SLIB_INLINE void mergePoint(const PointT<T>& pt)
	{
		mergePoint(pt.x, pt.y);
	}
	
	void mergePoints(const PointT<T>* points, sl_size count)
	{
		for (sl_size i = 0; i < count; i++) {
			const PointT<T>& v = points[i];
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
	
	void mergePoints(const List< PointT<T> >& points)
	{
		ListLocker< PointT<T> > list(points);
		mergePoints(list.data(), list.getCount());
	}
	
	void setFromPoints(const PointT<T>* points, sl_size count)
	{
		if (count > 0) {
			setFromPoint(points[0]);
			if (count > 1) {
				mergePoints(points + 1, count - 1);
			}
		}
	}
	
	void setFromPoints(const List< PointT<T> >& points)
	{
		ListLocker< PointT<T> > list(points);
		setFromPoints(list.data(), list.getCount());
	}
	
	SLIB_INLINE void setFromPoints(const PointT<T>& pt1, const PointT<T>& pt2)
	{
		setFromPoint(pt1);
		mergePoint(pt2);
	}
	
	SLIB_INLINE void mergeRectangle(const RectangleT<T>& rect)
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
	
	// 4 points
	void getCornerPoints(PointT<T>* _out) const
	{
		_out[0].x = left; _out[0].y = top;
		_out[1].x = right; _out[1].y = top;
		_out[2].x = left; _out[2].y = bottom;
		_out[3].x = right; _out[3].y = bottom;
	}
};

template <class T>
T RectangleT<T>::_zero[4] = {0, 0, 0, 0};

typedef RectangleT<sl_real> Rectangle;
typedef RectangleT<sl_int32> Rectanglei;
typedef RectangleT<sl_int64> Rectangleli;
typedef RectangleT<float> Rectanglef;
typedef RectangleT<double> Rectanglelf;

SLIB_MATH_NAMESPACE_END

#endif
