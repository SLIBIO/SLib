#ifndef CHECKHEADER_SLIB_CORE_COMPARE
#define CHECKHEADER_SLIB_CORE_COMPARE

#include "definition.h"

SLIB_NAMESPACE_BEGIN

/*************************************************************************
	Compare<TYPE>::compare(v1, v2) returns
		negative: v1 < v2
		0		: v1 = v2
		positive: v1 > v2
*************************************************************************/

template <class TYPE1, class TYPE2=TYPE1>
class SLIB_EXPORT Compare
{
public:
	static int compare(const TYPE1& a, const TYPE2& b);

	static sl_bool equals(const TYPE1& a, const TYPE2& b);
	
};

template <class TYPE1, class TYPE2=TYPE1>
class SLIB_EXPORT CompareDescending
{
public:
	static int compare(const TYPE1& a, const TYPE2& b);
	
	static sl_bool eqauls(const TYPE1& a, const TYPE2& b);
	
};


template <class TYPE1, class TYPE2>
SLIB_INLINE int Compare<TYPE1, TYPE2>::compare(const TYPE1& a, const TYPE2& b)
{
	return (a < b) ? -1 : (a > b);
}

template <class TYPE1, class TYPE2>
SLIB_INLINE sl_bool Compare<TYPE1, TYPE2>::equals(const TYPE1& a, const TYPE2& b)
{
	return a == b;
}

template <class TYPE1, class TYPE2>
SLIB_INLINE int CompareDescending<TYPE1, TYPE2>::compare(const TYPE1& a, const TYPE2& b)
{
	return - (Compare<TYPE1, TYPE2>::compare(a, b));
}

template <class TYPE1, class TYPE2>
SLIB_INLINE sl_bool CompareDescending<TYPE1, TYPE2>::eqauls(const TYPE1& a, const TYPE2& b)
{
	return a == b;
}

SLIB_NAMESPACE_END

#endif
