#include "../../../inc/slib/device/vibrator.h"

#if defined(SLIB_PLATFORM_IS_DESKTOP)

SLIB_DEVICE_NAMESPACE_BEGIN
Ref<Vibrator> Vibrator::create()
{
	return Ref<Vibrator>::null();
}
SLIB_DEVICE_NAMESPACE_END

#endif
