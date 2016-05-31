#ifndef CHECKHEADER_SLIB_UI_EVENT
#define CHECKHEADER_SLIB_UI_EVENT

#include "definition.h"

#include "constants.h"

#include "../core/string.h"
#include "../math/point.h"
#include "../math/size.h"
#include "../math/matrix3.h"

SLIB_UI_NAMESPACE_BEGIN

class Window;
class View;

class SLIB_EXPORT TouchPoint
{
public:
	Point point;
	sl_real pressure;
	TouchPhase phase;

public:
	TouchPoint();

	TouchPoint(const TouchPoint& other);
	
	TouchPoint(const Point& point);
	
	TouchPoint(const Point& point, sl_real pressure);
	
	TouchPoint(const Point& point, sl_real pressure, TouchPhase phase);
	
	TouchPoint(sl_real x, sl_real y);
	
	TouchPoint(sl_real x, sl_real y, sl_real pressure);
	
	TouchPoint(sl_real x, sl_real y, sl_real pressure, TouchPhase phase);
	
public:
	TouchPoint& operator=(const TouchPoint& other);

};

template<>
SLIB_INLINE sl_uint32 Hash<Keycode>::hash(const Keycode& code)
{
	return sl_hash((sl_uint32)code);
}

class SLIB_EXPORT KeycodeAndModifiers
{
public:
	SLIB_DECLARE_PRIMITIVE_WRAPPER(int, KeycodeAndModifiers)
	
public:
	KeycodeAndModifiers();
	
	KeycodeAndModifiers(Keycode keycode);
	
	KeycodeAndModifiers(Keycode keycode, const Modifiers& modifiers);

public:
	Keycode getKeycode() const;
	
	void setKeycode(Keycode keycode);
	
	Modifiers getModifiers() const;
	
	void setModifiers(const Modifiers& modifiers);
	
	void setShiftKey();
	
	void clearShiftKey();
	
	sl_bool isShiftKey() const;
	
	void setAltKey();
	
	void clearAltKey();
	
	sl_bool isAltKey() const;
	
	void setOptionKey();
	
	void clearOptionKey();
	
	sl_bool isOptionKey() const;
	
	void setControlKey();
	
	void clearControlKey();
	
	sl_bool isControlKey() const;
	
	void setWindowsKey();
	
	void clearWindowsKey();
	
	sl_bool isWindowsKey() const;
	
	void setCommandKey();
	
	void clearCommandKey();
	
	sl_bool isCommandKey() const;

	String toString() const;
	
};

KeycodeAndModifiers operator|(Keycode keycode, const Modifiers& modifiers);


class SLIB_EXPORT UIEvent : public Referable
{	
protected:
	UIEvent();
	
public:
	static Ref<UIEvent> createKeyEvent(UIAction action, Keycode keycode, sl_uint32 systemKeycode);
	
	static Ref<UIEvent> createMouseEvent(UIAction action, sl_real x, sl_real y);
	
	static Ref<UIEvent> createMouseWheelEvent(sl_real deltaX, sl_real deltaY);
	
	static Ref<UIEvent> createTouchEvent(UIAction action, const Array<TouchPoint>& points);
	
	static Ref<UIEvent> createSetCursorEvent(sl_real x, sl_real y);
	
public:
	UIAction getAction() const;
	
	void setAction(UIAction action);
	
	sl_bool isKeyEvent();
	
	sl_bool isMouseEvent();
	
	sl_bool isTouchEvent();
	
	// key
	const KeycodeAndModifiers& getKeycodeAndModifiers() const;
	
	void setKeycodeAndModifiers(const KeycodeAndModifiers& km);
	
	Keycode getKeycode() const;
	
	void setKeycode(Keycode keycode);
	
	Modifiers getModifiers() const;
	
	void setModifiers(const Modifiers& modifiers);

	sl_uint32 getSystemKeycode() const;
	
	void setSystemKeycode(sl_uint32 keycode);
	
	// mouse, touch
	const Point& getPoint() const;
	
	void setPoint(const Point& pt);
	
	void setPoint(sl_real x, sl_real y);
	
	sl_real getX() const;
	
	void setX(sl_real x);
	
	sl_real getY() const;
	
	void setY(sl_real y);
	
	// mouse wheel
	sl_real getDelta() const;
	
	void setDelta(sl_real delta);
	
	sl_real getDeltaX() const;
	
	void setDeltaX(sl_real x);
	
	sl_real getDeltaY() const;
	
	void setDeltaY(sl_real y);
	
	// touch
	const TouchPoint& getTouchPoint() const;
	
	void setTouchPoint(const TouchPoint& pt);
	
	void setTouchPoint(const Point& pt);
	
	void setTouchPoint(const Point& pt, sl_real pressure);
	
	void setTouchPoint(sl_real x, sl_real y);
	
	void setTouchPoint(sl_real x, sl_real y, sl_real pressure);
	
	sl_real getPressure() const;
	
	void setPressure(sl_real pressure);
	
	Array<TouchPoint> getTouchPoints() const;
	
	sl_uint32 getTouchPointsCount() const;
	
	TouchPoint getTouchPoint(sl_uint32 index) const;
	
	void setTouchPoints(const Array<TouchPoint>& points);

	void transformPoints(const Matrix3& mat);
	
	// modifiers
	void setShiftKey();
	
	void clearShiftKey();
	
	sl_bool isShiftKey() const;
	
	void setAltKey();
	
	void clearAltKey();
	
	sl_bool isAltKey() const;
	
	void setOptionKey();
	
	void clearOptionKey();
	
	sl_bool isOptionKey() const;
	
	void setControlKey();
	
	void clearControlKey();
	
	sl_bool isControlKey() const;
	
	void setWindowsKey();
	
	void clearWindowsKey();
	
	sl_bool isWindowsKey() const;
	
	void setCommandKey();
	
	void clearCommandKey();
	
	sl_bool isCommandKey() const;
	
	// event management
	void resetStatus();
	
	void preventDefault();
	
	sl_bool isPreventedDefault() const;
	
	void setPreventedDefault(sl_bool flag);
	
	void stopPropagation();
	
	sl_bool isStoppedPropagation() const;
	
	void setStoppedPropagation(sl_bool flag);
	
	void passToNext();
	
	sl_bool isPassedToNext();
	
	void setPassedToNext(sl_bool flag);
	
	Ref<UIEvent> duplicate();
	
	static sl_uint32 getSystemKeycode(Keycode key);
	
	static Keycode getKeycodeFromSystemKeycode(sl_uint32 systemKeycode);

protected:
	sl_uint32 m_flags;
	UIAction m_action;
	
	// keyboard
	KeycodeAndModifiers m_keycodeAndModifiers;
	sl_uint32 m_systemKeycode;
	
	// mouse, touch
	TouchPoint m_point;
	
	// touch
	Array<TouchPoint> m_points;
	
};

class SLIB_EXPORT IViewListener
{
public:
	virtual void onMouseEvent(View* view, UIEvent* ev);
	
	virtual void onTouchEvent(View* view, UIEvent* ev);
	
	virtual void onMouseWheelEvent(View* view, UIEvent* ev);
	
	virtual void onKeyEvent(View* view, UIEvent* ev);
	
	virtual void onClick(View* view, UIEvent* ev);
	
	virtual void onSetCursor(View* view, UIEvent* ev);

	virtual void onResize(View* view, sl_real width, sl_real height);
	
	virtual void onShow(View* view);
	
	virtual void onHide(View* view);

};

class SLIB_EXPORT IWindowListener
{
public:
	virtual sl_bool onClose(Window* window);
	
	virtual void onActivate(Window* window);
	
	virtual void onDeactivate(Window* window);
	
	virtual void onMove(Window* window);
	
	virtual void onResize(Window* window, Size& size);
	
	virtual void onMinimize(Window* window);
	
	virtual void onDeminimize(Window* window);
	
	virtual void onMaximize(Window* window);
	
	virtual void onDemaximize(Window* window);

};

class SLIB_EXPORT UIEventLogListener : public Referable, public IWindowListener, public IViewListener
{
public:
	void onMouseEvent(View* view, UIEvent* ev);
	
	void onMouseWheelEvent(View* view, UIEvent* ev);
	
	void onTouchEvent(View* view, UIEvent* ev);
	
	void onKeyEvent(View* view, UIEvent* ev);
	
	void onClick(View* view, UIEvent* ev);
	
	void onSetCursor(View* view, UIEvent* ev);
	
	
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
	void processKey(String name, UIEvent* ev);
	
	void processMouse(String name, UIEvent* ev);
	
	void processMouseWheel(String name, UIEvent* ev);
	
	void processTouch(String name, UIEvent* ev);
	
	String getModifierText(UIEvent* ev);
	
};

SLIB_UI_NAMESPACE_END


SLIB_UI_NAMESPACE_BEGIN

SLIB_INLINE KeycodeAndModifiers::KeycodeAndModifiers() : value(0)
{
}

SLIB_INLINE KeycodeAndModifiers::KeycodeAndModifiers(Keycode keycode, const Modifiers& modifiers) : value((int)keycode | modifiers)
{
}

SLIB_INLINE Keycode KeycodeAndModifiers::getKeycode() const
{
	return (Keycode)(value & 0xFFFF);
}

SLIB_INLINE void KeycodeAndModifiers::setKeycode(Keycode keycode)
{
	value = (value & Modifiers::Mask) | (int)(keycode);
}

SLIB_INLINE Modifiers KeycodeAndModifiers::getModifiers() const
{
	return value & Modifiers::Mask;
}

SLIB_INLINE void KeycodeAndModifiers::setModifiers(const Modifiers& modifiers)
{
	value = modifiers | (value & 0xFFFF);
}

SLIB_INLINE KeycodeAndModifiers operator|(Keycode keycode, int modifiers)
{
	return KeycodeAndModifiers(keycode, modifiers);
}

SLIB_INLINE KeycodeAndModifiers operator|(int modifiers, Keycode keycode)
{
	return KeycodeAndModifiers(keycode, modifiers);
}

SLIB_UI_NAMESPACE_END

#endif
