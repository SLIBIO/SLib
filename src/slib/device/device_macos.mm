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

#if defined(SLIB_PLATFORM_IS_MACOS)

#include "slib/device/device.h"
#include "slib/core/variant.h"
#include "slib/core/platform_apple.h"

#import <sys/utsname.h>

namespace slib
{

	String Device::getDeviceId()
	{
		/* https://developer.apple.com/library/archive/technotes/tn1103/_index.html */
		NSString* ret = nil;
		io_service_t platformExpert = IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceMatching("IOPlatformExpertDevice"));
		if (platformExpert) {
			CFTypeRef serialNumberAsCFString = IORegistryEntryCreateCFProperty(platformExpert, CFSTR(kIOPlatformSerialNumberKey), kCFAllocatorDefault, 0);
			if (serialNumberAsCFString) {
				ret = CFBridgingRelease(serialNumberAsCFString);
			}
			IOObjectRelease(platformExpert);
		}
		return Apple::getStringFromNSString(ret);
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
		return String::format("macOS %s", osVersion);
	}
	
	double Device::getScreenPPI()
	{
		NSScreen *screen = [NSScreen mainScreen];
		NSDictionary *description = [screen deviceDescription];
		NSSize displayPixelSize = [[description objectForKey:NSDeviceSize] sizeValue];
		CGSize displayPhysicalSize = CGDisplayScreenSize([[description objectForKey:@"NSScreenNumber"] unsignedIntValue]);
		return displayPixelSize.width / displayPhysicalSize.width * 25.4;
	}

	Sizei Device::getScreenSize()
	{
		NSScreen* screen = [NSScreen mainScreen];
		if (screen != nil) {
			NSRect frame = screen.frame;
			return Sizei((int)(frame.size.width), (int)(frame.size.height));
		}
		return Sizei::zero();
	}
	
}

#endif
