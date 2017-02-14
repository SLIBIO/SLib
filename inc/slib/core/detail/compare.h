#ifndef CHECKHEADER_SLIB_CORE_DETAIL_COMPARE
#define CHECKHEADER_SLIB_CORE_DETAIL_COMPARE

#include "../compare.h"

namespace slib
{

	template <class T1, class T2>
	SLIB_INLINE int Compare<T1, T2>::operator()(const T1& a, const T2& b) const
	{
		return (a < b) ? -1 : (a > b);
	}

	template <class T1, class T2>
	SLIB_INLINE int CompareDescending<T1, T2>::operator()(const T1& a, const T2& b) const
	{
		return -(Compare<T1, T2>::compare(a, b));
	}

	template <class T1, class T2>
	SLIB_INLINE sl_bool Equals<T1, T2>::operator()(const T1& a, const T2& b) const
	{
		return a == b;
	}

}

#endif
