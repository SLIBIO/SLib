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

#include "slib/core/system.h"

#include "slib/core/file.h"

namespace slib
{

	String System::getApplicationDirectory()
	{
		String path = getApplicationPath();
#if !defined(SLIB_PLATFORM_IS_ANDROID)
		path = File::getParentDirectoryPath(path);
#endif
		return path;
	}

#if !defined(SLIB_PLATFORM_IS_WINDOWS)
	String System::getLocalAppDataDirectory()
	{
		return getHomeDirectory();
	}
#endif

#if !defined(SLIB_PLATFORM_IS_APPLE)
	String System::getCachesDirectory()
	{
		String dir = getHomeDirectory() + "/.caches";
		File::createDirectory(dir);
		return dir;
	}
#endif

#if defined(SLIB_PLATFORM_IS_MOBILE)
	void System::setCrashHandler(SIGNAL_HANDLER handler)
	{
	}
#endif
	
#if !defined(SLIB_PLATFORM_IS_WIN32)
	void System::setDebugFlags()
	{
	}
#endif

	void System::yield(sl_uint32 elapsed)
	{
		if (elapsed < 16) {
			return;
		}
		if (elapsed < 32) {
			System::yield();
		} else {
			System::sleep(1);
		}
	}

#if defined(SLIB_PLATFORM_IS_MOBILE)

	void System::registerApplicationRunAtStartup(const String& path)
	{
	}
	
	void System::registerApplicationRunAtStartup()
	{
	}
	
	void System::unregisterApplicationRunAtStartup(const String& path)
	{
	}
	
	void System::unregisterApplicationRunAtStartup()
	{
	}
	
#endif
	
}
