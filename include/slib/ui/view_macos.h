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
		SLIB_DECLARE_OBJECT
		
	public:
		macOS_ViewInstance();
		
		~macOS_ViewInstance();
		
	public:
		template <class T>
		static Ref<T> create(NSView* handle)
		{
			if (handle != nil) {
				Ref<T> ret = new T;
				if (ret.isNotNull()) {
					ret->initialize(handle);
					return ret;
				}
			}
			return sl_null;
		}

		template <class T>
		static Ref<T> create(NSView* handle, NSView* parent, View* view)
		{
			if (handle != nil) {
				Ref<T> ret = new T;
				if (ret.isNotNull()) {
					ret->initialize(handle, parent, view);
					return ret;
				}
			}
			return sl_null;
		}
		
		template <class INSTANCE, class HANDLE>
		static Ref<INSTANCE> create(View* view, ViewInstance* _parent)
		{
			NSView* parent = UIPlatform::getViewHandle(_parent);
			UIRect frameView = view->getFrameInInstance();
			Matrix3 transformView = view->getFinalTransformInInstance();
			sl_real rotation;
			NSRect frame = getViewFrameAndTransform(frameView, transformView, rotation);
			HANDLE* handle = [[HANDLE alloc] initWithFrame:frame];
			if (handle != nil) {
				if (!(Math::isAlmostZero(rotation))) {
					handle.frameRotation = Math::getDegreesFromRadian(rotation);
				}
				Ref<INSTANCE> ret = create<INSTANCE>(handle, parent, view);
				if (ret.isNotNull()) {
					ret->m_frame = frameView;
					ret->m_transform = transformView;
					handle->m_viewInstance = ret;
					return ret;
				}
			}
			return sl_null;
		}
		
	public:
		void initialize(NSView* handle);
		
		void initialize(NSView* handle, NSView* parent, View* view);
		
		NSView* getHandle();
		
		sl_bool isValid(View* view) override;
		
		void setFocus(View* view, sl_bool flagFocus) override;
		
		void invalidate(View* view) override;
		
		void invalidate(View* view, const UIRect& rect) override;
		
		void setFrame(View* view, const UIRect& frame) override;
		
		void setTransform(View* view, const Matrix3& transform) override;

		void setVisible(View* view, sl_bool flag) override;
		
		void setEnabled(View* view, sl_bool flag) override;
		
		void setOpaque(View* view, sl_bool flag) override;
		
		void setAlpha(View* view, sl_real alpha) override;
		
		void setClipping(View* view, sl_bool flag) override;
		
		void setDrawing(View* view, sl_bool flag) override;
		
		UIPointf convertCoordinateFromScreenToView(View* view, const UIPointf& ptScreen) override;
		
		UIPointf convertCoordinateFromViewToScreen(View* view, const UIPointf& ptView) override;
		
		void addChildInstance(View* view, const Ref<ViewInstance>& instance) override;
		
		void removeChildInstance(View* view, const Ref<ViewInstance>& instance) override;
		
		void bringToFront(View* view) override;
		
		void setShadowOpacity(View* view, float opacity) override;

		void setShadowRadius(View* view, sl_ui_posf radius) override;
		
		void setShadowOffset(View* view, sl_ui_posf x, sl_ui_posf y) override;
		
		void setShadowColor(View* view, const Color& color) override;
		
		void setDroppable(View* view, sl_bool flag) override;
		
	public:
		static NSRect getViewFrameAndTransform(const UIRect& frame, const Matrix3& transform, sl_real& rotation);
		
		template <class HANDLE>
		static void setHandleFont(HANDLE* handle, const Ref<Font>& font)
		{
			if (font.isNotNull()) {
				NSFont* hFont = GraphicsPlatform::getNativeFont(font.get());
				if (hFont != nil) {
					[handle setFont:hFont];
				}
			}
		}
		
	public:
		void onDraw(NSRect rectDirty);
		
		UIEventFlags onEventKey(sl_bool flagDown, NSEvent* event);
		
		UIEventFlags onEventMouse(UIAction action, NSEvent* event);
		
		UIEventFlags onEventMouse(UIAction action, const NSPoint& point);

		UIEventFlags onEventMouseWheel(NSEvent* event);
		
		UIEventFlags onEventUpdateCursor(NSEvent* event);
		
		Ref<UIEvent> onEventDrop(UIAction action, id<NSDraggingInfo> info);
		
		void updateFrameAndTransform();
		
	private:
		void _release();
		
	protected:
		NSView* m_handle;
		
	public:
		UIRect m_frame;
		Matrix3 m_transform;
		
	};

}

#endif

#endif
