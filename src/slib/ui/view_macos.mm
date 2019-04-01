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

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_MACOS)

#include "view_macos.h"

#include "slib/ui/core.h"
#include "slib/ui/view.h"

#include "slib/math/transform2d.h"

namespace slib
{
	
	NSRect _priv_macOS_getViewFrameAndTransform(const UIRect& frame, const Matrix3& transform, sl_real& rotation)
	{
		rotation = Transform2::getRotationAngleFromMatrix(transform);
		Vector2 translation = Transform2::getTranslationFromMatrix(transform);
		NSRect ret;
		ret.origin.x = frame.left + translation.x;
		ret.origin.y = frame.top + translation.y;
		ret.size.width = frame.getWidth();
		ret.size.height = frame.getHeight();
		if (!(Math::isAlmostZero(rotation))) {
			sl_real ax = frame.getWidth() / 2;
			sl_real ay = frame.getHeight() / 2;
			sl_real cr = Math::cos(rotation);
			sl_real sr = Math::sin(rotation);
			ret.origin.x += (- ax * cr + ay * sr) + ax;
			ret.origin.y += (- ax * sr - ay * cr) + ay;
		}
		return ret;
	}
	

/******************************************
			macOS_ViewInstance
******************************************/

	macOS_ViewInstance::macOS_ViewInstance()
	{
		m_handle = nil;
		m_transform = Matrix3::identity();
	}

	macOS_ViewInstance::~macOS_ViewInstance()
	{
		_release();
	}

	void macOS_ViewInstance::_release()
	{
		UIPlatform::removeViewInstance(m_handle);
		m_handle = nil;
	}

	Ref<macOS_ViewInstance> macOS_ViewInstance::create(NSView* handle)
	{
		if (handle != nil) {
			Ref<macOS_ViewInstance> ret = new macOS_ViewInstance();
			if (ret.isNotNull()) {
				ret->m_handle = handle;
				UIPlatform::registerViewInstance(handle, ret.get());
				return ret;
			}
		}
		return sl_null;
	}

	Ref<macOS_ViewInstance> macOS_ViewInstance::create(NSView* handle, NSView* parent, View* view)
	{
		if (handle != nil) {
			
			Ref<macOS_ViewInstance> instance = create(handle);
			
			if (instance.isNotNull()) {
				
				[handle setHidden:(view->isVisibleInInstance() ? NO : YES)];
				
				if (!(view->isEnabled())) {
					if ([handle isKindOfClass:[NSControl class]]) {
						NSControl* control = (NSControl*)(handle);
						[control setEnabled:FALSE];
					}
				}
				if ([handle isKindOfClass:[_priv_Slib_macOS_ViewBase class]]) {
					((_priv_Slib_macOS_ViewBase*)handle)->m_flagOpaque = view->isOpaque();
					((_priv_Slib_macOS_ViewBase*)handle)->m_flagClipping = view->isClipping();
					((_priv_Slib_macOS_ViewBase*)handle)->m_flagDrawing = view->isDrawing();
				}
				
				if (parent != nil) {
					[parent addSubview:handle];				
				}
				
				return instance;
				
			}
		}
		
		return sl_null;
		
	}

	NSView* macOS_ViewInstance::getHandle()
	{
		return m_handle;
	}

	sl_bool macOS_ViewInstance::isValid()
	{
		return sl_true;
	}

	void macOS_ViewInstance::setFocus(sl_bool flagFocus)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			NSWindow* window = [handle window];
			if (window != nil) {
				if (!(UI::isUiThread())) {
					UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), macOS_ViewInstance, setFocus, this, flagFocus));
					return;
				}
				if (flagFocus) {
					[window makeFirstResponder:handle];
				} else {
					if (window.firstResponder == handle) {
						[window makeFirstResponder:nil];
					}
				}
			}
		}
	}

	void macOS_ViewInstance::invalidate()
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			if (UI::isUiThread()) {
				[handle setNeedsDisplay: YES];
				dispatch_async(dispatch_get_main_queue(), ^{
					[handle displayIfNeeded];
				});
			} else {
				dispatch_async(dispatch_get_main_queue(), ^{
					[handle setNeedsDisplay: YES];
					[handle displayIfNeeded];
				});
			}
		}
	}

	void macOS_ViewInstance::invalidate(const UIRect& rect)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			NSRect _rect;
			_rect.origin.x = (CGFloat)(rect.left);
			_rect.origin.y = (CGFloat)(rect.top);
			_rect.size.width = (CGFloat)(rect.getWidth());
			_rect.size.height = (CGFloat)(rect.getHeight());
			if (UI::isUiThread()) {
				[handle setNeedsDisplayInRect: _rect];
				dispatch_async(dispatch_get_main_queue(), ^{
					[handle displayIfNeeded];
				});
			} else {
				dispatch_async(dispatch_get_main_queue(), ^{
					[handle setNeedsDisplayInRect: _rect];
					[handle displayIfNeeded];
				});
			}
		}
	}

	UIRect macOS_ViewInstance::getFrame()
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			NSRect frame = handle.frame;
			UIRect ret;
			ret.left = (sl_ui_pos)(frame.origin.x);
			ret.top = (sl_ui_pos)(frame.origin.y);
			ret.right = ret.left + (sl_ui_pos)(frame.size.width);
			ret.bottom = ret.top + (sl_ui_pos)(frame.size.height);
			ret.fixSizeError();
			return ret;
		}
		return UIRect::zero();
	}

	void macOS_ViewInstance::setFrame(const UIRect& frame)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), macOS_ViewInstance, setFrame, this, frame));
			return;
		}
		m_frame = frame;
		updateFrameAndTransform();
	}

	void macOS_ViewInstance::setTransform(const Matrix3 &transform)
	{
		if (!(UI::isUiThread())) {
			UI::dispatchToUiThread(SLIB_BIND_WEAKREF(void(), macOS_ViewInstance, setTransform, this, transform));
			return;
		}
		m_transform = transform;
		updateFrameAndTransform();
	}

	void macOS_ViewInstance::setVisible(sl_bool flag)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			if (UI::isUiThread()) {
				[handle setHidden:(flag ? NO : YES)];
			} else {
				dispatch_async(dispatch_get_main_queue(), ^{
					[handle setHidden:(flag ? NO : YES)];
				});
			}
		}
	}

	void macOS_ViewInstance::setEnabled(sl_bool flag)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			if ([handle isKindOfClass:[NSControl class]]) {
				NSControl* control = (NSControl*)handle;
				if (UI::isUiThread()) {
					[control setEnabled:(flag ? YES : NO)];
				} else {
					dispatch_async(dispatch_get_main_queue(), ^{
						[control setEnabled:(flag ? YES : NO)];
					});
				}
			}
		}
	}

	void macOS_ViewInstance::setOpaque(sl_bool flag)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			if ([handle isKindOfClass:[_priv_Slib_macOS_ViewBase class]]) {
				_priv_Slib_macOS_ViewBase* control = (_priv_Slib_macOS_ViewBase*)handle;
				control->m_flagOpaque = flag;
			}
		}
	}

	void macOS_ViewInstance::setAlpha(sl_real alpha)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			if (UI::isUiThread()) {
				[handle setNeedsDisplay: TRUE];
			} else {
				dispatch_async(dispatch_get_main_queue(), ^{
					[handle setNeedsDisplay: TRUE];
				});
			}
		}
	}
	
	void macOS_ViewInstance::setClipping(sl_bool flag)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			if ([handle isKindOfClass:[_priv_Slib_macOS_ViewBase class]]) {
				_priv_Slib_macOS_ViewBase* control = (_priv_Slib_macOS_ViewBase*)handle;
				control->m_flagClipping = flag;
			}
		}
	}
	
	void macOS_ViewInstance::setDrawing(sl_bool flag)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			if ([handle isKindOfClass:[_priv_Slib_macOS_ViewBase class]]) {
				_priv_Slib_macOS_ViewBase* control = (_priv_Slib_macOS_ViewBase*)handle;
				control->m_flagDrawing = flag;
			}
		}
	}
	
	UIPointf macOS_ViewInstance::convertCoordinateFromScreenToView(const UIPointf& ptScreen)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			NSWindow* window = [handle window];
			if (window != nil) {
				NSScreen* screen = [window screen];
				NSRect rect;
				rect.origin.x = (CGFloat)(ptScreen.x);
				rect.origin.y = [screen frame].size.height - 1 - (CGFloat)(ptScreen.y);
				rect.size.width = 0;
				rect.size.height = 0;
				rect = [window convertRectFromScreen:rect];
				NSPoint pw;
				pw.x = rect.origin.x;
				pw.y = rect.origin.y;
				NSPoint pt = [handle convertPoint:pw fromView:nil];
				UIPointf ret;
				ret.x = (sl_ui_posf)(pt.x);
				ret.y = (sl_ui_posf)(pt.y);
				return ret;
			}
		}
		return ptScreen;
	}

	UIPointf macOS_ViewInstance::convertCoordinateFromViewToScreen(const UIPointf& ptView)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			NSWindow* window = [handle window];
			if (window != nil) {
				NSScreen* screen = [window screen];
				NSPoint pt;
				pt.x = (CGFloat)(ptView.x);
				pt.y = (CGFloat)(ptView.y);
				NSPoint pw = [handle convertPoint:pt toView:nil];
				NSRect rect;
				rect.origin.x = pw.x;
				rect.origin.y = pw.y;
				rect.size.width = 0;
				rect.size.height = 0;
				rect = [window convertRectToScreen:rect];
				UIPointf ret;
				ret.x = (sl_ui_posf)(rect.origin.x);
				ret.y = (sl_ui_posf)([screen frame].size.height - 1 - rect.origin.y);
				return ret;
			}
		}
		return ptView;
	}

	void macOS_ViewInstance::addChildInstance(const Ref<ViewInstance>& _child)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			macOS_ViewInstance* child = (macOS_ViewInstance*)(_child.get());
			if (child) {
				NSView* child_handle = child->m_handle;
				if (child_handle != nil) {
					[handle addSubview:child_handle];
				}
			}
		}
	}

	void macOS_ViewInstance::removeChildInstance(const Ref<ViewInstance>& _child)
	{
		macOS_ViewInstance* child = (macOS_ViewInstance*)(_child.get());
		if (child) {
			NSView* child_handle = child->m_handle;
			if (child_handle != nil) {
				[child_handle removeFromSuperview];
			}
		}
	}

	void macOS_ViewInstance::bringToFront()
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			NSView* parent = handle.superview;
			if (parent != nil) {
				if (UI::isUiThread()) {
					[handle removeFromSuperviewWithoutNeedingDisplay];
					[parent addSubview:handle];
				} else {
					dispatch_async(dispatch_get_main_queue(), ^{
						[handle removeFromSuperviewWithoutNeedingDisplay];
						[parent addSubview:handle];
					});
				}
			}
		}
	}

	void macOS_ViewInstance::onDraw(NSRect rcDirty)
	{
		NSView* handle = m_handle;
		
		if (handle != nil) {
			
			NSGraphicsContext* graphics = [NSGraphicsContext currentContext];
			
			if (graphics != nil) {
				
				CGContextRef context = (CGContextRef)([graphics graphicsPort]);
				
				NSRect rectBound = [handle bounds];
				Ref<Canvas> canvas = GraphicsPlatform::createCanvas(CanvasType::View, context, (sl_uint32)(rectBound.size.width), (sl_uint32)(rectBound.size.height));
				if (canvas.isNotNull()) {
					canvas->setInvalidatedRect(Rectangle((sl_real)(rcDirty.origin.x), (sl_real)(rcDirty.origin.y), (sl_real)(rcDirty.origin.x + rcDirty.size.width), (sl_real)(rcDirty.origin.y + rcDirty.size.height)));
					ViewInstance::onDraw(canvas.get());
				}
				
			}
		}
	}

	UIEventFlags macOS_ViewInstance::onEventKey(sl_bool flagDown, NSEvent* event)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			UIAction action = flagDown ? UIAction::KeyDown : UIAction::KeyUp;
			sl_uint32 vkey = [event keyCode];
			Keycode key = UIEvent::getKeycodeFromSystemKeycode(vkey);
			Time t;
			t.setSecondsCountf([event timestamp]);
			Ref<UIEvent> ev = UIEvent::createKeyEvent(action, key, vkey, t);
			if (ev.isNotNull()) {
				applyModifiers(ev.get(), event);
				onKeyEvent(ev.get());
				return ev->getFlags();
			}
		}
		return 0;
	}

	UIEventFlags macOS_ViewInstance::onEventMouse(UIAction action, NSEvent* event)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			NSWindow* window = [handle window];
			if (window != nil) {
				NSPoint pw = [event locationInWindow];
				NSPoint pt = [handle convertPoint:pw fromView:nil];
				sl_ui_posf x = (sl_ui_posf)(pt.x);
				sl_ui_posf y = (sl_ui_posf)(pt.y);
				Time t;
				t.setSecondsCountf([event timestamp]);
				Ref<UIEvent> ev = UIEvent::createMouseEvent(action, x, y, t);
				if (ev.isNotNull()) {
					applyModifiers(ev.get(), event);
					onMouseEvent(ev.get());
					return ev->getFlags();
				}
			}
		}
		return 0;
	}

	UIEventFlags macOS_ViewInstance::onEventMouse(UIAction action, const NSPoint& pt)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			NSWindow* window = [handle window];
			if (window != nil) {
				sl_ui_posf x = (sl_ui_posf)(pt.x);
				sl_ui_posf y = (sl_ui_posf)(pt.y);
				Time t;
				t.setSecondsCountf([[NSProcessInfo processInfo] systemUptime]);
				Ref<UIEvent> ev = UIEvent::createMouseEvent(action, x, y, t);
				if (ev.isNotNull()) {
					onMouseEvent(ev.get());
					return ev->getFlags();
				}
			}
		}
		return 0;
	}

	UIEventFlags macOS_ViewInstance::onEventMouseWheel(NSEvent* event)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			sl_real deltaX = (sl_real)([event deltaX]);
			sl_real deltaY = (sl_real)([event deltaY]);
			if (Math::isAlmostZero(deltaX) && Math::isAlmostZero(deltaY)) {
				return sl_false;
			}
			NSPoint pw = [event locationInWindow];
			NSPoint pt = [handle convertPoint:pw fromView:nil];
			sl_ui_posf x = (sl_ui_posf)(pt.x);
			sl_ui_posf y = (sl_ui_posf)(pt.y);
			Time t;
			t.setSecondsCountf([event timestamp]);
			Ref<UIEvent> ev = UIEvent::createMouseWheelEvent(x, y, deltaX, deltaY, t);
			if (ev.isNotNull()) {
				applyModifiers(ev.get(), event);
				onMouseWheelEvent(ev.get());
				return ev->getFlags();
			}
		}
		return 0;
	}

	UIEventFlags macOS_ViewInstance::onEventUpdateCursor(NSEvent* event)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			NSPoint pw = [event locationInWindow];
			NSPoint pt = [handle convertPoint:pw fromView:nil];
			sl_ui_posf x = (sl_ui_posf)(pt.x);
			sl_ui_posf y = (sl_ui_posf)(pt.y);
			Time t;
			t.setSecondsCountf([event timestamp]);
			Ref<UIEvent> ev = UIEvent::createSetCursorEvent(x, y, t);
			if (ev.isNotNull()) {
				onSetCursor(ev.get());
				return ev->getFlags();
			}
		}
		return 0;
	}

	void macOS_ViewInstance::applyModifiers(UIEvent* ev, NSEvent* event)
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

	void macOS_ViewInstance::updateFrameAndTransform()
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			sl_real rotation = 0;
			handle.frame = _priv_macOS_getViewFrameAndTransform(m_frame, m_transform, rotation);
			handle.frameRotation = Math::getDegreesFromRadian(rotation);
			[handle setNeedsDisplay:YES];
		}
	}

/******************************************
				View
******************************************/

	Ref<ViewInstance> View::createGenericInstance(ViewInstance* _parent)
	{
		MACOS_VIEW_CREATE_INSTANCE_BEGIN
		_priv_Slib_macOS_ViewHandle* handle = [[_priv_Slib_macOS_ViewHandle alloc] initWithFrame:frame];
		MACOS_VIEW_CREATE_INSTANCE_END
		return ret;
	}

}

@implementation _priv_Slib_macOS_ViewBase

- (BOOL)isFlipped
{
	return TRUE;
}

- (BOOL)isOpaque
{
	return m_flagOpaque ? YES : NO;
}

@end

@implementation _priv_Slib_macOS_ViewHandle

MACOS_VIEW_DEFINE_ON_FOCUS

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
	NSRect rc = [self bounds];
	m_trackingArea = [[NSTrackingArea alloc] initWithRect:rc options: (NSTrackingMouseEnteredAndExited | NSTrackingMouseMoved | NSTrackingActiveInKeyWindow | NSTrackingInVisibleRect) owner:self userInfo:nil];
	if (m_trackingArea != nil) {
		[self addTrackingArea:m_trackingArea];
		slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
		if (instance.isNotNull()) {
			NSPoint mouseLocation = [[self window] mouseLocationOutsideOfEventStream];
			mouseLocation = [self convertPoint: mouseLocation fromView: nil];
			if (NSPointInRect(mouseLocation, [self bounds])) {
				instance->onEventMouse(slib::UIAction::MouseEnter, mouseLocation);
			} else {
				instance->onEventMouse(slib::UIAction::MouseLeave, mouseLocation);
			}
		}
	}
	[super updateTrackingAreas];
}

- (BOOL)acceptsFirstResponder
{
	return TRUE;
}

- (void)drawRect:(NSRect)dirtyRect
{
	if (m_flagDrawing) {
		slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
		if (instance.isNotNull()) {
			instance->onDraw(dirtyRect);
		}
	}
}

- (void)keyDown:(NSEvent*)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::UIEventFlags flags = instance->onEventKey(sl_true, theEvent);
		if (flags & slib::UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] keyDown:theEvent];
}

- (void)keyUp:(NSEvent*)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::UIEventFlags flags = instance->onEventKey(sl_false, theEvent);
		if (flags & slib::UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] keyUp:theEvent];
}

- (void)mouseDown:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::UIEventFlags flags = instance->onEventMouse(slib::UIAction::LeftButtonDown, theEvent);
		if (flags & slib::UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] mouseDown:theEvent];
}

- (void)mouseUp:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::UIEventFlags flags = instance->onEventMouse(slib::UIAction::LeftButtonUp, theEvent);
		NSInteger clicks = [theEvent clickCount];
		if (clicks == 2) {
			flags |= instance->onEventMouse(slib::UIAction::LeftButtonDoubleClick, theEvent);
		}
		if (flags & slib::UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] mouseUp:theEvent];
}

- (void)mouseDragged:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::UIEventFlags flags = instance->onEventMouse(slib::UIAction::LeftButtonDrag, theEvent);
		if (flags & slib::UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] mouseDragged:theEvent];
}

- (void)rightMouseDown:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::UIEventFlags flags = instance->onEventMouse(slib::UIAction::RightButtonDown, theEvent);
		if (flags & slib::UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] rightMouseDown:theEvent];
}

- (void)rightMouseUp:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::UIEventFlags flags = instance->onEventMouse(slib::UIAction::RightButtonUp, theEvent);
		NSInteger clicks = [theEvent clickCount];
		if (clicks == 2) {
			flags |= instance->onEventMouse(slib::UIAction::RightButtonDoubleClick, theEvent);
		}
		if (flags & slib::UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] rightMouseUp:theEvent];
}

- (void)rightMouseDragged:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::UIEventFlags flags = instance->onEventMouse(slib::UIAction::RightButtonDrag, theEvent);
		if (flags & slib::UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] rightMouseDragged:theEvent];
}

- (void)otherMouseDown:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::UIEventFlags flags = instance->onEventMouse(slib::UIAction::MiddleButtonDown, theEvent);
		if (flags & slib::UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] otherMouseDown:theEvent];
}

- (void)otherMouseUp:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::UIEventFlags flags = instance->onEventMouse(slib::UIAction::MiddleButtonUp, theEvent);
		NSInteger clicks = [theEvent clickCount];
		if (clicks == 2) {
			flags |= instance->onEventMouse(slib::UIAction::MiddleButtonDoubleClick, theEvent);
		}
		if (flags & slib::UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] otherMouseUp:theEvent];
}

- (void)otherMouseDragged:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::UIEventFlags flags = instance->onEventMouse(slib::UIAction::MiddleButtonDrag, theEvent);
		if (flags & slib::UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] otherMouseDragged:theEvent];
}

- (void)mouseMoved:(NSEvent *)theEvent
{
	[[self window] invalidateCursorRectsForView:self];
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::UIEventFlags flags = instance->onEventMouse(slib::UIAction::MouseMove, theEvent);
		if (flags & slib::UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] mouseMoved:theEvent];
}

- (void)mouseEntered:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::UIEventFlags flags = instance->onEventMouse(slib::UIAction::MouseEnter, theEvent);
		if (flags & slib::UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] mouseEntered:theEvent];
}

- (void)mouseExited:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::UIEventFlags flags = instance->onEventMouse(slib::UIAction::MouseLeave, theEvent);
		if (flags & slib::UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] mouseExited:theEvent];
}

- (void)scrollWheel:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::UIEventFlags flags = instance->onEventMouseWheel(theEvent);
		if (flags & slib::UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] scrollWheel:theEvent];
}

- (void)cursorUpdate:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::UIEventFlags flags = instance->onEventUpdateCursor(theEvent);
		if (flags & slib::UIEventFlags::PreventDefault) {
			return;
		}
	}
	[super cursorUpdate: theEvent];
}

- (NSView *)hitTest:(NSPoint)aPoint
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> view = instance->getView();
		if (view.isNotNull()) {
			if (!(view->isEnabled())) {
				return nil;
			}
			slib::Function<sl_bool(const slib::UIPoint&)> hitTestCapture(view->getCapturingChildInstanceEvents());
			if (hitTestCapture.isNotNull()) {
				if (hitTestCapture(slib::UIPoint((sl_ui_pos)(aPoint.x), (sl_ui_pos)(aPoint.y)))) {
					return self;
				}
			}
		}
	}
	return [super hitTest:aPoint];
}

@end


/******************************************
				UIPlatform
******************************************/

namespace slib
{

	Ref<ViewInstance> UIPlatform::createViewInstance(NSView* handle)
	{
		Ref<ViewInstance> ret = UIPlatform::_getViewInstance((__bridge void*)handle);
		if (ret.isNotNull()) {
			return ret;
		}
		return macOS_ViewInstance::create(handle);
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
		macOS_ViewInstance* instance = (macOS_ViewInstance*)_instance;
		if (instance) {
			return instance->getHandle();
		} else {
			return nil;
		}
	}

	NSView* UIPlatform::getViewHandle(View* view)
	{
		if (view) {
			Ref<ViewInstance> instance = view->getViewInstance();
			if (instance.isNotNull()) {
				macOS_ViewInstance* _instance = (macOS_ViewInstance*)(instance.get());
				return _instance->getHandle();
			}
		}
		return nil;
	}

}

#endif
