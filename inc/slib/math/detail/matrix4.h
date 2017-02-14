#ifndef CHECKHEADER_SLIB_MATH_DETAIL_MATRIX4
#define CHECKHEADER_SLIB_MATH_DETAIL_MATRIX4

#include "../matrix4.h"

namespace slib
{
	
	template <class T>
	SLIB_INLINE const Matrix4T<T>& Matrix4T<T>::zero()
	{
		return *(reinterpret_cast<Matrix4T<T> const*>(&_zero));
	}
	
	template <class T>
	SLIB_INLINE const Matrix4T<T>& Matrix4T<T>::one()
	{
		return *(reinterpret_cast<Matrix4T<T> const*>(&_one));
	}
	
	template <class T>
	SLIB_INLINE const Matrix4T<T>& Matrix4T<T>::identity()
	{
		return *(reinterpret_cast<Matrix4T<T> const*>(&_identity));
	}
	
	template <class T>
	SLIB_INLINE const Matrix4T<T>& Matrix4T<T>::fromArray(const T arr[16])
	{
		return *(reinterpret_cast<Matrix4T<T> const*>(arr));
	}
	
	template <class T>
	SLIB_INLINE Matrix4T<T>& Matrix4T<T>::fromArray(T arr[16])
	{
		return *(reinterpret_cast<Matrix4T<T>*>(arr));
	}
	
	template <class T>
	template <class O>
	Matrix4T<T>& Matrix4T<T>::operator=(const Matrix4T<O>& other)
	{
		m00 = (T)(other.m00); m01 = (T)(other.m01); m02 = (T)(other.m02); m03 = (T)(other.m03);
		m10 = (T)(other.m10); m11 = (T)(other.m11); m12 = (T)(other.m12); m13 = (T)(other.m13);
		m20 = (T)(other.m20); m21 = (T)(other.m21); m22 = (T)(other.m22); m23 = (T)(other.m23);
		m30 = (T)(other.m30); m31 = (T)(other.m31); m32 = (T)(other.m32); m33 = (T)(other.m33);
		return *this;
	}
	
	template <class T>
	Matrix4T<T> operator*(T value, const Matrix4T<T>& m)
	{
		Matrix4T<T> ret(m);
		ret.multiply(value);
		return ret;
	}
	
	template <class T>
	Vector4T<T> operator*(const Vector4T<T>& v, const Matrix4T<T>& m)
	{
		return m.multiplyLeft(v);
	}
	
	
	template <class T>
	SLIB_INLINE Matrix4T<T> Interpolation< Matrix4T<T> >::interpolate(const Matrix4T<T>& a, const Matrix4T<T>& b, float factor)
	{
		return a.lerp(b, factor);
	}

}

#endif
