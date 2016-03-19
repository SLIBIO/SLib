#ifndef CHECKHEADER_SLIB_MATH_VECTOR4
#define CHECKHEADER_SLIB_MATH_VECTOR4

#include "definition.h"

#include "vector3.h"

SLIB_MATH_NAMESPACE_BEGIN

template <class T, class FT = T>
class SLIB_EXPORT Vector4T
{
public:
	T x;
	T y;
	T z;
	T w;
	
public:
	Vector4T() = default;
	
	Vector4T(const Vector4T<T, FT>& other) = default;

	template <class O, class FO>
	Vector4T(const Vector4T<O, FO>& other);

	Vector4T(T x, T y, T z, T w);

	Vector4T(const Vector3T<T, FT>& xyz, T w);
	
public:
	static const Vector4T<T, FT>& zero();
	
	const Vector3T<T, FT>& xyz() const;
	
	Vector3T<T, FT>& xyz();
	
	static Vector4T<T, FT> fromLocation(const Vector3T<T, FT>& v);
	
	static Vector4T<T, FT> fromDirection(const Vector3T<T, FT>& v);
	
	T dot(const Vector4T<T, FT>& other) const;
	
	T getLength2p() const;
	
	FT getLength() const;
	
	T getLength2p(const Vector4T<T, FT>& other) const;
	
	FT getLength(const Vector4T<T, FT>& other) const;
	
	void normalize();
	
	Vector4T<T, FT> getNormalized();
	
	FT getCosBetween(const Vector4T<T, FT>& other) const;
	
	FT getAngleBetween(const Vector4T<T, FT>& other) const;
	
public:
	Vector4T<T, FT>& operator=(const Vector4T<T, FT>& other) = default;
	
	template <class O, class FO>
	Vector4T<T, FT>& operator=(const Vector4T<O, FO>& other);
	
	Vector4T<T, FT> operator+(const Vector4T<T, FT>& other) const;
	
	Vector4T<T, FT>& operator+=(const Vector4T<T, FT>& other);
	
	Vector4T<T, FT> operator-(const Vector4T<T, FT>& other) const;
	
	Vector4T<T, FT>& operator-=(const Vector4T<T, FT>& other);
	
	Vector4T<T, FT> operator*(T f) const;
	
	Vector4T<T, FT>& operator*=(T f);
	
	Vector4T<T, FT> operator*(const Vector4T<T, FT>& other) const;
	
	Vector4T<T, FT>& operator*=(const Vector4T<T, FT>& other);
	
	Vector4T<T, FT> operator/(T f) const;
	
	Vector4T<T, FT>& operator/=(T f);
	
	Vector4T<T, FT> operator/(const Vector4T<T, FT>& other) const;
	
	Vector4T<T, FT>& operator/(const Vector4T<T, FT>& other);
	
	Vector4T<T, FT> operator-() const;
	
	sl_bool operator==(const Vector4T<T, FT>& other) const;
	
	sl_bool operator!=(const Vector4T<T, FT>& other) const;
	
private:
	static T _zero[4];
	
};

SLIB_DECLARE_GEOMETRY_TYPE_EX(Vector4)

template <class T, class FT>
Vector4T<T, FT> operator*(T f, const Vector4T<T, FT>& v);

template <class T, class FT>
Vector4T<T, FT> operator/(T f, const Vector4T<T, FT>& v);


SLIB_MATH_NAMESPACE_END


SLIB_MATH_NAMESPACE_BEGIN

template <class T, class FT>
template <class O, class FO>
SLIB_INLINE Vector4T<T, FT>::Vector4T(const Vector4T<O, FO>& other) : x((T)(other.x)), y(other.y), z(other.z), w(other.w)
{
}

template <class T, class FT>
SLIB_INLINE Vector4T<T, FT>::Vector4T(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w)
{
}

template <class T, class FT>
SLIB_INLINE Vector4T<T, FT>::Vector4T(const Vector3T<T, FT>& xyz, T _w) : x(xyz.x), y(xyz.y), z(xyz.z), w(_w)
{
}

template <class T, class FT>
template <class O, class FO>
SLIB_INLINE Vector4T<T, FT>& Vector4T<T, FT>::operator=(const Vector4T<O, FO>& other)
{
	x = (T)(other.x);
	y = (T)(other.y);
	z = (T)(other.z);
	w = (T)(other.w);
	return *this;
}

template <class T, class FT>
SLIB_INLINE const Vector4T<T, FT>& Vector4T<T, FT>::zero()
{
	return *((Vector4T<T, FT>*)((void*)(_zero)));
}

template <class T, class FT>
SLIB_INLINE const Vector3T<T, FT>& Vector4T<T, FT>::xyz() const
{
	return *((const Vector3T<T, FT>*)((const void*)(this)));
}

template <class T, class FT>
SLIB_INLINE Vector3T<T, FT>& Vector4T<T, FT>::xyz()
{
	return *((Vector3T<T, FT>*)((void*)(this)));
}

template <class T, class FT>
Vector4T<T, FT> operator*(T f, const Vector4T<T, FT>& v)
{
	return {f * v.x, f * v.y, f * v.z, f * v.w};
}

template <class T, class FT>
Vector4T<T, FT> operator/(T f, const Vector4T<T, FT>& v)
{
	return {f / v.x, f / v.y, f / v.z, f / v.w};
}

SLIB_MATH_NAMESPACE_END

#endif
