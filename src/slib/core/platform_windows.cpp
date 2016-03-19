#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WINDOWS)

#include "../../../inc/slib/core/platform_windows.h"
#include "../../../inc/slib/core/scoped_pointer.h"

SLIB_NAMESPACE_BEGIN

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
	hWnd = ::GetParent(hWnd);
	if (hWnd) {
		return Windows::isWindowVisible(hWnd);
	}
	return sl_true;
}

HWND Windows::getRootWindow(HWND hWnd)
{
	HWND hWndParent = ::GetParent(hWnd);
	if (hWndParent) {
		return Windows::getRootWindow(hWndParent);
	} else {
		return hWndParent;
	}
}

static HWND _g_app_main_window_win32 = 0;
HWND Windows::getAppMainWindow()
{
	return _g_app_main_window_win32;
}

void Windows::setAppMainWindow(HWND hWnd)
{
	_g_app_main_window_win32 = hWnd;
}

String Windows::getWindowText(HWND hWnd)
{
	sl_int32 len = ::GetWindowTextLengthW(hWnd);
	if (len > 0) {
		SLIB_SCOPED_BUFFER(WCHAR, 1024, buf, len + 2);
		if (buf) {
			len = ::GetWindowTextW(hWnd, buf, len + 1);
			return String::fromUtf16(buf, len);
		}
	}
	return String::null();
}

void Windows::setWindowText(HWND hWnd, const String& _str)
{
	if (hWnd) {
		String16 str = _str;
		::SetWindowTextW(hWnd, (LPCWSTR)(str.getData()));
	}
}

sl_bool Windows::processWindowScrollEvents(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, sl_uint32 nLine, sl_uint32 nWheel)
{
	int nSBCode = LOWORD(wParam);
	int nPos = HIWORD(wParam);

	if (uMsg == WM_HSCROLL) {

		SCROLLINFO si;
		Base::zeroMemory(&si, sizeof(si));
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS | SIF_PAGE | SIF_RANGE;
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
			si.nPos = nPos;
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

	} else if (uMsg == WM_VSCROLL) {

		SCROLLINFO si;
		Base::zeroMemory(&si, sizeof(si));
		si.cbSize = sizeof(si);
		si.fMask = SIF_POS | SIF_PAGE | SIF_RANGE;
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
			si.nPos = nPos;
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

void Windows::setWindowHorizontalScrollParam(HWND hWnd, sl_int32 nMin, sl_int32 nMax, sl_int32 nPage)
{
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
	SCROLLINFO si;
	Base::zeroMemory(&si, sizeof(si));
	si.cbSize = sizeof(si);
	si.fMask = SIF_PAGE | SIF_RANGE;
	si.nMin = nMin;
	si.nMax = nMax;
	si.nPage = nPage;
	SetScrollInfo(hWnd, SB_VERT, &si, TRUE);
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

SLIB_NAMESPACE_END

#endif
