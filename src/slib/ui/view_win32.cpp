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
					if (parent->isCapturingEvents() || hitTestCapture.isNotNull()) {
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
								if (parent->isCapturingEvents() || hitTestCapture(UIPoint((sl_ui_pos)(pt.x), (sl_ui_pos)(pt.y)))) {
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

	SLIB_DEFINE_OBJECT(Win32_ViewInstance, ViewInstance)
	
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
		LPCWSTR wndClass, LPCWSTR text,
		const UIRect& frame, const Matrix3& transform,
		int style, int styleEx, int styleRemove)
	{

		HWND hWndParent = UIPlatform::getViewHandle(parent);

		if (hWndParent) {

			HINSTANCE hInst = GetModuleHandleW(NULL);
			style |= WS_CHILD;
			if (view->isVisible()) {
				style |= WS_VISIBLE;
			}
			if (view->isCreatingNativeWidget()) {
				if (view->isBorder()) {
					style |= WS_BORDER;
				}
				if (view->isHorizontalScrollBarVisible()) {
					style |= WS_HSCROLL;
				}
				if (view->isVerticalScrollBarVisible()) {
					style |= WS_VSCROLL;
				}
			}
			style &= (~styleRemove);

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

	void Win32_ViewInstance::initialize(HWND hWnd, sl_bool flagDestroyOnRelease)
	{
		m_handle = hWnd;
		m_flagDestroyOnRelease = flagDestroyOnRelease;
		UIPlatform::registerViewInstance(hWnd, this);
	}

	void Win32_ViewInstance::initialize(HWND hWnd, View* view, const String16& text, const UIRect& frame, const Matrix3& transform)
	{
		initialize(hWnd, sl_true);
		m_text = text;
		m_frame = frame;
		m_translation = Transform2::getTranslationFromMatrix(transform);
		if (view->isUsingFont()) {
			setFont(view, view->getFont());
		}
	}

	HWND Win32_ViewInstance::getHandle()
	{
		return m_handle;
	}

	sl_bool Win32_ViewInstance::isValid(View* view)
	{
		HWND hWnd = m_handle;
		if (hWnd) {
			if (IsWindow(hWnd)) {
				return sl_true;
			}
		}
		return sl_false;
	}

	void Win32_ViewInstance::setFocus(View* view, sl_bool flag)
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

	void Win32_ViewInstance::invalidate(View* view)
	{
		if (!(UI::isUiThread()) || g_flagDuringPaint) {
			void (Win32_ViewInstance::*func)(View*) = &ViewInstance::invalidate;
			UI::dispatchToUiThreadUrgently(Function<void()>::bindWeakRef(this, func, sl_null));
			return;
		}
		if (m_layered.isNotNull()) {
			m_layered->flagInvalidated = sl_true;
			UI::dispatchToUiThreadUrgently(SLIB_FUNCTION_WEAKREF(Win32_ViewInstance, updateLayered, this));
			return;
		}
		HWND hWnd = m_handle;
		if (hWnd) {
			InvalidateRect(hWnd, NULL, TRUE);
		}
	}

	void Win32_ViewInstance::invalidate(View* view, const UIRect& rect)
	{
		if (!(UI::isUiThread()) || g_flagDuringPaint) {
			void (ViewInstance::*func)(View*, const UIRect&) = &ViewInstance::invalidate;
			UI::dispatchToUiThreadUrgently(Function<void()>::bindWeakRef(this, func, sl_null, rect));
			return;
		}
		if (m_layered.isNotNull()) {
			m_layered->flagInvalidated = sl_true;
			UI::dispatchToUiThreadUrgently(SLIB_FUNCTION_WEAKREF(Win32_ViewInstance, updateLayered, this));
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

	void Win32_ViewInstance::setFrame(View* view, const UIRect& frame)
	{
		if (isWindowContent()) {
			return;
		}
		m_frame = frame;
		HWND hWnd = m_handle;
		if (hWnd) {
			UINT uFlags = SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOACTIVATE
				| SWP_NOCOPYBITS
				| SWP_ASYNCWINDOWPOS;
			SetWindowPos(hWnd, NULL
				, (int)(frame.left + m_translation.x), (int)(frame.top + m_translation.y)
				, (int)(frame.getWidth()), (int)(frame.getHeight())
				, uFlags
			);
		}
		if (m_layered.isNotNull()) {
			onDrawLayered();
		}
	}

	void Win32_ViewInstance::setTransform(View* view, const Matrix3 &transform)
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
		if (m_layered.isNotNull()) {
			onDrawLayered();
		}
	}

	void Win32_ViewInstance::setVisible(View* view, sl_bool flag)
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

	void Win32_ViewInstance::setEnabled(View* view, sl_bool flag)
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

	void Win32_ViewInstance::setOpaque(View* view, sl_bool flag)
	{
	}

	void Win32_ViewInstance::setAlpha(View* view, sl_real alpha)
	{
	}

	void Win32_ViewInstance::setClipping(View* view, sl_bool flag)
	{
	}

	void Win32_ViewInstance::setDrawing(View* view, sl_bool flag)
	{
	}

	UIPointf Win32_ViewInstance::convertCoordinateFromScreenToView(View* view, const UIPointf& ptScreen)
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

	UIPointf Win32_ViewInstance::convertCoordinateFromViewToScreen(View* view, const UIPointf& ptView)
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

	void Win32_ViewInstance::addChildInstance(View* view, const Ref<ViewInstance>& _child)
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

	void Win32_ViewInstance::removeChildInstance(View* view, const Ref<ViewInstance>& _child)
	{
		Win32_ViewInstance* child = (Win32_ViewInstance*)(_child.get());
		HWND hWnd = child->getHandle();
		if (hWnd) {
			SetParent(hWnd, HWND_MESSAGE);
		}
	}

	void Win32_ViewInstance::bringToFront(View* view)
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

	void Win32_ViewInstance::setFont(View* view, const Ref<Font>& font)
	{
		HWND handle = m_handle;
		if (handle) {
			HFONT hFont = GraphicsPlatform::getGdiFont(font.get());
			if (hFont) {
				SendMessageW(handle, WM_SETFONT, (WPARAM)hFont, TRUE);
				m_font = font;
			}
		}
	}

	void Win32_ViewInstance::setBorder(View* view, sl_bool flag)
	{
		Windows::setWindowStyle(m_handle, WS_BORDER, flag);
	}

	void Win32_ViewInstance::setScrollBarsVisible(View* view, sl_bool flagHorizontal, sl_bool flagVertical)
	{
		HWND handle = m_handle;
		if (handle) {
			WINAPI_ShowScrollBar func = Windows::getAPI_ShowScrollBar();
			if (func) {
				func(handle, SB_HORZ, flagHorizontal);
				func(handle, SB_VERT, flagVertical);
			}
		}
	}

	void Win32_ViewInstance::setText(const String16& text)
	{
		HWND handle = m_handle;
		if (handle) {
			Windows::setWindowText(handle, text);
			m_text = text;
		}
	}

	void Win32_ViewInstance::setLayered(sl_bool flagLayered)
	{
		if (flagLayered) {
			if (m_layered.isNull()) {
				m_layered = new Win32_LayeredViewContext;
			}
			if (m_layered.isNotNull()) {
				onDrawLayered();
			}
		} else {
			m_layered.setNull();
		}
	}

	void Win32_ViewInstance::updateLayered()
	{
		if (m_layered.isNotNull()) {
			if (m_layered->flagInvalidated) {
				m_layered->flagInvalidated = sl_false;
				onDrawLayered();
			}
		}
	}

	void Win32_ViewInstance::onPaint(Canvas* canvas)
	{
		Ref<View> view = m_view;
		if (view.isNull()) {
			return;
		}

		static Ref<Bitmap> bitmap;
		static Ref<Canvas> canvasBitmap;
		UIRect rc = canvas->getInvalidatedRect();
		UISize size = rc.getSize();
		if (size.x < 1) {
			return;
		}
		if (size.y < 1) {
			return;
		}
		UISize screenSize = UI::getScreenSize();
		if (size.x > screenSize.x) {
			size.x = screenSize.x;
		}
		if (size.y > screenSize.y) {
			size.y = screenSize.y;
		}

		sl_uint32 widthBitmap = (sl_uint32)(size.x);
		sl_uint32 heightBitmap = (sl_uint32)(size.y);
		if (bitmap.isNull() || canvasBitmap.isNull() || bitmap->getWidth() < widthBitmap || bitmap->getHeight() < heightBitmap) {
			bitmap = Bitmap::create(widthBitmap, heightBitmap);
			if (bitmap.isNull()) {
				return;
			}
			canvasBitmap = bitmap->getCanvas();
			if (canvasBitmap.isNull()) {
				return;
			}
			canvasBitmap->setAntiAlias(sl_false);
		}

		Color colorBack(0);		
		do {
			if (view->isOpaque()) {
				break;
			}
			if (m_flagWindowContent) {
				Ref<Window> window = view->getWindow();
				if (window.isNull()) {
					return;
				}
				colorBack = window->getBackgroundColor();
				if (colorBack.a == 255) {
					break;
				}
			}
			Color c = GetDefaultBackColor();
			c.blend_PA_NPA(colorBack);
			colorBack = c;
		} while (0);
		rc.setSize(size);
		if (colorBack.isNotZero()) {
			bitmap->resetPixels(0, 0, widthBitmap, heightBitmap, colorBack);
		} else {
			bitmap->resetPixels(0, 0, widthBitmap, 1, Color::White);
		}
		canvasBitmap->setInvalidatedRect(rc);
		CanvasStateScope scope(canvasBitmap.get());
		canvasBitmap->translate(-(sl_real)(rc.left), -(sl_real)(rc.top));
		view->dispatchDraw(canvasBitmap.get());
		canvasBitmap->translate((sl_real)(rc.left), (sl_real)(rc.top));
		canvas->draw(rc, bitmap, Rectangle(0, 0, (sl_real)(size.x), (sl_real)(size.y)));
	}

	void Win32_ViewInstance::onPaint()
	{
		HWND hWnd = m_handle;
		if (!hWnd) {
			return;
		}
		PAINTSTRUCT ps;
		HDC hDC = BeginPaint(hWnd, &ps);
		if (!hDC) {
			return;
		}
		if (ps.rcPaint.right > ps.rcPaint.left && ps.rcPaint.bottom > ps.rcPaint.top) {
			Gdiplus::Graphics graphics(hDC);
			RECT rect;
			GetClientRect(hWnd, &rect);
			Ref<Canvas> canvas = GraphicsPlatform::createCanvas(CanvasType::View, &graphics, rect.right, rect.bottom, sl_false);
			if (canvas.isNotNull()) {
				canvas->setAntiAlias(sl_false);
				canvas->setInvalidatedRect(Rectangle((sl_real)(ps.rcPaint.left), (sl_real)(ps.rcPaint.top), (sl_real)(ps.rcPaint.right), (sl_real)(ps.rcPaint.bottom)));
				g_flagDuringPaint = sl_true;
				onPaint(canvas.get());
				g_flagDuringPaint = sl_false;
			}
		}
		EndPaint(hWnd, &ps);
	}

	void Win32_ViewInstance::onDrawLayered()
	{
		if (m_layered.isNull()) {
			return;
		}

		HWND hWnd = m_handle;
		if (!hWnd) {
			return;
		}

		RECT rc;
		GetWindowRect(hWnd, &rc);
		sl_uint32 width = (sl_uint32)(rc.right - rc.left);
		sl_uint32 height = (sl_uint32)(rc.bottom - rc.top);
		if (width < 1 || height < 1) {
			return;
		}

		if (!(m_layered->prepare(width, height))) {
			return;
		}

		Ref<Canvas> canvas = GraphicsPlatform::createCanvas(CanvasType::View, m_layered->graphicsCache, width, height, sl_false);
		if (canvas.isNotNull()) {
			canvas->setAntiAlias(sl_false);
			m_layered->graphicsCache->Clear(Gdiplus::Color(0));
			canvas->setInvalidatedRect(Rectangle(0, 0, (sl_real)(width), (sl_real)(height)));
			Ref<View> view = getView();
			if (view.isNotNull()) {
				view->dispatchDraw(canvas.get());
			}
		}

		m_layered->sync(0, 0, width, height);

		BLENDFUNCTION bf;
		bf.AlphaFormat = AC_SRC_ALPHA;
		bf.SourceConstantAlpha = 255;
		bf.BlendFlags = 0;
		bf.BlendOp = AC_SRC_OVER;
		POINT ptSrc;
		ptSrc.x = 0;
		ptSrc.y = 0;
		SIZE size;
		size.cx = (LONG)width;
		size.cy = (LONG)height;

		UpdateLayeredWindow(hWnd, NULL, NULL, &size, m_layered->hdcCache, &ptSrc, 0, &bf, ULW_ALPHA);
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
					onPaint();
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

	Win32_LayeredViewContext::Win32_LayeredViewContext()
	{
		flagInvalidated = sl_false;

		hdcCache = NULL;
		hbmCache = NULL;
		hbmOld = NULL;
		graphicsCache = sl_null;
		bitmapCache = sl_null;
		widthCache = 0;
		heightCache = 0;
	}

	Win32_LayeredViewContext::~Win32_LayeredViewContext()
	{
		clear();
	}

	sl_bool Win32_LayeredViewContext::prepare(sl_uint32 width, sl_uint32 height)
	{
		if (!width || !height) {
			return sl_false;
		}
		if (widthCache >= width && heightCache >= height) {
			return sl_true;
		}
		clear();
		width = ((width - 1) | 0xFF) + 1;
		height = ((height - 1) | 0xFF) + 1;
		sl_uint32 sx = (sl_uint32)(GetSystemMetrics(SM_CXSCREEN));
		sl_uint32 sy = (sl_uint32)(GetSystemMetrics(SM_CYSCREEN));
		if (width > sx) {
			width = sx;
		}
		if (height > sy) {
			height = sy;
		}
		HDC hdcScreen = GetDC(HWND_DESKTOP);
		if (!hdcScreen) {
			return sl_false;
		}
		sl_bool bRet = sl_false;
		Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap((int)width, (int)height, PixelFormat32bppARGB);
		if (bitmap) {
			Gdiplus::Graphics* graphics = new Gdiplus::Graphics(bitmap);
			if (graphics) {
				HDC hdc = CreateCompatibleDC(hdcScreen);
				if (hdc) {
					HBITMAP hbm = CreateCompatibleBitmap(hdcScreen, (int)width, (int)height);
					if (hbm) {
						hbmOld = SelectObject(hdc, hbm);
						hbmCache = hbm;
						hdcCache = hdc;
						graphicsCache = graphics;
						graphics = NULL;
						bitmapCache = bitmap;
						bitmap = NULL;
						widthCache = width;
						heightCache = height;
						bRet = sl_true;
					} else {
						DeleteDC(hdc);
					}
				}
				if (graphics) {
					delete graphics;
				}
			}
			if (bitmap) {
				delete bitmap;
			}
		}
		return bRet;
	}

	void Win32_LayeredViewContext::clear()
	{
		if (hdcCache) {
			if (hbmOld) {
				SelectObject(hdcCache, hbmOld);
			}
			DeleteDC(hdcCache);
			hdcCache = NULL;
		}
		hbmOld = NULL;
		if (hbmCache) {
			DeleteObject(hbmCache);
			hbmCache = NULL;
		}
		if (graphicsCache) {
			delete graphicsCache;
			graphicsCache = sl_null;
		}
		if (bitmapCache) {
			delete bitmapCache;
			bitmapCache = sl_null;
		}
		widthCache = 0;
		heightCache = 0;
	}

	void Win32_LayeredViewContext::sync(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height)
	{
		if (bitmapCache && hdcCache) {
			Gdiplus::Rect rc;
			rc.X = (int)x;
			rc.Y = (int)y;
			rc.Width = (int)width;
			rc.Height = (int)height;
			Gdiplus::BitmapData data;
			if (bitmapCache->LockBits(&rc, Gdiplus::ImageLockModeRead, PixelFormat32bppARGB, &data) == Gdiplus::Ok) {
				BITMAPINFOHEADER bih;
				Base::zeroMemory(&bih, sizeof(bih));
				bih.biSize = sizeof(bih);
				bih.biBitCount = 32;
				bih.biCompression = BI_RGB;
				bih.biWidth = data.Stride >> 2;
				bih.biHeight = height;
				bih.biPlanes = 1;
				StretchDIBits(hdcCache, 0, 0, width, height, 0, height - 1, width, -(int)(height), data.Scan0, (BITMAPINFO*)&bih, DIB_RGB_COLORS, SRCCOPY);
				bitmapCache->UnlockBits(&data);
			}
		}
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
		Ref<Win32_ViewInstance> ret = Win32_ViewInstance::create<Win32_ViewInstance>(this, parent, (LPCWSTR)((LONG_PTR)(shared->wndClassForView)), sl_null, style, styleEx);
		if (ret.isNotNull()) {
			ret->setGenericView(sl_true);
			return ret;
		}
		return sl_null;
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
