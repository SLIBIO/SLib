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

#if defined(SLIB_PLATFORM_IS_WINDOWS)

#include <Windows.h>
#include <stdio.h>
#include <assert.h>
#include <process.h>
#include <signal.h>
#include <float.h>
#include <conio.h>

#if defined(SLIB_PLATFORM_IS_UWP)
using namespace Windows::Storage;
using namespace Platform;
#endif

#include "slib/core/system.h"

#include "slib/core/app.h"
#include "slib/core/file.h"
#include "slib/core/console.h"
#include "slib/core/list.h"
#include "slib/core/string_buffer.h"
#include "slib/core/log.h"
#include "slib/core/platform_windows.h"


#define PRIV_PATH_MAX 1024

namespace slib
{

	String System::getApplicationPath()
	{
#if defined(SLIB_PLATFORM_IS_WIN32)
		sl_char16 bufAppPath[PRIV_PATH_MAX] = {0};
		::GetModuleFileNameW(GetModuleHandle(NULL), (WCHAR*)bufAppPath, PRIV_PATH_MAX - 1);
		return String(bufAppPath);
#endif
#if defined(SLIB_PLATFORM_IS_UWP)
		return String::fromUtf16(ApplicationData::Current->LocalFolder->Path->Data());
#endif
	}

	String System::getTempDirectory()
	{
#if defined(SLIB_PLATFORM_IS_WIN32)
		sl_char16 sz[PRIV_PATH_MAX] = {0};
		sl_int32 n = ::GetTempPathW(PRIV_PATH_MAX - 1, (LPWSTR)sz);
		return String(sz, n);
#else
		SLIB_STATIC_STRING(temp, "/temp");
		String dir = System::getApplicationDirectory() + temp;
		File::createDirectory(dir);
		return dir;
#endif
	}

	String System::getCurrentDirectory()
	{
		WCHAR path[PRIV_PATH_MAX] = { 0 };
		::GetCurrentDirectoryW(PRIV_PATH_MAX - 1, path);
		return String(path);
	}

	sl_bool System::setCurrentDirectory(const String& _dir)
	{
		String16 dir = _dir;
		if (::SetCurrentDirectoryW((LPCWSTR)(dir.getData()))) {
			return sl_true;
		}
		return sl_false;
	}

	sl_uint32 System::getTickCount()
	{
#if defined(SLIB_PLATFORM_IS_WIN32)
		return GetTickCount();
#else
		return (sl_uint32)(GetTickCount64());
#endif
	}

	sl_uint64 System::getTickCount64()
	{
#if defined(SLIB_PLATFORM_IS_WIN32)
		WINAPI_GetTickCount64 func = Windows::getAPI_GetTickCount64();
		if (func) {
			return (sl_uint64)(func());
		}		
		static sl_uint32 tickOld = 0;
		static sl_uint32 tickHigh = 0;
		SLIB_STATIC_SPINLOCKER(lock)
		sl_uint32 tick = GetTickCount();
		if (tick < tickOld) {
			tickHigh++;
		}
		tickOld = tick;
		return SLIB_MAKE_QWORD4(tickHigh, tick);
#else
		return (sl_uint64)(GetTickCount64());
#endif
	}

	sl_uint32 System::getProcessId()
	{
		return ::GetCurrentProcessId();
	}

	sl_uint32 System::getThreadId()
	{
		return ::GetCurrentThreadId();
	}

#if defined (SLIB_PLATFORM_IS_WIN32)
	sl_bool System::createProcess(const String& _pathExecutable, const String* cmds, sl_uint32 nCmds)
	{
		String16 pathExecutable = _pathExecutable;
		String16 cmd;
		{
			StringBuffer16 sb;
			sb.add(SLIB_UNICODE("\""));
			sb.add(pathExecutable);
			sb.add(SLIB_UNICODE("\""));
			String args = Application::buildCommandLine(cmds, nCmds);
			if (args.isNotEmpty()) {
				sb.add(SLIB_UNICODE(" "));
				sb.add(args);
			}
			cmd = sb.merge();
		}
		STARTUPINFOW si;
		ZeroMemory(&si, sizeof(si));
		si.dwFlags |= STARTF_USESTDHANDLES;
		si.cb = sizeof(si);

		PROCESS_INFORMATION pi;
		ZeroMemory(&pi, sizeof(pi));
		BOOL bRet = ::CreateProcessW((LPCWSTR)(pathExecutable.getData())
			, (LPWSTR)(cmd.getData())
			, NULL, NULL, FALSE
			, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
		if (bRet) {
			::CloseHandle(pi.hProcess);
			::CloseHandle(pi.hThread);
			return sl_true;
		} else {
			return sl_false;
		}
	}

	void System::exec(const String& pathExecutable, const String* cmds, sl_uint32 nCmds)
	{
		char* exe = pathExecutable.getData();
		char* args[1024];
		args[0] = exe;
		if (nCmds > 1020) {
			nCmds = 1020;
		}
		for (sl_size i = 0; i < nCmds; i++) {
			args[i + 1] = cmds[i].getData();
		}
		args[nCmds + 1] = 0;

		::_execvp(exe, args);
		::exit(1);
	}

	void System::exit(int code)
	{
		::exit(code);
	}
#endif

	void System::sleep(sl_uint32 milliseconds)
	{
		::Sleep(milliseconds);
	}

	void System::yield()
	{
#if defined(YieldProcessor)
		YieldProcessor();
#else
		System::sleep(0);
#endif
	}

	sl_uint32 System::getLastError()
	{
		return (sl_uint32)(::GetLastError());
	}

	String System::formatErrorCode(sl_uint32 errorCode)
	{
		String ret;
		if (errorCode > 0) {
			LPWSTR buf = sl_null;
			DWORD size = ::FormatMessageW(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_HMODULE,
				Windows::loadLibrary_wininet(),
				errorCode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				(LPWSTR)&buf, 0,
				NULL);
			if (buf) {
				ret = String((sl_char16*)buf, size);
				::LocalFree(buf);
			}
		}
		if (ret.isEmpty()) {
			return String::format("Unknown error: %d", errorCode);
		}
		return ret;
	}

	void System::abort(const String& _msg, const String& _file, sl_uint32 line)
	{
#if defined(SLIB_DEBUG)
		String16 msg = _msg;
		String16 file = _file;
		::_wassert((wchar_t*)(msg.getData()), (wchar_t*)(file.getData()), line);
#endif
	}

#if defined(SLIB_PLATFORM_IS_WIN32)

	namespace priv
	{
		namespace system
		{
			volatile double g_signal_fpe_dummy = 0.0f;
			SIGNAL_HANDLER g_handlerSignalCrash;

			static void handleSignalCrash(int sig)
			{
				if (sig == SIGFPE) {
					_fpreset();
				}
				g_handlerSignalCrash(sig);
			}

			static LONG WINAPI handleException(PEXCEPTION_POINTERS pExceptionPtrs)
			{
				DWORD code = pExceptionPtrs->ExceptionRecord->ExceptionCode;
				switch (code) {
				case EXCEPTION_ACCESS_VIOLATION:
				case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
				case EXCEPTION_DATATYPE_MISALIGNMENT:
				case EXCEPTION_FLT_DENORMAL_OPERAND:
				case EXCEPTION_FLT_DIVIDE_BY_ZERO:
				case EXCEPTION_FLT_INEXACT_RESULT:
				case EXCEPTION_FLT_INVALID_OPERATION:
				case EXCEPTION_FLT_OVERFLOW:
				case EXCEPTION_FLT_STACK_CHECK:
				case EXCEPTION_FLT_UNDERFLOW:
				case EXCEPTION_ILLEGAL_INSTRUCTION:
				case EXCEPTION_IN_PAGE_ERROR:
				case EXCEPTION_INT_DIVIDE_BY_ZERO:
				case EXCEPTION_INT_OVERFLOW:
				case EXCEPTION_INVALID_DISPOSITION:
				case EXCEPTION_NONCONTINUABLE_EXCEPTION:
				case EXCEPTION_PRIV_INSTRUCTION:
				case EXCEPTION_STACK_OVERFLOW:
					g_handlerSignalCrash(-1);
					break;
				}
				return EXCEPTION_EXECUTE_HANDLER;
			}

		}
	}
	
	void System::setCrashHandler(SIGNAL_HANDLER handler)
	{
		priv::system::g_handlerSignalCrash = handler;
		::SetUnhandledExceptionFilter(priv::system::handleException);
		handler = priv::system::handleSignalCrash;
		signal(SIGFPE, handler);
		signal(SIGSEGV, handler);
		signal(SIGILL, handler);
		signal(SIGABRT, handler);
		signal(SIGABRT_COMPAT, handler);
	}
#endif

	void System::setDebugFlags()
	{
		Windows::setDebugFlags();
	}

#if defined(SLIB_PLATFORM_IS_WIN32)

	namespace priv
	{
		namespace system
		{
			class GlobalUniqueInstanceImpl : public GlobalUniqueInstance
			{
			public:
				HANDLE m_hMutex;

			public:
				GlobalUniqueInstanceImpl()
				{
				}

				~GlobalUniqueInstanceImpl()
				{
					::CloseHandle(m_hMutex);
				}
			};
		}
	}


	Ref<GlobalUniqueInstance> GlobalUniqueInstance::create(const String& _name)
	{
		if (_name.isEmpty()) {
			return sl_null;
		}
		String16 name = "Global\\" + File::makeSafeFileName(_name);
		HANDLE hMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, (LPCWSTR)(name.getData()));
		if (hMutex != NULL) {
			::CloseHandle(hMutex);
			return sl_null;
		}
		Ref<priv::system::GlobalUniqueInstanceImpl> instance = new priv::system::GlobalUniqueInstanceImpl;
		if (instance.isNotNull()) {
			hMutex = ::CreateMutexW(NULL, FALSE, (LPCWSTR)(name.getData()));
			if (hMutex) {
				instance->m_hMutex = hMutex;
				return instance;
			}
		}
		return sl_null;
	}

	sl_bool GlobalUniqueInstance::exists(const String& _name)
	{
		if (_name.isEmpty()) {
			return sl_false;
		}
		String16 name = "Global\\" + File::makeSafeFileName(_name);
		HANDLE hMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, (LPCWSTR)(name.getData()));
		if (hMutex != NULL) {
			::CloseHandle(hMutex);
			return sl_true;
		}
		return sl_false;
	}

#endif

	namespace priv
	{
		void Abort(const char* msg, const char* file, sl_uint32 line) noexcept
		{
#if defined(SLIB_DEBUG)
			slib::System::abort(msg, file, line);
#endif
		}
	}

	void Console::print(const String& _s)
	{
		String16 s = _s;
		::wprintf(L"%s", (LPCWSTR)(s.getData()));
		::OutputDebugStringW((LPCWSTR)s.getData());
	}

#if defined(SLIB_PLATFORM_IS_WIN32)
	String Console::readLine()
	{
		char line[512];
		char* l = ::gets_s(line);
		line[511] = 0;
		return l;
	}

	sl_char16 Console::readChar(sl_bool flagPrintEcho)
	{
		if (flagPrintEcho) {
			return (sl_char16)(::_getche());
		} else {
			return (sl_char16)(::_getch());
		}
	}
#endif

}

#endif
