#ifndef CHECKHEADER_SLIB_MATH_POINT
#define CHECKHEADER_SLIB_MATH_POINT

#include "definition.h"

#include "vector2.h"

SLIB_MATH_NAMESPACE_BEGIN

template <typename T>
using PointT = Vector2T<T>;

typedef PointT<sl_real> Point;
typedef PointT<sl_int32> Pointi;
typedef PointT<sl_int64> Pointli;
typedef PointT<float> Pointf;
typedef PointT<double> Pointlf;

SLIB_MATH_NAMESPACE_END

#endif
