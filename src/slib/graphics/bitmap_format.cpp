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

#include "slib/graphics/bitmap_format.h"

namespace slib
{

	ColorSpace BitmapFormats::getColorSpace(BitmapFormat format)
	{
		return (ColorSpace)(((sl_uint32)format) >> 28);
	}

	sl_bool BitmapFormats::hasAlpha(BitmapFormat format)
	{
		return (((sl_uint32)format) >> 27) & 1;
	}

	sl_bool BitmapFormats::isPrecomputedAlpha(BitmapFormat format)
	{
		return (((sl_uint32)format) >> 26) & 1;
	}

	sl_uint32 BitmapFormats::getPlanesCount(BitmapFormat format)
	{
		return (((sl_uint32)format) >> 24) & 3;
	}

	sl_uint32 BitmapFormats::getBitsPerSample(BitmapFormat format)
	{
		return (((sl_uint32)format) >> 16) & 255;
	}

	sl_uint32 BitmapFormats::getBytesPerSample(BitmapFormat format)
	{
		return (((sl_uint32)format) >> 19) & 31;
	}

	sl_uint32 BitmapFormats::getBitsPerPixel(BitmapFormat format)
	{
		return (((sl_uint32)format) >> 8) & 255;
	}

	sl_uint32 BitmapFormats::getBytesPerPixel(BitmapFormat format)
	{
		return (((sl_uint32)format) >> 11) & 31;
	}

	sl_bool BitmapFormats::isYUV_420(BitmapFormat format)
	{
		return (((sl_uint32)format) & 0xF0) == 0xB0;
	}

	BitmapFormat BitmapFormats::getNonPrecomputedAlphaFormat(BitmapFormat format)
	{
		switch (format) {
			case BitmapFormat::RGBA_PA:
				return BitmapFormat::RGBA;
			case BitmapFormat::BGRA_PA:
				return BitmapFormat::BGRA;
			case BitmapFormat::ARGB_PA:
				return BitmapFormat::ARGB;
			case BitmapFormat::ABGR_PA:
				return BitmapFormat::ABGR;
			case BitmapFormat::RGBA_PLANAR_PA:
				return BitmapFormat::RGBA_PLANAR;
			case BitmapFormat::YUVA_PA:
				return BitmapFormat::YUVA;
			case BitmapFormat::YUVA_PLANAR_PA:
				return BitmapFormat::YUVA_PLANAR;
			default:
				break;
		}
		return format;
	}

	BitmapFormat BitmapFormats::getPrecomputedAlphaFormat(BitmapFormat format)
	{
		switch (format) {
			case BitmapFormat::RGBA:
				return BitmapFormat::RGBA_PA;
			case BitmapFormat::BGRA:
				return BitmapFormat::BGRA_PA;
			case BitmapFormat::ARGB:
				return BitmapFormat::ARGB_PA;
			case BitmapFormat::ABGR:
				return BitmapFormat::ABGR_PA;
			case BitmapFormat::RGBA_PLANAR:
				return BitmapFormat::RGBA_PLANAR_PA;
			case BitmapFormat::YUVA:
				return BitmapFormat::YUVA_PA;
			case BitmapFormat::YUVA_PLANAR:
				return BitmapFormat::YUVA_PLANAR_PA;
			default:
				break;
		}
		return format;
	}

	BitmapFormat BitmapFormats::getCompatibleRGBFormat(BitmapFormat format)
	{
		switch (format) {
			case BitmapFormat::YUVA:
				return BitmapFormat::RGBA;
			case BitmapFormat::YUVA_PLANAR:
				return BitmapFormat::RGBA_PLANAR;
			case BitmapFormat::YUVA_PA:
				return BitmapFormat::RGBA_PA;
			case BitmapFormat::YUVA_PLANAR_PA:
				return BitmapFormat::RGBA_PLANAR_PA;
			case BitmapFormat::YUV444:
				return BitmapFormat::RGB;
			case BitmapFormat::YUV444_PLANAR:
				return BitmapFormat::RGB_PLANAR;
			default:
				break;
		}
		return BitmapFormat::None;
	}

}
