#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/mobile/vibrator.h"
#include "../../../inc/slib/core/platform_android.h"

SLIB_MOBILE_NAMESPACE_BEGIN

SLIB_JNI_BEGIN_CLASS(_JAndroidVibrator, "slib/platform/android/mobile/Vibrator")
	SLIB_JNI_STATIC_METHOD(vibrate, "vibrate", "(Landroid/app/Activity;I)Z");
	SLIB_JNI_STATIC_METHOD(cancel, "cancel", "(Landroid/app/Activity)Z");
SLIB_JNI_END_CLASS

class _Android_Vibrator : public Vibrator
{
public:
	_Android_Vibrator(){}

	~_Android_Vibrator(){}

	static sl_bool vibrate(sl_int32 millisec)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			sl_bool ret = _JAndroidVibrator::vibrate.callBoolean(sl_null, jactivity, millisec);
			return ret;
		}
		return sl_false;
	}

	static sl_bool cancel()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			sl_bool ret = _JAndroidVibrator::cancel.callBoolean(sl_null, jactivity);
			return ret;
		}
		return sl_false;
	}
};

sl_bool Vibrator::vibrate(sl_int32 millisec)
{
	return _Android_Vibrator::vibrate(millisec);
}

sl_bool Vibrator::cancel()
{
	return _Android_Vibrator::cancel();
}
SLIB_MOBILE_NAMESPACE_END

#endif
