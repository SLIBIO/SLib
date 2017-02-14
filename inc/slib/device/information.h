#ifndef CHECKHEADER_SLIB_DEVICE_INFORMATION
#define CHECKHEADER_SLIB_DEVICE_INFORMATION

#include "definition.h"

#include "../core/string.h"
#include "../math/size.h"

namespace slib
{

	class SLIB_EXPORT DeviceInformation
	{
	public:
		static String getDeviceId();

		static String getDeviceName();

		static String getSystemVersion();

		static String getSystemName();
	
		static sl_uint32 getDevicePPI();

		static Size getScreenSize();

		static sl_uint32 getScreenWidth();

		static sl_uint32 getScreenHeight();

	};

}

#endif
