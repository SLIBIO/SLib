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

#include <windows.h>

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
		OSVERSIONINFOEXW vi;
		Base::zeroMemory(&vi, sizeof(vi));
		vi.dwOSVersionInfoSize = sizeof(vi);
		if (::GetVersionExW((OSVERSIONINFOW*)&vi)) {
			return String::format("%d.%d", vi.dwMajorVersion, vi.dwMinorVersion);
		}
		return sl_null;
	}
	
	/*
		Applications not manifested for Windows 8.1 or Windows 10 will return the Windows 8 OS version value (6.2).
		For more information, visit at https://docs.microsoft.com/en-us/windows/desktop/SysInfo/targeting-your-application-at-windows-8-1
	*/
	String Device::getSystemName()
	{
		OSVERSIONINFOEXW vi;
		Base::zeroMemory(&vi, sizeof(vi));
		vi.dwOSVersionInfoSize = sizeof(vi);
		if (::GetVersionExW((OSVERSIONINFOW*)&vi)) {
			switch (vi.dwMajorVersion) {
			case 10:
				if (vi.dwMinorVersion == 0) {
					if (vi.wProductType != VER_NT_WORKSTATION) {
						return "Windows Server 2016";
					}
				}
				break;
			case 6:
				switch (vi.dwMinorVersion) {
				case 3:
					if (vi.wProductType == VER_NT_WORKSTATION) {
						return "Windows 8.1";
					} else {
						return "Windows Server 2012 R2";
					}
				case 2:
					if (vi.wProductType == VER_NT_WORKSTATION) {
						return "Windows 8";
					} else {
						return "Windows Server 2012";
					}
				case 1:
					if (vi.wProductType == VER_NT_WORKSTATION) {
						return "Windows 7";
					} else {
						return "Windows Server 2008 R2";
					}
				case 0:
					if (vi.wProductType == VER_NT_WORKSTATION) {
						return "Windows Vista";
					} else {
						return "Windows Server 2008";
					}
				}
				break;
			case 5:
				switch (vi.dwMinorVersion) {
				case 2:
					if (vi.wProductType == VER_NT_WORKSTATION) {
						return "Windows XP Professional x64 Edition";
					} else {
						if (GetSystemMetrics(SM_SERVERR2) != 0) {
							return "Windows Server 2003 R2";
						} else if (vi.wSuiteMask & VER_SUITE_WH_SERVER) {
							return "Windows Home Server";
						} else {
							return "Windows Server 2003";
						}
					}
				case 1:
					return "Windows XP";
				case 0:
					return "Windows 2000";
				}
				break;
			}
		}
		return String::format("Windows %s", getSystemVersion());
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
