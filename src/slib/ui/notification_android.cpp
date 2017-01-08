#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/ui/notification.h"
#include "../../../inc/slib/ui/platform.h"

SLIB_UI_NAMESPACE_BEGIN

void _Notification_onTokenRefresh(JNIEnv* env, jobject _this, jstring token);
void _Notification_onMessageReceived(JNIEnv* env, jobject _this, jstring message);

SLIB_JNI_BEGIN_CLASS(_JAndroidFirebaseInstanceIDService, "slib/platform/android/ui/SLFirebaseInstanceIDService")
SLIB_JNI_NATIVE(onCompleted, "nativeOnTokenRefresh", "(Ljava/lang/String;)V", _Notification_onTokenRefresh);
SLIB_JNI_END_CLASS

SLIB_JNI_BEGIN_CLASS(_JAndroidFirebaseMessagingService, "slib/platform/android/ui/SLFirebaseMessagingService")
SLIB_JNI_NATIVE(onPrepared, "nativeOnMessageReceived", "(Ljava/lang/String;)V", _Notification_onMessageReceived);
SLIB_JNI_END_CLASS


void Notification::registerForNotification()
{
	
}


void _Notification_onTokenRefresh(JNIEnv* env, jobject _this, jstring token)
{
	String tokenString = Jni::getString(token);
	Function<void (const String&)> callback = Notification::getTokenRefreshCallback();
	if (callback.isNotNull()) {
		callback(tokenString);
	}
}


void _Notification_onMessageReceived(JNIEnv* env, jobject _this, jstring message)
{
	String messageBody = Jni::getString(message);
	Function<void (const Map<String, Variant>&)> callback = Notification::getNotificationReceivedCallback();
	if (callback.isNotNull()) {
		callback(sl_null);
	}
}

SLIB_UI_NAMESPACE_END

#endif
