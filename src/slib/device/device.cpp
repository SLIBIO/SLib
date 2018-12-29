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

#include "slib/device/device.h"

namespace slib
{

#if !defined(SLIB_PLATFORM_IS_IOS)
	void Device::setAudioCategory(const DeviceAudioCategory& category)
	{
	}
#endif

#if !defined(SLIB_UI_IS_ANDROID)
	sl_bool Device::checkPermissions(const DevicePermissions& permissions)
	{
		return sl_true;
	}
	
	void Device::grantPermissions(const DevicePermissions& permissions, const Function<void()>& callback)
	{
		callback();
	}
#endif
	
	String Device::getIMEI()
	{
		return getIMEIs().getValueAt(0);
	}

	String Device::getPhoneNumber()
	{
		return getPhoneNumbers().getValueAt(0);
	}

#if !defined(SLIB_PLATFORM_IS_ANDROID)
	List<String> Device::getIMEIs()
	{
		return sl_null;
	}

	List<String> Device::getPhoneNumbers()
	{
		return sl_null;
	}
#endif
	
#if !defined(SLIB_PLATFORM_IS_IOS) && !defined(SLIB_PLATFORM_IS_ANDROID) && !defined(SLIB_PLATFORM_IS_TIZEN)
	String Device::getDeviceId()
	{
		return sl_null;
	}
	
	String Device::getDeviceName()
	{
		return sl_null;
	}
	
	String Device::getSystemVersion()
	{
		return sl_null;
	}
	
	String Device::getSystemName()
	{
		return sl_null;
	}
	
	sl_uint32 Device::getDevicePPI()
	{
		return 72;
	}
	
	Size Device::getScreenSize()
	{
		return UI::getScreenSize();
	}
#endif
	
	sl_uint32 Device::getScreenWidth()
	{
		return (sl_uint32)(getScreenSize().x);
	}

	sl_uint32 Device::getScreenHeight()
	{
		return (sl_uint32)(getScreenSize().y);
	}

}
