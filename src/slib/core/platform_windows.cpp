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

#include "slib/core/platform_windows.h"

#include "slib/core/scoped.h"
#include "slib/core/variant.h"
#include "slib/core/endian.h"

#include <crtdbg.h>

namespace slib
{

	String Windows::getStringFromGUID(const GUID& guid)
	{
		String ret;
		WCHAR sz[40] = { 0 };
		if (::StringFromGUID2(guid, sz, 40) < 40) {
			ret = (sl_char16*)sz;
		}
		return ret;
	}

	sl_bool Windows::getGUIDFromString(const String& _str, GUID* pguid)
	{
		String16 str = _str;
		CLSID clsid;
		HRESULT hr = ::CLSIDFromString((LPWSTR)(str.getData()), &clsid);
		if (hr == NOERROR) {
			if (pguid) {
				*pguid = clsid;
			}
			return sl_true;
		}
		return sl_false;
	}

	sl_bool Windows::isWindowVisible(HWND hWnd)
	{
		if (!::IsWindow(hWnd)) {
			return sl_false;
		}
		if (!::IsWindowVisible(hWnd)) {
			return sl_false;
		}
		if (::IsIconic(hWnd)) {
			return sl_false;
		}
		hWnd = Windows::getParentWindow(hWnd);
		if (hWnd) {
			return Windows::isWindowVisible(hWnd);
		}
		return sl_true;
	}

	HWND Windows::getOwnerWindow(HWND hWnd)
	{
		return ::GetWindow(hWnd, GW_OWNER);
	}

	HWND Windows::getParentWindow(HWND hWnd)
	{
		return ::GetAncestor(hWnd, GA_PARENT);
	}

	HWND Windows::getRootWindow(HWND hWnd)
	{
		return ::GetAncestor(hWnd, GA_ROOT);
	}

	void Windows::getWindowFrame(HWND hWnd, RECT& rc)
	{
		::GetWindowRect(hWnd, &rc);
		HWND hWndParent = Windows::getParentWindow(hWnd);
		if (hWndParent) {
			::ScreenToClient(hWnd, (POINT*)(&rc));
			::ScreenToClient(hWnd, (POINT*)(&rc) + 1);
		} else {
			rc.right -= rc.left;
			rc.bottom -= rc.top;
			rc.left = 0;
			rc.top = 0;
		}
	}

	String Windows::getWindowText(HWND hWnd)
	{
		sl_int32 len = ::GetWindowTextLengthW(hWnd);
		if (len > 0) {
			SLIB_SCOPED_BUFFER(WCHAR, 1024, buf, len + 2);
			if (buf) {
				len = ::GetWindowTextW(hWnd, buf, len + 1);
				return String(buf, len);
			}
		}
		return sl_null;
	}

	void Windows::setWindowText(HWND hWnd, const String& _str)
	{
		if (hWnd) {
			String16 str = _str;
			::SetWindowTextW(hWnd, (LPCWSTR)(str.getData()));
		}
	}

	sl_bool Windows::processWindowHorizontalScrollEvents(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, sl_uint32 nLine, sl_uint32 nWheel)
	{
		int nSBCode = LOWORD(wParam);

		if (uMsg == WM_HSCROLL) {

			SCROLLINFO si;
			Base::zeroMemory(&si, sizeof(si));
			si.cbSize = sizeof(si);
			si.fMask = SIF_POS | SIF_PAGE | SIF_RANGE | SIF_TRACKPOS;
			::GetScrollInfo(hWnd, SB_HORZ, &si);
			switch (nSBCode) {
			case SB_TOP:
			case SB_LINEUP:
				si.nPos -= (int)nLine;
				break;
			case SB_BOTTOM:
			case SB_LINEDOWN:
				si.nPos += (int)nLine;
				break;
			case SB_PAGEUP:
				si.nPos -= si.nPage;
				break;
			case SB_PAGEDOWN:
				si.nPos += si.nPage;
				break;
			case SB_THUMBPOSITION:
			case SB_THUMBTRACK:
				si.nPos = si.nTrackPos;
				break;
			}

			if (si.nPos < si.nMin) {
				si.nPos = si.nMin;
			}
			if (si.nPos >= si.nMax) {
				si.nPos = si.nMax - 1;
			}

			si.fMask = SIF_POS;
			::SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);

			return sl_true;

		} else if (uMsg == 0x020E) {
			// WM_MOUSEHWHEEL
			int delta = (short)((wParam >> 16) & 0xffff);

			if (delta != 0) {

				SCROLLINFO si;
				Base::zeroMemory(&si, sizeof(si));
				si.cbSize = sizeof(si);
				si.fMask = SIF_POS | SIF_PAGE | SIF_RANGE;
				::GetScrollInfo(hWnd, SB_HORZ, &si);

				si.nPos += delta * (int)nWheel / WHEEL_DELTA;
				if (si.nPos < si.nMin) {
					si.nPos = si.nMin;
				}
				if (si.nPos >= si.nMax) {
					si.nPos = si.nMax - 1;
				}

				si.fMask = SIF_POS;
				::SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
			}
			return sl_true;
		}
		return sl_false;
	}

	sl_bool Windows::processWindowVerticalScrollEvents(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, sl_uint32 nLine, sl_uint32 nWheel)
	{
		int nSBCode = LOWORD(wParam);

		if (uMsg == WM_VSCROLL) {

			SCROLLINFO si;
			Base::zeroMemory(&si, sizeof(si));
			si.cbSize = sizeof(si);
			si.fMask = SIF_POS | SIF_PAGE | SIF_RANGE | SIF_TRACKPOS;
			::GetScrollInfo(hWnd, SB_VERT, &si);

			switch (nSBCode) {
			case SB_TOP:
			case SB_LINEUP:
				si.nPos -= (int)nLine;
				break;
			case SB_BOTTOM:
			case SB_LINEDOWN:
				si.nPos += (int)nLine;
				break;
			case SB_PAGEUP:
				si.nPos -= si.nPage;
				break;
			case SB_PAGEDOWN:
				si.nPos += si.nPage;
				break;
			case SB_THUMBPOSITION:
			case SB_THUMBTRACK:
				si.nPos = si.nTrackPos;
				break;
			}

			if (si.nPos < si.nMin) {
				si.nPos = si.nMin;
			}
			if (si.nPos >= si.nMax) {
				si.nPos = si.nMax - 1;
			}
			si.fMask = SIF_POS;
			::SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

			return sl_true;

		} else if (uMsg == WM_MOUSEWHEEL) {

			int delta = (short)((wParam >> 16) & 0xffff);

			if (delta != 0) {

				SCROLLINFO si;
				Base::zeroMemory(&si, sizeof(si));
				si.cbSize = sizeof(si);
				si.fMask = SIF_POS | SIF_PAGE | SIF_RANGE;
				::GetScrollInfo(hWnd, SB_VERT, &si);

				si.nPos += delta * (int)nWheel / WHEEL_DELTA;
				if (si.nPos < si.nMin) {
					si.nPos = si.nMin;
				}
				if (si.nPos >= si.nMax) {
					si.nPos = si.nMax - 1;
				}

				si.fMask = SIF_POS;
				::SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
			}
			return sl_true;

		}
		return sl_false;
	}

	void Windows::setWindowHorizontalScrollParam(HWND hWnd, sl_int32 nMin, sl_int32 nMax, sl_int32 nPage)
	{
		if (nMax < nMin) {
			nMax = nMin;
		}
		SCROLLINFO si;
		Base::zeroMemory(&si, sizeof(si));
		si.cbSize = sizeof(si);
		si.fMask = SIF_PAGE | SIF_RANGE;
		si.nMin = nMin;
		si.nMax = nMax;
		si.nPage = nPage;
		::SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
	}

	void Windows::setWindowVerticalScrollParam(HWND hWnd, sl_int32 nMin, sl_int32 nMax, sl_int32 nPage)
	{
		if (nMax < nMin) {
			nMax = nMin;
		}
		SCROLLINFO si;
		Base::zeroMemory(&si, sizeof(si));
		si.cbSize = sizeof(si);
		si.fMask = SIF_PAGE | SIF_RANGE;
		si.nMin = nMin;
		si.nMax = nMax;
		si.nPage = nPage;
		::SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
	}

	void Windows::setDebugFlags()
	{
#ifdef SLIB_DEBUG
		int flag = ::_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
		// logically OR leak check bit
		flag |= _CRTDBG_LEAK_CHECK_DF;
		// set the flags again
		::_CrtSetDbgFlag(flag);
#endif
	}

	HMODULE Windows::loadLibrary(const String& _path)
	{
		String16 path = _path;
		if (path.isNotEmpty()) {
			return ::LoadLibraryW((LPCWSTR)(path.getData()));
		} else {
			return 0;
		}
	}

#define LOAD_LIBRARY(name, path) \
	HMODULE Windows::loadLibrary_##name() \
	{ \
		static HMODULE hDll = 0; \
		static sl_bool flagLoad = sl_false; \
		if (!flagLoad) { \
			hDll = loadLibrary(path); \
			flagLoad = sl_true; \
		} \
		return hDll; \
	}

#define GET_API(dll, func) \
	WINAPI_##func Windows::getAPI_##func() { \
		static FARPROC proc = 0; \
		static sl_bool flagLoad = sl_false; \
		if (!flagLoad) { \
			HMODULE hDll = loadLibrary_##dll(); \
			if (hDll) { \
				proc = ::GetProcAddress(hDll, #func); \
			} \
			flagLoad = sl_true; \
		} \
		return (WINAPI_##func)(proc); \
	}

	LOAD_LIBRARY(kernel32, "kernel32.dll")
	GET_API(kernel32, GetQueuedCompletionStatusEx)

	sl_bool Windows::getRegistryValue(HKEY hKeyParent, const String16& path, const String16& name, Variant* out)
	{
		if (!hKeyParent) {
			return sl_false;
		}
		HKEY hKey;
		sl_bool flagOpened = sl_false;
		if (path.isEmpty()) {
			hKey = hKeyParent;
		} else {
			if (path.isEmpty()) {
				return sl_false;
			}
			hKey = NULL;
			::RegOpenKeyExW(hKeyParent, (LPCWSTR)(path.getData()), 0, KEY_QUERY_VALUE, &hKey);
			if (!hKey) {
				return sl_false;
			}
			flagOpened = sl_true;
		}
		DWORD type = 0;
		DWORD size = 0;
		sl_bool flagSuccess = sl_false;
		if (ERROR_SUCCESS == ::RegQueryValueExW(hKey, (LPCWSTR)(name.getData()), NULL, &type, NULL, &size)) {
			if (out) {
				if (size > 0) {
					switch (type) {
						case REG_BINARY:
						case REG_MULTI_SZ:
							{
								SLIB_SCOPED_BUFFER(BYTE, 512, buf, size);
								if (ERROR_SUCCESS == ::RegQueryValueExW(hKey, (LPCWSTR)(name.getData()), NULL, &type, buf, &size)) {
									Memory mem = Memory::create(buf, size);
									if (mem.isNotEmpty()) {
										out->setMemory(mem);
										flagSuccess = sl_true;
									}
								}
							}
							break;
						case REG_EXPAND_SZ:
						case REG_SZ:
							{
								SLIB_SCOPED_BUFFER(BYTE, 512, buf, size);
								if (ERROR_SUCCESS == ::RegQueryValueExW(hKey, (LPCWSTR)(name.getData()), NULL, &type, buf, &size)) {
									String16 s(reinterpret_cast<sl_char16*>(buf), size / 2 - 1);
									out->setString(s);
									flagSuccess = sl_true;
								}
							}
							break;
						case REG_DWORD:
						case REG_DWORD_BIG_ENDIAN:
							if (size == 4) {
								sl_uint32 n;
								if (ERROR_SUCCESS == ::RegQueryValueExW(hKey, (LPCWSTR)(name.getData()), NULL, &type, reinterpret_cast<BYTE*>(&n), &size)) {
									if (size == 4) {
										if (type == REG_DWORD) {
											out->setUint32(n);
										} else {
											out->setUint32(Endian::swap32(n));
										}
										flagSuccess = sl_true;
									}
								}
							}
							break;
						case REG_QWORD:
							if (size == 8) {
								sl_uint64 n;
								if (ERROR_SUCCESS == ::RegQueryValueExW(hKey, (LPCWSTR)(name.getData()), NULL, &type, reinterpret_cast<BYTE*>(&n), &size)) {
									if (size == 8) {
										out->setUint64(n);
										flagSuccess = sl_true;
									}
								}
							}
							break;
						default: // REG_NONE
							out->setNull();
							flagSuccess = sl_true;
							break;
					}
				} else {
					out->setNull();
					flagSuccess = sl_true;
				}
			} else {
				flagSuccess = sl_true;
			}
		}
		if (flagOpened) {
			::RegCloseKey(hKey);
		}
		return flagSuccess;
	}

	sl_bool Windows::setRegistryValue(HKEY hKeyParent, const String16& path, const String16& name, const Variant& value)
	{
		if (!hKeyParent) {
			return sl_false;
		}
		HKEY hKey;
		sl_bool flagOpened = sl_false;
		if (path.isEmpty()) {
			hKey = hKeyParent;
		} else {
			if (path.isEmpty()) {
				return sl_false;
			}
			hKey = NULL;
			::RegOpenKeyExW(hKeyParent, (LPCWSTR)(path.getData()), 0, KEY_SET_VALUE, &hKey);
			if (!hKey) {
				::RegCreateKeyExW(hKeyParent, (LPCWSTR)(path.getData()), NULL, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, NULL);
				if (!hKey) {
					return sl_false;
				}
			}
			flagOpened = sl_true;
		}
		sl_bool flagSuccess = sl_false;
		if (value.isInt64() || value.isUint64()) {
			sl_uint64 n = value.getUint64();
			if (ERROR_SUCCESS == ::RegSetValueExW(hKey, (LPCWSTR)(name.getData()), NULL, REG_QWORD, reinterpret_cast<BYTE*>(&n), 8)) {
				flagSuccess = sl_true;
			}
		} else if (value.isInteger()) {
			sl_uint32 n = value.getUint32();
			if (ERROR_SUCCESS == ::RegSetValueExW(hKey, (LPCWSTR)(name.getData()), NULL, REG_DWORD, reinterpret_cast<BYTE*>(&n), 4)) {
				flagSuccess = sl_true;
			}
		} else if (value.isMemory()) {
			Memory mem = value.getMemory();
			if (mem.isNotNull()) {
				if (ERROR_SUCCESS == ::RegSetValueExW(hKey, (LPCWSTR)(name.getData()), NULL, REG_BINARY, reinterpret_cast<BYTE*>(mem.getData()), (DWORD)(mem.getSize()))) {
					flagSuccess = sl_true;
				}
			}
		} else if (value.isString()) {
			String16 str = value.getString16();
			if (ERROR_SUCCESS == ::RegSetValueExW(hKey, (LPCWSTR)(name.getData()), NULL, REG_SZ, reinterpret_cast<BYTE*>(str.getData()), (DWORD)(str.getLength() + 1) * 2)) {
				flagSuccess = sl_true;
			}
		}
		if (flagOpened) {
			::RegCloseKey(hKey);
		}
		return flagSuccess;
	}

}

#endif
