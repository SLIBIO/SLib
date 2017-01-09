#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/ui/notification.h"
#include "../../../inc/slib/ui/platform.h"

SLIB_UI_NAMESPACE_BEGIN

void _Notification_onTokenRefresh(JNIEnv* env, jobject _this, jstring token);
void _Notification_onMessageReceived(JNIEnv* env, jobject _this, jstring title, jstring body, jobjectArray data);

SLIB_JNI_BEGIN_CLASS(_JAndroidFirebaseInstanceIDService, "slib/platform/android/ui/FirebaseInstanceIDService")
SLIB_JNI_NATIVE(onCompleted, "nativeOnTokenRefresh", "(Ljava/lang/String;)V", _Notification_onTokenRefresh);
SLIB_JNI_STATIC_METHOD(getToken, "getToken", "()V");
SLIB_JNI_END_CLASS

SLIB_JNI_BEGIN_CLASS(_JAndroidFirebaseMessagingService, "slib/platform/android/ui/FirebaseMessagingService")
SLIB_JNI_NATIVE(onPrepared, "nativeOnMessageReceived", "(Ljava/lang/String;Ljava/lang/String;[Ljava/lang/String;)V", _Notification_onMessageReceived);
SLIB_JNI_END_CLASS


void PushNotification::registerForNotification()
{
	_JAndroidFirebaseInstanceIDService::getToken.call(sl_null);
}

void _Notification_onTokenRefresh(JNIEnv* env, jobject _this, jstring token)
{
	String tokenString = Jni::getString(token);
	Function<void (const String&)> callback = PushNotification::getTokenRefreshCallback();
	if (callback.isNotNull()) {
		callback(tokenString);
	}
}


void _Notification_onMessageReceived(JNIEnv* env, jobject _this, jstring title, jstring body, jobjectArray data)
{
	Map<String, String> _data;
	if (data) {
		sl_uint32 n = Jni::getArrayLength(data);
		if (n > 0) {
			_data.initHash(0, HashIgnoreCaseString(), CompareIgnoreCaseString());
			if (_data.isNotNull()) {
				for (sl_uint32 i = 0; i < n - 1; i += 2) {
					String key = Jni::getStringArrayElement(data, i);
					String value = Jni::getStringArrayElement(data, i + 1);
					_data.ref->put_NoLock(key, value, MapPutMode::AddAlways);
				}
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

SLIB_UI_NAMESPACE_END

#endif
