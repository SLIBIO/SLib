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

#include "slib/ui/view.h"

#include "slib/math/transform2d.h"

namespace slib
{
	
	SLIB_DEFINE_OBJECT(macOS_ViewInstance, ViewInstance)

	macOS_ViewInstance::macOS_ViewInstance()
	{
		m_handle = nil;
		m_frame.setZero();
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

	void macOS_ViewInstance::initialize(NSView* handle)
	{
		m_handle = handle;
		UIPlatform::registerViewInstance(handle, this);
	}

	void macOS_ViewInstance::initialize(NSView* handle, NSView* parent, View* view)
	{
		initialize(handle);

		if (view->isCreatingNativeLayer()) {
			[handle setWantsLayer:YES];
		}
		
		[handle setHidden:(view->isVisibleInInstance() ? NO : YES)];
		if (!(view->isEnabled())) {
			if ([handle isKindOfClass:[NSControl class]]) {
				NSControl* control = (NSControl*)(handle);
				[control setEnabled:FALSE];
			}
		}
		if ([handle isKindOfClass:[SLIBViewBaseHandle class]]) {
			((SLIBViewBaseHandle*)handle)->m_flagOpaque = view->isOpaque();
			((SLIBViewBaseHandle*)handle)->m_flagClipping = view->isClipping();
			((SLIBViewBaseHandle*)handle)->m_flagDrawing = view->isDrawing();
		}
		if (parent != nil) {
			[parent addSubview:handle];
		}
		
		CALayer* layer = handle.layer;
		if (layer != nil) {
			float shadowOpacity = view->getShadowOpacity();
			if (shadowOpacity > SLIB_EPSILON) {
				layer.shadowOpacity = shadowOpacity;
				layer.shadowRadius = (CGFloat)(view->getShadowRadius());
				UIPointf offset = view->getShadowOffset();
				layer.shadowOffset = CGSizeMake((CGFloat)(offset.x), (CGFloat)(offset.y));
				CGColorRef color = GraphicsPlatform::getCGColorFromColor(view->getShadowColor());
				if (color) {
					layer.shadowColor = color;
					CFRelease(color);
				}
			}
		}
	}
	
	NSView* macOS_ViewInstance::getHandle()
	{
		return m_handle;
	}

	sl_bool macOS_ViewInstance::isValid(View* view)
	{
		return sl_true;
	}

	void macOS_ViewInstance::setFocus(View* view, sl_bool flagFocus)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			NSWindow* window = [handle window];
			if (window != nil) {
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

	void macOS_ViewInstance::invalidate(View* view)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			if ([NSThread isMainThread]) {
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

	void macOS_ViewInstance::invalidate(View* view, const UIRect& rect)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			NSRect _rect;
			_rect.origin.x = (CGFloat)(rect.left);
			_rect.origin.y = (CGFloat)(rect.top);
			_rect.size.width = (CGFloat)(rect.getWidth());
			_rect.size.height = (CGFloat)(rect.getHeight());
			if ([NSThread isMainThread]) {
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

	void macOS_ViewInstance::setFrame(View* view, const UIRect& frame)
	{
		m_frame = frame;
		updateFrameAndTransform();
	}

	void macOS_ViewInstance::setTransform(View* view, const Matrix3& transform)
	{
		m_transform = transform;
		updateFrameAndTransform();
	}

	void macOS_ViewInstance::setVisible(View* view, sl_bool flag)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			[handle setHidden:(flag ? NO : YES)];
		}
	}

	void macOS_ViewInstance::setEnabled(View* view, sl_bool flag)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			if ([handle isKindOfClass:[NSControl class]]) {
				NSControl* control = (NSControl*)handle;
				[control setEnabled:(flag ? YES : NO)];
			}
		}
	}

	void macOS_ViewInstance::setOpaque(View* view, sl_bool flag)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			if ([handle isKindOfClass:[SLIBViewBaseHandle class]]) {
				SLIBViewBaseHandle* control = (SLIBViewBaseHandle*)handle;
				control->m_flagOpaque = flag;
			}
		}
	}

	void macOS_ViewInstance::setAlpha(View* view, sl_real alpha)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			[handle setNeedsDisplay: TRUE];
		}
	}
	
	void macOS_ViewInstance::setClipping(View* view, sl_bool flag)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			if ([handle isKindOfClass:[SLIBViewBaseHandle class]]) {
				SLIBViewBaseHandle* control = (SLIBViewBaseHandle*)handle;
				control->m_flagClipping = flag;
			}
		}
	}
	
	void macOS_ViewInstance::setDrawing(View* view, sl_bool flag)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			if ([handle isKindOfClass:[SLIBViewBaseHandle class]]) {
				SLIBViewBaseHandle* control = (SLIBViewBaseHandle*)handle;
				control->m_flagDrawing = flag;
			}
		}
	}
	
	UIPointf macOS_ViewInstance::convertCoordinateFromScreenToView(View* view, const UIPointf& ptScreen)
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

	UIPointf macOS_ViewInstance::convertCoordinateFromViewToScreen(View* view, const UIPointf& ptView)
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

	void macOS_ViewInstance::addChildInstance(View* view, const Ref<ViewInstance>& _child)
	{
		NSView* handle = m_handle;
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

	void macOS_ViewInstance::removeChildInstance(View* view, const Ref<ViewInstance>& _child)
	{
		macOS_ViewInstance* child = (macOS_ViewInstance*)(_child.get());
		if (child) {
			NSView* child_handle = child->m_handle;
			if (child_handle != nil) {
				[child_handle removeFromSuperview];
			}
		}
	}

	void macOS_ViewInstance::bringToFront(View* view)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			NSView* parent = handle.superview;
			if (parent != nil) {
				[handle removeFromSuperviewWithoutNeedingDisplay];
				[parent addSubview:handle];
			}
		}
	}
	
	void macOS_ViewInstance::setShadowOpacity(View* view, float opacity)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			CALayer* layer = handle.layer;
			if (layer != nil) {
				layer.shadowOpacity = opacity;
			}
		}
	}
	
	void macOS_ViewInstance::setShadowRadius(View* view, sl_ui_posf radius)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			CALayer* layer = handle.layer;
			if (layer != nil) {
				layer.shadowRadius = (CGFloat)radius;
			}
		}
	}
	
	void macOS_ViewInstance::setShadowOffset(View* view, sl_ui_posf x, sl_ui_posf y)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			CALayer* layer = handle.layer;
			if (layer != nil) {
				layer.shadowOffset = CGSizeMake((CGFloat)x, (CGFloat)y);
			}
		}
	}
	
	void macOS_ViewInstance::setShadowColor(View* view, const Color& _color)
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			CALayer* layer = handle.layer;
			if (layer != nil) {
				CGColorRef color = GraphicsPlatform::getCGColorFromColor(_color);
				if (color) {
					layer.shadowColor = color;
					CFRelease(color);
				}
			}
		}
	}
	
	NSRect macOS_ViewInstance::getViewFrameAndTransform(const UIRect& frame, const Matrix3& transform, sl_real& rotation)
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
	
	void macOS_ViewInstance::onDraw(NSRect rcDirty)
	{
		Ref<View> view = getView();
		if (view.isNull()) {
			return;
		}
		NSView* handle = m_handle;
		if (handle == nil) {
			return;
		}
		NSGraphicsContext* graphics = [NSGraphicsContext currentContext];
		if (graphics == nil) {
			return;
		}
		
		CGContextRef context = (CGContextRef)([graphics graphicsPort]);
		NSRect rectBound = [handle bounds];
		Ref<Canvas> canvas = GraphicsPlatform::createCanvas(CanvasType::View, context, (sl_uint32)(rectBound.size.width), (sl_uint32)(rectBound.size.height));
		if (canvas.isNull()) {
			return;
		}
		
		canvas->setInvalidatedRect(Rectangle((sl_real)(rcDirty.origin.x), (sl_real)(rcDirty.origin.y), (sl_real)(rcDirty.origin.x + rcDirty.size.width), (sl_real)(rcDirty.origin.y + rcDirty.size.height)));
		
		sl_real alpha = view->getAlpha();
		Ref<View> parent = view->getParent();
		while (parent.isNotNull()) {
			alpha *= parent->getAlpha();
			parent = parent->getParent();
		}
		if (alpha < 0.005f) {
			return;
		}
		if (alpha < 0.995f) {
			canvas->setAlpha(alpha);
		}
		
		view->dispatchDraw(canvas.get());		
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
				UIPlatform::applyEventModifiers(ev.get(), event);
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
					switch (action) {
						case UIAction::LeftButtonDown:
						case UIAction::RightButtonDown:
						case UIAction::MiddleButtonDown:
						case UIAction::LeftButtonDoubleClick:
						case UIAction::RightButtonDoubleClick:
						case UIAction::MiddleButtonDoubleClick:
							{
								for (NSView* subview in [handle subviews]) {
									if (!(subview.isHidden)) {
										NSRect frame = subview.frame;
										if (NSPointInRect(pt, frame)) {
											ev->addFlag(UIEventFlags::NotDispatchToChildren);
											break;
										}
									}
								}
							}
							break;
						case UIAction::LeftButtonDrag:
						case UIAction::RightButtonDrag:
						case UIAction::MiddleButtonDrag:
						case UIAction::LeftButtonUp:
						case UIAction::RightButtonUp:
						case UIAction::MiddleButtonUp:
						case UIAction::MouseMove:
						case UIAction::MouseEnter:
						case UIAction::MouseLeave:
							break;
						default:
							break;
					}
					UIPlatform::applyEventModifiers(ev.get(), event);
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
				UIPlatform::applyEventModifiers(ev.get(), event);
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

	void macOS_ViewInstance::updateFrameAndTransform()
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			sl_real rotation = 0;
			handle.frame = getViewFrameAndTransform(m_frame, m_transform, rotation);
			handle.frameRotation = Math::getDegreesFromRadian(rotation);
			[handle setNeedsDisplay:YES];
		}
	}

	
	Ref<ViewInstance> View::createGenericInstance(ViewInstance* parent)
	{
		return macOS_ViewInstance::create<macOS_ViewInstance, SLIBViewHandle>(this, parent);
	}
	
	
	Ref<ViewInstance> UIPlatform::createViewInstance(NSView* handle)
	{
		Ref<ViewInstance> ret = UIPlatform::_getViewInstance((__bridge void*)handle);
		if (ret.isNotNull()) {
			return ret;
		}
		return macOS_ViewInstance::create<macOS_ViewInstance>(handle);
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
	
	sl_bool UIPlatform::measureNativeWidgetFittingSize(View* view, UISize& _out)
	{
		NSView* handle = UIPlatform::getViewHandle(view);
		if (handle != nil) {
			NSSize size = handle.fittingSize;
			_out.x = (sl_ui_len)(size.width);
			_out.y = (sl_ui_len)(size.height);
			return sl_true;
		}
		return sl_false;
	}

	sl_bool UIPlatform::measureNativeWidgetFittingSize(ViewInstance* instance, UISize& _out)
	{
		NSView* handle = UIPlatform::getViewHandle(instance);
		if (handle != nil) {
			NSSize size = handle.fittingSize;
			_out.x = (sl_ui_len)(size.width);
			_out.y = (sl_ui_len)(size.height);
			return sl_true;
		}
		return sl_false;
	}
	
}

using namespace slib;

@implementation SLIBViewBaseHandle

- (BOOL)isFlipped
{
	return TRUE;
}

- (BOOL)isOpaque
{
	return m_flagOpaque ? YES : NO;
}

@end

@implementation SLIBViewHandle

MACOS_VIEW_DEFINE_ON_FOCUS

- (BOOL)acceptsFirstResponder
{
	return TRUE;
}

- (void)drawRect:(NSRect)dirtyRect
{
	if (m_flagDrawing) {
		Ref<macOS_ViewInstance> instance = m_viewInstance;
		if (instance.isNotNull()) {
			instance->onDraw(dirtyRect);
		}
	}
}

- (void)keyDown:(NSEvent*)theEvent
{
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		UIEventFlags flags = instance->onEventKey(sl_true, theEvent);
		if (flags & UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] keyDown:theEvent];
}

- (void)keyUp:(NSEvent*)theEvent
{
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		UIEventFlags flags = instance->onEventKey(sl_false, theEvent);
		if (flags & UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] keyUp:theEvent];
}

- (void)mouseDown:(NSEvent *)theEvent
{
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		UIEventFlags flags = instance->onEventMouse(UIAction::LeftButtonDown, theEvent);
		if (flags & UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] mouseDown:theEvent];
}

- (void)mouseUp:(NSEvent *)theEvent
{
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		UIEventFlags flags = instance->onEventMouse(UIAction::LeftButtonUp, theEvent);
		NSInteger clicks = [theEvent clickCount];
		if (clicks == 2) {
			flags |= instance->onEventMouse(UIAction::LeftButtonDoubleClick, theEvent);
		}
		if (flags & UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] mouseUp:theEvent];
}

- (void)mouseDragged:(NSEvent *)theEvent
{
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		UIEventFlags flags = instance->onEventMouse(UIAction::LeftButtonDrag, theEvent);
		if (flags & UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] mouseDragged:theEvent];
}

- (void)rightMouseDown:(NSEvent *)theEvent
{
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		UIEventFlags flags = instance->onEventMouse(UIAction::RightButtonDown, theEvent);
		if (flags & UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] rightMouseDown:theEvent];
}

- (void)rightMouseUp:(NSEvent *)theEvent
{
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		UIEventFlags flags = instance->onEventMouse(UIAction::RightButtonUp, theEvent);
		NSInteger clicks = [theEvent clickCount];
		if (clicks == 2) {
			flags |= instance->onEventMouse(UIAction::RightButtonDoubleClick, theEvent);
		}
		if (flags & UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] rightMouseUp:theEvent];
}

- (void)rightMouseDragged:(NSEvent *)theEvent
{
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		UIEventFlags flags = instance->onEventMouse(UIAction::RightButtonDrag, theEvent);
		if (flags & UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] rightMouseDragged:theEvent];
}

- (void)otherMouseDown:(NSEvent *)theEvent
{
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		UIEventFlags flags = instance->onEventMouse(UIAction::MiddleButtonDown, theEvent);
		if (flags & UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] otherMouseDown:theEvent];
}

- (void)otherMouseUp:(NSEvent *)theEvent
{
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		UIEventFlags flags = instance->onEventMouse(UIAction::MiddleButtonUp, theEvent);
		NSInteger clicks = [theEvent clickCount];
		if (clicks == 2) {
			flags |= instance->onEventMouse(UIAction::MiddleButtonDoubleClick, theEvent);
		}
		if (flags & UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] otherMouseUp:theEvent];
}

- (void)otherMouseDragged:(NSEvent *)theEvent
{
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		UIEventFlags flags = instance->onEventMouse(UIAction::MiddleButtonDrag, theEvent);
		if (flags & UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] otherMouseDragged:theEvent];
}

- (void)mouseMoved:(NSEvent *)theEvent
{
	[[self window] invalidateCursorRectsForView:self];
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		UIEventFlags flags = instance->onEventMouse(UIAction::MouseMove, theEvent);
		if (flags & UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] mouseMoved:theEvent];
}

-(void)onMouseEntered:(NSEvent *)theEvent
{
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onEventMouse(UIAction::MouseEnter, theEvent);
	}
}

- (void)mouseEntered:(NSEvent *)theEvent
{
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		UIEventFlags flags = instance->onEventMouse(UIAction::MouseEnter, theEvent);
		if (flags & UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] mouseEntered:theEvent];
}

-(void)onMouseExited:(NSEvent *)theEvent
{
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onEventMouse(UIAction::MouseLeave, theEvent);
	}
}

- (void)mouseExited:(NSEvent *)theEvent
{
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		UIEventFlags flags = instance->onEventMouse(UIAction::MouseLeave, theEvent);
		if (flags & UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] mouseExited:theEvent];
}

- (void)scrollWheel:(NSEvent *)theEvent
{
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		UIEventFlags flags = instance->onEventMouseWheel(theEvent);
		if (flags & UIEventFlags::StopPropagation) {
			return;
		}
	}
	[[self nextResponder] scrollWheel:theEvent];
}

- (void)cursorUpdate:(NSEvent *)theEvent
{
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		UIEventFlags flags = instance->onEventUpdateCursor(theEvent);
		if (flags & UIEventFlags::PreventDefault) {
			return;
		}
	}
	[super cursorUpdate: theEvent];
}

- (NSView *)hitTest:(NSPoint)aPoint
{
	NSPoint pt = [self convertPoint:aPoint fromView:[self superview]];
	Ref<macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		Ref<View> view = instance->getView();
		if (view.isNotNull()) {
			if (!(view->isEnabled())) {
				return nil;
			}
			if (view->isCapturingEvents()) {
				return self;
			}
			Function<sl_bool(const UIPoint&)> hitTestCapture(view->getCapturingChildInstanceEvents());
			if (hitTestCapture.isNotNull()) {
				if (hitTestCapture(UIPoint((sl_ui_pos)(pt.x), (sl_ui_pos)(pt.y)))) {
					return self;
				}
			}
		}
	}
	for (NSView* subview in [self subviews]) {
		if (!(subview.isHidden)) {
			if (NSPointInRect(pt, subview.frame)) {
				NSView* view = [subview hitTest:pt];
				if (view != nil) {
					return view;
				}
			}
		}
	}
	if (NSPointInRect(aPoint, self.frame)) {
		return self;
	}
	return nil;
}

@end

#endif
