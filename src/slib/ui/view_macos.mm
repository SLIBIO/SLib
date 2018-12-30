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

namespace slib
{

/******************************************
			macOS_ViewInstance
******************************************/

	macOS_ViewInstance::macOS_ViewInstance()
	{
		m_handle = nil;
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
				
				[handle setHidden:(view->isVisible() ? NO : YES)];
				
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
				
				/*****************
				Don't use alphaValue because this causes displaying error when used with frameRotation or bounds
				******************/
				/*
				sl_real alpha = view->getFinalAlpha();
				if (alpha < 1 - SLIB_EPSILON) {
					handle.alphaValue = alpha;
				}
				*/
				
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

	void macOS_ViewInstance::setFocus()
	{
		NSView* handle = m_handle;
		if (handle != nil) {
			NSWindow* window = [handle window];
			if (window != nil) {
				if (UI::isUiThread()) {
					[window makeFirstResponder:handle];
				} else {
					dispatch_async(dispatch_get_main_queue(), ^{
						[window makeFirstResponder:handle];
					});
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
				[handle displayIfNeeded];
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
				[handle displayIfNeeded];
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
		NSView* handle = m_handle;
		if (handle != nil) {
			NSRect frameNew;
			frameNew.origin.x = (int)(frame.left);
			frameNew.origin.y = (int)(frame.top);
			frameNew.size.width = (int)(frame.getWidth());
			frameNew.size.height = (int)(frame.getHeight());
			if (UI::isUiThread()) {
				[handle setFrame:frameNew];
				[handle setNeedsDisplay:YES];
			} else {
				dispatch_async(dispatch_get_main_queue(), ^{
					[handle setFrame:frameNew];
					[handle setNeedsDisplay:YES];
				});
			}
		}
	}

	void macOS_ViewInstance::setTransform(const Matrix3 &transform)
	{
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
		/*********
			Don't use alphaValue because this causes displaying error when used with frameRotation or bounds
		*********/
		
		/*
		NSView* handle = m_handle;
		if (handle != nil) {
			handle.alphaValue = alpha;
			[handle setNeedsDisplay:YES];
		}
		*/
		
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
				
				/******
					adjust transform when NSView's alphaValue is less than 1 and, bounds scale is not equal to 1 or frame rotation is not equal to 0.
					This error is found at macOS Sierra, but it seems to be the common problem of all series of macOS system
					Later, decided to do not use alphaValue!
				*****/
				/*
				BOOL flagAdjustTransform = FALSE;
				if (handle.alphaValue < 1 - SLIB_EPSILON) {
					CGFloat w = handle.frame.size.width;
					if (w < 1) {
						return;
					}
					CGFloat h = handle.frame.size.height;
					if (h < 1) {
						return;
					}
					CGFloat bw = handle.bounds.size.width;
					CGFloat bh = handle.bounds.size.height;
					CGFloat sx = bw / w;
					CGFloat sy = bh / h;
					CGFloat r = handle.frameRotation;
					if (Math::abs(sx - 1) > 0.0000001 || Math::abs(sy - 1) > 0.0000001 || Math::abs(r) > 0.0000001) {
						flagAdjustTransform = TRUE;
						CGContextSaveGState(context);
						CGContextTranslateCTM(context, 0, bh/2);
						CGContextScaleCTM(context, sx, sy);
						CGContextTranslateCTM(context, 0, h/2-bh);
					}
				}
				*/
				
				NSRect rectBound = [handle bounds];
				Ref<Canvas> canvas = GraphicsPlatform::createCanvas(CanvasType::View, context, (sl_uint32)(rectBound.size.width), (sl_uint32)(rectBound.size.height));
				if (canvas.isNotNull()) {
					canvas->setInvalidatedRect(Rectangle((sl_real)(rcDirty.origin.x), (sl_real)(rcDirty.origin.y), (sl_real)(rcDirty.origin.x + rcDirty.size.width), (sl_real)(rcDirty.origin.y + rcDirty.size.height)));
					ViewInstance::onDraw(canvas.get());
				}
				
				/*
				if (flagAdjustTransform) {
					CGContextRestoreGState(context);
				}
				*/
			}
		}
	}

	sl_bool macOS_ViewInstance::onEventKey(sl_bool flagDown, NSEvent* event)
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
				return ev->isStoppedPropagation();
			}
		}
		return sl_false;
	}

	sl_bool macOS_ViewInstance::onEventMouse(UIAction action, NSEvent* event)
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
					return ev->isStoppedPropagation();
				}
			}
		}
		return sl_false;
	}

	sl_bool macOS_ViewInstance::onEventMouse(UIAction action, const NSPoint& pt)
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
					return ev->isStoppedPropagation();
				}
			}
		}
		return sl_false;
	}

	sl_bool macOS_ViewInstance::onEventMouseWheel(NSEvent* event)
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
				return ev->isStoppedPropagation();
			}
		}
		return sl_false;
	}

	sl_bool macOS_ViewInstance::onEventUpdateCursor(NSEvent* event)
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
				return ev->isPreventedDefault();
			}
		}
		return sl_false;
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

	void View::_setFrame_NI(const UIRect& _frame)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			UIRect _frame = getFrame();
			Vector2 t;
			sl_real r;
			Vector2 s;
			Vector2 anchor;
			if (getFinalTranslationRotationScale(&t, &r, &s, &anchor)) {
				NSPoint pt;
				NSSize size;
				_priv_macOS_transformViewFrame(pt, size, _frame, t.x, t.y, s.x, s.y, r, anchor.x, anchor.y);
				NSRect bounds;
				bounds.origin.x = 0;
				bounds.origin.y = 0;
				bounds.size.width = (CGFloat)(_frame.getWidth());
				bounds.size.height = (CGFloat)(_frame.getHeight());
				if (UI::isUiThread()) {
					[handle setFrameOrigin:pt];
					[handle setFrameSize:size];
					handle.bounds = bounds;
				} else {
					dispatch_async(dispatch_get_main_queue(), ^{
						[handle setFrameOrigin:pt];
						[handle setFrameSize:size];
						handle.bounds = bounds;
					});
				}
			} else {
				NSRect frame;
				frame.origin.x = (CGFloat)(_frame.left);
				frame.origin.y = (CGFloat)(_frame.top);
				frame.size.width = (CGFloat)(_frame.getWidth());
				frame.size.height = (CGFloat)(_frame.getHeight());
				if (UI::isUiThread()) {
					handle.frame = frame;
					[handle setNeedsDisplay:YES];
				} else {
					dispatch_async(dispatch_get_main_queue(), ^{
						handle.frame = frame;
						[handle setNeedsDisplay:YES];
					});
				}
			}
		}
	}

	void View::_setTransform_NI(const Matrix3& matrix)
	{
		NSView* handle = UIPlatform::getViewHandle(this);
		if (handle != nil) {
			UIRect _frame = getFrame();
			Vector2 t;
			sl_real r;
			Vector2 s;
			Vector2 anchor;
			if (getFinalTranslationRotationScale(&t, &r, &s, &anchor)) {
				NSPoint pt;
				NSSize size;
				_priv_macOS_transformViewFrame(pt, size, _frame, t.x, t.y, s.x, s.y, r, anchor.x, anchor.y);
				NSRect bounds;
				bounds.origin.x = 0;
				bounds.origin.y = 0;
				bounds.size.width = (CGFloat)(_frame.getWidth());
				bounds.size.height = (CGFloat)(_frame.getHeight());
				if (UI::isUiThread()) {
					handle.frameRotation = Math::getDegreesFromRadian(r);
					[handle setFrameOrigin:pt];
					[handle setFrameSize:size];
					handle.bounds = bounds;
				} else {
					dispatch_async(dispatch_get_main_queue(), ^{
						handle.frameRotation = Math::getDegreesFromRadian(r);
						[handle setFrameOrigin:pt];
						[handle setFrameSize:size];
						handle.bounds = bounds;
					});
				}
			} else {
				NSRect frame;
				frame.origin.x = (CGFloat)(_frame.left);
				frame.origin.y = (CGFloat)(_frame.top);
				frame.size.width = (CGFloat)(_frame.getWidth());
				frame.size.height = (CGFloat)(_frame.getHeight());
				if (UI::isUiThread()) {
					handle.frame = frame;
					[handle setNeedsDisplay:YES];
				} else {
					dispatch_async(dispatch_get_main_queue(), ^{
						handle.frame = frame;
						[handle setNeedsDisplay:YES];
					});
				}
			}
		}
	}


	void _priv_macOS_transformViewFrame(NSPoint& origin, NSSize& size, const UIRect& frame, sl_real tx, sl_real ty, sl_real sx, sl_real sy, sl_real r, sl_real anchorOffsetX, sl_real anchorOffsetY)
	{
		sl_ui_pos x = frame.left;
		sl_ui_pos y = frame.top;
		sl_ui_pos w = frame.getWidth();
		sl_ui_pos h = frame.getHeight();
		sl_bool flagScale = !(Math::isAlmostZero(sx - 1) && Math::isAlmostZero(sy - 1));
		sl_bool flagRotate = !(Math::isAlmostZero(r));
		if (flagScale || flagRotate) {
			sl_real ax = (sl_real)(w) / 2 + anchorOffsetX;
			sl_real ay = (sl_real)(h) / 2 + anchorOffsetY;
			sl_bool flagAnchor = !(Math::isAlmostZero(ax) && Math::isAlmostZero(ay));
			if (flagAnchor) {
				sl_real cr = Math::cos(r);
				sl_real sr = Math::sin(r);
				tx += (- ax * cr + ay * sr) * sx + ax;
				ty += (- ax * sr - ay * cr) * sy + ay;
			}
		}
		if (!(Math::isAlmostZero(tx) && Math::isAlmostZero(ty))) {
			x += (sl_ui_pos)tx;
			y += (sl_ui_pos)ty;
		}
		if (flagScale) {
			w = (sl_ui_pos)(w * sx);
			h = (sl_ui_pos)(h * sy);
		}
		origin.x = x;
		origin.y = y;
		size.width = w;
		size.height = h;
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
		sl_bool flagStopPropagation = instance->onEventKey(sl_true, theEvent);
		if (flagStopPropagation) {
			return;
		}
	}
	[[self nextResponder] keyDown:theEvent];
}

- (void)keyUp:(NSEvent*)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		sl_bool flagStopPropagation = instance->onEventKey(sl_false, theEvent);
		if (flagStopPropagation) {
			return;
		}
	}
	[[self nextResponder] keyUp:theEvent];
}

- (void)mouseDown:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		sl_bool flagStopPropagation = instance->onEventMouse(slib::UIAction::LeftButtonDown, theEvent);
		if (flagStopPropagation) {
			return;
		}
	}
	[[self nextResponder] mouseDown:theEvent];
}

- (void)mouseUp:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onEventMouse(slib::UIAction::LeftButtonUp, theEvent);
		NSInteger clicks = [theEvent clickCount];
		if (clicks == 2) {
			sl_bool flagStopPropagation = instance->onEventMouse(slib::UIAction::LeftButtonDoubleClick, theEvent);
			if (flagStopPropagation) {
				return;
			}
		}
	}
	[[self nextResponder] mouseUp:theEvent];
}

- (void)mouseDragged:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		sl_bool flagStopPropagation = instance->onEventMouse(slib::UIAction::LeftButtonDrag, theEvent);
		if (flagStopPropagation) {
			return;
		}
	}
	[[self nextResponder] mouseDragged:theEvent];
}

- (void)rightMouseDown:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		sl_bool flagStopPropagation = instance->onEventMouse(slib::UIAction::RightButtonDown, theEvent);
		if (flagStopPropagation) {
			return;
		}
	}
	[[self nextResponder] rightMouseDown:theEvent];
}

- (void)rightMouseUp:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onEventMouse(slib::UIAction::RightButtonUp, theEvent);
		NSInteger clicks = [theEvent clickCount];
		if (clicks == 2) {
			sl_bool flagStopPropagation = instance->onEventMouse(slib::UIAction::RightButtonDoubleClick, theEvent);
			if (flagStopPropagation) {
				return;
			}
		}
	}
	[[self nextResponder] rightMouseUp:theEvent];
}

- (void)rightMouseDragged:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		sl_bool flagStopPropagation = instance->onEventMouse(slib::UIAction::RightButtonDrag, theEvent);
		if (flagStopPropagation) {
			return;
		}
	}
	[[self nextResponder] rightMouseDragged:theEvent];
}

- (void)otherMouseDown:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		sl_bool flagStopPropagation = instance->onEventMouse(slib::UIAction::MiddleButtonDown, theEvent);
		if (flagStopPropagation) {
			return;
		}
	}
	[[self nextResponder] otherMouseDown:theEvent];
}

- (void)otherMouseUp:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onEventMouse(slib::UIAction::MiddleButtonUp, theEvent);
		NSInteger clicks = [theEvent clickCount];
		if (clicks == 2) {
			sl_bool flagStopPropagation = instance->onEventMouse(slib::UIAction::MiddleButtonDoubleClick, theEvent);
			if (flagStopPropagation) {
				return;
			}
		}
	}
	[[self nextResponder] otherMouseUp:theEvent];
}

- (void)otherMouseDragged:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		sl_bool flagStopPropagation = instance->onEventMouse(slib::UIAction::MiddleButtonDrag, theEvent);
		if (flagStopPropagation) {
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
		sl_bool flagStopPropagation = instance->onEventMouse(slib::UIAction::MouseMove, theEvent);
		if (flagStopPropagation) {
			return;
		}
	}
	[[self nextResponder] mouseMoved:theEvent];
}

- (void)mouseEntered:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		sl_bool flagStopPropagation = instance->onEventMouse(slib::UIAction::MouseEnter, theEvent);
		if (flagStopPropagation) {
			return;
		}
	}
	[[self nextResponder] mouseEntered:theEvent];
}

- (void)mouseExited:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		sl_bool flagStopPropagation = instance->onEventMouse(slib::UIAction::MouseLeave, theEvent);
		if (flagStopPropagation) {
			return;
		}
	}
	[[self nextResponder] mouseExited:theEvent];
}

- (void)scrollWheel:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		sl_bool flagStopPropagation = instance->onEventMouseWheel(theEvent);
		if (flagStopPropagation) {
			return;
		}
	}
	[[self nextResponder] scrollWheel:theEvent];
}

- (void)cursorUpdate:(NSEvent *)theEvent
{
	slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		sl_bool flagNoDefault = instance->onEventUpdateCursor(theEvent);
		if (flagNoDefault) {
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
			if (view->isCapturingChildInstanceEvents()) {
				if (view->hitTestForCapturingChildInstanceEvents(slib::UIPoint((sl_ui_pos)(aPoint.x), (sl_ui_pos)(aPoint.y)))) {
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
