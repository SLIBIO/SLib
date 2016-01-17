#include "../../../inc/slib/graphics/bitmap_format.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

const BitmapFormatValue BitmapFormat::_null = bitmapFormat_Null;

ColorSpace BitmapFormat::getColorSpace() const
{
	return (ColorSpace)(m_format >> 28);
}

sl_bool BitmapFormat::hasAlpha() const
{
	return (m_format >> 27) & 1;
}

sl_bool BitmapFormat::isPrecomputedAlpha() const
{
	return (m_format >> 26) & 1;
}

sl_uint32 BitmapFormat::getPlanesCount() const
{
	return (m_format >> 24) & 3;
}

sl_uint32 BitmapFormat::getBitsPerSample() const
{
	return (m_format >> 16) & 255;
}

sl_uint32 BitmapFormat::getBytesPerSample() const
{
	return (m_format >> 19) & 31;
}

sl_uint32 BitmapFormat::getBitsPerPixel() const
{
	return (m_format >> 8) & 255;
}

sl_uint32 BitmapFormat::getBytesPerPixel() const
{
	return (m_format >> 11) & 31;
}

sl_bool BitmapFormat::isYUV_420() const
{
	return (m_format & 0xF0) == 0xB0;
}

BitmapFormat BitmapFormat::getNonPrecomputedAlphaFormat() const
{
	switch (m_format) {
		case bitmapFormat_RGBA_PA:
			return bitmapFormat_RGBA;
		case bitmapFormat_BGRA_PA:
			return bitmapFormat_BGRA;
		case bitmapFormat_ARGB_PA:
			return bitmapFormat_ARGB;
		case bitmapFormat_ABGR_PA:
			return bitmapFormat_ABGR;
		case bitmapFormat_RGBA_PLANAR_PA:
			return bitmapFormat_RGBA_PLANAR;
		case bitmapFormat_YUVA_PA:
			return bitmapFormat_YUVA;
		case bitmapFormat_YUVA_PLANAR_PA:
			return bitmapFormat_YUVA_PLANAR;
		default:
			break;
	}
	return *this;
}

BitmapFormat BitmapFormat::getPrecomputedAlphaFormat() const
{
	switch (m_format) {
		case bitmapFormat_RGBA:
			return bitmapFormat_RGBA_PA;
		case bitmapFormat_BGRA:
			return bitmapFormat_BGRA_PA;
		case bitmapFormat_ARGB:
			return bitmapFormat_ARGB_PA;
		case bitmapFormat_ABGR:
			return bitmapFormat_ABGR_PA;
		case bitmapFormat_RGBA_PLANAR:
			return bitmapFormat_RGBA_PLANAR_PA;
		case bitmapFormat_YUVA:
			return bitmapFormat_YUVA_PA;
		case bitmapFormat_YUVA_PLANAR:
			return bitmapFormat_YUVA_PLANAR_PA;
		default:
			break;
	}
	return *this;
}

BitmapFormat BitmapFormat::getCompatibleRGBFormat() const
{
	switch (m_format) {
		case bitmapFormat_YUVA:
			return bitmapFormat_RGBA;
		case bitmapFormat_YUVA_PLANAR:
			return bitmapFormat_RGBA_PLANAR;
		case bitmapFormat_YUVA_PA:
			return bitmapFormat_RGBA_PA;
		case bitmapFormat_YUVA_PLANAR_PA:
			return bitmapFormat_RGBA_PLANAR_PA;
		case bitmapFormat_YUV444:
			return bitmapFormat_RGB;
		case bitmapFormat_YUV444_PLANAR:
			return bitmapFormat_RGB_PLANAR;
		default:
			break;
	}
	return bitmapFormat_Null;

}
SLIB_GRAPHICS_NAMESPACE_END

