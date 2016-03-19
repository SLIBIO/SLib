#ifndef CHECKHEADER_SLIB_MATH_TRIANGLE3
#define CHECKHEADER_SLIB_MATH_TRIANGLE3

#include "definition.h"

#include "vector3.h"
#include "line3.h"
#include "plane.h"
#include "matrix4.h"

SLIB_MATH_NAMESPACE_BEGIN

template <class T>
class SLIB_EXPORT Triangle3T
{
public:
	Vector3T<T> point1;
	Vector3T<T> point2;
	Vector3T<T> point3;
	
public:
	Triangle3T() = default;
	
	Triangle3T(const Triangle3T<T>& other) = default;
	
	template <class O>
	Triangle3T(const Triangle3T<O>& other);
	
	Triangle3T(const Vector3T<T>& point1, const Vector3T<T>& point2, const Vector3T<T>& point3);
	
public:
	static Vector3T<T> getNormal(const Vector3T<T>& point1, const Vector3T<T>& point2, const Vector3T<T>& point3);
	
	Vector3T<T> getNormal() const;
	
	T getSize() const;
	
	PlaneT<T> getPlane() const;
	
	void transform(const Matrix4T<T>& mat);
	
	Vector3T<T> projectPoint(const Vector3T<T>& point, T* pDist = sl_null, T* pU = sl_null, T* pV = sl_null) const;
	
	sl_bool intersectLine(
		Line3T<T>& line,
		Vector3T<T>* outIntersectPoint = sl_null,
		T* pDist = sl_null,
		T* pU = sl_null,
		T* pV = sl_null,
		sl_bool* pFlagParallel = sl_null,
		sl_bool* pFlagExtendPoint1 = sl_null,
		sl_bool* pFlagExtendPoint2 = sl_null,
		sl_bool* pFlagExtendTriangle = sl_null) const;
	
	sl_bool intersectPlane(
		const PlaneT<T>& plane,
		Line3T<T>* outLine = sl_null,
		sl_uint32* outLineNo1 = sl_null,
		sl_uint32* outLineNo2 = sl_null) const;
	
	sl_bool intersectTriangle(const Triangle3T<T>& triangle, Line3T<T>* outLine) const;
	
public:
	Triangle3T<T>& operator=(const Triangle3T<T>& other) = default;
	
	template <class O>
	Triangle3T<T>& operator=(const Triangle3T<O>& other);

};

SLIB_DECLARE_GEOMETRY_TYPE(Triangle3)

SLIB_MATH_NAMESPACE_END


SLIB_MATH_NAMESPACE_BEGIN

template <class T>
template <class O>
Triangle3T<T>::Triangle3T(const Triangle3T<O>& other)
: point1(other.point1), point2(other.point2), point3(other.point3)
{
}

template <class T>
template <class O>
Triangle3T<T>& Triangle3T<T>::operator=(const Triangle3T<O>& other)
{
	point1 = other.point1;
	point2 = other.point2;
	point3 = other.point3;
	return *this;
}

SLIB_MATH_NAMESPACE_END

#endif
