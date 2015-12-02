#ifndef CHECKHEADER_SLIB_MATH_GEOMETRY3D
#define CHECKHEADER_SLIB_MATH_GEOMETRY3D

#include "definition.h"
#include "vector.h"
#include "matrix.h"
#include "geometry2d.h"

SLIB_MATH_NAMESPACE_BEGIN

template <class T>
class SLIB_EXPORT QuaternionT
{
public:
	T x;
	T y;
	T z;
	T w;

public:
	SLIB_INLINE QuaternionT() {}

	SLIB_INLINE QuaternionT(T x, T y, T z, T w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}
	
	template <class O>
	SLIB_INLINE QuaternionT(const QuaternionT<O>& other)
	{
		x = (T)(other.x);
		y = (T)(other.y);
		z = (T)(other.z);
		w = (T)(other.w);
	}

	SLIB_INLINE QuaternionT(const Vector4T<T>& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
	}

	const Vector4T<T>& toVector4() const
	{
		return *((Vector4T<T>*)this);
	}
	Vector4T<T>& toVector4()
	{
		return *((Vector4T<T>*)this);
	}

private:
	SLIB_EXPORT static T _identity[4];
public:
	static const QuaternionT<T>& identity()
	{
		return *((QuaternionT<T>*)((void*)_identity));
	}

	SLIB_INLINE sl_bool operator==(const QuaternionT<T>& other) const
	{
		return ((x == other.x) && (y == other.y) && (z == other.z) && (w == other.w));
	}

	SLIB_INLINE sl_bool operator!=(const QuaternionT<T>& other) const
	{
		return !(*this == other);
	}

	template <class O>
	SLIB_INLINE QuaternionT<T>& operator=(const QuaternionT<O>& other)
	{
		x = (T)(other.x);
		y = (T)(other.y);
		z = (T)(other.z);
		w = (T)(other.w);
		return *this;
	}
	SLIB_INLINE QuaternionT<T>& operator=(const Vector4T<T>& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
		return *this;
	}

	SLIB_INLINE QuaternionT<T>& operator*=(const QuaternionT<T>& other)
	{
		multiply(other);
		return *this;
	}

	SLIB_INLINE QuaternionT<T> operator*(const QuaternionT<T>& other) const
	{
		QuaternionT<T> ret = *this;
		ret.multiply(other);
		return ret;
	}

	SLIB_INLINE QuaternionT<T>& operator/=(const QuaternionT<T>& other)
	{
		divide(other);
		return *this;
	}

	SLIB_INLINE QuaternionT<T> operator/(QuaternionT<T>& other) const
	{
		QuaternionT<T> ret = *this;
		ret.divide(other);
		return ret;
	}

public:
	SLIB_INLINE T getLength2p() const
	{
		return x * x + y * y + z * z + w * w;
	}
	SLIB_INLINE T length2p() const
	{
		return getLength2p();
	}

	SLIB_INLINE T getLength() const
	{
		return Math::sqrt(getLength2p());
	}
	SLIB_INLINE T length() const
	{
		return getLength();
	}

	void multiply(const QuaternionT<T>& other)
	{
		T ox = other.x, oy = other.y, oz = other.z, ow = other.w;
		T qx = w * ox + x * ow + y * oz - z * oy;
		T qy = w * oy + y * ow + z * ox - x * oz;
		T qz = w * oz + z * ow + x * oy - y * ox;
		T qw = w * ow + x * ox + y * oy - z * oz;
		x = qx; y = qy; z = qz; w = qw;
	}

	void divide(const QuaternionT<T>& other)
	{
		T ox = -other.x, oy = -other.y, oz = -other.z, ow = other.w;
		T qx = w * ox + x * ow + y * oz - z * oy;
		T qy = w * oy + y * ow + z * ox - x * oz;
		T qz = w * oz + z * ow + x * oy - y * ox;
		T qw = w * ow + x * ox + y * oy - z * oz;
		x = qx; y = qy; z = qz; w = qw;
	}

	void setRotation(const Vector3T<T>& vAxis, T fAngle)
	{
		T f = Math::sin(fAngle / 2) / vAxis.getLength();
		x = vAxis.x * f;
		y = vAxis.y * f;
		z = vAxis.z * f;
		w = Math::cos(fAngle / 2);
	}

	SLIB_INLINE static QuaternionT<T> getRotation(const Vector3T<T>& vAxis, T fAngle)
	{
		QuaternionT<T> ret;
		ret.setRotation(vAxis, fAngle);
		return ret;
	}
public:
	SLIB_INLINE T getAngle() const
	{
		return 2 * Math::arccos(w);
	}

	SLIB_INLINE Vector4T<T> getAxis() const
	{
		return Vector4T<T>(x, y, z, 0);
	}

	SLIB_INLINE void makeInverse()
	{
		x = -x;
		y = -y;
		z = -z;
	}

	SLIB_INLINE QuaternionT<T> inverse() const
	{
		QuaternionT<T> ret = *this;
		ret.makeInverse();
		return ret;
	}
};

template <class T>
T QuaternionT<T>::_identity[4] = { 0, 0, 0, 1 };

typedef QuaternionT<sl_real> Quaternion;
typedef QuaternionT<float> Quaternionf;
typedef QuaternionT<double> Quaternionlf;

template <class T>
class SLIB_EXPORT Line3T
{
public:
	Vector3T<T> point1;
	Vector3T<T> point2;

public:
	SLIB_INLINE Line3T() {}

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

	template <class O>
	SLIB_INLINE Line3T<T>& operator=(const Line3T<O>& other)
	{
		point1 = other.point1;
		point2 = other.point2;
		return *this;
	}

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
	SLIB_INLINE PlaneT() {}

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

	template <class O>
	SLIB_INLINE PlaneT<T>& operator=(const PlaneT<O>& other)
	{
		a = (T)(other.a);
		b = (T)(other.b);
		c = (T)(other.c);
		d = (T)(other.d);
		return *this;
	}

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
	sl_bool intersectLine(const Line3T<T>& line, Vector3T<T>* outIntersectPoint = sl_null
		, sl_bool* pFlagParallel = sl_null, sl_bool* pFlagExtendPoint1 = sl_null, sl_bool* pFlagExtendPoint2 = sl_null)
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

template <class T>
class SLIB_EXPORT Triangle3T
{
public:
	Vector3T<T> point1;
	Vector3T<T> point2;
	Vector3T<T> point3;

	SLIB_INLINE Triangle3T() {}

	template <class O>
	SLIB_INLINE Triangle3T(const Triangle3T<O>& other)
	{
		point1 = other.point1;
		point2 = other.point2;
		point3 = other.point3;
	}

	SLIB_INLINE Triangle3T(const Vector3T<T>& _point1, const Vector3T<T>& _point2, const Vector3T<T>& _point3)
	{
		point1 = _point1;
		point2 = _point2;
		point3 = _point3;
	}

	template <class O>
	SLIB_INLINE Triangle3T& operator=(const Triangle3T<O>& other)
	{
		point1 = other.point1;
		point2 = other.point2;
		point3 = other.point3;
		return *this;
	}

	void transform(const Matrix4T<T>& mat)
	{
		point1 = mat.transformPosition(point1);
		point2 = mat.transformPosition(point2);
		point3 = mat.transformPosition(point3);
	}

	static Vector3T<T> getNormal(const Vector3T<T>& point1, const Vector3T<T>& point2, const Vector3T<T>& point3)
	{
		Vector3T<T> v = (point1 - point2).cross(point2 - point3);
		return v;
	}
	SLIB_INLINE Vector3T<T> getNormal() const
	{
		return getNormal(point1, point2, point3);
	}
	SLIB_INLINE Vector3T<T> normal() const
	{
		return getNormal();
	}

	T getSize() const
	{
		return (point1 - point2).cross(point2 - point3).length() / 2;
	}
	SLIB_INLINE T size() const
	{
		return getSize();
	}

	SLIB_INLINE PlaneT<T> getPlane() const
	{
		return PlaneT<T>(point1, getNormal());
	}
	SLIB_INLINE PlaneT<T> plane() const
	{
		return getPlane();
	}

	Vector3T<T> projectPoint(const Vector3T<T>& point, T* pDist = sl_null, T* pU = sl_null, T* pV = sl_null)
	{
		Triangle3T<T>& triangle = *this;
		Vector3T<T> P = triangle.point2 - triangle.point1;
		Vector3T<T> Q = triangle.point3 - triangle.point1;
		Vector3T<T> N = P.cross(Q);
		T LN = N.getLength();
		if (Math::isNearZero(LN)) {
			if (pDist) {
				*pDist = 0;
			}
			if (pU) {
				*pU = 0;
			}
			if (pV) {
				*pV = 0;
			}
			return point;
		}
		N /= LN;
		T dist = N.dot(point - triangle.point1);
		Vector3T<T> R = point + N * dist;
		if (pDist) {
			*pDist = Math::abs(dist);
		}
		Vector3T<T> K = R - triangle.point1;
		if (pU) {
			*pU = K.cross(Q).getLength() / LN;
		}
		if (pV) {
			*pV = P.cross(K).getLength() / LN;
		}
		return R;
	}

	sl_bool intersectLine(
		Line3T<T>& line, Vector3T<T>* outIntersectPoint = sl_null, T* pDist = sl_null, T* pU = sl_null, T* pV = sl_null
		, sl_bool* pFlagParallel = sl_null, sl_bool* pFlagExtendPoint1 = sl_null, sl_bool* pFlagExtendPoint2 = sl_null, sl_bool* pFlagExtendTriangle = sl_null)
	{
		Triangle3T<T>& triangle = *this;
		T L = line.getLength();
		if (Math::isNearZero(L)) {
			if (pFlagParallel) {
				*pFlagParallel = sl_false;
			}
			if (pFlagExtendPoint1) {
				*pFlagExtendPoint1 = sl_false;
			}
			if (pFlagExtendPoint2) {
				*pFlagExtendPoint2 = sl_false;
			}
			T dist, u, v;
			Vector3T<T> P = projectPoint(line.point1, &dist, &u, &v);
			if (Math::isNearZero(dist)) {
				if (u >= 0 && v >= 0 && u + v <= 1) {
					if (pFlagExtendTriangle) {
						*pFlagExtendTriangle = sl_false;
					}
				} else {
					if (pFlagExtendTriangle) {
						*pFlagExtendTriangle = sl_true;
					}
				}
				return sl_true;
			} else {
				if (pFlagExtendTriangle) {
					*pFlagExtendTriangle = sl_false;
				}
				return sl_false;
			}
		}

		T dist, u, v;
		Vector3T<T> P = triangle.point2 - triangle.point1;
		Vector3T<T> Q = triangle.point3 - triangle.point1;
		Vector3T<T> D = line.getDirection() / L;
		Vector3T<T> S = line.point1;

		Vector3T<T> DQ = D.cross(Q);
		T det = DQ.dot(P);

		if (Math::isNearZero(det)) {
			if (pFlagParallel) {
				*pFlagParallel = sl_true;
			}
			if (pFlagExtendPoint1) {
				*pFlagExtendPoint1 = sl_false;
			}
			if (pFlagExtendPoint2) {
				*pFlagExtendPoint2 = sl_false;
			}
			T dist, u, v;
			Vector3T<T> P = projectPoint(line.point1, &dist, &u, &v);
			if (Math::isNearZero(dist)) {
				if (u >= 0 && v >= 0 && u + v <= 1) {
					if (pFlagExtendTriangle) {
						*pFlagExtendTriangle = sl_false;
					}
				} else {
					if (pFlagExtendTriangle) {
						*pFlagExtendTriangle = sl_true;
					}
				}
				return sl_true;
			} else {
				if (pFlagExtendTriangle) {
					*pFlagExtendTriangle = sl_false;
				}
				return sl_false;
			}
		}
		
		if (pFlagParallel) {
			*pFlagParallel = sl_false;
		}

		Vector3T<T> _T = S - triangle.point1;
		u = _T.dot(DQ) / det;
		Vector3T<T> TP = _T.cross(P);
		v = D.dot(TP) / det;
		dist = Q.dot(TP) / det;

		if (outIntersectPoint) {
			*outIntersectPoint = line.point1 + D * dist;
		}
		if (pDist) {
			*pDist = dist;
		}
		if (pU) {
			*pU = u;
		}
		if (pV) {
			*pV = v;
		}
		sl_bool flagIntersect = sl_true;
		if (dist > 0) {
			if (dist < L) {
				if (pFlagExtendPoint1) {
					*pFlagExtendPoint1 = sl_false;
				}
				if (pFlagExtendPoint2) {
					*pFlagExtendPoint2 = sl_false;
				}
			} else {
				if (pFlagExtendPoint1) {
					*pFlagExtendPoint1 = sl_false;
				}
				if (pFlagExtendPoint2) {
					*pFlagExtendPoint2 = sl_true;
				}
				flagIntersect = sl_false;
			}
		} else {
			if (pFlagExtendPoint1) {
				*pFlagExtendPoint1 = sl_true;
			}
			if (pFlagExtendPoint2) {
				*pFlagExtendPoint2 = sl_false;
			}
			flagIntersect = sl_false;
		}
		if (u >= 0 && v >= 0 && u + v <= 1) {
			if (pFlagExtendTriangle) {
				*pFlagExtendTriangle = sl_false;
			}
		} else {
			if (pFlagExtendTriangle) {
				*pFlagExtendTriangle = sl_true;
			}
			flagIntersect = sl_false;
		}
		return flagIntersect;
	}

	sl_bool intersectPlane(
		const PlaneT<T>& plane
		, Line3T<T>* outLine = sl_null, sl_uint32* outLineNo1 = sl_null, sl_uint32* outLineNo2 = sl_null)
	{
		Triangle3T<T>& triangle = *this;
		Line3T<T> line1(triangle.point1, triangle.point2);
		Line3T<T> line2(triangle.point2, triangle.point3);
		Line3T<T> line3(triangle.point3, triangle.point1);
		Vector3T<T> v1, v2;
		if (plane.intersectLine(line1, &v1)) {
			if (plane.intersectLine(line2, &v2)) {
				if (outLine) {
					outLine->point1 = v1;
					outLine->point2 = v2;
				}
				if (outLineNo1) {
					*outLineNo1 = 0;
				}
				if (outLineNo2) {
					*outLineNo2 = 1;
				}
				return sl_true;
			} else if (plane.intersectLine(line3, &v2)) {
				if (outLine) {
					outLine->point1 = v2;
					outLine->point2 = v1;
				}
				if (outLineNo1) {
					*outLineNo1 = 2;
				}
				if (outLineNo2) {
					*outLineNo2 = 0;
				}
				return sl_true;
			}
		} else if (plane.intersectLine(line2, &v1) ) {
			if (plane.intersectLine(line3, &v2)) {
				if (outLine) {
					outLine->point1 = v1;
					outLine->point2 = v2;
				}
				if (outLineNo1) {
					*outLineNo1 = 1;
				}
				if (outLineNo2) {
					*outLineNo2 = 2;
				}
				return sl_true;
			}
		}
		return sl_false;
	}

	sl_bool intersectTriangle(const Triangle3T<T>& triangle, Line3T<T>* outLine)
	{
		Triangle3T<T>& triangle1 = *this;
		Triangle3T<T>& triangle2 = triangle;
		Line3T<T> line11(triangle1.point1, triangle1.point2);
		Line3T<T> line12(triangle1.point2, triangle1.point3);
		Line3T<T> line13(triangle1.point3, triangle1.point1);
		Line3T<T> line21(triangle2.point1, triangle2.point2);
		Line3T<T> line22(triangle2.point2, triangle2.point3);
		Line3T<T> line23(triangle2.point3, triangle2.point1);
		Vector3T<T> v1, v2;
		sl_bool flagSuccess = sl_false;
		if (triangle2.intersectLine(line11, &v1)) {
			if (triangle2.intersectLine(line12, &v2)) {
				flagSuccess = sl_true;
			} else if (triangle2.intersectLine(line13, &v2)) {
				flagSuccess = sl_true;
			} else {
				if (triangle1.intersectLine(line21, &v2)) {
					flagSuccess = sl_true;
				} else if (triangle1.intersectLine(line22, &v2)) {
					flagSuccess = sl_true;
				} else if (triangle1.intersectLine(line23, &v2)) {
					flagSuccess = sl_true;
				}
			}
		} else if (triangle2.intersectLine(line12, &v1)) {
			if (triangle2.intersectLine(line13, &v2)) {
				flagSuccess = sl_true;
			} else {
				if (triangle1.intersectLine(line21, &v2)) {
					flagSuccess = sl_true;
				} else if (triangle1.intersectLine(line22, &v2)) {
					flagSuccess = sl_true;
				} else if (triangle1.intersectLine(line23, &v2)) {
					flagSuccess = sl_true;
				}
			}
		} else if (triangle2.intersectLine(line13, &v1)) {
			if (triangle1.intersectLine(line21, &v2)) {
				flagSuccess = sl_true;
			} else if (triangle1.intersectLine(line22, &v2)) {
				flagSuccess = sl_true;
			} else if (triangle1.intersectLine(line23, &v2)) {
				flagSuccess = sl_true;
			}
		} else {
			if (triangle1.intersectLine(line21, &v1)) {
				if (triangle1.intersectLine(line22, &v2)) {
					flagSuccess = sl_true;
				} else if (triangle1.intersectLine(line23, &v2)) {
					flagSuccess = sl_true;
				}
			} else if (triangle1.intersectLine(line22, &v1)) {
				if (triangle1.intersectLine(line23, &v2)) {
					flagSuccess = sl_true;
				}
			}
		}
		if (flagSuccess) {
			if (outLine) {
				outLine->point1 = v1;
				outLine->point2 = v2;
			}
			return sl_true;
		}
		return sl_false;
	}

};

typedef Triangle3T<sl_real> Triangle3;
typedef Triangle3T<float> Triangle3f;
typedef Triangle3T<double> Triangle3lf;

template <class T>
class SLIB_EXPORT SphereT
{
public:
	Vector3T<T> center;
	T radius;

	SLIB_INLINE SphereT() {}

	template <class O>
	SLIB_INLINE SphereT(const SphereT<O>& other)
	{
		center = other.center;
		radius = (T)(other.radius);
	}

	template <class O>
	SLIB_INLINE SphereT& operator=(const SphereT<O>& other)
	{
		center = other.center;
		radius = (T)(other.radius);
		return *this;
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

	sl_bool containsPoint(const Vector3T<T>& point)
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

template <class T>
class SLIB_EXPORT BoxT
{
public:
	T x1, y1, z1;
	T x2, y2, z2;

public:
	SLIB_INLINE BoxT() {}

	template <class O>
	SLIB_INLINE BoxT(const BoxT<O>& other)
	{
		x1 = (T)(other.x1); x2 = (T)(other.x2);
		y1 = (T)(other.y1); y2 = (T)(other.y2);
		z1 = (T)(other.z1); z2 = (T)(other.z2);
	}

	SLIB_INLINE BoxT(T _x1, T _y1, T _z1
		, T _x2, T _y2, T _z2)
	{
		x1 = _x1; y1 = _y1; z1 = _z1;
		x2 = _x2; y2 = _y2; z2 = _z2;
	}

	template <class O>
	SLIB_INLINE BoxT<T>& operator=(const BoxT<O>& other)
	{
		x1 = (T)(other.x1); x2 = (T)(other.x2);
		y1 = (T)(other.y1); y2 = (T)(other.y2);
		z1 = (T)(other.z1); z2 = (T)(other.z2);
		return *this;
	}

private:
	static T _zero[6];
public:
	static const BoxT<T>& zero()
	{
		return *((BoxT*)((void*)_zero));
	}

	SLIB_INLINE Vector3T<T> getStart() const
	{
		return Vector3T<T>(x1, y1, z1);
	}
	SLIB_INLINE void setStart(T x, T y, T z)
	{
		x1 = x;
		y1 = y;
		z1 = z;
	}
	SLIB_INLINE void setStart(const Vector3T<T>& v)
	{
		setStart(v.x, v.y, v.z);
	}

	SLIB_INLINE Vector3T<T> getEnd() const
	{
		return Vector3T<T>(x2, y2, z2);
	}
	SLIB_INLINE void setEnd(T x, T y, T z)
	{
		x2 = x;
		y2 = y;
		z2 = z;
	}
	SLIB_INLINE void setEnd(const Vector3T<T>& v)
	{
		setEnd(v.x, v.y, v.z);
	}

	SLIB_INLINE Vector3T<T> getSize() const
	{
		return Vector3T<T>(x2 - x1, y2 - y1, z2 - z1);
	}
	SLIB_INLINE Vector3T<T> size() const
	{
		return getSize();
	}

	SLIB_INLINE Vector3T<T> getCenter() const
	{
		return Vector3T<T>((x2 + x1) / 2, (y2 + y1) / 2, (z2 + z1) / 2);
	}
	SLIB_INLINE Vector3T<T> center() const
	{
		return getCenter();
	}

	SLIB_INLINE void setZero()
	{
		x1 = 0; y1 = 0; z1 = 0;
		x2 = 0; y2 = 0; z2 = 0;
	}

	SLIB_INLINE sl_bool containsPoint(T x, T y, T z) const
	{
		return x >= x1 && x <= x2
			&& y >= y1 && y <= y2
			&& z >= z1 && z <= z2;
	}
	SLIB_INLINE sl_bool containsPoint(const Vector3T<T>& pt) const
	{
		return pt.x >= x1 && pt.x <= x2
			&& pt.y >= y1 && pt.y <= y2
			&& pt.z >= z1 && pt.z <= z2;
	}

	SLIB_INLINE sl_bool containsBox(const BoxT<T>& other) const
	{
		return x1 <= other.x2 && x2 >= other.x1
			&& y1 <= other.y2 && y2 >= other.y1
			&& z1 <= other.z2 && z2 >= other.z1;
	}

	SLIB_INLINE void setFromPoint(T x, T y, T z)
	{
		x1 = x2 = x;
		y1 = y2 = y;
		z1 = z2 = z;
	}
	SLIB_INLINE void setFromPoint(const Vector3T<T>& pt)
	{
		setFromPoint(pt.x, pt.y, pt.z);
	}

	void mergePoint(T x, T y, T z)
	{
		if (x1 > x) {
			x1 = x;
		}
		if (x2 < x) {
			x2 = x;
		}
		if (y1 > y) {
			y1 = y;
		}
		if (y2 < y) {
			y2 = y;
		}
		if (z1 > z) {
			z1 = z;
		}
		if (z2 < z) {
			z2 = z;
		}
	}
	SLIB_INLINE void mergePoint(const Vector3T<T>& pt)
	{
		mergePoint(pt.x, pt.y, pt.z);
	}
	void mergePoints(const Vector3T<T>* points, sl_size count)
	{
		for (sl_size i = 0; i < count; i++) {
			const Vector3T<T>& v = points[i];
			if (x1 > v.x) {
				x1 = v.x;
			}
			if (x2 < v.x) {
				x2 = v.x;
			}
			if (y1 > v.y) {
				y1 = v.y;
			}
			if (y2 < v.y) {
				y2 = v.y;
			}
			if (z1 > v.z) {
				z1 = v.z;
			}
			if (z2 < v.z) {
				z2 = v.z;
			}
		}
	}
	void mergePoints(const List< Vector3T<T> >& points)
	{
		ListLocker< Vector3T<T> > list(points);
		mergePoints(list.getBuffer(), list.getCount());
	}

	void setFromPoints(const Vector3T<T>* points, sl_size count)
	{
		if (count > 0) {
			setFromPoint(points[0]);
			if (count > 1) {
				mergePoints(points + 1, count - 1);
			}
		}
	}
	void setFromPoints(const List< Vector3T<T> >& points)
	{
		ListLocker< Vector3T<T> > list(points);
		setFromPoints(list.getBuffer(), list.getCount());
	}
	SLIB_INLINE void setFromPoints(const Vector3T<T>& pt1, const Vector3T<T>& pt2)
	{
		setFromPoint(pt1);
		mergePoint(pt2);
	}

	SLIB_INLINE void mergeBox(const BoxT<T>& other)
	{
		if (x1 > other.x1) {
			x1 = other.x1;
		}
		if (x2 < other.x2) {
			x2 = other.x2;
		}
		if (y1 > other.y1) {
			y1 = other.y1;
		}
		if (y2 < other.y2) {
			y2 = other.y2;
		}
		if (z1 > other.z1) {
			z1 = other.z1;
		}
		if (z2 < other.z2) {
			z2 = other.z2;
		}
	}

	// 8 points
	void getCornerPoints(Vector3T<T>* out) const
	{
		out[0].x = x1; out[0].y = y1; out[0].z = z1;
		out[1].x = x2; out[1].y = y1; out[1].z = z1;
		out[2].x = x1; out[2].y = y2; out[2].z = z1;
		out[3].x = x2; out[3].y = y2; out[3].z = z1;
		out[4].x = x1; out[4].y = y1; out[4].z = z2;
		out[5].x = x2; out[5].y = y1; out[5].z = z2;
		out[6].x = x1; out[6].y = y2; out[6].z = z2;
		out[7].x = x2; out[7].y = y2; out[7].z = z2;
	}
};

template <class T>
T BoxT<T>::_zero[6] = {0, 0, 0, 0, 0, 0};

typedef BoxT<sl_real> Box;
typedef BoxT<float> Boxf;
typedef BoxT<double> Boxlf;

/*
	ViewFrustum defined on View Coordinate
*/
template <class T>
class SLIB_EXPORT ViewFrustumT
{
public:
	Matrix4T<T> MVP;
	
public:
	SLIB_INLINE ViewFrustumT()
	{
	}

	template <class O>
	SLIB_INLINE ViewFrustumT(const ViewFrustumT<O>& other) : MVP(other.MVP)
	{
	}

	template <class O>
	SLIB_INLINE ViewFrustumT<T>& operator=(const ViewFrustumT<O>& other)
	{
		MVP = other.MVP;
		return *this;
	}

	SLIB_INLINE static const ViewFrustumT<T>& fromMVP(const Matrix4T<T>& MVP)
	{
		return *((ViewFrustumT<T>*)(void*)&MVP);
	}

	void getPlanes(PlaneT<T>& near, PlaneT<T>& far, PlaneT<T>& left, PlaneT<T>& right, PlaneT<T>& top, PlaneT<T>& bottom) const
	{
		const Matrix4T<T>& m = MVP;
		left.a = m.m03 + m.m00;
		left.b = m.m13 + m.m10;
		left.c = m.m23 + m.m20;
		left.d = m.m33 + m.m30;
		right.a = m.m03 - m.m00;
		right.b = m.m13 - m.m10;
		right.c = m.m23 - m.m20;
		right.d = m.m33 - m.m30;
		bottom.a = m.m03 + m.m01;
		bottom.b = m.m13 + m.m11;
		bottom.c = m.m23 + m.m21;
		bottom.d = m.m33 + m.m31;
		top.a = m.m03 - m.m01;
		top.b = m.m13 - m.m11;
		top.c = m.m23 - m.m21;
		top.d = m.m33 - m.m31;
		near.a = m.m02;
		near.b = m.m12;
		near.c = m.m22;
		near.d = m.m32;
		far.a = m.m03 - m.m02;
		far.b = m.m13 - m.m12;
		far.c = m.m23 - m.m22;
		far.d = m.m33 - m.m32;
	}

	// get 6 planes
	SLIB_INLINE void getPlanes(PlaneT<T>* out) const
	{
		getPlanes(out[0], out[1], out[2], out[3], out[4], out[5]);
	}

	sl_bool containsPoint(const Vector3T<T>& pt, sl_bool flagSkipNearFar = sl_true) const
	{
		PlaneT<T> planes[6];
		getPlanes(planes);
		for (sl_uint32 i = flagSkipNearFar ? 2 : 0; i < 6; i++) {
			if (planes[i].getDistanceFromPointOnNormalized(pt) < 0) {
				return sl_false;
			}
		}
		return sl_true;
	}

	sl_bool containsFacets(const Vector3T<T>* pts, sl_uint32 n, sl_bool* pFlagIntersect = sl_null, sl_bool flagSkipNearFar = sl_true) const
	{
		PlaneT<T> planes[6];
		getPlanes(planes);
		sl_bool flagIntersect = sl_false;
		for (sl_uint32 i = flagSkipNearFar ? 2 : 0; i < 6; i++) {
			sl_uint32 nIn = 0;
			for (sl_uint32 k = 0; k < n; k++) {
				if (planes[i].getDistanceFromPointOnNormalized(pts[k]) >= 0) {
					nIn++;
				}
			}
			if (nIn == 0) {
				return sl_false;
			}
			if (nIn != n) {
				flagIntersect = sl_true;
			}
		}
		if (pFlagIntersect) {
			*pFlagIntersect = flagIntersect;
		}
		return sl_true;
	}

	sl_bool containsSphere(const SphereT<T>& sphere, sl_bool* pFlagIntersect = sl_null, sl_bool flagSkipNearFar = sl_true) const
	{
		PlaneT<T> planes[6];
		getPlanes(planes);
		sl_bool flagIntersect = sl_false;
		for (sl_uint32 i = flagSkipNearFar ? 2 : 0; i < 6; i++) {
			T d = planes[i].getDistanceFromPoint(sphere.center);
			if (d < -sphere.radius) {
				return sl_false;
			}
			if (d < sphere.radius) {
				flagIntersect = sl_true;
			}
		}
		if (pFlagIntersect) {
			*pFlagIntersect = flagIntersect;
		}
		return sl_true;
	}

	sl_bool containsBox(const BoxT<T>& box, sl_bool* pFlagIntersect = sl_null, sl_bool flagSkipNearFar = sl_true) const
	{
		Vector3T<T> corners[8];
		box.getCornerPoints(corners);
		return containsFacets(corners, 8, pFlagIntersect, flagSkipNearFar);
	}
};

typedef ViewFrustumT<sl_real> ViewFrustum;
typedef ViewFrustumT<float> ViewFrustumf;
typedef ViewFrustumT<double> ViewFrustumlf;

template <class T>
class SLIB_EXPORT Transform3T
{
public:
	static void setTranslation(Matrix4T<T>& out, T x, T y, T z)
	{
		out.m00 = 1; out.m01 = 0; out.m02 = 0; out.m03 = 0;
		out.m10 = 0; out.m11 = 1; out.m12 = 0; out.m13 = 0;
		out.m20 = 0; out.m21 = 0; out.m22 = 1; out.m23 = 0;
		out.m30 = x; out.m31 = y; out.m32 = z; out.m33 = 1;
	}
	SLIB_INLINE static void setTranslation(Matrix4T<T>& out, const Vector3T<T>& v)
	{
		setTranslation(out, v.x, v.y, v.z);
	}
	SLIB_INLINE static Matrix4T<T> getTranslationMatrix(T x, T y, T z)
	{
		Matrix4T<T> ret;
		setTranslation(ret, x, y, z);
		return ret;
	}
	SLIB_INLINE static Matrix4T<T> getTranslationMatrix(const Vector3T<T>& v)
	{
		Matrix4T<T> ret;
		setTranslation(ret, v);
		return ret;
	}

	static void setScaling(Matrix4T<T>& out, T sx, T sy, T sz)
	{
		out.m00 = sx; out.m01 = 0; out.m02 = 0; out.m03 = 0;
		out.m10 = 0; out.m11 = sy; out.m12 = 0; out.m13 = 0;
		out.m20 = 0; out.m21 = 0; out.m22 = sz; out.m23 = 0;
		out.m30 = 0; out.m31 = 0; out.m32 = 0; out.m33 = 1;
	}
	SLIB_INLINE static void setScaling(Matrix4T<T>& out, const Vector3T<T>& v)
	{
		setScaling(out, v.x, v.y, v.z);
	}
	SLIB_INLINE static Matrix4T<T> getScalingMatrix(T x, T y, T z)
	{
		Matrix4T<T> ret;
		setScaling(ret, x, y, z);
		return ret;
	}
	SLIB_INLINE static Matrix4T<T> getScalingMatrix(const Vector3T<T>& v)
	{
		Matrix4T<T> ret;
		setScaling(ret, v);
		return ret;
	}

	static void setRotationX(Matrix4T<T>& out, T radians)
	{
		T c = Math::cos(radians);
		T s = Math::sin(radians);
		out.m00 = 1; out.m01 = 0; out.m02 = 0; out.m03 = 0;
		out.m10 = 0; out.m11 = c; out.m12 = s; out.m13 = 0;
		out.m20 = 0; out.m21 = -s; out.m22 = c; out.m23 = 0;
		out.m30 = 0; out.m31 = 0; out.m32 = 0; out.m33 = 1;
	}
	static void setRotationY(Matrix4T<T>& out, T radians)
	{
		T c = Math::cos(radians);
		T s = Math::sin(radians);
		out.m00 = c; out.m01 = 0; out.m02 = -s; out.m03 = 0;
		out.m10 = 0; out.m11 = 1; out.m12 = 0; out.m13 = 0;
		out.m20 = s; out.m21 = 0; out.m22 = c; out.m23 = 0;
		out.m30 = 0; out.m31 = 0; out.m32 = 0; out.m33 = 1;
	}
	static void setRotationZ(Matrix4T<T>& out, T radians)
	{
		T c = Math::cos(radians);
		T s = Math::sin(radians);
		out.m00 = c; out.m01 = s; out.m02 = 0; out.m03 = 0;
		out.m10 = -s; out.m11 = c; out.m12 = 0; out.m13 = 0;
		out.m20 = 0; out.m21 = 0; out.m22 = 1; out.m23 = 0;
		out.m30 = 0; out.m31 = 0; out.m32 = 0; out.m33 = 1;
	}
	static void setRotation(Matrix4T<T>& out, const QuaternionT<T>& q)
	{
		T d = q.getLength2p();
		T s = 2 / d;
		T x = q.x * s, y = q.y * s, z = q.z * s;
		T wx = q.w * x, wy = q.w * y, wz = q.w * z;
		T xx = q.x * x, xy = q.x * y, xz = q.x * z;
		T yy = q.y * y, yz = q.y * z, zz = q.z * z;

		out.m00 = 1 - (yy + zz);
		out.m01 = xy - wz;
		out.m02 = xz + wy;
		out.m03 = 0;
		out.m10 = xy + wz;
		out.m11 = 1 - (xx + zz);
		out.m12 = yz - wx;
		out.m13 = 0;
		out.m20 = xz - wy;
		out.m21 = yz + wx;
		out.m22 = 1 - (xx + yy);
		out.m23 = 0;
		out.m30 = 0;
		out.m31 = 0;
		out.m32 = 0;
		out.m33 = 1;
	}
	static void setRotation(Matrix4T<T>& out, const Vector3T<T>& vAxis, T fAngle)
	{
		QuaternionT<T> q;
		q.setRotation(vAxis, fAngle);
		setRotation(out, q);
	}
	SLIB_INLINE static Matrix4T<T> getRotationXMatrix(T radians)
	{
		Matrix4T<T> ret;
		setRotationX(ret, radians);
		return ret;
	}
	SLIB_INLINE static Matrix4T<T> getRotationYMatrix(T radians)
	{
		Matrix4T<T> ret;
		setRotationY(ret, radians);
		return ret;
	}
	SLIB_INLINE static Matrix4T<T> getRotationZMatrix(T radians)
	{
		Matrix4T<T> ret;
		setRotationZ(ret, radians);
		return ret;
	}
	SLIB_INLINE static Matrix4T<T> getRotationMatrix(const QuaternionT<T>& q)
	{
		Matrix4T<T> ret;
		setRotation(ret, q);
		return ret;
	}
	SLIB_INLINE static Matrix4T<T> getRotationMatrix(const Vector3T<T>& vAxis, T fAngle)
	{
		Matrix4T<T> ret;
		setRotation(ret, vAxis, fAngle);
		return ret;
	}

	static void setPerspectiveProjectionFovY(Matrix4T<T>& out, T fovY, T fAspectWH, T zNear, T zFar)
	{
		T sy = Math::cot(fovY / 2);
		T sx = sy / fAspectWH;
		setPerspectiveProjection(out, sx, sy, zNear, zFar);
	}
	SLIB_INLINE static Matrix4T<T> getPerspectiveProjectionFovYMatrix(T fovY, T fAspectWH, T zNear, T zFar)
	{
		Matrix4T<T> ret;
		setPerspectiveProjectionFovY(ret, fovY, fAspectWH, zNear, zFar);
		return ret;
	}

	// Slib uses Left-Handed coordinate system
	static void setPerspectiveProjection(Matrix4T<T>& out, T sx, T sy, T zNear, T zFar)
	{
		out.m00 = sx; out.m01 = 0; out.m02 = 0; out.m03 = 0;
		out.m10 = 0; out.m11 = sy; out.m12 = 0; out.m13 = 0;
		out.m20 = 0; out.m21 = 0; out.m22 = zFar / (zFar - zNear); out.m23 = 1;
		out.m30 = 0; out.m31 = 0; out.m32 = -zNear * zFar / (zFar - zNear); out.m33 = 0;
	}
	SLIB_INLINE static Matrix4T<T> getPerspectiveProjectionMatrix(T sx, T sy, T zNear, T zFar)
	{
		Matrix4T<T> ret;
		setPerspectiveProjection(ret, sx, sy, zNear, zFar);
		return ret;
	}

	static void setOrthogonalProjection(Matrix4T<T>& out, T sx, T sy, T zNear, T zFar)
	{
		out.m00 = sx; out.m01 = 0; out.m02 = 0; out.m03 = 0;
		out.m10 = 0; out.m11 = sy; out.m12 = 0; out.m13 = 0;
		out.m20 = 0; out.m21 = 0; out.m22 = 1 / (zFar - zNear); out.m23 = 0;
		out.m30 = 0; out.m31 = 0; out.m32 = zNear / (zFar - zNear); out.m33 = 1;
	}
	SLIB_INLINE static Matrix4T<T> getOrthogonalProjectionMatrix(T sx, T sy, T zNear, T zFar)
	{
		Matrix4T<T> ret;
		setOrthogonalProjection(ret, sx, sy, zNear, zFar);
		return ret;
	}

	static void lookAt(Matrix4T<T>& out, const Vector3T<T>& eye, const Vector3T<T>& at, const Vector3T<T>& up)
	{
		Vector3T<T> xAxis, yAxis, zAxis;
		zAxis = (at - eye);
		zAxis.normalize();
		if (Math::isLessThanEpsilon(zAxis.length2p())) {
			zAxis = Vector3T<T>::axisZ();
		}
		xAxis = up.cross(zAxis);
		xAxis.normalize();
		if (Math::isLessThanEpsilon(xAxis.length2p())) {
			xAxis = Vector3T<T>::axisX();
		}
		yAxis = zAxis.cross(xAxis);
		if (Math::isLessThanEpsilon(yAxis.length2p())) {
			yAxis = Vector3T<T>::axisY();
		}
		out.m00 = xAxis.x; out.m01 = yAxis.x; out.m02 = zAxis.x; out.m03 = 0;
		out.m10 = xAxis.y; out.m11 = yAxis.y; out.m12 = zAxis.y; out.m13 = 0;
		out.m20 = xAxis.z; out.m21 = yAxis.z; out.m22 = zAxis.z; out.m23 = 0;
		out.m30 = -xAxis.dot(eye); out.m31 = -yAxis.dot(eye); out.m32 = -zAxis.dot(eye); out.m33 = 1;
	}
	SLIB_INLINE static Matrix4T<T> getLookAtMatrix(const Vector3T<T>& eye, const Vector3T<T>& at, const Vector3T<T>& up)
	{
		Matrix4T<T> ret;
		lookAt(ret, eye, at, up);
		return ret;
	}

	static void makeTransform(Matrix4T<T>& out, const Vector3T<T>& position, const Vector3T<T>& scaling, const QuaternionT<T>& rotation)
	{
		setRotation(out, rotation);
		out.m00 *= scaling.x; out.m01 *= scaling.x; out.m02 *= scaling.x;
		out.m10 *= scaling.y; out.m11 *= scaling.y; out.m12 *= scaling.y;
		out.m20 *= scaling.z; out.m21 *= scaling.z; out.m22 *= scaling.z;
		out.m30 = position.x; out.m31 = position.y; out.m32 *= position.z;
	}
	SLIB_INLINE static Matrix4T<T> getTransformMatrix(const Vector3T<T>& position, const Vector3T<T>& scaling, const QuaternionT<T>& rotation)
	{
		Matrix4T<T> ret;
		makeTransform(ret, position, scaling, rotation);
		return ret;
	}

	static void getRotationFromDirToDir(Vector3T<T>& outAxis, T& outAngle, const Vector3T<T>& from, const Vector3T<T>& to)
	{
		Vector3T<T> dirBefore = from;
		dirBefore.normalize();
		Vector3T<T> dirNext = to;
		dirNext.normalize();
		outAxis = dirBefore.cross(dirNext);
		outAngle = -Math::arccos(dirBefore.dot(dirNext));
	}

	SLIB_INLINE static void setQuaternionFromDirToDir(QuaternionT<T>& out, const Vector3T<T>& from, const Vector3T<T>& to)
	{
		Vector3T<T> dirAxisRotation;
		T angleRotation;
		getRotationFromDirToDir(dirAxisRotation, angleRotation, from, to);
		out.setRotation(dirAxisRotation, angleRotation);
	}
	SLIB_INLINE static QuaternionT<T> getQuaternionRotationFromDirToDir(const Vector3T<T>& from, const Vector3T<T>& to)
	{
		QuaternionT<T> ret;
		setQuaternionFromDirToDir(from, to);
		return ret;
	}

	SLIB_INLINE static void setTransformFromDirToDir(Matrix4T<T>& out, const Vector3T<T>& from, const Vector3T<T>& to)
	{
		QuaternionT<T> q;
		setQuaternionFromDirToDir(q, from, to);
		setRotation(out, q);
	}
	SLIB_INLINE static Matrix4T<T> getTransformMatrixFromDirToDir(const Vector3T<T>& from, const Vector3T<T>& to)
	{
		Matrix4T<T> ret;
		setTransformFromDirToDir(ret, from, to);
		return ret;
	}

	SLIB_INLINE static Vector3T<T> getTransformedOrigin(const Matrix4T<T>& transform)
	{
		return Vector3T<T>(transform.m30, transform.m31, transform.m32);
	}

	SLIB_INLINE static Vector3T<T> getTransformedAxisX(const Matrix4T<T>& transform)
	{
		return Vector3T<T>(transform.m00, transform.m01, transform.m02);
	}

	SLIB_INLINE static Vector3T<T> getTransformedAxisY(const Matrix4T<T>& transform)
	{
		return Vector3T<T>(transform.m10, transform.m11, transform.m12);
	}

	SLIB_INLINE static Vector3T<T> getTransformedAxisZ(const Matrix4T<T>& transform)
	{
		return Vector3T<T>(transform.m20, transform.m21, transform.m22);
	}

	static Vector3T<T> projectToViewport(const Matrix4T<T>& matViewProjection, const Vector3T<T>& point)
	{
		Vector4T<T> v = Vector4T<T>(point, 1) * matViewProjection;
		if (v.w >= 0 && Math::isLessThanEpsilon(v.w)) {
			Math::getEpsilon(&(v.w));
		}
		if (v.w <= 0 && Math::isLessThanEpsilon(-v.w)) {
			Math::getEpsilon(&(v.w));
			v.w = -v.w;
		}
		return Vector3T<T>(v.x / v.w, v.y / v.w, v.z / v.w);
	}

	static Line3T<T> unprojectViewportPoint(const Matrix4T<T>& matProjection, const Vector2T<T>& pt)
	{
		Vector4T<T> vTest1(1, 1, 1, 1);
		Vector4T<T> vTest2(1, 1, 2, 1);
		Vector4T<T> sTest1, sTest2;
		sTest1 = vTest1 * matProjection;
		sTest2 = vTest2 * matProjection;
		vTest1.x = pt.x / sTest1.x * sTest1.w;
		vTest1.y = pt.y / sTest1.y * sTest1.w;
		vTest2.x = pt.x / sTest2.x * sTest2.w;
		vTest2.y = pt.y / sTest2.y * sTest2.w;
		Line3T<T> ret(vTest1.xyz(), vTest2.xyz());
		return ret;
	}
	SLIB_INLINE static Line3T<T> unprojectScreenPoint(const Matrix4T<T>& matProjection, const Vector2T<T>& pt, T viewportWidth, T viewportHeight)
	{
		return unprojectViewportPoint(matProjection, Vector2T<T>((pt.x / viewportWidth * 2) - 1, 1 - (pt.y / viewportHeight * 2)));
	}
	SLIB_INLINE static Line3T<T> unprojectScreenPoint(const Matrix4T<T>& matProjection, const Vector2T<T>& pt, const RectangleT<T>& viewport)
	{
		return unprojectScreenPoint(matProjection, Vector2T<T>(pt.x - viewport.left, pt.y - viewport.top), viewport.getWidth(), viewport.getHeight());
	}

	SLIB_INLINE static Vector2T<T> convertViewportToScreen(const Vector2T<T>& ptViewport, const RectangleT<T>& viewport)
	{
		return Vector2T<T>(
			((viewport.left + viewport.right) + ptViewport.x * (viewport.right - viewport.left)) / 2
			, ((viewport.top + viewport.bottom) - ptViewport.y * (viewport.bottom - viewport.top)) / 2
			);
	}
	SLIB_INLINE static Vector2T<T> convertViewportToScreen(const Vector2T<T>& ptViewport, T viewportWidth, T viewportHeight)
	{
		return Vector2T<T>(
			(1 + ptViewport.x) * viewportWidth / 2
			, (1 - ptViewport.y) * viewportHeight / 2
			);
	}
	SLIB_INLINE static Vector2T<T> convertScreenToViewport(const Vector2T<T>& ptScreen, const RectangleT<T>& viewport)
	{
		return Vector2T<T>(
			(ptScreen.x - viewport.left) * 2 / (viewport.right - viewport.left) - 1
			, 1 - (ptScreen.y - viewport.top) * 2 / (viewport.bottom - viewport.top)
			);
	}
	SLIB_INLINE static Vector2T<T> convertScreenToViewport(const Vector2T<T>& ptScreen, T viewportWidth, T viewportHeight)
	{
		return Vector2T<T>(
			ptScreen.x * 2 / viewportWidth - 1
			, 1 - ptScreen.y * 2 / viewportHeight
			);
	}

	SLIB_INLINE static RectangleT<T> convertViewportToScreen(const RectangleT<T>& rcInViewport, const RectangleT<T>& viewport)
	{
		return RectangleT<T>(
			((viewport.left + viewport.right) + rcInViewport.left * (viewport.right - viewport.left)) / 2
			, ((viewport.top + viewport.bottom) - rcInViewport.bottom * (viewport.bottom - viewport.top)) / 2
			, ((viewport.left + viewport.right) + rcInViewport.right * (viewport.right - viewport.left)) / 2
			, ((viewport.top + viewport.bottom) - rcInViewport.top * (viewport.bottom - viewport.top)) / 2
			);
	}
	SLIB_INLINE static RectangleT<T> convertViewportToScreen(const RectangleT<T>& rcInViewport, T viewportWidth, T viewportHeight)
	{
		return RectangleT<T>(
			(1 + rcInViewport.left) * viewportWidth / 2
			, (1 - rcInViewport.bottom) * viewportHeight / 2
			, (1 + rcInViewport.right) * viewportWidth / 2
			, (1 - rcInViewport.top) * viewportHeight / 2
			);
	}
	SLIB_INLINE static RectangleT<T> convertScreenToViewport(const RectangleT<T>& rcInScreen, const RectangleT<T>& viewport)
	{
		return RectangleT<T>(
			(rcInScreen.left - viewport.left) * 2 / (viewport.right - viewport.left) - 1
			, 1 - (rcInScreen.bottom - viewport.top) * 2 / (viewport.bottom - viewport.top)
			, (rcInScreen.right - viewport.left) * 2 / (viewport.right - viewport.left) - 1
			, 1 - (rcInScreen.top - viewport.top) * 2 / (viewport.bottom - viewport.top)
			);
	}
	SLIB_INLINE static RectangleT<T> convertScreenToViewport(const RectangleT<T>& rcInScreen, T viewportWidth, T viewportHeight)
	{
		return RectangleT<T>(
			rcInScreen.left * 2 / viewportWidth - 1
			, 1 - rcInScreen.bottom * 2 / viewportHeight
			, rcInScreen.right * 2 / viewportWidth - 1
			, 1 - rcInScreen.top * 2 / viewportHeight
			);
	}
};

typedef Transform3T<sl_real> Transform3;
typedef Transform3T<float> Transform3f;
typedef Transform3T<double> Transform3lf;

SLIB_MATH_NAMESPACE_END

#endif
