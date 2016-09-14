#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "view_win32.h"

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/window.h"
#include "../../../inc/slib/ui/scroll_view.h"

#define USE_SCREEN_BACK_BUFFER

SLIB_UI_NAMESPACE_BEGIN

class _View : public View
{
	friend class Win32_ViewInstance;
};

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
	View* _view, ViewInstance* parent
	, LPCWSTR wndClass, LPCWSTR text
	, int style, int styleEx)
{
	_View* view = (_View*)_view;
	HWND hWndParent = UIPlatform::getViewHandle(parent);
	if (hWndParent) {
		HINSTANCE hInst = ::GetModuleHandleW(NULL);
		UIRect frame = view->getFrame();
		style |= WS_CHILD;
		if (view->isVisible()) {
			style |= WS_VISIBLE;
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
		::GetWindowRect(hWnd, &rc);
		UIRect ret;
		ret.left = (sl_ui_pos)(rc.left);
		ret.top = (sl_ui_pos)(rc.top);
		ret.right = (sl_ui_pos)(rc.right);
		ret.bottom = (sl_ui_pos)(rc.bottom);
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
		Win32_ViewInstance* child = (Win32_ViewInstance*)(_child.ptr);
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
	Win32_ViewInstance* child = (Win32_ViewInstance*)(_child.ptr);
	HWND hWnd = child->getHandle();
	if (hWnd) {
		::SetParent(hWnd, HWND_MESSAGE);
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
		Ref<UIEvent> ke = UIEvent::createKeyEvent(action, key, vkey);
		if (ke.isNotNull()) {
			applyModifiers(ke.ptr);
			onKeyEvent(ke.ptr);
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

		Ref<UIEvent> me = UIEvent::createMouseEvent(action, x, y);
		if (me.isNotNull()) {
			applyModifiers(me.ptr);
			onMouseEvent(me.ptr);
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
		Ref<UIEvent> me = UIEvent::createMouseWheelEvent((sl_ui_posf)(pt.x), (sl_ui_posf)(pt.y), deltaX, deltaY);
		if (me.isNotNull()) {
			applyModifiers(me.ptr);
			onMouseWheelEvent(me.ptr);
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
		Ref<UIEvent> ev = UIEvent::createSetCursorEvent((sl_ui_posf)(pt.x), (sl_ui_posf)(pt.y));
		if (ev.isNotNull()) {
			onSetCursor(ev.ptr);
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

static void _Win32_DrawView(Canvas* canvas, View* view, sl_bool flagErased)
{
	Color colorClear = Color::zero();
	do {
		if (view->isDoubleBuffering()) {
			break;
		}
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
				Color c = UIPlatform::getColorFromColorRef(::GetSysColor(COLOR_MENU));
				c.blend_PA_NPA(colorClear);
				colorClear = c;
			}
			break;
		} else {
			if (!flagErased) {
				colorClear = UIPlatform::getColorFromColorRef(::GetSysColor(COLOR_MENU));
			}
		}
	} while (0);
	if (colorClear.isNotZero()) {
		canvas->fillRectangle(view->getBounds(), colorClear);
	}
	view->dispatchDraw(canvas);
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
			bitmapBuffer = UI::createBitmap((width + 255) & 0xFFFFFF00, (height + 255) & 0xFFFFFF00);
			if (bitmapBuffer.isNull()) {
				return;
			}
			canvasBuffer = bitmapBuffer->getCanvas();
			if (canvasBuffer.isNull()) {
				return;
			}
			bitmap = UIPlatform::getBitmapHandle(bitmapBuffer.ptr);
			graphics = UIPlatform::getCanvasHandle(canvasBuffer.ptr);
		}
	}

};

static void _Win32_DrawViewInBackbuffer(Gdiplus::Graphics* graphics, View* view, RECT& rcPaint)
{
	Color backgroundColor = view->getBackgroundColor();
	sl_bool flagOpaque = sl_false;
	if (view->isOpaque()) {
		flagOpaque = sl_true;
	} else if (view->isDoubleBuffering()) {
		flagOpaque = sl_true;
	} else if (backgroundColor.a == 255) {
		flagOpaque = sl_true;
	}
	sl_bool flagDrawParent = sl_false;
	Ref<View> parent = view->getParent();
	if (parent.isNotNull()) {
		if (parent->isInstance() && !(parent->isNativeWidget())) {
			RECT rcParent;
			int x = (int)(view->getLeft());
			int y = (int)(view->getTop());
			rcParent.left = rcPaint.left + x;
			if (rcParent.left < 0) {
				rcParent.left = 0;
			}
			rcParent.top = rcPaint.top + y;
			if (rcParent.top < 0) {
				rcParent.top = 0;
			}
			rcParent.right = rcPaint.right + x;
			int width = (int)(parent->getWidth());
			if (rcParent.right > width) {
				rcParent.right = width;
			}
			rcParent.bottom = rcPaint.bottom + y;
			int height = (int)(parent->getHeight());
			if (rcParent.bottom > height) {
				rcParent.bottom = height;
			}
			if (rcParent.right > rcParent.left && rcParent.bottom > rcParent.top) {
				_Win32_DrawViewInBackbuffer(graphics, parent.ptr, rcParent);
			}
			flagDrawParent = sl_true;
		}
	}
	sl_uint32 widthRedraw = (sl_uint32)(rcPaint.right - rcPaint.left);
	sl_uint32 heightRedraw = (sl_uint32)(rcPaint.bottom - rcPaint.top);
	Ref<Canvas> canvas = UIPlatform::createCanvas(graphics, widthRedraw, heightRedraw, sl_null, sl_false);
	if (canvas.isNotNull()) {
		CanvasStatusScope scope(canvas);
		canvas->clipToRectangle(0, 0, (sl_real)(widthRedraw), (sl_real)(heightRedraw));
		canvas->translate(-(sl_real)(rcPaint.left), -(sl_real)(rcPaint.top));
		_Win32_DrawView(canvas.ptr, view, flagDrawParent);
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
						if (view.isNotNull()) {
							Gdiplus::Graphics* _graphics = new Gdiplus::Graphics(hDC);
							if (_graphics) {
#if defined(USE_SCREEN_BACK_BUFFER)
								sl_uint32 widthRedraw = (sl_uint32)(ps.rcPaint.right - ps.rcPaint.left);
								sl_uint32 heightRedraw = (sl_uint32)(ps.rcPaint.bottom - ps.rcPaint.top);
								SLIB_SAFE_STATIC(_Win32_Draw_BackBuffer, bb)
								if (!(SLIB_SAFE_STATIC_CHECK_FREED(bb))) {
									MutexLocker lock(&(bb.mutex));
									bb.allocateBackBuffer(widthRedraw, heightRedraw);
									if (bb.bitmap && bb.graphics) {
										_Win32_DrawViewInBackbuffer(bb.graphics, view.ptr, ps.rcPaint);
										_graphics->DrawImage(bb.bitmap, Gdiplus::Rect(ps.rcPaint.left, ps.rcPaint.top, widthRedraw, heightRedraw), 0, 0, widthRedraw, heightRedraw, Gdiplus::UnitPixel);
									}
								}

#else
								RECT rect;
								::GetClientRect(hWnd, &rect);
								Rectangle rcDirty((sl_real)(ps.rcPaint.left), (sl_real)(ps.rcPaint.top), (sl_real)(ps.rcPaint.right), (sl_real)(ps.rcPaint.bottom));
								Ref<Canvas> graphics = UIPlatform::createCanvas(_graphics, rect.right, rect.bottom, &rcDirty);
								if (graphics.isNotNull()) {
									_graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
									_Win32_DrawView(graphics.ptr, view.ptr, sl_false);
								}
#endif
							}
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

/******************************************
				View
******************************************/
Ref<ViewInstance> View::createGenericInstance(ViewInstance* parent)
{
	Win32_UI_Shared* shared = Win32_UI_Shared::get();
	if (!shared) {
		return Ref<ViewInstance>::null();
	}

	DWORD styleEx = 0;
	DWORD style = 0;
	if (m_flagCreatingChildInstances) {
		styleEx = WS_EX_CONTROLPARENT;
#if defined(_SLIB_UI_WIN32_USE_COMPOSITE_VIEWS)
		styleEx |= WS_EX_COMPOSITED;
#endif
#if defined(_SLIB_UI_WIN32_USE_CLIP_CHILDREN)
		//style |= WS_CLIPSIBLINGS;
		style |= WS_CLIPCHILDREN;
#endif
	}
	Ref<Win32_ViewInstance> ret = Win32_ViewInstance::create<Win32_ViewInstance>(this, parent, (LPCWSTR)((LONG_PTR)(shared->wndClassForView)), L"", style, styleEx);
	return ret;
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

HWND UIPlatform::getViewHandle(View* view)
{
	if (view) {
		Ref<ViewInstance> _instance = view->getViewInstance();
		Win32_ViewInstance* instance = (Win32_ViewInstance*)(_instance.ptr);
		if (instance) {
			return instance->getHandle();
		}
	}
	return 0;
}

SLIB_UI_NAMESPACE_END

#endif
