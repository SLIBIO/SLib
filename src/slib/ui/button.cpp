#include "../../../inc/slib/ui/button.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(Button, View)

Button::Button()
{
	m_flagDefaultButton = sl_false;
}

void Button::dispatchClick()
{
	View::dispatchClick();
	Ref<Runnable> callback = getOnClick();
	if (callback.isNotNull()) {
		callback->run();
	}
}

sl_bool Button::isDefaultButton()
{
	return m_flagDefaultButton;
}

Ref<Font> Button::getFont()
{
	return m_font;
}

SLIB_UI_NAMESPACE_END
