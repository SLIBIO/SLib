#ifndef CHECKHEADER_SLIB_CORE_COMPARE
#define CHECKHEADER_SLIB_CORE_COMPARE

#include "definition.h"

SLIB_NAMESPACE_BEGIN

template <class T1, class T2 = T1>
class SLIB_EXPORT Compare
{
public:
	int operator()(const T1& a, const T2& b) const;
};

template <class T1, class T2 = T1>
class SLIB_EXPORT CompareDescending
{
public:
	int operator()(const T1& a, const T2& b) const;
};

template <class T1, class T2 = T1>
class SLIB_EXPORT Equals
{
public:
	sl_bool operator()(const T1& a, const T2& b) const;
};


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

SLIB_NAMESPACE_END

#endif
