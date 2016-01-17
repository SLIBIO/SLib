#include "../../../inc/slib/render/engine.h"
#include "../../../inc/slib/render/object.h"

SLIB_RENDER_NAMESPACE_BEGIN

void RenderObject::onUIEvent(UIEvent* event)
{
	if (getButtonState() == buttonState_Disabled) {
		return;
	}
	UIEventAction action = event->getAction();
	if (action == actionLeftButtonDown || action == actionTouchBegin) {
		setButtonState(buttonState_Down);
	} else if (action == actionMouseMove || action == actionTouchMove) {
		if (hitTest(event->getPoint())) {
			setButtonState(buttonState_Hover);
		} else {
			setButtonState(buttonState_Normal);
		}
	} else if (action == actionLeftButtonUp || action == actionTouchEnd || action == actionTouchCancel) {
		setButtonState(buttonState_Normal);
		if (hitTest(event->getPoint())) {
			dispatchClick();
		}
	}
}

void RenderObject::dispatchUIEvent(UIEvent* event)
{
	onUIEvent(event);
	if (event->isStoppedPropagation()) {
		return;
	}
	PtrLocker<IRenderObjectListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onUIEvent(this, event);
	}
}

void RenderObject::dispatchClick()
{
	onClick();
	PtrLocker<IRenderObjectListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onClick(this);
	}
}
SLIB_RENDER_NAMESPACE_END
