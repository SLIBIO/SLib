/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "slib/device/device.h"

#include "slib/core/variant.h"
#include "slib/core/platform_android.h"
#include "slib/core/safe_static.h"

namespace slib
{
	SLIB_JNI_BEGIN_CLASS(JAndroidPoint, "android/graphics/Point")
		SLIB_JNI_INT_FIELD(x);
		SLIB_JNI_INT_FIELD(y);
	SLIB_JNI_END_CLASS

	SLIB_JNI_BEGIN_CLASS(JAndroidDevice, "slib/platform/android/device/Device")
		SLIB_JNI_STATIC_METHOD(checkPermissions, "checkPermissions", "(Landroid/app/Activity;I)Z");
		SLIB_JNI_STATIC_METHOD(grantPermissions, "grantPermissions", "(Landroid/app/Activity;I)V");
		SLIB_JNI_STATIC_METHOD(getIMEIs, "getIMEIs", "(Landroid/app/Activity;)Ljava/lang/String;");
		SLIB_JNI_STATIC_METHOD(getPhoneNumbers, "getPhoneNumbers", "(Landroid/app/Activity;)Ljava/lang/String;");
		SLIB_JNI_STATIC_METHOD(getDeviceId, "getDeviceId", "(Landroid/app/Activity;)Ljava/lang/String;");
		SLIB_JNI_STATIC_METHOD(getDeviceOSVersion, "getDeviceOSVersion", "()Ljava/lang/String;");
		SLIB_JNI_STATIC_METHOD(getDeviceName, "getDeviceName", "()Ljava/lang/String;");
		SLIB_JNI_STATIC_METHOD(getScreenSize, "getScreenSize", "(Landroid/app/Activity;)Landroid/graphics/Point;");
		SLIB_JNI_STATIC_METHOD(getScreenPPI, "getScreenPPI", "(Landroid/app/Activity;)I");
	SLIB_JNI_END_CLASS

	sl_bool Device::checkPermissions(const DevicePermissions& permissions)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return JAndroidDevice::checkPermissions.callBoolean(sl_null, jactivity, (int)permissions);
		}
		return sl_null;
	}
	
	SLIB_STATIC_ZERO_INITIALIZED(AtomicFunction<void()>, _g_priv_onGrantPermissionCallback);

	void Device::grantPermissions(const DevicePermissions& permissions, const Function<void()>& callback)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_onGrantPermissionCallback)) {
			return;
		}
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			_g_priv_onGrantPermissionCallback = callback;
			JAndroidDevice::grantPermissions.call(sl_null, jactivity, (int)permissions);
		}
	}

	SLIB_JNI_BEGIN_CLASS_SECTION(JAndroidDevice)
		SLIB_JNI_NATIVE_IMPL(nativeOnCallbackGrantPermissions, "nativeOnCallbackGrantPermissions", "()V", void, jlong instance)
		{
			if (SLIB_SAFE_STATIC_CHECK_FREED(_g_priv_onGrantPermissionCallback)) {
				return;
			}
			_g_priv_onGrantPermissionCallback();
			_g_priv_onGrantPermissionCallback.setNull();
		}
	SLIB_JNI_END_CLASS_SECTION

	List<String> Device::getIMEIs()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return JAndroidDevice::getIMEIs.callString(sl_null, jactivity).split(";");
		}
		return sl_null;
	}

	List<String> Device::getPhoneNumbers()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return JAndroidDevice::getPhoneNumbers.callString(sl_null, jactivity).split(";");
		}
		return sl_null;
	}

	String Device::getDeviceId()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return JAndroidDevice::getDeviceId.callString(sl_null, jactivity);
		}
		return sl_null;
	}

	String Device::getDeviceName()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return JAndroidDevice::getDeviceName.callString(sl_null);
		}
		return sl_null;
	}

	String Device::getSystemVersion()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return JAndroidDevice::getDeviceOSVersion.callString(sl_null);
		}
		return sl_null;
	}

	String Device::getSystemName()
	{
		String osVersion = getSystemVersion();
		return String::format("Android %s", osVersion);
	}

	Sizei Device::getScreenSize()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JniLocal<jobject> jpt = JAndroidDevice::getScreenSize.callObject(sl_null, jactivity);
			if (jpt.isNotNull()) {
				Sizei ret;
				ret.x = JAndroidPoint::x.get(jpt);
				ret.y = JAndroidPoint::y.get(jpt);
				return ret;
			}
		}
		return Sizei::zero();
	}

	double Device::getScreenPPI()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return JAndroidDevice::getScreenPPI.callInt(sl_null, jactivity);
		}
		return 0;
	}

}

#endif
