#include "../../../inc/slib/ui/event.h"

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/core/hashtable.h"
#include "../../../inc/slib/core/log.h"

SLIB_UI_NAMESPACE_BEGIN

TouchPoint::TouchPoint(const Point& _point) : point(_point), pressure(0)
{
}

TouchPoint::TouchPoint(const Point& _point, sl_real _pressure) : point(_point), pressure(_pressure)
{
}

TouchPoint::TouchPoint(sl_real x, sl_real y) : point(x, y), pressure(0)
{
}

TouchPoint::TouchPoint(sl_real x, sl_real y, sl_real _pressure) : point(x, y), pressure(_pressure)
{
}


enum
{
	flagShiftKey = 0x1,
	flagAltKey = 0x2,
	flagOptionKey = 0x2, // (mac)
	flagControlKey = 0x4,
	flagWindowsKey = 0x8,
	flagCommandKey = 0x8, // (mac)
	
	flagPreventDefault = 0x10000,
	flagStopPropagation = 0x20000
};

UIEvent::UIEvent()
{
	m_flags = 0;
	m_action = UIAction::Unknown;
}

Ref<UIEvent> UIEvent::createKeyEvent(UIAction action, Keycode keycode, sl_uint32 systemKeycode)
{
	Ref<UIEvent> ret = new UIEvent;
	if (ret.isNotNull()) {
		ret->setAction(action);
		ret->setKeycode(keycode);
		ret->setSystemKeycode(systemKeycode);
	}
	return ret;
}

Ref<UIEvent> UIEvent::createMouseEvent(UIAction action, sl_real x, sl_real y)
{
	Ref<UIEvent> ret = new UIEvent;
	if (ret.isNotNull()) {
		ret->setAction(action);
		ret->setX(x);
		ret->setY(y);
	}
	return ret;
}

Ref<UIEvent> UIEvent::createMouseWheelEvent(sl_real deltaX, sl_real deltaY)
{
	Ref<UIEvent> ret = new UIEvent;
	if (ret.isNotNull()) {
		ret->setAction(UIAction::MouseWheel);
		ret->setDeltaX(deltaX);
		ret->setDeltaY(deltaY);
	}
	return ret;
}

Ref<UIEvent> UIEvent::createTouchEvent(UIAction action, const Array<TouchPoint>& points)
{
	Ref<UIEvent> ret = new UIEvent;
	if (ret.isNotNull()) {
		ret->setAction(action);
		ret->setTouchPoints(points);
		if (points.getCount() > 0) {
			ret->setTouchPoint((points.getData())[0]);
		}
	}
	return ret;
}

Ref<UIEvent> UIEvent::createSetCursorEvent()
{
	Ref<UIEvent> ret = new UIEvent;
	if (ret.isNotNull()) {
		ret->setAction(UIAction::SetCursor);
	}
	return ret;
}

UIAction UIEvent::getAction() const
{
	return m_action;
}

void UIEvent::setAction(UIAction action)
{
	m_action = action;
}

sl_bool UIEvent::isKeyEvent()
{
	return ((sl_uint32)m_action & 0xff00) == 0x0100;
}

sl_bool UIEvent::isMouseEvent()
{
	return ((sl_uint32)m_action & 0xff00) == 0x0200;
}

sl_bool UIEvent::isTouchEvent()
{
	return ((sl_uint32)m_action & 0xff00) == 0x0300;
}

Keycode UIEvent::getKeycode() const
{
	return m_keycode;
}

void UIEvent::setKeycode(Keycode keycode)
{
	m_keycode = keycode;
}

sl_uint32 UIEvent::getSystemKeycode() const
{
	return m_systemKeycode;
}

void UIEvent::setSystemKeycode(sl_uint32 keycode)
{
	m_systemKeycode = keycode;
}

const Point& UIEvent::getPoint() const
{
	return m_point.point;
}

void UIEvent::setPoint(const Point& pt)
{
	m_point.point = pt;
}

void UIEvent::setPoint(sl_real x, sl_real y)
{
	m_point.point.x = x;
	m_point.point.y = y;
}

sl_real UIEvent::getX() const
{
	return m_point.point.x;
}

void UIEvent::setX(sl_real x)
{
	m_point.point.x = x;
}

sl_real UIEvent::getY() const
{
	return m_point.point.y;
}

void UIEvent::setY(sl_real y)
{
	m_point.point.y = y;
}

sl_real UIEvent::getDelta() const
{
	return m_point.point.y;
}

void UIEvent::setDelta(sl_real delta)
{
	m_point.point.y = delta;
}

sl_real UIEvent::getDeltaX() const
{
	return m_point.point.x;
}

void UIEvent::setDeltaX(sl_real x)
{
	m_point.point.x = x;
}

sl_real UIEvent::getDeltaY() const
{
	return m_point.point.y;
}

void UIEvent::setDeltaY(sl_real y)
{
	m_point.point.y = y;
}

const TouchPoint& UIEvent::getTouchPoint() const
{
	return m_point;
}

void UIEvent::setTouchPoint(const TouchPoint& pt)
{
	m_point = pt;
}

void UIEvent::setTouchPoint(const Point& pt)
{
	m_point.point = pt;
	m_point.pressure = 0;
}

void UIEvent::setTouchPoint(const Point& pt, sl_real pressure)
{
	m_point.point = pt;
	m_point.pressure = pressure;
}

void UIEvent::setTouchPoint(sl_real x, sl_real y)
{
	m_point.point.x = x;
	m_point.point.y = y;
	m_point.pressure = 0;
}

void UIEvent::setTouchPoint(sl_real x, sl_real y, sl_real pressure)
{
	m_point.point.x = x;
	m_point.point.y = y;
	m_point.pressure = pressure;
}

sl_real UIEvent::getPressure() const
{
	return m_point.pressure;
}

void UIEvent::setPressure(sl_real pressure)
{
	m_point.pressure = pressure;
}

Array<TouchPoint> UIEvent::getTouchPoints() const
{
	return m_points;
}

sl_uint32 UIEvent::getTouchPointsCount() const
{
	return (sl_uint32)(m_points.getCount());
}

TouchPoint UIEvent::getTouchPoint(sl_uint32 index) const
{
	TouchPoint pt;
	m_points.getItem(index, &pt);
	return pt;
}

void UIEvent::setTouchPoints(const Array<TouchPoint>& points)
{
	m_points = points;
}

void UIEvent::transformPoints(const Matrix3& mat)
{
	if (isMouseEvent() || isTouchEvent()) {
		m_point.point = mat.transformPosition(m_point.point);
	}
	if (isTouchEvent()) {
		Array<TouchPoint> points = m_points;
		sl_size n = points.getCount();
		TouchPoint* pts = points.getData();
		for (sl_size i = 0; i < n; i++) {
			pts[i].point = mat.transformPosition(pts[i].point);
		}
	}
}

void UIEvent::setShiftKey()
{
	SLIB_SET_FLAG(m_flags, (sl_uint32)flagShiftKey);
}

void UIEvent::clearShiftKey()
{
	SLIB_RESET_FLAG(m_flags, (sl_uint32)flagShiftKey);
}

sl_bool UIEvent::isShiftKey() const
{
	return SLIB_CHECK_FLAG(m_flags, (sl_uint32)flagShiftKey);
}


void UIEvent::setAltKey()
{
	SLIB_SET_FLAG(m_flags, (sl_uint32)flagAltKey);
}

void UIEvent::clearAltKey()
{
	SLIB_RESET_FLAG(m_flags, (sl_uint32)flagAltKey);
}

sl_bool UIEvent::isAltKey() const
{
	return SLIB_CHECK_FLAG(m_flags, (sl_uint32)flagAltKey);
}

void UIEvent::setOptionKey()
{
	SLIB_SET_FLAG(m_flags, (sl_uint32)flagOptionKey);
}

void UIEvent::clearOptionKey()
{
	SLIB_RESET_FLAG(m_flags, (sl_uint32)flagOptionKey);
}

sl_bool UIEvent::isOptionKey() const
{
	return SLIB_CHECK_FLAG(m_flags, (sl_uint32)flagOptionKey);
}

void UIEvent::setControlKey()
{
	SLIB_SET_FLAG(m_flags, (sl_uint32)flagControlKey);
}

void UIEvent::clearControlKey()
{
	SLIB_RESET_FLAG(m_flags, (sl_uint32)flagControlKey);
}

sl_bool UIEvent::isControlKey() const
{
	return SLIB_CHECK_FLAG(m_flags, (sl_uint32)flagControlKey);
}

void UIEvent::setWindowsKey()
{
	SLIB_SET_FLAG(m_flags, (sl_uint32)flagWindowsKey);
}

void UIEvent::clearWindowsKey()
{
	SLIB_RESET_FLAG(m_flags, (sl_uint32)flagWindowsKey);
}

sl_bool UIEvent::isWindowsKey() const
{
	return SLIB_CHECK_FLAG(m_flags, (sl_uint32)flagWindowsKey);
}

void UIEvent::setCommandKey()
{
	SLIB_SET_FLAG(m_flags, (sl_uint32)flagCommandKey);
}

void UIEvent::clearCommandKey()
{
	SLIB_RESET_FLAG(m_flags, (sl_uint32)flagCommandKey);
}

sl_bool UIEvent::isCommandKey() const
{
	return SLIB_CHECK_FLAG(m_flags, (sl_uint32)flagCommandKey);
}

void UIEvent::preventDefault()
{
	SLIB_SET_FLAG(m_flags, (sl_uint32)flagPreventDefault);
}

sl_bool UIEvent::isPreventedDefault() const
{
	return SLIB_CHECK_FLAG(m_flags, (sl_uint32)flagPreventDefault);
}

void UIEvent::stopPropagation()
{
	SLIB_SET_FLAG(m_flags, (sl_uint32)flagStopPropagation);
}

sl_bool UIEvent::isStoppedPropagation() const
{
	return SLIB_CHECK_FLAG(m_flags, (sl_uint32)flagStopPropagation);
}

Ref<UIEvent> UIEvent::duplicate()
{
	Ref<UIEvent> ret = new UIEvent();
	if (ret.isNotNull()) {
		ret->m_flags = m_flags;
		ret->m_action = m_action;
		ret->m_keycode = m_keycode;
		ret->m_systemKeycode = m_systemKeycode;
		ret->m_point = m_point;
		ret->m_points = m_points.duplicate();
	}
	return ret;
}

class _UIKeyNameMapper
{
private:
	HashTable<sl_uint32, String> map;
	String nameInvalid;
	
public:

#define _MAP_KEY(A, B) \
	{ \
		SLIB_STATIC_STRING(_s, B); \
		map.put((sl_uint32)(A), _s); \
	}

	_UIKeyNameMapper()
	{
		SLIB_STATIC_STRING(_invalid, "Invalid");
		nameInvalid = _invalid;
		_MAP_KEY(Keycode::Unknown, "Unknown");
		
		_MAP_KEY(Keycode::Backspace, "Backspace");
		_MAP_KEY(Keycode::Tab, "Tab");
		_MAP_KEY(Keycode::Enter, "Enter");
		_MAP_KEY(Keycode::Escape, "Escape");
		
		_MAP_KEY(Keycode::Space, "Space");
		_MAP_KEY(Keycode::Grave, "Grave");
		_MAP_KEY(Keycode::Equal, "Equal");
		_MAP_KEY(Keycode::Semicolon, "Semicolon");
		_MAP_KEY(Keycode::Backslash, "Backslash");
		_MAP_KEY(Keycode::LeftBaracket, "LeftBaracket");
		_MAP_KEY(Keycode::RightBaracket, "RightBaracket");
		_MAP_KEY(Keycode::Quote, "Quote");
		_MAP_KEY(Keycode::Comma, "Comma");
		_MAP_KEY(Keycode::Minus, "Minus");
		_MAP_KEY(Keycode::Period, "Period");
		_MAP_KEY(Keycode::Divide, "Divide");
		
		_MAP_KEY(Keycode::Num0, "0");
		_MAP_KEY(Keycode::Num1, "1");
		_MAP_KEY(Keycode::Num2, "2");
		_MAP_KEY(Keycode::Num3, "3");
		_MAP_KEY(Keycode::Num4, "4");
		_MAP_KEY(Keycode::Num5, "5");
		_MAP_KEY(Keycode::Num6, "6");
		_MAP_KEY(Keycode::Num7, "7");
		_MAP_KEY(Keycode::Num8, "8");
		_MAP_KEY(Keycode::Num9, "9");
		
		_MAP_KEY(Keycode::A, "A");
		_MAP_KEY(Keycode::B, "B");
		_MAP_KEY(Keycode::C, "C");
		_MAP_KEY(Keycode::D, "D");
		_MAP_KEY(Keycode::E, "E");
		_MAP_KEY(Keycode::F, "F");
		_MAP_KEY(Keycode::G, "G");
		_MAP_KEY(Keycode::H, "H");
		_MAP_KEY(Keycode::I, "I");
		_MAP_KEY(Keycode::J, "J");
		_MAP_KEY(Keycode::K, "K");
		_MAP_KEY(Keycode::L, "L");
		_MAP_KEY(Keycode::M, "M");
		_MAP_KEY(Keycode::N, "N");
		_MAP_KEY(Keycode::O, "O");
		_MAP_KEY(Keycode::P, "P");
		_MAP_KEY(Keycode::Q, "Q");
		_MAP_KEY(Keycode::R, "R");
		_MAP_KEY(Keycode::S, "S");
		_MAP_KEY(Keycode::T, "T");
		_MAP_KEY(Keycode::U, "U");
		_MAP_KEY(Keycode::V, "V");
		_MAP_KEY(Keycode::W, "W");
		_MAP_KEY(Keycode::X, "X");
		_MAP_KEY(Keycode::Y, "Y");
		_MAP_KEY(Keycode::Z, "Z");
		
		_MAP_KEY(Keycode::Numpad0, "Numpad0");
		_MAP_KEY(Keycode::Numpad1, "Numpad1");
		_MAP_KEY(Keycode::Numpad2, "Numpad2");
		_MAP_KEY(Keycode::Numpad3, "Numpad3");
		_MAP_KEY(Keycode::Numpad4, "Numpad4");
		_MAP_KEY(Keycode::Numpad5, "Numpad5");
		_MAP_KEY(Keycode::Numpad6, "Numpad6");
		_MAP_KEY(Keycode::Numpad7, "Numpad7");
		_MAP_KEY(Keycode::Numpad8, "Numpad8");
		_MAP_KEY(Keycode::Numpad9, "Numpad9");
		
		_MAP_KEY(Keycode::NumpadDivide, "NumpadDivide");
		_MAP_KEY(Keycode::NumpadMultiply, "NumpadMultiply");
		_MAP_KEY(Keycode::NumpadMinus, "NumpadMinus");
		_MAP_KEY(Keycode::NumpadPlus, "NumpadPlus");
		_MAP_KEY(Keycode::NumpadEnter, "NumpadEnter");
		_MAP_KEY(Keycode::NumpadDecimal, "NumpadDecimal");
		
		_MAP_KEY(Keycode::F1, "F1");
		_MAP_KEY(Keycode::F2, "F2");
		_MAP_KEY(Keycode::F3, "F3");
		_MAP_KEY(Keycode::F4, "F4");
		_MAP_KEY(Keycode::F5, "F5");
		_MAP_KEY(Keycode::F6, "F6");
		_MAP_KEY(Keycode::F7, "F7");
		_MAP_KEY(Keycode::F8, "F8");
		_MAP_KEY(Keycode::F9, "F9");
		_MAP_KEY(Keycode::F10, "F10");
		_MAP_KEY(Keycode::F11, "F11");
		_MAP_KEY(Keycode::F12, "F12");
		
		_MAP_KEY(Keycode::PageUp, "PageUp");
		_MAP_KEY(Keycode::PageDown, "PageDown");
		_MAP_KEY(Keycode::Home, "Home");
		_MAP_KEY(Keycode::End, "End");
		_MAP_KEY(Keycode::Left, "Left");
		_MAP_KEY(Keycode::Up, "Up");
		_MAP_KEY(Keycode::Right, "Right");
		_MAP_KEY(Keycode::Down, "Down");
		_MAP_KEY(Keycode::PrintScreen, "PrintScreen");
		_MAP_KEY(Keycode::Insert, "Insert");
		_MAP_KEY(Keycode::Delete, "Delete");
		_MAP_KEY(Keycode::Sleep, "Sleep");
		_MAP_KEY(Keycode::Pause, "Pause");
		
		_MAP_KEY(Keycode::GoHome, "GoHome");
		_MAP_KEY(Keycode::GoMenu, "GoMenu");
		_MAP_KEY(Keycode::GoBack, "GoBack");
		_MAP_KEY(Keycode::Camera, "Camera");
		_MAP_KEY(Keycode::VolumeMute, "VolumeMute");
		_MAP_KEY(Keycode::VolumeDown, "VolumeDown");
		_MAP_KEY(Keycode::VolumeUp, "VolumeUp");
		_MAP_KEY(Keycode::MediaPrev, "MediaPrev");
		_MAP_KEY(Keycode::MediaNext, "MediaNext");
		_MAP_KEY(Keycode::MediaPause, "MediaPause");
		_MAP_KEY(Keycode::MediaStop, "MediaStop");
		_MAP_KEY(Keycode::PhoneStar, "Dial*");
		_MAP_KEY(Keycode::PhonePound, "Dial#");

		_MAP_KEY(Keycode::LeftShift, "LeftShift");
		_MAP_KEY(Keycode::RightShift, "RightShift");
		_MAP_KEY(Keycode::LeftControl, "LeftControl");
		_MAP_KEY(Keycode::RightControl, "RightControl");
#if defined(SLIB_PLATFORM_IS_OSX)
		_MAP_KEY(Keycode::LeftOption, "LeftOption");
		_MAP_KEY(Keycode::RightOption, "RightOption");
		_MAP_KEY(Keycode::LeftCommand, "LeftCommand");
		_MAP_KEY(Keycode::RightCommand, "RightCommand");
#else
		_MAP_KEY(Keycode::LeftAlt, "LeftAlt");
		_MAP_KEY(Keycode::RightAlt, "RightAlt");
		_MAP_KEY(Keycode::LeftWin, "LeftWin");
		_MAP_KEY(Keycode::RightWin, "RightWin");
#endif
		_MAP_KEY(Keycode::CapsLock, "CapsLock");
		_MAP_KEY(Keycode::ScrollLock, "ScrollLock");
		_MAP_KEY(Keycode::NumLock, "NumLock");
		_MAP_KEY(Keycode::ContextMenu, "ContextMenu");

	}
	
	String get(Keycode code)
	{
		String ret;
		if (map.get((sl_uint32)code, &ret)) {
			return ret;
		} else {
			return nameInvalid;
		}
	}
};

String UI::getKeyName(Keycode code)
{
	SLIB_SAFE_STATIC(_UIKeyNameMapper, mapper);
	return mapper.get(code);
}


void IViewListener::onClick(View* view)
{
}

void IViewListener::onKeyEvent(View* view, UIEvent* event)
{
}

void IViewListener::onMouseEvent(View* view, UIEvent* event)
{
}

void IViewListener::onTouchEvent(View* view, UIEvent* event)
{
}

void IViewListener::onMouseWheelEvent(View* view, UIEvent* event)
{
}

void IViewListener::onSetCursor(View* view, UIEvent* event)
{
}


sl_bool IWindowListener::onClose(Window* window)
{
	return sl_true;
}

void IWindowListener::onActivate(Window* window)
{
}

void IWindowListener::onDeactivate(Window* window)
{
}

void IWindowListener::onMove(Window* window)
{
}

void IWindowListener::onResize(Window* window, Size& size)
{
}

void IWindowListener::onMinimize(Window* window)
{
}

void IWindowListener::onDeminimize(Window* window)
{
}

void IWindowListener::onMaximize(Window* window)
{
}

void IWindowListener::onDemaximize(Window* window)
{
}



// UIEventLogListener implementation
void UIEventLogListener::onClick(View* view)
{
	SLIB_LOG("View", "onClick");
}

void UIEventLogListener::onKeyEvent(View* view, UIEvent* event)
{
	processKey("View", event);
}

void UIEventLogListener::onMouseEvent(View* view, UIEvent* event)
{
	processMouse("View", event);
}

void UIEventLogListener::onTouchEvent(View* view, UIEvent* event)
{
	processTouch("View", event);
}

void UIEventLogListener::onMouseWheelEvent(View* view, UIEvent* event)
{
	processMouseWheel("View", event);
}

void UIEventLogListener::onSetCursor(View* view, UIEvent* event)
{
	SLIB_LOG("View", "OnSetCursor");
}

sl_bool UIEventLogListener::onClose(Window* window)
{
	SLIB_LOG("Window", "OnClose");
	return sl_true;
}

void UIEventLogListener::onActivate(Window* window)
{
	SLIB_LOG("Window", "onActivate");
}

void UIEventLogListener::onDeactivate(Window* window)
{
	SLIB_LOG("Window", "onDeactivate");
}

void UIEventLogListener::onMove(Window* window)
{
	SLIB_LOG("Window", "onMove");
}

void UIEventLogListener::onResize(Window* window, Size& size)
{
	SLIB_LOG("Window", "onResize (" + String::fromFloat(size.x) + "," + String::fromFloat(size.y) + ")");
}

void UIEventLogListener::onMinimize(Window* window)
{
	SLIB_LOG("Window", "onMinimize");
}

void UIEventLogListener::onDeminimize(Window* window)
{
	SLIB_LOG("Window", "onDeminimize");
}

void UIEventLogListener::onMaximize(Window* window)
{
	SLIB_LOG("Window", "onMaximize");
}

void UIEventLogListener::onDemaximize(Window* window)
{
	SLIB_LOG("Window", "onDemaximize");
}

String UIEventLogListener::getModifierText(UIEvent* event)
{
	String ret;
	if (event->isShiftKey()) {
		ret += "SHIFT ";
	}
	if (event->isControlKey()) {
		ret += "CTRL ";
	}
	if (event->isWindowsKey()) {
		ret += "WIN ";
	}
	if (event->isAltKey()) {
		ret += "ALT ";
	}
	if (ret.getLength() > 0) {
		ret = "[" + ret.trim() + "]";
	}
	return ret;
}

void UIEventLogListener::processKey(String name, UIEvent* event)
{
	String str;
	if (event->getAction() == UIAction::KeyDown) {
		str = "KeyDown ";
	} else {
		str = "KeyUp ";
	}
	str += UI::getKeyName(event->getKeycode());
	
	str += " VK-" + String::fromInt32(event->getSystemKeycode());
	str += " " + getModifierText(event);
	SLIB_LOG(name, str);
}

void UIEventLogListener::processMouse(String name, UIEvent* event)
{
	String str;
	switch (event->getAction()) {
		case UIAction::LeftButtonDown:
			str = "LButtonDown";
			break;
		case UIAction::LeftButtonUp:
			str = "LButtonUp";
			break;
		case UIAction::LeftButtonDrag:
			str = "LButtonDrag";
			break;
		case UIAction::LeftButtonDoubleClick:
			str = "LButtonDbl";
			break;
		case UIAction::RightButtonDown:
			str = "RButtonDown";
			break;
		case UIAction::RightButtonUp:
			str = "RButtonUp";
			break;
		case UIAction::RightButtonDrag:
			str = "RButtonDrag";
			break;
		case UIAction::RightButtonDoubleClick:
			str = "RButtonDbl";
			break;
		case UIAction::MiddleButtonDown:
			str = "MButtonDown";
			break;
		case UIAction::MiddleButtonUp:
			str = "MButtonUp";
			break;
		case UIAction::MiddleButtonDrag:
			str = "MButtonDrag";
			break;
		case UIAction::MiddleButtonDoubleClick:
			str = "MButtonDbl";
			break;
		case UIAction::MouseMove:
			str = "MouseMove";
			break;
		case UIAction::MouseEnter:
			str = "MouseEnter";
			break;
		case UIAction::MouseLeave:
			str = "MouseLeave";
			break;
		default:
			return;
	}
	str += " (" + String::fromInt32((sl_int32)(event->getX())) + "," + String::fromInt32((sl_int32)(event->getY())) + ")";
	str += " " + getModifierText(event);
	SLIB_LOG(name, str);
}

void UIEventLogListener::processTouch(String name, UIEvent* event)
{
	String str;
	switch (event->getAction()) {
		case UIAction::TouchBegin:
			str = "TouchDown";
			break;
		case UIAction::TouchMove:
			str = "TouchMove";
			break;
		case UIAction::TouchEnd:
			str = "TouchUp";
			break;
		case UIAction::TouchCancel:
			str = "TouchCancel";
			break;
		default:
			return;
	}
	str += " (" + String::fromInt32((sl_int32)(event->getX())) + "," + String::fromInt32((sl_int32)(event->getY())) + ", Pressure-" + String::fromFloat(event->getPressure()) + ")";
	SLIB_LOG(name, str);
}

void UIEventLogListener::processMouseWheel(String name, UIEvent* event)
{
	String str = "MouseWheel (" + String::fromFloat(event->getDeltaX()) + "," + String::fromFloat(event->getDeltaY());
	str += ") " + getModifierText(event);
	SLIB_LOG(name, str);
}

String UI::getStatusDescription()
{
	String str;
	if (UI::checkKeyPressed(Keycode::LeftShift)) {
		str += "LSHIFT ";
	}
	if (UI::checkKeyPressed(Keycode::RightShift)) {
		str += "RSHIFT ";
	}
	if (UI::checkKeyPressed(Keycode::LeftControl)) {
		str += "LCTRL ";
	}
	if (UI::checkKeyPressed(Keycode::RightControl)) {
		str += "RCTRL ";
	}
	if (UI::checkKeyPressed(Keycode::LeftCommand)) {
		str += "LWIN ";
	}
	if (UI::checkKeyPressed(Keycode::RightCommand)) {
		str += "RWIN ";
	}
	if (UI::checkKeyPressed(Keycode::LeftAlt)) {
		str += "LALT ";
	}
	if (UI::checkKeyPressed(Keycode::RightAlt)) {
		str += "RALT ";
	}
	if (UI::checkCapsLockOn()) {
		str += "CAPS ";
	}
	if (UI::checkScrollLockOn()) {
		str += "SCROLL ";
	}
	if (UI::checkNumLockOn()) {
		str += "NUM ";
	}
	if (UI::checkLeftButtonPressed()) {
		str += "LBUTTON ";
	}
	if (UI::checkRightButtonPressed()) {
		str += "RBUTTON ";
	}
	if (UI::checkMiddleButtonPressed()) {
		str += "MBUTTON ";
	}
	Point pt = getCursorPos();
	str += " MOUSE(" + String::fromInt32((sl_int32)(pt.x)) + "," + String::fromInt32((sl_int32)(pt.y)) + ")";
	return str;
}

SLIB_UI_NAMESPACE_END

