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

#include "slib/ui/select_view.h"

#include "view_android.h"

namespace slib
{

	void JNICALL _priv_JAndroidSelectView_nativeOnSelect(JNIEnv* env, jobject _this, jlong instance, jint n)
	{
		Ref<View> _view = Android_ViewInstance::findView(instance);
		if (SelectView* view = CastInstance<SelectView>(_view.get())) {
			view->dispatchSelectItem(n);
		}
	}

	SLIB_JNI_BEGIN_CLASS(JAndroidSelectView, "slib/platform/android/ui/view/UiSelectView")

		SLIB_JNI_STATIC_METHOD(create, "_create", "(Landroid/content/Context;)Lslib/platform/android/ui/view/UiSelectView;");

		SLIB_JNI_STATIC_METHOD(applyList, "_applyList", "(Landroid/view/View;[Ljava/lang/String;)V");
		SLIB_JNI_STATIC_METHOD(getSelectedIndex, "_getSelectedIndex", "(Landroid/view/View;)I");
		SLIB_JNI_STATIC_METHOD(select, "_select", "(Landroid/view/View;I)V");
		SLIB_JNI_STATIC_METHOD(setAlignment, "_setAlignment", "(Landroid/view/View;I)Z");
		SLIB_JNI_STATIC_METHOD(setTextColor, "_setTextColor", "(Landroid/view/View;I)Z");
		SLIB_JNI_STATIC_METHOD(setBorder, "_setBorder", "(Landroid/view/View;Z)Z");
		SLIB_JNI_STATIC_METHOD(setBackgroundColor, "_setBackgroundColor", "(Landroid/view/View;I)Z");
		SLIB_JNI_STATIC_METHOD(setFont, "_setFont", "(Landroid/view/View;Lslib/platform/android/ui/UiFont;)Z");

		SLIB_JNI_NATIVE(nativeOnSelect, "nativeOnSelect", "(JI)V", _priv_JAndroidSelectView_nativeOnSelect);

	SLIB_JNI_END_CLASS

	class _priv_SelectView : public SelectView
	{
	public:
		void _copyItems(jobject jview)
		{
			ListLocker<String> titles(m_titles);
			sl_uint32 n = (sl_uint32)(titles.count);
			JniLocal<jobjectArray> arr = Jni::newStringArray(n);
			if (arr.isNotNull()) {
				for (sl_uint32 i = 0; i < titles.count; i++) {
					Jni::setStringArrayElement(arr, i, titles[i]);
				}
				titles.unlock();
				JAndroidSelectView::applyList.call(sl_null, jview, arr.get());
				_select(jview, m_indexSelected);
			}
		}

		sl_uint32 _getSelectedIndex(jobject jview)
		{
			jint n = JAndroidSelectView::getSelectedIndex.callInt(sl_null, jview);
			if (n < 0) {
				n = 0;
			}
			return n;
		}
		
		void _select(jobject jview, sl_uint32 n)
		{
			JAndroidSelectView::select.call(sl_null, jview, n);
		}

	};

	Ref<ViewInstance> SelectView::createNativeWidget(ViewInstance* _parent)
	{
		Ref<Android_ViewInstance> ret;
		Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
		if (parent) {
			JniLocal<jobject> handle = JAndroidSelectView::create.callObject(sl_null, parent->getContext());
			ret = Android_ViewInstance::create<Android_ViewInstance>(this, parent, handle.get());
			if (ret.isNotNull()) {
				jobject handle = ret->getHandle();

				((_priv_SelectView*)this)->_copyItems(handle);

				JAndroidSelectView::setAlignment.callBoolean(sl_null, handle, m_textAlignment.value);
				JAndroidSelectView::setTextColor.callBoolean(sl_null, handle, m_textColor.getARGB());
				JAndroidSelectView::setBorder.callBoolean(sl_null, handle, isBorder());
				JAndroidSelectView::setBackgroundColor.callBoolean(sl_null, handle, getBackgroundColor().getARGB());

				Ref<Font> font = getFont();
				jobject jfont = GraphicsPlatform::getNativeFont(font.get());
				if (jfont) {
					JAndroidSelectView::setFont.callBoolean(sl_null, handle, jfont);
				}
			}
		}
		return ret;
	}

	void SelectView::_getSelectedIndex_NW()
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			m_indexSelected = ((_priv_SelectView*)this)->_getSelectedIndex(handle);
		}
	}

	void SelectView::_select_NW(sl_uint32 index)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			((_priv_SelectView*)this)->_select(handle, index);
		}
	}

	void SelectView::_refreshItemsCount_NW()
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			((_priv_SelectView*)this)->_copyItems(handle);
		}
	}

	void SelectView::_refreshItemsContent_NW()
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			((_priv_SelectView*)this)->_copyItems(handle);
		}
	}

	void SelectView::_setItemTitle_NW(sl_uint32 index, const String& title)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			((_priv_SelectView*)this)->_copyItems(handle);
		}
	}

	void SelectView::_setTextAlignment_NW(Alignment align)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JAndroidSelectView::setAlignment.callBoolean(sl_null, handle, align.value);
		}
	}

	void SelectView::_setTextColor_NW(const Color& color)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JAndroidSelectView::setTextColor.callBoolean(sl_null, handle, color.getARGB());
		}
	}

	void SelectView::_setBorder_NW(sl_bool flag)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JAndroidSelectView::setBorder.callBoolean(sl_null, handle, flag);
		}
	}

	void SelectView::_setBackgroundColor_NW(const Color& color)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JAndroidSelectView::setBackgroundColor.callBoolean(sl_null, handle, color.getARGB());
		}
	}

	void SelectView::_setFont_NW(const Ref<Font>& font)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			jobject jfont = GraphicsPlatform::getNativeFont(font.get());
			if (jfont) {
				JAndroidSelectView::setFont.callBoolean(sl_null, handle, jfont);
			}
		}
	}

}

#endif
