#ifndef CHECKHEADER_SLIB_RENDER_OBJECT
#define CHECKHEADER_SLIB_RENDER_OBJECT

#include "definition.h"
#include "engine.h"

#include "../core/object.h"
#include "../ui/constants.h"
#include "../ui/event.h"

SLIB_RENDER_NAMESPACE_BEGIN

class RenderObject;
class SLIB_EXPORT IRenderObjectListener
{
public:
	virtual sl_bool onUIEvent(RenderObject* object, UIEvent* event)
	{
		return sl_true;
	}
	
	virtual void onClick(RenderObject* object) {}
};

class SLIB_EXPORT RenderObject : public Object
{
	SLIB_DECLARE_OBJECT(RenderObject, Object)
	
public:
	SLIB_PROPERTY_INLINE(Ptr<IRenderObjectListener>, Listener)
	SLIB_PROPERTY_INLINE(String, Id)

	SLIB_BOOLEAN_PROPERTY_INLINE(Visible)
	SLIB_BOOLEAN_PROPERTY_INLINE(HitTestable)
	SLIB_PROPERTY_INLINE(ButtonState, ButtonState)

public:
	SLIB_INLINE RenderObject()
	{
		setVisible(sl_true);
		setHitTestable(sl_true);
		setButtonState(buttonStateNormal);
	}

	virtual void render(RenderEngine* engine) = 0;

	virtual sl_bool hitTest(const Point& ptWorld)
	{
		return sl_false;
	}

	virtual void onUpdateTransform() {}

public:
	virtual void onUIEvent(UIEvent* event);
	virtual void onClick() {}

	virtual void dispatchUIEvent(UIEvent* event);
	virtual void dispatchClick();

};

SLIB_RENDER_NAMESPACE_END

#endif

