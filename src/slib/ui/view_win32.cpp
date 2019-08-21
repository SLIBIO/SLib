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

#include "view_win32.h"

#include "slib/ui/core.h"
#include "slib/ui/window.h"
#include "slib/ui/select_view.h"
#include "slib/core/safe_static.h"
#include "slib/math/transform2d.h"

#include <commctrl.h>

namespace slib
{

	namespace priv
	{
		namespace view
		{

			Color GetDefaultBackColor()
			{
				return GraphicsPlatform::getColorFromColorRef(GetSysColor(COLOR_MENU));
			}

			LRESULT CALLBACK DefaultViewInstanceProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
			{
				switch (uMsg) {
					case WM_COMMAND:
						{
							HWND hWndSender = (HWND)lParam;
							if (hWndSender) {
								Ref<Win32_ViewInstance> instance = Ref<Win32_ViewInstance>::from(UIPlatform::getViewInstance(hWndSender));
								if (instance.isNotNull()) {
									SHORT code = HIWORD(wParam);
									LRESULT result = 0;
									if (instance->processCommand(code, result)) {
										return result;
									}
								}
							}
						}
						break;
					case WM_NOTIFY:
						{
							NMHDR* nh = (NMHDR*)(lParam);
							HWND hWndSender = (HWND)(nh->hwndFrom);
							Ref<Win32_ViewInstance> instance = Ref<Win32_ViewInstance>::from(UIPlatform::getViewInstance(hWndSender));
							if (instance.isNotNull()) {
								LRESULT result = 0;
								if (instance->processNotify(nh, result)) {
									return result;
								}
							}
						}
						break;
					case WM_CTLCOLOREDIT:
					case WM_CTLCOLORSTATIC:
					case WM_CTLCOLORLISTBOX:
					case WM_CTLCOLORBTN:
					case WM_CTLCOLORSCROLLBAR:
						{
							HWND hWndSender = (HWND)lParam;
							Ref<Win32_ViewInstance> instance = Ref<Win32_ViewInstance>::from(UIPlatform::getViewInstance(hWndSender));
							if (instance.isNotNull()) {
								HDC hDC = (HDC)(wParam);
								HBRUSH result = NULL;
								if (!(instance->processControlColor(uMsg, hDC, result))) {
									result = (HBRUSH)(DefWindowProcW(hWnd, uMsg, wParam, lParam));
								}
								instance->processPostControlColor(uMsg, hDC, result);
								return (LRESULT)result;
							}
						}
						break;
				}
				return DefWindowProcW(hWnd, uMsg, wParam, lParam);
			}

			LRESULT CALLBACK ViewInstanceProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
			{
				Ref<Win32_ViewInstance> instance = Ref<Win32_ViewInstance>::from(UIPlatform::getViewInstance(hWnd));
				if (instance.isNotNull()) {
					return instance->processWindowMessage(uMsg, wParam, lParam);
				}
				return DefaultViewInstanceProc(hWnd, uMsg, wParam, lParam);
			}

			LRESULT CALLBACK ViewInstanceSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData)
			{
				Ref<Win32_ViewInstance> instance = Ref<Win32_ViewInstance>::from(UIPlatform::getViewInstance(hWnd));
				if (instance.isNotNull()) {
					return instance->processSubclassMessage(uMsg, wParam, lParam);
				}
				return DefSubclassProc(hWnd, uMsg, wParam, lParam);
			}

			sl_bool CaptureChildInstanceEvents(View* view, UINT uMsg)
			{
				Ref<View> parent = view->getParent();
				while (parent.isNotNull()) {
					Function<sl_bool(const UIPoint&)> hitTestCapture(parent->getCapturingChildInstanceEvents());
					if (hitTestCapture.isNotNull()) {
						if (CaptureChildInstanceEvents(parent.get(), uMsg)) {
							return sl_true;
						}
						Ref<ViewInstance> _instance = parent->getViewInstance();
						if (_instance.isNotNull()) {
							Win32_ViewInstance* instance = (Win32_ViewInstance*)(_instance.get());
							HWND hWnd = instance->getHandle();
							if (hWnd) {
								DWORD lParam = GetMessagePos();
								POINT pt;
								pt.x = (short)(lParam & 0xffff);
								pt.y = (short)((lParam >> 16) & 0xffff);
								ScreenToClient(hWnd, &pt);
								if (hitTestCapture(UIPoint((sl_ui_pos)(pt.x), (sl_ui_pos)(pt.y)))) {
									LPARAM lParam = POINTTOPOINTS(pt);
									instance->processWindowMessage(uMsg, 0, lParam);
									return sl_true;
								}
							}
						}
					}
					parent = parent->getParent();
				}
				return sl_false;
			}

			sl_bool CaptureChildInstanceEvents(View* view, MSG& msg)
			{
				UINT uMsg = msg.message;
				switch (uMsg) {
				case WM_LBUTTONDOWN:
				case WM_LBUTTONDBLCLK:
				case WM_RBUTTONDOWN:
				case WM_RBUTTONDBLCLK:
				case WM_MBUTTONDOWN:
				case WM_MBUTTONDBLCLK:
				case WM_MOUSEMOVE:
					break;
				case WM_NCLBUTTONDOWN:
					uMsg = WM_LBUTTONDOWN;
					break;
				case WM_NCRBUTTONDOWN:
					uMsg = WM_RBUTTONDOWN;
					break;
				case WM_NCMBUTTONDOWN:
					uMsg = WM_MBUTTONDOWN;
					break;
				case WM_NCMOUSEMOVE:
					uMsg = WM_MOUSEMOVE;
					break;
				default:
					return sl_false;
				}
				return CaptureChildInstanceEvents(view, uMsg);
			}

			sl_bool g_flagDuringPaint = sl_false;

		}
	}

	using namespace priv::view;

	Win32_ViewInstance::Win32_ViewInstance()
	{
		m_handle = NULL;
		m_flagGenericView = sl_false;
		m_flagDestroyOnRelease = sl_false;
		m_actionMouseCapture = UIAction::MouseMove;
	}

	Win32_ViewInstance::~Win32_ViewInstance()
	{
		if (m_handle) {
			UIPlatform::removeViewInstance(m_handle);
			if (m_flagDestroyOnRelease) {
				PostMessageW(m_handle, SLIB_UI_MESSAGE_CLOSE, 0, 0);
			}
		}
	}

	HWND Win32_ViewInstance::createHandle(
		View* view, ViewInstance* parent,
		LPCWSTR wndClass, LPCWSTR text, int style, int styleEx,
		const UIRect& frame, const Matrix3& transform)
	{

		HWND hWndParent = UIPlatform::getViewHandle(parent);

		if (hWndParent) {

			HINSTANCE hInst = GetModuleHandleW(NULL);
			style |= WS_CHILD;
			if (view->isVisible()) {
				style |= WS_VISIBLE;
			}

			sl_ui_pos x = frame.left;
			sl_ui_pos y = frame.top;
			Vector2 t = Transform2::getTranslationFromMatrix(transform);
			x += (sl_ui_pos)(t.x);
			y += (sl_ui_pos)(t.y);

			HWND hWnd = CreateWindowExW(
				styleEx // ex-style
				, wndClass
				, text
				, style
				, (int)(x), (int)(y)
				, (int)(frame.getWidth()), (int)(frame.getHeight())
				, hWndParent // parent
				, NULL // menu
				, hInst
				, NULL);
			if (hWnd) {
				if (!(view->isEnabled())) {
					EnableWindow(hWnd, FALSE);
				}
				Win32_UI_Shared* shared = Win32_UI_Shared::get();
				if (shared) {
					if (wndClass != (LPCWSTR)(shared->wndClassForView)) {
						SetWindowSubclass(hWnd, ViewInstanceSubclassProc, 0, 0);
					}
				}
				return hWnd;
			}
		}
		return NULL;
	}

	void Win32_ViewInstance::initialize(View* view, const UIRect& frame, const Matrix3& transform)
	{
		m_frame = frame;
		m_translation = Transform2::getTranslationFromMatrix(transform);
	}

	HWND Win32_ViewInstance::getHandle()
	{
		return m_handle;
	}

	sl_bool Win32_ViewInstance::isValid()
	{
		HWND hWnd = m_handle;
		if (hWnd) {
			if (IsWindow(hWnd)) {
				return sl_true;
			}
		}
		return sl_false;
	}

	void Win32_ViewInstance::setFocus(sl_bool flag)
	{
		HWND hWnd = m_handle;
		if (hWnd) {
			if (flag) {
				SetFocus(hWnd);
			} else {
				if (GetFocus() == hWnd) {
					SetFocus(NULL);
				}
			}
		}
	}

	void Win32_ViewInstance::invalidate()
	{
		if (!(UI::isUiThread()) || g_flagDuringPaint) {
			void (ViewInstance::*func)() = &ViewInstance::invalidate;
			UI::dispatchToUiThread(Function<void()>::fromWeakRef(WeakRef<ViewInstance>(this), func));
			return;
		}
		HWND hWnd = m_handle;
		if (hWnd) {
			InvalidateRect(hWnd, NULL, TRUE);
		}
	}

	void Win32_ViewInstance::invalidate(const UIRect& rect)
	{
		if (!(UI::isUiThread()) || g_flagDuringPaint) {
			void (ViewInstance::*func)(const UIRect&) = &ViewInstance::invalidate;
			UI::dispatchToUiThread(Function<void()>::bindWeakRef(WeakRef<ViewInstance>(this), func, rect));
			return;
		}
		HWND hWnd = m_handle;
		if (hWnd) {
			RECT rc;
			rc.left = (int)(rect.left);
			rc.top = (int)(rect.top);
			rc.right = (int)(rect.right);
			rc.bottom = (int)(rect.bottom);
			InvalidateRect(hWnd, &rc, TRUE);
		}
	}

	UIRect Win32_ViewInstance::getFrame()
	{
		HWND hWnd = m_handle;
		if (hWnd) {
			RECT rc;
			Windows::getWindowFrame(hWnd, rc);
			UIRect ret;
			ret.left = (sl_ui_pos)(rc.left);
			ret.top = (sl_ui_pos)(rc.top);
			ret.right = (sl_ui_pos)(rc.right);
			ret.bottom = (sl_ui_pos)(rc.bottom);
			return ret;
		}
		return UIRect::zero();
	}

	void Win32_ViewInstance::setFrame(const UIRect& frame)
	{
		if (isWindowContent()) {
			return;
		}
		m_frame = frame;
		HWND hWnd = m_handle;
		if (hWnd) {
			UINT uFlags = SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOACTIVATE
				| SWP_NOCOPYBITS
				| SWP_ASYNCWINDOWPOS
				;
			SetWindowPos(hWnd, NULL
				, (int)(frame.left + m_translation.x), (int)(frame.top + m_translation.y)
				, (int)(frame.getWidth()), (int)(frame.getHeight())
				, uFlags
			);
		}
	}

	void Win32_ViewInstance::setTransform(const Matrix3 &transform)
	{
		if (isWindowContent()) {
			return;
		}
		m_translation = Transform2::getTranslationFromMatrix(transform);
		HWND hWnd = m_handle;
		if (hWnd) {
			UINT uFlags = SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOACTIVATE
				| SWP_NOCOPYBITS
				| SWP_ASYNCWINDOWPOS
				;
			SetWindowPos(hWnd, NULL
				, (int)(m_frame.left + m_translation.x), (int)(m_frame.top + m_translation.y)
				, (int)(m_frame.getWidth()), (int)(m_frame.getHeight())
				, uFlags
			);
		}
	}

	void Win32_ViewInstance::setVisible(sl_bool flag)
	{
		HWND hWnd = m_handle;
		if (hWnd) {
			sl_bool f1 = IsWindowVisible(hWnd) ? sl_true : sl_false;
			sl_bool f2 = flag ? sl_true : sl_false;
			if (f1 != f2) {
				if (f2) {
					//::ShowWindow(hWnd, SW_SHOW);
					ShowWindowAsync(hWnd, SW_SHOW);
				} else {
					//::ShowWindow(hWnd, SW_HIDE);
					ShowWindowAsync(hWnd, SW_HIDE);
				}
			}
		}
	}

	void Win32_ViewInstance::setEnabled(sl_bool flag)
	{
		HWND hWnd = m_handle;
		if (hWnd) {
			sl_bool f1 = IsWindowEnabled(hWnd) ? sl_true : sl_false;
			sl_bool f2 = flag ? sl_true : sl_false;
			if (f1 != f2) {
				EnableWindow(hWnd, f2);
			}
		}
	}

	void Win32_ViewInstance::setOpaque(sl_bool flag)
	{
	}

	void Win32_ViewInstance::setAlpha(sl_real alpha)
	{
	}

	void Win32_ViewInstance::setClipping(sl_bool flag)
	{
	}

	void Win32_ViewInstance::setDrawing(sl_bool flag)
	{
	}

	UIPointf Win32_ViewInstance::convertCoordinateFromScreenToView(const UIPointf& ptScreen)
	{
		HWND hWnd = m_handle;
		if (hWnd) {
			POINT pt;
			pt.x = (LONG)(ptScreen.x);
			pt.y = (LONG)(ptScreen.y);
			ScreenToClient(hWnd, &pt);
			return UIPointf((sl_ui_posf)(pt.x), (sl_ui_posf)(pt.y));
		}
		return ptScreen;
	}

	UIPointf Win32_ViewInstance::convertCoordinateFromViewToScreen(const UIPointf& ptView)
	{
		HWND hWnd = m_handle;
		if (hWnd) {
			POINT pt;
			pt.x = (LONG)(ptView.x);
			pt.y = (LONG)(ptView.y);
			ClientToScreen(hWnd, &pt);
			return UIPointf((sl_ui_posf)(pt.x), (sl_ui_posf)(pt.y));
		}
		return ptView;
	}

	void Win32_ViewInstance::addChildInstance(const Ref<ViewInstance>& _child)
	{
		HWND hWnd = m_handle;
		if (hWnd) {
			Win32_ViewInstance* child = (Win32_ViewInstance*)(_child.get());
			if (child) {
				HWND hWndChild = child->getHandle();
				if (hWndChild) {
					SetParent(hWndChild, hWnd);
				}
			}
		}
	}

	void Win32_ViewInstance::removeChildInstance(const Ref<ViewInstance>& _child)
	{
		Win32_ViewInstance* child = (Win32_ViewInstance*)(_child.get());
		HWND hWnd = child->getHandle();
		if (hWnd) {
			SetParent(hWnd, HWND_MESSAGE);
		}
	}

	void Win32_ViewInstance::bringToFront()
	{
		HWND hWnd = m_handle;
		if (hWnd) {
			BringWindowToTop(hWnd);
			Ref<View> view = getView();
			if (view.isNotNull()) {
				view->invalidateBoundsInParent();
			}
		}
	}

	sl_bool Win32_ViewInstance::onEventKey(sl_bool flagDown, WPARAM wParam, LPARAM lParam)
	{
		HWND hWnd = m_handle;
		if (hWnd) {
			UIAction action = flagDown ? UIAction::KeyDown : UIAction::KeyUp;
			sl_uint32 vkey = (sl_uint32)wParam;
			UINT scancode = (lParam & 0x00ff0000) >> 16;
			int extended = (lParam & 0x01000000) != 0;
			switch (vkey) {
			case VK_SHIFT:
				vkey = MapVirtualKeyW(scancode, MAPVK_VSC_TO_VK_EX);
				break;
			case VK_CONTROL:
				vkey = extended ? VK_RCONTROL : VK_LCONTROL;
				break;
			case VK_MENU:
				vkey = extended ? VK_RMENU : VK_LMENU;
				break;
			}
			Keycode key = UIEvent::getKeycodeFromSystemKeycode(vkey);
			Time t;
			t.setMillisecondsCount(GetMessageTime());
			Ref<UIEvent> ev = UIEvent::createKeyEvent(action, key, vkey, t);
			if (ev.isNotNull()) {
				UIPlatform::applyEventModifiers(ev.get());
				ev->addFlag(UIEventFlags::DispatchToParent);
				onKeyEvent(ev.get());
				if (ev->isPreventedDefault()) {
					return sl_true;
				}
			}
		}
		return sl_false;
	}

	sl_bool Win32_ViewInstance::onEventMouse(UIAction action, WPARAM wParam, LPARAM lParam)
	{
		HWND hWnd = m_handle;
		if (hWnd) {
			int _x = (short)(lParam & 0xffff);
			int _y = (short)((lParam >> 16) & 0xffff);

			sl_ui_posf x = (sl_ui_posf)(_x);
			sl_ui_posf y = (sl_ui_posf)(_y);

			Time t;
			t.setMillisecondsCount(GetMessageTime());
			Ref<UIEvent> ev = UIEvent::createMouseEvent(action, x, y, t);
			if (ev.isNotNull()) {
				UIPlatform::applyEventModifiers(ev.get());
				ev->addFlag(UIEventFlags::DispatchToParent);
				onMouseEvent(ev.get());
				if (ev->isPreventedDefault()) {
					return sl_true;
				}
			}
		}
		return sl_false;
	}

	sl_bool Win32_ViewInstance::onEventMouseWheel(sl_bool flagVertical, WPARAM wParam, LPARAM lParam)
	{
		HWND hWnd = m_handle;
		if (hWnd) {
			int delta = (short)((wParam >> 16) & 0xffff);
			sl_real deltaX, deltaY;
			if (flagVertical) {
				deltaX = 0;
				deltaY = (sl_real)delta;
			} else {
				deltaX = (sl_real)delta;
				deltaY = 0;
			}
			const DWORD lParam = GetMessagePos();
			POINT pt;
			pt.x = (short)(lParam & 0xffff);
			pt.y = (short)((lParam >> 16) & 0xffff);
			ScreenToClient(hWnd, &pt);
			Time t;
			t.setMillisecondsCount(GetMessageTime());
			Ref<UIEvent> ev = UIEvent::createMouseWheelEvent((sl_ui_posf)(pt.x), (sl_ui_posf)(pt.y), deltaX, deltaY, t);
			if (ev.isNotNull()) {
				UIPlatform::applyEventModifiers(ev.get());
				onMouseWheelEvent(ev.get());
				if (ev->isPreventedDefault()) {
					return sl_true;
				}
			}
		}
		return sl_false;
	}

	sl_bool Win32_ViewInstance::onEventSetCursor()
	{
		HWND hWnd = m_handle;
		if (hWnd) {
			const DWORD lParam = GetMessagePos();
			POINT pt;
			pt.x = (short)(lParam & 0xffff);
			pt.y = (short)((lParam >> 16) & 0xffff);
			ScreenToClient(hWnd, &pt);
			Time t;
			t.setMillisecondsCount(GetMessageTime());
			Ref<UIEvent> ev = UIEvent::createSetCursorEvent((sl_ui_posf)(pt.x), (sl_ui_posf)(pt.y), t);
			if (ev.isNotNull()) {
				onSetCursor(ev.get());
				if (ev->isPreventedDefault()) {
					return sl_true;
				}
			}
		}
		return sl_false;
	}

	void Win32_ViewInstance::setGenericView(sl_bool flag)
	{
		m_flagGenericView = flag;
	}

	LRESULT Win32_ViewInstance::processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		HWND hWnd = m_handle;
		if (!hWnd) {
			return 0;
		}
		switch (msg) {
			case WM_ERASEBKGND:
				return TRUE;

			case WM_PAINT:
				{
					PAINTSTRUCT ps;
					HDC hDC = BeginPaint(hWnd, &ps);
					if (hDC) {
						if (ps.rcPaint.right > ps.rcPaint.left && ps.rcPaint.bottom > ps.rcPaint.top) {
							Gdiplus::Graphics graphics(hDC);
							RECT rect;
							GetClientRect(hWnd, &rect);
							Ref<Canvas> canvas = GraphicsPlatform::createCanvas(CanvasType::View, &graphics, rect.right, rect.bottom, sl_false);
							if (canvas.isNotNull()) {
								canvas->setInvalidatedRect(Rectangle((sl_real)(ps.rcPaint.left), (sl_real)(ps.rcPaint.top), (sl_real)(ps.rcPaint.right), (sl_real)(ps.rcPaint.bottom)));
								g_flagDuringPaint = sl_true;
								onDraw(canvas.get());
								g_flagDuringPaint = sl_false;
							}
						}
						EndPaint(hWnd, &ps);
					}
					return 0;
				}
			case WM_LBUTTONDOWN:
				{
					sl_bool flag = onEventMouse(UIAction::LeftButtonDown, wParam, lParam);
					m_actionMouseCapture = UIAction::LeftButtonDown;
					SetCapture(hWnd);
					if (flag) {
						return 0;
					}
					break;
				}
			case WM_LBUTTONDBLCLK:
				{
					onEventMouse(UIAction::LeftButtonDown, wParam, lParam);
					if (onEventMouse(UIAction::LeftButtonDoubleClick, wParam, lParam)) {
						return 0;
					}
					break;
				}
			case WM_LBUTTONUP:
				{
					ReleaseCapture();
					if (onEventMouse(UIAction::LeftButtonUp, wParam, lParam)) {
						return 0;
					}
					break;
				}
			case WM_RBUTTONDOWN:
				{
					sl_bool flag = onEventMouse(UIAction::RightButtonDown, wParam, lParam);
					m_actionMouseCapture = UIAction::RightButtonDown;
					SetCapture(hWnd);
					if (flag) {
						return 0;
					}
					break;
				}
			case WM_RBUTTONDBLCLK:
				{
					onEventMouse(UIAction::RightButtonDown, wParam, lParam);
					if (onEventMouse(UIAction::RightButtonDoubleClick, wParam, lParam)) {
						return 0;
					}
					break;
				}
			case WM_RBUTTONUP:
				{
					ReleaseCapture();
					if (onEventMouse(UIAction::RightButtonUp, wParam, lParam)) {
						return 0;
					}
					break;
				}
			case WM_MBUTTONDOWN:
				{
					sl_bool flag = onEventMouse(UIAction::MiddleButtonDown, wParam, lParam);
					m_actionMouseCapture = UIAction::MiddleButtonDown;
					SetCapture(hWnd);
					if (flag) {
						return 0;
					}
					break;
				}
			case WM_MBUTTONDBLCLK:
				{
					onEventMouse(UIAction::MiddleButtonDown, wParam, lParam);
					if (onEventMouse(UIAction::MiddleButtonDoubleClick, wParam, lParam)) {
						return 0;
					}
					break;
				}
			case WM_MBUTTONUP:
				{
					ReleaseCapture();
					if (onEventMouse(UIAction::MiddleButtonUp, wParam, lParam)) {
						return 0;
					}
					break;
				}
			case WM_MOUSEMOVE:
				{
					TRACKMOUSEEVENT track;
					track.cbSize = sizeof(track);
					track.dwFlags = TME_LEAVE | TME_QUERY;
					track.hwndTrack = hWnd;
					track.dwHoverTime = 0;
					TrackMouseEvent(&track);
					if (track.dwFlags == 0) {
						track.cbSize = sizeof(track);
						track.dwFlags = TME_LEAVE;
						track.hwndTrack = hWnd;
						track.dwHoverTime = HOVER_DEFAULT;
						TrackMouseEvent(&track);
						onEventMouse(UIAction::MouseEnter, wParam, lParam);
					}

					if (GetCapture() == hWnd) {
						if (m_actionMouseCapture == UIAction::LeftButtonDown) {
							if (onEventMouse(UIAction::LeftButtonDrag, wParam, lParam)) {
								return 0;
							}
						} else if (m_actionMouseCapture == UIAction::RightButtonDown) {
							if (onEventMouse(UIAction::RightButtonDrag, wParam, lParam)) {
								return 0;
							}
						} else if (m_actionMouseCapture == UIAction::MiddleButtonDown) {
							if (onEventMouse(UIAction::MiddleButtonDrag, wParam, lParam)) {
								return 0;
							}
						} else {
							if (onEventMouse(UIAction::MouseMove, wParam, lParam)) {
								return 0;
							}
						}
					} else {
						if (onEventMouse(UIAction::MouseMove, wParam, lParam)) {
							return 0;
						}
					}
					break;
				}
			case WM_MOUSELEAVE:
				{
					if (onEventMouse(UIAction::MouseLeave, wParam, lParam)) {
						return 0;
					}
					break;
				}
			case WM_MOUSEWHEEL:
				{
					if (onEventMouseWheel(sl_true, wParam, lParam)) {
						return 0;
					}
					break;
				}
			case 0x020E: // WM_MOUSEHWHEEL
				{
					if (onEventMouseWheel(sl_false, wParam, lParam)) {
						return 0;
					}
					break;
				}
			case WM_KEYDOWN:
			case WM_SYSKEYDOWN:
				{
					if (onEventKey(sl_true, wParam, lParam)) {
						return 0;
					}
					break;
				}
			case WM_KEYUP:
			case WM_SYSKEYUP:
				{
					if (onEventKey(sl_false, wParam, lParam)) {
						return 0;
					}
					break;
				}
			case WM_SETFOCUS:
				{
					onSetFocus();
					break;
				}
			case WM_SETCURSOR:
				{
					if (onEventSetCursor()) {
						return TRUE;
					}
					break;
				}
		}
		return DefaultViewInstanceProc(hWnd, msg, wParam, lParam);
	}

	LRESULT Win32_ViewInstance::processSubclassMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		HWND hWnd = m_handle;
		if (!hWnd) {
			return 0;
		}
		switch (msg) {		
		case WM_LBUTTONDOWN:
			if (onEventMouse(UIAction::LeftButtonDown, wParam, lParam)) {
				return 0;
			}
			break;
		case WM_LBUTTONDBLCLK:
			if (onEventMouse(UIAction::LeftButtonDoubleClick, wParam, lParam)) {
				return 0;
			}
			break;
		case WM_LBUTTONUP:
			if (onEventMouse(UIAction::LeftButtonUp, wParam, lParam)) {
				return 0;
			}
			break;
		case WM_RBUTTONDOWN:
			if (onEventMouse(UIAction::RightButtonDown, wParam, lParam)) {
				return 0;
			}
			break;
		case WM_RBUTTONDBLCLK:
			if (onEventMouse(UIAction::RightButtonDoubleClick, wParam, lParam)) {
				return 0;
			}
			break;
		case WM_RBUTTONUP:
			if (onEventMouse(UIAction::RightButtonUp, wParam, lParam)) {
				return 0;
			}
			break;
		case WM_MBUTTONDOWN:
			if (onEventMouse(UIAction::MiddleButtonDown, wParam, lParam)) {
				return 0;
			}
			break;
		case WM_MBUTTONDBLCLK:
			if (onEventMouse(UIAction::MiddleButtonDoubleClick, wParam, lParam)) {
				return 0;
			}
			break;
		case WM_MBUTTONUP:
			if (onEventMouse(UIAction::MiddleButtonUp, wParam, lParam)) {
				return 0;
			}
			break;
		case WM_MOUSEMOVE:
			if (onEventMouse(UIAction::MouseMove, wParam, lParam)) {
				return 0;
			}
			break;
		case WM_MOUSEWHEEL:
			if (onEventMouseWheel(sl_true, wParam, lParam)) {
				return 0;
			}
			break;
		case 0x020E: // WM_MOUSEHWHEEL
			if (onEventMouseWheel(sl_false, wParam, lParam)) {
				return 0;
			}
			break;
		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			if (onEventKey(sl_true, wParam, lParam)) {
				return 0;
			}
			break;
		case WM_KEYUP:
		case WM_SYSKEYUP:
			if (onEventKey(sl_false, wParam, lParam)) {
				return 0;
			}
			break;
		case WM_SETFOCUS:
			onSetFocus();
			break;
		case WM_SETCURSOR:
			if (onEventSetCursor()) {
				return TRUE;
			}
			break;
		}
		return DefSubclassProc(hWnd, msg, wParam, lParam);
	}

	sl_bool Win32_ViewInstance::processCommand(SHORT code, LRESULT& result)
	{
		return sl_false;
	}

	sl_bool Win32_ViewInstance::processNotify(NMHDR* nmhdr, LRESULT& result)
	{
		return sl_false;
	}

	sl_bool Win32_ViewInstance::processControlColor(UINT msg, HDC hDC, HBRUSH& result)
	{
		return sl_false;
	}

	void Win32_ViewInstance::processPostControlColor(UINT msg, HDC hDC, HBRUSH& result)
	{
	}

	Ref<ViewInstance> View::createGenericInstance(ViewInstance* parent)
	{
		Win32_UI_Shared* shared = Win32_UI_Shared::get();
		if (!shared) {
			return sl_null;
		}

		DWORD styleEx = 0;
		DWORD style = 0;
		if (m_flagCreatingChildInstances) {
			styleEx = WS_EX_CONTROLPARENT;
			style = WS_CLIPCHILDREN;
		}
		Ref<Win32_ViewInstance> ret = Win32_ViewInstance::create<Win32_ViewInstance>(this, parent, (LPCWSTR)((LONG_PTR)(shared->wndClassForView)), L"", style, styleEx);
		if (ret.isNotNull()) {
			ret->setGenericView(sl_true);
		}
		return ret;
	}

	HWND UIPlatform::getViewHandle(View* view)
	{
		if (view) {
			Ref<ViewInstance> _instance = view->getViewInstance();
			Win32_ViewInstance* instance = (Win32_ViewInstance*)(_instance.get());
			if (instance) {
				return instance->getHandle();
			}
		}
		return 0;
	}
	
	Ref<ViewInstance> UIPlatform::createViewInstance(HWND hWnd, sl_bool flagDestroyOnRelease)
	{
		Ref<ViewInstance> ret = UIPlatform::_getViewInstance((void*)hWnd);
		if (ret.isNotNull()) {
			return ret;
		}
		return Win32_ViewInstance::create<Win32_ViewInstance>(hWnd, flagDestroyOnRelease);
	}

	void UIPlatform::registerViewInstance(HWND hWnd, ViewInstance* instance)
	{
		UIPlatform::_registerViewInstance((void*)hWnd, instance);
	}

	Ref<ViewInstance> UIPlatform::getViewInstance(HWND hWnd)
	{
		return UIPlatform::_getViewInstance((void*)hWnd);
	}

	void UIPlatform::removeViewInstance(HWND hWnd)
	{
		UIPlatform::_removeViewInstance((void*)hWnd);
	}

	HWND UIPlatform::getViewHandle(ViewInstance* _instance)
	{
		Win32_ViewInstance* instance = (Win32_ViewInstance*)_instance;
		if (instance) {
			return instance->getHandle();
		} else {
			return 0;
		}
	}

}

#endif
