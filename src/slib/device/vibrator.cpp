#include "../../../inc/slib/device/vibrator.h"

SLIB_DEVICE_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(Vibrator, Object);

SLIB_DEVICE_NAMESPACE_END

#if defined(SLIB_PLATFORM_IS_DESKTOP)

SLIB_DEVICE_NAMESPACE_BEGIN

Ref<Vibrator> Vibrator::create()
{
	return Ref<Vibrator>::null();
}

SLIB_DEVICE_NAMESPACE_END

#endif
