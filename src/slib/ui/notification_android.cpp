/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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


	void PushNotification::_initToken()
	{
		JAndroidFirebaseInstanceIDService::getToken.call(sl_null);
	}

	void _priv_Notification_onTokenRefresh(JNIEnv* env, jobject _this, jstring token)
	{
		String tokenString = Jni::getString(token);
		Function<void (String)> callback = PushNotification::getTokenRefreshCallback();
		if (callback.isNotNull()) {
			callback(tokenString);
		}
	}


	void _priv_Notification_onMessageReceived(JNIEnv* env, jobject _this, jstring title, jstring body, jobjectArray data)
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
		String _body = Jni::getString(body);

		PushNotificationMessage message;
		message.title = _title;
		message.body = _body;
		message.data = _data;

		Function<void(PushNotificationMessage&)> callback = PushNotification::getNotificationReceivedCallback();
		if (callback.isNotNull()) {
			callback(message);
		}
	}

}

#endif
