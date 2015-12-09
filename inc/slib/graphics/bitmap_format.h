#ifndef CHECKHEADER_SLIB_GRAPHICS_BITMAP_FORMAT
#define CHECKHEADER_SLIB_GRAPHICS_BITMAP_FORMAT

#include "definition.h"

#include "constants.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

/*
	Format Bits Description
 
 0~7: unique id
 8~15: bits per pixel
 16~23: bits per sample
 24~25: number of planes
 26: precomputed alpha
 27: has alpha
 28~31: color space
 
 */

#define SLIB_DEFINE_BITMAP_FORMAT(colorSpace, hasAlpha, isPA, planes, bitsPerSample, bitsPerPixel, ID) \
	((colorSpace << 28) | (hasAlpha << 27) | (isPA << 26) | (planes << 24) | (bitsPerSample << 16) | (bitsPerPixel << 8) | (ID))

enum BitmapFormatValue {
	
	bitmapFormatNull = 0,
	
	// RGB
	// 32 bit formats
	bitmapFormatRGBA =				SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_RGB, 1, 0, 1, 32, 32, 0x01),
	bitmapFormatBGRA =				SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_RGB, 1, 0, 1, 32, 32, 0x02),
	bitmapFormatARGB =				SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_RGB, 1, 0, 1, 32, 32, 0x03),
	bitmapFormatABGR =				SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_RGB, 1, 0, 1, 32, 32, 0x04),
	
	// 32 bit formats, precomputed alpha
	bitmapFormatRGBA_PA =			SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_RGB, 1, 1, 1, 32, 32, 0x05),
	bitmapFormatBGRA_PA =			SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_RGB, 1, 1, 1, 32, 32, 0x06),
	bitmapFormatARGB_PA =			SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_RGB, 1, 1, 1, 32, 32, 0x07),
	bitmapFormatABGR_PA =			SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_RGB, 1, 1, 1, 32, 32, 0x08),
	
	// 24 bit formats
	bitmapFormatRGB =				SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_RGB, 0, 0, 1, 24, 24, 0x10),
	bitmapFormatBGR =				SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_RGB, 0, 0, 1, 24, 24, 0x11),
	
	// 16 bit formats
	bitmapFormatRGB565BE =			SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_RGB, 0, 0, 1, 16, 16, 0x16),
	bitmapFormatRGB565LE =			SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_RGB, 0, 0, 1, 16, 16, 0x17),
	bitmapFormatBGR565BE =			SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_RGB, 0, 0, 1, 16, 16, 0x18),
	bitmapFormatBGR565LE =			SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_RGB, 0, 0, 1, 16, 16, 0x19),
	
	// 8 bit formats
	bitmapFormatGRAY8 =				SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_RGB, 0, 0, 1, 8, 8, 0x20),
	
	// planar formats
	bitmapFormatRGBA_PLANAR =		SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_RGB, 1, 0, 4, 8, 32, 0x30),
	bitmapFormatRGBA_PLANAR_PA =	SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_RGB, 1, 1, 4, 8, 32, 0x31), // precomputed alpha
	bitmapFormatRGB_PLANAR =		SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_RGB, 0, 0, 3, 8, 24, 0x32),
	
	// YUV
	// 32 bit formats
	bitmapFormatYUVA =				SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_YUV, 1, 0, 1, 32, 32, 0x80),
	bitmapFormatYUVA_PA =			SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_YUV, 1, 1, 1, 32, 32, 0x81),
	// 24 bit formats
	bitmapFormatYUV444 =			SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_YUV, 0, 0, 1, 24, 24, 0x88),
	// planar formats
	bitmapFormatYUVA_PLANAR =		SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_YUV, 1, 0, 4, 8, 32, 0x82),
	bitmapFormatYUVA_PLANAR_PA =	SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_YUV, 1, 1, 4, 8, 32, 0x83),
	bitmapFormatYUV444_PLANAR =		SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_YUV, 0, 0, 3, 8, 24, 0x89),
	
	// YUV420 planar formats
	bitmapFormatYUV_I420 =			SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_YUV, 0, 0, 3, 0, 12, 0xB0), // 8 bit Y plane followed by 8 bit 2x2 subsampled U and V planes
	bitmapFormatYUV_YV12 =			SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_YUV, 0, 0, 3, 0, 12, 0xB1), // 8 bit Y plane followed by 8 bit 2x2 subsampled V and U planes
	bitmapFormatYUV_NV21 =			SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_YUV, 0, 0, 2, 0, 12, 0xB2), // 8-bit Y plane followed by an interleaved V/U plane with 2x2 subsampling (Android camera standard)
	bitmapFormatYUV_NV12 =			SLIB_DEFINE_BITMAP_FORMAT(ColorSpace_YUV, 0, 0, 2, 0, 12, 0xB3) // 8-bit Y plane followed by an interleaved U/V plane with 2x2 subsampling
};


class SLIB_EXPORT BitmapFormat
{
private:
	BitmapFormatValue m_format;
	
public:
	SLIB_INLINE BitmapFormat()
	{
		m_format = bitmapFormatNull;
	}
	
	SLIB_INLINE BitmapFormat(const BitmapFormat& other)
	{
		m_format = other.m_format;
	}
	
	SLIB_INLINE BitmapFormat(BitmapFormatValue format)
	{
		m_format = format;
	}
	
private:
	static BitmapFormatValue _null;
	
public:
	SLIB_INLINE BitmapFormatValue getValue()
	{
		return m_format;
	}

	SLIB_INLINE static const BitmapFormat& null()
	{
		return *((BitmapFormat*)((void*)&_null));
	}
	
	SLIB_INLINE void setNull()
	{
		m_format = bitmapFormatNull;
	}
	
	SLIB_INLINE sl_bool isNull() const
	{
		return m_format == bitmapFormatNull;
	}
	
	SLIB_INLINE sl_bool isNotNull() const
	{
		return m_format != bitmapFormatNull;
	}
	
public:
	SLIB_INLINE BitmapFormat& operator=(const BitmapFormat& other)
	{
		m_format = other.m_format;
		return *this;
	}
	
	SLIB_INLINE BitmapFormat& operator=(BitmapFormatValue other)
	{
		m_format = other;
		return *this;
	}
	
	SLIB_INLINE sl_bool operator==(const BitmapFormat& other) const
	{
		return m_format == other.m_format;
	}
	
	SLIB_INLINE sl_bool operator==(BitmapFormatValue other) const
	{
		return m_format == other;
	}
	
	SLIB_INLINE sl_bool operator!=(const BitmapFormat& other) const
	{
		return m_format != other.m_format;
	}
	
	SLIB_INLINE sl_bool operator!=(BitmapFormatValue other) const
	{
		return m_format != other;
	}
	
public:
	ColorSpace getColorSpace() const;
	
	sl_bool hasAlpha() const;
	
	sl_bool isPrecomputedAlpha() const;
	
	sl_uint32 getPlanesCount() const;
	
	sl_uint32 getBitsPerSample() const;
	
	sl_uint32 getBytesPerSample() const;
	
	sl_uint32 getBitsPerPixel() const;
	
	sl_uint32 getBytesPerPixel() const;
	
	BitmapFormat getNonPrecomputedAlphaFormat() const;
	
	BitmapFormat getPrecomputedAlphaFormat() const;
	
	BitmapFormat getCompatibleRGBFormat() const;
	
public:
	sl_bool isYUV_420() const;
};

SLIB_GRAPHICS_NAMESPACE_END

#endif
