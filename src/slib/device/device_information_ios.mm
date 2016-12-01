#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "../../../inc/slib/device/information.h"
#include "../../../inc/slib/core/platform_apple.h"
#include "../../../inc/slib/core/variant.h"

#import <UIKit/UIKit.h>
#import <sys/utsname.h>

SLIB_DEVICE_NAMESPACE_BEGIN

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

SLIB_DEVICE_NAMESPACE_END

#endif
