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

#include "slib/ui/edit_view.h"

#include "view_android.h"

namespace slib
{

	void JNICALL Android_EditView_nativeOnChange(JNIEnv* env, jobject _this, jlong instance)
	{
		Ref<View> _view = Android_ViewInstance::findView(instance);
		if (EditView* view = CastInstance<EditView>(_view.get())) {
			String text = view->getText();
			String textNew = text;
			view->dispatchChange(&textNew);
			if (text != textNew) {
				view->setText(text);
			}
		}
	}

	void JNICALL Android_EditView_nativeOnReturn(JNIEnv* env, jobject _this, jlong instance)
	{
		Ref<View> _view = Android_ViewInstance::findView(instance);
		if (EditView* view = CastInstance<EditView>(_view.get())) {
			view->dispatchReturnKey();
		}
	}

	SLIB_JNI_BEGIN_CLASS(JAndroidEditView, "slib/platform/android/ui/view/UiEditView")

		SLIB_JNI_STATIC_METHOD(create, "_create", "(Landroid/content/Context;I)Landroid/view/View;");

		SLIB_JNI_STATIC_METHOD(getText, "_getText", "(Landroid/view/View;)Ljava/lang/String;");
		SLIB_JNI_STATIC_METHOD(setText, "_setText", "(Landroid/view/View;Ljava/lang/String;)Z");
		SLIB_JNI_STATIC_METHOD(setBorder, "_setBorder", "(Landroid/view/View;Z)Z");
		SLIB_JNI_STATIC_METHOD(setAlignment, "_setAlignment", "(Landroid/view/View;I)Z");
		SLIB_JNI_STATIC_METHOD(setHintText, "_setHintText", "(Landroid/view/View;Ljava/lang/String;)Z");
		SLIB_JNI_STATIC_METHOD(setReadOnly, "_setReadOnly", "(Landroid/view/View;Z)Z");
		SLIB_JNI_STATIC_METHOD(setMultiLine, "_setMultiLine", "(Landroid/view/View;Z)Z");
		SLIB_JNI_STATIC_METHOD(setTextColor, "_setTextColor", "(Landroid/view/View;I)Z");
		SLIB_JNI_STATIC_METHOD(setHintTextColor, "_setHintTextColor", "(Landroid/view/View;I)Z");
		SLIB_JNI_STATIC_METHOD(setBackgroundColor, "_setBackgroundColor", "(Landroid/view/View;I)Z");
		SLIB_JNI_STATIC_METHOD(setFont, "_setFont", "(Landroid/view/View;Lslib/platform/android/ui/UiFont;)Z");
		SLIB_JNI_STATIC_METHOD(setReturnKeyType, "_setReturnKeyType", "(Landroid/view/View;I)Z");
		SLIB_JNI_STATIC_METHOD(setInputType, "_setInputType", "(Landroid/view/View;IIZ)Z");

		SLIB_JNI_NATIVE(nativeOnChange, "nativeOnChange", "(J)V", Android_EditView_nativeOnChange);
		SLIB_JNI_NATIVE(nativeOnReturn, "nativeOnReturn", "(J)V", Android_EditView_nativeOnReturn);

	SLIB_JNI_END_CLASS

	class EditView_Impl : public EditView
	{
	public:
		void applyParameters(jobject handle)
		{
			JniLocal<jstring> jtext = Jni::getJniString(m_text);
			JAndroidEditView::setText.callBoolean(sl_null, handle, jtext.get());
			JAndroidEditView::setBorder.callBoolean(sl_null, handle, isBorder());
			JAndroidEditView::setAlignment.callBoolean(sl_null, handle, m_textAlignment.value);
			JniLocal<jstring> jhintText = Jni::getJniString(m_hintText);
			JAndroidEditView::setHintText.callBoolean(sl_null, handle, jhintText.get());
			JAndroidEditView::setReadOnly.callBoolean(sl_null, handle, m_flagReadOnly);
			JAndroidEditView::setMultiLine.callBoolean(sl_null, handle, m_flagMultiLine);
			JAndroidEditView::setTextColor.callBoolean(sl_null, handle, m_textColor.getARGB());
			JAndroidEditView::setHintTextColor.callBoolean(sl_null, handle, m_hintTextColor.getARGB());
			JAndroidEditView::setBackgroundColor.callBoolean(sl_null, handle, getBackgroundColor().getARGB());
			Ref<Font> font = getFont();
			jobject jfont = GraphicsPlatform::getNativeFont(font.get());
			if (jfont) {
				JAndroidEditView::setFont.callBoolean(sl_null, handle, jfont);
			}
			JAndroidEditView::setReturnKeyType.callBoolean(sl_null, handle, (int)m_returnKeyType);
			JAndroidEditView::setInputType.callBoolean(sl_null, handle, (int)m_keyboardType, (int)m_autoCapitalizationType, m_flagPassword ? 1 : 0);
		}
	};

	Ref<ViewInstance> EditView::createNativeWidget(ViewInstance* _parent)
	{
		Ref<Android_ViewInstance> ret;
		Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
		if (parent) {
			JniLocal<jobject> handle = JAndroidEditView::create.callObject(sl_null, parent->getContext(), 0);
			ret = Android_ViewInstance::create<Android_ViewInstance>(this, parent, handle.get());
			if (ret.isNotNull()) {
				jobject handle = ret->getHandle();
				((EditView_Impl*)this)->applyParameters(handle);
			}
		}
		return ret;
	}

	Ref<ViewInstance> TextArea::createNativeWidget(ViewInstance* _parent)
	{
		Ref<Android_ViewInstance> ret;
		Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
		if (parent) {
			JniLocal<jobject> handle = JAndroidEditView::create.callObject(sl_null, parent->getContext(), 2);
			ret = Android_ViewInstance::create<Android_ViewInstance>(this, parent, handle.get());
			if (ret.isNotNull()) {
				jobject handle = ret->getHandle();
				((EditView_Impl*)this)->applyParameters(handle);
			}
		}
		return ret;
	}

	void EditView::_getText_NW()
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			m_text = JAndroidEditView::getText.callString(sl_null, handle);
		}
	}

	void EditView::_setText_NW(const String& text)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JniLocal<jstring> jstr = Jni::getJniString(text);
			JAndroidEditView::setText.callBoolean(sl_null, handle, jstr.get());
		}
	}

	void EditView::_setBorder_NW(sl_bool flag)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JAndroidEditView::setBorder.callBoolean(sl_null, handle, flag);
		}
	}

	void EditView::_setTextAlignment_NW(Alignment align)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JAndroidEditView::setAlignment.callBoolean(sl_null, handle, align.value);
		}
	}

	void EditView::_setHintText_NW(const String& text)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JniLocal<jstring> jstr = Jni::getJniString(text);
			JAndroidEditView::setHintText.callBoolean(sl_null, handle, jstr.get());
		}
	}

	void EditView::_setReadOnly_NW(sl_bool flag)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JAndroidEditView::setReadOnly.callBoolean(sl_null, handle, flag);
		}
	}

	void EditView::_setMultiLine_NW(sl_bool flag)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JAndroidEditView::setMultiLine.callBoolean(sl_null, handle, flag);
		}
	}

	void EditView::_setTextColor_NW(const Color& color)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JAndroidEditView::setTextColor.callBoolean(sl_null, handle, color.getARGB());
		}
	}

	void EditView::_setHintTextColor_NW(const Color& color)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JAndroidEditView::setHintTextColor.callBoolean(sl_null, handle, color.getARGB());
		}
	}

	void EditView::_setBackgroundColor_NW(const Color& color)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JAndroidEditView::setBackgroundColor.callBoolean(sl_null, handle, color.getARGB());
		}
	}

	void EditView::_setFont_NW(const Ref<Font>& font)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			jobject jfont = GraphicsPlatform::getNativeFont(font.get());
			if (jfont) {
				JAndroidEditView::setFont.callBoolean(sl_null, handle, jfont);
			}
		}
	}

	void EditView::_setReturnKeyType_NW(UIReturnKeyType type)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JAndroidEditView::setReturnKeyType.callBoolean(sl_null, handle, (int)type);
		}
	}

	void EditView::_setKeyboardType_NW(UIKeyboardType type)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JAndroidEditView::setInputType.callBoolean(sl_null, handle, (int)type, (int)m_autoCapitalizationType, m_flagPassword ? 1 : 0);
		}
	}

	void EditView::_setAutoCapitalizationType_NW(UIAutoCapitalizationType type)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JAndroidEditView::setInputType.callBoolean(sl_null, handle, (int)m_keyboardType, (int)type, m_flagPassword ? 1 : 0);
		}
	}

	void EditView::_setPassword_NW(sl_bool flag)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JAndroidEditView::setInputType.callBoolean(sl_null, handle, (int)m_keyboardType, (int)m_autoCapitalizationType, flag ? 1 : 0);
		}
	}

}

#endif
