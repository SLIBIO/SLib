#ifndef CHECKHEADER_SLIB_CORE_INTERPOLATION
#define CHECKHEADER_SLIB_CORE_INTERPOLATION

#include "definition.h"

SLIB_NAMESPACE_BEGIN

template <class TYPE>
class SLIB_EXPORT Interpolation
{
public:
	static TYPE interpolate(const TYPE& a, const TYPE& b, float factor);
	
};

#define SLIB_LERP(a, b, factor) (((a) * (1 - factor)) + ((b) * factor))


template <class TYPE>
SLIB_INLINE TYPE Interpolation<TYPE>::interpolate(const TYPE& a, const TYPE& b, float factor)
{
	return SLIB_LERP(a, b, factor);
}

SLIB_NAMESPACE_END

#endif
