#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "view_ios.h"

#include "../../../inc/slib/ui/core.h"

SLIB_UI_NAMESPACE_BEGIN

/******************************************
			iOS_ViewInstance
******************************************/

iOS_ViewInstance::iOS_ViewInstance()
{
	m_handle = nil;
}

iOS_ViewInstance::~iOS_ViewInstance()
{
	release();
}

void iOS_ViewInstance::release()
{
	UIPlatform::removeViewInstance(m_handle);
	if (m_flagFreeOnRelease) {
		freeHandle(m_handle);
	}
	m_handle = nil;
}

void iOS_ViewInstance::freeHandle(UIView* handle)
{
	if (handle != nil) {
		[handle removeFromSuperview];
	}
}

Ref<iOS_ViewInstance> iOS_ViewInstance::create(UIView* handle, sl_bool flagFreeOnRelease)
{
	Ref<iOS_ViewInstance> ret;
	if (handle != nil) {
		ret = new iOS_ViewInstance();
		if (ret.isNotNull()) {
			ret->m_handle = handle;
			ret->m_flagFreeOnRelease = flagFreeOnRelease;
			UIPlatform::registerViewInstance(handle, ret.ptr);
		} else {
			if (flagFreeOnRelease) {
				freeHandle(handle);
			}
		}
	}
	return ret;
}

Ref<iOS_ViewInstance> iOS_ViewInstance::create(UIView* handle, UIView* parent, View* view)
{
	Ref<iOS_ViewInstance> instance;
	
	if (handle != nil) {
		
		instance = create(handle, sl_true);
		
		if (instance.isNotNull()) {
			
			[handle setClipsToBounds:YES];
			[handle setMultipleTouchEnabled:YES];

			[handle setHidden:(view->isVisible() ? NO : YES)];
			if (!(view->isEnabled())) {
				if ([handle isKindOfClass:[UIControl class]]) {
					UIControl* control = (UIControl*)(handle);
					[control setEnabled:NO];
				}
			}
			[handle setOpaque:(view->isOpaque() ? YES : NO)];
			if (!([handle isKindOfClass:[Slib_iOS_ViewHandle class]])) {
				sl_real alpha = view->getFinalAlpha();
				if (alpha < 0.995f) {
					[handle setAlpha: alpha];
				}
			}
			Matrix3 transform;
			if (view->getFinalTransform(&transform)) {
				CGAffineTransform t;
				GraphicsPlatform::getCGAffineTransform(t, transform, UIPlatform::getGlobalScaleFactor(), 0, 0);
				[handle setTransform: t];
			}
			
			if (parent != nil) {
				[parent addSubview:handle];
			}
		}
	}
	return instance;
}

UIView* iOS_ViewInstance::getHandle()
{
	return m_handle;
}

sl_bool iOS_ViewInstance::isValid()
{
	return sl_true;
}

void iOS_ViewInstance::setFocus()
{
}

void iOS_ViewInstance::invalidate()
{
	UIView* handle = m_handle;
	if (handle != nil) {
		if (UI::isUiThread()) {
			[handle setNeedsDisplay];
		} else {
			dispatch_async(dispatch_get_main_queue(), ^{
				[handle setNeedsDisplay];
			});
		}
	}
}

void iOS_ViewInstance::invalidate(const UIRect& _rect)
{
	UIView* handle = m_handle;
	if (handle != nil) {
		CGRect rect;
		CGFloat f = UIPlatform::getGlobalScaleFactor();
		rect.origin.x = (CGFloat)(_rect.left) / f;
		rect.origin.y = (CGFloat)(_rect.top) / f;
		rect.size.width = (CGFloat)(_rect.getWidth()) / f;
		rect.size.height = (CGFloat)(_rect.getHeight()) / f;
		if (UI::isUiThread()) {
			[handle setNeedsDisplayInRect: rect];
		} else {
			dispatch_async(dispatch_get_main_queue(), ^{
				[handle setNeedsDisplayInRect: rect];
			});
		}
	}
}

UIRect iOS_ViewInstance::getFrame()
{
	UIView* handle = m_handle;
	if (handle != nil) {
		CGPoint center = handle.center;
		CGSize size = handle.bounds.size;
		size.width /= 2;
		size.height /= 2;
		UIRect ret;
		CGFloat f = UIPlatform::getGlobalScaleFactor();
		ret.left = (sl_ui_pos)((center.x - size.width) * f);
		ret.top = (sl_ui_pos)((center.y - size.height) * f);
		ret.right = (sl_ui_pos)((center.x + size.width) * f);
		ret.bottom = (sl_ui_pos)((center.y + size.height) * f);
		ret.fixSizeError();
		return ret;
	}
	return UIRect::zero();
}

void iOS_ViewInstance::setFrame(const UIRect& frame)
{
	UIView* handle = m_handle;
	if (handle != nil) {
		CGFloat f = UIPlatform::getGlobalScaleFactor();
		CGPoint center;
		CGRect bounds;
		bounds.origin.x = 0;
		bounds.origin.y = 0;
		bounds.size.width = (CGFloat)(frame.getWidth()) / f;
		bounds.size.height = (CGFloat)(frame.getHeight()) / f;
		center.x = (CGFloat)(frame.left) / f + bounds.size.width / 2;
		center.y = (CGFloat)(frame.top) / f + bounds.size.height / 2;
		if (UI::isUiThread()) {
			[handle setBounds:bounds];
			[handle setCenter:center];
			[handle setNeedsDisplay];
		} else {
			dispatch_async(dispatch_get_main_queue(), ^{
				[handle setBounds:bounds];
				[handle setCenter:center];
				[handle setNeedsDisplay];
			});
		}
	}
}

void iOS_ViewInstance::setTransform(const Matrix3& m)
{
	UIView* handle = m_handle;
	if (handle != nil) {
		CGAffineTransform t;
		GraphicsPlatform::getCGAffineTransform(t, m, UIPlatform::getGlobalScaleFactor(), 0, 0);
		if (UI::isUiThread()) {
			[handle setTransform: t];
		} else {
			dispatch_async(dispatch_get_main_queue(), ^{
				[handle setTransform: t];
			});
		}
	}
}

void iOS_ViewInstance::setVisible(sl_bool flag)
{
	UIView* handle = m_handle;
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

void iOS_ViewInstance::setEnabled(sl_bool flag)
{
	UIView* handle = m_handle;
	if (handle != nil) {
		if ([handle isKindOfClass:[UIControl class]]) {
			UIControl* control = (UIControl*)handle;
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

void iOS_ViewInstance::setOpaque(sl_bool flag)
{
	UIView* handle = m_handle;
	if (handle != nil) {
		if (UI::isUiThread()) {
			[handle setOpaque:(flag?YES:NO)];
		} else {
			dispatch_async(dispatch_get_main_queue(), ^{
				[handle setOpaque:(flag ? YES : NO)];
			});
		}
	}
}

void iOS_ViewInstance::setAlpha(sl_real alpha)
{
	UIView* handle = m_handle;
	if (handle != nil) {
		if (UI::isUiThread()) {
			[handle setAlpha:alpha];
		} else {
			dispatch_async(dispatch_get_main_queue(), ^{
				[handle setAlpha:alpha];
			});
		}
	}
}

UIPointf iOS_ViewInstance::convertCoordinateFromScreenToView(const UIPointf& ptScreen)
{
	UIView* handle = m_handle;
	if (handle != nil) {
		UIWindow* window = [handle window];
		if (window != nil) {
			CGPoint pt;
			CGFloat f = UIPlatform::getGlobalScaleFactor();
			pt.x = ptScreen.x / f;
			pt.y = ptScreen.y / f;
			pt = [window convertPoint:pt fromWindow:nil];
			pt = [window convertPoint:pt toView:handle];
			UIPointf ret;
			ret.x = (sl_ui_posf)(pt.x * f);
			ret.y = (sl_ui_posf)(pt.y * f);
			return ret;
		}
	}
	return ptScreen;
}

UIPointf iOS_ViewInstance::convertCoordinateFromViewToScreen(const UIPointf& ptView)
{
	UIView* handle = m_handle;
	if (handle != nil) {
		UIWindow* window = [handle window];
		if (window != nil) {
			CGPoint pt;
			CGFloat f = UIPlatform::getGlobalScaleFactor();
			pt.x = ptView.x / f;
			pt.y = ptView.y / f;
			pt = [window convertPoint:pt fromView:handle];
			pt = [window convertPoint:pt toWindow:nil];
			UIPointf ret;
			ret.x = (sl_ui_posf)(pt.x * f);
			ret.y = (sl_ui_posf)(pt.y * f);
			return ret;
		}
	}
	return ptView;
}

void iOS_ViewInstance::addChildInstance(const Ref<ViewInstance>& _child)
{
	iOS_ViewInstance* child = (iOS_ViewInstance*)(_child.ptr);
	if (child) {
		UIView* handle = m_handle;
		if (handle != nil) {
			UIView* child_handle = child->m_handle;
			if (child_handle != nil) {
				[handle addSubview:child_handle];
			}
		}
	}
}

void iOS_ViewInstance::removeChildInstance(const Ref<ViewInstance>& _child)
{
	iOS_ViewInstance* child = (iOS_ViewInstance*)(_child.ptr);
	if (child) {
		UIView* child_handle = child->m_handle;
		if (child_handle != nil) {
			[child_handle removeFromSuperview];
		}
	}
}

void iOS_ViewInstance::bringToFront()
{
	UIView* handle = m_handle;
	if (handle != nil) {
		UIView* parent = handle.superview;
		if (parent != nil) {
			if (UI::isUiThread()) {
				[parent bringSubviewToFront:handle];
				[handle setNeedsDisplay];
			} else {
				dispatch_async(dispatch_get_main_queue(), ^{
					[parent bringSubviewToFront:handle];
					[handle setNeedsDisplay];
				});
			}
		}
	}
}

void iOS_ViewInstance::onDraw(CGRect rectDirty)
{
	
	UIView* handle = m_handle;
	
	if (handle != nil) {
		
		CGRect rectBound = [handle bounds];
		
		CGContextRef context = UIGraphicsGetCurrentContext();
		
		if (context != nil) {
			
			CGFloat f = UIPlatform::getGlobalScaleFactor();
			CGContextScaleCTM(context, 1/f, 1/f);
			
			Ref<Canvas> canvas = GraphicsPlatform::createCanvas(CanvasType::View, context, (sl_uint32)(rectBound.size.width), (sl_uint32)(rectBound.size.height));
			
			if (canvas.isNotNull()) {
				canvas->setInvalidatedRect(Rectangle((sl_real)(rectDirty.origin.x * f), (sl_real)(rectDirty.origin.y * f), (sl_real)((rectDirty.origin.x + rectDirty.size.width) * f), (sl_real)((rectDirty.origin.y + rectDirty.size.height) * f)));
				ViewInstance::onDraw(canvas.ptr);
			}
		}
	}
}

sl_bool iOS_ViewInstance::onEventTouch(UIAction action, NSSet* touches, ::UIEvent* event)
{
	UIView* handle = m_handle;
	
	if (handle != nil) {
		
		CGFloat f = UIPlatform::getGlobalScaleFactor();
		
		sl_uint32 n = (sl_uint32)([touches count]);
		Array<TouchPoint> points(n);
		if (points.isNotNull()) {
			TouchPoint* pts = points.getData();
			sl_uint32 i = 0;
			for (UITouch* touch in touches) {
				CGPoint pt = [touch locationInView:handle];
				sl_real pressure = (sl_real)([touch force]);
				UITouchPhase _phase = [touch phase];
				TouchPhase phase;
				if (_phase == UITouchPhaseBegan) {
					phase = TouchPhase::Begin;
				} else if (_phase == UITouchPhaseEnded) {
					phase = TouchPhase::End;
				} else if (_phase == UITouchPhaseCancelled) {
					phase = TouchPhase::Cancel;
				} else {
					phase = TouchPhase::Move;
				}
				TouchPoint point((sl_ui_posf)(pt.x * f), (sl_ui_posf)(pt.y * f), pressure, phase);
				pts[i] = point;
				i++;
				if (i >= n) {
					break;
				}
			}
		}
		
		Time t;
		t.setSecondsCountf(event.timestamp);
		Ref<UIEvent> ev = UIEvent::createTouchEvent(action, points, t);
		if (ev.isNotNull()) {
			onTouchEvent(ev.ptr);
			if (ev->isStoppedPropagation()) {
				return sl_true;
			}
		}
	}
	
	return sl_false;
}


/******************************************
				View
******************************************/
Ref<ViewInstance> View::createGenericInstance(ViewInstance* _parent)
{
	IOS_VIEW_CREATE_INSTANCE_BEGIN
	Slib_iOS_ViewHandle* handle = [[Slib_iOS_ViewHandle alloc] initWithFrame:frame];
	IOS_VIEW_CREATE_INSTANCE_END
	return ret;
}

SLIB_UI_NAMESPACE_END

@implementation Slib_iOS_ViewHandle

- (void)drawRect:(CGRect)dirtyRect
{
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		instance->onDraw(dirtyRect);
	}
}

- (UIView *)hitTest:(CGPoint)aPoint withEvent:(UIEvent *)event
{
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance;
	if (instance.isNotNull()) {
		slib::Ref<slib::View> view = instance->getView();
		if (view.isNotNull()) {
			if (!(view->isEnabled())) {
				return nil;
			}
			if (view->isCapturingChildInstanceEvents()) {
				CGFloat f = slib::UIPlatform::getGlobalScaleFactor();
				if (view->hitTestForCapturingChildInstanceEvents(slib::UIPoint((sl_ui_pos)(aPoint.x * f), (sl_ui_pos)(aPoint.y * f)))) {
					return self;
				}
			}
		}
	}
	return [super hitTest:aPoint withEvent:event];
}

IOS_VIEW_EVENTS

@end

/******************************************
			UIPlatform
******************************************/

SLIB_UI_NAMESPACE_BEGIN

Ref<ViewInstance> UIPlatform::createViewInstance(UIView* handle, sl_bool flagFreeOnRelease)
{
	Ref<ViewInstance> ret = UIPlatform::_getViewInstance((__bridge void*)handle);
	if (ret.isNotNull()) {
		return ret;
	}
	return iOS_ViewInstance::create(handle, flagFreeOnRelease);
}

void UIPlatform::registerViewInstance(UIView* handle, ViewInstance* instance)
{
	UIPlatform::_registerViewInstance((__bridge void*)handle, instance);
}

Ref<ViewInstance> UIPlatform::getViewInstance(UIView* handle)
{
	return UIPlatform::_getViewInstance((__bridge void*)handle);
}

void UIPlatform::removeViewInstance(UIView* handle)
{
	UIPlatform::_removeViewInstance((__bridge void*)handle);
}

UIView* UIPlatform::getViewHandle(ViewInstance* _instance)
{
	iOS_ViewInstance* instance = static_cast<iOS_ViewInstance*>(_instance);
	if (instance) {
		return instance->getHandle();
	} else {
		return nil;
	}
}

UIView* UIPlatform::getViewHandle(View* view)
{
	if (view) {
		Ref<ViewInstance> _instance = view->getViewInstance();
		if (_instance.isNotNull()) {
			iOS_ViewInstance* instance = static_cast<iOS_ViewInstance*>(_instance.ptr);
			return instance->getHandle();
		}
	}
	return nil;
}

sl_bool GestureDetector::_enableNative(const Ref<View>& view, GestureType type)
{
	Ref<ViewInstance> _instance = view->getViewInstance();
	iOS_ViewInstance* instance = static_cast<iOS_ViewInstance*>(_instance.ptr);
	if (instance) {
		switch (type) {
			case GestureType::SwipeLeft:
				if (instance->m_gestureSwipeLeft == nil) {
					UISwipeGestureRecognizer* gesture = [[UISwipeGestureRecognizer alloc] initWithTarget:instance->m_handle action:NSSelectorFromString(@"onSwipeLeft")];
					gesture.direction = UISwipeGestureRecognizerDirectionLeft;
					[instance->m_handle addGestureRecognizer:gesture];
					instance->m_gestureSwipeLeft = gesture;
				}
				return sl_true;
			case GestureType::SwipeRight:
				if (instance->m_gestureSwipeRight == nil) {
					UISwipeGestureRecognizer* gesture = [[UISwipeGestureRecognizer alloc] initWithTarget:instance->m_handle action:NSSelectorFromString(@"onSwipeRight")];
					gesture.direction = UISwipeGestureRecognizerDirectionRight;
					[instance->m_handle addGestureRecognizer:gesture];
					instance->m_gestureSwipeRight = gesture;
				}
				return sl_true;
			case GestureType::SwipeUp:
				if (instance->m_gestureSwipeUp == nil) {
					UISwipeGestureRecognizer* gesture = [[UISwipeGestureRecognizer alloc] initWithTarget:instance->m_handle action:NSSelectorFromString(@"onSwipeUp")];
					gesture.direction = UISwipeGestureRecognizerDirectionUp;
					[instance->m_handle addGestureRecognizer:gesture];
					instance->m_gestureSwipeUp = gesture;
				}
				return sl_true;
			case GestureType::SwipeDown:
				if (instance->m_gestureSwipeDown == nil) {
					UISwipeGestureRecognizer* gesture = [[UISwipeGestureRecognizer alloc] initWithTarget:instance->m_handle action:NSSelectorFromString(@"onSwipeDown")];
					gesture.direction = UISwipeGestureRecognizerDirectionDown;
					[instance->m_handle addGestureRecognizer:gesture];
					instance->m_gestureSwipeDown = gesture;
				}
				return sl_true;
		}
	}
	return sl_false;
}

SLIB_UI_NAMESPACE_END

#endif


