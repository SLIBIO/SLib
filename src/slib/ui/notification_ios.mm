#include "../../../inc/slib/ui/notification.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include <UIKit/UIKit.h>

namespace slib
{
	void PushNotification::_initToken()
	{
		UIApplication* application = [UIApplication sharedApplication];
		UIUserNotificationSettings* notificationSettings = [UIUserNotificationSettings settingsForTypes:UIUserNotificationTypeBadge | UIUserNotificationTypeSound | UIUserNotificationTypeAlert categories:nil];
		[application registerUserNotificationSettings:notificationSettings];
	}
}

#endif
