#include "../../../inc/slib/ui/label_view.h"

#include "../../../inc/slib/core/scoped.h"
#include "../../../inc/slib/graphics/util.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(LabelView, View)

LabelView::LabelView()
{
	setCreatingNativeWidget(sl_false);

	setUsingFont(sl_true);
	
	m_textAlignment = Alignment::Left;
	m_textColor = Color::Black;
	m_multiLineMode = MultiLineMode::Single;

	setPadding(1, 1, 1, 1, UIUpdateMode::Init);
}

LabelView::~LabelView()
{
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
	}
	invalidateLayoutFromResizeContent(mode);
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

MultiLineMode LabelView::getMultiLineMode()
{
	return m_multiLineMode;
}

void LabelView::setMultiLineMode(MultiLineMode multiLineMode, UIUpdateMode updateMode)
{
	m_multiLineMode = multiLineMode;
	invalidateLayoutFromResizeContent(updateMode);
}

void LabelView::onDraw(Canvas* canvas)
{
	m_textBox.draw(canvas, m_text, getFont(), getBoundsInnerPadding(), isWidthWrapping(), m_multiLineMode, m_textAlignment, m_textColor);
}

void LabelView::onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical)
{
	if (!flagVertical && !flagHorizontal) {
		return;
	}

	sl_ui_pos paddingWidth = getPaddingLeft() + getPaddingRight();
	m_textBox.update(m_text, getFont(), (sl_real)(getWidth() - paddingWidth), isWidthWrapping(), m_multiLineMode, m_textAlignment);
	if (flagHorizontal) {
		setMeasuredWidth((sl_ui_pos)(m_textBox.getContentWidth()) + paddingWidth);
	}
	if (flagVertical) {
		setMeasuredHeight((sl_ui_pos)(m_textBox.getContentHeight()) + getPaddingTop() + getPaddingBottom());
	}
	
}

#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_WIN32)) && !(defined(SLIB_PLATFORM_IS_ANDROID))

Ref<ViewInstance> LabelView::createNativeWidget(ViewInstance* parent)
{
	return sl_null;
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
