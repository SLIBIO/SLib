/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/device/information.h"

#include "../../../inc/slib/core/variant.h"
#include "../../../inc/slib/core/platform_apple.h"

#import <UIKit/UIKit.h>
#import <sys/utsname.h>

namespace slib
{

	String DeviceInformation::getDeviceId()
	{
		UIDevice* device = [UIDevice currentDevice];
		NSString* currentDeviceId = [[device identifierForVendor] UUIDString];
		return Apple::getStringFromNSString(currentDeviceId);
	}

	String DeviceInformation::getDeviceName()
	{
		struct utsname systemInfo;
		uname(&systemInfo);
		
		return systemInfo.machine;
	}

	String DeviceInformation::getSystemVersion()
	{
		UIDevice* device = [UIDevice currentDevice];
		return Apple::getStringFromNSString(device.systemVersion);
	}

	String DeviceInformation::getSystemName()
	{
		String osVersion = getSystemVersion();
		return String::format("iOS %s", osVersion);
	}

	Size DeviceInformation::getScreenSize()
	{
		CGRect screenRect = [[UIScreen mainScreen] bounds];
		Size ret;
		CGFloat scale = [[UIScreen mainScreen] scale];
		ret.x = (sl_real)(screenRect.size.width * scale);
		ret.y = (sl_real)(screenRect.size.height * scale);
		return ret;
	}

	sl_uint32 DeviceInformation::getDevicePPI()
	{
		return (sl_uint32) ([[UIScreen mainScreen] scale] * 160);
	}

}

#endif
