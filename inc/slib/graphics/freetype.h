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
public:
	static Ref<FreeType> loadFromMemory(const Memory& mem, sl_uint32 indexFace = 0);
	
	static Ref<FreeType> loadFromFile(const String& fontFilePath, sl_uint32 index = 0);
	
public:
	virtual sl_uint32 getFacesCount() = 0;
	
	virtual sl_uint32 getGlyphsCount() = 0;

	// set size in pixels
	virtual sl_bool setSize(sl_uint32 width, sl_uint32 height) = 0;
	
	void setSize(sl_uint32 size);

	virtual Size getStringExtent(const sl_char16* sz, sl_uint32 len) = 0;

	Size getStringExtent(const String16& text);

	// draw starting at left-bottom corner
	virtual void drawString(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const sl_char16* sz, sl_uint32 len, const Color& color) = 0;

	void drawString(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const String16& text, const Color& color);

	virtual void strokeString(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const sl_char16* sz, sl_uint32 len, const Color& color, sl_uint32 lineWidth) = 0;

	void strokeString(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const String16& text, const Color& color, sl_uint32 lineWidth);

	virtual void strokeStringInside(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const sl_char16* sz, sl_uint32 len, const Color& color, sl_uint32 lineWidth) = 0;

	void strokeStringInside(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const String16& text, const Color& color, sl_uint32 lineWidth);

	virtual void strokeStringOutside(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const sl_char16* sz, sl_uint32 len, const Color& color, sl_uint32 lineWidth) = 0;

	void strokeStringOutside(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const String16& text, const Color& color, sl_uint32 lineWidth);

};

SLIB_GRAPHICS_NAMESPACE_END

#endif
