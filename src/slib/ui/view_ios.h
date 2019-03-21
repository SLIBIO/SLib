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

#ifndef CHECKHEADER_SLIB_UI_VIEW_IOS
#define CHECKHEADER_SLIB_UI_VIEW_IOS

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_IOS)

#include "slib/ui/view.h"
#include "slib/ui/platform.h"

namespace slib
{
	
	class iOS_ViewInstance : public ViewInstance
	{
	public:
		iOS_ViewInstance();
		
		~iOS_ViewInstance();
		
	public:
		static Ref<iOS_ViewInstance> create(UIView* handle);
		
		static Ref<iOS_ViewInstance> create(UIView* handle, UIView* parent, View* view);

	public:
		UIView* getHandle();
		
		sl_bool isValid() override;
		
		void setFocus(sl_bool flag) override;
		
		void invalidate() override;
		
		void invalidate(const UIRect& rect) override;
		
		UIRect getFrame() override;
		
		void setFrame(const UIRect& frame) override;
		
		void setTransform(const Matrix3& transform) override;
		
		void setVisible(sl_bool flag) override;
		
		void setEnabled(sl_bool flag) override;
		
		void setOpaque(sl_bool flag) override;
		
		void setAlpha(sl_real alpha) override;
		
		void setClipping(sl_bool flag) override;
		
		void setDrawing(sl_bool flag) override;
		
		UIPointf convertCoordinateFromScreenToView(const UIPointf& ptScreen) override;
		
		UIPointf convertCoordinateFromViewToScreen(const UIPointf& ptView) override;
		
		void addChildInstance(const Ref<ViewInstance>& instance) override;
		
		void removeChildInstance(const Ref<ViewInstance>& instance) override;
		
		void bringToFront() override;
		
	public:
		void onDraw(CGRect rectDirty);
		
		UIEventFlags onEventTouch(UIAction action, NSSet* touches, ::UIEvent* event);
		
	private:
		void _release();
		
	protected:
		UIView* m_handle;
		sl_bool m_flagDrawing;
		
		UISwipeGestureRecognizer* m_gestureSwipeLeft;
		UISwipeGestureRecognizer* m_gestureSwipeRight;
		UISwipeGestureRecognizer* m_gestureSwipeUp;
		UISwipeGestureRecognizer* m_gestureSwipeDown;
		
		friend GestureDetector;
		
	};
	
}

@interface _priv_Slib_iOS_ViewHandle : UIView {

	@public slib::WeakRef<slib::iOS_ViewInstance> m_viewInstance;

}

@end


#define IOS_VIEW_CREATE_INSTANCE_BEGIN \
	Ref<iOS_ViewInstance> ret; \
	UIView* parent = UIPlatform::getViewHandle(_parent); \
	CGRect frame; \
	CGFloat f = UIPlatform::getGlobalScaleFactor();	\
	UIRect _frame = getFrameInInstance(); \
	frame.origin.x = (CGFloat)(_frame.left) / f; \
	frame.origin.y = (CGFloat)(_frame.top) / f; \
	frame.size.width = (CGFloat)(_frame.getWidth()) / f; \
	frame.size.height = (CGFloat)(_frame.getHeight()) / f; \

#define IOS_VIEW_CREATE_INSTANCE_END \
	if (handle != nil) { \
		ret = iOS_ViewInstance::create(handle, parent, this); \
		if (ret.isNotNull()) { \
			handle->m_viewInstance = ret; \
		} \
	}


#define IOS_VIEW_DEFINE_ON_FOCUS \
	- (BOOL)becomeFirstResponder \
	{ \
		slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance; \
		if (instance.isNotNull()) { \
			instance->onSetFocus(); \
		} \
		return [super becomeFirstResponder]; \
	}

#define IOS_VIEW_EVENTS \
IOS_VIEW_DEFINE_ON_FOCUS \
- (BOOL)canBecomeFirstResponder \
{ \
	return NO; \
} \
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)theEvent \
{ \
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance; \
	if (instance.isNotNull()) { \
		slib::UIEventFlags flags = instance->onEventTouch(slib::UIAction::TouchBegin, touches, theEvent); \
		if (flags & slib::UIEventFlags::StopPropagation) { \
			return; \
		} \
	} \
	[super touchesBegan:touches withEvent:theEvent]; \
} \
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)theEvent \
{ \
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance; \
	if (instance.isNotNull()) { \
		slib::UIEventFlags flags = instance->onEventTouch(slib::UIAction::TouchMove, touches, theEvent); \
		if (flags & slib::UIEventFlags::StopPropagation) { \
			return; \
		} \
	} \
	[super touchesMoved:touches withEvent:theEvent]; \
} \
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)theEvent \
{ \
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance; \
	if (instance.isNotNull()) { \
		slib::UIEventFlags flags = instance->onEventTouch(slib::UIAction::TouchEnd, touches, theEvent); \
		if (flags & slib::UIEventFlags::StopPropagation) { \
			return; \
		} \
	} \
	[super touchesEnded:touches withEvent:theEvent]; \
} \
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)theEvent \
{ \
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance; \
	if (instance.isNotNull()) { \
		slib::UIEventFlags flags = instance->onEventTouch(slib::UIAction::TouchCancel, touches, theEvent); \
		if (flags & slib::UIEventFlags::StopPropagation) { \
			return; \
		} \
	} \
	[super touchesCancelled:touches withEvent:theEvent]; \
} \
- (void)onSwipeLeft \
{ \
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance; \
	if (instance.isNotNull()) { \
		instance->onSwipe(slib::GestureType::SwipeLeft); \
	} \
} \
- (void)onSwipeRight \
{ \
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance; \
	if (instance.isNotNull()) { \
		instance->onSwipe(slib::GestureType::SwipeRight); \
	} \
} \
- (void)onSwipeUp \
{ \
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance; \
	if (instance.isNotNull()) { \
		instance->onSwipe(slib::GestureType::SwipeUp); \
	} \
} \
- (void)onSwipeDown \
{ \
	slib::Ref<slib::iOS_ViewInstance> instance = m_viewInstance; \
	if (instance.isNotNull()) { \
		instance->onSwipe(slib::GestureType::SwipeDown); \
	} \
}

#endif

#endif
