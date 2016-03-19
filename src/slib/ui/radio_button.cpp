#include "../../../inc/slib/ui/radio_button.h"

#if defined(SLIB_UI_SUPPORT_NATIVE_RADIO_BUTTON)

SLIB_UI_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(RadioButton, View)

RadioButton::RadioButton()
{
	m_flagSelected = sl_false;
}

void RadioButton::dispatchClick()
{
	Ref<RadioGroup> group = getRadioGroup();
	if (group.isNotNull()) {
		group->select(this);
	}
	View::dispatchClick();
	Ref<Runnable> callback = getOnClick();
	if (callback.isNotNull()) {
		callback->run();
	}
}

Ref<Font> RadioButton::getFont()
{
	return m_font;
}

SLIB_UI_NAMESPACE_END

#endif
