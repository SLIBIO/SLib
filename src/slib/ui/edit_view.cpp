#include "../../../inc/slib/ui/edit_view.h"

SLIB_UI_NAMESPACE_BEGIN

/**********************
	EditView
 ***********************/

SLIB_DEFINE_OBJECT(EditView, View)

EditView::EditView()
{
	setCreatingNativeWidget(sl_true);
	setUsingFont(sl_true);
	m_textAlignment = Alignment::MiddleCenter;
	m_flagReadOnly = sl_false;
	m_flagMultiLine = sl_false;
	m_textColor = Color::Black;
	setBorder(sl_true, UIUpdateMode::Init);
	m_returnKeyType = UIReturnKeyType::Default;
	m_keyboardType = UIKeyboardType::Default;
	m_autoCapitalizationType = UIAutoCapitalizationType::None;
	m_flagAutoDismissKeyboard = sl_true;
}

String EditView::getText()
{
	if (isNativeWidget()) {
		_getText_NW();
	}
	return m_text;
}

void EditView::setText(const String& text, UIUpdateMode mode)
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

Alignment EditView::getGravity()
{
	return m_textAlignment;
}

void EditView::setGravity(Alignment align, UIUpdateMode mode)
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

String EditView::getHintText()
{
	return m_hintText;
}

void EditView::setHintText(const String& str, UIUpdateMode mode)
{
	m_hintText = str;
	if (isNativeWidget()) {
		_setHintText_NW(str);
	} else {
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

sl_bool EditView::isReadOnly()
{
	return m_flagReadOnly;
}

void EditView::setReadOnly(sl_bool flag, UIUpdateMode mode)
{
	m_flagReadOnly = flag;
	if (isNativeWidget()) {
		_setReadOnly_NW(flag);
	} else {
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

sl_bool EditView::isMultiLine()
{
	return m_flagMultiLine;
}

void EditView::setMultiLine(sl_bool flag, UIUpdateMode mode)
{
	m_flagMultiLine = flag;
	if (isNativeWidget()) {
		_setMultiLine_NW(flag);
	} else {
		if (mode == UIUpdateMode::Redraw) {
			invalidate();
		}
	}
}

Color EditView::getTextColor()
{
	return m_textColor;
}

void EditView::setTextColor(const Color& color, UIUpdateMode mode)
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

UIReturnKeyType EditView::getReturnKeyType()
{
	return m_returnKeyType;
}

void EditView::setReturnKeyType(UIReturnKeyType type)
{
	m_returnKeyType = type;
	if (isNativeWidget()) {
		_setReturnKeyType_NW(type);
	}
}

UIKeyboardType EditView::getKeyboardType()
{
	return m_keyboardType;
}

void EditView::setKeyboardType(UIKeyboardType type)
{
	m_keyboardType = type;
	if (isNativeWidget()) {
		_setKeyboardType_NW(type);
	}
}

void EditView::setAutoCapitalizationType(UIAutoCapitalizationType type)
{
	m_autoCapitalizationType = type;
	if (isNativeWidget()) {
		_setAutoCapitalizationType_NW(type);
	}
}

sl_bool EditView::isAutoDismissKeyboard()
{
	return m_flagAutoDismissKeyboard;
}

void EditView::setAutoDismissKeyboard(sl_bool flag)
{
	m_flagAutoDismissKeyboard = flag;
}

void EditView::onMeasureLayout(sl_bool flagHorizontal, sl_bool flagVertical)
{
	if (!flagHorizontal && !flagVertical) {
		return;
	}

	if (flagHorizontal) {
		sl_ui_pos width = getPaddingLeft() + getPaddingRight();
		if (width < 0) {
			width = 0;
		}
		setMeasuredWidth(width);
	}
	if (flagVertical) {
		sl_ui_pos height = 0;
		Ref<Font> font = getFont();
		if (font.isNotNull()) {
			height = (sl_ui_pos)(font->getFontHeight() * 1.5f);
			if (height < 0) {
				height = 0;
			}
		}
		height += getPaddingTop() + getPaddingBottom();
		if (height < 0) {
			height = 0;
		}
		setMeasuredHeight(height);
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
	(getEnterAction())();
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

void PasswordView::setMultiLine(sl_bool flag, UIUpdateMode mode)
{
}

/**********************
	TextArea
 ***********************/

SLIB_DEFINE_OBJECT(TextArea, EditView)

TextArea::TextArea()
{
	m_flagMultiLine = sl_true;
	setReturnKeyType(UIReturnKeyType::Return);
}

sl_bool TextArea::isMultiLine()
{
	return sl_true;
}

void TextArea::setMultiLine(sl_bool flag, UIUpdateMode mode)
{
}


#if !(defined(SLIB_PLATFORM_IS_OSX)) && !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_WIN32)) && !(defined(SLIB_PLATFORM_IS_ANDROID))

Ref<ViewInstance> EditView::createNativeWidget(ViewInstance* parent)
{
	return sl_null;
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
	return sl_null;
}

Ref<ViewInstance> TextArea::createNativeWidget(ViewInstance* parent)
{
	return sl_null;
}

#endif


#if !(defined(SLIB_PLATFORM_IS_IOS)) && !(defined(SLIB_PLATFORM_IS_ANDROID))

void EditView::_setReturnKeyType_NW(UIReturnKeyType type)
{
}

void EditView::_setKeyboardType_NW(UIKeyboardType type)
{
}

void EditView::_setAutoCapitalizationType_NW(UIAutoCapitalizationType type)
{
}

#endif

SLIB_UI_NAMESPACE_END
