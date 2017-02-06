#include "../../../inc/slib/graphics/font_atlas.h"

#include "../../../inc/slib/core/variant.h"
#include "../../../inc/slib/core/safe_static.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

#define PLANE_SIZE_MIN 32
#define PLANE_WIDTH_DEFAULT 0
#define PLANE_HEIGHT_DEFAULT 0
#define MAX_PLANES_DEFAULT 16
#define FONT_SIZE_MIN 4
#define FONT_SIZE_MAX 48

FontAtlasParam::FontAtlasParam()
: planeWidth(PLANE_WIDTH_DEFAULT), planeHeight(PLANE_HEIGHT_DEFAULT), maxPlanes(MAX_PLANES_DEFAULT)
{
}

FontAtlasParam::~FontAtlasParam()
{
}


FontAtlasChar::FontAtlasChar()
{
}

FontAtlasChar::FontAtlasChar(const FontAtlasChar& other) = default;

FontAtlasChar::~FontAtlasChar()
{
}


SLIB_DEFINE_OBJECT(FontAtlas, Object)

FontAtlas::FontAtlas()
{
	m_currentPlaneY = 0;
	m_currentPlaneX = 0;
	m_currentPlaneRowHeight = 0;
}

FontAtlas::~FontAtlas()
{
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
				if (planeWidth == 0) {
					planeWidth = (sl_uint32)(fontDraw->getFontHeight() * 16);
					if (planeWidth > 1024) {
						planeWidth = 1024;
					}
				}
				if (planeWidth < PLANE_SIZE_MIN) {
					planeWidth = PLANE_SIZE_MIN;
				}
				sl_uint32 planeHeight = param.planeHeight;
				if (planeHeight == 0) {
					planeHeight = (sl_uint32)(fontDraw->getFontHeight());
				}
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
							ret->m_fontSourceHeight = fontSource->getFontHeight();
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
			sl_real fontSize = SLIB_FONT_SIZE_PRECISION_APPLY(font->getSize());
			sl_bool fontBold = font->isBold();
			String sig = String::format("%s_%d_%s", fontFamily, fontSize * SLIB_FONT_SIZE_PRECISION_MULTIPLIER, fontBold?"B":"");
			WeakRef<FontAtlas> weakFA;
			map->get(sig, &weakFA);
			Ref<FontAtlas> ret = weakFA;
			if (ret.isNotNull()) {
				return ret;
			}
			Ref<Font> fontSource = Font::create(fontFamily, fontSize, fontBold);
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

void FontAtlas::removeAllShared()
{
	_FontAtlasMap* map = _FontAtlas_getShared();
	if (map) {
		map->removeAll();
	}
}

sl_bool FontAtlas::getChar(sl_char16 ch, FontAtlasChar& _out)
{
	ObjectLocker lock(this);
	return _getChar(ch, sl_false, _out);
}

Size FontAtlas::getFontSize(sl_char16 ch)
{
	ObjectLocker lock(this);
	FontAtlasChar fac;
	if (_getChar(ch, sl_true, fac)) {
		return Size(fac.fontWidth, fac.fontHeight);
	}
	return Size::zero();
}

Size FontAtlas::getFontSize_NoLock(sl_char16 ch)
{
	FontAtlasChar fac;
	if (_getChar(ch, sl_true, fac)) {
		return Size(fac.fontWidth, fac.fontHeight);
	}
	return Size::zero();
}

Size FontAtlas::measureText(const String16& str, sl_bool flagMultiLine)
{
	if (str.isEmpty()) {
		return Size::zero();
	}
	ObjectLocker lock(this);
	sl_char16* sz = str.getData();
	sl_size len = str.getLength();
	sl_real lineWidth = 0;
	sl_real lineHeight = 0;
	sl_real maxWidth = 0;
	sl_real totalHeight = 0;
	FontAtlasChar fac;
	for (sl_size i = 0; i < len; i++) {
		sl_char16 ch = sz[i];
		if (ch == '\r' || ch == '\n') {
			if (!flagMultiLine) {
				return Size(lineWidth, lineHeight);
			}
			if (lineHeight < m_fontSourceHeight) {
				lineHeight = m_fontSourceHeight;
			}
			if (lineWidth > maxWidth) {
				maxWidth = lineWidth;
			}
			totalHeight += lineHeight;
			lineWidth = 0;
			lineHeight = 0;
			if (ch == '\r' && i + 1 < len) {
				if (sz[i + 1] == '\n') {
					i++;
				}
			}
		} else {
			if (_getChar(sz[i], sl_true, fac)) {
				lineWidth += fac.fontWidth;
				if (lineHeight < fac.fontHeight) {
					lineHeight = fac.fontHeight;
				}
			}
		}
	}
	if (lineWidth > maxWidth) {
		maxWidth = lineWidth;
	}
	totalHeight += lineHeight;
	return Size(maxWidth, totalHeight);
}

sl_bool FontAtlas::_getChar(sl_char16 ch, sl_bool flagSizeOnly, FontAtlasChar& _out)
{
	if (ch == ' ' || ch == '\r' || ch == '\n') {
		_out.fontWidth = m_fontSourceHeight * 0.3f;
		_out.fontHeight = m_fontSourceHeight;
		if (!flagSizeOnly) {
			_out.bitmap.setNull();
		}
		return sl_true;
	}
	if (ch == '\t') {
		_out.fontWidth = m_fontSourceHeight * 2.0f;
		_out.fontHeight = m_fontSourceHeight;
		if (!flagSizeOnly) {
			_out.bitmap.setNull();
		}
		return sl_true;
	}
	if (m_map.get_NoLock(ch, &_out)) {
		
		if (_out.fontWidth <= 0 || _out.fontHeight <= 0) {
			return sl_false;
		}
		
		if (flagSizeOnly) {
			return sl_true;
		}
		
		if (_out.bitmap.isNotNull()) {
			return sl_true;
		}
		
	} else {
		
		String s(&ch, 1);
		
		Size sizeFont = m_fontSource->measureSingleLineText(s);
		
		if (sizeFont.x <= 0 || sizeFont.y <= 0) {
			FontAtlasChar fac;
			fac.fontWidth = 0;
			fac.fontHeight = 0;
			m_map.put_NoLock(ch, fac);
			return sl_false;
		}
		
		_out.fontWidth = sizeFont.x;
		_out.fontHeight = sizeFont.y;
		_out.bitmap.setNull();
		
		if (flagSizeOnly) {
			return m_map.put_NoLock(ch, _out);
		}
		
	}
	
	String s(&ch, 1);
	
	Sizei sizeDraw = m_fontDraw->measureSingleLineText(s);

	sl_uint32 widthChar = sizeDraw.x;
	sl_uint32 heightChar = sizeDraw.y;
	if (sizeDraw.x <= 0 || sizeDraw.y <= 0) {
		_out.fontWidth = 0;
		_out.fontHeight = 0;
		m_map.put_NoLock(ch, _out);
		return sl_false;
	}
	
	if (m_currentPlaneX > 0 && m_currentPlaneX + widthChar > m_planeWidth) {
		m_currentPlaneX = 0;
		m_currentPlaneY += m_currentPlaneRowHeight;
		m_currentPlaneRowHeight = 0;
	}
	if (m_currentPlaneY > 0 && m_currentPlaneY + heightChar > m_planeHeight) {
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
	_out.region.left = x;
	_out.region.top = y;
	_out.region.right = x + widthChar;
	_out.region.bottom = y + heightChar;

	m_currentPlane->resetPixels(x, y, widthChar, heightChar, Color::Zero);
	m_currentCanvas->drawText(s, (sl_real)x, (sl_real)y, m_fontDraw, Color::White);

	m_currentPlane->update(x, y, widthChar, heightChar);
	
	m_currentPlaneX += widthChar;
	
	if (heightChar > m_currentPlaneRowHeight) {
		m_currentPlaneRowHeight = heightChar;
	}
	
	_out.bitmap = m_currentPlane;
	
	return m_map.put_NoLock(ch, _out);
	
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
