#ifndef CHECKHEADER_SLIB_GRAPHICS_IMAGE
#define CHECKHEADER_SLIB_GRAPHICS_IMAGE

#include "definition.h"

#include "constants.h"
#include "color.h"
#include "bitmap.h"

#include "../core/object.h"
#include "../core/memory.h"
#include "../core/string.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

struct SLIB_EXPORT ImageDesc
{
	sl_uint32 width;
	sl_uint32 height;
	sl_int32 stride;
	Color* colors;
	
	Ref<Referable> ref;
};

class SLIB_EXPORT Image : public Bitmap
{
	SLIB_DECLARE_OBJECT(Image, Bitmap)
protected:
	Image();
	
public:
	static Ref<Image> createStatic(const ImageDesc& desc);
	
	static Ref<Image> createStatic(sl_uint32 width, sl_uint32 height, const Color* pixels, sl_int32 stride = 0);
	
	static Ref<Image> createStatic(sl_uint32 width, sl_uint32 height, const Color* pixels, sl_int32 stride, const Referable* ref);
	
	
	static Ref<Image> create(const ImageDesc& desc);
	
	static Ref<Image> create(sl_uint32 width, sl_uint32 height, const Color* pixels = sl_null, sl_int32 strideSource = 0);
	
	static Ref<Image> create(const BitmapData& bitmapData);

	
	static Ref<Image> createFromBitmap(const Ref<Bitmap>& bitmap);
	
	static Ref<Image> createFromBitmap(const Ref<Bitmap>& bitmap, sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height);
	
	
public:
	// override
	sl_uint32 getBitmapWidth();
	
	// override
	sl_uint32 getBitmapHeight();

	// override
	sl_bool readPixels(sl_uint32 x, sl_uint32 y, BitmapData& desc);
	
	// override
	sl_bool writePixels(sl_uint32 x, sl_uint32 y, const BitmapData& desc);
	
	// override
	sl_bool resetPixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, const Color& color);
	
	sl_bool readPixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, Color* colors, sl_int32 stride = 0);
	
	sl_bool writePixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, const Color* colors, sl_int32 stride = 0);
	
	sl_bool resetPixels(const Color& color);
		
	virtual Ref<Canvas> getCanvas();
	
public:
	SLIB_INLINE sl_uint32 getWidth() const
	{
		return m_desc.width;
	}
	
	SLIB_INLINE sl_uint32 getHeight() const
	{
		return m_desc.height;
	}
	
	SLIB_INLINE sl_int32 getStride() const
	{
		return m_desc.stride;
	}
	
	SLIB_INLINE Color* getColors() const
	{
		return (Color*)(m_desc.colors);
	}

	SLIB_INLINE Color* getColorsAt(sl_int32 x, sl_int32 y) const
	{
		return getColors() + x + y * m_desc.stride;
	}
	
	SLIB_INLINE Color& getPixel(sl_int32 x, sl_int32 y) const
	{
		return getColors()[x + y * m_desc.stride];
	}

	SLIB_INLINE void getDesc(ImageDesc& desc) const
	{
		desc = m_desc;
	}

	void fillColor(const Color& color);

	Ref<Image> createSub(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height) const;

public:
	static void draw(ImageDesc& dst, const ImageDesc& src, BlendMode blend = blendMode_Copy, StretchMode stretch = stretchMode_Halftone);
	
	void drawImage(sl_int32 dx, sl_int32 dy, sl_int32 dw, sl_int32 dh
		, const Ref<Image>& src, sl_int32 sx, sl_int32 sy, sl_int32 sw, sl_int32 sh
		, BlendMode blend = blendMode_Copy, StretchMode stretch = stretchMode_Halftone);
	
	void drawImage(const Rectanglei& rectDst
		, const Ref<Image>& src, const Rectanglei& rectSrc
		, BlendMode blend = blendMode_Copy, StretchMode stretch = stretchMode_Halftone);
	
	void drawImage(sl_int32 dx, sl_int32 dy, sl_int32 dw, sl_int32 dh
		, const Ref<Image>& src, sl_int32 sx, sl_int32 sy
		, BlendMode blend = blendMode_Copy, StretchMode stretch = stretchMode_Halftone);
	
	Ref<Image> scale(sl_uint32 width, sl_uint32 height, StretchMode stretch = stretchMode_Halftone) const;
	
public:
	static ImageFileType getFileType(const void* mem, sl_size size);
	
	SLIB_INLINE static ImageFileType getFileType(Memory mem)
	{
		return getFileType(mem.getBuf(), mem.getSize());
	}

	static Ref<Image> loadFromMemory(const void* mem, sl_size size, sl_uint32 width = 0, sl_uint32 height = 0);
	
	SLIB_INLINE static Ref<Image> loadFromMemory(Memory mem, sl_uint32 width = 0, sl_uint32 height = 0)
	{
		return loadFromMemory(mem.getBuf(), mem.getSize(), width, height);
	}
	
	static Ref<Image> loadFromFile(const String& filePath, sl_uint32 width = 0, sl_uint32 height = 0);
	
	static Ref<Image> loadFromResource(const String& path, sl_uint32 width = 0, sl_uint32 height = 0);

#ifdef SLIB_GRAPHICS_IMAGE_SUPPORT_PNG
	static Ref<Image> loadFromPNG(const void* content, sl_size size);
	
	static Memory saveToPNG(const Ref<Image>& image);
	
	SLIB_INLINE Memory saveToPNG()
	{
		return saveToPNG(this);
	}
	
	static sl_bool saveToPNG(String filePath, const Ref<Image>& image);
	
	SLIB_INLINE sl_bool saveToPNG(String filePath)
	{
		return saveToPNG(filePath, this);
	}
#endif

#ifdef SLIB_GRAPHICS_IMAGE_SUPPORT_JPEG
	static Ref<Image> loadFromJPEG(const void* content, sl_size size);
	
	static Memory saveToJPEG(const Ref<Image>& image, float quality = 0.5f);
	
	SLIB_INLINE Memory saveToJPEG(float quality = 0.5f)
	{
		return saveToJPEG(this, quality);
	}
	
	static sl_bool saveToJPEG(String filePath, const Ref<Image>& image, float quality = 0.5f);
	
	SLIB_INLINE sl_bool saveToJPEG(String filePath, float quality = 0.5f)
	{
		return saveToJPEG(filePath, this, quality);
	}
#endif

protected:
	ImageDesc m_desc;
};

SLIB_GRAPHICS_NAMESPACE_END

#endif
