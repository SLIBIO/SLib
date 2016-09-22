#ifndef CHECKHEADER_SLIB_MATH_RECTANGLE
#define CHECKHEADER_SLIB_MATH_RECTANGLE

#include "definition.h"

#include "point.h"
#include "size.h"

#include "../core/list.h"

SLIB_MATH_NAMESPACE_BEGIN

template <class T, class FT = T>
class SLIB_EXPORT RectangleT
{
public:
	T left;
	T top;
	T right;
	T bottom;
	
public:
	RectangleT() = default;
	
	RectangleT(const RectangleT<T, FT>& other) = default;
	
	template <class O, class FO>
	RectangleT(const RectangleT<O, FO>& other);
	
	RectangleT(T left, T top, T right, T bottom);
	
	RectangleT(const PointT<T, FT>& pt, const SizeT<T, FT>& size);
	
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
	
	sl_bool equals(const RectangleT<T, FT>& other) const;
	
	sl_bool isAlmostEqual(const RectangleT<T, FT>& other) const;
	
	sl_bool isValidSize();
	
	sl_bool fixSizeError();
	
public:
	RectangleT<T, FT>& operator=(const RectangleT<T, FT>& other) = default;
	
	template <class O, class FO>
	RectangleT<T, FT>& operator=(const RectangleT<O, FO>& other);
	
private:
	static T _zero[4];

};

SLIB_DECLARE_GEOMETRY_TYPE_EX(Rectangle)

SLIB_MATH_NAMESPACE_END


SLIB_NAMESPACE_BEGIN

template <class T, class FT>
template <class O, class FO>
SLIB_INLINE RectangleT<T, FT>::RectangleT(const RectangleT<O, FO>& other)
: left((T)(other.left)), top((T)(other.top)), right((T)(other.right)), bottom((T)(other.bottom))
{
}

template <class T, class FT>
SLIB_INLINE RectangleT<T, FT>::RectangleT(T _left, T _top, T _right, T _bottom)
: left(_left), top(_top), right(_right), bottom(_bottom)
{
}

template <class T, class FT>
SLIB_INLINE RectangleT<T, FT>::RectangleT(const PointT<T, FT>& pt, const SizeT<T, FT>& size)
: left(pt.x), top(pt.y), right(pt.x + size.x), bottom(pt.y + size.y)
{
}

template <class T, class FT>
SLIB_INLINE const RectangleT<T, FT>& RectangleT<T, FT>::zero()
{
	return *((RectangleT<T, FT>*)((void*)_zero));
}

template <class T, class FT>
SLIB_INLINE T RectangleT<T, FT>::getWidth() const
{
	return right - left;
}

template <class T, class FT>
SLIB_INLINE T RectangleT<T, FT>::getHeight() const
{
	return bottom - top;
}

template <class T, class FT>
SLIB_INLINE SizeT<T, FT> RectangleT<T, FT>::getSize() const
{
	return {right - left, bottom - top};
}

template <class T, class FT>
template <class O, class FO>
SLIB_INLINE RectangleT<T, FT>& RectangleT<T, FT>::operator=(const RectangleT<O, FO>& other)
{
	left = (T)(other.left);
	top = (T)(other.top);
	right = (T)(other.right);
	bottom = (T)(other.bottom);
	return *this;
}

SLIB_NAMESPACE_END

#endif
