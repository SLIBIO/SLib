/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/ui/event.h"

#include "slib/ui/core.h"
#include "slib/core/hash_table.h"
#include "slib/core/string_buffer.h"
#include "slib/core/log.h"
#include "slib/core/safe_static.h"

namespace slib
{

	TouchPoint::TouchPoint(): pressure(0), phase(TouchPhase::Move)
	{
	}

	TouchPoint::TouchPoint(const TouchPoint& other) = default;

	TouchPoint::TouchPoint(const UIPointf& _point) : point(_point), pressure(0), phase(TouchPhase::Move)
	{
	}

	TouchPoint::TouchPoint(const UIPointf& _point, sl_real _pressure, TouchPhase _phase) : point(_point), pressure(_pressure), phase(_phase)
	{
	}

	TouchPoint::TouchPoint(sl_ui_posf x, sl_ui_posf y) : point(x, y), pressure(0), phase(TouchPhase::Move)
	{
	}

	TouchPoint::TouchPoint(sl_ui_posf x, sl_ui_posf y, sl_real _pressure) : point(x, y), pressure(_pressure), phase(TouchPhase::Move)
	{
	}

	TouchPoint::TouchPoint(sl_ui_posf x, sl_ui_posf y, sl_real _pressure, TouchPhase _phase) : point(x, y), pressure(_pressure), phase(_phase)
	{
	}

	TouchPoint& TouchPoint::operator=(const TouchPoint& other) = default;


#define DEFINE_MODIFIER_FUNCS(NAME) \
void KeycodeAndModifiers::set##NAME##Key() \
{ \
	SLIB_SET_FLAG(value, Modifiers::NAME); \
} \
void KeycodeAndModifiers::clear##NAME##Key() \
{ \
	SLIB_RESET_FLAG(value, Modifiers::NAME); \
} \
sl_bool KeycodeAndModifiers::is##NAME##Key() const \
{ \
	return SLIB_CHECK_FLAG(value, Modifiers::NAME); \
} \
void UIEvent::set##NAME##Key() \
{ \
	SLIB_SET_FLAG(m_keycodeAndModifiers.value, Modifiers::NAME); \
} \
void UIEvent::clear##NAME##Key() \
{ \
	SLIB_RESET_FLAG(m_keycodeAndModifiers.value, Modifiers::NAME); \
} \
sl_bool UIEvent::is##NAME##Key() const \
{ \
	return SLIB_CHECK_FLAG(m_keycodeAndModifiers.value, Modifiers::NAME); \
}

	DEFINE_MODIFIER_FUNCS(Shift)
	DEFINE_MODIFIER_FUNCS(Alt)
	DEFINE_MODIFIER_FUNCS(Option)
	DEFINE_MODIFIER_FUNCS(Control)
	DEFINE_MODIFIER_FUNCS(Windows)
	DEFINE_MODIFIER_FUNCS(Command)

	KeycodeAndModifiers KeycodeAndModifiers::none()
	{
		return 0;
	}

	Keycode KeycodeAndModifiers::getKeycode() const
	{
		return (Keycode)(value & 0xFFFF);
	}

	void KeycodeAndModifiers::setKeycode(Keycode keycode)
	{
		value = (value & Modifiers::Mask) | (int)(keycode);
	}

	Modifiers KeycodeAndModifiers::getModifiers() const
	{
		return value & Modifiers::Mask;
	}

	void KeycodeAndModifiers::setModifiers(const Modifiers& modifiers)
	{
		value = modifiers | (value & 0xFFFF);
	}

	String KeycodeAndModifiers::toString() const
	{
		StringBuffer sb;
		if (isWindowsKey()) {
#if defined(SLIB_PLATFORM_IS_APPLE)
			sb.addStatic("Command+", 8);
#else
			sb.addStatic("Win+", 4);
#endif
		}
		if (isControlKey()) {
			sb.addStatic("Ctrl+", 5);
		}
		if (isShiftKey()) {
			sb.addStatic("Shift+", 6);
		}
		if (isAltKey()) {
#if defined(SLIB_PLATFORM_IS_APPLE)
			sb.addStatic("Option+", 8);
#else
			sb.addStatic("Alt+", 4);
#endif
		}
		sb.add(UI::getKeyName(getKeycode(), sl_true));
		return sb.merge();
	}

	sl_bool KeycodeAndModifiers::parse(const String& str)
	{
		KeycodeAndModifiers km;
		ListElements<String> list(str.split("+"));
		for (sl_size i = 0; i < list.count; i++) {
			if (km.getKeycode() != Keycode::Unknown) {
				return sl_false;
			}
			String s = list[i].toLower();
			if (s == "control" || s == "ctrl") {
				km |= Modifiers::Control;
			} else if (s == "shift") {
				km |= Modifiers::Shift;
			} else if (s == "alt" || s == "option") {
				km |= Modifiers::Alt;
			} else if (s == "command" || s == "window" || s == "win") {
				km |= Modifiers::Windows;
			} else {
				Keycode keycode = UI::getKeycodeFromName(s);
				if (keycode == Keycode::Unknown) {
					return sl_false;
				}
				km.setKeycode(keycode);
			}
		}
		if (km.getKeycode() == Keycode::Unknown) {
			return sl_false;
		}
		*this = km;
		return sl_true;
	}

	KeycodeAndModifiers& KeycodeAndModifiers::operator|=(int modifiers)
	{
		value |= modifiers;
		return *this;
	}

	KeycodeAndModifiers KeycodeAndModifiers::operator|(int modifiers)
	{
		return KeycodeAndModifiers(value | modifiers);
	}

	KeycodeAndModifiers operator|(int modifiers, const KeycodeAndModifiers& km)
	{
		return KeycodeAndModifiers(km.value | modifiers);
	}

	KeycodeAndModifiers operator|(Keycode keycode, int modifiers)
	{
		return KeycodeAndModifiers(keycode, modifiers);
	}

	KeycodeAndModifiers operator|(int modifiers, Keycode keycode)
	{
		return KeycodeAndModifiers(keycode, modifiers);
	}


	enum
	{
		flagPreventDefault = 0x1,
		flagStopPropagation = 0x2,
		flagPassToNext = 0x4,
	};

	UIEvent::UIEvent() : m_flags(0), m_action(UIAction::Unknown), m_time(0), m_systemKeycode(0), m_deltaX(0), m_deltaY(0)
	{
	}

	UIEvent::~UIEvent()
	{
	}

	Ref<UIEvent> UIEvent::create(UIAction action)
	{
		Ref<UIEvent> ret = new UIEvent;
		if (ret.isNotNull()) {
			ret->setAction(action);
		}
		return ret;
	}

	Ref<UIEvent> UIEvent::createKeyEvent(UIAction action, Keycode keycode, sl_uint32 systemKeycode, const Time& time)
	{
		Ref<UIEvent> ret = new UIEvent;
		if (ret.isNotNull()) {
			ret->setAction(action);
			ret->setKeycode(keycode);
			ret->setSystemKeycode(systemKeycode);
			ret->setTime(time);
		}
		return ret;
	}

	Ref<UIEvent> UIEvent::createMouseEvent(UIAction action, sl_ui_posf x, sl_ui_posf y, const Time& time)
	{
		Ref<UIEvent> ret = new UIEvent;
		if (ret.isNotNull()) {
			ret->setAction(action);
			ret->setX(x);
			ret->setY(y);
			ret->setTime(time);
		}
		return ret;
	}

	Ref<UIEvent> UIEvent::createMouseWheelEvent(sl_ui_posf mouseX, sl_ui_posf mouseY, sl_real deltaX, sl_real deltaY, const Time& time)
	{
		Ref<UIEvent> ret = new UIEvent;
		if (ret.isNotNull()) {
			ret->setAction(UIAction::MouseWheel);
			ret->setX(mouseX);
			ret->setY(mouseY);
			ret->setDeltaX(deltaX);
			ret->setDeltaY(deltaY);
			ret->setTime(time);
		}
		return ret;
	}

	Ref<UIEvent> UIEvent::createTouchEvent(UIAction action, const Array<TouchPoint>& points, const Time& time)
	{
		Ref<UIEvent> ret = new UIEvent;
		if (ret.isNotNull()) {
			ret->setAction(action);
			ret->setTouchPoints(points);
			if (points.getCount() > 0) {
				ret->setTouchPoint((points.getData())[0]);
			}
			ret->setTime(time);
		}
		return ret;
	}
	
	Ref<UIEvent> UIEvent::createTouchEvent(UIAction action, const TouchPoint& point, const Time& time)
	{
		Ref<UIEvent> ret = new UIEvent;
		if (ret.isNotNull()) {
			ret->setAction(action);
			ret->setTouchPoint(point);
			ret->setTime(time);
		}
		return ret;
	}

	Ref<UIEvent> UIEvent::createSetCursorEvent(sl_ui_posf x, sl_ui_posf y, const Time& time)
	{
		Ref<UIEvent> ret = new UIEvent;
		if (ret.isNotNull()) {
			ret->setAction(UIAction::SetCursor);
			ret->setPoint(x, y);
			ret->setTime(time);
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

	sl_bool UIEvent::isKeyEvent() const
	{
		return ((sl_uint32)m_action & 0xff00) == 0x0100;
	}

	sl_bool UIEvent::isMouseEvent() const
	{
		return ((sl_uint32)m_action & 0xff00) == 0x0200;
	}

	sl_bool UIEvent::isTouchEvent() const
	{
		return ((sl_uint32)m_action & 0xff00) == 0x0300;
	}

	Time UIEvent::getTime() const
	{
		return m_time;
	}

	void UIEvent::setTime(const Time& time)
	{
		m_time = time;
	}

	const KeycodeAndModifiers& UIEvent::getKeycodeAndModifiers() const
	{
		return m_keycodeAndModifiers;
	}

	void UIEvent::setKeycodeAndModifiers(const KeycodeAndModifiers& km)
	{
		m_keycodeAndModifiers = km;
	}

	Keycode UIEvent::getKeycode() const
	{
		return m_keycodeAndModifiers.getKeycode();
	}

	void UIEvent::setKeycode(Keycode keycode)
	{
		m_keycodeAndModifiers.setKeycode(keycode);
	}

	Modifiers UIEvent::getModifiers() const
	{
		return m_keycodeAndModifiers.getModifiers();
	}

	void UIEvent::setModifiers(const Modifiers& modifiers)
	{
		m_keycodeAndModifiers.setModifiers(modifiers);
	}

	sl_uint32 UIEvent::getSystemKeycode() const
	{
		return m_systemKeycode;
	}

	void UIEvent::setSystemKeycode(sl_uint32 keycode)
	{
		m_systemKeycode = keycode;
	}

	const UIPointf& UIEvent::getPoint() const
	{
		return m_point.point;
	}

	void UIEvent::setPoint(const UIPointf& pt)
	{
		m_point.point = pt;
	}

	void UIEvent::setPoint(sl_ui_posf x, sl_ui_posf y)
	{
		m_point.point.x = x;
		m_point.point.y = y;
	}

	sl_ui_posf UIEvent::getX() const
	{
		return m_point.point.x;
	}

	void UIEvent::setX(sl_ui_posf x)
	{
		m_point.point.x = x;
	}

	sl_ui_posf UIEvent::getY() const
	{
		return m_point.point.y;
	}

	void UIEvent::setY(sl_ui_posf y)
	{
		m_point.point.y = y;
	}

	sl_real UIEvent::getDelta() const
	{
		return m_deltaY;
	}

	void UIEvent::setDelta(sl_real delta)
	{
		m_deltaY = delta;
	}

	sl_real UIEvent::getDeltaX() const
	{
		return m_deltaX;
	}

	void UIEvent::setDeltaX(sl_real x)
	{
		m_deltaX = x;
	}

	sl_real UIEvent::getDeltaY() const
	{
		return m_deltaY;
	}

	void UIEvent::setDeltaY(sl_real y)
	{
		m_deltaY = y;
	}

	const TouchPoint& UIEvent::getTouchPoint() const
	{
		return m_point;
	}

	void UIEvent::setTouchPoint(const TouchPoint& pt)
	{
		m_point = pt;
	}

	void UIEvent::setTouchPoint(const UIPointf& pt)
	{
		m_point.point = pt;
		m_point.pressure = 0;
	}

	void UIEvent::setTouchPoint(const UIPointf& pt, sl_real pressure)
	{
		m_point.point = pt;
		m_point.pressure = pressure;
	}

	void UIEvent::setTouchPoint(sl_ui_posf x, sl_ui_posf y)
	{
		m_point.point.x = x;
		m_point.point.y = y;
		m_point.pressure = 0;
	}

	void UIEvent::setTouchPoint(sl_ui_posf x, sl_ui_posf y, sl_real pressure)
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
		if (index < m_points.getCount()) {
			TouchPoint pt;
			m_points.getAt(index, &pt);
			return pt;
		} else {
			return m_point;
		}
	}

	void UIEvent::setTouchPoints(const Array<TouchPoint>& points)
	{
		m_points = points;
	}

	void UIEvent::transformPoints(const Matrix3f& mat)
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

	void UIEvent::transformPoints(const Matrix3lf& mat)
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

	void UIEvent::resetStates()
	{
		m_flags = 0;
	}

	void UIEvent::preventDefault()
	{
		SLIB_SET_FLAG(m_flags, (sl_uint32)flagPreventDefault);
	}

	sl_bool UIEvent::isPreventedDefault() const
	{
		return SLIB_CHECK_FLAG(m_flags, (sl_uint32)flagPreventDefault);
	}

	void UIEvent::setPreventedDefault(sl_bool flag)
	{
		if (flag) {
			SLIB_SET_FLAG(m_flags, (sl_uint32)flagPreventDefault);
		} else {
			SLIB_RESET_FLAG(m_flags, (sl_uint32)flagPreventDefault);
		}
	}

	void UIEvent::stopPropagation()
	{
		SLIB_SET_FLAG(m_flags, (sl_uint32)flagStopPropagation);
	}

	sl_bool UIEvent::isStoppedPropagation() const
	{
		return SLIB_CHECK_FLAG(m_flags, (sl_uint32)flagStopPropagation);
	}

	void UIEvent::setStoppedPropagation(sl_bool flag)
	{
		if (flag) {
			SLIB_SET_FLAG(m_flags, (sl_uint32)flagStopPropagation);
		} else {
			SLIB_RESET_FLAG(m_flags, (sl_uint32)flagStopPropagation);
		}
	}

	void UIEvent::passToNext()
	{
		SLIB_SET_FLAG(m_flags, (sl_uint32)flagPassToNext);
	}

	sl_bool UIEvent::isPassedToNext()
	{
		return SLIB_CHECK_FLAG(m_flags, (sl_uint32)flagPassToNext);
	}

	void UIEvent::setPassedToNext(sl_bool flag)
	{
		if (flag) {
			SLIB_SET_FLAG(m_flags, (sl_uint32)flagPassToNext);
		} else {
			SLIB_RESET_FLAG(m_flags, (sl_uint32)flagPassToNext);
		}
	}

	Ref<UIEvent> UIEvent::duplicate()
	{
		Ref<UIEvent> ret = new UIEvent();
		if (ret.isNotNull()) {
			ret->m_flags = m_flags;
			ret->m_action = m_action;
			ret->m_time = m_time;
			ret->m_keycodeAndModifiers = m_keycodeAndModifiers;
			ret->m_systemKeycode = m_systemKeycode;
			ret->m_point = m_point;
			ret->m_deltaX = m_deltaX;
			ret->m_deltaY = m_deltaY;
			ret->m_points = m_points.duplicate();
		}
		return ret;
	}

	class _UIKeyNameMapper
	{
	private:
		HashTable<sl_uint32, String> mapLong;
		HashTable<sl_uint32, String> mapShort;
		HashTable<String, sl_uint32> mapName;
		String nameInvalid;
		
	public:

#define _MAP_KEY(NAME) \
		{ \
			SLIB_STATIC_STRING(_s, #NAME); \
			mapLong.put((sl_uint32)(Keycode::NAME), _s); \
			mapShort.put((sl_uint32)(Keycode::NAME), _s); \
			mapName.put(_s.toLower(), (sl_uint32)(Keycode::NAME)); \
		}
#define _MAP_KEY2(NAME, SHORT_NAME) \
		{ \
			SLIB_STATIC_STRING(_s1, #NAME); \
			mapLong.put((sl_uint32)(Keycode::NAME), _s1); \
			SLIB_STATIC_STRING(_s2, SHORT_NAME); \
			mapShort.put((sl_uint32)(Keycode::NAME), _s2); \
			mapName.put(_s1.toLower(), (sl_uint32)(Keycode::NAME)); \
			mapName.put(_s2.toLower(), (sl_uint32)(Keycode::NAME)); \
		}

		_UIKeyNameMapper()
		{
			SLIB_STATIC_STRING(_invalid, "Invalid")
			nameInvalid = _invalid;
			_MAP_KEY(Unknown);
			
			_MAP_KEY2(Backspace, "Back")
			_MAP_KEY(Tab)
			_MAP_KEY(Enter)
			_MAP_KEY2(Escape, "Esc")
			
			_MAP_KEY(Space)
			_MAP_KEY2(Grave, "`")
			_MAP_KEY2(Equal, "=")
			_MAP_KEY2(Semicolon, ";")
			_MAP_KEY2(Backslash, "\\")
			_MAP_KEY2(LeftBaracket, "[")
			_MAP_KEY2(RightBaracket, "]")
			_MAP_KEY2(Quote, "'")
			_MAP_KEY2(Comma, ",")
			_MAP_KEY2(Minus, "-")
			_MAP_KEY2(Period, ".")
			_MAP_KEY2(Divide, "/")
			
			_MAP_KEY2(Num0, "0")
			_MAP_KEY2(Num1, "1")
			_MAP_KEY2(Num2, "2")
			_MAP_KEY2(Num3, "3")
			_MAP_KEY2(Num4, "4")
			_MAP_KEY2(Num5, "5")
			_MAP_KEY2(Num6, "6")
			_MAP_KEY2(Num7, "7")
			_MAP_KEY2(Num8, "8")
			_MAP_KEY2(Num9, "9")
			
			_MAP_KEY(A)
			_MAP_KEY(B)
			_MAP_KEY(C)
			_MAP_KEY(D)
			_MAP_KEY(E)
			_MAP_KEY(F)
			_MAP_KEY(G)
			_MAP_KEY(H)
			_MAP_KEY(I)
			_MAP_KEY(J)
			_MAP_KEY(K)
			_MAP_KEY(L)
			_MAP_KEY(M)
			_MAP_KEY(N)
			_MAP_KEY(O)
			_MAP_KEY(P)
			_MAP_KEY(Q)
			_MAP_KEY(R)
			_MAP_KEY(S)
			_MAP_KEY(T)
			_MAP_KEY(U)
			_MAP_KEY(V)
			_MAP_KEY(W)
			_MAP_KEY(X)
			_MAP_KEY(Y)
			_MAP_KEY(Z)
			
			_MAP_KEY(Numpad0)
			_MAP_KEY(Numpad1)
			_MAP_KEY(Numpad2)
			_MAP_KEY(Numpad3)
			_MAP_KEY(Numpad4)
			_MAP_KEY(Numpad5)
			_MAP_KEY(Numpad6)
			_MAP_KEY(Numpad7)
			_MAP_KEY(Numpad8)
			_MAP_KEY(Numpad9)
			
			_MAP_KEY2(NumpadDivide, "Numpad/")
			_MAP_KEY2(NumpadMultiply, "Numpad*")
			_MAP_KEY2(NumpadMinus, "Numpad-")
			_MAP_KEY2(NumpadPlus, "Numpad+")
			_MAP_KEY2(NumpadEnter, "NumpadEnter")
			_MAP_KEY2(NumpadDecimal, "Numpad.")
			
			_MAP_KEY(F1)
			_MAP_KEY(F2)
			_MAP_KEY(F3)
			_MAP_KEY(F4)
			_MAP_KEY(F5)
			_MAP_KEY(F6)
			_MAP_KEY(F7)
			_MAP_KEY(F8)
			_MAP_KEY(F9)
			_MAP_KEY(F10)
			_MAP_KEY(F11)
			_MAP_KEY(F12)
			
			_MAP_KEY2(PageUp, "PgUp")
			_MAP_KEY2(PageDown, "PgDn")
			_MAP_KEY(Home)
			_MAP_KEY(End)
			_MAP_KEY(Left)
			_MAP_KEY(Up)
			_MAP_KEY(Right)
			_MAP_KEY(Down)
			_MAP_KEY2(PrintScreen, "PrtSc")
			_MAP_KEY2(Insert, "Ins")
			_MAP_KEY2(Delete, "Del")
			_MAP_KEY(Sleep)
			_MAP_KEY(Pause)
			
			_MAP_KEY(GoHome)
			_MAP_KEY(GoMenu)
			_MAP_KEY(GoBack)
			_MAP_KEY(Camera)
			_MAP_KEY(VolumeMute)
			_MAP_KEY(VolumeDown)
			_MAP_KEY(VolumeUp)
			_MAP_KEY(MediaPrev)
			_MAP_KEY(MediaNext)
			_MAP_KEY(MediaPause)
			_MAP_KEY(MediaStop)
			_MAP_KEY2(PhoneStar, "Dial*")
			_MAP_KEY2(PhonePound, "Dial#")

			_MAP_KEY2(LeftShift, "LShift")
			_MAP_KEY2(RightShift, "RShift")
			_MAP_KEY2(LeftControl, "LCtrl")
			_MAP_KEY2(RightControl, "RCtrl")
#if defined(SLIB_PLATFORM_IS_APPLE)
			_MAP_KEY2(LeftAlt, "LAlt")
			_MAP_KEY2(RightAlt, "RAlt")
			_MAP_KEY2(LeftWin, "LWin")
			_MAP_KEY2(RightWin, "RWin")
			_MAP_KEY2(LeftOption, "LAlt")
			_MAP_KEY2(RightOption, "RAlt")
			_MAP_KEY2(LeftCommand, "LCmd")
			_MAP_KEY2(RightCommand, "RCmd")
#else
			_MAP_KEY2(LeftOption, "LAlt")
			_MAP_KEY2(RightOption, "RAlt")
			_MAP_KEY2(LeftCommand, "LCmd")
			_MAP_KEY2(RightCommand, "RCmd")
			_MAP_KEY2(LeftAlt, "LAlt")
			_MAP_KEY2(RightAlt, "RAlt")
			_MAP_KEY2(LeftWin, "LWin")
			_MAP_KEY2(RightWin, "RWin")
#endif
			_MAP_KEY(CapsLock)
			_MAP_KEY(ScrollLock)
			_MAP_KEY(NumLock)
			_MAP_KEY(ContextMenu)

		}
		
		String get(Keycode code, sl_bool flagShort)
		{
			String ret;
			if (flagShort) {
				if (mapShort.get((sl_uint32)code, &ret)) {
					return ret;
				}
			} else {
				if (mapLong.get((sl_uint32)code, &ret)) {
					return ret;
				}
			}
			return nameInvalid;
		}
		
		Keycode getCode(const String& keyName)
		{
			sl_uint32 keycode;
			if (mapName.get(keyName.toLower(), &keycode)) {
				return (Keycode)keycode;
			}
			return Keycode::Unknown;
		}
		
	};

	SLIB_SAFE_STATIC_GETTER(_UIKeyNameMapper, _UI_getKeyNameMapper)

	String UI::getKeyName(Keycode code, sl_bool flagShort)
	{
		_UIKeyNameMapper* mapper = _UI_getKeyNameMapper();
		if (mapper) {
			return mapper->get(code, flagShort);
		}
		return sl_null;
	}

	Keycode UI::getKeycodeFromName(const String& keyName)
	{
		_UIKeyNameMapper* mapper = _UI_getKeyNameMapper();
		if (mapper) {
			return mapper->getCode(keyName);
		}
		return Keycode::Unknown;
	}


	IViewListener::IViewListener()
	{
	}

	IViewListener::~IViewListener()
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

	void IViewListener::onKeyEvent(View* view, UIEvent* event)
	{
	}

	void IViewListener::onClick(View* view, UIEvent* event)
	{
	}

	void IViewListener::onSetCursor(View* view, UIEvent* event)
	{
	}

	void IViewListener::onResize(View* view, sl_ui_len width, sl_ui_len height)
	{
	}

	void IViewListener::onChangeVisibility(View* view, Visibility oldVisibility, Visibility newVisibility)
	{
	}

	void IViewListener::onScroll(View* view, sl_scroll_pos x, sl_scroll_pos y)
	{
	}

	void IViewListener::onSwipe(View* view, GestureEvent* ev)
	{
	}


	IWindowListener::IWindowListener()
	{
	}

	IWindowListener::~IWindowListener()
	{
	}

	void IWindowListener::onClose(Window* window, UIEvent* ev)
	{
	}

	void IWindowListener::onCancel(Window* window)
	{
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

	void IWindowListener::onResizing(Window* window, UISize& size)
	{
	}

	void IWindowListener::onResize(Window* window, sl_ui_len width, sl_ui_len height)
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
	UIEventLogListener::UIEventLogListener()
	{
	}

	UIEventLogListener::~UIEventLogListener()
	{
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

	void UIEventLogListener::onKeyEvent(View* view, UIEvent* event)
	{
		processKey("View", event);
	}

	void UIEventLogListener::onClick(View* view, UIEvent* event)
	{
		Log("View", "OnClick");
	}

	void UIEventLogListener::onSetCursor(View* view, UIEvent* event)
	{
		Log("View", "OnSetCursor");
	}

	void UIEventLogListener::onSwipe(View* view, GestureEvent* ev)
	{
		Log("View", "OnSwipe");
	}

	void UIEventLogListener::onResize(View* view, sl_ui_len width, sl_ui_len height)
	{
		Log("View", String::format("onResize (%d, %d)", width, height));
	}

	void UIEventLogListener::onClose(Window* window, UIEvent* ev)
	{
		Log("Window", "OnClose");
	}

	void UIEventLogListener::onCancel(Window* window)
	{
		Log("Window", "onCancel");
	}

	void UIEventLogListener::onActivate(Window* window)
	{
		Log("Window", "onActivate");
	}

	void UIEventLogListener::onDeactivate(Window* window)
	{
		Log("Window", "onDeactivate");
	}

	void UIEventLogListener::onMove(Window* window)
	{
		Log("Window", "onMove");
	}

	void UIEventLogListener::onResize(Window* window, sl_ui_len width, sl_ui_len height)
	{
		Log("Window", String::format("onResize (%d, %d)", width, height));
	}

	void UIEventLogListener::onMinimize(Window* window)
	{
		Log("Window", "onMinimize");
	}

	void UIEventLogListener::onDeminimize(Window* window)
	{
		Log("Window", "onDeminimize");
	}

	void UIEventLogListener::onMaximize(Window* window)
	{
		Log("Window", "onMaximize");
	}

	void UIEventLogListener::onDemaximize(Window* window)
	{
		Log("Window", "onDemaximize");
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
		Log(name, "%s %s VK-%d %s",
			str,
			UI::getKeyName(event->getKeycode()),
			event->getSystemKeycode(),
			getModifierText(event));
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
		Log(name, "%s (%d, %d) %s",
			str,
			(sl_int32)(event->getX()), (sl_int32)(event->getY()),
			getModifierText(event));
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
		Log(name, "%s (%d, %d), Pressure-%f",
			str,
			(sl_int32)(event->getX()), (sl_int32)(event->getY()),
			event->getPressure());
	}

	void UIEventLogListener::processMouseWheel(String name, UIEvent* event)
	{
		Log(name, "MouseWheel (%f, %f) %s", event->getDeltaX(), event->getDeltaY(), getModifierText(event));
	}

	String UI::getStatesDescription()
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
		return String::format("%s MOUSE(%d, %d)", str, (sl_int32)(pt.x), (sl_int32)(pt.y));
	}

}

