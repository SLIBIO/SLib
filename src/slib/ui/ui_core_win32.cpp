#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "ui_core_win32.h"
#include "view_win32.h"

#include "../../../inc/slib/ui/screen.h"
#include "../../../inc/slib/ui/app.h"
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

sl_bool UI::isUiThread()
{
	return (_g_thread_ui == ::GetCurrentThreadId());
}

void UI::dispatchToUiThread(const Ref<Runnable>& callback)
{
	if (callback.isNull()) {
		return;
	}
	_Win32_UI* ui = _Win32_UI::get();
	if (!ui) {
		return;
	}
	Win32_UI_Shared* shared = Win32_UI_Shared::get();
	if (!shared) {
		return;
	}
	ui->m_queueDispatch.push(callback);
	::PostMessageW(shared->hWndMessage, SLIB_UI_MESSAGE_DISPATCH, 0, 0);
}

void _Win32_processUiDispatchQueue()
{
	_Win32_UI* ui = _Win32_UI::get();
	if (!ui) {
		return;
	}
	Ref<Runnable> callback;
	while (ui->m_queueDispatch.pop(&callback)) {
		if (callback.isNotNull()) {
			callback->run();
		}
	}
}

void _Win32_processMenuCommand(WPARAM wParam, LPARAM lParam);
void _Win32_processCustomMsgBox(WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK _Win32_MessageProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == SLIB_UI_MESSAGE_DISPATCH) {
		_Win32_processUiDispatchQueue();
	} else if (uMsg == SLIB_UI_MESSAGE_CUSTOM_MSGBOX) {
		_Win32_processCustomMsgBox(wParam, lParam);
		return 0;
	} else if (uMsg == WM_MENUCOMMAND) {
		_Win32_processMenuCommand(wParam, lParam);
		return 0;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void UIPlatform::runLoop(sl_uint32 level)
{
	_Win32_UI* ui = _Win32_UI::get();

	MSG msg;
	while (::GetMessageW(&msg, NULL, 0, 0)) {
		if (msg.message == WM_QUIT) {
			break;
		}
		if (msg.message == SLIB_UI_MESSAGE_DISPATCH) {
			_Win32_processUiDispatchQueue();
		} else if (msg.message == SLIB_UI_MESSAGE_CLOSE) {
			::DestroyWindow(msg.hwnd);
		} else if (msg.message == WM_MENUCOMMAND) {
			_Win32_processMenuCommand(msg.wParam, msg.lParam);
		} else {
			HWND hWnd = ::GetActiveWindow();
			sl_bool flagDefaultProcess = sl_true;
			if (hWnd) {
				if (::GetWindowLongW(hWnd, GWL_STYLE) & WS_POPUP) {
					if (::IsDialogMessageW(hWnd, &msg)) {
						flagDefaultProcess = sl_false;
					}
				}
			}
			Ref<Win32_ViewInstance> instance = Ref<Win32_ViewInstance>::from(UIPlatform::getViewInstance(msg.hwnd));
			if (instance.isNotNull()) {
				if (instance->preprocessWindowMessage(msg)) {
					flagDefaultProcess = sl_false;
				}
			}
			if (flagDefaultProcess) {
				::TranslateMessage(&msg);
				::DispatchMessageW(&msg);
			}
		}
	}

	ui->m_queueDispatch.removeAll();

}

void UIPlatform::quitLoop()
{
	::PostQuitMessage(0);
}

void UIPlatform::runApp()
{
	Win32_UI_Shared::get();

	_g_thread_ui = ::GetCurrentThreadId();

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	::OleInitialize(NULL);

	INITCOMMONCONTROLSEX icex;
	icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_TAB_CLASSES;
	InitCommonControlsEx(&icex);

	UIApp::dispatchStartToApp();

	runLoop(0);

	UIApp::dispatchExitToApp();

	//Gdiplus::GdiplusShutdown(gdiplusToken);
}

void UIPlatform::quitApp()
{
	::PostQuitMessage(0);
}

_Win32_UI::_Win32_UI()
{
	m_screenPrimary = new _Win32_Screen();
}

COLORREF UIPlatform::getColorRef(const Color& color)
{
	return (COLORREF)(color.getBGR());
}

LRESULT CALLBACK _Win32_ViewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK _Win32_WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

Win32_UI_Shared::Win32_UI_Shared()
{
	hInstance = ::GetModuleHandleW(NULL);
	
	// register view class
	{
		WNDCLASSEXW wc;
		::ZeroMemory(&wc, sizeof(wc));
		wc.cbSize = sizeof(wc);
		wc.style = CS_DBLCLKS | CS_PARENTDC;
		wc.lpfnWndProc = _Win32_ViewProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = NULL;
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = L"SLIBUIVIEW";
		wc.hIconSm = NULL;
		wndClassForView = ::RegisterClassExW(&wc);
	}

	// register window class
	{
		WNDCLASSEXW wc;
		::ZeroMemory(&wc, sizeof(wc));
		wc.cbSize = sizeof(wc);
		wc.style = CS_DBLCLKS;
		wc.lpfnWndProc = _Win32_WindowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInstance;
		wc.hIcon = ::LoadIcon(hInstance, IDI_APPLICATION);
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = L"SLIBUIWINDOW";
		wc.hIconSm = NULL;
		wndClassForWindow = ::RegisterClassExW(&wc);
	}

	// Mesage Window
	{
		WNDCLASSW wc;
		::ZeroMemory(&wc, sizeof(wc));
		wc.hInstance = hInstance;
		wc.lpfnWndProc = _Win32_MessageProc;
		wc.lpszClassName = L"SLIBMESSAGEHANDLER";
		m_wndClassForMessage = ::RegisterClassW(&wc);
		hWndMessage = ::CreateWindowExW(0, (LPCWSTR)((LONG_PTR)m_wndClassForMessage), L"", 0, 0, 0, 0, 0, HWND_MESSAGE, 0, hInstance, 0);
	}

}

Win32_UI_Shared::~Win32_UI_Shared()
{
	if (hWndMessage) {
		::DestroyWindow(hWndMessage);
	}
}

Win32_UI_Shared* Win32_UI_Shared::get()
{
	SLIB_SAFE_STATIC(Win32_UI_Shared, ret);
	return &ret;
}

SLIB_UI_NAMESPACE_END

#endif
