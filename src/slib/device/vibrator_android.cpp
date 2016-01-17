#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/device/vibrator.h"

#include "../../../inc/slib/core/platform_android.h"

SLIB_DEVICE_NAMESPACE_BEGIN

SLIB_JNI_BEGIN_CLASS(_JAndroidVibrator, "slib/platform/android/device/Vibrator")
	SLIB_JNI_STATIC_METHOD(vibrate, "vibrate", "(Landroid/app/Activity;I)Z");
	SLIB_JNI_STATIC_METHOD(cancel, "cancel", "(Landroid/app/Activity)Z");
SLIB_JNI_END_CLASS

class _Android_Vibrator : public Vibrator
{
public:
	sl_bool vibrate(sl_int32 millisec)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			sl_bool ret = _JAndroidVibrator::vibrate.callBoolean(sl_null, jactivity, millisec);
			return ret;
		}
		return sl_false;
	}

	sl_bool cancel()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			sl_bool ret = _JAndroidVibrator::cancel.callBoolean(sl_null, jactivity);
			return ret;
		}
		return sl_false;
	}
};

Ref<Vibrator> Vibrator::create()
{
	return new _Android_Vibrator;
}

SLIB_DEVICE_NAMESPACE_END

#endif
