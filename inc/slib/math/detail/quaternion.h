#ifndef CHECKHEADER_SLIB_MATH_DETAIL_QUATERNION
#define CHECKHEADER_SLIB_MATH_DETAIL_QUATERNION

#include "../quaternion.h"

namespace slib
{

	template <class T>
	SLIB_INLINE const QuaternionT<T>& QuaternionT<T>::identity()
	{
		return *(reinterpret_cast<QuaternionT<T> const*>(&_identity));
	}

	template <class T>
	SLIB_INLINE const QuaternionT<T>& QuaternionT<T>::fromArray(const T arr[2])
	{
		return *(reinterpret_cast<QuaternionT<T> const*>(arr));
	}
	
	template <class T>
	SLIB_INLINE QuaternionT<T>& QuaternionT<T>::fromArray(T arr[2])
	{
		return *(reinterpret_cast<QuaternionT<T>*>(arr));
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

}

#endif
