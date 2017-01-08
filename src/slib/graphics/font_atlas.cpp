#include "../../../inc/slib/graphics/font_atlas.h"

#include "../../../inc/slib/core/variant.h"
#include "../../../inc/slib/core/safe_static.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

#define PLANE_SIZE_MIN 64
#define PLANE_WIDTH_DEFAULT 1024
#define PLANE_HEIGHT_DEFAULT 64
#define MAX_PLANES_DEFAULT 16
#define FONT_SIZE_MIN 4
#define FONT_SIZE_MAX 48

FontAtlasParam::FontAtlasParam()
: planeWidth(PLANE_WIDTH_DEFAULT), planeHeight(PLANE_HEIGHT_DEFAULT), maxPlanes(MAX_PLANES_DEFAULT)
{
}

SLIB_DEFINE_OBJECT(FontAtlas, Object)

FontAtlas::FontAtlas()
{
	m_currentPlaneY = 0;
	m_currentPlaneX = 0;
	m_currentPlaneRowHeight = 0;
}

Ref<FontAtlas> FontAtlas::create(const FontAtlasParam& param)
{
	if (param.font.isNotNull()) {
		String fontFamily = param.font->getFamilyName();
		sl_real fontSize = param.font->getSize();
		sl_bool fontBold = param.font->isBold();
		Ref<Font> fontSource = Font::create(fontFamily, fontSize, fontBold);
		if (fontSource.isNotNull()) {
			if (fontSize > FONT_SIZE_MAX) {
				fontSize = FONT_SIZE_MAX;
			}
			if (fontSize < FONT_SIZE_MIN) {
				fontSize = FONT_SIZE_MIN;
			}
			Ref<Font> fontDraw = Font::create(fontFamily, fontSize, fontBold);
			if (fontDraw.isNotNull()) {
				sl_uint32 planeWidth = param.planeWidth;
				if (planeWidth < PLANE_SIZE_MIN) {
					planeWidth = PLANE_SIZE_MIN;
				}
				sl_uint32 planeHeight = param.planeHeight;
				if (planeHeight < PLANE_SIZE_MIN) {
					planeHeight = PLANE_SIZE_MIN;
				}
				sl_uint32 maxPlanes = param.maxPlanes;
				if (maxPlanes < 1) {
					maxPlanes = 1;
				}
				Ref<Bitmap> plane = Bitmap::create(planeWidth, planeHeight);
				if (plane.isNotNull()) {
					Ref<Canvas> canvas = plane->getCanvas();
					if (canvas.isNotNull()) {
						Ref<FontAtlas> ret = new FontAtlas;
						if (ret.isNotNull()) {
							ret->m_fontSource = fontSource;
							ret->m_fontDraw = fontDraw;
							ret->m_planeWidth = planeWidth;
							ret->m_planeHeight = planeHeight;
							ret->m_maxPlanes = maxPlanes;
							ret->m_currentPlane = plane;
							ret->m_currentCanvas = canvas;
							ret->m_countPlanes = 1;
							return ret;
						}
					}
				}
			}
		}
	}
	return sl_null;
}

typedef HashMap< String, WeakRef<FontAtlas> > _FontAtlasMap;
SLIB_SAFE_STATIC_GETTER(_FontAtlasMap, _FontAtlas_getShared)

Ref<FontAtlas> FontAtlas::getShared(const Ref<Font>& font)
{
	if (font.isNotNull()) {
		_FontAtlasMap* map = _FontAtlas_getShared();
		if (map) {
			String fontFamily = font->getFamilyName();
			int fontSize10 = (int)(font->getSize() * 10);
			sl_bool fontBold = font->isBold();
			String sig = String::format("%s_%d_%s", fontFamily, fontSize10, fontBold?"B":"");
			WeakRef<FontAtlas> weakFA;
			map->get(sig, &weakFA);
			Ref<FontAtlas> ret = weakFA;
			if (ret.isNotNull()) {
				return ret;
			}
			Ref<Font> fontSource = Font::create(fontFamily, ((sl_real)fontSize10) / 10, fontBold);
			if (fontSource.isNotNull()) {
				FontAtlasParam fap;
				fap.font = fontSource;
				ret = FontAtlas::create(fap);
				if (ret.isNotNull()) {
					map->put(sig, ret);
					return ret;
				}
			}
		}
	}
	return sl_null;
}

sl_bool FontAtlas::getChar(sl_char16 ch, FontAtlasChar& _out)
{
	ObjectLocker lock(this);
	if (m_map.get_NoLock(ch, &_out)) {
		return _out.bitmap.isNotNull();
	}
	String s(&ch, 1);
	Sizei sizeDraw = m_fontDraw->getTextSize(s);
	Size sizeFont = m_fontSource->getTextSize(s);
	sl_uint32 widthChar = sizeDraw.x;
	sl_uint32 heightChar = sizeDraw.y;
	if (sizeDraw.x <= 0 || sizeDraw.y <= 0 || widthChar > m_planeWidth || heightChar > m_planeHeight || sizeFont.x <= 0 || sizeFont.y <= 0) {
		FontAtlasChar fac;
		fac.fontWidth = 0;
		fac.fontHeight = 0;
		m_map.put_NoLock(ch, fac);
		return sl_false;
	}
	if (m_currentPlaneX + widthChar > m_planeWidth) {
		m_currentPlaneX = 0;
		m_currentPlaneY += m_currentPlaneRowHeight;
		m_currentPlaneRowHeight = 0;
	}
	if (m_currentPlaneY + heightChar > m_planeHeight) {
		if (m_countPlanes >= m_maxPlanes) {
			m_map.removeAll_NoLock();
			m_countPlanes = 1;
		} else {
			Ref<Bitmap> plane = Bitmap::create(m_planeWidth, m_planeHeight);
			if (plane.isNull()) {
				return sl_false;
			}
			Ref<Canvas> canvas = plane->getCanvas();
			if (canvas.isNull()) {
				return sl_false;
			}
			m_currentPlane = plane;
			m_currentCanvas = canvas;
			m_countPlanes++;
		}
		m_currentPlaneX = 0;
		m_currentPlaneY = 0;
		m_currentPlaneRowHeight = 0;
	}
	sl_uint32 x = m_currentPlaneX;
	sl_uint32 y = m_currentPlaneY;
	m_currentPlane->resetPixels(x, y, widthChar, heightChar, Color::Zero);
	m_currentCanvas->drawText(s, (sl_real)x, (sl_real)y, m_fontDraw, Color::White);
	m_currentPlane->update(x, y, widthChar, heightChar);
	m_currentPlaneX += widthChar;
	if (heightChar > m_currentPlaneRowHeight) {
		m_currentPlaneRowHeight = heightChar;
	}
	_out.bitmap = m_currentPlane;
	_out.region.left = x;
	_out.region.top = y;
	_out.region.right = x + widthChar;
	_out.region.bottom = y + heightChar;
	_out.fontWidth = sizeFont.x;
	_out.fontHeight = sizeFont.y;
	m_map.put_NoLock(ch, _out);
	return sl_true;
}

void FontAtlas::removeAll()
{
	ObjectLocker lock(this);
	m_map.removeAll_NoLock();
	m_countPlanes = 1;
	m_currentPlaneX = 0;
	m_currentPlaneY = 0;
	m_currentPlaneRowHeight = 0;
}

SLIB_GRAPHICS_NAMESPACE_END
