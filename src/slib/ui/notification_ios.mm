/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/ui/notification.h"

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
