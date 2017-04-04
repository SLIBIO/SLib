#ifdef _MSC_VER
#pragma warning(disable : 4996)
#endif

#include "src/png.c"
#include "src/pngerror.c"
#include "src/pngget.c"
#include "src/pngmem.c"
#include "src/pngpread.c"
#include "src/pngread.c"
#include "src/pngrio.c"
#include "src/pngrtran.c"
#include "src/pngrutil.c"
#include "src/pngset.c"
#include "src/pngtrans.c"
#include "src/pngwio.c"
#include "src/pngwrite.c"
#include "src/pngwutil.c"
#include "src/pngwtran.c"

#if defined(__arm__) || defined(__aarch64__) || defined(__arm64__)
#include "src/arm/arm_init.c"
#include "src/arm/filter_neon_intrinsics.c"
#endif
