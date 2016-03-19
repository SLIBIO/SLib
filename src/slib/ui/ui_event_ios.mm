#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/ui/event.h"
#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/platform.h"

SLIB_UI_NAMESPACE_BEGIN

sl_uint32 UIEvent::getSystemKeycode(Keycode key)
{
	return -1;
}

Keycode UIEvent::getKeycodeFromSystemKeycode(sl_uint32 vkey)
{
	return Keycode::Unknown;
}

sl_bool UI::checkKeyPressed(Keycode key)
{
	return sl_false;
}

sl_bool UI::checkCapsLockOn()
{
	return sl_false;
}

sl_bool UI::checkNumLockOn()
{
	return sl_false;
}

sl_bool UI::checkScrollLockOn()
{
	return sl_false;
}


Point UI::getCursorPos()
{
	return Point::zero();
}

sl_bool UI::checkLeftButtonPressed()
{
	return sl_false;
}

sl_bool UI::checkRightButtonPressed()
{
	return sl_false;
}

sl_bool UI::checkMiddleButtonPressed()
{
	return sl_false;
}

SLIB_UI_NAMESPACE_END

#endif
