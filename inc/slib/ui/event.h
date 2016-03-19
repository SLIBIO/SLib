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

public:
	TouchPoint() = default;

	TouchPoint(const TouchPoint& other) = default;
	
	TouchPoint(const Point& point);
	
	TouchPoint(const Point& point, sl_real pressure);
	
	TouchPoint(sl_real x, sl_real y);
	
	TouchPoint(sl_real x, sl_real y, sl_real pressure);
	
public:
	TouchPoint& operator=(const TouchPoint& other) = default;

};

class SLIB_EXPORT UIEvent : public Referable
{	
protected:
	UIEvent();
	
public:
	static Ref<UIEvent> createKeyEvent(UIAction action, Keycode keycode, sl_uint32 systemKeycode);
	
	static Ref<UIEvent> createMouseEvent(UIAction action, sl_real x, sl_real y);
	
	static Ref<UIEvent> createMouseWheelEvent(sl_real deltaX, sl_real deltaY);
	
	static Ref<UIEvent> createTouchEvent(UIAction action, const Array<TouchPoint>& points);
	
	static Ref<UIEvent> createSetCursorEvent();
	
public:
	UIAction getAction() const;
	
	void setAction(UIAction action);
	
	sl_bool isKeyEvent();
	
	sl_bool isMouseEvent();
	
	sl_bool isTouchEvent();
	
	// key
	Keycode getKeycode() const;
	
	void setKeycode(Keycode keycode);
	
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
	void preventDefault();
	
	sl_bool isPreventedDefault() const;
	
	void stopPropagation();
	
	sl_bool isStoppedPropagation() const;
	
	Ref<UIEvent> duplicate();
	
	static sl_uint32 getSystemKeycode(Keycode key);
	
	static Keycode getKeycodeFromSystemKeycode(sl_uint32 systemKeycode);

protected:
	sl_uint32 m_flags;
	UIAction m_action;
	
	// keyboard
	Keycode m_keycode;
	sl_uint32 m_systemKeycode;
	
	// mouse, touch
	TouchPoint m_point;
	
	// touch
	Array<TouchPoint> m_points;
	
};

class SLIB_EXPORT IViewListener
{
public:
	virtual void onClick(View* view);
	
	virtual void onKeyEvent(View* view, UIEvent* event);
	
	virtual void onMouseEvent(View* view, UIEvent* event);
	
	virtual void onTouchEvent(View* view, UIEvent* event);
	
	virtual void onMouseWheelEvent(View* view, UIEvent* event);
	
	virtual void onSetCursor(View* view, UIEvent* event);

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
	
};

SLIB_UI_NAMESPACE_END

#endif
