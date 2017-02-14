#ifndef CHECKHEADER_SLIB_MATH_DETAIL_LINE
#define CHECKHEADER_SLIB_MATH_DETAIL_LINE

#include "../line.h"

namespace slib
{
	
	template <class T>
	template <class O>
	SLIB_INLINE LineT<T>& LineT<T>::operator=(const LineT<O>& other)
	{
		a = (T)(other.a);
		b = (T)(other.b);
		c = (T)(other.c);
		return *this;
	}

}

#endif
