#include "../../../inc/slib/graphics/font.h"
#include "../../../inc/slib/graphics/context.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

FontDesc::FontDesc()
{
	size = Font::getDefaultFontSize();
	flagBold = sl_false;
	flagItalic = sl_false;
	flagUnderline = sl_false;
}

sl_real _g_graphics_font_default_size = 12;

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
	SLIB_SAFE_STATIC(SafeRef<Font>, defaultFont, create(FontDesc()));
	return defaultFont;
}

Ref<Font> Font::create(const FontDesc &desc)
{
	Ref<Font> ret = new Font;
	if (ret.isNotNull()) {
		ret->m_desc = desc;
	}
	return ret;
}

Ref<Font> Font::create(String familyName, sl_real size, sl_bool flagBold, sl_bool flagItalic, sl_bool flagUnderline)
{
	FontDesc desc;
	desc.familyName = familyName;
	desc.size = size;
	desc.flagBold = flagBold;
	desc.flagItalic = flagItalic;
	desc.flagUnderline = flagUnderline;
	return create(desc);
}

Size Font::getTextSize(const Ref<GraphicsContext>& context, const String& text)
{
	if (context.isNotNull()) {
		return context->getFontTextSize(this, text);
	}
	return Size::zero();
}

SLIB_GRAPHICS_NAMESPACE_END


