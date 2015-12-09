#ifndef CHECKHEADER_SLIB_MATH_VECTOR2
#define CHECKHEADER_SLIB_MATH_VECTOR2

#include "definition.h"

#include "../core/math.h"

SLIB_MATH_NAMESPACE_BEGIN

template <class T>
class SLIB_EXPORT Vector2T
{
public:
	T x;
	T y;

public:
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
	SLIB_INLINE static const Vector2T<T>& zero()
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

SLIB_MATH_NAMESPACE_END

#endif
