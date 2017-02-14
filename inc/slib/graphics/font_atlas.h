#ifndef CHECKHEADER_SLIB_GRAPHICS_FONT_ATLAS
#define CHECKHEADER_SLIB_GRAPHICS_FONT_ATLAS

#include "definition.h"

#include "font.h"
#include "bitmap.h"

#include "../math/rectangle.h"
#include "../core/map.h"

namespace slib
{
	
	class SLIB_EXPORT FontAtlasParam
	{
	public:
		Ref<Font> font;
		sl_uint32 planeWidth;
		sl_uint32 planeHeight;
		sl_uint32 maxPlanes;

	public:
		FontAtlasParam();

		~FontAtlasParam();

	};
	
	class SLIB_EXPORT FontAtlasChar
	{
	public:
		Ref<Bitmap> bitmap;
		Rectanglei region;
		sl_real fontWidth;
		sl_real fontHeight;

	public:
		FontAtlasChar();

		FontAtlasChar(const FontAtlasChar& other);

		~FontAtlasChar();

	};
	
	class SLIB_EXPORT FontAtlas : public Object
	{
		SLIB_DECLARE_OBJECT

	protected:
		FontAtlas();

		~FontAtlas();

	public:
		static Ref<FontAtlas> create(const FontAtlasParam& param);

		static Ref<FontAtlas> getShared(const Ref<Font>& font);

		static void removeAllShared();

	public:
		sl_bool getChar(sl_char16 ch, FontAtlasChar& _out);

		Size getFontSize(sl_char16 ch);

		Size getFontSize_NoLock(sl_char16 ch);

		Size measureText(const String16& text, sl_bool flagMultiLine = sl_false);
	
		void removeAll();

	protected:
		sl_bool _getChar(sl_char16 ch, sl_bool flagSizeOnly, FontAtlasChar& _out);

	protected:
		Ref<Font> m_fontSource;
		Ref<Font> m_fontDraw;
		sl_uint32 m_planeWidth;
		sl_uint32 m_planeHeight;
		sl_uint32 m_maxPlanes;
		sl_real m_fontSourceHeight;

		HashMap<sl_char16, FontAtlasChar> m_map;
		sl_uint32 m_countPlanes;
		Ref<Bitmap> m_currentPlane;
		Ref<Canvas> m_currentCanvas;
		sl_uint32 m_currentPlaneY;
		sl_uint32 m_currentPlaneX;
		sl_uint32 m_currentPlaneRowHeight;

	};

}

#endif

