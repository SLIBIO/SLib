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

#include "slib/ui/event.h"

#include "slib/ui/core.h"
#include "slib/core/hash_table.h"
#include "slib/core/string_buffer.h"
#include "slib/core/safe_static.h"

namespace slib
{
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(TouchPoint)

	TouchPoint::TouchPoint(): pressure(0), phase(TouchPhase::Move), pointerId(0)
	{
	}

	TouchPoint::TouchPoint(const UIPointf& _point) : point(_point), pressure(0), phase(TouchPhase::Move), pointerId(0)
	{
	}

	TouchPoint::TouchPoint(const UIPointf& _point, sl_real _pressure, TouchPhase _phase) : point(_point), pressure(_pressure), phase(_phase), pointerId(0)
	{
	}

	TouchPoint::TouchPoint(const UIPointf& _point, sl_real _pressure, TouchPhase _phase, sl_uint64 _pointerId) : point(_point), pressure(_pressure), phase(_phase), pointerId(_pointerId)
	{
	}

	TouchPoint::TouchPoint(sl_ui_posf x, sl_ui_posf y) : point(x, y), pressure(0), phase(TouchPhase::Move), pointerId(0)
	{
	}

	TouchPoint::TouchPoint(sl_ui_posf x, sl_ui_posf y, sl_real _pressure) : point(x, y), pressure(_pressure), phase(TouchPhase::Move), pointerId(0)
	{
	}

	TouchPoint::TouchPoint(sl_ui_posf x, sl_ui_posf y, sl_real _pressure, TouchPhase _phase) : point(x, y), pressure(_pressure), phase(_phase), pointerId(0)
	{
	}

	TouchPoint::TouchPoint(sl_ui_posf x, sl_ui_posf y, sl_real _pressure, TouchPhase _phase, sl_uint64 _pointerId) : point(x, y), pressure(_pressure), phase(_phase), pointerId(_pointerId)
	{
	}


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
		sb.add(UIEvent::getKeyName(getKeycode(), sl_true));
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
				Keycode keycode = UIEvent::getKeycodeFromName(s);
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
			return ret;
		}
		return sl_null;
	}

	Ref<UIEvent> UIEvent::createKeyEvent(UIAction action, Keycode keycode, sl_uint32 systemKeycode, const Time& time)
	{
		Ref<UIEvent> ret = new UIEvent;
		if (ret.isNotNull()) {
			ret->setAction(action);
			ret->setKeycode(keycode);
			ret->setSystemKeycode(systemKeycode);
			ret->setTime(time);
			return ret;
		}
		return sl_null;
	}

	Ref<UIEvent> UIEvent::createMouseEvent(UIAction action, sl_ui_posf x, sl_ui_posf y, const Time& time)
	{
		Ref<UIEvent> ret = new UIEvent;
		if (ret.isNotNull()) {
			ret->setAction(action);
			ret->setX(x);
			ret->setY(y);
			ret->setTime(time);
			return ret;
		}
		return sl_null;
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
			return ret;
		}
		return sl_null;
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
			return ret;
		}
		return sl_null;
	}
	
	Ref<UIEvent> UIEvent::createTouchEvent(UIAction action, const TouchPoint& point, const Time& time)
	{
		Ref<UIEvent> ret = new UIEvent;
		if (ret.isNotNull()) {
			ret->setAction(action);
			ret->setTouchPoint(point);
			ret->setTime(time);
			return ret;
		}
		return sl_null;
	}

	Ref<UIEvent> UIEvent::createSetCursorEvent(sl_ui_posf x, sl_ui_posf y, const Time& time)
	{
		Ref<UIEvent> ret = new UIEvent;
		if (ret.isNotNull()) {
			ret->setAction(UIAction::SetCursor);
			ret->setPoint(x, y);
			ret->setTime(time);
			return ret;
		}
		return sl_null;
	}
	
	Ref<UIEvent> UIEvent::createOpenUrlEvent(const String& url)
	{
		Ref<UIEvent> ret = new UIEvent;
		if (ret.isNotNull()) {
			ret->setAction(UIAction::OpenUrl);
			ret->setUrl(url);
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

	const String& UIEvent::getUrl() const
	{
		return m_url;
	}
	
	void UIEvent::setUrl(const String& url)
	{
		m_url = url;
	}
	
	UIEventFlags UIEvent::getFlags() const
	{
		return m_flags;
	}
	
	void UIEvent::resetFlags()
	{
		m_flags = 0;
	}
	
	void UIEvent::addFlag(UIEventFlags flags)
	{
		SLIB_SET_FLAG(m_flags, flags);
	}
	
	void UIEvent::removeFlag(UIEventFlags flags)
	{
		SLIB_RESET_FLAG(m_flags, flags);
	}
	
	void UIEvent::preventDefault()
	{
		SLIB_SET_FLAG(m_flags, UIEventFlags::PreventDefault);
	}

	sl_bool UIEvent::isPreventedDefault() const
	{
		return SLIB_CHECK_FLAG(m_flags, UIEventFlags::PreventDefault);
	}

	void UIEvent::setPreventedDefault(sl_bool flag)
	{
		if (flag) {
			SLIB_SET_FLAG(m_flags, UIEventFlags::PreventDefault);
		} else {
			SLIB_RESET_FLAG(m_flags, UIEventFlags::PreventDefault);
		}
	}

	void UIEvent::stopPropagation()
	{
		SLIB_SET_FLAG(m_flags, UIEventFlags::StopPropagation);
	}

	sl_bool UIEvent::isStoppedPropagation() const
	{
		return SLIB_CHECK_FLAG(m_flags, UIEventFlags::StopPropagation);
	}

	void UIEvent::setStoppedPropagation(sl_bool flag)
	{
		if (flag) {
			SLIB_SET_FLAG(m_flags, UIEventFlags::StopPropagation);
		} else {
			SLIB_RESET_FLAG(m_flags, UIEventFlags::StopPropagation);
		}
	}

	void UIEvent::passToNext()
	{
		SLIB_SET_FLAG(m_flags, UIEventFlags::PassToNext);
	}

	sl_bool UIEvent::isPassedToNext()
	{
		return SLIB_CHECK_FLAG(m_flags, UIEventFlags::PassToNext);
	}

	void UIEvent::setPassedToNext(sl_bool flag)
	{
		if (flag) {
			SLIB_SET_FLAG(m_flags, UIEventFlags::PassToNext);
		} else {
			SLIB_RESET_FLAG(m_flags, UIEventFlags::PassToNext);
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

	class _priv_UIKeyNameMapper
	{
	private:
		HashTable<sl_uint32, String> mapLong;
		HashTable<sl_uint32, String> mapShort;
		HashTable<String, sl_uint32> mapName;
		String nameInvalid;
		
	public:

#define PRIV_MAP_KEY(NAME) \
		{ \
			SLIB_STATIC_STRING(_s, #NAME); \
			mapLong.put((sl_uint32)(Keycode::NAME), _s); \
			mapShort.put((sl_uint32)(Keycode::NAME), _s); \
			mapName.put(_s.toLower(), (sl_uint32)(Keycode::NAME)); \
		}
#define PRIV_MAP_KEY2(NAME, SHORT_NAME) \
		{ \
			SLIB_STATIC_STRING(_s1, #NAME); \
			mapLong.put((sl_uint32)(Keycode::NAME), _s1); \
			SLIB_STATIC_STRING(_s2, SHORT_NAME); \
			mapShort.put((sl_uint32)(Keycode::NAME), _s2); \
			mapName.put(_s1.toLower(), (sl_uint32)(Keycode::NAME)); \
			mapName.put(_s2.toLower(), (sl_uint32)(Keycode::NAME)); \
		}

		_priv_UIKeyNameMapper()
		{
			SLIB_STATIC_STRING(_invalid, "Invalid")
			nameInvalid = _invalid;
			PRIV_MAP_KEY(Unknown);
			
			PRIV_MAP_KEY2(Backspace, "Back")
			PRIV_MAP_KEY(Tab)
			PRIV_MAP_KEY(Enter)
			PRIV_MAP_KEY2(Escape, "Esc")
			
			PRIV_MAP_KEY(Space)
			PRIV_MAP_KEY2(Grave, "`")
			PRIV_MAP_KEY2(Equal, "=")
			PRIV_MAP_KEY2(Semicolon, ";")
			PRIV_MAP_KEY2(Backslash, "\\")
			PRIV_MAP_KEY2(LeftBaracket, "[")
			PRIV_MAP_KEY2(RightBaracket, "]")
			PRIV_MAP_KEY2(Quote, "'")
			PRIV_MAP_KEY2(Comma, ",")
			PRIV_MAP_KEY2(Minus, "-")
			PRIV_MAP_KEY2(Period, ".")
			PRIV_MAP_KEY2(Divide, "/")
			
			PRIV_MAP_KEY2(Num0, "0")
			PRIV_MAP_KEY2(Num1, "1")
			PRIV_MAP_KEY2(Num2, "2")
			PRIV_MAP_KEY2(Num3, "3")
			PRIV_MAP_KEY2(Num4, "4")
			PRIV_MAP_KEY2(Num5, "5")
			PRIV_MAP_KEY2(Num6, "6")
			PRIV_MAP_KEY2(Num7, "7")
			PRIV_MAP_KEY2(Num8, "8")
			PRIV_MAP_KEY2(Num9, "9")
			
			PRIV_MAP_KEY(A)
			PRIV_MAP_KEY(B)
			PRIV_MAP_KEY(C)
			PRIV_MAP_KEY(D)
			PRIV_MAP_KEY(E)
			PRIV_MAP_KEY(F)
			PRIV_MAP_KEY(G)
			PRIV_MAP_KEY(H)
			PRIV_MAP_KEY(I)
			PRIV_MAP_KEY(J)
			PRIV_MAP_KEY(K)
			PRIV_MAP_KEY(L)
			PRIV_MAP_KEY(M)
			PRIV_MAP_KEY(N)
			PRIV_MAP_KEY(O)
			PRIV_MAP_KEY(P)
			PRIV_MAP_KEY(Q)
			PRIV_MAP_KEY(R)
			PRIV_MAP_KEY(S)
			PRIV_MAP_KEY(T)
			PRIV_MAP_KEY(U)
			PRIV_MAP_KEY(V)
			PRIV_MAP_KEY(W)
			PRIV_MAP_KEY(X)
			PRIV_MAP_KEY(Y)
			PRIV_MAP_KEY(Z)
			
			PRIV_MAP_KEY(Numpad0)
			PRIV_MAP_KEY(Numpad1)
			PRIV_MAP_KEY(Numpad2)
			PRIV_MAP_KEY(Numpad3)
			PRIV_MAP_KEY(Numpad4)
			PRIV_MAP_KEY(Numpad5)
			PRIV_MAP_KEY(Numpad6)
			PRIV_MAP_KEY(Numpad7)
			PRIV_MAP_KEY(Numpad8)
			PRIV_MAP_KEY(Numpad9)
			
			PRIV_MAP_KEY2(NumpadDivide, "Numpad/")
			PRIV_MAP_KEY2(NumpadMultiply, "Numpad*")
			PRIV_MAP_KEY2(NumpadMinus, "Numpad-")
			PRIV_MAP_KEY2(NumpadPlus, "Numpad+")
			PRIV_MAP_KEY2(NumpadEnter, "NumpadEnter")
			PRIV_MAP_KEY2(NumpadDecimal, "Numpad.")
			
			PRIV_MAP_KEY(F1)
			PRIV_MAP_KEY(F2)
			PRIV_MAP_KEY(F3)
			PRIV_MAP_KEY(F4)
			PRIV_MAP_KEY(F5)
			PRIV_MAP_KEY(F6)
			PRIV_MAP_KEY(F7)
			PRIV_MAP_KEY(F8)
			PRIV_MAP_KEY(F9)
			PRIV_MAP_KEY(F10)
			PRIV_MAP_KEY(F11)
			PRIV_MAP_KEY(F12)
			
			PRIV_MAP_KEY2(PageUp, "PgUp")
			PRIV_MAP_KEY2(PageDown, "PgDn")
			PRIV_MAP_KEY(Home)
			PRIV_MAP_KEY(End)
			PRIV_MAP_KEY(Left)
			PRIV_MAP_KEY(Up)
			PRIV_MAP_KEY(Right)
			PRIV_MAP_KEY(Down)
			PRIV_MAP_KEY2(PrintScreen, "PrtSc")
			PRIV_MAP_KEY2(Insert, "Ins")
			PRIV_MAP_KEY2(Delete, "Del")
			PRIV_MAP_KEY(Sleep)
			PRIV_MAP_KEY(Pause)
			
			PRIV_MAP_KEY(GoHome)
			PRIV_MAP_KEY(GoMenu)
			PRIV_MAP_KEY(GoBack)
			PRIV_MAP_KEY(Camera)
			PRIV_MAP_KEY(VolumeMute)
			PRIV_MAP_KEY(VolumeDown)
			PRIV_MAP_KEY(VolumeUp)
			PRIV_MAP_KEY(MediaPrev)
			PRIV_MAP_KEY(MediaNext)
			PRIV_MAP_KEY(MediaPause)
			PRIV_MAP_KEY(MediaStop)
			PRIV_MAP_KEY2(PhoneStar, "Dial*")
			PRIV_MAP_KEY2(PhonePound, "Dial#")

			PRIV_MAP_KEY2(LeftShift, "LShift")
			PRIV_MAP_KEY2(RightShift, "RShift")
			PRIV_MAP_KEY2(LeftControl, "LCtrl")
			PRIV_MAP_KEY2(RightControl, "RCtrl")
#if defined(SLIB_PLATFORM_IS_APPLE)
			PRIV_MAP_KEY2(LeftAlt, "LAlt")
			PRIV_MAP_KEY2(RightAlt, "RAlt")
			PRIV_MAP_KEY2(LeftWin, "LWin")
			PRIV_MAP_KEY2(RightWin, "RWin")
			PRIV_MAP_KEY2(LeftOption, "LAlt")
			PRIV_MAP_KEY2(RightOption, "RAlt")
			PRIV_MAP_KEY2(LeftCommand, "LCmd")
			PRIV_MAP_KEY2(RightCommand, "RCmd")
#else
			PRIV_MAP_KEY2(LeftOption, "LAlt")
			PRIV_MAP_KEY2(RightOption, "RAlt")
			PRIV_MAP_KEY2(LeftCommand, "LCmd")
			PRIV_MAP_KEY2(RightCommand, "RCmd")
			PRIV_MAP_KEY2(LeftAlt, "LAlt")
			PRIV_MAP_KEY2(RightAlt, "RAlt")
			PRIV_MAP_KEY2(LeftWin, "LWin")
			PRIV_MAP_KEY2(RightWin, "RWin")
#endif
			PRIV_MAP_KEY(CapsLock)
			PRIV_MAP_KEY(ScrollLock)
			PRIV_MAP_KEY(NumLock)
			PRIV_MAP_KEY(ContextMenu)

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

	SLIB_SAFE_STATIC_GETTER(_priv_UIKeyNameMapper, _priv_UI_getKeyNameMapper)

	String UIEvent::getKeyName(Keycode code, sl_bool flagShort)
	{
		_priv_UIKeyNameMapper* mapper = _priv_UI_getKeyNameMapper();
		if (mapper) {
			return mapper->get(code, flagShort);
		}
		return sl_null;
	}

	Keycode UIEvent::getKeycodeFromName(const String& keyName)
	{
		_priv_UIKeyNameMapper* mapper = _priv_UI_getKeyNameMapper();
		if (mapper) {
			return mapper->getCode(keyName);
		}
		return Keycode::Unknown;
	}

}
