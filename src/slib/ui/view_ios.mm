#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_IOS)

#include "view_ios.h"

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
			[handle setHidden:(view->isVisible() ? NO : YES)];
			if (!(view->isEnabled())) {
				if ([handle isKindOfClass:[UIControl class]]) {
					UIControl* control = (UIControl*)(handle);
					[control setEnabled:NO];
				}
			}
			[handle setOpaque:(view->isOpaque() ? YES : NO)];
			[handle setClipsToBounds:YES];
			[handle setMultipleTouchEnabled:YES];
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
	UIView* handle = m_handle;
	if (handle != nil) {
		[handle becomeFirstResponder];
	}
}

void iOS_ViewInstance::invalidate()
{
	UIView* handle = m_handle;
	if (handle != nil) {
		[handle setNeedsDisplay];
	}
}

void iOS_ViewInstance::invalidate(const UIRect& _rect)
{
	UIView* handle = m_handle;
	if (handle != nil) {
		CGRect rect;
		rect.origin.x = (CGFloat)(_rect.left);
		rect.origin.y = (CGFloat)(_rect.top);
		rect.size.width = (CGFloat)(_rect.getWidth());
		rect.size.height = (CGFloat)(_rect.getHeight());
		[handle setNeedsDisplayInRect: rect];
	}
}

UIRect iOS_ViewInstance::getFrame()
{
	UIView* handle = m_handle;
	if (handle != nil) {
		CGRect frame = handle.frame;
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

void iOS_ViewInstance::setFrame(const UIRect& frame)
{
	UIView* handle = m_handle;
	if (handle != nil) {
		CGRect rect;
		rect.origin.x = (CGFloat)(frame.left);
		rect.origin.y = (CGFloat)(frame.top);
		rect.size.width = (CGFloat)(frame.getWidth());
		rect.size.height = (CGFloat)(frame.getHeight());
		[handle setFrame:rect];
		[handle setNeedsDisplay];
	}
}

void iOS_ViewInstance::setVisible(sl_bool flag)
{
	UIView* handle = m_handle;
	if (handle != nil) {
		[handle setHidden:(flag ? NO : YES)];
	}
}

void iOS_ViewInstance::setEnabled(sl_bool flag)
{
	UIView* handle = m_handle;
	if (handle != nil) {
		if ([handle isKindOfClass:[UIControl class]]) {
			UIControl* control = (UIControl*)handle;
			[control setEnabled:(flag ? YES : NO)];
		}
	}
}

void iOS_ViewInstance::setOpaque(sl_bool flag)
{
	UIView* handle = m_handle;
	if (handle != nil) {
		[handle setOpaque:(flag?YES:NO)];
	}
}

UIPointf iOS_ViewInstance::convertCoordinateFromScreenToView(const UIPointf& ptScreen)
{
	UIView* handle = m_handle;
	if (handle != nil) {
		UIWindow* window = [handle window];
		if (window != nil) {
			CGPoint pt;
			pt.x = ptScreen.x;
			pt.y = ptScreen.y;
			pt = [window convertPoint:pt fromWindow:nil];
			pt = [window convertPoint:pt toView:handle];
			UIPointf ret;
			ret.x = (sl_ui_posf)(pt.x);
			ret.y = (sl_ui_posf)(pt.y);
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
			pt.x = ptView.x;
			pt.y = ptView.y;
			pt = [window convertPoint:pt fromView:handle];
			pt = [window convertPoint:pt toWindow:nil];
			UIPointf ret;
			ret.x = (sl_ui_posf)(pt.x);
			ret.y = (sl_ui_posf)(pt.y);
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

void iOS_ViewInstance::onDraw(CGRect _rectDirty)
{
	
	UIView* handle = m_handle;
	
	if (handle != nil) {
		
		CGRect rectBound = [handle bounds];
		
		CGContextRef context = UIGraphicsGetCurrentContext();
		
		if (context != nil) {
			
			CGContextTranslateCTM(context, 0.5, -0.5);
			
			Ref<Canvas> canvas = UIPlatform::createCanvas(context, (sl_uint32)(rectBound.size.width), (sl_uint32)(rectBound.size.height));
			
			if (canvas.isNotNull()) {
				ViewInstance::onDraw(canvas.ptr);
			}
		}
	}
}

sl_bool iOS_ViewInstance::onEventTouch(UIAction action, NSSet* touches, ::UIEvent* event)
{
	UIView* handle = m_handle;
	
	if (handle != nil) {
		
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
				TouchPoint point((sl_ui_posf)(pt.x), (sl_ui_posf)(pt.y), pressure, phase);
				pts[i] = point;
				i++;
				if (i >= n) {
					break;
				}
			}
		}
		
		Ref<UIEvent> ev = UIEvent::createTouchEvent(action, points);
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
		if (view->isCapturingChildInstanceEvents()) {
			if (view->hitTestForCapturingChildInstanceEvents(slib::UIPoint((sl_ui_pos)(aPoint.x), (sl_ui_pos)(aPoint.y)))) {
				return self;
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

UIView* UIPlatform::getViewHandle(ViewInstance* instance)
{
	iOS_ViewInstance* view = (iOS_ViewInstance*)instance;
	if (view) {
		return view->getHandle();
	} else {
		return nil;
	}
}

UIView* UIPlatform::getViewHandle(View* view)
{
	if (view) {
		Ref<ViewInstance> instance = view->getViewInstance();
		if (instance.isNotNull()) {
			iOS_ViewInstance* osx_instance = (iOS_ViewInstance*)(instance.ptr);
			return osx_instance->getHandle();
		}
	}
	return nil;
}
SLIB_UI_NAMESPACE_END

#endif


