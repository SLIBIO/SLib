#ifndef CHECKHEADER_SLIB_MATH_QUATERNION
#define CHECKHEADER_SLIB_MATH_QUATERNION

#include "definition.h"

#include "vector4.h"

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
	QuaternionT() = default;
	
	QuaternionT(const QuaternionT<T>& other) = default;
	
	template <class O>
	QuaternionT(const QuaternionT<O>& other);

	QuaternionT(T x, T y, T z, T w);
	
	QuaternionT(const Vector4T<T>& other);
	
public:
	static const QuaternionT<T>& identity();
	
	const Vector4T<T>& toVector4() const;
	
	Vector4T<T>& toVector4();
	
	T getLength2p() const;
	
	T getLength() const;
	
	void multiply(const QuaternionT<T>& other);
	
	void divide(const QuaternionT<T>& other);
	
	void setRotation(const Vector3T<T>& vAxis, T fAngle);
	
	static QuaternionT<T> getRotation(const Vector3T<T>& vAxis, T fAngle);
	
	T getAngle() const;
	
	Vector4T<T> getAxis() const;
	
	void makeInverse();
	
	QuaternionT<T> inverse() const;
	
public:
	QuaternionT<T>& operator=(const QuaternionT<T>& other) = default;
	
	template <class O>
	QuaternionT<T>& operator=(const QuaternionT<O>& other);
	
	QuaternionT<T>& operator=(const Vector4T<T>& other);
	
	QuaternionT<T>& operator*=(const QuaternionT<T>& other);
	
	QuaternionT<T> operator*(const QuaternionT<T>& other) const;
	
	QuaternionT<T>& operator/=(const QuaternionT<T>& other);
	
	QuaternionT<T> operator/(QuaternionT<T>& other) const;
	
	sl_bool operator==(const QuaternionT<T>& other) const;
	
	sl_bool operator!=(const QuaternionT<T>& other) const;
	
private:
	static T _identity[4];
	
};

SLIB_DECLARE_GEOMETRY_TYPE(Quaternion)

SLIB_MATH_NAMESPACE_END


SLIB_MATH_NAMESPACE_BEGIN

template <class T>
template <class O>
SLIB_INLINE QuaternionT<T>::QuaternionT(const QuaternionT<O>& other)
: x((T)(other.x)), y((T)(other.y)), z((T)(other.z)), w((T)(other.w))
{
}

template <class T>
SLIB_INLINE QuaternionT<T>::QuaternionT(T _x, T _y, T _z, T _w)
: x(_x), y(_y), z(_z), w(_w)
{
}

template <class T>
SLIB_INLINE QuaternionT<T>::QuaternionT(const Vector4T<T>& other)
: x(other.x), y(other.y), z(other.z), w(other.w)
{
}

template <class T>
SLIB_INLINE const QuaternionT<T>& QuaternionT<T>::identity()
{
	return *((QuaternionT<T>*)((void*)_identity));
}

template <class T>
SLIB_INLINE const Vector4T<T>& QuaternionT<T>::toVector4() const
{
	return *((Vector4T<T>*)((void*)this));
}

template <class T>
SLIB_INLINE Vector4T<T>& QuaternionT<T>::toVector4()
{
	return *((Vector4T<T>*)((void*)this));
}

template <class T>
template <class O>
SLIB_INLINE QuaternionT<T>& QuaternionT<T>::operator=(const QuaternionT<O>& other)
{
	x = (T)(other.x);
	y = (T)(other.y);
	z = (T)(other.z);
	w = (T)(other.w);
	return *this;
}

template <class T>
SLIB_INLINE QuaternionT<T>& QuaternionT<T>::operator=(const Vector4T<T>& other)
{
	x = other.x;
	y = other.y;
	z = other.z;
	w = other.w;
	return *this;
}

SLIB_MATH_NAMESPACE_END

#endif
