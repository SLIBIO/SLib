#include "../../../inc/slib/ui/event.h"
#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/core/hashtable.h"
#include "../../../inc/slib/core/log.h"

SLIB_UI_NAMESPACE_BEGIN
Ref<UIEvent> UIEvent::createKeyEvent(UIEventAction action, Keycode keycode, sl_uint32 systemKeycode)
{
	Ref<UIEvent> ret = new UIEvent;
	if (ret.isNotNull()) {
		ret->setAction(action);
		ret->setKeycode(keycode);
		ret->setSystemKeycode(systemKeycode);
	}
	return ret;
}

Ref<UIEvent> UIEvent::createMouseEvent(UIEventAction action, sl_real x, sl_real y)
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
		ret->setAction(actionMouseWheel);
		ret->setDeltaX(deltaX);
		ret->setDeltaY(deltaY);
	}
	return ret;
}

Ref<UIEvent> UIEvent::createTouchEvent(UIEventAction action, const Array<TouchPoint>& _points)
{
	Ref<UIEvent> ret = new UIEvent;
	if (ret.isNotNull()) {
		ret->setAction(action);
		Array<TouchPoint> points = _points;
		ret->setTouchPoints(points);
		if (points.count() > 0) {
			ret->setTouchPoint(points[0]);
		}
	}
	return ret;
}

Ref<UIEvent> UIEvent::createSetCursorEvent()
{
	Ref<UIEvent> ret = new UIEvent;
	if (ret.isNotNull()) {
		ret->setAction(actionSetCursor);
	}
	return ret;
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

void UIEvent::transformPoints(const Matrix3& mat)
{
	if (isMouseEvent() || isTouchEvent()) {
		m_point.point = mat.transformPosition(m_point.point);
	}
	if (isTouchEvent()) {
		Array<TouchPoint> points = m_points;
		sl_size n = points.count();
		TouchPoint* pts = points.getBuf();
		for (sl_size i = 0; i < n; i++) {
			pts[i].point = mat.transformPosition(pts[i].point);
		}
	}
}

class _UIKeyNameMapper
{
private:
	HashTable<Keycode, String> map;
	String nameInvalid;
	
public:

#define _MAP_KEY(A, B) \
	{ \
		SLIB_STATIC_STRING(_s, B); \
		map.put(A, _s); \
	}

	_UIKeyNameMapper()
	{
		SLIB_STATIC_STRING(_invalid, "Invalid");
		nameInvalid = _invalid;
		_MAP_KEY(keyUnknown, "Unknown");
		
		_MAP_KEY(keyBackspace, "Backspace");
		_MAP_KEY(keyTab, "Tab");
		_MAP_KEY(keyEnter, "Enter");
		_MAP_KEY(keyEscape, "Escape");
		
		_MAP_KEY(keySpace, "Space");
		_MAP_KEY(keyGrave, "Grave");
		_MAP_KEY(keyEqual, "Equal");
		_MAP_KEY(keySemicolon, "Semicolon");
		_MAP_KEY(keyBackslash, "Backslash");
		_MAP_KEY(keyLeftBaracket, "LeftBaracket");
		_MAP_KEY(keyRightBaracket, "RightBaracket");
		_MAP_KEY(keyQuote, "Quote");
		_MAP_KEY(keyComma, "Comma");
		_MAP_KEY(keyMinus, "Minus");
		_MAP_KEY(keyPeriod, "Period");
		_MAP_KEY(keyDivide, "Divide");
		
		_MAP_KEY(key0, "0");
		_MAP_KEY(key1, "1");
		_MAP_KEY(key2, "2");
		_MAP_KEY(key3, "3");
		_MAP_KEY(key4, "4");
		_MAP_KEY(key5, "5");
		_MAP_KEY(key6, "6");
		_MAP_KEY(key7, "7");
		_MAP_KEY(key8, "8");
		_MAP_KEY(key9, "9");
		
		_MAP_KEY(keyA, "A");
		_MAP_KEY(keyB, "B");
		_MAP_KEY(keyC, "C");
		_MAP_KEY(keyD, "D");
		_MAP_KEY(keyE, "E");
		_MAP_KEY(keyF, "F");
		_MAP_KEY(keyG, "G");
		_MAP_KEY(keyH, "H");
		_MAP_KEY(keyI, "I");
		_MAP_KEY(keyJ, "J");
		_MAP_KEY(keyK, "K");
		_MAP_KEY(keyL, "L");
		_MAP_KEY(keyM, "M");
		_MAP_KEY(keyN, "N");
		_MAP_KEY(keyO, "O");
		_MAP_KEY(keyP, "P");
		_MAP_KEY(keyQ, "Q");
		_MAP_KEY(keyR, "R");
		_MAP_KEY(keyS, "S");
		_MAP_KEY(keyT, "T");
		_MAP_KEY(keyU, "U");
		_MAP_KEY(keyV, "V");
		_MAP_KEY(keyW, "W");
		_MAP_KEY(keyX, "X");
		_MAP_KEY(keyY, "Y");
		_MAP_KEY(keyZ, "Z");
		
		_MAP_KEY(keyNumpad0, "Numpad0");
		_MAP_KEY(keyNumpad1, "Numpad1");
		_MAP_KEY(keyNumpad2, "Numpad2");
		_MAP_KEY(keyNumpad3, "Numpad3");
		_MAP_KEY(keyNumpad4, "Numpad4");
		_MAP_KEY(keyNumpad5, "Numpad5");
		_MAP_KEY(keyNumpad6, "Numpad6");
		_MAP_KEY(keyNumpad7, "Numpad7");
		_MAP_KEY(keyNumpad8, "Numpad8");
		_MAP_KEY(keyNumpad9, "Numpad9");
		
		_MAP_KEY(keyNumpadDivide, "NumpadDivide");
		_MAP_KEY(keyNumpadMultiply, "NumpadMultiply");
		_MAP_KEY(keyNumpadMinus, "NumpadMinus");
		_MAP_KEY(keyNumpadPlus, "NumpadPlus");
		_MAP_KEY(keyNumpadEnter, "NumpadEnter");
		_MAP_KEY(keyNumpadDecimal, "NumpadDecimal");
		
		_MAP_KEY(keyF1, "F1");
		_MAP_KEY(keyF2, "F2");
		_MAP_KEY(keyF3, "F3");
		_MAP_KEY(keyF4, "F4");
		_MAP_KEY(keyF5, "F5");
		_MAP_KEY(keyF6, "F6");
		_MAP_KEY(keyF7, "F7");
		_MAP_KEY(keyF8, "F8");
		_MAP_KEY(keyF9, "F9");
		_MAP_KEY(keyF10, "F10");
		_MAP_KEY(keyF11, "F11");
		_MAP_KEY(keyF12, "F12");
		
		_MAP_KEY(keyPageUp, "PageUp");
		_MAP_KEY(keyPageDown, "PageDown");
		_MAP_KEY(keyHome, "Home");
		_MAP_KEY(keyEnd, "End");
		_MAP_KEY(keyLeft, "Left");
		_MAP_KEY(keyUp, "Up");
		_MAP_KEY(keyRight, "Right");
		_MAP_KEY(keyDown, "Down");
		_MAP_KEY(keyPrintScreen, "PrintScreen");
		_MAP_KEY(keyInsert, "Insert");
		_MAP_KEY(keyDelete, "Delete");
		_MAP_KEY(keySleep, "Sleep");
		_MAP_KEY(keyPause, "Pause");
		
		_MAP_KEY(keyGoHome, "GoHome");
		_MAP_KEY(keyGoMenu, "GoMenu");
		_MAP_KEY(keyGoBack, "GoBack");
		_MAP_KEY(keyCamera, "Camera");
		_MAP_KEY(keyVolumeMute, "VolumeMute");
		_MAP_KEY(keyVolumeDown, "VolumeDown");
		_MAP_KEY(keyVolumeUp, "VolumeUp");
		_MAP_KEY(keyMediaPrev, "MediaPrev");
		_MAP_KEY(keyMediaNext, "MediaNext");
		_MAP_KEY(keyMediaPause, "MediaPause");
		_MAP_KEY(keyMediaStop, "MediaStop");
		_MAP_KEY(keyPhoneStar, "Dial*");
		_MAP_KEY(keyPhonePound, "Dial#");

		_MAP_KEY(keyLeftShift, "LeftShift");
		_MAP_KEY(keyRightShift, "RightShift");
		_MAP_KEY(keyLeftControl, "LeftControl");
		_MAP_KEY(keyRightControl, "RightControl");
#if defined(SLIB_PLATFORM_IS_OSX)
		_MAP_KEY(keyLeftOption, "LeftOption");
		_MAP_KEY(keyRightOption, "RightOption");
		_MAP_KEY(keyLeftCommand, "LeftCommand");
		_MAP_KEY(keyRightCommand, "RightCommand");
#else
		_MAP_KEY(keyLeftAlt, "LeftAlt");
		_MAP_KEY(keyRightAlt, "RightAlt");
		_MAP_KEY(keyLeftWin, "LeftWin");
		_MAP_KEY(keyRightWin, "RightWin");
#endif
		_MAP_KEY(keyCapsLock, "CapsLock");
		_MAP_KEY(keyScrollLock, "ScrollLock");
		_MAP_KEY(keyNumLock, "NumLock");
		_MAP_KEY(keyContextMenu, "ContextMenu");

	}
	
	String get(Keycode code)
	{
		String ret;
		if (map.get(code, &ret)) {
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
	if (ret.length() > 0) {
		ret = "[" + ret.trim() + "]";
	}
	return ret;
}

void UIEventLogListener::processKey(String name, UIEvent* event)
{
	String str;
	if (event->getAction() == actionKeyDown) {
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
		case actionLeftButtonDown:
			str = "LButtonDown";
			break;
		case actionLeftButtonUp:
			str = "LButtonUp";
			break;
		case actionLeftButtonDrag:
			str = "LButtonDrag";
			break;
		case actionLeftButtonDoubleClick:
			str = "LButtonDbl";
			break;
		case actionRightButtonDown:
			str = "RButtonDown";
			break;
		case actionRightButtonUp:
			str = "RButtonUp";
			break;
		case actionRightButtonDrag:
			str = "RButtonDrag";
			break;
		case actionRightButtonDoubleClick:
			str = "RButtonDbl";
			break;
		case actionMiddleButtonDown:
			str = "MButtonDown";
			break;
		case actionMiddleButtonUp:
			str = "MButtonUp";
			break;
		case actionMiddleButtonDrag:
			str = "MButtonDrag";
			break;
		case actionMiddleButtonDoubleClick:
			str = "MButtonDbl";
			break;
		case actionMouseMove:
			str = "MouseMove";
			break;
		case actionMouseEnter:
			str = "MouseEnter";
			break;
		case actionMouseLeave:
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
		case actionTouchBegin:
			str = "TouchDown";
			break;
		case actionTouchMove:
			str = "TouchMove";
			break;
		case actionTouchEnd:
			str = "TouchUp";
			break;
		case actionTouchCancel:
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
	if (UI::checkKeyPressed(keyLeftShift)) {
		str += "LSHIFT ";
	}
	if (UI::checkKeyPressed(keyRightShift)) {
		str += "RSHIFT ";
	}
	if (UI::checkKeyPressed(keyLeftControl)) {
		str += "LCTRL ";
	}
	if (UI::checkKeyPressed(keyRightControl)) {
		str += "RCTRL ";
	}
	if (UI::checkKeyPressed(keyLeftCommand)) {
		str += "LWIN ";
	}
	if (UI::checkKeyPressed(keyRightCommand)) {
		str += "RWIN ";
	}
	if (UI::checkKeyPressed(keyLeftAlt)) {
		str += "LALT ";
	}
	if (UI::checkKeyPressed(keyRightAlt)) {
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

Ref<UIEventLogListener> UIEventLogListener::create()
{
	return new UIEventLogListener;
}
SLIB_UI_NAMESPACE_END

