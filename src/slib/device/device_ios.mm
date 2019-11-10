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

#include "slib/device/device.h"
#include "slib/core/variant.h"
#include "slib/core/platform_apple.h"

#import <UIKit/UIKit.h>
#import <sys/utsname.h>

namespace slib
{

	String Device::getDeviceId()
	{
		UIDevice* device = [UIDevice currentDevice];
		NSString* currentDeviceId = [[device identifierForVendor] UUIDString];
		return Apple::getStringFromNSString(currentDeviceId);
	}
	
	String Device::getDeviceName()
	{
		struct utsname systemInfo;
		uname(&systemInfo);
		return systemInfo.machine;
	}
	
	String Device::getSystemVersion()
	{
		return Apple::getSystemVersion();
	}
	
	String Device::getSystemName()
	{
		String osVersion = getSystemVersion();
		return String::format("iOS %s", osVersion);
	}
	
	Sizei Device::getScreenSize()
	{
		UIScreen* screen = [UIScreen mainScreen];
		if (screen != nil) {
			Sizei ret;
			CGRect screenRect = screen.bounds;
			CGFloat scale = screen.scale;
			ret.x = (int)(screenRect.size.width * scale);
			ret.y = (int)(screenRect.size.height * scale);
			return ret;
		}
		return Sizei::zero();
	}
	
	double Device::getScreenPPI()
	{
		return [[UIScreen mainScreen] scale] * 160;
	}

	void Device::openUrl(const String& _url)
	{
		if (_url.isNotEmpty()) {
			if (![NSThread isMainThread]) {
				String url = _url;
				dispatch_async(dispatch_get_main_queue(), ^{
					Device::openUrl(url);
				});
			} else {
				NSString* s = Apple::getNSStringFromString(_url);
				NSURL* url = [NSURL URLWithString:s];
#ifdef SLIB_PLATFORM_IS_IOS_CATALYST
				[[UIApplication sharedApplication] openURL:url options:@{} completionHandler:nil];
#else
				[[UIApplication sharedApplication] openURL:url];
#endif
			}
		}
	}

}

#endif
