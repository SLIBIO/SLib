#include "../../../inc/slib/ui/check_box.h"

#if defined(SLIB_UI_SUPPORT_NATIVE_CHECK_BOX)

SLIB_UI_NAMESPACE_BEGIN

CheckBox::CheckBox()
{
	m_flagChecked = sl_false;
}

void CheckBox::dispatchClick()
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

Ref<Font> CheckBox::getFont()
{
	return m_font;
}

SLIB_UI_NAMESPACE_END

#endif
