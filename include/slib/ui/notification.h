/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_UI_NOTIFICATION
#define CHECKHEADER_SLIB_UI_NOTIFICATION

#include "definition.h"

#include "../core/string.h"
#include "../core/json.h"
#include "../core/function.h"

namespace slib
{

	class SLIB_EXPORT PushNotificationMessage
	{
	public:
		String title;
		String body;
		JsonMap data;

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
