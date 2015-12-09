#ifndef CHECKHEADER_SLIB_GRAPHICS_YUV
#define CHECKHEADER_SLIB_GRAPHICS_YUV

#include "definition.h"

#include "../core/math.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

#define _SLIB_IMAGE_COLOR_YUV_YG 18997 /* round(1.164 * 64 * 256 * 256 / 257) */
#define _SLIB_IMAGE_COLOR_YUV_YGB 1160 /* 1.164 * 64 * 16 - adjusted for even error distribution */
#define _SLIB_IMAGE_COLOR_YUV_UB -128 /* -min(128, round(2.018 * 64)) */
#define _SLIB_IMAGE_COLOR_YUV_UG 25 /* -round(-0.391 * 64) */
#define _SLIB_IMAGE_COLOR_YUV_VG 52 /* -round(-0.813 * 64) */
#define _SLIB_IMAGE_COLOR_YUV_VR -102 /* -round(1.596 * 64) */
#define _SLIB_IMAGE_COLOR_YUV_BB (_SLIB_IMAGE_COLOR_YUV_UB * 128 - _SLIB_IMAGE_COLOR_YUV_YGB)
#define _SLIB_IMAGE_COLOR_YUV_BG (_SLIB_IMAGE_COLOR_YUV_UG * 128 + _SLIB_IMAGE_COLOR_YUV_VG * 128 - _SLIB_IMAGE_COLOR_YUV_YGB)
#define _SLIB_IMAGE_COLOR_YUV_BR (_SLIB_IMAGE_COLOR_YUV_VR * 128 - _SLIB_IMAGE_COLOR_YUV_YGB)

class SLIB_EXPORT YUV
{
public:
	SLIB_INLINE static void convertRGBToYUV(sl_uint8 R, sl_uint8 G, sl_uint8 B, sl_uint8& Y, sl_uint8& U, sl_uint8& V)
	{
		sl_int32 _r = R;
		sl_int32 _g = G;
		sl_int32 _b = B;
		Y = (sl_uint8)(Math::clamp0_255((66 * _r + 129 * _g + 25 * _b + 0x1080) >> 8)); // y
		U = (sl_uint8)(Math::clamp0_255((112 * _b - 74 * _g - 38 * _r + 0x8080) >> 8)); // u
		V = (sl_uint8)(Math::clamp0_255((112 * _r - 94 * _g - 18 * _b + 0x8080) >> 8)); // v
	}
	
	SLIB_INLINE static void convertYUVToRGB(sl_uint8 Y, sl_uint8 U, sl_uint8 V, sl_uint8& R, sl_uint8& G, sl_uint8& B)
	{
		sl_int32 _y = Y;
		sl_int32 _u = U;
		sl_int32 _v = V;
		sl_int32 y1 = (sl_uint32)(_y * 0x0101 * _SLIB_IMAGE_COLOR_YUV_YG) >> 16;
		B = (sl_uint8)(Math::clamp0_255((sl_int32)(_SLIB_IMAGE_COLOR_YUV_BB - (_u * _SLIB_IMAGE_COLOR_YUV_UB) + y1) >> 6));
		G = (sl_uint8)(Math::clamp0_255((sl_int32)(_SLIB_IMAGE_COLOR_YUV_BG - (_v * _SLIB_IMAGE_COLOR_YUV_VG + _u * _SLIB_IMAGE_COLOR_YUV_UG) + y1) >> 6));
		R = (sl_uint8)(Math::clamp0_255((sl_int32)(_SLIB_IMAGE_COLOR_YUV_BR - (_v * _SLIB_IMAGE_COLOR_YUV_VR) + y1) >> 6));
	}
};

SLIB_GRAPHICS_NAMESPACE_END

#endif
