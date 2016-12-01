#ifndef CHECKHEADER_SLIB_DEVICE_INFORMATION
#define CHECKHEADER_SLIB_DEVICE_INFORMATION

#include "definition.h"

#include "../core/string.h"

SLIB_DEVICE_NAMESPACE_BEGIN

class SLIB_EXPORT DeviceInformation
{
public:
	static String getDeviceId();
	
	static String getDeviceName();
	
	static String getSystemVersion();
	
	static String getSystemName();
	
};

SLIB_DEVICE_NAMESPACE_END

#endif
