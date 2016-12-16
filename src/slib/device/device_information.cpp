#include "../../../inc/slib/device/information.h"

SLIB_DEVICE_NAMESPACE_BEGIN

sl_uint32 DeviceInformation::getScreenWidth()
{
	return (sl_uint32)(getScreenSize().x);
}

sl_uint32 DeviceInformation::getScreenHeight()
{
	return (sl_uint32)(getScreenSize().y);
}

SLIB_DEVICE_NAMESPACE_END
