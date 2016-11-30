#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/device/vibrator.h"

#import <AudioToolbox/AudioServices.h>

SLIB_DEVICE_NAMESPACE_BEGIN

sl_bool Vibrator::vibrate(sl_int32 millisec)
{
	AudioServicesPlayAlertSound(kSystemSoundID_Vibrate);
	return sl_true;
}

SLIB_DEVICE_NAMESPACE_END

#endif
