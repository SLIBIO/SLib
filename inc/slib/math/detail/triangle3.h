#ifndef CHECKHEADER_SLIB_MATH_DETAIL_TRIANGLE3
#define CHECKHEADER_SLIB_MATH_DETAIL_TRIANGLE3

#include "../triangle3.h"

namespace slib
{
	
	template <class T>
	template <class O>
	Triangle3T<T>::Triangle3T(const Triangle3T<O>& other)
	 : point1(other.point1), point2(other.point2), point3(other.point3)
	{
	}
	
	template <class T>
	template <class O>
	Triangle3T<T>& Triangle3T<T>::operator=(const Triangle3T<O>& other)
	{
		point1 = other.point1;
		point2 = other.point2;
		point3 = other.point3;
		return *this;
	}

}

#endif
