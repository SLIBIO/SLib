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

#if defined(SLIB_UI_IS_WIN32)

#include "ui_core_win32.h"
#include "ui_core_common.h"
#include "view_win32.h"

#include "slib/ui/screen.h"
#include "slib/ui/app.h"
#include "slib/ui/window.h"
#include "slib/core/queue.h"
#include "slib/core/dispatch.h"
#include "slib/core/safe_static.h"

#include <commctrl.h>
#include <shobjidl.h>

#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif

#pragma comment(lib, "gdiplus.lib")
#pragma comment(lib, "comctl32.lib")

namespace slib
{

	namespace priv
	{

		namespace menu
		{
			void ProcessMenuCommand(WPARAM wParam, LPARAM lParam);
			sl_bool ProcessMenuShortcutKey(MSG& msg);
		}

		namespace alert_dialog
		{
			void ProcessCustomMsgBox(WPARAM wParam, LPARAM lParam);
		}

		namespace view
		{
			sl_bool CaptureChildInstanceEvents(View* view, MSG& msg);
		}

		namespace window
		{
			LRESULT CALLBACK WindowInstanceProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		}

		namespace ui_core
		{

			DWORD g_threadMain = 0;

			class MainContext
			{
			public:
				Ref<Screen> m_screenPrimary;

				MainContext();

			};

			SLIB_SAFE_STATIC_GETTER(MainContext, GetMainContext)

			class ScreenImpl : public Screen
			{
			public:
				ScreenImpl()
				{
				}

				UIRect getRegion()
				{
					UIRect ret;
					ret.left = 0;
					ret.top = 0;
					ret.right = (sl_ui_pos)(GetSystemMetrics(SM_CXSCREEN));
					ret.bottom = (sl_ui_pos)(GetSystemMetrics(SM_CYSCREEN));
					return ret;
				}
			};

			static LRESULT CALLBACK MessageWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
			{
				if (uMsg == SLIB_UI_MESSAGE_DISPATCH) {
					UIDispatcher::processCallbacks();
				} else if (uMsg == SLIB_UI_MESSAGE_DISPATCH_DELAYED) {
					UIDispatcher::processDelayedCallback((sl_reg)lParam);
				} else if (uMsg == SLIB_UI_MESSAGE_CUSTOM_MSGBOX) {
					priv::alert_dialog::ProcessCustomMsgBox(wParam, lParam);
					return 0;
				} else if (uMsg == WM_MENUCOMMAND) {
					priv::menu::ProcessMenuCommand(wParam, lParam);
					return 0;
				}
				return DefWindowProc(hWnd, uMsg, wParam, lParam);
			}

			static void PostGlobalMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
			{
				Win32_UI_Shared* shared = Win32_UI_Shared::get();
				if (!shared) {
					return;
				}
				PostMessageW(shared->hWndMessage, uMsg, wParam, lParam);
			}

			UINT g_messageTaskbarButtonCreated = 0;

			static void InitTaskbarButtonList()
			{
				if (!g_messageTaskbarButtonCreated) {
					g_messageTaskbarButtonCreated = RegisterWindowMessageW(L"TaskbarButtonCreated");
				}
			}

			sl_uint32 g_nBadgeNumber = 0;
			
			void ApplyBadgeNumber()
			{
				SLIB_SAFE_STATIC(Ref<Font>, font1, Font::create("Courier", 24, sl_true));
				if (SLIB_SAFE_STATIC_CHECK_FREED(font1)) {
					return;
				}
				SLIB_SAFE_STATIC(Ref<Font>, font2, Font::create("Courier", 20, sl_true));
				if (SLIB_SAFE_STATIC_CHECK_FREED(font2)) {
					return;
				}
				Ref<UIApp> app = UIApp::getApp();
				if (app.isNull()) {
					return;
				}
				Ref<Window> window = app->getMainWindow();
				HWND hWnd = UIPlatform::getWindowHandle(window.get());
				if (!hWnd) {
					return;
				}
				ITaskbarList3* pList = NULL;
				CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pList));
				if (pList) {
					HRESULT hr = pList->HrInit();
					if (SUCCEEDED(hr)) {
						HICON hIcon = NULL;
						sl_uint32 n = g_nBadgeNumber;
						if (n > 0) {
							if (n >= 100) {
								n = 99;
							}
							Ref<Bitmap> bitmap = Bitmap::create(32, 32);
							if (bitmap.isNotNull()) {
								bitmap->resetPixels(Color::zero());
								Ref<Canvas> canvas = bitmap->getCanvas();
								if (canvas.isNotNull()) {
									canvas->setAntiAlias(sl_true);
									canvas->fillEllipse(0, 0, 32, 32, Color::Red);
									canvas->setAntiAlias(sl_false);
									canvas->drawText(String::fromUint32(n), Rectangle(0, 0, 32, 30), n < 10 ? font1 : font2, Color::White, Alignment::MiddleCenter);
									canvas.setNull();
									hIcon = GraphicsPlatform::createIconFromBitmap(bitmap);
								}
							}
						}
						pList->SetOverlayIcon(hWnd, hIcon, L"Status");
						if (hIcon) {
							DestroyIcon(hIcon);
						}
					}
					pList->Release();
				}
			}

			MainContext::MainContext()
			{
				m_screenPrimary = new ScreenImpl();
			}

		}

	}

	using namespace priv::ui_core;

	List< Ref<Screen> > UI::getScreens()
	{
		MainContext* ui = GetMainContext();
		if (!ui) {
			return sl_null;
		}
		List< Ref<Screen> > ret;
		ret.add_NoLock(ui->m_screenPrimary);
		return ret;
	}

	Ref<Screen> UI::getPrimaryScreen()
	{
		MainContext* ui = GetMainContext();
		if (!ui) {
			return sl_null;
		}
		return ui->m_screenPrimary;
	}

	Ref<Screen> UI::getFocusedScreen()
	{
		MainContext* ui = GetMainContext();
		if (!ui) {
			return sl_null;
		}
		return ui->m_screenPrimary;
	}

	sl_bool UI::isUiThread()
	{
		return (g_threadMain == GetCurrentThreadId());
	}

	void UI::dispatchToUiThread(const Function<void()>& callback, sl_uint32 delayMillis)
	{
		if (callback.isNull()) {
			return;
		}
		if (delayMillis == 0) {
			Win32_UI_Shared* shared = Win32_UI_Shared::get();
			if (!shared) {
				return;
			}
			if (UIDispatcher::addCallback(callback)) {
				PostMessageW(shared->hWndMessage, SLIB_UI_MESSAGE_DISPATCH, 0, 0);
			}
		} else {
			sl_reg callbackId;
			if (UIDispatcher::addDelayedCallback(callback, callbackId)) {
				Dispatch::setTimeout(Function<void()>::bind(&PostGlobalMessage, SLIB_UI_MESSAGE_DISPATCH_DELAYED, 0, (LPARAM)callbackId), delayMillis);
			}
		}
	}

	void UI::openUrl(const String& url)
	{
		Windows::ShellExecuteParam param;
		param.operation = "open";
		param.path = url;
		Windows::shellExecute(param);
	}

	void UI::setBadgeNumber(sl_uint32 num)
	{
		g_nBadgeNumber = num;
		InitTaskbarButtonList();
		ApplyBadgeNumber();
	}

	void UIPlatform::runLoop(sl_uint32 level)
	{
		ReleaseCapture();
		MSG msg;
		while (GetMessageW(&msg, NULL, 0, 0)) {
			if (msg.message == WM_QUIT) {
				break;
			}
			if (msg.message == SLIB_UI_MESSAGE_DISPATCH) {
				UIDispatcher::processCallbacks();
			} else if (msg.message == SLIB_UI_MESSAGE_DISPATCH_DELAYED) {
				UIDispatcher::processDelayedCallback((sl_reg)(msg.lParam));
			} else if (msg.message == SLIB_UI_MESSAGE_CLOSE) {
				DestroyWindow(msg.hwnd);
			} else if (msg.message == WM_MENUCOMMAND) {
				priv::menu::ProcessMenuCommand(msg.wParam, msg.lParam);
			} else if (g_messageTaskbarButtonCreated && msg.message == g_messageTaskbarButtonCreated) {
				ApplyBadgeNumber();
			} else {
				do {
					if (priv::menu::ProcessMenuShortcutKey(msg)) {
						break;
					}
					Ref<Win32_ViewInstance> instance = Ref<Win32_ViewInstance>::from(UIPlatform::getViewInstance(msg.hwnd));
					if (instance.isNotNull()) {
						Ref<View> view = instance->getView();
						if (view.isNotNull()) {
							if (priv::view::CaptureChildInstanceEvents(view.get(), msg)) {
								break;
							}
							if (instance->preprocessWindowMessage(msg)) {
								break;
							}
						}
					}
					HWND hWnd = GetActiveWindow();
					if (hWnd) {
						if (GetWindowLongW(hWnd, GWL_STYLE) & WS_POPUP) {
							if (IsDialogMessageW(hWnd, &msg)) {
								break;
							}
						}
					}
					TranslateMessage(&msg);
					DispatchMessageW(&msg);
				} while (0);
			}
		}

		UIDispatcher::removeAllCallbacks();

	}

	void UIPlatform::quitLoop()
	{
		PostQuitMessage(0);
	}

	void UIPlatform::runApp()
	{
		Win32_UI_Shared::get();

		g_threadMain = GetCurrentThreadId();

		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		OleInitialize(NULL);

		INITCOMMONCONTROLSEX icex;
		icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_TAB_CLASSES | ICC_DATE_CLASSES;
		InitCommonControlsEx(&icex);

		UIApp::dispatchStartToApp();

		runLoop(0);

		UIApp::dispatchExitToApp();

		//Gdiplus::GdiplusShutdown(gdiplusToken);
	}

	void UIPlatform::quitApp()
	{
		PostQuitMessage(0);
	}

	Win32_UI_Shared::Win32_UI_Shared()
	{
		hInstance = GetModuleHandleW(NULL);
		
		// register view class
		{
			WNDCLASSEXW wc;
			ZeroMemory(&wc, sizeof(wc));
			wc.cbSize = sizeof(wc);
			wc.style = CS_DBLCLKS | CS_PARENTDC;
			wc.lpfnWndProc = priv::view::ViewInstanceProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = hInstance;
			wc.hIcon = NULL;
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)(COLOR_MENU + 1);
			wc.lpszMenuName = NULL;
			wc.lpszClassName = L"SLIBUIVIEW";
			wc.hIconSm = NULL;
			wndClassForView = RegisterClassExW(&wc);
		}

		// register window class
		{
			WNDCLASSEXW wc;
			ZeroMemory(&wc, sizeof(wc));
			wc.cbSize = sizeof(wc);
			wc.style = CS_DBLCLKS;
			wc.lpfnWndProc = priv::window::WindowInstanceProc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hInstance = hInstance;
			wc.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hbrBackground = (HBRUSH)(COLOR_MENU + 1);
			wc.lpszMenuName = NULL;
			wc.lpszClassName = L"SLIBUIWINDOW";
			wc.hIconSm = NULL;
			wndClassForWindow = RegisterClassExW(&wc);
		}

		// Mesage Window
		{
			WNDCLASSW wc;
			ZeroMemory(&wc, sizeof(wc));
			wc.hInstance = hInstance;
			wc.lpfnWndProc = MessageWindowProc;
			wc.lpszClassName = L"SLIBMESSAGEHANDLER";
			m_wndClassForMessage = RegisterClassW(&wc);
			hWndMessage = CreateWindowExW(0, (LPCWSTR)((LONG_PTR)m_wndClassForMessage), L"", 0, 0, 0, 0, 0, HWND_MESSAGE, 0, hInstance, 0);
		}

	}

	Win32_UI_Shared::~Win32_UI_Shared()
	{
		if (hWndMessage) {
			DestroyWindow(hWndMessage);
		}
	}

	Win32_UI_Shared* Win32_UI_Shared::get()
	{
		SLIB_SAFE_STATIC(Win32_UI_Shared, ret);
		if (SLIB_SAFE_STATIC_CHECK_FREED(ret)) {
			return sl_null;
		}
		return &ret;
	}

}

#endif
