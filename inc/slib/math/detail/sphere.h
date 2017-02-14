#ifndef CHECKHEADER_SLIB_MATH_DETAIL_SPHERE
#define CHECKHEADER_SLIB_MATH_DETAIL_SPHERE

#include "../sphere.h"

namespace slib
{
	
	template <class T>
	template <class O>
	SLIB_INLINE SphereT<T>::SphereT(const SphereT<O>& other) : center(other.center), radius((T)(other.radius))
	{
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE SphereT<T>& SphereT<T>::operator=(const SphereT<O>& other)
	{
		center = other.center;
		radius = (T)(other.radius);
		return *this;
	}

}

#endif
