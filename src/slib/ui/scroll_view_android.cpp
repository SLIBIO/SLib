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
				void applyContent(jobject sv, ViewInstance* scrollViewInstance)
				{
					Ref<View> viewContent = m_viewContent;
					if (viewContent.isNotNull()) {
						viewContent->attachToNewInstance(scrollViewInstance);
						viewContent->setParent(this);
					}
				}

				void onScroll(int x, int y)
				{
					_onScroll_NW((sl_scroll_pos)x, (sl_scroll_pos)y);
				}
			};

			class ScrollViewInstance : public Android_ViewInstance, public IScrollViewInstance
			{
				SLIB_DECLARE_OBJECT

			public:
				void refreshContentSize(ScrollView* view) override
				{
				}

				void setContentView(ScrollView* view, const Ref<View>& content) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						(static_cast<ScrollViewHelper*>(view))->applyContent(handle, this);
					}
				}

				sl_bool getScrollPosition(ScrollView* view, ScrollPoint& _out) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						_out.x = (sl_scroll_pos)(JScrollView::getScrollX.callInt(sl_null, handle));
						_out.y = (sl_scroll_pos)(JScrollView::getScrollY.callInt(sl_null, handle));
						return sl_true;
					}
					return sl_false;
				}

				sl_bool getScrollRange(ScrollView* view, ScrollPoint& _out) override
				{
					Ref<View> content = view->getContentView();
					if (content.isNotNull()) {
						_out.x = (sl_scroll_pos)(content->getWidth() - view->getWidth());
						if (_out.x < 0) {
							_out.x = 0;
						}
						_out.y = (sl_scroll_pos)(content->getHeight() - view->getHeight());
						if (_out.y < 0) {
							_out.y = 0;
						}
						return sl_true;
					}
					return sl_false;
				}

				void scrollTo(View* view, sl_scroll_pos x, sl_scroll_pos y, sl_bool flagAnimate) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JScrollView::scrollTo.call(sl_null, handle, (int)x, (int)y, flagAnimate ? 1 : 0);
					}
				}

				void setPaging(View* view, sl_bool flagPaging, sl_ui_len pageWidth, sl_ui_len pageHeight) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JScrollView::setPaging.call(sl_null, handle, flagPaging, pageWidth, pageHeight);
					}
				}

				void setScrollBarsVisible(View* view, sl_bool flagHorizontal, sl_bool flagVertical) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JScrollView::setScrollBarsVisible.call(sl_null, handle, flagHorizontal, flagVertical);
					}
				}

				void setBackgroundColor(View* view, const Color& color) override
				{
					jobject handle = m_handle.get();
					if (handle) {
						JScrollView::setBackgroundColor.call(sl_null, handle, color.getARGB());
					}
				}

			};

			SLIB_DEFINE_OBJECT(ScrollViewInstance, Android_ViewInstance)

			void JNICALL OnScroll(JNIEnv* env, jobject _this, jlong instance, int x, int y)
			{
				Ref<ScrollViewHelper> helper = CastRef<ScrollViewHelper>(Android_ViewInstance::findView(instance));
				if (helper.isNotNull()) {
					helper->onScroll(x, y);
				}
			}

		}
	}

	using namespace priv::scroll_view;

	Ref<ViewInstance> ScrollView::createNativeWidget(ViewInstance* _parent)
	{
		Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
		if (parent) {
			JniLocal<jobject> handle = JScrollView::create.callObject(sl_null, parent->getContext(), isVerticalScrolling());
			Ref<ScrollViewInstance> ret = Android_ViewInstance::create<ScrollViewInstance>(this, parent, handle.get());
			if (ret.isNotNull()) {
				jobject jhandle = ret->getHandle();
				JScrollView::setBackgroundColor.call(sl_null, jhandle, getBackgroundColor().getARGB());
				if (isPaging()) {
					JScrollView::setPaging.call(sl_null, jhandle, 1, getPageWidth(), getPageHeight());
				}
				JScrollView::setScrollBarsVisible.call(sl_null, jhandle, isHorizontalScrollBarVisible(), isVerticalScrollBarVisible());
				(static_cast<ScrollViewHelper*>(this))->applyContent(jhandle, ret.get());
				JScrollView::scrollTo.call(sl_null, jhandle, (int)(getScrollX()), (int)(getScrollY()), 0);
				return ret;
			}
		}
		return sl_null;
	}

	Ptr<IScrollViewInstance> ScrollView::getScrollViewInstance()
	{
		return CastRef<ScrollViewInstance>(getViewInstance());
	}

}

#endif
