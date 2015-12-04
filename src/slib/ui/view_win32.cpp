#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "view_win32.h"

#include "../../../inc/slib/ui/view_group.h"
#include "../../../inc/slib/ui/scroll_view.h"

SLIB_UI_NAMESPACE_BEGIN
/******************************************
		_Win32_View_Static
******************************************/

LRESULT CALLBACK _Win32_ViewWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK _Win32_ViewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

class _View : public View
{
	friend class Win32_ViewInstance;
};

Win32_View_Shared::Win32_View_Shared()
{
	HINSTANCE hInst = ::GetModuleHandleW(NULL);
	// register view class
	{
		WNDCLASSEXW wc;
		::ZeroMemory(&wc, sizeof(wc));
		wc.cbSize = sizeof(wc);
		wc.style = CS_DBLCLKS | CS_PARENTDC;
		wc.lpfnWndProc = _Win32_ViewProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = hInst;
		wc.hIcon = NULL;
		wc.hCursor = ::LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = L"SLIBUIVIEW";
		wc.hIconSm = NULL;
		wndClass = ::RegisterClassExW(&wc);
	}
}

Win32_View_Shared* Win32_View_Shared::get()
{
	SLIB_SAFE_STATIC(Win32_View_Shared, ret);
	return &ret;
}

/******************************************
		Win32_ViewInstance
******************************************/
Win32_ViewInstance::Win32_ViewInstance()
{
	m_handle = NULL;
	m_flagDestroyOnRelease = sl_false;
	m_actionMouseCapture = actionMouseMove;
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
		const Rectangle& frame = view->getFrame();
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

void Win32_ViewInstance::onEventKey(sl_bool flagDown, WPARAM wParam, LPARAM lParam)
{
	HWND hWnd = m_handle;
	if (hWnd) {
		UIEventAction action = flagDown ? actionKeyDown : actionKeyUp;
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
			applyModifiers(ke.get());
			onKeyEvent(ke.get());
		}
	}
}

void Win32_ViewInstance::onEventMouse(UIEventAction action, WPARAM wParam, LPARAM lParam)
{
	HWND hWnd = m_handle;
	if (hWnd) {
		int _x = (short)(lParam & 0xffff);
		int _y = (short)((lParam >> 16) & 0xffff);

		sl_real x = (sl_real)(_x);
		sl_real y = (sl_real)(_y);

		Ref<UIEvent> me = UIEvent::createMouseEvent(action, x, y);
		if (me.isNotNull()) {
			applyModifiers(me.get());
			onMouseEvent(me.get());
		}
	}
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
		Ref<UIEvent> me = UIEvent::createMouseWheelEvent(deltaX, deltaY);
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
		Ref<UIEvent> ev = UIEvent::createSetCursorEvent();
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

sl_bool Win32_ViewInstance::processWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT& result)
{
	HWND hWnd = m_handle;
	switch (msg) {
		case WM_ERASEBKGND:
#if defined(_SLIB_UI_WIN32_USE_CLIP_CHILDREN)
			{
				Ref<View> _view = getView();
				if (ViewGroup::checkInstance(_view)) {
					ViewGroup* view = (ViewGroup*)(_view.get());
					Color color = view->getBackgroundColor();
					if (color.getAlpha() == 0) {
						return sl_false;
					}
				} else if (_view.isNotNull()) {
					if (_view->isOpaque()) {
						result = TRUE;
						return sl_true;
					}
					_view = _view->getParent();
					Color color;
					sl_bool flagClear = sl_false;
					if (ViewGroup::checkInstance(_view)) {
						ViewGroup* view = (ViewGroup*)(_view.get());
						color = view->getBackgroundColor();
						flagClear = sl_true;
					} else if (ScrollView::checkInstance(_view)) {
						ScrollView* view = (ScrollView*)(_view.get());
						color = view->getBackgroundColor();
						flagClear = sl_true;
					}
					if (flagClear) {
						if (color.getAlpha() == 0) {
							return sl_false;
						} else {
							HBRUSH hbr = ::CreateSolidBrush(RGB(color.getRed(), color.getGreen(), color.getBlue()));
							if (hbr) {
								HDC hDC = (HDC)(wParam);
								RECT rc;
								::GetClientRect(hWnd, &rc);
								::FillRect(hDC, &rc, hbr);
								::DeleteObject(hbr);
								result = TRUE;
								return sl_true;
							}
						}
					}
				}
			}
#endif
			result = TRUE;
			return sl_true;

		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hDC = ::BeginPaint(hWnd, &ps);
				if (hDC) {
					Gdiplus::Graphics* _graphics = new Gdiplus::Graphics(hDC);
					if (_graphics) {
						RECT rect;
						::GetClientRect(hWnd, &rect);
						Rectangle rcDirty((sl_real)(ps.rcPaint.left), (sl_real)(ps.rcPaint.top), (sl_real)(ps.rcPaint.right), (sl_real)(ps.rcPaint.bottom));
						Ref<Canvas> graphics = UIPlatform::createCanvas(_graphics, rect.right, rect.bottom, &rcDirty);
						if (graphics.isNotNull()) {
							_graphics->SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);
							onDraw(graphics.getObject());
						}
					}
					::EndPaint(hWnd, &ps);
				}
				result = TRUE;
				return sl_true;
			}

		case WM_KEYDOWN:
			{
				onEventKey(sl_true, wParam, lParam);
				return sl_true;
			}
		case WM_KEYUP:
			{
				onEventKey(sl_false, wParam, lParam);
				return sl_true;
			}
		case WM_LBUTTONDOWN:
			{
				onEventMouse(actionLeftButtonDown, wParam, lParam);
				m_actionMouseCapture = actionLeftButtonDown;
				::SetCapture(hWnd);
				return sl_true;
			}
		case WM_LBUTTONDBLCLK:
			{
				onEventMouse(actionLeftButtonDoubleClick, wParam, lParam);
				return sl_true;
			}
		case WM_LBUTTONUP:
			{
				onEventMouse(actionLeftButtonUp, wParam, lParam);
				::ReleaseCapture();
				return sl_true;
			}
		case WM_RBUTTONDOWN:
			{
				onEventMouse(actionRightButtonDown, wParam, lParam);
				m_actionMouseCapture = actionRightButtonDown;
				::SetCapture(hWnd);
				return sl_true;
			}
		case WM_RBUTTONDBLCLK:
			{
				onEventMouse(actionRightButtonDoubleClick, wParam, lParam);
				return sl_true;
			}
		case WM_RBUTTONUP:
			{
				onEventMouse(actionRightButtonUp, wParam, lParam);
				::ReleaseCapture();
				return sl_true;
			}
		case WM_MBUTTONDOWN:
			{
				onEventMouse(actionMiddleButtonDown, wParam, lParam);
				m_actionMouseCapture = actionMiddleButtonDown;
				::SetCapture(hWnd);
				return sl_true;
			}
		case WM_MBUTTONDBLCLK:
			{
				onEventMouse(actionMiddleButtonDoubleClick, wParam, lParam);
				return sl_true;
			}
		case WM_MBUTTONUP:
			{
				onEventMouse(actionMiddleButtonUp, wParam, lParam);
				::ReleaseCapture();
				return sl_true;
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
					onEventMouse(actionMouseEnter, wParam, lParam);
				}

				if (::GetCapture() == hWnd) {
					if (m_actionMouseCapture == actionLeftButtonDown) {
						onEventMouse(actionLeftButtonDrag, wParam, lParam);
					} else if (m_actionMouseCapture == actionRightButtonDown) {
						onEventMouse(actionRightButtonDrag, wParam, lParam);
					} else if (m_actionMouseCapture == actionMiddleButtonDown) {
						onEventMouse(actionMiddleButtonDrag, wParam, lParam);
					} else {
						onEventMouse(actionMouseMove, wParam, lParam);
					}
				} else {
					onEventMouse(actionMouseMove, wParam, lParam);
				}
				return sl_true;
			}
		case WM_MOUSELEAVE:
			{
				onEventMouse(actionMouseLeave, wParam, lParam);
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

LRESULT CALLBACK _Win32_ViewWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
					if (! (instance->processControlColor(uMsg, hDC, result))) {
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

LRESULT CALLBACK _Win32_ViewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Ref<Win32_ViewInstance> instance = Ref<Win32_ViewInstance>::from(UIPlatform::getViewInstance(hWnd));
	if (instance.isNotNull()) {
		LRESULT result = 0;
		if (instance->processWindowMessage(uMsg, wParam, lParam, result)) {
			return result;
		}
	}
	return _Win32_ViewWindowProc(hWnd, uMsg, wParam, lParam);
}

/******************************************
				View
******************************************/
Ref<ViewInstance> View::createInstance(ViewInstance* parent)
{
	Win32_View_Shared* shared = Win32_View_Shared::get();
	DWORD styleEx = 0;
	DWORD style = 0;
	if (m_flagGroup) {
		styleEx = WS_EX_CONTROLPARENT;
#if defined(_SLIB_UI_WIN32_USE_COMPOSITE_VIEWS)
		styleEx |= WS_EX_COMPOSITED;
#endif
#if defined(_SLIB_UI_WIN32_USE_CLIP_CHILDREN)
		style |= (WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
#endif
	}
	Ref<Win32_ViewInstance> ret = Win32_ViewInstance::create<Win32_ViewInstance>(this, parent, (LPCWSTR)((LONG_PTR)(shared->wndClass)), L"", style, styleEx);
	return Ref<ViewInstance>::from(ret);
}

sl_bool View::_isValid()
{
	HWND hWnd = UIPlatform::getViewHandle(this);
	if (hWnd) {
		if (::IsWindow(hWnd)) {
			return sl_true;
		}
	}
	return sl_false;
}

void View::_setFocus()
{
	HWND hWnd = UIPlatform::getViewHandle(this);
	if (hWnd) {
		::SetFocus(hWnd);
	}
}

void View::_invalidate()
{
	HWND hWnd = UIPlatform::getViewHandle(this);
	if (hWnd) {
		::InvalidateRect(hWnd, NULL, TRUE);
	}
}

void View::_invalidate(const Rectangle& rect)
{
	HWND hWnd = UIPlatform::getViewHandle(this);
	if (hWnd) {
		RECT rc;
		rc.left = (int)(rect.left);
		rc.top = (int)(rect.top);
		rc.right = (int)(rect.right);
		rc.bottom = (int)(rect.bottom);
		::InvalidateRect(hWnd, &rc, TRUE);
	}
}

Rectangle View::getInstanceFrame()
{
	HWND hWnd = UIPlatform::getViewHandle(this);
	if (hWnd) {
		RECT rc;
		::GetWindowRect(hWnd, &rc);
		Rectangle ret;
		ret.left = (sl_real)rc.left;
		ret.top = (sl_real)rc.top;
		ret.right = (sl_real)rc.right;
		ret.bottom = (sl_real)rc.bottom;
	}
	return Rectangle::zero();
}

void View::_setFrame(const Rectangle& frame)
{
	HWND hWnd = UIPlatform::getViewHandle(this);
	if (hWnd) {
		::SetWindowPos(hWnd, NULL
			, (int)(frame.left), (int)(frame.top)
			, (int)(frame.getWidth()), (int)(frame.getHeight())
			, SWP_NOREPOSITION | SWP_NOZORDER | SWP_NOACTIVATE | SWP_ASYNCWINDOWPOS);
	}
}

void View::_setVisible(sl_bool flag)
{
	HWND hWnd = UIPlatform::getViewHandle(this);
	if (hWnd) {
		sl_bool f1 = ::IsWindowVisible(hWnd) ? sl_true : sl_false;
		sl_bool f2 = flag ? sl_true : sl_false;
		if (f1 != f2) {
			if (f2) {
				::ShowWindowAsync(hWnd, SW_SHOW);
			} else {
				::ShowWindowAsync(hWnd, SW_HIDE);
			}
		}
	}
}

void View::_setEnabled(sl_bool flag)
{
	HWND hWnd = UIPlatform::getViewHandle(this);
	if (hWnd) {
		sl_bool f1 = ::IsWindowEnabled(hWnd) ? sl_true : sl_false;
		sl_bool f2 = flag ? sl_true : sl_false;
		if (f1 != f2) {
			::EnableWindow(hWnd, f2);
		}
	}
}

void View::_setOpaque(sl_bool flag)
{
}

Point View::_convertCoordinateFromScreenToView(const Point& ptScreen)
{
	HWND hWnd = UIPlatform::getViewHandle(this);
	if (hWnd) {
		POINT pt;
		pt.x = (LONG)(ptScreen.x);
		pt.y = (LONG)(ptScreen.y);
		::ScreenToClient(hWnd, &pt);
		return Point((sl_real)(pt.x), (sl_real)(pt.y));
	}
	return ptScreen;
}

Point View::_convertCoordinateFromViewToScreen(const Point& ptView)
{
	HWND hWnd = UIPlatform::getViewHandle(this);
	if (hWnd) {
		POINT pt;
		pt.x = (LONG)(ptView.x);
		pt.y = (LONG)(ptView.y);
		::ClientToScreen(hWnd, &pt);
		return Point((sl_real)(pt.x), (sl_real)(pt.y));
	}
	return ptView;
}

void View::_addChildInstance(ViewInstance* _child)
{
	HWND hWnd = UIPlatform::getViewHandle(this);
	if (hWnd) {
		Win32_ViewInstance* child = (Win32_ViewInstance*)(_child);
		if (child) {
			HWND hWndChild = child->getHandle();
			if (hWndChild) {
				::SetParent(hWndChild, hWnd);
			}
		}
	}
}

void View::_removeChildInstance(ViewInstance* _child)
{
	Win32_ViewInstance* child = (Win32_ViewInstance*)(_child);
	HWND hWnd = child->getHandle();
	if (hWnd) {
		::SetParent(hWnd, HWND_MESSAGE);
	}
}
SLIB_UI_NAMESPACE_END

/******************************************
			UIPlatform
******************************************/
SLIB_UI_NAMESPACE_BEGIN
Ref<ViewInstance> UIPlatform::createViewInstance(HWND hWnd, sl_bool flagDestroyOnRelease)
{
	Ref<ViewInstance> ret = UIPlatform::_getViewInstance((void*)hWnd);
	if (ret.isNotNull()) {
		return ret;
	}
	ret = Ref<ViewInstance>::from(Win32_ViewInstance::create<Win32_ViewInstance>(hWnd, flagDestroyOnRelease));
	return Ref<ViewInstance>::from(ret);
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
		Win32_ViewInstance* instance = (Win32_ViewInstance*)(_instance.get());
		if (instance) {
			return instance->getHandle();
		}
	}
	return 0;
}
SLIB_UI_NAMESPACE_END

#endif
