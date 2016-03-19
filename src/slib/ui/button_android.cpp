#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/ui/button.h"

#include "view_android.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_JNI_BEGIN_CLASS(_JAndroidButton, "slib/platform/android/ui/view/UiButton")

	SLIB_JNI_STATIC_METHOD(create, "_create", "(Landroid/content/Context;)Lslib/platform/android/ui/view/UiButton;");

	SLIB_JNI_STATIC_METHOD(getText, "_getText", "(Landroid/view/View;)Ljava/lang/String;");
	SLIB_JNI_STATIC_METHOD(setText, "_setText", "(Landroid/view/View;Ljava/lang/String;)Z");
	SLIB_JNI_STATIC_METHOD(setFont, "_setFont", "(Landroid/view/View;Lslib/platform/android/ui/UiFont;)Z");

SLIB_JNI_END_CLASS

Ref<ViewInstance> Button::createInstance(ViewInstance* _parent)
{
	Ref<Android_ViewInstance> ret;
	Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
	if (parent) {
		JniLocal<jobject> handle = _JAndroidButton::create.callObject(sl_null, parent->getContext());
		ret = Android_ViewInstance::create<Android_ViewInstance>(this, parent, handle.get());
		if (ret.isNotNull()) {
			jobject handle = ret->getHandle();
			JniLocal<jstring> jtext = Jni::getJniString(m_text);
			_JAndroidButton::setText.callBoolean(sl_null, handle, jtext.get());
			Ref<Font> font = m_font;
			Ref<FontInstance> fontInstance;
			jobject jfont = UIPlatform::getNativeFont(font.ptr, fontInstance);
			if (jfont) {
				_JAndroidButton::setFont.callBoolean(sl_null, handle, jfont);
			}
		}
	}
	return ret;
}

String Button::getText()
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		m_text = _JAndroidButton::getText.callString(sl_null, handle);
	}
	return m_text;
}

void Button::setText(const String& text)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		JniLocal<jstring> jstr = Jni::getJniString(text);
		if (_JAndroidButton::setText.callBoolean(sl_null, handle, jstr.get()) != 0) {
			m_text = text;
		}
	} else {
		m_text = text;
	}
}

void Button::setFont(const Ref<Font>& font)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		Ref<FontInstance> fontInstance;
		jobject jfont = UIPlatform::getNativeFont(font.ptr, fontInstance);
		if (jfont) {
			_JAndroidButton::setFont.callBoolean(sl_null, handle, jfont);
		}
	}
	m_font = font;
}

void Button::setDefaultButton(sl_bool flag)
{
}

SLIB_UI_NAMESPACE_END

#endif
