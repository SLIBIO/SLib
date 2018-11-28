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

#include "slib/ui/button.h"

#include "view_android.h"

namespace slib
{

	SLIB_JNI_BEGIN_CLASS(JAndroidButton, "slib/platform/android/ui/view/UiButton")

		SLIB_JNI_STATIC_METHOD(create, "_create", "(Landroid/content/Context;)Lslib/platform/android/ui/view/UiButton;");

		SLIB_JNI_STATIC_METHOD(getText, "_getText", "(Landroid/view/View;)Ljava/lang/String;");
		SLIB_JNI_STATIC_METHOD(setText, "_setText", "(Landroid/view/View;Ljava/lang/String;)Z");
		SLIB_JNI_STATIC_METHOD(setFont, "_setFont", "(Landroid/view/View;Lslib/platform/android/ui/UiFont;)Z");

	SLIB_JNI_END_CLASS

	Ref<ViewInstance> Button::createNativeWidget(ViewInstance* _parent)
	{
		Ref<Android_ViewInstance> ret;
		Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
		if (parent) {
			JniLocal<jobject> handle = JAndroidButton::create.callObject(sl_null, parent->getContext());
			ret = Android_ViewInstance::create<Android_ViewInstance>(this, parent, handle.get());
			if (ret.isNotNull()) {
				jobject handle = ret->getHandle();
				JniLocal<jstring> jtext = Jni::getJniString(m_text);
				JAndroidButton::setText.callBoolean(sl_null, handle, jtext.get());
				Ref<Font> font = getFont();
				jobject jfont = GraphicsPlatform::getNativeFont(font.get());
				if (jfont) {
					JAndroidButton::setFont.callBoolean(sl_null, handle, jfont);
				}
			}
		}
		return ret;
	}

	void Button::_setText_NW(const String& text)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			JniLocal<jstring> jstr = Jni::getJniString(text);
			JAndroidButton::setText.callBoolean(sl_null, handle, jstr.get());
		}
	}

	void Button::_setDefaultButton_NW(sl_bool flag)
	{
	}

	void Button::_setFont_NW(const Ref<Font>& font)
	{
		jobject handle = UIPlatform::getViewHandle(this);
		if (handle) {
			jobject jfont = GraphicsPlatform::getNativeFont(font.get());
			if (jfont) {
				JAndroidButton::setFont.callBoolean(sl_null, handle, jfont);
			}
		}
	}

}

#endif
