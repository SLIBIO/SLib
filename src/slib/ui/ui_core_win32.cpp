#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/screen.h"
#include "../../../inc/slib/ui/platform.h"

#include "../../../inc/slib/core/queue.h"

#include <commctrl.h>

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "comctl32.lib")

SLIB_UI_NAMESPACE_BEGIN

DWORD _g_thread_ui = 0;

class _Win32_UI
{
public:
	Ref<Screen> m_screenPrimary;
	Queue< Ref<Runnable> > m_queueDispatch;
	
	_Win32_UI();

	static _Win32_UI* get()
	{
		SLIB_SAFE_STATIC(_Win32_UI, ui);
		return &ui;
	}
};

class _Win32_Screen : public Screen
{
public:
	_Win32_Screen()
	{
	}

	Rectangle getRegion()
	{
		Rectangle ret;
		ret.left = 0;
		ret.top = 0;
		ret.right = (sl_real)(GetSystemMetrics(SM_CXSCREEN));
		ret.bottom = (sl_real)(GetSystemMetrics(SM_CYSCREEN));
		return ret;
	}
};

List< Ref<Screen> > UI::getScreens()
{
	_Win32_UI* ui = _Win32_UI::get();
	List< Ref<Screen> > ret;
	ret.add(ui->m_screenPrimary);
	return ret;
}

Ref<Screen> UI::getPrimaryScreen()
{
	_Win32_UI* ui = _Win32_UI::get();
	return ui->m_screenPrimary;
}

Ref<Screen> UI::getFocusedScreen()
{
	_Win32_UI* ui = _Win32_UI::get();
	return ui->m_screenPrimary;
}

sl_bool UI::isUIThread()
{
	return (_g_thread_ui == ::GetCurrentThreadId());
}

void UI::runOnUIThread(const Ref<Runnable>& _callback)
{
	_Win32_UI* ui = _Win32_UI::get();
	Ref<Runnable> callback = _callback;
	if (callback.isNotNull()) {
		ui->m_queueDispatch.push(callback);
		::PostThreadMessageW(_g_thread_ui, SLIB_UI_MESSAGE_DISPATCH, 0, 0);
	}
}

void UI::runLoop()
{
	_Win32_UI* ui = _Win32_UI::get();
	_g_thread_ui = ::GetCurrentThreadId();

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	::OleInitialize(NULL);

	INITCOMMONCONTROLSEX icex;
	icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_TAB_CLASSES;
	InitCommonControlsEx(&icex);

	UIApp::dispatchStart();

	MSG msg;
	while (::GetMessageW(&msg, NULL, 0, 0)) {
		if (msg.message == WM_QUIT) {
			break;
		}
		if (msg.message == SLIB_UI_MESSAGE_DISPATCH) {
			Ref<Runnable> callback;
			while (ui->m_queueDispatch.pop(&callback)) {
				if (callback.isNotNull()) {
					callback->run();
				}
			}
		} else if (msg.message == SLIB_UI_MESSAGE_CLOSE) {
			::DestroyWindow(msg.hwnd);
		} else {
			HWND hWnd = ::GetActiveWindow();
			sl_bool flagDefaultProcess = sl_true;
			if (hWnd) {
				if (::IsDialogMessageW(hWnd, &msg)) {
					flagDefaultProcess = sl_false;
				}
			}
			if (flagDefaultProcess) {
				::TranslateMessage(&msg);
				::DispatchMessageW(&msg);
			}
		}
	}

	ui->m_queueDispatch.clear();

	UIApp::dispatchExit();

	//Gdiplus::GdiplusShutdown(gdiplusToken);
}

void UI::quitLoop()
{
	::PostQuitMessage(0);
}

#define CMB_MSG_INIT (WM_USER+1218)
LRESULT CALLBACK _CustomizedMsgBox_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == CMB_MSG_INIT) {
		HWND hWndMsg = ::FindWindowW(NULL, L"CustomizedMsgBox");
		if (hWndMsg == NULL) {
			return 0;
		}
		AlertParam& param = *((AlertParam*)lParam);
		String16 caption = param.caption;
		::SetWindowTextW(hWndMsg, (LPCWSTR)(caption.getBuf()));

		switch (param.type) {
		case AlertParam::typeOk:
			if (param.titleOk.isNotNull()) {
				String16 titleOk = param.titleOk;
				::SetDlgItemTextW(hWndMsg, 2, (LPCWSTR)(titleOk.getBuf()));
			}
			break;
		case AlertParam::typeOkCancel:
			if (param.titleOk.isNotNull()) {
				String16 titleOk = param.titleOk;
				::SetDlgItemTextW(hWndMsg, 1, (LPCWSTR)(titleOk.getBuf()));
			}
			if (param.titleCancel.isNotNull()) {
				String16 titleCancel = param.titleCancel;
				::SetDlgItemTextW(hWndMsg, 2, (LPCWSTR)(titleCancel.getBuf()));
			}
			break;
		case AlertParam::typeYesNo:
			if (param.titleYes.isNotNull()) {
				String16 titleYes = param.titleYes;
				::SetDlgItemTextW(hWndMsg, 6, (LPCWSTR)(titleYes.getBuf()));
			}
			if (param.titleNo.isNotNull()) {
				String16 titleNo = param.titleNo;
				::SetDlgItemTextW(hWndMsg, 7, (LPCWSTR)(titleNo.getBuf()));
			}
			break;
		case AlertParam::typeYesNoCancel:
			if (param.titleYes.isNotNull()) {
				String16 titleYes = param.titleYes;
				::SetDlgItemTextW(hWndMsg, 6, (LPCWSTR)(titleYes.getBuf()));
			}
			if (param.titleNo.isNotNull()) {
				String16 titleNo = param.titleNo;
				::SetDlgItemTextW(hWndMsg, 7, (LPCWSTR)(titleNo.getBuf()));
			}
			if (param.titleCancel.isNotNull()) {
				String16 titleCancel = param.titleCancel;
				::SetDlgItemTextW(hWndMsg, 2, (LPCWSTR)(titleCancel.getBuf()));
			}
			break;
		}
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
void _UI_AlertProc(AlertParam param)
{
	int style = MB_OK;
	switch (param.type) {
	case AlertParam::typeOkCancel:
		style = MB_OKCANCEL;
		break;
	case AlertParam::typeYesNo:
		style = MB_YESNO;
		break;
	case AlertParam::typeYesNoCancel:
		style = MB_YESNOCANCEL;
		break;
	}

	HWND hWndParent = ::GetActiveWindow();

	static HWND hWndMsgBoxCustom = NULL;
	if (!hWndMsgBoxCustom) {
		WNDCLASSW wc;
		::ZeroMemory(&wc, sizeof(wc));
		wc.hbrBackground = (HBRUSH)(::GetStockObject(BLACK_BRUSH));
		wc.hInstance = ::GetModuleHandleW(NULL);
		wc.lpfnWndProc = _CustomizedMsgBox_WndProc;
		wc.lpszClassName = L"CustomizedMsgBoxInitializer";
		::RegisterClassW(&wc);
		hWndMsgBoxCustom = ::CreateWindowW(wc.lpszClassName, L"", WS_POPUP, 0, 0, 0, 0, NULL, 0, wc.hInstance, 0);
	}
	int result;
	String16 text = param.text;
	if (hWndMsgBoxCustom) {
		::PostMessage(hWndMsgBoxCustom, CMB_MSG_INIT, 0, (LPARAM)(&param));
		result = ::MessageBoxW(hWndParent, (LPCWSTR)(text.getBuf()), L"CustomizedMsgBox", style);
	} else {
		String16 caption = param.caption;
		result = ::MessageBoxW(hWndParent, (LPCWSTR)(text.getBuf()), (LPCWSTR)(caption.getBuf()), style);
	}

	switch (result) {
	case IDOK:
		if (param.onOk.isNotNull()) {
			param.onOk->run();
		}
		break;
	case IDCANCEL:
		if (param.onCancel.isNotNull()) {
			param.onCancel->run();
		}
		break;
	case IDYES:
		if (param.onYes.isNotNull()) {
			param.onYes->run();
		}
		break;
	case IDNO:
		if (param.onNo.isNotNull()) {
			param.onNo->run();
		}
		break;
	}
}
void UI::showAlert(const AlertParam& param)
{
	UI::runOnUIThread(SLIB_CALLBACK(_UI_AlertProc, param));
}

_Win32_UI::_Win32_UI()
{
	m_screenPrimary = new _Win32_Screen();
}

COLORREF UIPlatform::getColorRef(const Color& color)
{
	return (COLORREF)(color.getBGR());
}
SLIB_UI_NAMESPACE_END

#endif
