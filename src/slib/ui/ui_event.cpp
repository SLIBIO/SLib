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
#include "slib/ui/view.h"
#include "slib/graphics/drawable.h"
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
			sb.addStatic("Command+");
#else
			sb.addStatic("Win+");
#endif
		}
		if (isControlKey()) {
			sb.addStatic("Ctrl+");
		}
		if (isShiftKey()) {
			sb.addStatic("Shift+");
		}
		if (isAltKey()) {
#if defined(SLIB_PLATFORM_IS_APPLE)
			sb.addStatic("Option+");
#else
			sb.addStatic("Alt+");
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


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DragItem)

	DragItem::DragItem()
	{
	}

	const UIRect& DragItem::getFrame() const
	{
		return m_frame;
	}

	void DragItem::setDraggingFrame(const UIRect& frame)
	{
		m_frame = frame;
	}

	const Ref<Drawable>& DragItem::getDraggingImage() const
	{
		return m_image;
	}

	void DragItem::setDraggingImage(const Ref<Drawable>& image)
	{
		m_image = image;
	}


	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(DragContext)

	DragContext::DragContext()
	{
	}


	SLIB_DEFINE_ROOT_OBJECT(UIEvent)

	namespace priv
	{
		namespace ui_event
		{
			
			class KeyboardEvent : public UIEvent
			{
			public:
				sl_uint32 m_systemKeycode;
				
			public:
				KeyboardEvent(UIAction action, const Time& time, sl_uint32 systemKeyboard) : UIEvent(action, time), m_systemKeycode(systemKeyboard)
				{
				}
				
			public:
				Ref<UIEvent> duplicate() const override
				{
					KeyboardEvent* ret = new KeyboardEvent(m_action, m_time, m_systemKeycode);
					if (ret) {
						ret->_copyProperties(this);
						return ret;
					}
					return sl_null;
				}

			};
			
			class MouseEvent : public UIEvent
			{
			public:
				TouchPoint m_pt;
				
			public:
				MouseEvent(UIAction action, const Time& time, sl_ui_posf x, sl_ui_posf y) : UIEvent(action, time), m_pt(x, y)
				{
				}

				MouseEvent(UIAction action, const Time& time, const TouchPoint& pt) : UIEvent(action, time), m_pt(pt)
				{
				}
				
			public:
				Ref<UIEvent> duplicate() const override
				{
					MouseEvent* ret = new MouseEvent(m_action, m_time, m_pt);
					if (ret) {
						ret->_copyProperties(this);
						return ret;
					}
					return sl_null;
				}

			};
			
			class MouseWheelEvent : public MouseEvent
			{
			public:
				sl_real m_deltaX;
				sl_real m_deltaY;
				
			public:
				MouseWheelEvent(UIAction action, const Time& time, sl_ui_posf x, sl_ui_posf y, sl_real deltaX, sl_real deltaY) : MouseEvent(action, time, x, y), m_deltaX(deltaX), m_deltaY(deltaY)
				{
				}
				
			public:
				Ref<UIEvent> duplicate() const override
				{
					MouseWheelEvent* ret = new MouseWheelEvent(m_action, m_time, m_pt.point.x, m_pt.point.y, m_deltaX, m_deltaY);
					if (ret) {
						ret->_copyProperties(this);
						return ret;
					}
					return sl_null;
				}

			};
			
			class TouchEvent : public MouseEvent
			{
			public:
				Array<TouchPoint> m_points;
				
			public:
				TouchEvent(UIAction action, const Time& time, const TouchPoint& pt) : MouseEvent(action, time, pt)
				{
				}

				TouchEvent(UIAction action, const Time& time, const Array<TouchPoint>& points) : MouseEvent(action, time, 0, 0), m_points(points)
				{
					if (points.getCount()) {
						m_pt = points[0];
					}
				}

			public:
				Ref<UIEvent> duplicate() const override
				{
					if (m_points.isNotNull()) {
						TouchEvent* ret = new TouchEvent(m_action, m_time, m_points.duplicate());
						if (ret) {
							ret->_copyProperties(this);
							ret->m_pt = m_pt;
							return ret;
						}
					} else {
						TouchEvent* ret = new TouchEvent(m_action, m_time, m_pt);
						if (ret) {
							ret->_copyProperties(this);
							return ret;
						}
					}
					return sl_null;
				}

			};
			
		}
	}

	using namespace priv::ui_event;

	UIEvent::UIEvent() : m_flags(0), m_action(UIAction::Unknown), m_time(0)
	{
	}

	UIEvent::UIEvent(UIAction action, const Time& time) : m_flags(0), m_action(action), m_time(time)
	{
	}

	UIEvent::~UIEvent()
	{
	}

	Ref<UIEvent> UIEvent::createUnknown(const Time& time)
	{
		return new UIEvent(UIAction::Unknown, time);
	}

	Ref<UIEvent> UIEvent::createKeyEvent(UIAction action, Keycode keycode, sl_uint32 systemKeycode, const Time& time)
	{
		Ref<KeyboardEvent> ret = new KeyboardEvent(action, time, systemKeycode);
		if (ret.isNotNull()) {
			ret->setKeycode(keycode);
			return Ref<UIEvent>::from(ret);
		}
		return sl_null;
	}

	Ref<UIEvent> UIEvent::createMouseEvent(UIAction action, sl_ui_posf x, sl_ui_posf y, const Time& time)
	{
		return new MouseEvent(action, time, x, y);
	}

	Ref<UIEvent> UIEvent::createSetCursorEvent(sl_ui_posf x, sl_ui_posf y, const Time& time)
	{
		return new MouseEvent(UIAction::SetCursor, time, x, y);
	}

	Ref<UIEvent> UIEvent::createMouseWheelEvent(sl_ui_posf mouseX, sl_ui_posf mouseY, sl_real deltaX, sl_real deltaY, const Time& time)
	{
		return new MouseWheelEvent(UIAction::MouseWheel, time, mouseX, mouseY, deltaX, deltaY);
	}

	Ref<UIEvent> UIEvent::createTouchEvent(UIAction action, const Array<TouchPoint>& points, const Time& time)
	{
		return new TouchEvent(action, time, points);
	}
	
	Ref<UIEvent> UIEvent::createTouchEvent(UIAction action, const TouchPoint& point, const Time& time)
	{
		return new TouchEvent(action, time, Array<TouchPoint>::create(&point, 1));
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
		return ((sl_uint32)m_action & SLIB_UI_ACTION_TYPE_KEYBOARD) != 0;
	}

	sl_bool UIEvent::isMouseEvent() const
	{
		return ((sl_uint32)m_action & SLIB_UI_ACTION_TYPE_MOUSE) != 0;
	}

	sl_bool UIEvent::isTouchEvent() const
	{
		return ((sl_uint32)m_action & SLIB_UI_ACTION_TYPE_TOUCH) != 0;
	}

	sl_bool UIEvent::isDragEvent() const
	{
		return ((sl_uint32)m_action & SLIB_UI_ACTION_TYPE_DRAG) != 0;
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
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_KEYBOARD) {
			return ((KeyboardEvent*)this)->m_systemKeycode;
		}
		return 0;
	}

	void UIEvent::setSystemKeycode(sl_uint32 keycode)
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_KEYBOARD) {
			((KeyboardEvent*)this)->m_systemKeycode = keycode;
		}
	}

	const UIPointf& UIEvent::getPoint() const
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_MOUSE) {
			return ((MouseEvent*)this)->m_pt.point;
		}
		return UIPointf::zero();
	}

	void UIEvent::setPoint(const UIPointf& pt)
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_MOUSE) {
			((MouseEvent*)this)->m_pt.point = pt;
		}
	}

	void UIEvent::setPoint(sl_ui_posf x, sl_ui_posf y)
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_MOUSE) {
			((MouseEvent*)this)->m_pt.point.x = x;
			((MouseEvent*)this)->m_pt.point.y = y;
		}
	}

	sl_ui_posf UIEvent::getX() const
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_MOUSE) {
			return ((MouseEvent*)this)->m_pt.point.x;
		}
		return 0;
	}

	void UIEvent::setX(sl_ui_posf x)
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_MOUSE) {
			((MouseEvent*)this)->m_pt.point.x = x;
		}
	}

	sl_ui_posf UIEvent::getY() const
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_MOUSE) {
			return ((MouseEvent*)this)->m_pt.point.y;
		}
		return 0;
	}

	void UIEvent::setY(sl_ui_posf y)
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_MOUSE) {
			((MouseEvent*)this)->m_pt.point.y = y;
		}
	}

	sl_real UIEvent::getDelta() const
	{
		return getDeltaY();
	}

	void UIEvent::setDelta(sl_real delta)
	{
		setDeltaY(delta);
	}

	sl_real UIEvent::getDeltaX() const
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_MOUSE_WHEEL) {
			return ((MouseWheelEvent*)this)->m_deltaX;
		}
		return 0;
	}

	void UIEvent::setDeltaX(sl_real x)
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_MOUSE_WHEEL) {
			((MouseWheelEvent*)this)->m_deltaX = x;
		}
	}

	sl_real UIEvent::getDeltaY() const
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_MOUSE_WHEEL) {
			return ((MouseWheelEvent*)this)->m_deltaY;
		}
		return 0;
	}

	void UIEvent::setDeltaY(sl_real y)
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_MOUSE_WHEEL) {
			((MouseWheelEvent*)this)->m_deltaY = y;
		}
	}

	const TouchPoint& UIEvent::getTouchPoint() const
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_MOUSE) {
			return ((MouseEvent*)this)->m_pt;
		}
		static const char zero[sizeof(TouchPoint)] = {0};
		return *((TouchPoint*)(void*)zero);
	}

	void UIEvent::setTouchPoint(const TouchPoint& pt)
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_MOUSE) {
			((MouseEvent*)this)->m_pt = pt;
		}
	}

	void UIEvent::setTouchPoint(const UIPointf& pt)
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_MOUSE) {
			((MouseEvent*)this)->m_pt.point = pt;
			((MouseEvent*)this)->m_pt.pressure = 0;
		}
	}

	void UIEvent::setTouchPoint(const UIPointf& pt, sl_real pressure)
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_MOUSE) {
			((MouseEvent*)this)->m_pt.point = pt;
			((MouseEvent*)this)->m_pt.pressure = pressure;
		}
	}

	void UIEvent::setTouchPoint(sl_ui_posf x, sl_ui_posf y)
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_MOUSE) {
			((MouseEvent*)this)->m_pt.point.x = x;
			((MouseEvent*)this)->m_pt.point.y = y;
			((MouseEvent*)this)->m_pt.pressure = 0;
		}
	}

	void UIEvent::setTouchPoint(sl_ui_posf x, sl_ui_posf y, sl_real pressure)
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_MOUSE) {
			((MouseEvent*)this)->m_pt.point.x = x;
			((MouseEvent*)this)->m_pt.point.y = y;
			((MouseEvent*)this)->m_pt.pressure = pressure;
		}
	}

	sl_real UIEvent::getPressure() const
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_MOUSE) {
			return ((MouseEvent*)this)->m_pt.pressure;
		}
		return 0;
	}

	void UIEvent::setPressure(sl_real pressure)
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_MOUSE) {
			((MouseEvent*)this)->m_pt.pressure = pressure;
		}
	}

	const Array<TouchPoint>& UIEvent::getTouchPoints() const
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_TOUCH) {
			return ((TouchEvent*)this)->m_points;
		}
		return Array<TouchPoint>::null();
	}

	sl_uint32 UIEvent::getTouchPointsCount() const
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_TOUCH) {
			return (sl_uint32)(((TouchEvent*)this)->m_points.getCount());
		}
		return 0;
	}

	const TouchPoint& UIEvent::getTouchPoint(sl_uint32 index) const
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_TOUCH) {
			if (index < ((TouchEvent*)this)->m_points.getCount()) {
				return ((TouchEvent*)this)->m_points[index];
			} else {
				return ((TouchEvent*)this)->m_pt;
			}
		}
		return getTouchPoint();
	}

	void UIEvent::setTouchPoints(const Array<TouchPoint>& points)
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_TOUCH) {
			((TouchEvent*)this)->m_points = points;
		}
	}

	void UIEvent::transformPoints(const Matrix3f& mat)
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_MOUSE) {
			((MouseEvent*)this)->m_pt.point = mat.transformPosition(((MouseEvent*)this)->m_pt.point);
		}
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_TOUCH) {
			Array<TouchPoint>& points = ((TouchEvent*)this)->m_points;
			sl_size n = points.getCount();
			TouchPoint* pts = points.getData();
			for (sl_size i = 0; i < n; i++) {
				pts[i].point = mat.transformPosition(pts[i].point);
			}
		}
	}

	void UIEvent::transformPoints(const Matrix3lf& mat)
	{
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_MOUSE) {
			((MouseEvent*)this)->m_pt.point = mat.transformPosition(((MouseEvent*)this)->m_pt.point);
		}
		if (((sl_uint32)m_action) & SLIB_UI_ACTION_TYPE_TOUCH) {
			Array<TouchPoint>& points = ((TouchEvent*)this)->m_points;
			sl_size n = points.getCount();
			TouchPoint* pts = points.getData();
			for (sl_size i = 0; i < n; i++) {
				pts[i].point = mat.transformPosition(pts[i].point);
			}
		}
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
	
	Ref<UIEvent> UIEvent::duplicate() const
	{
		Ref<UIEvent> ret = new UIEvent(m_action, m_time);
		if (ret.isNotNull()) {
			ret->_copyProperties(this);
			return ret;
		}
		return sl_null;
	}

	void UIEvent::_copyProperties(const UIEvent* other)
	{
		m_flags = other->m_flags;
		m_keycodeAndModifiers = other->m_keycodeAndModifiers;
	}

	namespace priv
	{
		namespace ui_event
		{

			class KeyNameMapper
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

				KeyNameMapper()
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

			SLIB_SAFE_STATIC_GETTER(KeyNameMapper, GetKeyNameMapper)

		}
	}

	String UIEvent::getKeyName(Keycode code, sl_bool flagShort)
	{
		priv::ui_event::KeyNameMapper* mapper = priv::ui_event::GetKeyNameMapper();
		if (mapper) {
			return mapper->get(code, flagShort);
		}
		return sl_null;
	}

	Keycode UIEvent::getKeycodeFromName(const String& keyName)
	{
		priv::ui_event::KeyNameMapper* mapper = priv::ui_event::GetKeyNameMapper();
		if (mapper) {
			return mapper->getCode(keyName);
		}
		return Keycode::Unknown;
	}

}
