#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/device/information.h"
#include "../../../inc/slib/core/platform_android.h"
#include "../../../inc/slib/core/variant.h"

SLIB_DEVICE_NAMESPACE_BEGIN

SLIB_JNI_BEGIN_CLASS(_JAndroidDeviceInformation, "slib/platform/android/device/DeviceInformation")
	SLIB_JNI_STATIC_METHOD(getDeviceId, "getDeviceId", "(Landroid/app/Activity;)Ljava/lang/String;");
	SLIB_JNI_STATIC_METHOD(getDeviceOSVersion, "getDeviceOSVersion", "()Ljava/lang/String;");
	SLIB_JNI_STATIC_METHOD(getDeviceName, "getDeviceName", "()Ljava/lang/String;");
SLIB_JNI_END_CLASS

String DeviceInformation::getDeviceId()
{
	jobject jactivity = Android::getCurrentActivity();
	if (jactivity) {
		return _JAndroidDeviceInformation::getDeviceId.callString(sl_null, jactivity);
	}
	return String::null();
}

String DeviceInformation::getDeviceName()
{
	jobject jactivity = Android::getCurrentActivity();
	if (jactivity) {
		return _JAndroidDeviceInformation::getDeviceName.callString(sl_null);
	}
	return String::null();
}

String DeviceInformation::getSystemVersion()
{
	jobject jactivity = Android::getCurrentActivity();
	if (jactivity) {
		return _JAndroidDeviceInformation::getDeviceOSVersion.callString(sl_null);
	}
	return String::null();
}

String DeviceInformation::getSystemName()
{
	String osVersion = getSystemVersion();
	return String::format("Android %s", osVersion);
}

SLIB_DEVICE_NAMESPACE_END

#endif
