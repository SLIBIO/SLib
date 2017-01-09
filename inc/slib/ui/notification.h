#ifndef CHECKHEADER_SLIB_UI_NOTIFICATION
#define CHECKHEADER_SLIB_UI_NOTIFICATION

#include "definition.h"

#include "../core/string.h"
#include "../core/variant.h"
#include "../core/function.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT PushNotificationMessage
{
public:
	String title;
	String body;
	Map<String, String> data;
};

class SLIB_EXPORT PushNotification
{
public:
	static void registerForNotification();
	static String getDeviceToken();
	static void setTokenRefreshCallback(const Function<void(String)>& callback);
	static void setNotificationReceivedCallback(const Function<void(PushNotificationMessage&)>& callback);
	static Function<void(String)> getTokenRefreshCallback();
	static Function<void(PushNotificationMessage&)> getNotificationReceivedCallback();
};

SLIB_UI_NAMESPACE_END

#endif
