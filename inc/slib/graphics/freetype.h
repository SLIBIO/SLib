#ifndef CHECKHEADER_SLIB_GRAPHICS_FREETYPE
#define CHECKHEADER_SLIB_GRAPHICS_FREETYPE

#include "definition.h"

#include "../core/object.h"
#include "../core/memory.h"
#include "../core/string.h"
#include "../math/size.h"

#include "color.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

class Image;

class SLIB_EXPORT FreeType : public Object
{
protected:
	FreeType();

public:
	virtual sl_uint32 getFacesCount() = 0;
	
	virtual sl_uint32 getGlyphsCount() = 0;

	// set size in pixels
	virtual sl_bool setSize(sl_uint32 width, sl_uint32 height) = 0;
	
	SLIB_INLINE void setSize(sl_uint32 size)
	{
		setSize(size, size);
	}

	virtual Size getStringExtent(const sl_char16* sz, sl_uint32 len) = 0;

	SLIB_INLINE Size getStringExtent(const String16& text)
	{
		return getStringExtent(text.getBuf(), text.getLength());
	}

	// draw starting at left-bottom corner
	virtual void drawString(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const sl_char16* sz, sl_uint32 len, const Color& color) = 0;

	SLIB_INLINE void drawString(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const String16& text, const Color& color)
	{
		return drawString(imageOutput, x, y, text.getBuf(), text.getLength(), color);
	}

	virtual void strokeString(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const sl_char16* sz, sl_uint32 len, const Color& color, sl_uint32 lineWidth) = 0;

	SLIB_INLINE void strokeString(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const String16& text, const Color& color, sl_uint32 lineWidth)
	{
		return strokeString(imageOutput, x, y, text.getBuf(), text.getLength(), color, lineWidth);
	}

	virtual void strokeStringInside(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const sl_char16* sz, sl_uint32 len, const Color& color, sl_uint32 lineWidth) = 0;

	SLIB_INLINE void strokeStringInside(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const String16& text, const Color& color, sl_uint32 lineWidth)
	{
		return strokeStringInside(imageOutput, x, y, text.getBuf(), text.getLength(), color, lineWidth);
	}

	virtual void strokeStringOutside(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const sl_char16* sz, sl_uint32 len, const Color& color, sl_uint32 lineWidth) = 0;

	SLIB_INLINE void strokeStringOutside(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const String16& text, const Color& color, sl_uint32 lineWidth)
	{
		return strokeString(imageOutput, x, y, text.getBuf(), text.getLength(), color, lineWidth);
	}

public:
	static Ref<FreeType> loadFromMemory(const Memory& mem, sl_uint32 indexFace = 0);
	
	static Ref<FreeType> loadFromFile(const String& fontFilePath, sl_uint32 index = 0);

};
SLIB_GRAPHICS_NAMESPACE_END

#endif
