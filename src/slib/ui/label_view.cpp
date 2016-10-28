#include "../../../inc/slib/ui/label_view.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(LabelView, View)

LabelView::LabelView()
{
	setCreatingNativeWidget(sl_false);
	setUsingFont(sl_true);
	
	m_textAlignment = Alignment::Left;
	m_textColor = Color::Black;
	setOccurringClick(sl_true);
	
}

String LabelView::getText()
{
	return m_text;
}

void LabelView::setText(const String& text, UIUpdateMode mode)
{
	m_text = text;
	if (isNativeWidget()) {
		_setText_NW(text);
	} else {
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

Color LabelView::getTextColor()
{
	return m_textColor;
}

void LabelView::setTextColor(const Color& color, UIUpdateMode mode)
{
	m_textColor = color;
	if (isNativeWidget()) {
		_setTextColor_NW(color);
	} else {
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

Alignment LabelView::getGravity()
{
	return m_textAlignment;
}

void LabelView::setGravity(Alignment align, UIUpdateMode mode)
{
	m_textAlignment = align;
	if (isNativeWidget()) {
		_setTextAlignment_NW(align);
	} else {
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

void LabelView::onDraw(Canvas* canvas)
{
	canvas->drawText(m_text, getBoundsInnerPadding(), getFont(), m_textColor, m_textAlignment);
}

void LabelView::onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical)
{
	if (!flagVertical && !flagHorizontal) {
		return;
	}
	
	UISize size;
	Ref<Font> font = getFont();
	if (font.isNotNull()) {
		size = font->getTextSize(m_text);
	} else {
		size = UISize::zero();
	}
	if (flagHorizontal) {
		if (size.x < 0) {
			size.x = 0;
		}
		sl_ui_pos width = size.x + getPaddingLeft() + getPaddingRight();
		if (width < 0) {
			width = 0;
		}
		setMeasuredWidth(width);
	}
	if (flagVertical) {
		if (size.y < 0) {
			size.y = 0;
		}
		sl_ui_pos height = size.y + getPaddingTop() + getPaddingBottom();
		if (height < 0) {
			height = 0;
		}
		setMeasuredHeight(height);
	}
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
