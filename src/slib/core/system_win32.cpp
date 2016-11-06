#include "../../../inc/slib/core/definition.h"

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

#include "../../../inc/slib/core/system.h"
#include "../../../inc/slib/core/app.h"
#include "../../../inc/slib/core/file.h"
#include "../../../inc/slib/core/list.h"
#include "../../../inc/slib/core/log.h"

SLIB_NAMESPACE_BEGIN

String System::getApplicationPath()
{
#if defined(SLIB_PLATFORM_IS_WIN32)
	sl_char16 bufAppPath[1024];
	::GetModuleFileNameW(GetModuleHandle(NULL), (WCHAR*)bufAppPath, 1024);
	return String::fromUtf16(bufAppPath);
#endif
#if defined(SLIB_PLATFORM_IS_WP8)
	String ret;
	ret = String::fromUtf16(ApplicationData::Current->LocalFolder->Path->Data());
	return ret;
#endif
}

String System::getTempDirectory()
{
#if defined(SLIB_PLATFORM_IS_WIN32)
	sl_char16 sz[1024];
	sl_int32 n = ::GetTempPathW(1024, (LPWSTR)sz);
	String ret(sz, n);
	return ret;
#else
	SLIB_STATIC_STRING(temp, "/temp");
	String dir = System::getApplicationDirectory() + temp;
	File::createDirectory(dir);
	return dir;
#endif
}

sl_uint32 System::getTickCount()
{
#if defined(SLIB_PLATFORM_IS_WIN32)
	return ::GetTickCount();
#else
	return (sl_uint32)(::GetTickCount64());
#endif
}

#if defined(SLIB_PLATFORM_IS_WIN32)
void* System::createGlobalUniqueInstance(const String& _name)
{
	String16 name = _name;
	if (name.isEmpty()) {
		return sl_null;
	}
	SLIB_STATIC_STRING16(g, "Global\\");
	name = g + File::makeSafeFileName(name);

	HANDLE hMutex = ::OpenMutexW(MUTEX_ALL_ACCESS, FALSE, (LPCWSTR)(name.getData()));
	if (hMutex != NULL) {
		::CloseHandle(hMutex);
		return sl_null;
	}
	hMutex = ::CreateMutexW(NULL, FALSE, (LPCWSTR)(name.getData()));
	if (hMutex) {
		return (void*)(hMutex);
	} else {
		return sl_null;
	}
}

void System::freeGlobalUniqueInstance(void* instance)
{
	if (instance) {
		::CloseHandle((HANDLE)instance);
	}
}
#endif

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
SIGNAL_HANDLER __signal_crash_handler;

void _signal_crash_handler(int sig)
{
	if (sig == SIGFPE) {
		_fpreset();
	}
	__signal_crash_handler(sig);
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
		__signal_crash_handler(-1);
		break;
	}
	return EXCEPTION_EXECUTE_HANDLER;
}

void System::setCrashHandler(SIGNAL_HANDLER handler)
{
	__signal_crash_handler = handler;
	SetUnhandledExceptionFilter(_seh_crash_handler);
	handler = _signal_crash_handler;
	signal(SIGFPE, handler);
	signal(SIGSEGV, handler);
	signal(SIGILL, handler);
	signal(SIGABRT, handler);
	signal(SIGABRT_COMPAT, handler);
}
#endif

void Console::print(const String& _s)
{
	String16 s = _s;
	wprintf(L"%s", s.getData());
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

SLIB_NAMESPACE_END

void sl_abort(const char* msg, const char* file, sl_uint32 line)
{
#if defined(SLIB_DEBUG)
	slib::System::abort(msg, file, line);
#endif
}

#endif
