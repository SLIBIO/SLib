#ifndef PNG_SLIB_SRC_H
#define PNG_SLIB_SRC_H

#include <libpng/png_slib.h>

#if defined(_MSC_VER)

#pragma optimize("t", on)
#pragma warning(disable: 4996)

#elif defined(__GNUC__)

#pragma GCC optimize ("O3")

#endif

#endif