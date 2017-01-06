#include "../../../inc/slib/ui/notification.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include <UIKit/UIKit.h>

SLIB_UI_NAMESPACE_BEGIN

void Notification::registerForNotification()
{
	UIApplication* application = [UIApplication sharedApplication];
	[application registerForRemoteNotifications];
}

SLIB_UI_NAMESPACE_END

#endif
