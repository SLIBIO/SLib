#include "../../../inc/slib/math/triangle.h"

SLIB_MATH_NAMESPACE_BEGIN

template <class T>
TriangleT<T>::TriangleT(const PointT<T>& _point1, const PointT<T>& _point2, const PointT<T>& _point3)
: point1(_point1), point2(_point2), point3(_point3)
{
}

template <class T>
T TriangleT<T>::getCross(const PointT<T>& _point1, const PointT<T>& _point2, const PointT<T>& _point3)
{
	return (_point1.x - _point2.x) * (_point2.y - _point3.y) - (_point2.x - _point3.x) * (_point1.y - _point2.y);
}

template <class T>
T TriangleT<T>::getCross() const
{
	return getCross(point1, point2, point3);
}

template <class T>
T TriangleT<T>::getSize() const
{
	return getCross(point1, point2, point3) / 2;
}

template <class T>
void TriangleT<T>::transform(Matrix3T<T>& mat)
{
	point1 = mat.transformPosition(point1);
	point2 = mat.transformPosition(point2);
	point3 = mat.transformPosition(point3);
}

SLIB_DEFINE_GEOMETRY_TYPE(Triangle)

SLIB_MATH_NAMESPACE_END
