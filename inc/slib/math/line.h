#ifndef CHECKHEADER_SLIB_MATH_LINE
#define CHECKHEADER_SLIB_MATH_LINE

#include "definition.h"

#include "point.h"

SLIB_MATH_NAMESPACE_BEGIN

/*
	ax + by + c = 0
*/
template <class T>
class SLIB_EXPORT LineT
{
public:
	T a;
	T b;
	T c;
	
public:
	SLIB_INLINE LineT() {}
	
	template <class O>
	SLIB_INLINE LineT(const LineT<O>& other)
	{
		a = (T)(other.a);
		b = (T)(other.b);
		c = (T)(other.c);
	}
	
	SLIB_INLINE LineT(T _a, T _b, T _c)
	{
		a = _a;
		b = _b;
		c = _c;
	}
	
	SLIB_INLINE LineT(const PointT<T>& point, const Vector2T<T>& dir)
	{
		setFromPointAndDirection(point, dir);
	}
	
	template <class O>
	SLIB_INLINE LineT<T>& operator=(const LineT<O>& other)
	{
		a = (T)(other.a);
		b = (T)(other.b);
		c = (T)(other.c);
		return *this;
	}
	
	SLIB_INLINE Vector2T<T> getDirection() const
	{
		return Vector2T<T>(b, -a);
	}
	
	SLIB_INLINE Vector2T<T> getNormal() const
	{
		return Vector2T<T>(a, b);
	}
	
	SLIB_INLINE Vector2T<T> getPointProjectedFromOriginOnNormalized() const
	{
		return Vector2T<T>(-a*c, -b*c);
	}
	
	Vector2T<T> getPointProjectedFromOrigin() const
	{
		T L = a * a + b * b;
		if (L > 0) {
			return Vector2T<T>(-a*c / L, -b*c / L);
		} else {
			return Vector2T<T>(0, 0);
		}
	}
	
	SLIB_INLINE T getDistanceFromPointOnNormalized(const PointT<T>& pos) const
	{
		return a * pos.x + b * pos.y + c;
	}
	
	T getDistanceFromPoint(const PointT<T>& pos) const
	{
		T L = a * a + b * b;
		if (L > 0) {
			L = Math::sqrt(L);
			return (a * pos.x + b * pos.y + c) / L;
		} else {
			return c;
		}
	}
	
	void setFromPointAndDirection(const PointT<T>& point, const Vector2T<T>& dir)
	{
		a = dir.y;
		b = -dir.x;
		c = -(point.x * a + point.y * b);
	}
	
	void setFromPointAndNormal(const PointT<T>& point, const Vector2T<T>& normal)
	{
		a = normal.x;
		b = normal.y;
		c = -point.dot(normal);
	}
	
	void normalize()
	{
		T l = Math::sqrt(a * a + b * b);
		a /= l;
		b /= l;
		c /= l;
	}
	
	void transform(const Matrix3T<T>& mat)
	{
		T _a = a * mat.m00 + b * mat.m10;
		T _b = a * mat.m01 + b * mat.m11;
		T L = a * a + b * b;
		if (L > 0) {
			T k = c / L;
			c = (k * _a + mat.m20) * _a + (k * _b + mat.m21) * _b;
			a = _a;
			b = _b;
		} else {
			c = 0;
		}
	}
	
};

typedef LineT<sl_real> Line;
typedef LineT<float> Linef;
typedef LineT<double> Linelf;

SLIB_MATH_NAMESPACE_END

#endif
