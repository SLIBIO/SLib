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

#if defined(SLIB_PLATFORM_IS_UNIX)

#include "slib/core/system.h"

#include "slib/core/file.h"
#include "slib/core/variant.h"

#include <string.h>
#include <time.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <sched.h>
#include <signal.h>
#include <pwd.h>

#include <sys/time.h>

#define PRIV_PATH_MAX 1024

namespace slib
{

#if !defined(SLIB_PLATFORM_IS_APPLE) && !defined(SLIB_PLATFORM_IS_ANDROID)
	String System::getApplicationPath()
	{
		char path[PRIV_PATH_MAX] = {0};
		int n = readlink("/proc/self/exe", path, PRIV_PATH_MAX-1);
		/*
		-- another solution --

			char a[50];
			sprintf(a, "/proc/%d/exe", getpid());
			int n = readlink(a, path, size);
		*/
		String ret;
		if (n > 0) {
			ret = String::fromUtf8(path, n);
		}
		return ret;
	}
	
	String System::getHomeDirectory()
	{
		passwd* pwd = getpwuid(getuid());
		return pwd->pw_dir;
	}

	String System::getTempDirectory()
	{
		return "/tmp";
	}
#endif

	String System::getCurrentDirectory()
	{
		char path[PRIV_PATH_MAX] = {0};
		char* r = getcwd(path, PRIV_PATH_MAX-1);
		if (r) {
			return path;
		}
		return sl_null;
	}

	sl_bool System::setCurrentDirectory(const String& dir)
	{
		int iRet = chdir(dir.getData());
		if (iRet == 0) {
			return sl_true;
		}
		return sl_false;
	}
	
#if !defined(SLIB_PLATFORM_IS_APPLE) && !defined(SLIB_PLATFORM_IS_ANDROID)
	String System::getComputerName()
	{
		char buf[512] = {0};
		gethostname(buf, 500);
		return buf;
	}
	
	String System::getUserName()
	{
		return getlogin();
	}
	
	String System::getFullUserName()
	{
		return getUserName();
	}
#endif

	sl_uint32 System::getTickCount()
	{
		return (sl_uint32)(getTickCount64());
	}
	
#if !defined(SLIB_PLATFORM_IS_APPLE)
	sl_uint64 System::getTickCount64()
	{
#if defined(_POSIX_TIMERS) && _POSIX_TIMERS > 0 && defined(CLOCK_MONOTONIC)
		{
			static sl_bool flagCheck = sl_true;
			static sl_bool flagEnabled = sl_false;
			struct timespec ts;
			if (flagEnabled) {
				clock_gettime(CLOCK_MONOTONIC, &ts);
				return (sl_uint64)(ts.tv_sec) * 1000 + (sl_uint64)(ts.tv_nsec) / 1000000;
			} else {
				if (flagCheck) {
					int iRet = clock_gettime(CLOCK_MONOTONIC, &ts);
					if (iRet < 0) {
						flagCheck = sl_false;
					} else {
						flagEnabled = sl_true;
						flagCheck = sl_false;
						return (sl_uint64)(ts.tv_sec) * 1000 + (sl_uint64)(ts.tv_nsec) / 1000000;
					}
				}
			}
		}
#endif
		struct timeval tv;
		if (gettimeofday(&tv, 0) == 0) {
			return (sl_uint64)(tv.tv_sec) * 1000 + tv.tv_usec / 1000;
		} else {
			return 0;
		}
	}
#endif

	void System::sleep(sl_uint32 milliseconds)
	{
		struct timespec req;
		req.tv_sec = milliseconds / 1000;
		req.tv_nsec = (milliseconds % 1000) * 1000000;
		nanosleep(&req, sl_null);
	}

	void System::yield()
	{
		sched_yield();
	}
	
	sl_uint32 System::getLastError()
	{
		return errno;
	}
	
	String System::formatErrorCode(sl_uint32 errorCode)
	{
		String ret = ::strerror(errorCode);
		if (ret.isEmpty()) {
			return String::format("Unknown error: %d", errorCode);
		}
		return ret;
	}

#if !defined(SLIB_PLATFORM_IS_ANDROID)
	void System::abort(const String& msg, const String& file, sl_uint32 line)
	{
#if defined(SLIB_DEBUG)
		__assert(msg.getData(), file.getData(), line);
#endif
	}
#endif

#if !defined(SLIB_PLATFORM_IS_MOBILE)
	namespace priv
	{
		namespace system
		{
			volatile double g_signal_fpe_dummy = 0.0f;
		}
	}

	void System::setCrashHandler(SIGNAL_HANDLER handler)
	{
		struct sigaction sa;
		sa.sa_flags = SA_NODEFER;
		sa.sa_handler = handler;
		sigemptyset(&(sa.sa_mask));
		sigaction(SIGFPE, &sa, NULL);
		sigaction(SIGSEGV, &sa, NULL);
		sigaction(SIGBUS, &sa, NULL);
		sigaction(SIGILL, &sa, NULL);
		sigaction(SIGABRT, &sa, NULL);
		sigaction(SIGIOT, &sa, NULL);
#	if defined(SLIB_PLATFORM_IS_MACOS)
		sigaction(SIGEMT, &sa, NULL);
#	endif
		sigaction(SIGSYS, &sa, NULL);
	}
#endif

#if !defined(SLIB_PLATFORM_IS_ANDROID)
	namespace priv
	{
		void Abort(const char* msg, const char* file, sl_uint32 line) noexcept
		{
#if defined(SLIB_DEBUG)
			__assert(msg, file, line);
#endif
		}
	}
#endif

}

#endif
