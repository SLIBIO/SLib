/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/graphics/font.h"

#include "slib/graphics/font_atlas.h"
#include "slib/core/safe_static.h"

namespace slib
{

	FontDesc::FontDesc()
	{
		size = Font::getDefaultFontSize();
		flagBold = sl_false;
		flagItalic = sl_false;
		flagUnderline = sl_false;
		flagStrikeout = sl_false;
	}

	FontDesc::~FontDesc()
	{
	}

	sl_real _g_graphics_font_default_size = 12;

	SLIB_DEFINE_ROOT_OBJECT(Font)

	Font::Font()
	{
		m_flagMetricsCache = sl_false;
	}

	Font::~Font()
	{
	}

	sl_real Font::getDefaultFontSize()
	{
		return _g_graphics_font_default_size;
	}

	void Font::setDefaultFontSize(sl_real size)
	{
		_g_graphics_font_default_size = size;
	}

	Ref<Font> Font::getDefault()
	{
		SLIB_SAFE_STATIC(AtomicRef<Font>, defaultFont, create(FontDesc()))
		if (SLIB_SAFE_STATIC_CHECK_FREED(defaultFont)) {
			return sl_null;
		}
		return defaultFont;
	}

	Ref<Font> Font::create(const FontDesc &desc)
	{
		Ref<Font> ret = new Font;
		if (ret.isNotNull()) {
			ret->m_desc.familyName = desc.familyName;
			ret->m_desc.size = SLIB_FONT_SIZE_PRECISION_APPLY(desc.size);
			ret->m_desc.flagBold = desc.flagBold;
			ret->m_desc.flagItalic = desc.flagItalic;
			ret->m_desc.flagUnderline = desc.flagUnderline;
			return ret;
		}
		return sl_null;
	}

	Ref<Font> Font::create(String familyName, sl_real size, sl_bool flagBold, sl_bool flagItalic, sl_bool flagUnderline, sl_bool flagStrikeout)
	{
		Ref<Font> ret = new Font;
		if (ret.isNotNull()) {
			ret->m_desc.familyName = familyName;
			ret->m_desc.size = SLIB_FONT_SIZE_PRECISION_APPLY(size);
			ret->m_desc.flagBold = flagBold;
			ret->m_desc.flagItalic = flagItalic;
			ret->m_desc.flagUnderline = flagUnderline;
			ret->m_desc.flagStrikeout = flagStrikeout;
			return ret;
		}
		return sl_null;
	}

	void Font::getDesc(FontDesc& desc)
	{
		desc = m_desc;
	}

	String Font::getFamilyName()
	{
		return m_desc.familyName;
	}

	sl_real Font::getSize()
	{
		return m_desc.size;
	}

	sl_bool Font::isBold()
	{
		return m_desc.flagBold;
	}

	sl_bool Font::isItalic()
	{
		return m_desc.flagItalic;
	}

	sl_bool Font::isUnderline()
	{
		return m_desc.flagUnderline;
	}
	
	sl_bool Font::isStrikeout()
	{
		return m_desc.flagStrikeout;
	}

	sl_bool Font::getFontMetrics(FontMetrics& _out)
	{
		if (m_flagMetricsCache) {
			_out = m_metricsCache;
			return sl_true;
		}
		if (_getFontMetrics_PO(m_metricsCache)) {
			m_flagMetricsCache = sl_true;
			_out = m_metricsCache;
			return sl_true;
		}
		return 0;
	}

	sl_real Font::getFontHeight()
	{
		FontMetrics fm;
		if (getFontMetrics(fm)) {
			return fm.leading + fm.ascent + fm.descent;
		}
		return 0;
	}

	sl_real Font::getFontAscent()
	{
		FontMetrics fm;
		if (getFontMetrics(fm)) {
			return fm.ascent;
		}
		return 0;
	}

	sl_real Font::getFontDescent()
	{
		FontMetrics fm;
		if (getFontMetrics(fm)) {
			return fm.descent;
		}
		return 0;
	}

	Size Font::measureSingleLineText(const String& text)
	{
		return _measureText_PO(text);
	}

	Size Font::measureMultiLineText(const String16& text)
	{
		sl_char16* sz = text.getData();
		sl_size len = text.getLength();
		sl_size startLine = 0;
		sl_size pos = 0;
		sl_real width = 0;
		sl_real height = 0;
		while (pos <= len) {
			sl_char16 ch;
			if (pos < len) {
				ch = sz[pos];
			} else {
				ch = '\n';
			}
			if (ch == '\r' || ch == '\n') {
				if (pos > startLine) {
					Size size = _measureText_PO(String(sz + startLine, pos - startLine));
					if (size.x > width) {
						width = size.x;
					}
					height += size.y;
				}
				if (ch == '\r' && pos + 1 < len) {
					if (sz[pos + 1] == '\n') {
						pos++;
					}
				}
				startLine = pos + 1;
			}
			pos++;
		}
		return Size(width, height);
	}

	Size Font::measureText(const String& text, sl_bool flagMultiLine)
	{
		if (flagMultiLine) {
			return measureMultiLineText(text);
		} else {
			return measureSingleLineText(text);
		}
	}

	Size Font::measureText16(const String16& text, sl_bool flagMultiLine)
	{
		if (flagMultiLine) {
			return measureMultiLineText(text);
		} else {
			return measureSingleLineText(text);
		}
	}

	Ref<FontAtlas> Font::getAtlas()
	{
		Ref<FontAtlas> fa = m_fontAtlas;
		if (fa.isNull()) {
			FontAtlasParam param;
			param.font = this;
			fa = FontAtlas::create(param);
			if (fa.isNotNull()) {
				m_fontAtlas = fa;
			}
		}
		return fa;
	}

	Ref<FontAtlas> Font::getSharedAtlas()
	{
		Ref<FontAtlas> fa = m_fontAtlas;
		if (fa.isNull()) {
			fa = FontAtlas::getShared(this);
			if (fa.isNotNull()) {
				m_fontAtlas = fa;
			}
		}
		return fa;
	}

	Ref<Referable> Font::getPlatformObject()
	{
		return m_platformObject;
	}

}
