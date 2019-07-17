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

#if defined(SLIB_UI_IS_ANDROID)

#include "slib/ui/scroll_view.h"

#include "view_android.h"

namespace slib
{

	namespace priv
	{
		namespace scroll_view
		{
			
			void JNICALL OnScroll(JNIEnv* env, jobject _this, jlong instance, int x, int y);

			SLIB_JNI_BEGIN_CLASS(JScrollView, "slib/platform/android/ui/view/UiScrollView")

				SLIB_JNI_STATIC_METHOD(create, "_create", "(Landroid/content/Context;Z)Landroid/view/View;");

				SLIB_JNI_STATIC_METHOD(setBackgroundColor, "_setBackgroundColor", "(Landroid/view/View;I)V");
				SLIB_JNI_STATIC_METHOD(scrollTo, "_scrollTo", "(Landroid/view/View;IIZ)V");
				SLIB_JNI_STATIC_METHOD(getScrollX, "_getScrollX", "(Landroid/view/View;)I");
				SLIB_JNI_STATIC_METHOD(getScrollY, "_getScrollY", "(Landroid/view/View;)I");
				SLIB_JNI_STATIC_METHOD(setPaging, "_setPaging", "(Landroid/view/View;ZII)V");
				SLIB_JNI_STATIC_METHOD(setScrollBarsVisible, "_setScrollBarsVisible", "(Landroid/view/View;ZZ)V");

				SLIB_JNI_NATIVE(nativeOnScroll, "nativeOnScroll", "(JII)V", OnScroll);

			SLIB_JNI_END_CLASS

			class ScrollViewHelper : public ScrollView
			{
			public:
				void _applyContent(jobject sv, ViewInstance* scrollViewInstance)
				{
					Ref<View> viewContent = m_viewContent;
					if (viewContent.isNotNull()) {
						viewContent->attachToNewInstance(scrollViewInstance);
						viewContent->setParent(this);
					}
				}
				
				void _applyProperties(jobject handle, ViewInstance* scrollViewInstance)
				{
					JScrollView::setBackgroundColor.call(sl_null, handle, getBackgroundColor().getARGB());
					if (isPaging()) {
						JScrollView::setPaging.call(sl_null, handle, 1, getPageWidth(), getPageHeight());
					}
					JScrollView::setScrollBarsVisible.call(sl_null, handle, isHorizontalScrollBarVisible(), isVerticalScrollBarVisible());
					_applyContent(handle, scrollViewInstance);
					JScrollView::scrollTo.call(sl_null, handle, (int)(getScrollX()), (int)(getScrollY()), 0);
				}

				void _onScroll(int x, int y)
				{
					_onScroll_NW((sl_scroll_pos)x, (sl_scroll_pos)y);
				}
			};

			void JNICALL OnScroll(JNIEnv* env, jobject _this, jlong instance, int x, int y)
			{
				Ref<View> _view = Android_ViewInstance::findView(instance);
				if (ScrollViewHelper* view = CastInstance<ScrollViewHelper>(_view.get())) {
					view->_onScroll(x, y);
				}
			}

		}
	}

	using namespace priv::scroll_view;

	Ref<ViewInstance> ScrollView::createNativeWidget(ViewInstance* _parent)
	{
		Ref<Android_ViewInstance> ret;
		Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
		if (parent) {
			JniLocal<jobject> handle = JScrollView::create.callObject(sl_null, parent->getContext(), isVerticalScrolling());
			ret = Android_ViewInstance::create<Android_ViewInstance>(this, parent, handle.get());
			if (ret.isNotNull()) {
				jobject handle = ret->getHandle();
				((ScrollViewHelper*)this)->_applyProperties(handle, ret.get());
			}
		}
		return ret;
	}

	void ScrollView::_refreshContentSize_NW()
	{
	}

	void ScrollView::_setContentView_NW(const Ref<View>& view)
	{
		Ref<ViewInstance> instance = getViewInstance();
		if (instance.isNotNull()) {
			jobject handle = UIPlatform::getViewHandle(instance.get());
			if (handle) {
				((ScrollViewHelper*)this)->_applyContent(handle, instance.get());
			}
		}
	}

	void ScrollView::_scrollTo_NW(sl_scroll_pos x, sl_scroll_pos y, sl_bool flagAnimate)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JScrollView::scrollTo.call(sl_null, handle, (int)x, (int)y, flagAnimate ? 1 : 0);
		}
	}

	ScrollPoint ScrollView::_getScrollPosition_NW()
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			ScrollPoint ret;
			ret.x = (sl_scroll_pos)(JScrollView::getScrollX.callInt(sl_null, handle));
			ret.y = (sl_scroll_pos)(JScrollView::getScrollY.callInt(sl_null, handle));
			return ret;
		}
		return ScrollPoint::zero();
	}

	ScrollPoint ScrollView::_getScrollRange_NW()
	{
		Ref<View> content = m_viewContent;
		if (content.isNotNull()) {
			ScrollPoint ret;
			ret.x = (sl_scroll_pos)(content->getWidth() - getWidth());
			if (ret.x < 0) {
				ret.x = 0;
			}
			ret.y = (sl_scroll_pos)(content->getHeight() - getHeight());
			if (ret.y < 0) {
				ret.y = 0;
			}
			return ret;
		}
		return ScrollPoint::zero();
	}

	void ScrollView::_setBorder_NW(sl_bool flag)
	{
	}

	void ScrollView::_setBackgroundColor_NW(const Color& color)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JScrollView::setBackgroundColor.call(sl_null, handle, color.getARGB());
		}
	}

	void ScrollView::_setScrollBarsVisible_NW(sl_bool flagHorizontal, sl_bool flagVertical)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JScrollView::setScrollBarsVisible.call(sl_null, handle, flagHorizontal, flagVertical);
		}
	}

	void ScrollView::_setPaging_NW(sl_bool flagPaging, sl_ui_len pageWidth, sl_ui_len pageHeight)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JScrollView::setPaging.call(sl_null, handle, flagPaging, pageWidth, pageHeight);
		}
	}

}

#endif
