#ifndef CHECKHEADER_SLIB_MATH_DETAIL_LINE3
#define CHECKHEADER_SLIB_MATH_DETAIL_LINE3

#include "../line3.h"

namespace slib
{

	template <class T>
	template <class O>
	SLIB_INLINE Line3T<T>::Line3T(const Line3T<O>& other)
	: point1(other.point1), point2(other.point2)
	{
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE Line3T<T>& Line3T<T>::operator=(const Line3T<O>& other)
	{
		point1 = other.point1;
		point2 = other.point2;
		return *this;
	}

}

#endif
