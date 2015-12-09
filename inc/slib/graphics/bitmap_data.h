#ifndef CHECKHEADER_SLIB_GRAPHICS_BITMAP_DATA
#define CHECKHEADER_SLIB_GRAPHICS_BITMAP_DATA

#include "definition.h"

#include "bitmap_format.h"
#include "color.h"

#include "../core/reference.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

class SLIB_EXPORT ColorComponentBuffer
{
public:
	sl_uint32 width;
	sl_uint32 height;
	void* data;
	sl_int32 sample_stride; // bytes offset between samples on a row
	sl_int32 pitch; // bytes of a physical row
	Ref<Referable> ref;
};

class SLIB_EXPORT BitmapData
{
public:
	sl_uint32 width;
	sl_uint32 height;
	BitmapFormat format;
	
	// plane0
	void* data; // samples
	sl_int32 pitch; // number of bytes of a row
	Ref<Referable> ref; // reference for samples
	
	// plane1
	void* data1; // samples
	sl_int32 pitch1; // number of bytes of a row
	Ref<Referable> ref1; // reference for samples

	// plane2
	void* data2; // samples
	sl_int32 pitch2; // number of bytes of a row
	Ref<Referable> ref2; // reference for samples
	
	// plane3
	void* data3; // samples
	sl_int32 pitch3; // number of bytes of a row
	Ref<Referable> ref3; // reference for samples

public:
	BitmapData();
	
	BitmapData(const BitmapData& other);
	
	SLIB_INLINE BitmapData(sl_uint32 width, sl_uint32 height, const Color* colors, sl_int32 stride = 0)
	{
		setFromColors(width, height, colors, stride);
	}
	
public:
	BitmapData& operator=(const BitmapData& other);
	
public:
	SLIB_INLINE void*& planeData(sl_uint32 plane)
	{
		return *(void**)(((sl_uint8*)&data) + (((sl_uint8*)&data1)-((sl_uint8*)&data)) * plane);
	}
	
	SLIB_INLINE void* const& planeData(sl_uint32 plane) const
	{
		return *(void**)(((sl_uint8*)&data) + (((sl_uint8*)&data1)-((sl_uint8*)&data)) * plane);
	}

	SLIB_INLINE sl_int32& planePitch(sl_uint32 plane)
	{
		return *(sl_int32*)(((sl_uint8*)&pitch) + (((sl_uint8*)&pitch1)-((sl_uint8*)&pitch)) * plane);
	}
	
	SLIB_INLINE sl_int32 const& planePitch(sl_uint32 plane) const
	{
		return *(sl_int32*)(((sl_uint8*)&pitch) + (((sl_uint8*)&pitch1)-((sl_uint8*)&pitch)) * plane);
	}
	
public:
	void fillDefaultValues();
	
	sl_size getTotalSize() const;
	
	// returns the number of color components
	sl_uint32 getColorComponentBuffers(ColorComponentBuffer* buffers) const;
	
	void copyPixelsFrom(const BitmapData& other);
	
	void setFromColors(sl_uint32 width, sl_uint32 height, const Color* colors, sl_int32 stride = 0);
	
public:
	static sl_int32 calculatePitchAlign1(sl_uint32 width, sl_uint32 bitsPerSample);
	static sl_int32 calculatePitchAlign2(sl_uint32 width, sl_uint32 bitsPerSample);
	static sl_int32 calculatePitchAlign4(sl_uint32 width, sl_uint32 bitsPerSample);
	static sl_int32 calculatePitchAlign8(sl_uint32 width, sl_uint32 bitsPerSample);
	static sl_int32 calculatePitchAlign16(sl_uint32 width, sl_uint32 bitsPerSample);
};

SLIB_GRAPHICS_NAMESPACE_END

#endif
