#ifndef CHECKHEADER_SLIB_CORE_INTERPOLATION
#define CHECKHEADER_SLIB_CORE_INTERPOLATION

#include "definition.h"

namespace slib
{
	
	template <class TYPE>
	class SLIB_EXPORT Interpolation
	{
	public:
		static TYPE interpolate(const TYPE& a, const TYPE& b, float factor);

	};

}

#define SLIB_LERP(a, b, factor) (((a) * (1 - factor)) + ((b) * factor))

#include "detail/interpolation.h"

#endif
