#include "../../../inc/slib/math/sphere.h"

#include "../../../inc/slib/core/math.h"

namespace slib
{

	template <class T>
	SphereT<T>::SphereT(const Vector3T<T>& _center, T _radius) : center(_center), radius(_radius)
	{
	}

	template <class T>
	SphereT<T>::SphereT(T xc, T yc, T zc, T _radius) : center(xc, yc, zc), radius(_radius)
	{
	}

	template <class T>
	sl_bool SphereT<T>::containsPoint(const Vector3T<T>& point) const
	{
		T dx = point.x - center.x;
		T dy = point.y - center.y;
		T dz = point.z - center.z;
		return dx * dx + dy * dy + dz * dz <= radius * radius;
	}

	template <class T>
	sl_uint32 SphereT<T>::intersectLine(const Line3T<T>& line, Vector3T<T>* pOut1, Vector3T<T>* pOut2) const
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

	SLIB_DEFINE_GEOMETRY_TYPE(Sphere)

}
