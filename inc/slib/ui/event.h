#ifndef CHECKHEADER_SLIB_UI_EVENT
#define CHECKHEADER_SLIB_UI_EVENT

#include "definition.h"
#include "constants.h"

#include "../core/string.h"
#include "../math/geometry2d.h"

SLIB_UI_NAMESPACE_BEGIN

class Window;
class View;

struct SLIB_EXPORT TouchPoint
{
	Point point;
	sl_real pressure;
	
	SLIB_INLINE TouchPoint() {}
	
	SLIB_INLINE TouchPoint(const TouchPoint& other) : point(other.point)
	{
		pressure = other.pressure;
	}
	
	SLIB_INLINE TouchPoint& operator=(const TouchPoint& other)
	{
		point = other.point;
		pressure = other.pressure;
		return *this;
	}
	
	SLIB_INLINE TouchPoint(const Point& _point) : point(_point)
	{
		pressure = 0;
	}

	SLIB_INLINE TouchPoint(const Point& _point, sl_real _pressure) : point(_point)
	{
		pressure = _pressure;
	}
	
	SLIB_INLINE TouchPoint(sl_real x, sl_real y) : point(x, y)
	{
		pressure = 0;
	}

	SLIB_INLINE TouchPoint(sl_real x, sl_real y, sl_real _pressure) : point(x, y)
	{
		pressure = _pressure;
	}
};

class SLIB_EXPORT UIEvent : public Referable
{	
protected:
	SLIB_INLINE UIEvent()
	{
		m_flags = 0;
		m_action = actionUnknown;
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
	
public:
	SLIB_INLINE UIEventAction getAction() const
	{
		return m_action;
	}
	SLIB_INLINE void setAction(UIEventAction action)
	{
		m_action = action;
	}
	
	// keyboard
	SLIB_INLINE Keycode getKeycode() const
	{
		return m_keycode;
	}
	SLIB_INLINE void setKeycode(Keycode keycode)
	{
		m_keycode = keycode;
	}
	SLIB_INLINE sl_uint32 getSystemKeycode() const
	{
		return m_systemKeycode;
	}
	SLIB_INLINE void setSystemKeycode(sl_uint32 keycode)
	{
		m_systemKeycode = keycode;
	}
	
	// mouse, touch
	SLIB_INLINE const Point& getPoint() const
	{
		return m_point.point;
	}
	SLIB_INLINE void setPoint(const Point& pt)
	{
		m_point.point = pt;
	}
	SLIB_INLINE void setPoint(sl_real x, sl_real y)
	{
		m_point.point.x = x;
		m_point.point.y = y;
	}
	SLIB_INLINE sl_real getX() const
	{
		return m_point.point.x;
	}
	SLIB_INLINE void setX(sl_real x)
	{
		m_point.point.x = x;
	}
	SLIB_INLINE sl_real getY() const
	{
		return m_point.point.y;
	}
	SLIB_INLINE void setY(sl_real y)
	{
		m_point.point.y = y;
	}
	
	// mouse wheel
	SLIB_INLINE sl_real getDelta() const
	{
		return m_point.point.y;
	}
	SLIB_INLINE void setDelta(sl_real delta)
	{
		m_point.point.y = delta;
	}
	SLIB_INLINE sl_real getDeltaX() const
	{
		return m_point.point.x;
	}
	SLIB_INLINE void setDeltaX(sl_real x)
	{
		m_point.point.x = x;
	}
	SLIB_INLINE sl_real getDeltaY() const
	{
		return m_point.point.y;
	}
	SLIB_INLINE void setDeltaY(sl_real y)
	{
		m_point.point.y = y;
	}
	
	// touch
	SLIB_INLINE const TouchPoint& getTouchPoint() const
	{
		return m_point;
	}
	SLIB_INLINE void setTouchPoint(const TouchPoint& pt)
	{
		m_point = pt;
	}
	SLIB_INLINE void setTouchPoint(const Point& pt)
	{
		m_point.point = pt;
		m_point.pressure = 0;
	}
	SLIB_INLINE void setTouchPoint(const Point& pt, sl_real pressure)
	{
		m_point.point = pt;
		m_point.pressure = pressure;
	}
	SLIB_INLINE void setTouchPoint(sl_real x, sl_real y)
	{
		m_point.point.x = x;
		m_point.point.y = y;
		m_point.pressure = 0;
	}
	SLIB_INLINE void setTouchPoint(sl_real x, sl_real y, sl_real pressure)
	{
		m_point.point.x = x;
		m_point.point.y = y;
		m_point.pressure = pressure;
	}
	SLIB_INLINE sl_real getPressure() const
	{
		return m_point.pressure;
	}
	SLIB_INLINE void setPressure(sl_real pressure)
	{
		m_point.pressure = pressure;
	}
	
	SLIB_INLINE Array<TouchPoint> getTouchPoints() const
	{
		return m_points;
	}
	SLIB_INLINE sl_uint32 getTouchPointsCount() const
	{
		return (sl_uint32)(m_points.count());
	}
	SLIB_INLINE TouchPoint getTouchPoint(sl_uint32 index) const
	{
		TouchPoint pt;
		m_points.getItem(index, &pt);
		return pt;
	}
	SLIB_INLINE void setTouchPoints(const Array<TouchPoint>& points)
	{
		m_points = points;
	}

	void transformPoints(const Matrix3& mat);
	
	// modifiers
	SLIB_INLINE void setShiftKey()
	{
		SLIB_SET_FLAG(m_flags, (sl_uint32)flagShiftKey);
	}
	SLIB_INLINE void clearShiftKey()
	{
		SLIB_RESET_FLAG(m_flags, (sl_uint32)flagShiftKey);
	}
	SLIB_INLINE sl_bool isShiftKey() const
	{
		return SLIB_CHECK_FLAG(m_flags, (sl_uint32)flagShiftKey);
	}
	
	SLIB_INLINE void setAltKey()
	{
		SLIB_SET_FLAG(m_flags, (sl_uint32)flagAltKey);
	}
	SLIB_INLINE void clearAltKey()
	{
		SLIB_RESET_FLAG(m_flags, (sl_uint32)flagAltKey);
	}
	SLIB_INLINE sl_bool isAltKey() const
	{
		return SLIB_CHECK_FLAG(m_flags, (sl_uint32)flagAltKey);
	}
	
	SLIB_INLINE void setOptionKey()
	{
		SLIB_SET_FLAG(m_flags, (sl_uint32)flagOptionKey);
	}
	SLIB_INLINE void clearOptionKey()
	{
		SLIB_RESET_FLAG(m_flags, (sl_uint32)flagOptionKey);
	}
	SLIB_INLINE sl_bool isOptionKey() const
	{
		return SLIB_CHECK_FLAG(m_flags, (sl_uint32)flagOptionKey);
	}
	
	SLIB_INLINE void setControlKey()
	{
		SLIB_SET_FLAG(m_flags, (sl_uint32)flagControlKey);
	}
	SLIB_INLINE void clearControlKey()
	{
		SLIB_RESET_FLAG(m_flags, (sl_uint32)flagControlKey);
	}
	SLIB_INLINE sl_bool isControlKey() const
	{
		return SLIB_CHECK_FLAG(m_flags, (sl_uint32)flagControlKey);
	}
	
	SLIB_INLINE void setWindowsKey()
	{
		SLIB_SET_FLAG(m_flags, (sl_uint32)flagWindowsKey);
	}
	SLIB_INLINE void clearWindowsKey()
	{
		SLIB_RESET_FLAG(m_flags, (sl_uint32)flagWindowsKey);
	}
	SLIB_INLINE sl_bool isWindowsKey() const
	{
		return SLIB_CHECK_FLAG(m_flags, (sl_uint32)flagWindowsKey);
	}
	
	SLIB_INLINE void setCommandKey()
	{
		SLIB_SET_FLAG(m_flags, (sl_uint32)flagCommandKey);
	}
	SLIB_INLINE void clearCommandKey()
	{
		SLIB_RESET_FLAG(m_flags, (sl_uint32)flagCommandKey);
	}
	SLIB_INLINE sl_bool isCommandKey() const
	{
		return SLIB_CHECK_FLAG(m_flags, (sl_uint32)flagCommandKey);
	}
	
	// event management
	SLIB_INLINE void preventDefault()
	{
		SLIB_SET_FLAG(m_flags, (sl_uint32)flagPreventDefault);
	}
	SLIB_INLINE sl_bool isPreventedDefault() const
	{
		return SLIB_CHECK_FLAG(m_flags, (sl_uint32)flagPreventDefault);
	}
	
	SLIB_INLINE void stopPropagation()
	{
		SLIB_SET_FLAG(m_flags, (sl_uint32)flagStopPropagation);
	}
	SLIB_INLINE sl_bool isStoppedPropagation() const
	{
		return SLIB_CHECK_FLAG(m_flags, (sl_uint32)flagStopPropagation);
	}
	
public:
	static Ref<UIEvent> createKeyEvent(UIEventAction action, Keycode keycode, sl_uint32 systemKeycode);
	static Ref<UIEvent> createMouseEvent(UIEventAction action, sl_real x, sl_real y);
	static Ref<UIEvent> createMouseWheelEvent(sl_real deltaX, sl_real deltaY);
	static Ref<UIEvent> createTouchEvent(UIEventAction action, const Array<TouchPoint>& _points);
	static Ref<UIEvent> createSetCursorEvent();

	Ref<UIEvent> duplicate();
	
	static sl_uint32 getSystemKeycode(Keycode key);
	static Keycode getKeycodeFromSystemKeycode(sl_uint32 systemKeycode);

	SLIB_INLINE sl_bool isKeyEvent()
	{
		return (m_action & 0xff00) == 0x0100;
	}
	SLIB_INLINE sl_bool isMouseEvent()
	{
		return (m_action & 0xff00) == 0x0200;
	}
	SLIB_INLINE sl_bool isTouchEvent()
	{
		return (m_action & 0xff00) == 0x0300;
	}

protected:
	sl_uint32 m_flags;
	UIEventAction m_action;
	
	// keyboard
	Keycode m_keycode;
	sl_uint32 m_systemKeycode;
	
	// mouse, touch
	TouchPoint m_point;
	
	// touch
	Array<TouchPoint> m_points;
};

class Variant;
class SLIB_EXPORT IViewListener
{
public:
	virtual void onClick(View* view)
	{
	}
	
	virtual void onKeyEvent(View* view, UIEvent* event)
	{
	}

	virtual void onMouseEvent(View* view, UIEvent* event)
	{
	}
	
	virtual void onTouchEvent(View* view, UIEvent* event)
	{
	}

	virtual void onMouseWheelEvent(View* view, UIEvent* event)
	{
	}
	
	virtual void onSetCursor(View* view, UIEvent* event)
	{
	}
};

class SLIB_EXPORT IWindowListener
{
public:
	virtual sl_bool onClose(Window* window)
	{
		return sl_true;
	}

	virtual void onActivate(Window* window)
	{
	}

	virtual void onDeactivate(Window* window)
	{
	}

	virtual void onMove(Window* window)
	{
	}

	virtual void onResize(Window* window, Size& size)
	{
	}

	virtual void onMinimize(Window* window)
	{
	}

	virtual void onDeminimize(Window* window)
	{
	}

	virtual void onMaximize(Window* window)
	{
	}

	virtual void onDemaximize(Window* window)
	{
	}
};

class SLIB_EXPORT UIEventLogListener : public Referable, public IWindowListener, public IViewListener
{
public:
	void onClick(View* view);
	void onKeyEvent(View* view, UIEvent* event);
	void onMouseEvent(View* view, UIEvent* event);
	void onMouseWheelEvent(View* view, UIEvent* event);
	void onTouchEvent(View* view, UIEvent* event);
	void onSetCursor(View* view, UIEvent* event);
	
	// window related events
	sl_bool onClose(Window* window);
	void onActivate(Window* window);
	void onDeactivate(Window* window);
	void onMove(Window* window);
	void onResize(Window* window, Size& size);
	void onMinimize(Window* window);
	void onDeminimize(Window* window);
	void onMaximize(Window* window);
	void onDemaximize(Window* window);
	
private:
	void processKey(String name, UIEvent* event);
	void processMouse(String name, UIEvent* event);
	void processMouseWheel(String name, UIEvent* event);
	void processTouch(String name, UIEvent* event);
	String getModifierText(UIEvent* event);
	
public:
	static Ref<UIEventLogListener> create();
};

SLIB_UI_NAMESPACE_END

#endif
