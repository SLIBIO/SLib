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

	namespace priv
	{
		namespace device
		{

			SLIB_JNI_BEGIN_CLASS(JPoint, "android/graphics/Point")
				SLIB_JNI_INT_FIELD(x);
				SLIB_JNI_INT_FIELD(y);
			SLIB_JNI_END_CLASS

			SLIB_JNI_BEGIN_CLASS(JDevice, "slib/platform/android/device/Device")
				SLIB_JNI_STATIC_METHOD(checkPermissions, "checkPermissions", "(Landroid/app/Activity;I)Z");
				SLIB_JNI_STATIC_METHOD(grantPermissions, "grantPermissions", "(Landroid/app/Activity;I)V");

				SLIB_JNI_STATIC_METHOD(getIMEIs, "getIMEIs", "(Landroid/app/Activity;)Ljava/lang/String;");
				SLIB_JNI_STATIC_METHOD(getPhoneNumbers, "getPhoneNumbers", "(Landroid/app/Activity;)Ljava/lang/String;");
				SLIB_JNI_STATIC_METHOD(getDeviceId, "getDeviceId", "(Landroid/app/Activity;)Ljava/lang/String;");
				SLIB_JNI_STATIC_METHOD(getDeviceOSVersion, "getDeviceOSVersion", "()Ljava/lang/String;");
				SLIB_JNI_STATIC_METHOD(getDeviceName, "getDeviceName", "()Ljava/lang/String;");
				SLIB_JNI_STATIC_METHOD(getScreenSize, "getScreenSize", "(Landroid/app/Activity;)Landroid/graphics/Point;");
				SLIB_JNI_STATIC_METHOD(getScreenPPI, "getScreenPPI", "(Landroid/app/Activity;)I");

				SLIB_JNI_STATIC_METHOD(isSupportedDefaultCallingApp, "isSupportedDefaultCallingApp", "()Z");
				SLIB_JNI_STATIC_METHOD(isDefaultCallingApp, "isDefaultCallingApp", "(Landroid/app/Activity;)Z");
				SLIB_JNI_STATIC_METHOD(setDefaultCallingApp, "setDefaultCallingApp", "(Landroid/app/Activity;)V");
				SLIB_JNI_STATIC_METHOD(changeDefaultCallingApp, "changeDefaultCallingApp", "(Landroid/app/Activity;)V");
			SLIB_JNI_END_CLASS

			SLIB_STATIC_ZERO_INITIALIZED(AtomicFunction<void()>, g_callbackOnGrantPermission);

			SLIB_JNI_BEGIN_CLASS_SECTION(JDevice)
				SLIB_JNI_NATIVE_IMPL(nativeOnCallbackGrantPermissions, "nativeOnCallbackGrantPermissions", "()V", void, jlong instance)
				{
					if (SLIB_SAFE_STATIC_CHECK_FREED(g_callbackOnGrantPermission)) {
						return;
					}
					g_callbackOnGrantPermission();
					g_callbackOnGrantPermission.setNull();
				}
			SLIB_JNI_END_CLASS_SECTION

			SLIB_STATIC_ZERO_INITIALIZED(AtomicFunction<void()>, g_callbackOnSetDefaultCallingApp);

			SLIB_JNI_BEGIN_CLASS_SECTION(JDevice)
				SLIB_JNI_NATIVE_IMPL(nativeOnCallbackSetDefaultCallingApp, "nativeOnCallbackSetDefaultCallingApp", "()V", void, jlong instance)
				{
					if (SLIB_SAFE_STATIC_CHECK_FREED(g_callbackOnSetDefaultCallingApp)) {
						return;
					}
					g_callbackOnSetDefaultCallingApp();
					g_callbackOnSetDefaultCallingApp.setNull();
				}
			SLIB_JNI_END_CLASS_SECTION

		}
	}

	using namespace priv::device;

	sl_bool Device::checkPermissions(const DevicePermissions& permissions)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return JDevice::checkPermissions.callBoolean(sl_null, jactivity, (int)permissions);
		}
		return sl_null;
	}
	
	void Device::grantPermissions(const DevicePermissions& permissions, const Function<void()>& callback)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(g_callbackOnGrantPermission)) {
			return;
		}
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			g_callbackOnGrantPermission();
			g_callbackOnGrantPermission = callback;
			JDevice::grantPermissions.call(sl_null, jactivity, (int)permissions);
		}
	}

	List<String> Device::getIMEIs()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return String(JDevice::getIMEIs.callString(sl_null, jactivity)).split(";");
		}
		return sl_null;
	}

	List<String> Device::getPhoneNumbers()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return String(JDevice::getPhoneNumbers.callString(sl_null, jactivity)).split(";");
		}
		return sl_null;
	}

	String Device::getDeviceId()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return JDevice::getDeviceId.callString(sl_null, jactivity);
		}
		return sl_null;
	}

	String Device::getDeviceName()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return JDevice::getDeviceName.callString(sl_null);
		}
		return sl_null;
	}

	String Device::getSystemVersion()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return JDevice::getDeviceOSVersion.callString(sl_null);
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
			JniLocal<jobject> jpt = JDevice::getScreenSize.callObject(sl_null, jactivity);
			if (jpt.isNotNull()) {
				Sizei ret;
				ret.x = JPoint::x.get(jpt);
				ret.y = JPoint::y.get(jpt);
				return ret;
			}
		}
		return Sizei::zero();
	}

	double Device::getScreenPPI()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return JDevice::getScreenPPI.callInt(sl_null, jactivity);
		}
		return 0;
	}

	sl_bool Device::isSupportedDefaultCallingApp()
	{
		return JDevice::isSupportedDefaultCallingApp.callBoolean(sl_null);
	}

	sl_bool Device::isDefaultCallingApp()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return JDevice::isDefaultCallingApp.callBoolean(sl_null, jactivity);
		}
		return sl_false;
	}

	void Device::setDefaultCallingApp(const Function<void()>& callback)
	{
		if (SLIB_SAFE_STATIC_CHECK_FREED(g_callbackOnSetDefaultCallingApp)) {
			return;
		}
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			g_callbackOnSetDefaultCallingApp();
			g_callbackOnSetDefaultCallingApp = callback;
			JDevice::setDefaultCallingApp.call(sl_null, jactivity);
		}
	}

	void Device::changeDefaultCallingApp()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JDevice::changeDefaultCallingApp.call(sl_null, jactivity);
		}
	}

}

#endif
