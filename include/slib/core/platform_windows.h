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

#ifndef CHECKHEADER_SLIB_CORE_PLATFORM_WINDOWS
#define CHECKHEADER_SLIB_CORE_PLATFORM_WINDOWS

#include "definition.h"

#if defined(SLIB_PLATFORM_IS_WINDOWS)

#include <windows.h>

#include "string.h"
#include "event.h"
#include "object.h"

namespace slib
{
	
	typedef BOOL (WINAPI *WINAPI_GetQueuedCompletionStatusEx)
	(
		HANDLE CompletionPort,
		LPOVERLAPPED_ENTRY lpCompletionPortEntries,
		ULONG ulCount,
		PULONG ulNumEntriesRemoved,
		DWORD dwMilliseconds,
		BOOL fAlertable
	);
	
	typedef int (WINAPI *WINAPI_GetUserDefaultLocaleName)(
		LPWSTR lpLocaleName,
		int cchLocaleName
	);

	class Variant;

	class SLIB_EXPORT Windows
	{
	public:
		static String getStringFromGUID(const GUID& guid);

		static sl_bool getGUIDFromString(const String& str, GUID* pguid);


		static sl_bool isWindowVisible(HWND hWnd);

		static HWND getOwnerWindow(HWND hWnd);

		static HWND getParentWindow(HWND hWnd);

		static HWND getRootWindow(HWND hWnd);
	
		static void getWindowFrame(HWND hWnd, RECT& rc);
	

		static String getWindowText(HWND hWnd);

		static void setWindowText(HWND hWnd, const String& text);
	

		static sl_bool processWindowHorizontalScrollEvents(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, sl_uint32 nLine, sl_uint32 nWheel);

		static sl_bool processWindowVerticalScrollEvents(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, sl_uint32 nLine, sl_uint32 nWheel);
	
		static void setWindowHorizontalScrollParam(HWND hWnd, sl_int32 nMin, sl_int32 nMax, sl_int32 nPage);

		static void setWindowVerticalScrollParam(HWND hWnd, sl_int32 nMin, sl_int32 nMax, sl_int32 nPage);

	
		static void setDebugFlags();
	
		static HMODULE loadLibrary(const String& path);

		static HMODULE loadLibrary_kernel32();

		static WINAPI_GetQueuedCompletionStatusEx getAPI_GetQueuedCompletionStatusEx();

		static WINAPI_GetUserDefaultLocaleName getAPI_GetUserDefaultLocaleName();

		static HMODULE loadLibrary_wininet();

	
		static Ref<Event> createEvent(HANDLE hEvent, sl_bool flagCloseOnRelease = sl_true);
	
	
		static sl_bool getRegistryValue(HKEY hKey, const String16& subPath, const String16& name, Variant* out);
	
		static sl_bool setRegistryValue(HKEY hKey, const String16& subPath, const String16& name, const Variant& value);
	

		static sl_bool isCurrentProcessInAdminGroup();

		static sl_bool isCurrentProcessRunAsAdmin();


		class ShellExecuteParam
		{
		public:
			String16 operation;
			String16 path;
			String16 params;
			sl_bool runAsAdmin; // `shellExecute` returns sl_false if the user refused the elevation
			String16 currentDirectory;
			HWND hWndParent;
			int nShow;
		public:
			ShellExecuteParam();
		};

		static sl_bool shellExecute(const ShellExecuteParam& param);

	};

}

#endif

#endif
