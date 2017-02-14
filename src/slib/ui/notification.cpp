#include "../../../inc/slib/ui/notification.h"
#include "../../../inc/slib/core/safe_static.h"

namespace slib
{

	PushNotificationMessage::PushNotificationMessage()
	{
	}

	PushNotificationMessage::~PushNotificationMessage()
	{
	}

	SLIB_SAFE_STATIC_GETTER(String, _getDeviceToken)
	SLIB_SAFE_STATIC_GETTER(Function<void (String)>, _getTokenRefreshCallback)
	SLIB_SAFE_STATIC_GETTER(Function<void(PushNotificationMessage&)>, _getNotificationReceivedCallback)
	
	String PushNotification::getDeviceToken()
	{
		String* token = _getDeviceToken();
		if (token) {
			return *token;
		}
		return String::null();
	}
	
	void PushNotification::setTokenRefreshCallback(const Function<void(String)>& callback)
	{
		Function<void (String)>* _refreshCallback = _getTokenRefreshCallback();
		if (_refreshCallback) {
			*_refreshCallback = callback;
		}
		
		_initToken();
	}
	
	void PushNotification::setNotificationReceivedCallback(const Function<void(PushNotificationMessage&)>& callback)
	{
		Function<void(PushNotificationMessage&)>* _receivedCallback = _getNotificationReceivedCallback();
		if (_receivedCallback) {
			*_receivedCallback = callback;
		}
	}
	
	Function<void(String)> PushNotification::getTokenRefreshCallback()
	{
		Function<void (String)>* callback = _getTokenRefreshCallback();
		if (callback) {
			return *callback;
		}
		return sl_null;
	}
	
	Function<void(PushNotificationMessage&)> PushNotification::getNotificationReceivedCallback()
	{
		Function<void(PushNotificationMessage&)>* callback = _getNotificationReceivedCallback();
		if (callback) {
			return *callback;
		}
		return sl_null;
	}

}
