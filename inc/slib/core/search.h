#ifndef CHECKHEADER_SLIB_CORE_SEARCH
#define CHECKHEADER_SLIB_CORE_SEARCH

#include "definition.h"

namespace slib
{

	class SLIB_EXPORT BinarySearch
	{
	public:
		template < class T1, class T2, class COMPARE=Compare<T1, T2> >
		static sl_bool search(const T1* list, sl_size size, const T2& what, sl_size* pIndexPrev = sl_null, const COMPARE& compare = COMPARE());

	};

}

#include "detail/search.h"

#endif
