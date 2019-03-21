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

#ifndef CHECKHEADER_SLIB_UI_VIEW_MACOS
#define CHECKHEADER_SLIB_UI_VIEW_MACOS

#include "slib/core/definition.h"

#if defined(SLIB_UI_IS_MACOS)

#include "slib/ui/view.h"
#include "slib/ui/platform.h"

namespace slib
{

	class macOS_ViewInstance : public ViewInstance
	{
	public:
		macOS_ViewInstance();
		
		~macOS_ViewInstance();
		
	public:
		static Ref<macOS_ViewInstance> create(NSView* handle);
		
		static Ref<macOS_ViewInstance> create(NSView* handle, NSView* parent, View* view);
		
	public:
		NSView* getHandle();
		
		sl_bool isValid() override;
		
		void setFocus(sl_bool flagFocus) override;
		
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
		void onDraw(NSRect rectDirty);
		
		UIEventFlags onEventKey(sl_bool flagDown, NSEvent* event);
		
		UIEventFlags onEventMouse(UIAction action, NSEvent* event);
		
		UIEventFlags onEventMouse(UIAction action, const NSPoint& point);

		UIEventFlags onEventMouseWheel(NSEvent* event);
		
		UIEventFlags onEventUpdateCursor(NSEvent* event);
		
		void applyModifiers(UIEvent* ev, NSEvent* event);
		
		void updateFrameAndTransform();
		
	private:
		void _release();
		
	protected:
		NSView* m_handle;
		
	public:
		UIRect m_frame;
		Matrix3 m_transform;
		
	};

	NSRect _priv_macOS_getViewFrameAndTransform(const UIRect& frame, const Matrix3& transform, sl_real& rotation);

}

@interface _priv_Slib_macOS_ViewBase : NSView
{
	@public sl_bool m_flagOpaque;
	@public sl_bool m_flagClipping;
	@public sl_bool m_flagDrawing;
}
@end

@interface _priv_Slib_macOS_ViewHandle : _priv_Slib_macOS_ViewBase
{
	@public slib::WeakRef<slib::macOS_ViewInstance> m_viewInstance;
	NSTrackingArea* m_trackingArea;
}
@end

@interface _priv_Slib_macOS_Button : NSButton
{
	@public slib::WeakRef<slib::macOS_ViewInstance> m_viewInstance;
}
@end


#define MACOS_VIEW_CREATE_INSTANCE_BEGIN \
	Ref<macOS_ViewInstance> ret; \
	NSView* parent = UIPlatform::getViewHandle(_parent); \
	UIRect frameView = getFrameInInstance(); \
	Matrix3 transformView = getFinalTransformInInstance(); \
	sl_real rotation; \
	NSRect frame = _priv_macOS_getViewFrameAndTransform(frameView, transformView, rotation); \

#define MACOS_VIEW_CREATE_INSTANCE_END \
	if (handle != nil) { \
		if (!(Math::isAlmostZero(rotation))) { \
			handle.frameRotation = Math::getDegreesFromRadian(rotation); \
		} \
		ret = macOS_ViewInstance::create(handle, parent, this); \
		if (ret.isNotNull()) { \
			ret->m_frame = frameView; \
			ret->m_transform = transformView; \
			handle->m_viewInstance = ret; \
		} \
	}

#define MACOS_VIEW_DEFINE_ON_FOCUS \
	- (BOOL)becomeFirstResponder \
	{ \
		slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance; \
		if (instance.isNotNull()) { \
			instance->onSetFocus(); \
		} \
		return [super becomeFirstResponder]; \
	}

#define MACOS_VIEW_DEFINE_ON_KEY \
	- (void)keyDown:(NSEvent*)theEvent \
	{ \
		slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance; \
		if (instance.isNotNull()) { \
			sl_bool flagNoDefault = instance->onEventKey(sl_true, theEvent) & slib::UIEventFlags::PreventDefault; \
			if (flagNoDefault) { \
				return; \
			} \
		} \
		[super keyDown:theEvent]; \
	} \
	- (void)keyUp:(NSEvent*)theEvent \
	{ \
		slib::Ref<slib::macOS_ViewInstance> instance = m_viewInstance; \
		if (instance.isNotNull()) { \
			sl_bool flagNoDefault = instance->onEventKey(sl_false, theEvent) & slib::UIEventFlags::PreventDefault; \
			if (flagNoDefault) { \
				return; \
			} \
		} \
		[super keyUp:theEvent]; \
	}


#endif

#endif
