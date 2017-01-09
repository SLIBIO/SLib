#include "../../../inc/slib/ui/notification.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include <UIKit/UIKit.h>

SLIB_UI_NAMESPACE_BEGIN

void PushNotification::registerForNotification()
{
	UIApplication* application = [UIApplication sharedApplication];
	UIUserNotificationSettings* notificationSettings = [UIUserNotificationSettings settingsForTypes:UIUserNotificationTypeBadge | UIUserNotificationTypeSound | UIUserNotificationTypeAlert categories:nil];
	[application registerUserNotificationSettings:notificationSettings];
}

SLIB_UI_NAMESPACE_END

#endif
