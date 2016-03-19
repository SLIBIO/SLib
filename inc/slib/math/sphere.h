#ifndef CHECKHEADER_SLIB_MATH_SPHERE
#define CHECKHEADER_SLIB_MATH_SPHERE

#include "definition.h"

#include "vector3.h"
#include "line3.h"

SLIB_MATH_NAMESPACE_BEGIN

template <class T>
class SLIB_EXPORT SphereT
{
public:
	Vector3T<T> center;
	T radius;
	
public:
	SphereT() = default;
	
	SphereT(const SphereT<T>& other) = default;
	
	template <class O>
	SphereT(const SphereT<O>& other);
	
	SphereT(const Vector3T<T>& center, T radius);
	
	SphereT(T xc, T yc, T zc, T radius);
	
public:
	sl_bool containsPoint(const Vector3T<T>& point) const;
	
	// returns the count of intersected points
	sl_uint32 intersectLine(const Line3T<T>& line, Vector3T<T>* pOut1, Vector3T<T>* pOut2) const;
	
public:
	SphereT<T>& operator=(const SphereT<T>& other) = default;
	
	template <class O>
	SphereT<T>& operator=(const SphereT<O>& other);

};

SLIB_DECLARE_GEOMETRY_TYPE(Sphere)

SLIB_MATH_NAMESPACE_END


SLIB_MATH_NAMESPACE_BEGIN

template <class T>
template <class O>
SLIB_INLINE SphereT<T>::SphereT(const SphereT<O>& other) : center(other.center), radius((T)(other.radius))
{
}

template <class T>
template <class O>
SLIB_INLINE SphereT<T>& SphereT<T>::operator=(const SphereT<O>& other)
{
	center = other.center;
	radius = (T)(other.radius);
	return *this;
}

SLIB_MATH_NAMESPACE_END

#endif
