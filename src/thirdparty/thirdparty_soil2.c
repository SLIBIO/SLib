#include "../../inc/slib/graphics/config.h"

#ifdef SLIB_GRAPHICS_IMAGE_USE_SOIL2

#if defined(_MSC_VER)
#define inline __inline
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "../../thirdparty/SOIL2/etc1_utils.c"
#include "../../thirdparty/SOIL2/image_helper.c"
#include "../../thirdparty/SOIL2/image_DXT.c"
#include "../../thirdparty/SOIL2/SOIL2.c"

#endif