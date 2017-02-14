#ifndef CHECKHEADER_SLIB_MATH_DETAIL_MATRIX3
#define CHECKHEADER_SLIB_MATH_DETAIL_MATRIX3

#include "../matrix3.h"

namespace slib
{
	
	template <class T>
	SLIB_INLINE const Matrix3T<T>& Matrix3T<T>::zero()
	{
		return *(reinterpret_cast<Matrix3T<T> const*>(&_zero));
	}
	
	template <class T>
	SLIB_INLINE const Matrix3T<T>& Matrix3T<T>::one()
	{
		return *(reinterpret_cast<Matrix3T<T> const*>(&_one));
	}
	
	template <class T>
	SLIB_INLINE const Matrix3T<T>& Matrix3T<T>::identity()
	{
		return *(reinterpret_cast<Matrix3T<T> const*>(&_identity));
	}
	
	template <class T>
	SLIB_INLINE const Matrix3T<T>& Matrix3T<T>::fromArray(const T arr[9])
	{
		return *(reinterpret_cast<Matrix3T<T> const*>(arr));
	}
	
	template <class T>
	SLIB_INLINE Matrix3T<T>& Matrix3T<T>::fromArray(T arr[9])
	{
		return *(reinterpret_cast<Matrix3T<T>*>(arr));
	}
	
	template <class T>
	template <class O>
	Matrix3T<T>& Matrix3T<T>::operator=(const Matrix3T<O>& other)
	{
		m00 = (T)(other.m00); m01 = (T)(other.m01); m02 = (T)(other.m02);
		m10 = (T)(other.m10); m11 = (T)(other.m11); m12 = (T)(other.m12);
		m20 = (T)(other.m20); m21 = (T)(other.m21); m22 = (T)(other.m22);
		return *this;
	}
	
	
	template <class T>
	Matrix3T<T> operator*(T value, const Matrix3T<T>& m)
	{
		Matrix3T<T> ret(m);
		ret.multiply(value);
		return ret;
	}
	
	template <class T>
	Vector3T<T> operator*(const Vector3T<T>& v, const Matrix3T<T>& m)
	{
		return m.multiplyLeft(v);
	}
	
	
	template <class T>
	SLIB_INLINE Matrix3T<T> Interpolation< Matrix3T<T> >::interpolate(const Matrix3T<T>& a, const Matrix3T<T>& b, float factor)
	{
		return a.lerp(b, factor);
	}

}

#endif
