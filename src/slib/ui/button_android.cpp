#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/ui/button.h"

#include "view_android.h"

namespace slib
{

	SLIB_JNI_BEGIN_CLASS(_JAndroidButton, "slib/platform/android/ui/view/UiButton")

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
			JniLocal<jobject> handle = _JAndroidButton::create.callObject(sl_null, parent->getContext());
			ret = Android_ViewInstance::create<Android_ViewInstance>(this, parent, handle.get());
			if (ret.isNotNull()) {
				jobject handle = ret->getHandle();
				JniLocal<jstring> jtext = Jni::getJniString(m_text);
				_JAndroidButton::setText.callBoolean(sl_null, handle, jtext.get());
				Ref<Font> font = getFont();
				jobject jfont = GraphicsPlatform::getNativeFont(font.get());
				if (jfont) {
					_JAndroidButton::setFont.callBoolean(sl_null, handle, jfont);
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
			_JAndroidButton::setText.callBoolean(sl_null, handle, jstr.get());
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
				_JAndroidButton::setFont.callBoolean(sl_null, handle, jfont);
			}
		}
	}

}

#endif
