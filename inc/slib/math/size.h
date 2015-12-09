#ifndef CHECKHEADER_SLIB_MATH_SIZE
#define CHECKHEADER_SLIB_MATH_SIZE

#include "definition.h"

#include "vector2.h"

SLIB_MATH_NAMESPACE_BEGIN

template <typename T>
using SizeT = Vector2T<T>;

typedef SizeT<sl_real> Size;
typedef SizeT<sl_int32> Sizei;
typedef SizeT<sl_int64> Sizeli;
typedef SizeT<float> Sizef;
typedef SizeT<double> Sizelf;

SLIB_MATH_NAMESPACE_END

#endif
