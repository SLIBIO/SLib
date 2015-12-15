#include "../../../inc/slib/ui/edit_view.h"

SLIB_UI_NAMESPACE_BEGIN
/**********************
	EditView
 ***********************/
String IEditViewListener::onChange(EditView* edit, const String& newValue)
{
	return newValue;
}

EditView::EditView()
{
	m_textAlignment = alignMiddleCenter;
	m_flagBorder = sl_true;
	m_flagReadOnly = sl_false;
	m_flagMultiLine = sl_false;
	m_textColor = Color::black();
	m_backgroundColor = Color::white();
}

void EditView::onKeyEvent(UIEvent* ev)
{
	if (!(isMultiLine())) {
		if (ev->getAction() == actionKeyUp) {
			if (ev->getKeycode() == keyEnter) {
				dispatchEnterAction();
			}
		}
	}
}

void EditView::dispatchEnterAction()
{
	Ref<Runnable> action = getEnterAction();
	if (action.isNotNull()) {
		action->run();
	}
}

String EditView::onChange(const String& newValue)
{
	PtrLocker<IEditViewListener> listener(getListener());
	if (listener.isNotNull()) {
		return listener->onChange(this, newValue);
	}
	return newValue;
}

Ref<Font> EditView::getFont()
{
	return m_font;
}

/**********************
	PasswordView
 ***********************/
PasswordView::PasswordView()
{
}

sl_bool PasswordView::isMultiLine()
{
	return sl_false;
}

void PasswordView::setMultiLine(sl_bool flag)
{
}

/**********************
	TextArea
 ***********************/
TextArea::TextArea()
{
	m_flagMultiLine = sl_true;
}

sl_bool TextArea::isMultiLine()
{
	return sl_true;
}

void TextArea::setMultiLine(sl_bool flag)
{
}
SLIB_UI_NAMESPACE_END
