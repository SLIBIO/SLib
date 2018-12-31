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

#if defined(SLIB_UI_IS_GTK)

#include "slib/device/device.h"

#include "slib/core/variant.h"

#include "gtk/gtk.h"

#include <sys/utsname.h>

namespace slib
{

	String Device::getDeviceName()
	{
		utsname systemInfo;
		uname(&systemInfo);
		return systemInfo.machine;
	}
	
	String Device::getSystemVersion()
	{
		utsname systemInfo;
		uname(&systemInfo);
		return systemInfo.release;
	}
	
	String Device::getSystemName()
	{
		utsname systemInfo;
		uname(&systemInfo);
		return String::format("%s %s", systemInfo.sysname, systemInfo.release);
	}
	
	double Device::getScreenPPI()
	{
		gtk_init_check(NULL, NULL);
		GdkScreen* screen = gdk_screen_get_default();
		if (screen) {
			return gdk_screen_get_resolution(screen);
		}
		return 96;
	}

	Sizei Device::getScreenSize()
	{
		gtk_init_check(NULL, NULL);
		GdkScreen* screen = gdk_screen_get_default();
		if (screen) {
			Sizei ret;
			ret.x = (int)(gdk_screen_get_width(screen));
			ret.y = (int)(gdk_screen_get_height(screen));
			return ret;
		}
		return Sizei::zero();
	}
	
}

#endif
