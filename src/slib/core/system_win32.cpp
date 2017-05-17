/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WINDOWS)

#include <Windows.h>
#include <stdio.h>
#include <assert.h>
#include <process.h>
#include <signal.h>
#include <float.h>

#if defined(SLIB_PLATFORM_IS_WP8)
using namespace Windows::Storage;
using namespace Platform;
#endif

#include "slib/core/system.h"

#include "slib/core/app.h"
#include "slib/core/file.h"
#include "slib/core/list.h"
#include "slib/core/string_buffer.h"
#include "slib/core/log.h"

#define _PATH_MAX 1024

namespace slib
{

	String System::getApplicationPath()
	{
#if defined(SLIB_PLATFORM_IS_WIN32)
		sl_char16 bufAppPath[_PATH_MAX] = {0};
		::GetModuleFileNameW(GetModuleHandle(NULL), (WCHAR*)bufAppPath, _PATH_MAX - 1);
		return String(bufAppPath);
#endif
#if defined(SLIB_PLATFORM_IS_WP8)
		return String::fromUtf16(ApplicationData::Current->LocalFolder->Path->Data());
#endif
	}

	String System::getTempDirectory()
	{
#if defined(SLIB_PLATFORM_IS_WIN32)
		sl_char16 sz[_PATH_MAX] = {0};
		sl_int32 n = ::GetTempPathW(_PATH_MAX - 1, (LPWSTR)sz);
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
		WCHAR path[_PATH_MAX] = { 0 };
		::GetCurrentDirectoryW(_PATH_MAX - 1, path);
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
		return ::GetTickCount();
#else
		return (sl_uint32)(::GetTickCount64());
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

	void System::abort(const String& _msg, const String& _file, sl_uint32 line)
	{
#if defined(SLIB_DEBUG)
		String16 msg = _msg;
		String16 file = _file;
		_wassert((wchar_t*)(msg.getData()), (wchar_t*)(file.getData()), line);
#endif
	}

#if defined(SLIB_PLATFORM_IS_WIN32)
	volatile double __signal_fpe_dummy = 0.0f;
	SIGNAL_HANDLER _g_signal_crash_handler;

	void _signal_crash_handler(int sig)
	{
		if (sig == SIGFPE) {
			_fpreset();
		}
		_g_signal_crash_handler(sig);
	}

	LONG WINAPI _seh_crash_handler(PEXCEPTION_POINTERS pExceptionPtrs)
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
			_g_signal_crash_handler(-1);
			break;
		}
		return EXCEPTION_EXECUTE_HANDLER;
	}

	void System::setCrashHandler(SIGNAL_HANDLER handler)
	{
		_g_signal_crash_handler = handler;
		SetUnhandledExceptionFilter(_seh_crash_handler);
		handler = _signal_crash_handler;
		signal(SIGFPE, handler);
		signal(SIGSEGV, handler);
		signal(SIGILL, handler);
		signal(SIGABRT, handler);
		signal(SIGABRT_COMPAT, handler);
	}
#endif

#if defined(SLIB_PLATFORM_IS_WIN32)

	class _GlobalUniqueInstance : public GlobalUniqueInstance
	{
	public:
		HANDLE m_hMutex;

	public:
		_GlobalUniqueInstance()
		{
		}

		~_GlobalUniqueInstance()
		{
			::CloseHandle(m_hMutex);
		}
	};


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
		Ref<_GlobalUniqueInstance> instance = new _GlobalUniqueInstance;
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

	void _abort(const char* msg, const char* file, sl_uint32 line) noexcept
	{
#if defined(SLIB_DEBUG)
		slib::System::abort(msg, file, line);
#endif
	}

	void Console::print(const String& _s)
	{
		String16 s = _s;
		wprintf(L"%s", (LPCWSTR)(s.getData()));
		OutputDebugStringW((LPCWSTR)s.getData());
	}

#if defined(SLIB_PLATFORM_IS_WIN32)
	String Console::readLine()
	{
		char line[512];
		char* l = gets_s(line);
		line[511] = 0;
		return l;
	}
#endif

}

#endif
