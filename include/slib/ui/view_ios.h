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
		SLIB_DECLARE_OBJECT
		
	public:
		iOS_ViewInstance();
		
		~iOS_ViewInstance();
		
	public:
		template <class T>
		static Ref<T> create(UIView* handle)
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
		static Ref<T> create(UIView* handle, UIView* parent, View* view)
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
			UIView* parent = UIPlatform::getViewHandle(_parent);
			CGRect frame;
			CGFloat f = UIPlatform::getGlobalScaleFactor();
			UIRect _frame = view->getFrameInInstance();
			frame.origin.x = (CGFloat)(_frame.left) / f;
			frame.origin.y = (CGFloat)(_frame.top) / f;
			frame.size.width = (CGFloat)(_frame.getWidth()) / f;
			frame.size.height = (CGFloat)(_frame.getHeight()) / f;
			HANDLE* handle = [[HANDLE alloc] initWithFrame:frame];
			if (handle != nil) {
				Ref<INSTANCE> ret = create<INSTANCE>(handle, parent, view);
				if (ret.isNotNull()) {
					handle->m_viewInstance = ret;
					return ret;
				}
			}
			return sl_null;
		}

	public:
		void initialize(UIView* handle);
		
		void initialize(UIView* handle, UIView* parent, View* view);
		
		UIView* getHandle();
		
		sl_bool isValid(View* view) override;
		
		void setFocus(View* view, sl_bool flag) override;
		
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
		
	public:
		template <class HANDLE>
		static void setHandleFont(HANDLE* handle, const Ref<Font>& font)
		{
			if (font.isNotNull()) {
				UIFont* hFont = GraphicsPlatform::getUIFont(font.get(), UIPlatform::getGlobalScaleFactor());
				if (hFont != nil) {
					[handle setFont:hFont];
				}
			}
		}
		
	public:
		void onDraw(CGRect rectDirty);
		
		UIEventFlags onEventTouch(UIAction action, NSSet* touches, ::UIEvent* event, sl_bool flagDispatchToChildren = sl_true);
		
	private:
		void _release();
		
	protected:
		UIView* m_handle;
		sl_bool m_flagDrawing;
		
		List<UITouch*> m_touchesCurrent;
		UISwipeGestureRecognizer* m_gestureSwipeLeft;
		UISwipeGestureRecognizer* m_gestureSwipeRight;
		UISwipeGestureRecognizer* m_gestureSwipeUp;
		UISwipeGestureRecognizer* m_gestureSwipeDown;
		
		friend GestureDetector;
		
	};
	
}

#endif

#endif
