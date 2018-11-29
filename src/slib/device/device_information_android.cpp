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

#include "slib/device/information.h"

#include "slib/core/variant.h"
#include "slib/core/platform_android.h"

namespace slib
{

	SLIB_JNI_BEGIN_CLASS(JAndroidPoint, "android/graphics/Point")
		SLIB_JNI_INT_FIELD(x);
		SLIB_JNI_INT_FIELD(y);
	SLIB_JNI_END_CLASS

	SLIB_JNI_BEGIN_CLASS(JAndroidDeviceInformation, "slib/platform/android/device/DeviceInformation")
		SLIB_JNI_STATIC_METHOD(getDeviceId, "getDeviceId", "(Landroid/app/Activity;)Ljava/lang/String;");
		SLIB_JNI_STATIC_METHOD(getDeviceOSVersion, "getDeviceOSVersion", "()Ljava/lang/String;");
		SLIB_JNI_STATIC_METHOD(getDeviceName, "getDeviceName", "()Ljava/lang/String;");
		SLIB_JNI_STATIC_METHOD(getScreenSize, "getScreenSize", "(Landroid/app/Activity;)Landroid/graphics/Point;");
		SLIB_JNI_STATIC_METHOD(getDevicePPI, "getDevicePPI", "(Landroid/app/Activity;)I");
	SLIB_JNI_END_CLASS

	String DeviceInformation::getDeviceId()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return JAndroidDeviceInformation::getDeviceId.callString(sl_null, jactivity);
		}
		return sl_null;
	}

	String DeviceInformation::getDeviceName()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return JAndroidDeviceInformation::getDeviceName.callString(sl_null);
		}
		return sl_null;
	}

	String DeviceInformation::getSystemVersion()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return JAndroidDeviceInformation::getDeviceOSVersion.callString(sl_null);
		}
		return sl_null;
	}

	String DeviceInformation::getSystemName()
	{
		String osVersion = getSystemVersion();
		return String::format("Android %s", osVersion);
	}

	Size DeviceInformation::getScreenSize()
	{
		Size ret;
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JniLocal<jobject> jpt = JAndroidDeviceInformation::getScreenSize.callObject(sl_null, jactivity);
			if (jpt.isNotNull()) {
				ret.x = JAndroidPoint::x.get(jpt);
				ret.y = JAndroidPoint::y.get(jpt);
			}
		}
		return ret;
	}

	sl_uint32 DeviceInformation::getDevicePPI()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return (sl_uint32)(JAndroidDeviceInformation::getDevicePPI.callInt(sl_null, jactivity));
		}
		return 0;
	}

}

#endif
