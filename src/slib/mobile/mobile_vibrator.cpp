#include "../../../inc/slib/mobile/vibrator.h"

#if defined(SLIB_PLATFORM_IS_DESKTOP)

SLIB_MOBILE_NAMESPACE_BEGIN
sl_bool Vibrator::vibrate(sl_int32 millisec)
{
	return sl_true;
}

sl_bool Vibrator::cancel()
{
	return sl_true;
}
SLIB_MOBILE_NAMESPACE_END

#endif
