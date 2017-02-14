#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/device/vibrator.h"

#include "../../../inc/slib/core/platform_android.h"

namespace slib
{

	SLIB_JNI_BEGIN_CLASS(_JAndroidVibrator, "slib/platform/android/device/Vibrator")
		SLIB_JNI_STATIC_METHOD(vibrate, "vibrate", "(Landroid/app/Activity;I)Z");
	SLIB_JNI_END_CLASS

	sl_bool Vibrator::vibrate(sl_int32 millisec)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			sl_bool ret = _JAndroidVibrator::vibrate.callBoolean(sl_null, jactivity, millisec);
			return ret;
		}
		return sl_false;
	}

}

#endif
