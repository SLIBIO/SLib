#ifndef CHECKHEADER_SLIB_MATH_LINE_SEGMENT
#define CHECKHEADER_SLIB_MATH_LINE_SEGMENT

#include "definition.h"

#include "point.h"

SLIB_MATH_NAMESPACE_BEGIN

template <class T>
class SLIB_EXPORT LineSegmentT
{
public:
	PointT<T> point1;
	PointT<T> point2;
	
public:
	SLIB_INLINE LineSegmentT() {}
	
	template <class O>
	SLIB_INLINE LineSegmentT(const LineSegmentT<O>& other)
	{
		point1 = other.point1;
		point2 = other.point2;
	}
	
	SLIB_INLINE LineSegmentT(const PointT<T>& _point1, const PointT<T>& _point2)
	{
		point1 = _point1;
		point2 = _point2;
	}
	
	SLIB_INLINE LineSegmentT(T x1, T y1, T x2, T y2)
	{
		point1.x = x1;
		point1.y = y1;
		point2.x = x2;
		point2.y = y2;
	}
	
	template <class O>
	SLIB_INLINE LineSegmentT<T>& operator=(const LineSegmentT<O>& other)
	{
		point1 = other.point1;
		point2 = other.point2;
		return *this;
	}
	
	SLIB_INLINE Vector2T<T> getDirection() const
	{
		return (point2 - point1);
	}
	SLIB_INLINE Vector2T<T> direction() const
	{
		return getDirection();
	}
	
	SLIB_INLINE T getLength2p() const
	{
		return point1.getLength2p(point2);
	}
	SLIB_INLINE T length2p() const
	{
		return getLength2p();
	}
	SLIB_INLINE T getLength() const
	{
		return point1.getLength(point2);
	}
	SLIB_INLINE T length() const
	{
		return getLength();
	}
	
	void transform(const Matrix3T<T>& mat)
	{
		point1 = mat.transformPosition(point1);
		point2 = mat.transformPosition(point2);
	}
	
	PointT<T> projectPoint(const PointT<T>& point) const
	{
		Vector2T<T> dir = point2 - point1;
		PointT<T> ret = point1 + (point - point1).dot(dir) * dir;
		return ret;
	}
	
	T getDistanceFromPoint(const PointT<T>& point) const
	{
		Vector2T<T> dir = point2 - point1;
		T f = (point - point1).dot(dir);
		Vector2T<T> proj = point1 + f * dir;
		if (f < 0) {
			return point1.getLength(point);
		} else {
			if (f > length()) {
				return point2.getLength(point);
			} else {
				return proj.getLength(point);
			}
		}
	}
	
	T getDistanceFromPointOnInfiniteLine(const PointT<T>& point) const
	{
		Vector2T<T> dir = point2 - point1;
		T f = (point - point1).dot(dir);
		Vector2T<T> proj = point1 + f * dir;
		return proj.getLength(point);
	}
};

typedef LineSegmentT<sl_real> LineSegment;
typedef LineSegmentT<float> LineSegmentf;
typedef LineSegmentT<double> LineSegmentlf;

SLIB_MATH_NAMESPACE_END

#endif
