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

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "slib/device/device.h"

#include "slib/core/variant.h"
#include "slib/core/platform_windows.h"

namespace slib
{

	String Device::getDeviceName()
	{
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		switch (si.wProcessorArchitecture) {
		case PROCESSOR_ARCHITECTURE_INTEL:
			return "x86";
		case PROCESSOR_ARCHITECTURE_AMD64:
			return "x64";
		case PROCESSOR_ARCHITECTURE_IA64:
			return "ia64";
		case PROCESSOR_ARCHITECTURE_ARM:
			return "arm";
		case 12: // PROCESSOR_ARCHITECTURE_ARM64
			return "arm64";
		}
		return "unknown";
	}

	String Device::getSystemVersion()
	{
		WindowsVersion version = Windows::getVersion();
		return String::format("%d.%d", SLIB_WINDOWS_MAJOR_VERSION(version), SLIB_WINDOWS_MINOR_VERSION(version));
	}
	
	/*
		Applications not manifested for Windows 8.1 or Windows 10 will return the Windows 8 OS version value (6.2).
		For more information, visit at https://docs.microsoft.com/en-us/windows/desktop/SysInfo/targeting-your-application-at-windows-8-1
	*/
	String Device::getSystemName()
	{
		WindowsVersion version = Windows::getVersion();
		switch (version) {
		case WindowsVersion::Server2016:
			return "Windows Server 2016";
		case WindowsVersion::Server2012_R2:
			return "Windows Server 2012 R2";
		case WindowsVersion::Server2012:
			return "Windows Server 2012";
		case WindowsVersion::Server2008_R2:
			return "Windows Server 2008 R2";
		case WindowsVersion::Server2008:
			return "Windows Server 2008";
		case WindowsVersion::Server2003:
			return "Windows Server 2003";
		case WindowsVersion::Windows10:
			return "Windows 10";
		case WindowsVersion::Windows8_1:
			return "Windows 8.1";
		case WindowsVersion::Windows8:
			return "Windows 8";
		case WindowsVersion::Windows7_SP1:
			return "Windows 7 SP1";
		case WindowsVersion::Windows7:
			return "Windows 7";
		case WindowsVersion::Vista_SP2:
			return "Windows Vista SP2";
		case WindowsVersion::Vista_SP1:
			return "Windows Vista SP1";
		case WindowsVersion::Vista:
			return "Windows Vista";
		case WindowsVersion::XP_64:
			return "Windows XP 64bit";
		case WindowsVersion::XP_SP3:
			return "Windows XP SP3";
		case WindowsVersion::XP_SP2:
			return "Windows XP SP2";
		case WindowsVersion::XP_SP1:
			return "Windows XP SP1";
		default:
			return "Windows XP";
		}
	}
	
	double Device::getScreenPPI()
	{
		return 96;
	}

	Sizei Device::getScreenSize()
	{
		Sizei ret;
		ret.x = (int)(GetSystemMetrics(SM_CXSCREEN));
		ret.y = (int)(GetSystemMetrics(SM_CYSCREEN));
		return ret;
	}
	
}

#endif
