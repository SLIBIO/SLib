#include "../../../inc/slib/ui/label_view.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(LabelView, View)

LabelView::LabelView()
{
	setCreatingNativeWidget(sl_true);
	
	m_textAlignment = Alignment::Left;
	m_textColor = Color::Black;
	setOccurringClick(sl_true);
	
}

String LabelView::getText()
{
	return m_text;
}

void LabelView::setText(const String& text, sl_bool flagRedraw)
{
	m_text = text;
	if (isNativeWidget()) {
		_setText_NW(text);
	} else {
		if (flagRedraw) {
			invalidate();
		}
	}
}

Color LabelView::getTextColor()
{
	return m_textColor;
}

void LabelView::setTextColor(const Color& color, sl_bool flagRedraw)
{
	m_textColor = color;
	if (isNativeWidget()) {
		_setTextColor_NW(color);
	} else {
		if (flagRedraw) {
			invalidate();
		}
	}
}

Alignment LabelView::getTextAlignment()
{
	return m_textAlignment;
}

void LabelView::setTextAlignment(Alignment align, sl_bool flagRedraw)
{
	m_textAlignment = align;
	if (isNativeWidget()) {
		_setTextAlignment_NW(align);
	} else {
		if (flagRedraw) {
			invalidate();
		}
	}
}

void LabelView::onDraw(Canvas* canvas)
{
	Rectangle bound = getBoundsInnerPadding();
	canvas->drawText(m_text, bound, getFont(), m_textColor, m_textAlignment);
}


#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_WIN32)) && !(defined(SLIB_PLATFORM_IS_ANDROID))

Ref<ViewInstance> LabelView::createNativeWidget(ViewInstance* parent)
{
	return Ref<ViewInstance>::null();
}

void LabelView::_setText_NW(const String& text)
{
}

void LabelView::_setTextColor_NW(const Color& color)
{
}

void LabelView::_setTextAlignment_NW(Alignment align)
{
}

void LabelView::_setFont_NW(const Ref<Font>& font)
{
}

void LabelView::_setBorder_NW(sl_bool flag)
{
}

void LabelView::_setBackgroundColor_NW(const Color& color)
{
}

#endif

SLIB_UI_NAMESPACE_END
