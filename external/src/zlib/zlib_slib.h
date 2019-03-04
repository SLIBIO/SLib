#ifndef ZLIB_SLIB_SRC_H
#define ZLIB_SLIB_SRC_H

#include "../../include/zlib/zlib_slib.h"

#if defined(_MSC_VER)

#pragma optimize("t", on)
#pragma warning(disable: 4996)
#pragma warning(disable: 4267)

#elif defined(__GNUC__)

#pragma GCC optimize ("O3")
#ifdef __clang__
#pragma GCC diagnostic ignored "-Wshorten-64-to-32"
#pragma GCC diagnostic ignored "-Wcomma"
#endif

#endif

#endif
