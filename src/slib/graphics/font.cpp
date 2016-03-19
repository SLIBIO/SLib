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

SLIB_DEFINE_OBJECT(Font, Object)

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

Size Font::getTextSize(const Ref<GraphicsContext>& context, const String& text)
{
	if (context.isNotNull()) {
		return context->getFontTextSize(this, text);
	}
	return Size::zero();
}


SLIB_DEFINE_OBJECT(FontInstance, Object)

SLIB_GRAPHICS_NAMESPACE_END


