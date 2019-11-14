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

#include "slib/core/platform_windows.h"

#include "slib/core/scoped.h"
#include "slib/core/variant.h"
#include "slib/core/endian.h"

#include <crtdbg.h>
#include <shlwapi.h>

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
		HRESULT hr = CLSIDFromString((LPWSTR)(str.getData()), &clsid);
		if (hr == NOERROR) {
			if (pguid) {
				*pguid = clsid;
			}
			return sl_true;
		}
		return sl_false;
	}

	HGLOBAL Windows::createGlobalData(const void* data, sl_size size)
	{
		HGLOBAL handle = GlobalAlloc(GMEM_MOVEABLE, size);
		if (handle) {
			void* dst = GlobalLock(handle);
			if (dst) {
				Base::copyMemory(dst, data, size);
				GlobalUnlock(dst);
			}
		}
		return handle;
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
		return GetWindow(hWnd, GW_OWNER);
	}

	HWND Windows::getParentWindow(HWND hWnd)
	{
		return GetAncestor(hWnd, GA_PARENT);
	}

	HWND Windows::getRootWindow(HWND hWnd)
	{
		return GetAncestor(hWnd, GA_ROOT);
	}

	void Windows::getWindowFrame(HWND hWnd, RECT& rc)
	{
		GetWindowRect(hWnd, &rc);
		HWND hWndParent = Windows::getParentWindow(hWnd);
		if (hWndParent) {
			ScreenToClient(hWnd, (POINT*)(&rc));
			ScreenToClient(hWnd, (POINT*)(&rc) + 1);
		} else {
			rc.right -= rc.left;
			rc.bottom -= rc.top;
			rc.left = 0;
			rc.top = 0;
		}
	}

	String Windows::getWindowText(HWND hWnd)
	{
		sl_int32 len = GetWindowTextLengthW(hWnd);
		if (len > 0) {
			SLIB_SCOPED_BUFFER(WCHAR, 1024, buf, len + 2);
			if (buf) {
				len = GetWindowTextW(hWnd, buf, len + 1);
				return String(buf, len);
			}
		}
		return sl_null;
	}

	String16 Windows::getWindowText16(HWND hWnd)
	{
		sl_int32 len = GetWindowTextLengthW(hWnd);
		if (len > 0) {
			SLIB_SCOPED_BUFFER(WCHAR, 1024, buf, len + 2);
			if (buf) {
				len = GetWindowTextW(hWnd, buf, len + 1);
				return String16(buf, len);
			}
		}
		return sl_null;
	}

	void Windows::setWindowText(HWND hWnd, const String16& str)
	{
		if (hWnd) {
			SetWindowTextW(hWnd, (LPCWSTR)(str.getData()));
		}
	}

	void Windows::setWindowStyle(HWND hWnd, LONG flags, sl_bool flagAdd)
	{
		if (!hWnd) {
			return;
		}
		LONG old = GetWindowLongW(hWnd, GWL_STYLE);
		if (flagAdd) {
			SetWindowLongW(hWnd, GWL_STYLE, old | flags);
		} else {
			SetWindowLongW(hWnd, GWL_STYLE, old & (~flags));
		}
		SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
			SWP_FRAMECHANGED | SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS);
	}

	void Windows::removeAndAddWindowStyle(HWND hWnd, LONG flagsRemove, LONG flagsAdd)
	{
		if (!hWnd) {
			return;
		}
		LONG old = GetWindowLongW(hWnd, GWL_STYLE);
		SetWindowLongW(hWnd, GWL_STYLE, (old & (~flagsRemove)) | flagsAdd);
		SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
			SWP_FRAMECHANGED | SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS);
	}

	void Windows::setWindowExStyle(HWND hWnd, LONG flags, sl_bool flagAdd)
	{
		if (!hWnd) {
			return;
		}
		LONG old = GetWindowLongW(hWnd, GWL_EXSTYLE);
		if (flagAdd) {
			SetWindowLongW(hWnd, GWL_EXSTYLE, old | flags);
		} else {
			SetWindowLongW(hWnd, GWL_EXSTYLE, old & (~flags));
		}
		SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
			SWP_FRAMECHANGED | SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS);
	}

	void Windows::removeAndAddWindowExStyle(HWND hWnd, LONG flagsRemove, LONG flagsAdd)
	{
		if (!hWnd) {
			return;
		}
		LONG old = GetWindowLongW(hWnd, GWL_EXSTYLE);
		SetWindowLongW(hWnd, GWL_EXSTYLE, (old & (~flagsRemove)) | flagsAdd);
		SetWindowPos(hWnd, NULL, 0, 0, 0, 0,
			SWP_FRAMECHANGED | SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS);
	}

	sl_bool Windows::processWindowHorizontalScrollEvents(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, sl_uint32 nLine, sl_uint32 nWheel)
	{
		int nSBCode = LOWORD(wParam);

		if (uMsg == WM_HSCROLL) {

			SCROLLINFO si;
			Base::zeroMemory(&si, sizeof(si));
			si.cbSize = sizeof(si);
			si.fMask = SIF_POS | SIF_PAGE | SIF_RANGE | SIF_TRACKPOS;
			GetScrollInfo(hWnd, SB_HORZ, &si);
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
			SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);

			return sl_true;

		} else if (uMsg == 0x020E) {
			// WM_MOUSEHWHEEL
			int delta = (short)((wParam >> 16) & 0xffff);

			if (delta != 0) {

				SCROLLINFO si;
				Base::zeroMemory(&si, sizeof(si));
				si.cbSize = sizeof(si);
				si.fMask = SIF_POS | SIF_PAGE | SIF_RANGE;
				GetScrollInfo(hWnd, SB_HORZ, &si);

				si.nPos += delta * (int)nWheel / WHEEL_DELTA;
				if (si.nPos < si.nMin) {
					si.nPos = si.nMin;
				}
				if (si.nPos >= si.nMax) {
					si.nPos = si.nMax - 1;
				}

				si.fMask = SIF_POS;
				SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
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
			GetScrollInfo(hWnd, SB_VERT, &si);

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
			SetScrollInfo(hWnd, SB_VERT, &si, TRUE);

			return sl_true;

		} else if (uMsg == WM_MOUSEWHEEL) {

			int delta = (short)((wParam >> 16) & 0xffff);

			if (delta != 0) {

				SCROLLINFO si;
				Base::zeroMemory(&si, sizeof(si));
				si.cbSize = sizeof(si);
				si.fMask = SIF_POS | SIF_PAGE | SIF_RANGE;
				GetScrollInfo(hWnd, SB_VERT, &si);

				si.nPos += delta * (int)nWheel / WHEEL_DELTA;
				if (si.nPos < si.nMin) {
					si.nPos = si.nMin;
				}
				if (si.nPos >= si.nMax) {
					si.nPos = si.nMax - 1;
				}

				si.fMask = SIF_POS;
				SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
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
		SetScrollInfo(hWnd, SB_HORZ, &si, TRUE);
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
		SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
	}

	void Windows::setDebugFlags()
	{
#ifdef SLIB_DEBUG
		int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
		// logically OR leak check bit
		flag |= _CRTDBG_LEAK_CHECK_DF;
		// set the flags again
		_CrtSetDbgFlag(flag);
#endif
	}

	namespace priv
	{
		namespace platform
		{
			WINDOWS_DEBUG_ALLOC_HOOK g_debugAllocHook;

			int DebugAllocHook(int allocType, void* userData, size_t size, int blockType, long requestNumber, const unsigned char* filename, int lineNumber)
			{
				return g_debugAllocHook(userData, (sl_size)size);
			}
		}
	}

	void Windows::setDebugAllocHook(WINDOWS_DEBUG_ALLOC_HOOK hook)
	{
#ifdef SLIB_DEBUG
		priv::platform::g_debugAllocHook = hook;
		_CrtSetAllocHook(priv::platform::DebugAllocHook);
#endif
	}

	HMODULE Windows::loadLibrary(const String& _path)
	{
		String16 path = _path;
		if (path.isNotEmpty()) {
			return LoadLibraryW((LPCWSTR)(path.getData()));
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
				proc = GetProcAddress(hDll, #func); \
			} \
			flagLoad = sl_true; \
		} \
		return (WINAPI_##func)(proc); \
	}

	LOAD_LIBRARY(kernel32, "kernel32.dll")
	GET_API(kernel32, GetQueuedCompletionStatusEx)
	GET_API(kernel32, GetUserDefaultLocaleName)
	GET_API(kernel32, GetTickCount64)

	LOAD_LIBRARY(user32, "user32.dll")
	GET_API(user32, ShowScrollBar)

	LOAD_LIBRARY(wininet, "wininet.dll")

	LOAD_LIBRARY(bcrypt, "bcrypt.dll")
	GET_API(bcrypt, BCryptOpenAlgorithmProvider)
	GET_API(bcrypt, BCryptCloseAlgorithmProvider)
	GET_API(bcrypt, BCryptGenRandom)

	LOAD_LIBRARY(gdiplus, "gdiplus.dll")
	GET_API(gdiplus, GdipCreateEffect)
	GET_API(gdiplus, GdipDeleteEffect)
	GET_API(gdiplus, GdipSetEffectParameters)
	GET_API(gdiplus, GdipDrawImageFX)

	extern "C"
	{

		Gdiplus::Status __stdcall SLIB_GdipCreateEffect(const GUID guid, Gdiplus::CGpEffect **effect)
		{
			WINAPI_GdipCreateEffect func = Windows::getAPI_GdipCreateEffect();
			if (func) {
				return func(guid, effect);
			}
			return Gdiplus::AccessDenied;
		}

		Gdiplus::Status __stdcall SLIB_GdipDeleteEffect(Gdiplus::CGpEffect *effect)
		{
			WINAPI_GdipDeleteEffect func = Windows::getAPI_GdipDeleteEffect();
			if (func) {
				return func(effect);
			}
			return Gdiplus::AccessDenied;
		}

		Gdiplus::Status __stdcall SLIB_GdipSetEffectParameters(Gdiplus::CGpEffect *effect, const VOID *params, const UINT size)
		{
			WINAPI_GdipSetEffectParameters func = Windows::getAPI_GdipSetEffectParameters();
			if (func) {
				return func(effect, params, size);
			}
			return Gdiplus::AccessDenied;
		}

		Gdiplus::GpStatus WINGDIPAPI SLIB_GdipDrawImageFX(
			Gdiplus::GpGraphics *graphics,
			Gdiplus::GpImage *image,
			Gdiplus::GpRectF *source,
			Gdiplus::GpMatrix *xForm,
			Gdiplus::CGpEffect *effect,
			Gdiplus::GpImageAttributes *imageAttributes,
			Gdiplus::GpUnit srcUnit)
		{
			WINAPI_GdipDrawImageFX func = Windows::getAPI_GdipDrawImageFX();
			if (func) {
				return func(graphics, image, source, xForm, effect, imageAttributes, srcUnit);
			}
			return Gdiplus::AccessDenied;
		}

	}

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
			RegOpenKeyExW(hKeyParent, (LPCWSTR)(path.getData()), 0, KEY_QUERY_VALUE, &hKey);
			if (!hKey) {
				return sl_false;
			}
			flagOpened = sl_true;
		}
		DWORD type = 0;
		DWORD size = 0;
		sl_bool flagSuccess = sl_false;
		if (ERROR_SUCCESS == RegQueryValueExW(hKey, (LPCWSTR)(name.getData()), NULL, &type, NULL, &size)) {
			if (out) {
				if (size > 0) {
					switch (type) {
					case REG_BINARY:
					case REG_MULTI_SZ:
					{
						SLIB_SCOPED_BUFFER(BYTE, 512, buf, size);
						if (ERROR_SUCCESS == RegQueryValueExW(hKey, (LPCWSTR)(name.getData()), NULL, &type, buf, &size)) {
							Memory mem = Memory::create(buf, size);
							if (mem.isNotNull()) {
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
						if (ERROR_SUCCESS == RegQueryValueExW(hKey, (LPCWSTR)(name.getData()), NULL, &type, buf, &size)) {
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
							if (ERROR_SUCCESS == RegQueryValueExW(hKey, (LPCWSTR)(name.getData()), NULL, &type, reinterpret_cast<BYTE*>(&n), &size)) {
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
							if (ERROR_SUCCESS == RegQueryValueExW(hKey, (LPCWSTR)(name.getData()), NULL, &type, reinterpret_cast<BYTE*>(&n), &size)) {
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
			RegCloseKey(hKey);
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
			RegOpenKeyExW(hKeyParent, (LPCWSTR)(path.getData()), 0, KEY_SET_VALUE, &hKey);
			if (!hKey) {
				RegCreateKeyExW(hKeyParent, (LPCWSTR)(path.getData()), NULL, NULL, 0, KEY_ALL_ACCESS, NULL, &hKey, NULL);
				if (!hKey) {
					return sl_false;
				}
			}
			flagOpened = sl_true;
		}
		sl_bool flagSuccess = sl_false;
		if (value.isNull()) {
			if (ERROR_SUCCESS == RegDeleteValueW(hKey, (LPCWSTR)(name.getData()))) {
				flagSuccess = sl_true;
			}
		} else if (value.isInt64() || value.isUint64()) {
			sl_uint64 n = value.getUint64();
			if (ERROR_SUCCESS == RegSetValueExW(hKey, (LPCWSTR)(name.getData()), NULL, REG_QWORD, reinterpret_cast<BYTE*>(&n), 8)) {
				flagSuccess = sl_true;
			}
		} else if (value.isInteger()) {
			sl_uint32 n = value.getUint32();
			if (ERROR_SUCCESS == RegSetValueExW(hKey, (LPCWSTR)(name.getData()), NULL, REG_DWORD, reinterpret_cast<BYTE*>(&n), 4)) {
				flagSuccess = sl_true;
			}
		} else if (value.isMemory()) {
			Memory mem = value.getMemory();
			if (mem.isNotNull()) {
				if (ERROR_SUCCESS == RegSetValueExW(hKey, (LPCWSTR)(name.getData()), NULL, REG_BINARY, reinterpret_cast<BYTE*>(mem.getData()), (DWORD)(mem.getSize()))) {
					flagSuccess = sl_true;
				}
			}
		} else if (value.isString()) {
			String16 str = value.getString16();
			if (ERROR_SUCCESS == RegSetValueExW(hKey, (LPCWSTR)(name.getData()), NULL, REG_SZ, reinterpret_cast<BYTE*>(str.getData()), (DWORD)(str.getLength() + 1) * 2)) {
				flagSuccess = sl_true;
			}
		}
		if (flagOpened) {
			RegCloseKey(hKey);
		}
		return flagSuccess;
	}

	void Windows::setApplicationRunAtStartup(const String16& appName, const String16& path, sl_bool flagRegister)
	{
		List<String16> listDelete;
		HKEY hKey = NULL;
		RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Run", 0, KEY_QUERY_VALUE | KEY_SET_VALUE, &hKey);
		if (hKey) {
			DWORD dwIndex = 0;
			sl_char16 name[513] = { 0 };
			sl_char16 data[1025] = { 0 };
			for (;;) {
				DWORD dwType = 0;
				DWORD dwLenName = 512;
				DWORD nData = 1024;
				LSTATUS lRet = RegEnumValueW(hKey, dwIndex, (LPWSTR)name, &dwLenName, NULL, &dwType, (LPBYTE)data, &nData);
				if (lRet == ERROR_SUCCESS) {
					if (dwType == REG_SZ) {
						if (path == data) {
							if (flagRegister) {
								// already registered
								return;
							} else {
								listDelete.add_NoLock(name);
							}
						}
					}
				} else {
					break;
				}
				dwIndex++;
			}
			if (flagRegister) {
				RegSetValueExW(hKey, (LPCWSTR)(appName.getData()), NULL, REG_SZ, (BYTE*)(path.getData()), (DWORD)(path.getLength() + 1) * 2);
			} else {
				ListElements<String16> names(listDelete);
				for (sl_size i = 0; i < names.count; i++) {
					RegDeleteValueW(hKey, (LPCWSTR)(names[i].getData()));
				}
			}
			RegCloseKey(hKey);
		}
	}

	namespace priv
	{
		namespace platform
		{

			// From VersionHelpers.h
			static sl_bool IsWindowsVersionOrGreater(WindowsVersion version)
			{
				OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0, {0}, 0, 0 };
				DWORDLONG const dwlConditionMask = VerSetConditionMask(
					VerSetConditionMask(
						VerSetConditionMask(
							0, VER_MAJORVERSION, VER_GREATER_EQUAL),
						VER_MINORVERSION, VER_GREATER_EQUAL),
					VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);
				osvi.dwMajorVersion = SLIB_WINDOWS_MAJOR_VERSION(version);
				osvi.dwMinorVersion = SLIB_WINDOWS_MINOR_VERSION(version);
				osvi.wServicePackMajor = SLIB_WINDOWS_SERVICE_PACK(version);
				return VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR, dwlConditionMask) != FALSE;
			}

			static sl_bool IsWindowsServer()
			{
				OSVERSIONINFOEXW osvi = { sizeof(osvi), 0, 0, 0, 0, {0}, 0, 0, 0, VER_NT_WORKSTATION };
				DWORDLONG const dwlConditionMask = VerSetConditionMask(0, VER_PRODUCT_TYPE, VER_EQUAL);
				return !VerifyVersionInfoW(&osvi, VER_PRODUCT_TYPE, dwlConditionMask);
			}

			static WindowsVersion GetWindowsVersion()
			{
				if (IsWindowsServer()) {
					if (IsWindowsVersionOrGreater(WindowsVersion::Server2016)) {
						return WindowsVersion::Server2016;
					}
					if (IsWindowsVersionOrGreater(WindowsVersion::Server2012_R2)) {
						return WindowsVersion::Server2012_R2;
					}
					if (IsWindowsVersionOrGreater(WindowsVersion::Server2012)) {
						return WindowsVersion::Server2012;
					}
					if (IsWindowsVersionOrGreater(WindowsVersion::Server2008_R2)) {
						return WindowsVersion::Server2008_R2;
					}
					if (IsWindowsVersionOrGreater(WindowsVersion::Server2008)) {
						return WindowsVersion::Server2008;
					}
					return WindowsVersion::Server2003;
				} else {
					if (IsWindowsVersionOrGreater(WindowsVersion::Windows10)) {
						return WindowsVersion::Windows10;
					}
					if (IsWindowsVersionOrGreater(WindowsVersion::Windows8_1)) {
						return WindowsVersion::Windows8_1;
					}
					if (IsWindowsVersionOrGreater(WindowsVersion::Windows8)) {
						return WindowsVersion::Windows8;
					}
					if (IsWindowsVersionOrGreater(WindowsVersion::Windows7_SP1)) {
						return WindowsVersion::Windows7_SP1;
					}
					if (IsWindowsVersionOrGreater(WindowsVersion::Windows7)) {
						return WindowsVersion::Windows7;
					}
					if (IsWindowsVersionOrGreater(WindowsVersion::Vista_SP2)) {
						return WindowsVersion::Vista_SP2;
					}
					if (IsWindowsVersionOrGreater(WindowsVersion::Vista_SP1)) {
						return WindowsVersion::Vista_SP1;
					}
					if (IsWindowsVersionOrGreater(WindowsVersion::Vista)) {
						return WindowsVersion::Vista;
					}
					if (IsWindowsVersionOrGreater(WindowsVersion::XP_64)) {
						return WindowsVersion::XP_64;
					}
					if (IsWindowsVersionOrGreater(WindowsVersion::XP_SP3)) {
						return WindowsVersion::XP_SP3;
					}
					if (IsWindowsVersionOrGreater(WindowsVersion::XP_SP2)) {
						return WindowsVersion::XP_SP2;
					}
					if (IsWindowsVersionOrGreater(WindowsVersion::XP_SP1)) {
						return WindowsVersion::XP_SP1;
					}
					return WindowsVersion::XP;
				}
			}

		}
	}

	WindowsVersion Windows::getVersion()
	{
		return priv::platform::GetWindowsVersion();
	}

	WindowsDllVersion Windows::getDllVersion(const String16& pathDll)
	{
		WindowsDllVersion ret;
		ret.major = 0;
		ret.minor = 0;
		ret.build = 0;
		HINSTANCE hDll = LoadLibraryW((LPCWSTR)(pathDll.getData()));
		if (hDll) {
			DLLGETVERSIONPROC proc = (DLLGETVERSIONPROC)(GetProcAddress(hDll, "DllGetVersion"));
			if (proc) {
				DLLVERSIONINFO info;
				Base::zeroMemory(&info, sizeof(info));
				info.cbSize = sizeof(info);
				HRESULT hr = proc(&info);
				if (SUCCEEDED(hr)) {
					ret.major = (sl_uint32)(info.dwMajorVersion);
					ret.minor = (sl_uint32)(info.dwMinorVersion);
					ret.build = (sl_uint32)(info.dwBuildNumber);
				}
			}
			FreeLibrary(hDll);
		}
		return ret;
	}

	sl_bool Windows::isCurrentProcessInAdminGroup()
	{
		BOOL flagResult = FALSE;
		HANDLE hToken;
		if (::OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_DUPLICATE, &hToken)) {
			WindowsVersion version = priv::platform::GetWindowsVersion();
			sl_bool flagError = sl_false;
			HANDLE hTokenToCheck = NULL;
			if (SLIB_WINDOWS_MAJOR_VERSION(version) >= 6) { // Windows Vista or later
				TOKEN_ELEVATION_TYPE elevType;
				DWORD cbSize = 0;
				if (::GetTokenInformation(hToken, TokenElevationType, &elevType, sizeof(elevType), &cbSize)) {
					if (elevType == TokenElevationTypeLimited) {
						if (!::GetTokenInformation(hToken, TokenLinkedToken, &hTokenToCheck, sizeof(hTokenToCheck), &cbSize)) {
							flagError = sl_true;
						}
					}
				} else {
					flagError = sl_true;
				}
			}
			if (!flagError) {
				if (!hTokenToCheck) {
					DuplicateToken(hToken, SecurityIdentification, &hTokenToCheck);
				}
				if (hTokenToCheck) {
					BYTE adminSID[SECURITY_MAX_SID_SIZE];
					DWORD cbSize = sizeof(adminSID);
					if (::CreateWellKnownSid(WinBuiltinAdministratorsSid, NULL, &adminSID, &cbSize)) {
						CheckTokenMembership(hTokenToCheck, &adminSID, &flagResult);
					}
					CloseHandle(hTokenToCheck);
				}
			}
			CloseHandle(hToken);
		}
		return flagResult != FALSE;
	}

	sl_bool Windows::isCurrentProcessRunAsAdmin()
	{
		BOOL flagResult = FALSE;
		SID_IDENTIFIER_AUTHORITY siAuthority = SECURITY_NT_AUTHORITY;
		PSID pSidAdmin = NULL;
		if (::AllocateAndInitializeSid(&siAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pSidAdmin)) {
			CheckTokenMembership(NULL, pSidAdmin, &flagResult);
			FreeSid(pSidAdmin);
		}
		return flagResult != FALSE;
	}

	Windows::ShellExecuteParam::ShellExecuteParam()
	{
		runAsAdmin = sl_false;
		hWndParent = NULL;
		nShow = SW_NORMAL;
	}

	Windows::ShellExecuteParam::~ShellExecuteParam() {}
	Windows::ShellExecuteParam::ShellExecuteParam(const ShellExecuteParam& other) = default;
	Windows::ShellExecuteParam::ShellExecuteParam(ShellExecuteParam&& other) = default;
	Windows::ShellExecuteParam& Windows::ShellExecuteParam::operator=(const ShellExecuteParam& other) = default;
	Windows::ShellExecuteParam& Windows::ShellExecuteParam::operator=(ShellExecuteParam&& other) = default;

	sl_bool Windows::shellExecute(const ShellExecuteParam& param)
	{
		SHELLEXECUTEINFOW sei;
		Base::zeroMemory(&sei, sizeof(sei));
		sei.cbSize = sizeof(sei);
		if (param.runAsAdmin) {
			sei.lpVerb = L"runas";
		} else if (param.operation.isNotEmpty()) {
			sei.lpVerb = (LPCWSTR)(param.operation.getData());
		}
		sei.lpFile = (LPCWSTR)(param.path.getData());
		if (param.params.isNotEmpty()) {
			sei.lpParameters = (LPCWSTR)(param.params.getData());
		}
		if (param.currentDirectory.isNotEmpty()) {
			sei.lpDirectory = (LPCWSTR)(param.currentDirectory.getData());
		}
		sei.hwnd = param.hWndParent;
		sei.nShow = param.nShow;
		if (::ShellExecuteExW(&sei)) {
			return sl_true;
		}
		return sl_false;
	}

	sl_bool Windows::getSYSTEMTIME(const Time& time, sl_bool flagUTC, SYSTEMTIME* _out)
	{
		SYSTEMTIME& st = *_out;
		sl_int64 n = (time.toInt() + SLIB_INT64(11644473600000000)) * 10;  // Convert 1970 Based (time_t mode) to 1601 Based (FILETIME mode)
		if (flagUTC) {
			if (!(FileTimeToSystemTime((PFILETIME)&n, &st))) {
				return sl_false;
			}
		} else {
			SYSTEMTIME utc;
			if (!(FileTimeToSystemTime((PFILETIME)&n, &utc))) {
				return sl_false;
			}
			if (!(SystemTimeToTzSpecificLocalTime(NULL, &utc, &st))) {
				return sl_false;
			}
		}
		return sl_true;
	}

	Time Windows::getTime(const SYSTEMTIME* _in, sl_bool flagUTC)
	{
		const SYSTEMTIME& st = *_in;
		sl_int64 n = 0;
		if (flagUTC) {
			SystemTimeToFileTime(&st, (PFILETIME)&n);
		} else {
			SYSTEMTIME utc;
			TzSpecificLocalTimeToSystemTime(NULL, &st, &utc);
			SystemTimeToFileTime(&utc, (PFILETIME)&n);
		}
		return n / 10 - SLIB_INT64(11644473600000000);  // Convert 1601 Based (FILETIME mode) to 1970 Based (time_t mode)
	}

}

#endif
