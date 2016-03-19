#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WINDOWS)

#include <Windows.h>
#include <stdio.h>
#include <assert.h>

#if defined(SLIB_PLATFORM_IS_WP8)
using namespace Windows::Storage;
using namespace Platform;
#endif

#include "../../../inc/slib/core/system.h"
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
	if (hMutex != NULL)
	{
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
sl_bool System::createProcess(const String& _pathExecutable, const String* commandLines, sl_uint32 nCommandLines)
{
	String16 pathExecutable = _pathExecutable;
	String16 cmd;
	{
		StringBuffer16 sb;
		sb.add(SLIB_UNICODE("\""));
		sb.add(pathExecutable);
		sb.add(SLIB_UNICODE("\" "));
		for (sl_size i = 0; i < nCommandLines; i++) {
			sb.add(SLIB_UNICODE("\""));
			sb.add(commandLines[i]);
			sb.add(SLIB_UNICODE("\" "));
		}
		cmd = sb.merge();
	}
	STARTUPINFOW si;
	ZeroMemory(&si, sizeof(si));
	si.dwFlags |= STARTF_USESTDHANDLES;
	si.cb = sizeof(si);

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	BOOL bRet = CreateProcessW((LPCWSTR)(pathExecutable.getData())
		, (LPWSTR)(cmd.getData())
		, NULL, NULL, FALSE
		, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);
	if (bRet) {
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		return sl_true;
	} else {
		return sl_false;
	}
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
