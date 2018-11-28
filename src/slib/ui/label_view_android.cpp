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

#include "slib/ui/label_view.h"

#include "view_android.h"


SLIB_JNI_BEGIN_CLASS(JAndroidEditView, "slib/platform/android/ui/view/UiEditView")

	SLIB_JNI_STATIC_METHOD(create, "_create", "(Landroid/content/Context;I)Landroid/view/View;");

	SLIB_JNI_STATIC_METHOD(setText, "_setText", "(Landroid/view/View;Ljava/lang/String;)Z");
	SLIB_JNI_STATIC_METHOD(setBorder, "_setBorder", "(Landroid/view/View;Z)Z");
	SLIB_JNI_STATIC_METHOD(setAlignment, "_setAlignment", "(Landroid/view/View;I)Z");
	SLIB_JNI_STATIC_METHOD(setTextColor, "_setTextColor", "(Landroid/view/View;I)Z");
	SLIB_JNI_STATIC_METHOD(setBackgroundColor, "_setBackgroundColor", "(Landroid/view/View;I)Z");
	SLIB_JNI_STATIC_METHOD(setFont, "_setFont", "(Landroid/view/View;Lslib/platform/android/ui/UiFont;)Z");

SLIB_JNI_END_CLASS


namespace slib
{

	class _priv_LabelView : public LabelView
	{
	public:
		void applyParameters(jobject handle)
		{
			JniLocal<jstring> jtext = Jni::getJniString(m_text);
			JAndroidEditView::setText.callBoolean(sl_null, handle, jtext.get());
			JAndroidEditView::setBorder.callBoolean(sl_null, handle, isBorder());
			JAndroidEditView::setAlignment.callBoolean(sl_null, handle, m_textAlignment.value);
			JAndroidEditView::setTextColor.callBoolean(sl_null, handle, m_textColor.getARGB());
			JAndroidEditView::setBackgroundColor.callBoolean(sl_null, handle, getBackgroundColor().getARGB());
			Ref<Font> font = getFont();
			jobject jfont = GraphicsPlatform::getNativeFont(font.get());
			if (jfont) {
				JAndroidEditView::setFont.callBoolean(sl_null, handle, jfont);
			}
		}
	};

	Ref<ViewInstance> LabelView::createNativeWidget(ViewInstance* _parent)
	{
		Ref<Android_ViewInstance> ret;
		Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
		if (parent) {
			JniLocal<jobject> handle = JAndroidEditView::create.callObject(sl_null, parent->getContext(), 3);
			ret = Android_ViewInstance::create<Android_ViewInstance>(this, parent, handle.get());
			if (ret.isNotNull()) {
				jobject handle = ret->getHandle();
				((_priv_LabelView*)this)->applyParameters(handle);
			}
		}
		return ret;
	}

	void LabelView::_setText_NW(const String& text)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JniLocal<jstring> jstr = Jni::getJniString(text);
			JAndroidEditView::setText.callBoolean(sl_null, handle, jstr.get());
		}
	}

	void LabelView::_setTextAlignment_NW(Alignment align)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JAndroidEditView::setAlignment.callBoolean(sl_null, handle, align.value);
		}
	}

	void LabelView::_setTextColor_NW(const Color& color)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JAndroidEditView::setTextColor.callBoolean(sl_null, handle, color.getARGB());
		}
	}

	void LabelView::_setFont_NW(const Ref<Font>& font)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			jobject jfont = GraphicsPlatform::getNativeFont(font.get());
			if (jfont) {
				JAndroidEditView::setFont.callBoolean(sl_null, handle, jfont);
			}
		}
	}

	void LabelView::_setBorder_NW(sl_bool flag)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JAndroidEditView::setBorder.callBoolean(sl_null, handle, flag);
		}
	}

	void LabelView::_setBackgroundColor_NW(const Color& color)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JAndroidEditView::setBackgroundColor.callBoolean(sl_null, handle, color.getARGB());
		}
	}

}

#endif
