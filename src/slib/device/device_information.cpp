#include "../../../inc/slib/device/information.h"

namespace slib
{

	sl_uint32 DeviceInformation::getScreenWidth()
	{
		return (sl_uint32)(getScreenSize().x);
	}

	sl_uint32 DeviceInformation::getScreenHeight()
	{
		return (sl_uint32)(getScreenSize().y);
	}

}
