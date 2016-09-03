#include "../../../inc/slib/ui/edit_view.h"
#include "../../../inc/slib/graphics/context.h"

SLIB_UI_NAMESPACE_BEGIN

/**********************
	EditView
 ***********************/

SLIB_DEFINE_OBJECT(EditView, View)

EditView::EditView()
{
	setCreatingNativeWidget(sl_true);
	m_textAlignment = Alignment::MiddleCenter;
	m_flagReadOnly = sl_false;
	m_flagMultiLine = sl_false;
	m_textColor = Color::Black;
	setBorder(sl_true, sl_false);
}

String EditView::getText()
{
	if (isNativeWidget()) {
		_getText_NW();
	}
	return m_text;
}

void EditView::setText(const String& text, sl_bool flagRedraw)
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

Alignment EditView::getGravity()
{
	if (isNativeWidget()) {
		_getTextAlignment_NW();
	}
	return m_textAlignment;
}

void EditView::setGravity(Alignment align, sl_bool flagRedraw)
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

String EditView::getHintText()
{
	if (isNativeWidget()) {
		_getHintText_NW();
	}
	return m_hintText;
}

void EditView::setHintText(const String& str, sl_bool flagRedraw)
{
	m_hintText = str;
	if (isNativeWidget()) {
		_setHintText_NW(str);
	} else {
		if (flagRedraw) {
			invalidate();
		}
	}
}

sl_bool EditView::isReadOnly()
{
	if (isNativeWidget()) {
		_isReadOnly_NW();
	}
	return m_flagReadOnly;
}

void EditView::setReadOnly(sl_bool flag, sl_bool flagRedraw)
{
	m_flagReadOnly = flag;
	if (isNativeWidget()) {
		_setReadOnly_NW(flag);
	} else {
		if (flagRedraw) {
			invalidate();
		}
	}
}

sl_bool EditView::isMultiLine()
{
	if (isNativeWidget()) {
		_isMultiLine_NW();
	}
	return m_flagMultiLine;
}

void EditView::setMultiLine(sl_bool flag, sl_bool flagRedraw)
{
	m_flagMultiLine = flag;
	if (isNativeWidget()) {
		_setMultiLine_NW(flag);
	} else {
		if (flagRedraw) {
			invalidate();
		}
	}
}

Color EditView::getTextColor()
{
	return m_textColor;
}

void EditView::setTextColor(const Color& color, sl_bool flagRedraw)
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


void EditView::onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical)
{
	if (!flagHorizontal && !flagVertical) {
		return;
	}
	
	Ref<GraphicsContext> gc = getGraphicsContext();
	if (gc.isNull()) {
		return;
	}
	
	Size sizeText = gc->getFontTextSize(getFont(), "|");
	if (flagHorizontal) {
		if (sizeText.x < 0) {
			sizeText.x = 0;
		}
		setMeasuredWidth(sizeText.x + getPaddingLeft() + getPaddingRight());
	}
	if (flagVertical) {
		if (sizeText.y < 0) {
			sizeText.y = 0;
		}
		setMeasuredHeight(sizeText.y + getPaddingTop() + getPaddingBottom());
	}
}

String EditView::onChange(const String& newValue)
{
	return newValue;
}

void EditView::onEnterAction()
{
}

void EditView::dispatchKeyEvent(UIEvent* ev)
{
	View::dispatchKeyEvent(ev);
	if (!(isMultiLine())) {
		if (ev->getAction() == UIAction::KeyUp) {
			if (ev->getKeycode() == Keycode::Enter) {
				dispatchEnterAction();
			}
		}
	}
}

String EditView::dispatchChange(const String& newValue)
{
	String value = onChange(newValue);
	PtrLocker<IEditViewListener> listener(getListener());
	if (listener.isNotNull()) {
		return listener->onChange(this, value);
	}
	return value;
}

void EditView::dispatchEnterAction()
{
	Ref<Runnable> action = getEnterAction();
	if (action.isNotNull()) {
		action->run();
	}
	onEnterAction();
}

/**********************
	PasswordView
 ***********************/

SLIB_DEFINE_OBJECT(PasswordView, EditView)

PasswordView::PasswordView()
{
}

sl_bool PasswordView::isMultiLine()
{
	return sl_false;
}

void PasswordView::setMultiLine(sl_bool flag, sl_bool flagRedraw)
{
}

/**********************
	TextArea
 ***********************/

SLIB_DEFINE_OBJECT(TextArea, EditView)

TextArea::TextArea()
{
	m_flagMultiLine = sl_true;
}

sl_bool TextArea::isMultiLine()
{
	return sl_true;
}

void TextArea::setMultiLine(sl_bool flag, sl_bool flagRedraw)
{
}


#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_WIN32)) && !(defined(SLIB_PLATFORM_IS_ANDROID))

Ref<ViewInstance> EditView::createNativeWidget(ViewInstance* parent)
{
	return Ref<ViewInstance>::null();
}

void EditView::_getText_NW()
{
}

void EditView::_setText_NW(const String& text)
{
}

void EditView::_getTextAlignment_NW()
{
}

void EditView::_setTextAlignment_NW(Alignment align)
{
}

void EditView::_getHintText_NW()
{
}

void EditView::_setHintText_NW(const String& str)
{
}

void EditView::_isReadOnly_NW()
{
}

void EditView::_setReadOnly_NW(sl_bool flag)
{
}

void EditView::_isMultiLine_NW()
{
}

void EditView::_setMultiLine_NW(sl_bool flag)
{
}

void EditView::_setTextColor_NW(const Color& color)
{
}

void EditView::_setFont_NW(const Ref<Font>& font)
{
}

void EditView::_setBorder_NW(sl_bool flag)
{
}

void EditView::_setBackgroundColor_NW(const Color& color)
{
}

Ref<ViewInstance> PasswordView::createNativeWidget(ViewInstance* parent)
{
	return Ref<ViewInstance>::null();
}

Ref<ViewInstance> TextArea::createNativeWidget(ViewInstance* parent)
{
	return Ref<ViewInstance>::null();
}

#endif

SLIB_UI_NAMESPACE_END
