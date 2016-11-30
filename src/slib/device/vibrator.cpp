#include "../../../inc/slib/device/vibrator.h"

#if defined(SLIB_PLATFORM_IS_DESKTOP)

SLIB_DEVICE_NAMESPACE_BEGIN

sl_bool Vibrator::vibrate(sl_int32 millisec)
{
	return sl_false;
}

SLIB_DEVICE_NAMESPACE_END

#endif
