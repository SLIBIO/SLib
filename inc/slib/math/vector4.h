#ifndef CHECKHEADER_SLIB_MATH_VECTOR4
#define CHECKHEADER_SLIB_MATH_VECTOR4

#include "definition.h"

#include "vector3.h"

#include "../core/math.h"

SLIB_MATH_NAMESPACE_BEGIN

template <class T>
class SLIB_EXPORT Vector4T
{
public:
	T x;
	T y;
	T z;
	T w;
	
public:
	SLIB_INLINE Vector4T()
	{
	}

	template <class O>
	SLIB_INLINE Vector4T(const Vector4T<O>& other)
	{
		x = (T)(other.x);
		y = (T)(other.y);
		z = (T)(other.z);
		w = (T)(other.w);
	}

	SLIB_INLINE Vector4T(T x, T y, T z, T w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	SLIB_INLINE Vector4T(const Vector3T<T>& xyz, T w)
	{
		this->x = xyz.x;
		this->y = xyz.y;
		this->z = xyz.z;
		this->w = w;
	}

public:
	SLIB_INLINE Vector3T<T> xyz() const
	{
		return Vector3T<T>(x, y, z);
	}

private:
	static T _zero[4];
	
public:
	SLIB_INLINE static const Vector4T<T>& zero()
	{
		return *((Vector4T<T>*)((void*)(_zero)));
	}
	
	SLIB_INLINE static Vector4T<T> fromLocation(const Vector3& v)
	{
		return Vector4T<T>(v.x, v.y, v.z, 1);
	}
	
	SLIB_INLINE static Vector4T<T> fromDirection(const Vector3& v)
	{
		return Vector4T<T>(v.x, v.y, v.z, 0);
	}


public:
	template <class O>
	SLIB_INLINE Vector4T<T>& operator=(const Vector4T<O>& other)
	{
		x = (T)(other.x);
		y = (T)(other.y);
		z = (T)(other.z);
		w = (T)(other.w);
		return *this;
	}
	
	SLIB_INLINE Vector4T<T> operator+(const Vector4T<T>& other) const
	{
		return Vector4T<T>(x + other.x, y + other.y, z + other.z, w + other.w);
	}
	
	SLIB_INLINE Vector4T<T>& operator+=(const Vector4T<T>& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}
	
	SLIB_INLINE Vector4T<T> operator-(const Vector4T<T>& other) const
	{
		return Vector4T<T>(x - other.x, y - other.y, z - other.z, w - other.w);
	}
	
	SLIB_INLINE Vector4T<T>& operator-=(const Vector4T<T>& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}
	
	SLIB_INLINE Vector4T<T> operator*(T f) const
	{
		return Vector4T<T>(x * f, y * f, z * f, w * f);
	}
	
	SLIB_INLINE Vector4T<T>& operator*=(T f)
	{
		x *= f;
		y *= f;
		z *= f;
		w *= f;
		return *this;
	}
	
	SLIB_INLINE friend Vector4T<T> operator*(T f, const Vector4T<T>& v)
	{
		return Vector4T<T>(f * v.x, f * v.y, f * v.z, f * v.w);
	}
	
	SLIB_INLINE Vector4T<T> operator*(const Vector4T<T>& other) const
	{
		return Vector4T<T>(x * other.x, y * other.y, z * other.z, w * other.w);
	}
	
	SLIB_INLINE Vector4T<T>& operator*=(const Vector4T<T>& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		w *= other.w;
		return *this;
	}
	
	SLIB_INLINE Vector4T<T> operator/(T f) const
	{
		return Vector4T<T>(x / f, y / f, z / f, w / f);
	}
	
	SLIB_INLINE Vector4T<T>& operator/=(T f)
	{
		x /= f;
		y /= f;
		z /= f;
		w /= f;
		return *this;
	}
	
	SLIB_INLINE friend Vector4T<T> operator/(T f, const Vector4T<T>& v)
	{
		return Vector4T<T>(f / v.x, f / v.y, f / v.z, f / v.w);
	}
	
	SLIB_INLINE Vector4T<T> operator/(const Vector4T<T>& other) const
	{
		return Vector4T<T>(x / other.x, y / other.y, z / other.z, w / other.w);
	}
	
	SLIB_INLINE Vector4T<T>& operator/(const Vector4T<T>& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		w /= other.w;
		return *this;
	}
	
	SLIB_INLINE Vector4T<T> operator-() const
	{
		return Vector4T<T>(-x, -y, -z, -w);
	}
	
	SLIB_INLINE sl_bool operator==(const Vector4T<T>& other) const
	{
		return (x == other.x && y == other.y && z == other.z && w == other.w);
	}
	
	SLIB_INLINE sl_bool operator!=(const Vector4T<T>& other) const
	{
		return !(*this == other);
	}

public:
	SLIB_INLINE T dot(const Vector4T<T>& other) const
	{
		return x * other.x + y * other.y + z * other.z + w * other.w;
	}

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

	SLIB_INLINE T getLength2p(const Vector4T<T>& other) const
	{
		T dx = x - other.x;
		T dy = y - other.y;
		T dz = z - other.z;
		T dw = w - other.w;
		return dx * dx + dy * dy + dz * dz + dw * dw;
	}
	
	SLIB_INLINE T getLength(const Vector4T<T>& other) const
	{
		return Math::sqrt(getLength2p(other));
	}

	void normalize()
	{
		T l = x * x + y * y + z * z + w * w;
		if (l > 0) {
			l = Math::sqrt(l);
			x /= l;
			y /= l;
			z /= l;
			w /= l;
		}
	}

	SLIB_INLINE Vector4T<T> getNormalized()
	{
		Vector4T<T> ret = *this;
		ret.normalize();
		return ret;
	}

	T getCosBetween(const Vector4T<T>& other) const
	{
		return dot(other) / Math::sqrt(getLength2p() * other.getLength2p());
	}

	T getAngleBetween(const Vector4T<T>& other) const
	{
		return Math::arccos(dot(other) / Math::sqrt(getLength2p() * other.getLength2p()));
	}
};

template <class T>
T Vector4T<T>::_zero[4] = {0, 0, 0, 0};

typedef Vector4T<sl_real> Vector4;
typedef Vector4T<float> Vector4f;
typedef Vector4T<double> Vector4lf;

SLIB_MATH_NAMESPACE_END

#endif
