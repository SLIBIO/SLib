#include "../../../inc/slib/graphics/bitmap_format.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

BitmapFormatValue BitmapFormat::_null = bitmapFormatNull;

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
	return (m_format >> 19) & 255;
}

sl_uint32 BitmapFormat::getBitsPerPixel() const
{
	return (m_format >> 8) & 255;
}

sl_uint32 BitmapFormat::getBytesPerPixel() const
{
	return (m_format >> 11) & 255;
}

sl_bool BitmapFormat::isYUV_420() const
{
	return (m_format & 0xF0) == 0xB0;
}

BitmapFormat BitmapFormat::getNonPrecomputedAlphaFormat() const
{
	switch (m_format) {
		case bitmapFormatRGBA_PA:
			return bitmapFormatRGBA;
		case bitmapFormatBGRA_PA:
			return bitmapFormatBGRA;
		case bitmapFormatARGB_PA:
			return bitmapFormatARGB;
		case bitmapFormatABGR_PA:
			return bitmapFormatABGR;
		case bitmapFormatRGBA_PLANAR_PA:
			return bitmapFormatRGBA_PLANAR;
		case bitmapFormatYUVA_PA:
			return bitmapFormatYUVA;
		case bitmapFormatYUVA_PLANAR_PA:
			return bitmapFormatYUVA_PLANAR;
		default:
			break;
	}
	return *this;
}

BitmapFormat BitmapFormat::getPrecomputedAlphaFormat() const
{
	switch (m_format) {
		case bitmapFormatRGBA:
			return bitmapFormatRGBA_PA;
		case bitmapFormatBGRA:
			return bitmapFormatBGRA_PA;
		case bitmapFormatARGB:
			return bitmapFormatARGB_PA;
		case bitmapFormatABGR:
			return bitmapFormatABGR_PA;
		case bitmapFormatRGBA_PLANAR:
			return bitmapFormatRGBA_PLANAR_PA;
		case bitmapFormatYUVA:
			return bitmapFormatYUVA_PA;
		case bitmapFormatYUVA_PLANAR:
			return bitmapFormatYUVA_PLANAR_PA;
		default:
			break;
	}
	return *this;
}

BitmapFormat BitmapFormat::getCompatibleRGBFormat() const
{
	switch (m_format) {
		case bitmapFormatYUVA:
			return bitmapFormatRGBA;
		case bitmapFormatYUVA_PLANAR:
			return bitmapFormatRGBA_PLANAR;
		case bitmapFormatYUVA_PA:
			return bitmapFormatRGBA_PA;
		case bitmapFormatYUVA_PLANAR_PA:
			return bitmapFormatRGBA_PLANAR_PA;
		case bitmapFormatYUV444:
			return bitmapFormatRGB;
		case bitmapFormatYUV444_PLANAR:
			return bitmapFormatRGB_PLANAR;
		default:
			break;
	}
	return bitmapFormatNull;

}
SLIB_GRAPHICS_NAMESPACE_END

