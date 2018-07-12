/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
	
	typedef BOOL(WINAPI *WINAPI_GetQueuedCompletionStatusEx)
	(
		HANDLE CompletionPort,
		LPOVERLAPPED_ENTRY lpCompletionPortEntries,
		ULONG ulCount,
		PULONG ulNumEntriesRemoved,
		DWORD dwMilliseconds,
		BOOL fAlertable
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

		static HMODULE loadLibrary_wininet();

	
		static Ref<Event> createEvent(HANDLE hEvent, sl_bool flagCloseOnRelease = sl_true);
	
	
		static sl_bool getRegistryValue(HKEY hKey, const String16& subPath, const String16& name, Variant* out);
	
		static sl_bool setRegistryValue(HKEY hKey, const String16& subPath, const String16& name, const Variant& value);
	

		static sl_bool isCurrentProcessInAdminGroup();

		static sl_bool isCurrentProcessRunAsAdmin();


		class ShellExecuteParam
		{
		public:
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
