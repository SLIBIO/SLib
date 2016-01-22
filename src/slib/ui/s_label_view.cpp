#include "../../../inc/slib/ui/s_label_view.h"

SLIB_UI_NAMESPACE_BEGIN

SLabelView::SLabelView()
{
	m_textAlignment = alignMiddleLeft;
}

String SLabelView::getText()
{
	return m_text;
}

void SLabelView::setText(const String& text)
{
	m_text = text;
	invalidate();
}

Color SLabelView::getTextColor()
{
	return m_textColor;
}

void SLabelView::setTextColor(const Color& color)
{
	m_textColor = color;
	invalidate();
}

Alignment SLabelView::getTextAlignment()
{
	return m_textAlignment;
}

void SLabelView::setTextAlignment(Alignment align)
{
	m_textAlignment = align;
	invalidate();
}

Ref<Font> SLabelView::getFont()
{
	return m_font;
}

void SLabelView::setFont(const Ref<Font>& font)
{
	m_font = font;
	invalidate();
}

void SLabelView::onDraw(Canvas* canvas)
{
	Rectangle bound = getContentBounds();
	canvas->drawText(m_text, bound, m_font, m_textColor, m_textAlignment);
}

SLIB_UI_NAMESPACE_END
