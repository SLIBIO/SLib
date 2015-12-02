#ifndef CHECKHEADER_SLIB_UI_VIEW
#define CHECKHEADER_SLIB_UI_VIEW

#include "definition.h"
#include "constants.h"
#include "event.h"

#include "../core/object.h"
#include "../core/pointer.h"
#include "../graphics/canvas.h"

SLIB_UI_NAMESPACE_BEGIN

class Window;
class ViewInstance;

class SLIB_EXPORT View : public Object
{
	SLIB_DECLARE_OBJECT(View, Object)
public:
	View();
	~View();
	
public:
	sl_bool isGroup();
	void setGroup(sl_bool flag);
	
	sl_bool isValid();

	virtual void setFocus();
	
	virtual void invalidate();
	virtual void invalidate(const Rectangle& rect);
	
	const Rectangle& getFrame();
	Rectangle getInstanceFrame();
	virtual void setFrame(const Rectangle& frame);
	
	sl_bool isVisible();
	virtual void setVisible(sl_bool flag);
	
	sl_bool isEnabled();
	virtual void setEnabled(sl_bool flag);
	
	sl_bool isOpaque();
	virtual void setOpaque(sl_bool flag);
	
	virtual Point convertCoordinateFromScreenToView(const Point& ptScreen);
	virtual Point convertCoordinateFromViewToScreen(const Point& ptView);
	
	virtual void addChildInstance(const Ref<ViewInstance>& instance);
	virtual void removeChildInstance(const Ref<ViewInstance>& instance);

protected:
	sl_bool _isValid();

	void _setFocus();
	
	void _invalidate();
	void _invalidate(const Rectangle& rect);
	
	void _setFrame(const Rectangle& frame);
	
	void _setVisible(sl_bool flag);
	
	void _setEnabled(sl_bool flag);
	
	void _setOpaque(sl_bool flag);
	
	Point _convertCoordinateFromScreenToView(const Point& ptScreen);
	Point _convertCoordinateFromViewToScreen(const Point& ptView);
	
	void _addChildInstance(ViewInstance* instance);
	void _addChildInstance_(Ref<ViewInstance> instance);

	void _removeChildInstance(ViewInstance* instance);
	void _removeChildInstance_(Ref<ViewInstance> instance);
	
public:
	const Ref<ViewInstance>& getViewInstance();
	
	Ref<Window> getWindow();
	void setWindow(const Ref<Window>& window);
	
	Ref<View> getParent();
	void setParent(const Ref<View>& parent);
	void removeParent(View* parent);
	
	void setFrame(sl_real x, sl_real y, sl_real width, sl_real height);
	
	sl_real getWidth();
	sl_real getHeight();
	
	Size getSize();
	void setSize(const Size& size);
	void setSize(sl_real width, sl_real height);
	
	sl_real getX();
	sl_real getY();
	
	Point getPosition();
	void setPosition(const Point& point);
	void setPosition(sl_real x, sl_real y);
	
	Rectangle getFrameBounds();
	virtual Rectangle getContentBounds();
	
	virtual void attach(const Ref<ViewInstance>& instance);
	virtual void detach();
	
	void attachChild(const Ref<View>& child);
	void _attachChild(const Ref<View> child);
	Ref<ViewInstance> attachToNewInstance(const Ref<ViewInstance>& parent);
	
public:
	SLIB_PROPERTY_INLINE(Ptr<IViewListener>, EventListener)
	
protected:
	virtual void onDraw(Canvas* canvas);

	virtual void onClick();
	virtual void onKeyEvent(UIEvent* event);
	virtual void onMouseEvent(UIEvent* event);
	virtual void onTouchEvent(UIEvent* event);
	virtual void onMouseWheelEvent(UIEvent* event);
	virtual void onSetCursor(UIEvent* event);
	
	virtual void onResize();
	virtual void onResizeChild(View* child);
	
	virtual void onAttach();

public:
	virtual void dispatchDraw(Canvas* canvas);
	virtual void dispatchClick();
	virtual void dispatchKeyEvent(UIEvent* event);
	virtual void dispatchMouseEvent(UIEvent* event);
	virtual void dispatchTouchEvent(UIEvent* event);
	virtual void dispatchMouseWheelEvent(UIEvent* event);
	virtual void dispatchSetCursor(UIEvent* event);

protected:
	virtual Ref<ViewInstance> createInstance(ViewInstance* parent);
	
private:
	Ref<ViewInstance> m_instance;
	WeakRef<Window> m_window;
	WeakRef<View> m_parent;

	sl_bool m_flagGroup;
	Rectangle m_frame;
	sl_bool m_flagVisible;
	sl_bool m_flagEnabled;
	sl_bool m_flagOpaque;
	
protected:
	friend class Window;
};

class SLIB_EXPORT ViewInstance : public Object
{
	SLIB_DECLARE_OBJECT(ViewInstance, Object)
protected:
	ViewInstance();
public:
	~ViewInstance();

public:
	Ref<View> getView();
	void setView(View* view);
	
public:
	void onDraw(Canvas* canvas);
	
	void onClick();
	void onKeyEvent(UIEvent* event);
	void onMouseEvent(UIEvent* event);
	void onTouchEvent(UIEvent* event);
	void onMouseWheelEvent(UIEvent* event);
	void onSetCursor(UIEvent* event);
	
protected:
	WeakRef<View> m_view;
	WeakRef<GraphicsContext> m_graphicsContext;

};

SLIB_UI_NAMESPACE_END

#endif
