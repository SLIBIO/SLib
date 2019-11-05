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
				SLIB_JNI_STATIC_METHOD(getIMEIs, "getIMEIs", "(Landroid/app/Activity;)Ljava/lang/String;");
				SLIB_JNI_STATIC_METHOD(getPhoneNumbers, "getPhoneNumbers", "(Landroid/app/Activity;)Ljava/lang/String;");
				SLIB_JNI_STATIC_METHOD(getSimSlotsCount, "getSimSlotsCount", "(Landroid/app/Activity;)I");
				SLIB_JNI_STATIC_METHOD(getPhoneNumber, "getPhoneNumber", "(Landroid/app/Activity;I)Ljava/lang/String;");
				SLIB_JNI_STATIC_METHOD(getDeviceId, "getDeviceId", "(Landroid/app/Activity;)Ljava/lang/String;");
				SLIB_JNI_STATIC_METHOD(getDeviceOSVersion, "getDeviceOSVersion", "()Ljava/lang/String;");
				SLIB_JNI_STATIC_METHOD(getDeviceName, "getDeviceName", "()Ljava/lang/String;");
				SLIB_JNI_STATIC_METHOD(getScreenSize, "getScreenSize", "(Landroid/app/Activity;)Landroid/graphics/Point;");
				SLIB_JNI_STATIC_METHOD(getScreenPPI, "getScreenPPI", "(Landroid/app/Activity;)I");

				SLIB_JNI_STATIC_METHOD(openURL, "openURL", "(Landroid/app/Activity;Ljava/lang/String;)V");
			SLIB_JNI_END_CLASS

			SLIB_JNI_BEGIN_CLASS(JPhoneCall, "slib/platform/android/device/PhoneCall")
				SLIB_JNI_STATIC_METHOD(openDial, "openDial", "(Landroid/app/Activity;Ljava/lang/String;)V");
				SLIB_JNI_STATIC_METHOD(callPhone, "callPhone", "(Landroid/app/Activity;Ljava/lang/String;)V");
				SLIB_JNI_STATIC_METHOD(callPhoneWithSim, "callPhone", "(Landroid/app/Activity;Ljava/lang/String;I)V");
				SLIB_JNI_STATIC_METHOD(answerCall, "answerCall", "(Ljava/lang/String;)V");
				SLIB_JNI_STATIC_METHOD(endCall, "endCall", "(Ljava/lang/String;)V");
			SLIB_JNI_END_CLASS

			SLIB_STATIC_ZERO_INITIALIZED(Atomic<PhoneCallCallback>, g_callbackOnIncomingCall)

			SLIB_JNI_BEGIN_CLASS_SECTION(JPhoneCall)
				SLIB_JNI_NATIVE_IMPL(nativeOnIncomingCall, "nativeOnIncomingCall", "(Ljava/lang/String;Ljava/lang/String;)V", void, jstring callId, jstring phoneNumber)
				{
					g_callbackOnIncomingCall(Jni::getString(callId), Jni::getString(phoneNumber));
				}
			SLIB_JNI_END_CLASS_SECTION

			SLIB_STATIC_ZERO_INITIALIZED(Atomic<PhoneCallCallback>, g_callbackOnOutgoingCall)

			SLIB_JNI_BEGIN_CLASS_SECTION(JPhoneCall)
				SLIB_JNI_NATIVE_IMPL(nativeOnOutgoingCall, "nativeOnOutgoingCall", "(Ljava/lang/String;Ljava/lang/String;)V", void, jstring callId, jstring phoneNumber)
				{
					g_callbackOnOutgoingCall(Jni::getString(callId), Jni::getString(phoneNumber));
				}
			SLIB_JNI_END_CLASS_SECTION

			SLIB_STATIC_ZERO_INITIALIZED(Atomic<PhoneCallCallback>, g_callbackOnEndCall)

			SLIB_JNI_BEGIN_CLASS_SECTION(JPhoneCall)
				SLIB_JNI_NATIVE_IMPL(nativeOnEndCall, "nativeOnEndCall", "(Ljava/lang/String;Ljava/lang/String;)V", void, jstring callId, jstring phoneNumber)
				{
					g_callbackOnEndCall(Jni::getString(callId), Jni::getString(phoneNumber));
				}
			SLIB_JNI_END_CLASS_SECTION

		}
	}

	using namespace priv::device;

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

	sl_uint32 Device::getSimSlotsCount()
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return (sl_uint32)(JDevice::getSimSlotsCount.callInt(sl_null, jactivity));
		}
		return 1;
	}

	String Device::getPhoneNumber(sl_uint32 indexSIM)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			return JDevice::getPhoneNumber.callString(sl_null, jactivity, (jint)indexSIM);
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
		return "Android " + osVersion;
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

	void Device::openUrl(const String& _url) {
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JniLocal<jstring> jurl = Jni::getJniString(_url);
			JDevice::openURL.call(sl_null, jactivity, jurl.get());
		}
	}

	void Device::openDial(const String& phoneNumber)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JniLocal<jstring> tel = Jni::getJniString(phoneNumber);
			JPhoneCall::openDial.call(sl_null, jactivity, tel.get());
		}
	}

	void Device::callPhone(const String& phoneNumber)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JniLocal<jstring> tel = Jni::getJniString(phoneNumber);
			JPhoneCall::callPhone.call(sl_null, jactivity, tel.get());
		}
	}

	void Device::callPhone(const String& phoneNumber, sl_uint32 indexSIM)
	{
		jobject jactivity = Android::getCurrentActivity();
		if (jactivity) {
			JniLocal<jstring> tel = Jni::getJniString(phoneNumber);
			JPhoneCall::callPhoneWithSim.call(sl_null, jactivity, tel.get(), (jint)indexSIM);
		}
	}

	void Device::answerCall(const String& callId)
	{
		JniLocal<jstring> s = Jni::getJniString(callId);
		JPhoneCall::answerCall.call(sl_null, s.get());
	}

	void Device::endCall(const String& callId)
	{
		JniLocal<jstring> s = Jni::getJniString(callId);
		JPhoneCall::endCall.call(sl_null, s.get());
	}

	void Device::addOnIncomingCall(const PhoneCallCallback& callback)
	{
		g_callbackOnIncomingCall.add(callback);
	}

	void Device::removeOnIncomingCall(const PhoneCallCallback& callback)
	{
		g_callbackOnIncomingCall.remove(callback);
	}

	void Device::addOnOutgoingCall(const PhoneCallCallback& callback)
	{
		g_callbackOnOutgoingCall.add(callback);
	}

	void Device::removeOnOutgoingCall(const PhoneCallCallback& callback)
	{
		g_callbackOnOutgoingCall.remove(callback);
	}

	void Device::addOnEndCall(const PhoneCallCallback& callback)
	{
		g_callbackOnEndCall.add(callback);
	}

	void Device::removeOnEndCall(const PhoneCallCallback& callback)
	{
		g_callbackOnEndCall.remove(callback);
	}

}

#endif
