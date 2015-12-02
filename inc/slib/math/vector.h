#ifndef CHECKHEADER_SLIB_MATH_VECTOR
#define CHECKHEADER_SLIB_MATH_VECTOR

#include "definition.h"
#include "../core/math.h"

SLIB_MATH_NAMESPACE_BEGIN

template <class T>
class SLIB_EXPORT Vector2T
{
public:
	union {
		T x;
		T width;
	};
	union {
		T y;
		T height;
	};

	SLIB_INLINE Vector2T()
	{
	}

	template <class O>
	SLIB_INLINE Vector2T(const Vector2T<O>& other)
	{
		x = (T)(other.x);
		y = (T)(other.y);
	}

	SLIB_INLINE Vector2T(T x, T y)
	{
		this->x = x;
		this->y = y;
	}

private:
	static T _zero[2];
public:
	static const Vector2T<T>& zero()
	{
		return *((Vector2T<T>*)((void*)(_zero)));
	}
	
public:
	template <class O>
	SLIB_INLINE Vector2T<T>& operator=(const Vector2T<O>& other)
	{
		x = (T)(other.x);
		y = (T)(other.y);
		return *this;
	}
	SLIB_INLINE Vector2T<T> operator+(const Vector2T<T>& other) const
	{
		return Vector2T<T>(x + other.x, y + other.y);
	}
	SLIB_INLINE Vector2T<T>& operator+=(const Vector2T<T>& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}
	SLIB_INLINE Vector2T<T> operator-(const Vector2T<T>& other) const
	{
		return Vector2T<T>(x - other.x, y - other.y);
	}
	SLIB_INLINE Vector2T<T>& operator-=(const Vector2T<T>& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}
	SLIB_INLINE Vector2T<T> operator*(T f) const
	{
		return Vector2T<T>(x * f, y * f);
	}
	SLIB_INLINE Vector2T<T>& operator*=(T f)
	{
		x *= f;
		y *= f;
		return *this;
	}
	SLIB_INLINE friend Vector2T<T> operator*(T f, const Vector2T<T>& v)
	{
		return Vector2T<T>(f * v.x, f * v.y);
	}
	SLIB_INLINE Vector2T<T> operator*(const Vector2T<T>& other) const
	{
		return Vector2T<T>(x * other.x, y * other.y);
	}
	SLIB_INLINE Vector2T<T>& operator*=(const Vector2T<T>& other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}
	SLIB_INLINE Vector2T<T> operator/(T f) const
	{
		return Vector2T<T>(x / f, y / f);
	}
	SLIB_INLINE Vector2T<T>& operator/=(T f)
	{
		x /= f;
		y /= f;
		return *this;
	}
	SLIB_INLINE friend Vector2T<T> operator/(T f, const Vector2T<T>& v)
	{
		return Vector2T<T>(f / v.x, f / v.y);
	}
	SLIB_INLINE Vector2T<T> operator/(const Vector2T<T>& other) const
	{
		return Vector2T<T>(x / other.x, y / other.y);
	}
	SLIB_INLINE Vector2T<T>& operator/(const Vector2T<T>& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}
	SLIB_INLINE Vector2T<T> operator-() const
	{
		return Vector2T<T>(-x, -y);
	}
	SLIB_INLINE sl_bool operator==(const Vector2T<T>& other) const
	{
		return (x == other.x && y == other.y);
	}
	SLIB_INLINE sl_bool operator!=(const Vector2T<T>& other) const
	{
		return !(*this == other);
	}

public:
	SLIB_INLINE T dot(const Vector2T<T>& other) const
	{
		return x * other.x + y * other.y;
	}
	SLIB_INLINE T cross(const Vector2T<T>& other) const
	{
		return x*other.y - y*other.x;
	}

	SLIB_INLINE T getLength2p() const
	{
		return x * x + y * y;
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

	SLIB_INLINE T getLength2p(const Vector2T<T>& other) const
	{
		T dx = x - other.x;
		T dy = y - other.y;
		return dx * dx + dy * dy;
	}
	SLIB_INLINE T getLength(const Vector2T<T>& other) const
	{
		return Math::sqrt(getLength2p(other));
	}

	void normalize()
	{
		T l = x * x + y * y;
		if (l > 0) {
			l = Math::sqrt(l);
			x /= l;
			y /= l;
		}
	}

	SLIB_INLINE Vector2T<T> getNormalized()
	{
		Vector2T<T> ret = *this;
		ret.normalize();
		return ret;
	}

	SLIB_INLINE T getCosBetween(const Vector2T& other) const
	{
		T ret = dot(other) / Math::sqrt(getLength2p() * other.getLength2p());
		if (ret > 1) {
			ret = 1;
		}
		return ret;
	}

	SLIB_INLINE T getAbsAngleBetween(const Vector2T& other) const
	{
		return Math::arccos(getCosBetween(other));
	}

	T getAngleBetween(const Vector2T& other) const
	{
		sl_real a = getAbsAngleBetween(other);
		if (cross(other) > 0) {
			a = -a;
		}
		return a;
	}
};

template <class T>
T Vector2T<T>::_zero[2] = {0, 0};

typedef Vector2T<sl_real> Vector2;
typedef Vector2T<float> Vector2f;
typedef Vector2T<double> Vector2lf;

template <class T>
class SLIB_EXPORT Vector3T
{
public:
	T x;
	T y;
	T z;

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
	static const Vector3T<T>& zero()
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

template <class T>
class SLIB_EXPORT Vector4T
{
public:
	T x;
	T y;
	T z;
	T w;
	
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
	static const Vector4T<T>& zero()
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
