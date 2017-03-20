/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "view_win32.h"

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/window.h"
#include "../../../inc/slib/ui/select_view.h"
#include "../../../inc/slib/core/safe_static.h"

#define USE_SCREEN_BACK_BUFFER

namespace slib
{

/******************************************
		Win32_ViewInstance
******************************************/
	Win32_ViewInstance::Win32_ViewInstance()
	{
		m_handle = NULL;
		m_flagDestroyOnRelease = sl_false;
		m_actionMouseCapture = UIAction::MouseMove;
	}

	Win32_ViewInstance::~Win32_ViewInstance()
	{
		if (m_handle) {
			UIPlatform::removeViewInstance(m_handle);
			if (m_flagDestroyOnRelease) {
				::PostMessageW(m_handle, SLIB_UI_MESSAGE_CLOSE, 0, 0);
			}
		}
	}

	HWND Win32_ViewInstance::createHandle(
		View* view, ViewInstance* parent
		, LPCWSTR wndClass, LPCWSTR text
		, int style, int styleEx)
	{

		HWND hWndParent = UIPlatform::getViewHandle(parent);

		if (hWndParent) {

			HINSTANCE hInst = ::GetModuleHandleW(NULL);
			UIRect frame = view->getFrame();
			style |= WS_CHILD;
			if (view->isVisible()) {
				style |= WS_VISIBLE;
			}

			Vector2 t;
			if (view->getFinalTranslationRotationScale(&t, NULL, NULL, NULL)) {
				frame.left += (sl_ui_pos)(t.x);
				frame.top += (sl_ui_pos)(t.y);
			}

			HWND hWnd = ::CreateWindowExW(
				styleEx // ex-style
				, wndClass
				, text
				, style
				, (int)(frame.left), (int)(frame.top)
				, (int)(frame.getWidth()), (int)(frame.getHeight())
				, hWndParent // parent
				, NULL // menu
				, hInst
				, NULL);
			if (hWnd) {
				if (!(view->isEnabled())) {
					::EnableWindow(hWnd, FALSE);
				}
				return hWnd;
			}
		}
		return NULL;
	}

	HWND Win32_ViewInstance::getHandle()
	{
		return m_handle;
	}

	sl_bool Win32_ViewInstance::isValid()
	{
		HWND hWnd = m_handle;
		if (hWnd) {
			if (::IsWindow(hWnd)) {
				return sl_true;
			}
		}
		return sl_false;
	}

	void Win32_ViewInstance::setFocus()
	{
		HWND hWnd = m_handle;
		if (hWnd) {
			::SetFocus(hWnd);
		}
	}

	void Win32_ViewInstance::invalidate()
	{
		HWND hWnd = m_handle;
		if (hWnd) {
			::InvalidateRect(hWnd, NULL, TRUE);
		}
	}

	void Win32_ViewInstance::invalidate(const UIRect& rect)
	{
		HWND hWnd = m_handle;
		if (hWnd) {
			RECT rc;
			rc.left = (int)(rect.left);
			rc.top = (int)(rect.top);
			rc.right = (int)(rect.right);
			rc.bottom = (int)(rect.bottom);
			::InvalidateRect(hWnd, &rc, TRUE);
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
		HWND hWnd = m_handle;
		if (hWnd) {
			UINT uFlags = SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOACTIVATE
				| SWP_NOCOPYBITS
				| SWP_ASYNCWINDOWPOS
				;
			::SetWindowPos(hWnd, NULL
				, (int)(frame.left), (int)(frame.top)
				, (int)(frame.getWidth()), (int)(frame.getHeight())
				, uFlags
			);
		}
	}

	void Win32_ViewInstance::setTransform(const Matrix3 &transform)
	{
	}

	void Win32_ViewInstance::setVisible(sl_bool flag)
	{
		HWND hWnd = m_handle;
		if (hWnd) {
			sl_bool f1 = ::IsWindowVisible(hWnd) ? sl_true : sl_false;
			sl_bool f2 = flag ? sl_true : sl_false;
			if (f1 != f2) {
				if (f2) {
					//::ShowWindow(hWnd, SW_SHOW);
					::ShowWindowAsync(hWnd, SW_SHOW);
				} else {
					//::ShowWindow(hWnd, SW_HIDE);
					::ShowWindowAsync(hWnd, SW_HIDE);
				}
			}
		}
	}

	void Win32_ViewInstance::setEnabled(sl_bool flag)
	{
		HWND hWnd = m_handle;
		if (hWnd) {
			sl_bool f1 = ::IsWindowEnabled(hWnd) ? sl_true : sl_false;
			sl_bool f2 = flag ? sl_true : sl_false;
			if (f1 != f2) {
				::EnableWindow(hWnd, f2);
			}
		}
	}

	void Win32_ViewInstance::setOpaque(sl_bool flag)
	{
	}

	void Win32_ViewInstance::setAlpha(sl_real alpha)
	{
		::InvalidateRect(m_handle, NULL, TRUE);
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
			::ScreenToClient(hWnd, &pt);
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
			::ClientToScreen(hWnd, &pt);
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
					::SetParent(hWndChild, hWnd);
				}
			}
		}
	}

	void Win32_ViewInstance::removeChildInstance(const Ref<ViewInstance>& _child)
	{
		Win32_ViewInstance* child = (Win32_ViewInstance*)(_child.get());
		HWND hWnd = child->getHandle();
		if (hWnd) {
			::SetParent(hWnd, HWND_MESSAGE);
		}
	}

	void Win32_ViewInstance::bringToFront()
	{
		HWND hWnd = m_handle;
		if (hWnd) {
			::BringWindowToTop(hWnd);
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
			t.setMillisecondsCount(::GetMessageTime());
			Ref<UIEvent> ke = UIEvent::createKeyEvent(action, key, vkey, t);
			if (ke.isNotNull()) {
				applyModifiers(ke.get());
				onKeyEvent(ke.get());
				if (ke->isPreventedDefault()) {
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
			t.setMillisecondsCount(::GetMessageTime());
			Ref<UIEvent> me = UIEvent::createMouseEvent(action, x, y, t);
			if (me.isNotNull()) {
				applyModifiers(me.get());
				onMouseEvent(me.get());
				if (me->isPreventedDefault()) {
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
			const DWORD lParam = ::GetMessagePos();
			POINT pt;
			pt.x = (short)(lParam & 0xffff);
			pt.y = (short)((lParam >> 16) & 0xffff);
			::ScreenToClient(hWnd, &pt);
			Time t;
			t.setMillisecondsCount(::GetMessageTime());
			Ref<UIEvent> me = UIEvent::createMouseWheelEvent((sl_ui_posf)(pt.x), (sl_ui_posf)(pt.y), deltaX, deltaY, t);
			if (me.isNotNull()) {
				applyModifiers(me.get());
				onMouseWheelEvent(me.get());
				if (me->isPreventedDefault()) {
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
			const DWORD lParam = ::GetMessagePos();
			POINT pt;
			pt.x = (short)(lParam & 0xffff);
			pt.y = (short)((lParam >> 16) & 0xffff);
			::ScreenToClient(hWnd, &pt);
			Time t;
			t.setMillisecondsCount(::GetMessageTime());
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

	void Win32_ViewInstance::applyModifiers(UIEvent* ev)
	{
		if (GetKeyState(VK_SHIFT) & 0x8000) {
			ev->setShiftKey();
		}
		if (GetKeyState(VK_MENU) & 0x8000) {
			ev->setAltKey();
		}
		if (GetKeyState(VK_CONTROL) & 0x8000) {
			ev->setControlKey();
		}
		if ((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & 0x8000) {
			ev->setWindowsKey();
		}
	}

	sl_bool Win32_ViewInstance::preprocessWindowMessage(MSG& msg)
	{
		return sl_false;
	}

	class _Win32_Draw_BackBuffer
	{
	public:
		Mutex mutex;
		Ref<Bitmap> bitmapBuffer;
		Ref<Canvas> canvasBuffer;
		Gdiplus::Bitmap* bitmap;
		Gdiplus::Graphics* graphics;

	public:
		_Win32_Draw_BackBuffer()
		{
			bitmap = sl_null;
			graphics = sl_null;
		}

		void allocateBackBuffer(sl_uint32 width, sl_uint32 height)
		{
			UISize screenSize = UI::getScreenSize();
			if ((sl_ui_pos)width > screenSize.x) {
				width = screenSize.x;
			}
			if ((sl_ui_pos)height > screenSize.y) {
				height = screenSize.y;
			}
			if (bitmapBuffer.isNull() || canvasBuffer.isNull() || bitmapBuffer->getWidth() < width || bitmapBuffer->getHeight() < height) {
				bitmap = sl_null;
				graphics = sl_null;
				bitmapBuffer = Bitmap::create((width + 255) & 0xFFFFFF00, (height + 255) & 0xFFFFFF00);
				if (bitmapBuffer.isNull()) {
					return;
				}
				canvasBuffer = bitmapBuffer->getCanvas();
				if (canvasBuffer.isNull()) {
					return;
				}
				bitmap = GraphicsPlatform::getBitmapHandle(bitmapBuffer.get());
				graphics = GraphicsPlatform::getCanvasHandle(canvasBuffer.get());
			}
		}

	};

	static sl_size _Win32_getSelfInvalidatableRects(View* view, UIRect bounds, UIRect* outRects, sl_size countRectsBuf)
	{
		if (!countRectsBuf) {
			return 0;
		}
		sl_size count = 0;
		ListElements< Ref<View> > children(view->getChildren());
		for (sl_size i = 0; i < children.count; i++) {
			Ref<View>& child = children[i];
			if (child.isNotNull()) {
				if (child->isVisible() && child->getWidth() > 0 && child->getHeight() > 0) {
					if (child->checkSelfInvalidatable()) {
						UIRect rect = child->getBoundsInParent();
						if (child->isNativeWidget() && IsInstanceOf<SelectView>(child)) {
							rect.setSize(child->getInstanceFrame().getSize());
						}
						if (rect.intersectRectangle(bounds, &rect)) {
							*outRects = rect;
							outRects++;
							count++;
							countRectsBuf--;
							if (!countRectsBuf) {
								return count;
							}
						}
					} else {
						sl_ui_pos x = child->getLeft();
						sl_ui_pos y = child->getTop();
						sl_size n = _Win32_getSelfInvalidatableRects(child.get(), child->getBounds(), outRects, countRectsBuf);
						sl_size m = 0;
						for (sl_size k = 0; k < n; k++) {
							outRects[k].left += x;
							outRects[k].top += y;
							outRects[k].right += x;
							outRects[k].bottom += y;
							if (outRects[k].intersectRectangle(bounds, &outRects[m])) {
								m++;
							}
						}
						count += m;
						outRects += m;
						countRectsBuf -= m;
					}
				}
			}
		}
		return count;
	}

	static void _Win32_DrawView(Canvas* canvas, View* view)
	{
		Color colorClear = Color::zero();
		do {
			if (view->isOpaque()) {
				break;
			}
			Color color = view->getBackgroundColor();
			if (color.a == 255) {
				break;
			}

			Ref<Window> window = view->getWindow();
			if (window.isNotNull() && window->getContentView() == view) {
				colorClear = window->getBackgroundColor();
				if (colorClear.a < 255) {
					Color c = GraphicsPlatform::getColorFromColorRef(::GetSysColor(COLOR_MENU));
					c.blend_PA_NPA(colorClear);
					colorClear = c;
				}
				break;
			} else {
				colorClear = GraphicsPlatform::getColorFromColorRef(::GetSysColor(COLOR_MENU));
			}
		} while (0);
		if (colorClear.isNotZero()) {
			canvas->fillRectangle(view->getBounds(), colorClear);
		}
		view->dispatchDraw(canvas);
	}

	static void _Win32_DrawViewRegion(Gdiplus::Graphics* graphics, View* view, RECT& rcPaint)
	{
		sl_uint32 widthRedraw = (sl_uint32)(rcPaint.right - rcPaint.left);
		sl_uint32 heightRedraw = (sl_uint32)(rcPaint.bottom - rcPaint.top);
		Ref<Canvas> canvas = GraphicsPlatform::createCanvas(CanvasType::Bitmap, graphics, widthRedraw, heightRedraw, sl_false);
		if (canvas.isNotNull()) {
			CanvasStateScope scope(canvas);
			canvas->translate(-(sl_real)(rcPaint.left), -(sl_real)(rcPaint.top));
			canvas->setInvalidatedRect(Rectangle((sl_real)(rcPaint.left), (sl_real)(rcPaint.top), (sl_real)(rcPaint.right), (sl_real)(rcPaint.bottom)));
			_Win32_DrawView(canvas.get(), view);
		}
	}

	sl_bool Win32_ViewInstance::processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result)
	{
		HWND hWnd = m_handle;
		switch (msg) {
			case WM_ERASEBKGND:
				{
					result = TRUE;
					return sl_true;
				}

			case WM_PAINT:
				{
					PAINTSTRUCT ps;
					HDC hDC = ::BeginPaint(hWnd, &ps);
					if (hDC) {
						if (ps.rcPaint.right > ps.rcPaint.left && ps.rcPaint.bottom > ps.rcPaint.top) {
							Ref<View> view = getView();
							if (view.isNotNull() && view->checkSelfInvalidatable()) {
								Gdiplus::Graphics graphics(hDC);
#if defined(USE_SCREEN_BACK_BUFFER)
								sl_uint32 widthRedraw = (sl_uint32)(ps.rcPaint.right - ps.rcPaint.left);
								sl_uint32 heightRedraw = (sl_uint32)(ps.rcPaint.bottom - ps.rcPaint.top);
								SLIB_SAFE_STATIC(_Win32_Draw_BackBuffer, bb)
								if (!(SLIB_SAFE_STATIC_CHECK_FREED(bb))) {
									MutexLocker lock(&(bb.mutex));
									bb.allocateBackBuffer(widthRedraw, heightRedraw);
									if (bb.bitmap && bb.graphics) {
										_Win32_DrawViewRegion(bb.graphics, view.get(), ps.rcPaint);
										Gdiplus::Rect rcPaint(ps.rcPaint.left, ps.rcPaint.top, widthRedraw, heightRedraw);
										Gdiplus::Region region(rcPaint);
										static UIRect rectsExclude[128];
										sl_size n = _Win32_getSelfInvalidatableRects(view.get(), UIRect(ps.rcPaint.left, ps.rcPaint.top, ps.rcPaint.right, ps.rcPaint.bottom), rectsExclude, 128);
										if (n > 0) {
											for (sl_size i = 0; i < n; i++) {
												region.Exclude(Gdiplus::Rect(rectsExclude[i].left, rectsExclude[i].top, rectsExclude[i].getWidth(), rectsExclude[i].getHeight()));
											}
											graphics.IntersectClip(&region);
										}
										graphics.DrawImage(bb.bitmap, rcPaint, 0, 0, widthRedraw, heightRedraw, Gdiplus::UnitPixel);
									}
								}
#else
								RECT rect;
								::GetClientRect(hWnd, &rect);
								Ref<Canvas> canvas = GraphicsPlatform::createCanvas(CanvasType::View, &graphics, rect.right, rect.bottom, sl_false);
								if (canvas.isNotNull()) {
									canvas->setInvalidatedRect(Rectangle((sl_real)(ps.rcPaint.left), (sl_real)(ps.rcPaint.top), (sl_real)(ps.rcPaint.right), (sl_real)(ps.rcPaint.bottom)));
									_Win32_DrawView(canvas.get(), view.get());
								}
#endif
							}
						}
						::EndPaint(hWnd, &ps);
					}
					result = TRUE;
					return sl_true;
				}

			case WM_KEYDOWN:
				{
					return onEventKey(sl_true, wParam, lParam);
				}
			case WM_KEYUP:
				{
					return onEventKey(sl_false, wParam, lParam);
				}
			case WM_LBUTTONDOWN:
				{
					sl_bool flag = onEventMouse(UIAction::LeftButtonDown, wParam, lParam);
					m_actionMouseCapture = UIAction::LeftButtonDown;
					::SetCapture(hWnd);
					return flag;
				}
			case WM_LBUTTONDBLCLK:
				{
					return onEventMouse(UIAction::LeftButtonDoubleClick, wParam, lParam);
				}
			case WM_LBUTTONUP:
				{
					sl_bool flag = onEventMouse(UIAction::LeftButtonUp, wParam, lParam);
					::ReleaseCapture();
					return flag;
				}
			case WM_RBUTTONDOWN:
				{
					sl_bool flag = onEventMouse(UIAction::RightButtonDown, wParam, lParam);
					m_actionMouseCapture = UIAction::RightButtonDown;
					::SetCapture(hWnd);
					return flag;
				}
			case WM_RBUTTONDBLCLK:
				{
					return onEventMouse(UIAction::RightButtonDoubleClick, wParam, lParam);
				}
			case WM_RBUTTONUP:
				{
					sl_bool flag = onEventMouse(UIAction::RightButtonUp, wParam, lParam);
					::ReleaseCapture();
					return flag;
				}
			case WM_MBUTTONDOWN:
				{
					sl_bool flag = onEventMouse(UIAction::MiddleButtonDown, wParam, lParam);
					m_actionMouseCapture = UIAction::MiddleButtonDown;
					::SetCapture(hWnd);
					return flag;
				}
			case WM_MBUTTONDBLCLK:
				{
					return onEventMouse(UIAction::MiddleButtonDoubleClick, wParam, lParam);
				}
			case WM_MBUTTONUP:
				{
					sl_bool flag = onEventMouse(UIAction::MiddleButtonUp, wParam, lParam);
					::ReleaseCapture();
					return flag;
				}
			case WM_MOUSEMOVE:
				{
					TRACKMOUSEEVENT track;
					track.cbSize = sizeof(track);
					track.dwFlags = TME_LEAVE | TME_QUERY;
					track.hwndTrack = hWnd;
					track.dwHoverTime = 0;
					::TrackMouseEvent(&track);
					if (track.dwFlags == 0) {
						track.cbSize = sizeof(track);
						track.dwFlags = TME_LEAVE;
						track.hwndTrack = hWnd;
						track.dwHoverTime = HOVER_DEFAULT;
						::TrackMouseEvent(&track);
						onEventMouse(UIAction::MouseEnter, wParam, lParam);
					}

					if (::GetCapture() == hWnd) {
						if (m_actionMouseCapture == UIAction::LeftButtonDown) {
							return onEventMouse(UIAction::LeftButtonDrag, wParam, lParam);
						} else if (m_actionMouseCapture == UIAction::RightButtonDown) {
							return onEventMouse(UIAction::RightButtonDrag, wParam, lParam);
						} else if (m_actionMouseCapture == UIAction::MiddleButtonDown) {
							return onEventMouse(UIAction::MiddleButtonDrag, wParam, lParam);
						} else {
							return onEventMouse(UIAction::MouseMove, wParam, lParam);
						}
					} else {
						return onEventMouse(UIAction::MouseMove, wParam, lParam);
					}
					break;
				}
			case WM_MOUSELEAVE:
				{
					onEventMouse(UIAction::MouseLeave, wParam, lParam);
					return sl_true;
				}
			case WM_MOUSEWHEEL:
				{
					if (onEventMouseWheel(sl_true, wParam, lParam)) {
						return sl_true;
					}
					break;
				}
			case 0x020E: // WM_MOUSEHWHEEL
				{
					if (onEventMouseWheel(sl_false, wParam, lParam)) {
						return sl_true;
					}
					break;
				}
			case WM_SETCURSOR:
				{
					if (onEventSetCursor()) {
						result = 1;
						return sl_true;
					}
					break;
				}
		}
		return sl_false;
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

	LRESULT CALLBACK _Win32_ViewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		Ref<Win32_ViewInstance> instance = Ref<Win32_ViewInstance>::from(UIPlatform::getViewInstance(hWnd));
		if (instance.isNotNull()) {
			LRESULT result = 0;
			if (instance->processWindowMessage(uMsg, wParam, lParam, result)) {
				return result;
			}
		}
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
							result = (HBRUSH)(::DefWindowProcW(hWnd, uMsg, wParam, lParam));
						}
						instance->processPostControlColor(uMsg, hDC, result);
						return (LRESULT)result;
					}
				}
				break;
		}
		return ::DefWindowProcW(hWnd, uMsg, wParam, lParam);
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

	sl_bool _Win32_captureChildInstanceEvents(View* view, UINT uMsg)
	{
		Ref<View> parent = view->getParent();
		while (parent.isNotNull()) {
			if (parent->isCapturingChildInstanceEvents()) {
				if (_Win32_captureChildInstanceEvents(parent.get(), uMsg)) {
					return sl_true;
				}
				Ref<ViewInstance> _instance = parent->getViewInstance();
				if (_instance.isNotNull()) {
					Win32_ViewInstance* instance = (Win32_ViewInstance*)(_instance.get());
					HWND hWnd = instance->getHandle();
					if (hWnd) {
						DWORD lParam = ::GetMessagePos();
						POINT pt;
						pt.x = (short)(lParam & 0xffff);
						pt.y = (short)((lParam >> 16) & 0xffff);
						::ScreenToClient(hWnd, &pt);
						if (parent->hitTestForCapturingChildInstanceEvents(UIPoint((sl_ui_pos)(pt.x), (sl_ui_pos)(pt.y)))) {
							LPARAM lParam = POINTTOPOINTS(pt);
							LRESULT res;
							instance->processWindowMessage(uMsg, 0, lParam, res);
							return sl_true;
						}
					}
				}
			}
			parent = parent->getParent();
		}
		return sl_false;
	}

	sl_bool _Win32_captureChildInstanceEvents(View* view, MSG& msg)
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
		return _Win32_captureChildInstanceEvents(view, uMsg);
	}

/******************************************
				View
******************************************/
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
		}
		Ref<Win32_ViewInstance> ret = Win32_ViewInstance::create<Win32_ViewInstance>(this, parent, (LPCWSTR)((LONG_PTR)(shared->wndClassForView)), L"", style, styleEx);
		return ret;
	}

	void View::_setFrame_NI(const UIRect& frame)
	{
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			if (instance->isWindowContent()) {
				return;
			}
			HWND hWnd = UIPlatform::getViewHandle(instance.get());
			if (hWnd) {
				UINT uFlags = SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOACTIVATE
					| SWP_NOCOPYBITS
					| SWP_ASYNCWINDOWPOS
					;
				int x = frame.left;
				int y = frame.top;
				Vector2 t;
				if (getFinalTranslationRotationScale(&t, NULL, NULL, NULL)) {
					x += (int)(t.x);
					y += (int)(t.y);
				}
				::SetWindowPos(hWnd, NULL
					, x, y
					, (int)(frame.getWidth()), (int)(frame.getHeight())
					, uFlags
				);
			}
		}
	}

	void View::_setTransform_NI(const Matrix3& matrix)
	{
		Ref<ViewInstance> instance = m_instance;
		if (instance.isNotNull()) {
			if (instance->isWindowContent()) {
				return;
			}
			HWND hWnd = UIPlatform::getViewHandle(instance.get());
			if (hWnd) {
				UINT uFlags = SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE
					| SWP_NOCOPYBITS
					| SWP_ASYNCWINDOWPOS
					;
				int x = (int)(m_frame.left);
				int y = (int)(m_frame.top);
				Vector2 t;
				if (getFinalTranslationRotationScale(&t, NULL, NULL, NULL)) {
					x += (int)(t.x);
					y += (int)(t.y);
				}
				::SetWindowPos(hWnd, NULL
					, x, y
					, 0, 0
					, uFlags
				);
			}
		}
	}

/******************************************
			UIPlatform
******************************************/
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
