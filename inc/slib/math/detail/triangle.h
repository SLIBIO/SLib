#ifndef CHECKHEADER_SLIB_MATH_DETAIL_TRIANGLE
#define CHECKHEADER_SLIB_MATH_DETAIL_TRIANGLE

#include "../triangle.h"

namespace slib
{

	template <class T>
	template <class O>
	TriangleT<T>::TriangleT(const TriangleT<O>& other) : point1(other.point1), point2(other.point2), point3(other.point3)
	{
	}
	
	template <class T>
	template <class O>
	TriangleT<T>& TriangleT<T>::operator=(const TriangleT<O>& other)
	{
		point1 = other.point1;
		point2 = other.point2;
		point3 = other.point3;
		return *this;
	}

}

#endif
