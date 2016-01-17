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
	SLIB_INLINE SphereT()
	{
	}
	
	template <class O>
	SLIB_INLINE SphereT(const SphereT<O>& other)
	{
		center = other.center;
		radius = (T)(other.radius);
	}
	
	SLIB_INLINE SphereT(const Vector3T<T>& _center, T _radius)
	{
		center = _center;
		radius = _radius;
	}
	
	SLIB_INLINE SphereT(T xc, T yc, T zc, T _radius)
	{
		center.x = xc;
		center.y = yc;
		center.z = zc;
		radius = _radius;
	}

public:
	template <class O>
	SLIB_INLINE SphereT& operator=(const SphereT<O>& other)
	{
		center = other.center;
		radius = (T)(other.radius);
		return *this;
	}
	
public:
	SLIB_INLINE sl_bool containsPoint(const Vector3T<T>& point)
	{
		T dx = point.x - center.x;
		T dy = point.y - center.y;
		T dz = point.z - center.z;
		return dx * dx + dy * dy + dz * dz <= radius * radius;
	}
	
	// returns the count of intersected points
	sl_uint32 intersectLine(const Line3T<T>& line, Vector3T<T>* pOut1, Vector3T<T>* pOut2)
	{
		const SphereT<T>& sphere = *this;
		Vector3T<T> l = line.getDirection();
		if (Math::isLessThanEpsilon(l.getLength2p())) {
			return 0;
		}
		l.normalize();
		Vector3T<T> o = line.point1;
		Vector3T<T> c = sphere.center;
		T r = sphere.radius;
		Vector3T<T> o_c = o - c;
		T s1 = l.dot(o_c);
		T s = s1 * s1 - o_c.getLength2p() + r * r;
		if (s < 0) {
			return 0;
		}
		s = Math::sqrt(s);
		T d0 = -l.dot(o_c);
		if (pOut1) {
			*pOut1 = o + l * (d0 - s);
		}
		if (pOut2) {
			*pOut2 = o + l * (d0 + s);
		}
		if (Math::isLessThanEpsilon(s)) {
			return 1;
		} else {
			return 2;
		}
	}
};

typedef SphereT<sl_real> Sphere;
typedef SphereT<float> Spheref;
typedef SphereT<double> Spherelf;

SLIB_MATH_NAMESPACE_END

#endif
