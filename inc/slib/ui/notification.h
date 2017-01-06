#ifndef CHECKHEADER_SLIB_UI_NOTIFICATION
#define CHECKHEADER_SLIB_UI_NOTIFICATION

#include "definition.h"

#include "../core/string.h"
#include "../core/variant.h"
#include "../core/function.h"

SLIB_UI_NAMESPACE_BEGIN

class SLIB_EXPORT Notification
{
public:
	static void registerForNotification();
	static String getDeviceToken();
	static void setTokenRefreshCallback(const Function<void(String const&)>& callback);
	static void setNotificationReceivedCallback(const Function<void (const Map<String, Variant>&)>& callback);
	static Function<void(String const&)> getTokenRefreshCallback();
	static Function<void (const Map<String, Variant>&)> getNotificationReceivedCallback();
};

SLIB_UI_NAMESPACE_END

#endif
