#ifndef CHECKHEADER_SLIB_GRAPHICS_FONT_ATLAS
#define CHECKHEADER_SLIB_GRAPHICS_FONT_ATLAS

#include "definition.h"

#include "font.h"
#include "bitmap.h"

#include "../math/rectangle.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

class SLIB_EXPORT FontAtlasParam
{
public:
	Ref<Font> font;
	sl_uint32 planeWidth;
	sl_uint32 planeHeight;
	sl_uint32 maxPlanes;
	
public:
	FontAtlasParam();
	
};

struct FontAtlasChar
{
	Ref<Bitmap> bitmap;
	Rectanglei region;
	sl_uint32 fontWidth;
	sl_uint32 fontHeight;
};

class SLIB_EXPORT FontAtlas : public Object
{
	SLIB_DECLARE_OBJECT
	
protected:
	FontAtlas();
	
public:
	static Ref<FontAtlas> create(const FontAtlasParam& param);
	
	static Ref<FontAtlas> getShared(const Ref<Font>& font);
	
public:
	sl_bool getChar(sl_char16 ch, FontAtlasChar& _out);

	void removeAll();
	
protected:
	Ref<Font> m_fontSource;
	Ref<Font> m_fontDraw;
	sl_uint32 m_planeWidth;
	sl_uint32 m_planeHeight;
	sl_uint32 m_maxPlanes;
	
	HashMap<sl_char16, FontAtlasChar> m_map;
	sl_uint32 m_countPlanes;
	Ref<Bitmap> m_currentPlane;
	Ref<Canvas> m_currentCanvas;
	sl_uint32 m_currentPlaneY;
	sl_uint32 m_currentPlaneX;
	sl_uint32 m_currentPlaneRowHeight;
	
};

SLIB_GRAPHICS_NAMESPACE_END

#endif

