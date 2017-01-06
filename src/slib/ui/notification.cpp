#include "../../../inc/slib/ui/notification.h"
#include "../../../inc/slib/core/safe_static.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_SAFE_STATIC_GETTER(String, _getDeviceToken)
SLIB_SAFE_STATIC_GETTER(Function<void (const String &)>, _getTokenRefreshCallback)
SLIB_SAFE_STATIC_GETTER(Function<void (const Map<String, Variant>&)>, _getNotificationReceivedCallback)

String Notification::getDeviceToken()
{
	String* token = _getDeviceToken();
	if (token) {
		return *token;
	}
	return String::null();
}

void Notification::setTokenRefreshCallback(const Function<void(String const&)>& callback)
{
	Function<void (const String&)>* _refreshCallback = _getTokenRefreshCallback();
	if (_refreshCallback) {
		*_refreshCallback = callback;
	}
}

void Notification::setNotificationReceivedCallback(const Function<void (const Map<String, Variant>&)>& callback)
{
	Function<void (const Map<String, Variant>&)>* _receivedCallback = _getNotificationReceivedCallback();
	if (_receivedCallback) {
		*_receivedCallback = callback;
	}
}

Function<void (const String &)> Notification::getTokenRefreshCallback()
{
	Function<void (const String &)>* callback = _getTokenRefreshCallback();
	if (callback) {
		return *callback;
	}
	return sl_null;
}

Function<void (const Map<String, Variant>&)> Notification::getNotificationReceivedCallback()
{
	Function<void (const Map<String, Variant>&)>* callback = _getNotificationReceivedCallback();
	if (callback) {
		return *callback;
	}
	return sl_null;
}

SLIB_UI_NAMESPACE_END
