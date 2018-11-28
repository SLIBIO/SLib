/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/graphics/yuv.h"

#include "slib/core/math.h"

#define YUV_YG 18997 /* round(1.164 * 64 * 256 * 256 / 257) */
#define YUV_YGB 1160 /* 1.164 * 64 * 16 - adjusted for even error distribution */
#define YUV_UB -128 /* -min(128, round(2.018 * 64)) */
#define YUV_UG 25 /* -round(-0.391 * 64) */
#define YUV_VG 52 /* -round(-0.813 * 64) */
#define YUV_VR -102 /* -round(1.596 * 64) */
#define YUV_BB (YUV_UB * 128 - YUV_YGB)
#define YUV_BG (YUV_UG * 128 + YUV_VG * 128 - YUV_YGB)
#define YUV_BR (YUV_VR * 128 - YUV_YGB)

namespace slib
{

	void YUV::convertRGBToYUV(sl_uint8 R, sl_uint8 G, sl_uint8 B, sl_uint8& Y, sl_uint8& U, sl_uint8& V)
	{
		sl_int32 _r = R;
		sl_int32 _g = G;
		sl_int32 _b = B;
		Y = (sl_uint8)(Math::clamp0_255((66 * _r + 129 * _g + 25 * _b + 0x1080) >> 8)); // y
		U = (sl_uint8)(Math::clamp0_255((112 * _b - 74 * _g - 38 * _r + 0x8080) >> 8)); // u
		V = (sl_uint8)(Math::clamp0_255((112 * _r - 94 * _g - 18 * _b + 0x8080) >> 8)); // v
	}

	void YUV::convertYUVToRGB(sl_uint8 Y, sl_uint8 U, sl_uint8 V, sl_uint8& R, sl_uint8& G, sl_uint8& B)
	{
		sl_int32 _y = Y;
		sl_int32 _u = U;
		sl_int32 _v = V;
		sl_int32 y1 = (sl_uint32)(_y * 0x0101 * YUV_YG) >> 16;
		B = (sl_uint8)(Math::clamp0_255((sl_int32)(YUV_BB - (_u * YUV_UB) + y1) >> 6));
		G = (sl_uint8)(Math::clamp0_255((sl_int32)(YUV_BG - (_v * YUV_VG + _u * YUV_UG) + y1) >> 6));
		R = (sl_uint8)(Math::clamp0_255((sl_int32)(YUV_BR - (_v * YUV_VR) + y1) >> 6));
	}

}
