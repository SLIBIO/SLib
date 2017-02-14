#ifndef CHECKHEADER_SLIB_CORE_COMPARE
#define CHECKHEADER_SLIB_CORE_COMPARE

#include "definition.h"

namespace slib
{

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

}

#include "detail/compare.h"

#endif
