#ifndef CHECKHEADER_SLIB_MATH_SIZE
#define CHECKHEADER_SLIB_MATH_SIZE

#include "definition.h"

#include "vector2.h"

namespace slib
{

	template <typename T, typename FT = T>
	using SizeT = Vector2T<T, FT>;

	typedef SizeT<sl_real> Size;
	typedef SizeT<sl_int32, float> Sizei;
	typedef SizeT<sl_int64, double> Sizeli;
	typedef SizeT<float> Sizef;
	typedef SizeT<double> Sizelf;

}

#endif
