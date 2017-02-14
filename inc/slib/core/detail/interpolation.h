#ifndef CHECKHEADER_SLIB_CORE_DETAIL_INTERPOLATION
#define CHECKHEADER_SLIB_CORE_DETAIL_INTERPOLATION

#include "../interpolation.h"

namespace slib
{
	
	template <class TYPE>
	SLIB_INLINE TYPE Interpolation<TYPE>::interpolate(const TYPE& a, const TYPE& b, float factor)
	{
		return SLIB_LERP(a, b, factor);
	}

}

#endif
