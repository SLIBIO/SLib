/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "slib/device/information.h"

#include "slib/core/variant.h"
#include "slib/core/platform_apple.h"

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
