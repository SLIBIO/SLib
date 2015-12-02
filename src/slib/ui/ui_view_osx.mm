#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_OSX)

#include "ui_view_osx.h"

#include "../../../inc/slib/ui/view_group.h"

SLIB_UI_NAMESPACE_BEGIN
/******************************************
			OSX_ViewInstance
******************************************/
OSX_ViewInstance::OSX_ViewInstance()
{
	m_handle = nil;
}

OSX_ViewInstance::~OSX_ViewInstance()
{
	release();
}

void OSX_ViewInstance::release()
{
	UIPlatform::removeViewInstance(m_handle);
	if (m_flagFreeOnRelease) {
		freeHandle(m_handle);
	}
	m_handle = nil;
}

void OSX_ViewInstance::freeHandle(NSView* handle)
{
	if (handle != nil) {
		[handle removeFromSuperview];
	}
}

Ref<OSX_ViewInstance> OSX_ViewInstance::create(NSView* handle, sl_bool flagFreeOnRelease)
{
	Ref<OSX_ViewInstance> ret;
	if (handle != nil) {
		ret = new OSX_ViewInstance();
		if (ret.isNotNull()) {
			ret->m_handle = handle;
			ret->m_flagFreeOnRelease = flagFreeOnRelease;
			UIPlatform::registerViewInstance(handle, ret.get());
		} else {
			if (flagFreeOnRelease) {
				freeHandle(handle);
			}
		}
	}
	return ret;
}

Ref<OSX_ViewInstance> OSX_ViewInstance::create(NSView* handle, NSView* parent, View* view)
{
	Ref<OSX_ViewInstance> instance;
	if (handle != nil) {
		instance = create(handle, sl_true);
		if (instance.isNotNull()) {
			[handle setHidden:(view->isVisible() ? NO : YES)];
			if (!(view->isEnabled())) {
				if ([handle isKindOfClass:[NSControl class]]) {
					NSControl* control = (NSControl*)(handle);
					[control setEnabled:FALSE];
				}
			}
			if ([handle isKindOfClass:[Slib_OSX_ViewBase class]]) {
				((Slib_OSX_ViewBase*)handle)->m_flagOpaque = view->isOpaque();
			}
			if (parent != nil) {
				[parent addSubview:handle];				
			}
		}
	}
	return instance;
}

NSView* OSX_ViewInstance::getHandle()
{
	return m_handle;
}

void OSX_ViewInstance::onDraw(NSRect _rectDirty)
{
	NSView* handle = m_handle;
	
	if (handle != nil) {
		
		NSRect rectBound = [handle bounds];
		Rectangle rectDirty;
		rectDirty.left = (sl_real)(_rectDirty.origin.x);
		rectDirty.top =  (sl_real)(_rectDirty.origin.y);
		rectDirty.right = (sl_real)(rectDirty.left + _rectDirty.size.width);
		rectDirty.bottom = (sl_real)(rectDirty.top + _rectDirty.size.height);
		
		NSGraphicsContext* _graphics = [NSGraphicsContext currentContext];
		
		if (_graphics != nil) {
			
			CGContextRef context = (CGContextRef)([_graphics graphicsPort]);
			
			/* flipping */
			//CGContextTranslateCTM(context, 0, rectBound.size.height);
			//CGContextScaleCTM(context, 1.0f, -1.0f);
			
			Ref<Canvas> canvas = UIPlatform::createCanvas(context, (sl_uint32)(rectBound.size.width), (sl_uint32)(rectBound.size.height));
			if (canvas.isNotNull()) {
				ViewInstance::onDraw(canvas.get());
			}
		}
	}
}

sl_bool OSX_ViewInstance::onEventKey(sl_bool flagDown, NSEvent* event)
{
	NSView* handle = m_handle;
	if (handle != nil) {
		UIEventAction action = flagDown ? actionKeyDown : actionKeyUp;
		sl_uint32 vkey = [event keyCode];
		Keycode key = UIEvent::getKeycodeFromSystemKeycode(vkey);
		Ref<UIEvent> ev = UIEvent::createKeyEvent(action, key, vkey);
		if (ev.isNotNull()) {
			applyModifiers(ev.get(), event);
			onKeyEvent(ev.get());
			return ev->isPreventedDefault();
		}
	}
	return sl_false;
}

sl_bool OSX_ViewInstance::onEventMouse(UIEventAction action, NSEvent* event)
{
	NSView* handle = m_handle;
	if (handle != nil) {
		NSWindow* window = [handle window];
		if (window != nil) {
			NSPoint pw = [event locationInWindow];
			NSPoint pt = [handle convertPoint:pw fromView:nil];
			sl_real x = (sl_real)(pt.x);
			sl_real y = (sl_real)(pt.y);
			Ref<UIEvent> ev = UIEvent::createMouseEvent(action, x, y);
			if (ev.isNotNull()) {
				applyModifiers(ev.get(), event);
				onMouseEvent(ev.get());
				return ev->isPreventedDefault();
			}
		}
	}
	return sl_false;
}

sl_bool OSX_ViewInstance::onEventMouseWheel(NSEvent* event)
{
	NSView* handle = m_handle;
	if (handle != nil) {
		sl_real deltaX = (sl_real)([event deltaX]);
		sl_real deltaY = (sl_real)([event deltaY]);
		if (Math::isNearZero(deltaX) && Math::isNearZero(deltaY)) {
			return sl_false;
		}
		Ref<UIEvent> ev = UIEvent::createMouseWheelEvent(deltaX, deltaY);
		if (ev.isNotNull()) {
			applyModifiers(ev.get(), event);
			onMouseWheelEvent(ev.get());
			return ev->isPreventedDefault();
		}
	}
	return sl_false;
}

sl_bool OSX_ViewInstance::onEventUpdateCursor(NSEvent* event)
{
	NSView* handle = m_handle;
	if (handle != nil) {
		Ref<UIEvent> ev = UIEvent::createSetCursorEvent();
		if (ev.isNotNull()) {
			onSetCursor(ev.get());
			return ev->isPreventedDefault();
		}
	}
	return sl_false;
}

void OSX_ViewInstance::applyModifiers(UIEvent* ev, NSEvent* event)
{
	NSUInteger flags = [event modifierFlags];
	if (flags & NSShiftKeyMask) {
		ev->setShiftKey();
	}
	if (flags & NSAlternateKeyMask) {
		ev->setOptionKey();
	}
	if (flags & NSControlKeyMask) {
		ev->setControlKey();
	}
	if (flags & NSCommandKeyMask) {
		ev->setCommandKey();
	}
}

/******************************************
				View
 ******************************************/
Ref<ViewInstance> View::createInstance(ViewInstance* _parent)
{
	OSX_VIEW_CREATE_INSTANCE_BEGIN
	Slib_OSX_ViewHandle* handle = [[Slib_OSX_ViewHandle alloc] initWithFrame:frame];
	OSX_VIEW_CREATE_INSTANCE_END
	return Ref<ViewInstance>::from(ret);
}

sl_bool View::_isValid()
{
	return sl_true;
}

void View::_setFocus()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		NSWindow* window = [handle window];
		if (window != nil) {
			[window makeFirstResponder:handle];
		}
	}
}

void View::_invalidate()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		[handle setNeedsDisplay: TRUE];
	}
}

void View::_invalidate(const Rectangle& rect)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		NSRect _rect;
		_rect.origin.x = rect.left;
		_rect.origin.y = rect.top;
		_rect.size.width = rect.getWidth();
		_rect.size.height = rect.getHeight();
		[handle setNeedsDisplayInRect: _rect];
	}
}

Rectangle View::getInstanceFrame()
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		NSRect frame = handle.frame;
		Rectangle ret;
		ret.left = frame.origin.x;
		ret.top = frame.origin.y;
		ret.right = ret.left + frame.size.width;
		ret.bottom = ret.top + frame.size.height;
		return ret;
	}
	return Rectangle::zero();
}

void View::_setFrame(const Rectangle& frame)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		NSRect frameNew;
		frameNew.origin.x = frame.left;
		frameNew.origin.y = frame.top;
		frameNew.size.width = frame.getWidth();
		frameNew.size.height = frame.getHeight();
		[handle setFrame:frameNew];
		[handle setNeedsDisplay:YES];
	}
}

void View::_setVisible(sl_bool flag)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		[handle setHidden:(flag ? NO : YES)];
	}
}

void View::_setEnabled(sl_bool flag)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[NSControl class]]) {
			NSControl* control = (NSControl*)handle;
			[control setEnabled:(flag ? YES : NO)];
		}
	}
}

void View::_setOpaque(sl_bool flag)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		if ([handle isKindOfClass:[Slib_OSX_ViewBase class]]) {
			Slib_OSX_ViewBase* control = (Slib_OSX_ViewBase*)handle;
			control->m_flagOpaque = flag;
		}
	}
}

Point View::_convertCoordinateFromScreenToView(const Point& ptScreen)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		NSWindow* window = [handle window];
		if (window != nil) {
			NSScreen* screen = [window screen];
			NSRect rect;
			rect.origin.x = ptScreen.x;
			rect.origin.y = [screen frame].size.height - 1 - ptScreen.y;
			rect.size.width = 0;
			rect.size.height = 0;
			rect = [window convertRectFromScreen:rect];
			NSPoint pw;
			pw.x = rect.origin.x;
			pw.y = rect.origin.y;
			NSPoint pt = [handle convertPoint:pw fromView:nil];
			Point ret;
			ret.x = (sl_real)(pt.x);
			ret.y = (sl_real)(pt.y);
			return ret;
		}
	}
	return ptScreen;
}

Point View::_convertCoordinateFromViewToScreen(const Point& ptView)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		NSWindow* window = [handle window];
		if (window != nil) {
			NSScreen* screen = [window screen];
			NSPoint pt;
			pt.x = ptView.x;
			pt.y = ptView.y;
			NSPoint pw = [handle convertPoint:pt toView:nil];
			NSRect rect;
			rect.origin.x = pw.x;
			rect.origin.y = pw.y;
			rect.size.width = 0;
			rect.size.height = 0;
			rect = [window convertRectToScreen:rect];
			Point ret;
			ret.x = (sl_real)(rect.origin.x);
			ret.y = (sl_real)([screen frame].size.height - 1 - rect.origin.y);
			return ret;
		}
	}
	return ptView;
}

void View::_addChildInstance(ViewInstance* _child)
{
	NSView* handle = UIPlatform::getViewHandle(this);
	if (handle != nil) {
		OSX_ViewInstance* child = (OSX_ViewInstance*)(_child);
		if (child) {
			[handle addSubview:(child->getHandle())];
		}
	}
}

void View::_removeChildInstance(ViewInstance* _child)
{
	OSX_ViewInstance* child = (OSX_ViewInstance*)(_child);
	if (child) {
		[(child->getHandle()) removeFromSuperview];
	}
}
SLIB_UI_NAMESPACE_END

@implementation Slib_OSX_ViewBase
- (BOOL)isFlipped
{
	return TRUE;
}

- (BOOL)isOpaque
{
	return m_flagOpaque ? YES : NO;
}
@end

@implementation Slib_OSX_ViewHandle
- (id)init
{
	self = [super init];
	if (self != nil) {
		[self updateTrackingAreas];
	}
	return self;
}

- (id)initWithFrame:(NSRect)frame
{
	self = [super initWithFrame:frame];
	if (self != nil) {
		[self updateTrackingAreas];
	}
	return self;
}

- (void)setFrame:(NSRect)frame
{
	[super setFrame:frame];
	[self updateTrackingAreas];
}

- (void)updateTrackingAreas
{
	if (m_trackingArea != nil) {
		[self removeTrackingArea:m_trackingArea];
	}
	NSRect rc;
	m_trackingArea = [[NSTrackingArea alloc] initWithRect:rc options: (NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved | NSTrackingCursorUpdate| NSTrackingActiveInKeyWindow | NSTrackingInVisibleRect) owner:self userInfo:nil];
	if (m_trackingArea != nil) {
		[self addTrackingArea:m_trackingArea];
	}
	[super updateTrackingAreas];
}

- (BOOL)acceptsFirstResponder
{
	return TRUE;
}

- (void)drawRect:(NSRect)dirtyRect
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance.lock();
	if (instance.isNotNull()) {
		instance->onDraw(dirtyRect);
	}
}

- (void)keyDown:(NSEvent*)theEvent
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance.lock();
	if (instance.isNotNull()) {
		sl_bool flagNoDefault = instance->onEventKey(sl_true, theEvent);
		if (flagNoDefault) {
			return;
		}
	}
	[[self nextResponder] keyDown:theEvent];
}

- (void)keyUp:(NSEvent*)theEvent
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance.lock();
	if (instance.isNotNull()) {
		sl_bool flagNoDefault = instance->onEventKey(sl_false, theEvent);
		if (flagNoDefault) {
			return;
		}
	}
	[[self nextResponder] keyUp:theEvent];
}

- (void)mouseDown:(NSEvent *)theEvent
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance.lock();
	if (instance.isNotNull()) {
		sl_bool flagNoDefault = instance->onEventMouse(slib::actionLeftButtonDown, theEvent);
		if (flagNoDefault) {
			return;
		}
	}
	[[self nextResponder] mouseDown:theEvent];
}

- (void)mouseUp:(NSEvent *)theEvent
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance.lock();
	if (instance.isNotNull()) {
		instance->onEventMouse(slib::actionLeftButtonUp, theEvent);
		NSInteger clicks = [theEvent clickCount];
		if (clicks == 2) {
			sl_bool flagNoDefault = instance->onEventMouse(slib::actionLeftButtonDoubleClick, theEvent);
			if (flagNoDefault) {
				return;
			}
		}
	}
	[[self nextResponder] mouseUp:theEvent];
}

- (void)mouseDragged:(NSEvent *)theEvent
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance.lock();
	if (instance.isNotNull()) {
		sl_bool flagNoDefault = instance->onEventMouse(slib::actionLeftButtonDrag, theEvent);
		if (flagNoDefault) {
			return;
		}
	}
	[[self nextResponder] mouseDragged:theEvent];
}

- (void)rightMouseDown:(NSEvent *)theEvent
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance.lock();
	if (instance.isNotNull()) {
		sl_bool flagNoDefault = instance->onEventMouse(slib::actionRightButtonDown, theEvent);
		if (flagNoDefault) {
			return;
		}
	}
	[[self nextResponder] rightMouseDown:theEvent];
}

- (void)rightMouseUp:(NSEvent *)theEvent
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance.lock();
	if (instance.isNotNull()) {
		instance->onEventMouse(slib::actionRightButtonUp, theEvent);
		NSInteger clicks = [theEvent clickCount];
		if (clicks == 2) {
			sl_bool flagNoDefault = instance->onEventMouse(slib::actionRightButtonDoubleClick, theEvent);
			if (flagNoDefault) {
				return;
			}
		}
	}
	[[self nextResponder] rightMouseUp:theEvent];
}

- (void)rightMouseDragged:(NSEvent *)theEvent
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance.lock();
	if (instance.isNotNull()) {
		sl_bool flagNoDefault = instance->onEventMouse(slib::actionRightButtonDrag, theEvent);
		if (flagNoDefault) {
			return;
		}
	}
	[[self nextResponder] rightMouseDragged:theEvent];
}

- (void)otherMouseDown:(NSEvent *)theEvent
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance.lock();
	if (instance.isNotNull()) {
		sl_bool flagNoDefault = instance->onEventMouse(slib::actionMiddleButtonDown, theEvent);
		if (flagNoDefault) {
			return;
		}
	}
	[[self nextResponder] otherMouseDown:theEvent];
}

- (void)otherMouseUp:(NSEvent *)theEvent
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance.lock();
	if (instance.isNotNull()) {
		instance->onEventMouse(slib::actionMiddleButtonUp, theEvent);
		NSInteger clicks = [theEvent clickCount];
		if (clicks == 2) {
			sl_bool flagNoDefault = instance->onEventMouse(slib::actionMiddleButtonDoubleClick, theEvent);
			if (flagNoDefault) {
				return;
			}
		}
	}
	[[self nextResponder] otherMouseUp:theEvent];
}

- (void)otherMouseDragged:(NSEvent *)theEvent
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance.lock();
	if (instance.isNotNull()) {
		sl_bool flagNoDefault = instance->onEventMouse(slib::actionMiddleButtonDrag, theEvent);
		if (flagNoDefault) {
			return;
		}
	}
	[[self nextResponder] otherMouseDragged:theEvent];
}

- (void)mouseMoved:(NSEvent *)theEvent
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance.lock();
	if (instance.isNotNull()) {
		sl_bool flagNoDefault = instance->onEventMouse(slib::actionMouseMove, theEvent);
		if (flagNoDefault) {
			return;
		}
	}
	[[self nextResponder] mouseMoved:theEvent];
}

- (void)mouseEntered:(NSEvent *)theEvent
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance.lock();
	if (instance.isNotNull()) {
		sl_bool flagNoDefault = instance->onEventMouse(slib::actionMouseEnter, theEvent);
		if (flagNoDefault) {
			return;
		}
	}
	[[self nextResponder] mouseEntered:theEvent];
}

- (void)mouseExited:(NSEvent *)theEvent
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance.lock();
	if (instance.isNotNull()) {
		sl_bool flagNoDefault = instance->onEventMouse(slib::actionMouseLeave, theEvent);
		if (flagNoDefault) {
			return;
		}
	}
	[[self nextResponder] mouseExited:theEvent];
}

- (void)scrollWheel:(NSEvent *)theEvent
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance.lock();
	if (instance.isNotNull()) {
		sl_bool flagNoDefault = instance->onEventMouseWheel(theEvent);
		if (flagNoDefault) {
			return;
		}
	}
	[[self nextResponder] scrollWheel:theEvent];
}

- (void)cursorUpdate:(NSEvent *)theEvent
{
	slib::Ref<slib::OSX_ViewInstance> instance = m_viewInstance.lock();
	if (instance.isNotNull()) {
		sl_bool flagNoDefault = instance->onEventUpdateCursor(theEvent);
		if (flagNoDefault) {
			return;
		}
	}
	[super cursorUpdate: theEvent];
}
@end


/******************************************
				UIPlatform
 ******************************************/
SLIB_UI_NAMESPACE_BEGIN
Ref<ViewInstance> UIPlatform::createViewInstance(NSView* handle, sl_bool flagFreeOnRelease)
{
	Ref<ViewInstance> ret = UIPlatform::_getViewInstance((__bridge void*)handle);
	if (ret.isNotNull()) {
		return ret;
	}
	ret = Ref<ViewInstance>::from(OSX_ViewInstance::create(handle, flagFreeOnRelease));
	return Ref<ViewInstance>::from(ret);
}

void UIPlatform::registerViewInstance(NSView* handle, ViewInstance* instance)
{
	UIPlatform::_registerViewInstance((__bridge void*)handle, instance);
}

Ref<ViewInstance> UIPlatform::getViewInstance(NSView* handle)
{
	return UIPlatform::_getViewInstance((__bridge void*)handle);
}

void UIPlatform::removeViewInstance(NSView* handle)
{
	UIPlatform::_removeViewInstance((__bridge void*)handle);
}

NSView* UIPlatform::getViewHandle(ViewInstance* _instance)
{
	OSX_ViewInstance* osx_instance = (OSX_ViewInstance*)_instance;
	if (osx_instance) {
		return osx_instance->getHandle();
	} else {
		return nil;
	}
}

NSView* UIPlatform::getViewHandle(View* view)
{
	if (view) {
		Ref<ViewInstance> instance = view->getViewInstance();
		if (instance.isNotNull()) {
			OSX_ViewInstance* osx_instance = (OSX_ViewInstance*)(instance.get());
			return osx_instance->getHandle();
		}
	}
	return nil;
}
SLIB_UI_NAMESPACE_END

#endif
