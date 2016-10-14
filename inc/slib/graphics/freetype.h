#ifndef CHECKHEADER_SLIB_GRAPHICS_FREETYPE
#define CHECKHEADER_SLIB_GRAPHICS_FREETYPE

#include "definition.h"

#include "../core/object.h"
#include "../core/memory.h"
#include "../core/string.h"
#include "../math/size.h"

#include "color.h"

struct FT_LibraryRec_;
struct FT_FaceRec_;

SLIB_GRAPHICS_NAMESPACE_BEGIN

class Image;

class _FreeTypeLibrary;

class SLIB_EXPORT FreeType : public Object
{
	SLIB_DECLARE_OBJECT
	
public:
	FreeType();
	~FreeType();
	
public:
	static Ref<FreeType> loadFromMemory(const Memory& mem, sl_uint32 indexFace = 0);
	
	static Ref<FreeType> loadFromFile(const String& fontFilePath, sl_uint32 index = 0);
	
public:
	sl_uint32 getFacesCount();
	
	sl_uint32 getGlyphsCount();

	// set size in pixels
	sl_bool setSize(sl_uint32 width, sl_uint32 height);
	
	void setSize(sl_uint32 size);

	Size getStringExtent(const sl_char16* sz, sl_uint32 len);

	Size getStringExtent(const String16& text);

	// draw starting at left-bottom corner
	void drawString(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const sl_char16* sz, sl_uint32 len, const Color& color);

	void drawString(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const String16& text, const Color& color);

	void strokeString(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const sl_char16* sz, sl_uint32 len, const Color& color, sl_uint32 lineWidth);

	void strokeString(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const String16& text, const Color& color, sl_uint32 lineWidth);

	void strokeStringInside(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const sl_char16* sz, sl_uint32 len, const Color& color, sl_uint32 lineWidth);

	void strokeStringInside(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const String16& text, const Color& color, sl_uint32 lineWidth);

	void strokeStringOutside(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const sl_char16* sz, sl_uint32 len, const Color& color, sl_uint32 lineWidth);

	void strokeStringOutside(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const String16& text, const Color& color, sl_uint32 lineWidth);

protected:
	void _strokeString(const Ref<Image>& imageOutput, sl_int32 x, sl_int32 y, const sl_char16* sz, sl_uint32 len, sl_bool flagBorder, sl_bool flagOutside, sl_uint32 radius, const Color& color);
	
protected:
	Ref<_FreeTypeLibrary> m_libraryRef;
	FT_LibraryRec_* m_library;
	FT_FaceRec_* m_face;
	Memory m_mem;
	
};

SLIB_GRAPHICS_NAMESPACE_END

#endif
