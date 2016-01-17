#ifndef CHECKHEADER_SLIB_MATH_LINE3
#define CHECKHEADER_SLIB_MATH_LINE3

#include "definition.h"

#include "vector3.h"
#include "matrix4.h"

SLIB_MATH_NAMESPACE_BEGIN

template <class T>
class SLIB_EXPORT Line3T
{
public:
	Vector3T<T> point1;
	Vector3T<T> point2;
	
public:
	SLIB_INLINE Line3T()
	{
	}
	
	template <class O>
	SLIB_INLINE Line3T(const Line3T<O>& other)
	{
		point1 = other.point1;
		point2 = other.point2;
	}
	
	SLIB_INLINE Line3T(const Vector3T<T>& _point1, const Vector3T<T>& _point2)
	{
		point1 = _point1;
		point2 = _point2;
	}
	
	SLIB_INLINE Line3T(T x1, T y1, T z1, T x2, T y2, T z2)
	{
		point1.x = x1;
		point1.y = y1;
		point1.z = z1;
		point2.x = x2;
		point2.y = y2;
		point2.z = z2;
	}
	
public:
	template <class O>
	SLIB_INLINE Line3T<T>& operator=(const Line3T<O>& other)
	{
		point1 = other.point1;
		point2 = other.point2;
		return *this;
	}
	
public:
	SLIB_INLINE Vector3T<T> getDirection() const
	{
		return (point2 - point1);
	}
	
	SLIB_INLINE Vector3T<T> direction() const
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
	
	void transform(const Matrix4T<T>& mat)
	{
		point1 = mat.transformPosition(point1);
		point2 = mat.transformPosition(point2);
	}
	
	Vector3T<T> projectPoint(const Vector3T<T>& point) const
	{
		Vector3T<T> dir = point2 - point1;
		Vector3T<T> ret = point1 + (point - point1).dot(dir) * dir;
		return ret;
	}
	
	T getDistanceFromPoint(const Vector3T<T>& point) const
	{
		Vector3T<T> dir = point2 - point1;
		T f = (point - point1).dot(dir);
		Vector3T<T> proj = point1 + f * dir;
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
	
	T getDistanceFromPointOnInfiniteLine(const Vector3T<T>& point) const
	{
		Vector3T<T> dir = point2 - point1;
		T f = (point - point1).dot(dir);
		Vector3T<T> proj = point1 + f * dir;
		return proj.getLength(point);
	}
	
};

typedef Line3T<sl_real> Line3;
typedef Line3T<float> Line3f;
typedef Line3T<double> Line3lf;

SLIB_MATH_NAMESPACE_END

#endif
