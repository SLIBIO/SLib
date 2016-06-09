#ifndef CHECKHEADER_SLIB_MATH_VECTOR3
#define CHECKHEADER_SLIB_MATH_VECTOR3

#include "definition.h"

#include "vector2.h"

SLIB_MATH_NAMESPACE_BEGIN

template <class T, class FT = T>
class SLIB_EXPORT Vector3T
{
public:
	T x;
	T y;
	T z;

public:
	Vector3T() = default;
	
	Vector3T(const Vector3T<T, FT>& other) = default;

	template <class O, class FO>
	Vector3T(const Vector3T<O, FO>& other);

	Vector3T(T x, T y, T z);
	
public:
	static const Vector3T<T, FT>& zero();
	
	static Vector3T<T, FT> fromLocation(const Vector2T<T>& v);
	
	static Vector3T<T, FT> fromDirection(const Vector2T<T>& v);
	
	T dot(const Vector3T<T, FT>& other) const;
	
	Vector3T<T, FT> cross(const Vector3T<T, FT>& other) const;
	
	T getLength2p() const;
	
	FT getLength() const;
	
	T getLength2p(const Vector3T<T, FT>& other) const;
	
	FT getLength(const Vector3T<T, FT>& other) const;
	
	void normalize();
	
	Vector3T<T, FT> getNormalized();
	
	FT getCosBetween(const Vector3T<T, FT>& other) const;
	
	FT getAngleBetween(const Vector3T<T, FT>& other) const;
	
	sl_bool equals(const Vector3T<T, FT>& other) const;
	
	sl_bool isAlmostEqual(const Vector3T<T, FT>& other) const;

public:
	Vector3T<T, FT>& operator=(const Vector3T<T, FT>& other) = default;
	
	template <class O, class FO>
	Vector3T<T, FT>& operator=(const Vector3T<O, FO>& other);
	
	Vector3T<T, FT> operator+(const Vector3T<T, FT>& other) const;
	
	Vector3T<T, FT>& operator+=(const Vector3T<T, FT>& other);
	
	Vector3T<T, FT> operator-(const Vector3T<T, FT>& other) const;
	
	Vector3T<T, FT>& operator-=(const Vector3T<T, FT>& other);
	
	Vector3T<T, FT> operator*(T f) const;
	
	Vector3T<T, FT>& operator*=(T f);
	
	Vector3T<T, FT> operator*(const Vector3T<T, FT>& other) const;
	
	Vector3T<T, FT>& operator*=(const Vector3T<T, FT>& other);
	
	Vector3T<T, FT> operator/(T f) const;
	
	Vector3T<T, FT>& operator/=(T f);
	
	Vector3T<T, FT> operator/(const Vector3T<T, FT>& other) const;
	
	Vector3T<T, FT>& operator/(const Vector3T<T, FT>& other);
	
	Vector3T<T, FT> operator-() const;
	
	sl_bool operator==(const Vector3T<T, FT>& other) const;
	
	sl_bool operator!=(const Vector3T<T, FT>& other) const;
	
private:
	static T _zero[3];
	
};

SLIB_DECLARE_GEOMETRY_TYPE_EX(Vector3)

template <class T, class FT>
Vector3T<T, FT> operator*(T f, const Vector3T<T, FT>& v);

template <class T, class FT>
Vector3T<T, FT> operator/(T f, const Vector3T<T, FT>& v);


SLIB_MATH_NAMESPACE_END


SLIB_MATH_NAMESPACE_BEGIN

template <class T, class FT>
template <class O, class FO>
SLIB_INLINE Vector3T<T, FT>::Vector3T(const Vector3T<O, FO>& other) : x((T)(other.x)), y((T)(other.y)), z((T)(other.z))
{
}

template <class T, class FT>
SLIB_INLINE Vector3T<T, FT>::Vector3T(T _x, T _y, T _z) : x(_x), y(_y), z(_z)
{
}

template <class T, class FT>
template <class O, class FO>
SLIB_INLINE Vector3T<T, FT>& Vector3T<T, FT>::operator=(const Vector3T<O, FO>& other)
{
	x = (T)(other.x);
	y = (T)(other.y);
	z = (T)(other.z);
	return *this;
}

template <class T, class FT>
SLIB_INLINE const Vector3T<T, FT>& Vector3T<T, FT>::zero()
{
	return *((Vector3T<T, FT>*)((void*)(_zero)));
}

template <class T, class FT>
Vector3T<T, FT> operator*(T f, const Vector3T<T, FT>& v)
{
	return {f * v.x, f * v.y, f * v.z};
}

template <class T, class FT>
Vector3T<T, FT> operator/(T f, const Vector3T<T, FT>& v)
{
	return {f / v.x, f / v.y, f / v.z};
}

SLIB_MATH_NAMESPACE_END

#endif
