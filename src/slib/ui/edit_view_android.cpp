#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/ui/edit_view.h"

#include "view_android.h"

SLIB_UI_NAMESPACE_BEGIN

void JNICALL _AndroidEditView_nativeOnChange(JNIEnv* env, jobject _this, jlong instance)
{
	Ref<View> _view = Android_ViewInstance::getAndroidView(instance);
	if (EditView::checkInstance(_view.ptr)) {
		EditView* view = (EditView*)(_view.ptr);
		String text = view->getText();
		String textNew = view->dispatchChange(text);
		if (text != textNew) {
			view->setText(text);
		}
	}
}

SLIB_JNI_BEGIN_CLASS(_JAndroidEditView, "slib/platform/android/ui/view/UiEditView")

	SLIB_JNI_STATIC_METHOD(create, "_create", "(Landroid/content/Context;I)Landroid/view/View;");

	SLIB_JNI_STATIC_METHOD(getText, "_getText", "(Landroid/view/View;)Ljava/lang/String;");
	SLIB_JNI_STATIC_METHOD(setText, "_setText", "(Landroid/view/View;Ljava/lang/String;)Z");
	SLIB_JNI_STATIC_METHOD(isBorder, "_isBorder", "(Landroid/view/View;)Z");
	SLIB_JNI_STATIC_METHOD(setBorder, "_setBorder", "(Landroid/view/View;Z)Z");
	SLIB_JNI_STATIC_METHOD(getAlignment, "_getAlignment", "(Landroid/view/View;)I");
	SLIB_JNI_STATIC_METHOD(setAlignment, "_setAlignment", "(Landroid/view/View;I)Z");
	SLIB_JNI_STATIC_METHOD(getHintText, "_getHintText", "(Landroid/view/View;)Ljava/lang/String;");
	SLIB_JNI_STATIC_METHOD(setHintText, "_setHintText", "(Landroid/view/View;Ljava/lang/String;)Z");
	SLIB_JNI_STATIC_METHOD(isReadOnly, "_isReadOnly", "(Landroid/view/View;)Z");
	SLIB_JNI_STATIC_METHOD(setReadOnly, "_setReadOnly", "(Landroid/view/View;Z)Z");
	SLIB_JNI_STATIC_METHOD(isMultiLine, "_isMultiLine", "(Landroid/view/View;)Z");
	SLIB_JNI_STATIC_METHOD(setMultiLine, "_setMultiLine", "(Landroid/view/View;Z)Z");
	SLIB_JNI_STATIC_METHOD(setTextColor, "_setTextColor", "(Landroid/view/View;I)Z");
	SLIB_JNI_STATIC_METHOD(setBackgroundColor, "_setBackgroundColor", "(Landroid/view/View;I)Z");
	SLIB_JNI_STATIC_METHOD(setFont, "_setFont", "(Landroid/view/View;Lslib/platform/android/ui/UiFont;)Z");
	SLIB_JNI_STATIC_METHOD(setReturnKeyType, "_setReturnKeyType", "(Landroid/view/View;I)Z");
	SLIB_JNI_STATIC_METHOD(setKeyboardType, "_setKeyboardType", "(Landroid/view/View;I)Z");

	SLIB_JNI_NATIVE(nativeOnChange, "nativeOnChange", "(J)V", _AndroidEditView_nativeOnChange);

SLIB_JNI_END_CLASS

class _EditView : public EditView
{
public:
	void applyParameters(jobject handle)
	{
		JniLocal<jstring> jtext = Jni::getJniString(m_text);
		_JAndroidEditView::setText.callBoolean(sl_null, handle, jtext.get());
		_JAndroidEditView::setBorder.callBoolean(sl_null, handle, isBorder());
		_JAndroidEditView::setAlignment.callBoolean(sl_null, handle, m_textAlignment.value);
		JniLocal<jstring> jhintText = Jni::getJniString(m_hintText);
		_JAndroidEditView::setHintText.callBoolean(sl_null, handle, jhintText.get());
		_JAndroidEditView::setReadOnly.callBoolean(sl_null, handle, m_flagReadOnly);
		_JAndroidEditView::setMultiLine.callBoolean(sl_null, handle, m_flagMultiLine);
		_JAndroidEditView::setTextColor.callBoolean(sl_null, handle, m_textColor.getARGB());
		_JAndroidEditView::setBackgroundColor.callBoolean(sl_null, handle, getBackgroundColor().getARGB());
		Ref<Font> font = getFont();
		jobject jfont = GraphicsPlatform::getNativeFont(font.ptr);
		if (jfont) {
			_JAndroidEditView::setFont.callBoolean(sl_null, handle, jfont);
		}
		_JAndroidEditView::setReturnKeyType.callBoolean(sl_null, handle, (int)m_returnKeyType);
		_JAndroidEditView::setKeyboardType.callBoolean(sl_null, handle, (int)m_keyboardType);
	}
};

Ref<ViewInstance> EditView::createNativeWidget(ViewInstance* _parent)
{
	Ref<Android_ViewInstance> ret;
	Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
	if (parent) {
		JniLocal<jobject> handle = _JAndroidEditView::create.callObject(sl_null, parent->getContext(), 0);
		ret = Android_ViewInstance::create<Android_ViewInstance>(this, parent, handle.get());
		if (ret.isNotNull()) {
			jobject handle = ret->getHandle();
			((_EditView*)this)->applyParameters(handle);
		}
	}
	return ret;
}

Ref<ViewInstance> PasswordView::createNativeWidget(ViewInstance* _parent)
{
	Ref<Android_ViewInstance> ret;
	Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
	if (parent) {
		JniLocal<jobject> handle = _JAndroidEditView::create.callObject(sl_null, parent->getContext(), 1);
		ret = Android_ViewInstance::create<Android_ViewInstance>(this, parent, handle.get());
		if (ret.isNotNull()) {
			jobject handle = ret->getHandle();
			((_EditView*)this)->applyParameters(handle);
		}
	}
	return ret;
}

Ref<ViewInstance> TextArea::createNativeWidget(ViewInstance* _parent)
{
	Ref<Android_ViewInstance> ret;
	Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
	if (parent) {
		JniLocal<jobject> handle = _JAndroidEditView::create.callObject(sl_null, parent->getContext(), 2);
		ret = Android_ViewInstance::create<Android_ViewInstance>(this, parent, handle.get());
		if (ret.isNotNull()) {
			jobject handle = ret->getHandle();
			((_EditView*)this)->applyParameters(handle);
		}
	}
	return ret;
}

void EditView::_getText_NW()
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		m_text = _JAndroidEditView::getText.callString(sl_null, handle);
	}
}

void EditView::_setText_NW(const String& text)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		JniLocal<jstring> jstr = Jni::getJniString(text);
		_JAndroidEditView::setText.callBoolean(sl_null, handle, jstr.get());
	}
}

void EditView::_setBorder_NW(sl_bool flag)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		_JAndroidEditView::setBorder.callBoolean(sl_null, handle, flag);
	}
}

void EditView::_setTextAlignment_NW(Alignment align)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		_JAndroidEditView::setAlignment.callBoolean(sl_null, handle, align.value);
	}
}

void EditView::_setHintText_NW(const String& text)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		JniLocal<jstring> jstr = Jni::getJniString(text);
		_JAndroidEditView::setHintText.callBoolean(sl_null, handle, jstr.get());
	}
}

void EditView::_setReadOnly_NW(sl_bool flag)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		_JAndroidEditView::setReadOnly.callBoolean(sl_null, handle, flag);
	}
}

void EditView::_setMultiLine_NW(sl_bool flag)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		_JAndroidEditView::setMultiLine.callBoolean(sl_null, handle, flag);
	}
}

void EditView::_setTextColor_NW(const Color& color)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		_JAndroidEditView::setTextColor.callBoolean(sl_null, handle, color.getARGB());
	}
}

void EditView::_setBackgroundColor_NW(const Color& color)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		_JAndroidEditView::setBackgroundColor.callBoolean(sl_null, handle, color.getARGB());
	}
}

void EditView::_setFont_NW(const Ref<Font>& font)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		jobject jfont = GraphicsPlatform::getNativeFont(font.ptr);
		if (jfont) {
			_JAndroidEditView::setFont.callBoolean(sl_null, handle, jfont);
		}
	}
}

void EditView::_setReturnKeyType_NW(UIReturnKeyType type)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		_JAndroidEditView::setReturnKeyType.callBoolean(sl_null, handle, (int)type);
	}
}

void EditView::_setKeyboardType_NW(UIKeyboardType type)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		_JAndroidEditView::setKeyboardType.callBoolean(sl_null, handle, (int)type);
	}
}

SLIB_UI_NAMESPACE_END

#endif
