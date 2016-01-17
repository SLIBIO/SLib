#ifndef CHECKHEADER_SLIB_MATH_PLANE
#define CHECKHEADER_SLIB_MATH_PLANE

#include "definition.h"

#include "vector3.h"
#include "line3.h"
#include "matrix4.h"

SLIB_MATH_NAMESPACE_BEGIN

/*
	ax + by + cz + d = 0
 */
template <class T>
class SLIB_EXPORT PlaneT
{
public:
	T a;
	T b;
	T c;
	T d;
	
public:
	SLIB_INLINE PlaneT()
	{
	}
	
	template <class O>
	SLIB_INLINE PlaneT(const PlaneT<O>& other)
	{
		a = (T)(other.a);
		b = (T)(other.b);
		c = (T)(other.c);
		d = (T)(other.d);
	}
	
	SLIB_INLINE PlaneT(T _a, T _b, T _c, T _d)
	{
		a = _a;
		b = _b;
		c = _c;
		d = _d;
	}
	
	SLIB_INLINE PlaneT(const Vector3T<T>& point, const Vector3T<T>& normal)
	{
		setFromPointAndNormal(point, normal);
	}
	
public:
	template <class O>
	SLIB_INLINE PlaneT<T>& operator=(const PlaneT<O>& other)
	{
		a = (T)(other.a);
		b = (T)(other.b);
		c = (T)(other.c);
		d = (T)(other.d);
		return *this;
	}
	
public:
	SLIB_INLINE Vector3T<T> getNormal() const
	{
		return Vector3T<T>(a, b, c);
	}
	
	SLIB_INLINE Vector3T<T> projectOriginOnNormalized() const
	{
		return Vector3T<T>(-a*d, -b*d, -c*d);
	}
	
	Vector3T<T> projectOrigin() const
	{
		T L = a * a + b * b + c * c;
		if (L > 0) {
			return Vector3T<T>(-a*d / L, -b*d / L, -c*d / L);
		} else {
			return Vector3T<T>(0, 0, 0);
		}
	}
	
	SLIB_INLINE T getDistanceFromPointOnNormalized(const Vector3T<T>& pos) const
	{
		return a * pos.x + b * pos.y + c * pos.z + d;
	}
	
	T getDistanceFromPoint(const Vector3T<T>& pos) const
	{
		T L = a * a + b * b + c * c;
		if (L > 0) {
			L = Math::sqrt(L);
			return (a * pos.x + b * pos.y + c * pos.z + d) / L;
		} else {
			return d;
		}
	}
	
	Vector3T<T> projectPointOnNormalized(const Vector3T<T>& pos) const
	{
		T D = a * pos.x + b * pos.y + c * pos.z + d;
		return Vector3T<T>(pos.x - D * a, pos.y - D * b, pos.z - D * c);
	}
	
	Vector3T<T> projectPoint(const Vector3T<T>& pos) const
	{
		T L = a * a + b * b + c * c;
		if (L > 0) {
			T D = a * pos.x + b * pos.y + c * pos.z + d;
			return Vector3T<T>(pos.x - D * a / L, pos.y - D * b / L, pos.z - D * c / L);
		} else {
			return d;
		}
	}
	
	void setFromPointAndNormal(const Vector3T<T>& point, const Vector3T<T>& normal)
	{
		a = normal.x;
		b = normal.y;
		c = normal.z;
		d = -point.dot(normal);
	}
	
	void normalize()
	{
		T l = Math::sqrt(a * a + b * b + c * c);
		if (l > 0) {
			a /= l;
			b /= l;
			c /= l;
			d /= l;
		}
	}
	
	void transform(const Matrix4T<T>& mat)
	{
		T _a = a * mat.m00 + b * mat.m10 + c * mat.m20;
		T _b = a * mat.m01 + b * mat.m11 + c * mat.m21;
		T _c = a * mat.m02 + b * mat.m12 + c * mat.m22;
		T L = a * a + b * b + c * c;
		if (L > 0) {
			T k = d / L;
			d = (k * _a + mat.m30) * _a + (k * _b + mat.m31) * _b + (k * _c + mat.m32) * _c;
			a = _a;
			b = _b;
			c = _c;
		} else {
			d = 0;
		}
	}
	
	// return sl_true when the plane intersects to the line segment
	sl_bool intersectLine(
		const Line3T<T>& line,
		Vector3T<T>* outIntersectPoint = sl_null,
		sl_bool* pFlagParallel = sl_null,
		sl_bool* pFlagExtendPoint1 = sl_null,
		sl_bool* pFlagExtendPoint2 = sl_null)
	{
		// distances from line end points
		T len = line.getLength();
		T d1 = getDistanceFromPointOnNormalized(line.point1);
		if (Math::isNearZero(len)) {
			if (pFlagParallel) {
				*pFlagParallel = sl_false;
			}
			if (pFlagExtendPoint1) {
				*pFlagExtendPoint1 = sl_false;
			}
			if (pFlagExtendPoint2) {
				*pFlagExtendPoint2 = sl_false;
			}
			if (Math::isNearZero(d1)) {
				return sl_true;
			} else {
				return sl_false;
			}
		}
		T d2 = getDistanceFromPointOnNormalized(line.point2);
		T dd = d1 - d2;
		if (Math::isNearZero(dd)) {
			if (pFlagParallel) {
				*pFlagParallel = sl_true;
			}
			if (pFlagExtendPoint1) {
				*pFlagExtendPoint1 = sl_false;
			}
			if (pFlagExtendPoint2) {
				*pFlagExtendPoint2 = sl_false;
			}
			if (Math::isNearZero(d1)) {
				return sl_true;
			} else {
				return sl_false;
			}
		}
		if (pFlagParallel) {
			*pFlagParallel = sl_false;
		}
		T ratioInter = d1 / dd;
		if (outIntersectPoint) {
			*outIntersectPoint = line.point1 + line.getDirection() * ratioInter;
		}
		if (d1 * d2 <= 0) {
			if (pFlagExtendPoint1) {
				*pFlagExtendPoint1 = sl_false;
			}
			if (pFlagExtendPoint2) {
				*pFlagExtendPoint2 = sl_false;
			}
			return sl_true;
		} else {
			if (ratioInter > 0) {
				if (pFlagExtendPoint1) {
					*pFlagExtendPoint1 = sl_false;
				}
				if (pFlagExtendPoint2) {
					*pFlagExtendPoint2 = sl_true;
				}
				return sl_false;
			} else {
				if (pFlagExtendPoint1) {
					*pFlagExtendPoint1 = sl_true;
				}
				if (pFlagExtendPoint2) {
					*pFlagExtendPoint2 = sl_false;
				}
				return sl_false;
			}
		}
	}
	
	sl_bool intersectPlane(const PlaneT<T>& plane, Line3T<T>* outIntersectLine = sl_null, sl_bool* pFlagParallel = sl_null)
	{
		PlaneT<T> plane1 = *this;
		PlaneT<T> plane2 = plane;
		const Vector3T<T>& N1 = plane1.getNormal();
		const Vector3T<T>& N2 = plane2.getNormal();
		T D1 = plane1.d;
		T D2 = plane2.d;
		T flagIntersect = sl_true;
		Vector3T<T> vStart;
		Vector3T<T> vDirection = N1.cross(N2);
		if (Math::isNearZero(vDirection.x)) {
			if (Math::isNearZero(vDirection.y)) {
				if (Math::isNearZero(vDirection.z)) {
					if (pFlagParallel) {
						*pFlagParallel = sl_true;
					}
					plane1.normalize();
					plane2.normalize();
					if (Math::isNearZero(D1 - D2)) {
						return sl_true;
					} else {
						return sl_false;
					}
				} else {
					vStart.z = 0;
					T D = N1.x * N2.y - N1.y * N2.x;
					vStart.x = (-D1*N2.y + D2*N1.y) / D;
					vStart.y = (-N1.x*D2 + N2.x*D1) / D;
				}
			} else {
				vStart.y = 0;
				T D = N1.x * N2.z - N1.z * N2.x;
				vStart.x = (-D1*N2.z + D2*N1.z) / D;
				vStart.z = (-N1.x*D2 + N2.x*D1) / D;
			}
		} else {
			vStart.x = 0;
			T D = N1.y * N2.z - N1.z * N2.y;
			vStart.y = (-D1*N2.z + D2*N1.z) / D;
			vStart.z = (-N1.y*D2 + N2.y*D1) / D;
		}
		if (pFlagParallel) {
			*pFlagParallel = sl_false;
		}
		if (outIntersectLine) {
			outIntersectLine->point1 = vStart;
			outIntersectLine->point2 = vStart + vDirection;
		}
		return sl_true;
	}
	
};

typedef PlaneT<sl_real> Plane;
typedef PlaneT<float> Planef;
typedef PlaneT<double> Planelf;

SLIB_MATH_NAMESPACE_END

#endif
