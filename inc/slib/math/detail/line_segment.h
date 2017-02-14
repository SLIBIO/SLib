#ifndef CHECKHEADER_SLIB_MATH_DETAIL_LINE_SEGMENT
#define CHECKHEADER_SLIB_MATH_DETAIL_LINE_SEGMENT

#include "../line_segment.h"

namespace slib
{
	
	template <class T>
	template <class O>
	SLIB_INLINE LineSegmentT<T>::LineSegmentT(const LineSegmentT<O>& other)
	: point1(other.point1), point2(other.point2)
	{
	}
	
	template <class T>
	template <class O>
	SLIB_INLINE LineSegmentT<T>& LineSegmentT<T>::operator=(const LineSegmentT<O>& other)
	{
		point1 = other.point1;
		point2 = other.point2;
		return *this;
	}

}

#endif
