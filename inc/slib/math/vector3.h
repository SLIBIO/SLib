#ifndef CHECKHEADER_SLIB_MATH_VECTOR3
#define CHECKHEADER_SLIB_MATH_VECTOR3

#include "definition.h"

#include "vector2.h"

#include "../core/math.h"

SLIB_MATH_NAMESPACE_BEGIN

template <class T>
class SLIB_EXPORT Vector3T
{
public:
	T x;
	T y;
	T z;

public:
	SLIB_INLINE Vector3T()
	{
	}

	template <class O>
	SLIB_INLINE Vector3T(const Vector3T<O>& other)
	{
		x = (T)(other.x);
		y = (T)(other.y);
		z = (T)(other.z);
	}

	SLIB_INLINE Vector3T(T x, T y, T z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	SLIB_INLINE Vector3T(const Vector2T<T>& xy, T z)
	{
		this->x = xy.x;
		this->y = xy.y;
		this->z = z;
	}

public:
	SLIB_INLINE Vector2T<T> xy() const
	{
		return Vector2T<T>(x, y);
	}
	
	SLIB_INLINE Vector2T<T> xz() const
	{
		return Vector2T<T>(x, z);
	}
	
	SLIB_INLINE Vector2T<T> yx() const
	{
		return Vector2T<T>(y, x);
	}
	
	SLIB_INLINE Vector2T<T> yz() const
	{
		return Vector2T<T>(y, z);
	}
	
	SLIB_INLINE Vector2T<T> zx() const
	{
		return Vector2T<T>(z, x);
	}
	
	SLIB_INLINE Vector2T<T> zy() const
	{
		return Vector2T<T>(z, y);
	}

private:
	static T _zero[3];
	
public:
	SLIB_INLINE static const Vector3T<T>& zero()
	{
		return *((Vector3T<T>*)((void*)(_zero)));
	}
	
	SLIB_INLINE static Vector3T<T> fromLocation(const Vector2T<T>& v)
	{
		return Vector3T<T>(v.x, v.y, 1);
	}
	
	SLIB_INLINE static Vector3T<T> fromDirection(const Vector2T<T>& v)
	{
		return Vector3T<T>(v.x, v.y, 0);
	}

public:
	template <class O>
	SLIB_INLINE Vector3T<T>& operator=(const Vector3T<O>& other)
	{
		x = (T)(other.x);
		y = (T)(other.y);
		z = (T)(other.z);
		return *this;
	}
	
	SLIB_INLINE Vector3T<T> operator+(const Vector3T<T>& other) const
	{
		return Vector3T<T>(x + other.x, y + other.y, z + other.z);
	}
	
	SLIB_INLINE Vector3T<T>& operator+=(const Vector3T<T>& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}
	
	SLIB_INLINE Vector3T<T> operator-(const Vector3T<T>& other) const
	{
		return Vector3T<T>(x - other.x, y - other.y, z - other.z);
	}
	
	SLIB_INLINE Vector3T<T>& operator-=(const Vector3T<T>& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}
	SLIB_INLINE Vector3T<T> operator*(T f) const
	{
		return Vector3T<T>(x * f, y * f, z * f);
	}
	
	SLIB_INLINE Vector3T<T>& operator*=(T f)
	{
		x *= f;
		y *= f;
		z *= f;
		return *this;
	}
	
	SLIB_INLINE friend Vector3T<T> operator*(T f, const Vector3T<T>& v)
	{
		return Vector3T<T>(f * v.x, f * v.y, f * v.z);
	}
	
	SLIB_INLINE Vector3T<T> operator*(const Vector3T<T>& other) const
	{
		return Vector3T<T>(x * other.x, y * other.y, z * other.z);
	}
	
	SLIB_INLINE Vector3T<T>& operator*=(const Vector3T<T>& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}
	
	SLIB_INLINE Vector3T<T> operator/(T f) const
	{
		return Vector3T<T>(x / f, y / f, z / f);
	}
	
	SLIB_INLINE Vector3T<T>& operator/=(T f)
	{
		x /= f;
		y /= f;
		z /= f;
		return *this;
	}
	
	SLIB_INLINE friend Vector3T<T> operator/(T f, const Vector3T<T>& v)
	{
		return Vector3T<T>(f / v.x, f / v.y, f / v.z);
	}
	
	SLIB_INLINE Vector3T<T> operator/(const Vector3T<T>& other) const
	{
		return Vector3T<T>(x / other.x, y / other.y, z / other.z);
	}
	
	SLIB_INLINE Vector3T<T>& operator/(const Vector3T<T>& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}
	
	SLIB_INLINE Vector3T<T> operator-() const
	{
		return Vector3T<T>(-x, -y, -z);
	}
	
	SLIB_INLINE sl_bool operator==(const Vector3T<T>& other) const
	{
		return (x == other.x && y == other.y && z == other.z);
	}
	
	SLIB_INLINE sl_bool operator!=(const Vector3T<T>& other) const
	{
		return !(*this == other);
	}

public:
	SLIB_INLINE T dot(const Vector3T<T>& other) const
	{
		return x * other.x + y * other.y + z * other.z;
	}

	Vector3T<T> cross(const Vector3T<T>& other) const
	{
		Vector3T<T> ret;
		ret.x = y*other.z - z*other.y;
		ret.y = z*other.x - x*other.z;
		ret.z = x*other.y - y*other.x;
		return ret;
	}

	SLIB_INLINE T getLength2p() const
	{
		return x * x + y * y + z * z;
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

	SLIB_INLINE T getLength2p(const Vector3T<T>& other) const
	{
		T dx = x - other.x;
		T dy = y - other.y;
		T dz = z - other.z;
		return dx * dx + dy * dy + dz * dz;
	}
	
	SLIB_INLINE T getLength(const Vector3T<T>& other) const
	{
		return Math::sqrt(getLength2p(other));
	}

	SLIB_INLINE void normalize()
	{
		T l = x * x + y * y + z * z;
		if (l > 0) {
			l = Math::sqrt(l);
			x /= l;
			y /= l;
			z /= l;
		}
	}

	SLIB_INLINE Vector3T<T> getNormalized()
	{
		Vector3T<T> ret = *this;
		ret.normalize();
		return ret;
	}

	T getCosBetween(const Vector3T<T>& other) const
	{
		return dot(other) / Math::sqrt(getLength2p() * other.getLength2p());
	}

	T getAngleBetween(const Vector3T<T>& other) const
	{
		return Math::arccos(dot(other) / Math::sqrt(getLength2p() * other.getLength2p()));
	}
};

template <class T>
T Vector3T<T>::_zero[3] = {0, 0, 0};

typedef Vector3T<sl_real> Vector3;
typedef Vector3T<float> Vector3f;
typedef Vector3T<double> Vector3lf;

SLIB_MATH_NAMESPACE_END

#endif
