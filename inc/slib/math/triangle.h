#ifndef CHECKHEADER_SLIB_MATH_TRIANGLE
#define CHECKHEADER_SLIB_MATH_TRIANGLE

#include "definition.h"

#include "point.h"

SLIB_MATH_NAMESPACE_BEGIN

template <class T>
class SLIB_EXPORT TriangleT
{
public:
	PointT<T> point1;
	PointT<T> point2;
	PointT<T> point3;
	
public:
	SLIB_INLINE TriangleT()
	{
	}
	
	template <class O>
	SLIB_INLINE TriangleT(const TriangleT<O>& other)
	{
		point1 = other.point1;
		point2 = other.point2;
		point3 = other.point3;
	}
	
	SLIB_INLINE TriangleT(const PointT<T>& _point1, const PointT<T>& _point2, const PointT<T>& _point3)
	{
		point1 = _point1;
		point2 = _point2;
		point3 = _point3;
	}
	
public:
	template <class O>
	SLIB_INLINE TriangleT<T>& operator=(const TriangleT<O>& other)
	{
		point1 = other.point1;
		point2 = other.point2;
		point3 = other.point3;
		return *this;
	}
	
public:
	void transform(Matrix3T<T>& mat)
	{
		point1 = mat.transformPosition(point1);
		point2 = mat.transformPosition(point2);
		point3 = mat.transformPosition(point3);
	}
	
	SLIB_INLINE static T getCross(const PointT<T>& _point1, const PointT<T>& _point2, const PointT<T>& _point3)
	{
		return (_point1.x - _point2.x) * (_point2.y - _point3.y) - (_point2.x - _point3.x) * (_point1.y - _point2.y);
	}
	
	SLIB_INLINE T getSize() const
	{
		return getCross(point1, point2, point3) / 2;
	}
	
	SLIB_INLINE T size() const
	{
		return getSize();
	}
};

typedef TriangleT<sl_real> Triangle;
typedef TriangleT<float> Trianglef;
typedef TriangleT<double> Trianglelf;

SLIB_MATH_NAMESPACE_END

#endif
