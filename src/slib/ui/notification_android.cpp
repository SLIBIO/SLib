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

#include "slib/ui/notification.h"
#include "slib/ui/platform.h"
#include "slib/core/json.h"

namespace slib
{

	void _priv_Notification_onTokenRefresh(JNIEnv* env, jobject _this, jstring token);
	void _priv_Notification_onMessageReceived(JNIEnv* env, jobject _this, jstring title, jstring body, jobjectArray data);

	SLIB_JNI_BEGIN_CLASS(JAndroidFirebaseInstanceIDService, "slib/platform/android/ui/FirebaseInstanceIDService")
		SLIB_JNI_NATIVE(onCompleted, "nativeOnTokenRefresh", "(Ljava/lang/String;)V", _priv_Notification_onTokenRefresh);
		SLIB_JNI_STATIC_METHOD(getToken, "getToken", "()V");
	SLIB_JNI_END_CLASS

	SLIB_JNI_BEGIN_CLASS(JAndroidFirebaseMessagingService, "slib/platform/android/ui/FirebaseMessagingService")
		SLIB_JNI_NATIVE(onPrepared, "nativeOnMessageReceived", "(Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;)V", _priv_Notification_onMessageReceived);
	SLIB_JNI_END_CLASS


	void PushNotification::_doInit()
	{
		JAndroidFirebaseInstanceIDService::getToken.call(sl_null);
	}

	void _priv_Notification_onTokenRefresh(JNIEnv* env, jobject _this, jstring _token)
	{
		String token = Jni::getString(_token);
		PushNotification::_onRefreshToken(token);
	}


	void _priv_Notification_onMessageReceived(JNIEnv* env, jobject _this, jstring title, jstring content, jobjectArray data)
	{
		JsonMap _data;
		if (data) {
			sl_uint32 n = Jni::getArrayLength(data);
			if (n > 0) {
				for (sl_uint32 i = 0; i + 1 < n; i += 2) {
					String key = Jni::getStringArrayElement(data, i);
					String strValue = Jni::getStringArrayElement(data, i + 1);
					Json value;
					if (strValue.isNotEmpty()) {
						JsonParseParam p;
						p.flagLogError = sl_false;
						value = Json::parseJson(strValue, p);
						if (value.isNull()) {
							value = strValue;
						}
					} else {
						value = strValue;
					}
					_data.add_NoLock(key, value);
				}
			}
		}

		String _title = Jni::getString(title);
		String _content = Jni::getString(content);

		PushNotificationMessage message;
		message.title = _title;
		message.content = _content;
		message.data = _data;

		PushNotification::_onNotificationReceived(message);
	}

}

#endif
