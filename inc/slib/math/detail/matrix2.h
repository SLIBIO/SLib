#ifndef CHECKHEADER_SLIB_MATH_DETAIL_MATRIX2
#define CHECKHEADER_SLIB_MATH_DETAIL_MATRIX2

#include "../matrix2.h"

namespace slib
{

	template <class T>
	SLIB_INLINE const Matrix2T<T>& Matrix2T<T>::zero()
	{
		return *(reinterpret_cast<Matrix2T<T> const*>(&_zero));
	}
	
	template <class T>
	SLIB_INLINE const Matrix2T<T>& Matrix2T<T>::one()
	{
		return *(reinterpret_cast<Matrix2T<T> const*>(&_one));
	}
	
	template <class T>
	SLIB_INLINE const Matrix2T<T>& Matrix2T<T>::identity()
	{
		return *(reinterpret_cast<Matrix2T<T> const*>(&_identity));
	}
	
	template <class T>
	SLIB_INLINE const Matrix2T<T>& Matrix2T<T>::fromArray(const T arr[4])
	{
		return *(reinterpret_cast<Matrix2T<T> const*>(arr));
	}
	
	template <class T>
	SLIB_INLINE Matrix2T<T>& Matrix2T<T>::fromArray(T arr[4])
	{
		return *(reinterpret_cast<Matrix2T<T>*>(arr));
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Matrix2T<T>& Matrix2T<T>::operator=(const Matrix2T<O>& other)
	{
		m00 = (T)(other.m00); m01 = (T)(other.m01);
		m10 = (T)(other.m10); m11 = (T)(other.m11);
		return *this;
	}
	
	template <class T>
	Matrix2T<T> operator*(T value, const Matrix2T<T>& m)
	{
		Matrix2T<T> ret(m);
		ret.multiply(value);
		return ret;
	}
	
	template <class T>
	Vector2T<T> operator*(const Vector2T<T>& v, const Matrix2T<T>& m)
	{
		return m.multiplyLeft(v);
	}
	
	
	template <class T>
	SLIB_INLINE Matrix2T<T> Interpolation< Matrix2T<T> >::interpolate(const Matrix2T<T>& a, const Matrix2T<T>& b, float factor)
	{
		return a.lerp(b, factor);
	}

}

#endif
