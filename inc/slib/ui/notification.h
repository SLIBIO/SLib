#ifndef CHECKHEADER_SLIB_UI_NOTIFICATION
#define CHECKHEADER_SLIB_UI_NOTIFICATION

#include "definition.h"

#include "../core/string.h"
#include "../core/variant.h"
#include "../core/function.h"

namespace slib
{

	class SLIB_EXPORT PushNotificationMessage
	{
	public:
		String title;
		String body;
		Map<String, Variant> data;

	public:
		PushNotificationMessage();

		~PushNotificationMessage();

	};
	
	class SLIB_EXPORT PushNotification
	{
	public:
		static String getDeviceToken();
		static void setTokenRefreshCallback(const Function<void(String)>& callback);
		static void setNotificationReceivedCallback(const Function<void(PushNotificationMessage&)>& callback);
		static Function<void(String)> getTokenRefreshCallback();
		static Function<void(PushNotificationMessage&)> getNotificationReceivedCallback();
		
	private:
		static void _initToken();
	};

}

#endif
