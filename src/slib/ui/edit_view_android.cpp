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

	SLIB_JNI_NATIVE(nativeOnChange, "nativeOnChange", "(J)V", _AndroidEditView_nativeOnChange);

SLIB_JNI_END_CLASS

class _EditView : public EditView
{
public:
	void applyParameters(jobject handle)
	{
		JniLocal<jstring> jtext = Jni::getJniString(m_text);
		_JAndroidEditView::setText.callBoolean(sl_null, handle, jtext.get());
		_JAndroidEditView::setBorder.callBoolean(sl_null, handle, m_flagBorder);
		_JAndroidEditView::setAlignment.callBoolean(sl_null, handle, m_textAlignment.value);
		JniLocal<jstring> jhintText = Jni::getJniString(m_hintText);
		_JAndroidEditView::setHintText.callBoolean(sl_null, handle, jhintText.get());
		_JAndroidEditView::setReadOnly.callBoolean(sl_null, handle, m_flagReadOnly);
		_JAndroidEditView::setMultiLine.callBoolean(sl_null, handle, m_flagMultiLine);
		_JAndroidEditView::setTextColor.callBoolean(sl_null, handle, m_textColor.getARGB());
		_JAndroidEditView::setBackgroundColor.callBoolean(sl_null, handle, m_backgroundColor.getARGB());
		Ref<Font> font = m_font;
		Ref<FontInstance> fontInstance;
		jobject jfont = UIPlatform::getNativeFont(font.ptr, fontInstance);
		if (jfont) {
			_JAndroidEditView::setFont.callBoolean(sl_null, handle, jfont);
		}
	}
};

Ref<ViewInstance> EditView::createInstance(ViewInstance* _parent)
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

Ref<ViewInstance> PasswordView::createInstance(ViewInstance* _parent)
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

Ref<ViewInstance> TextArea::createInstance(ViewInstance* _parent)
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

String EditView::getText()
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		m_text = _JAndroidEditView::getText.callString(sl_null, handle);
	}
	return m_text;
}

void EditView::setText(const String& text)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		JniLocal<jstring> jstr = Jni::getJniString(text);
		_JAndroidEditView::setText.callBoolean(sl_null, handle, jstr.get());
	}
    m_text = text;
}

sl_bool EditView::isBorder()
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		m_flagBorder = _JAndroidEditView::isBorder.callBoolean(sl_null, handle);
	}
	return m_flagBorder;
}

void EditView::setBorder(sl_bool flag)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		_JAndroidEditView::setBorder.callBoolean(sl_null, handle, flag);
	}
	m_flagBorder = flag;
}

Alignment EditView::getTextAlignment()
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		m_textAlignment = _JAndroidEditView::getAlignment.callInt(sl_null, handle);
	}
	return m_textAlignment;
}

void EditView::setTextAlignment(Alignment align)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		_JAndroidEditView::setAlignment.callBoolean(sl_null, handle, align.value);
	}
	m_textAlignment = align;
}

String EditView::getHintText()
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		m_hintText = _JAndroidEditView::getHintText.callString(sl_null, handle);
	}
	return m_hintText;
}

void EditView::setHintText(const String& text)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		JniLocal<jstring> jstr = Jni::getJniString(text);
		_JAndroidEditView::setHintText.callBoolean(sl_null, handle, jstr.get());
	}
	m_hintText = text;
}

sl_bool EditView::isReadOnly()
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		m_flagReadOnly = _JAndroidEditView::isReadOnly.callBoolean(sl_null, handle) != 0;
	}
	return m_flagReadOnly;
}

void EditView::setReadOnly(sl_bool flag)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		_JAndroidEditView::setReadOnly.callBoolean(sl_null, handle, flag);
	}
	m_flagReadOnly = flag;
}

sl_bool EditView::isMultiLine()
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		m_flagMultiLine = _JAndroidEditView::isMultiLine.callBoolean(sl_null, handle) != 0;
	}
	return m_flagMultiLine;
}

void EditView::setMultiLine(sl_bool flag)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		_JAndroidEditView::setMultiLine.callBoolean(sl_null, handle, flag);
	}
	m_flagMultiLine = flag;
}

Color EditView::getTextColor()
{
	return m_textColor;
}

void EditView::setTextColor(const Color& color)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		_JAndroidEditView::setTextColor.callBoolean(sl_null, handle, color.getARGB());
	}
	m_textColor = color;
}

Color EditView::getBackgroundColor()
{
	return m_backgroundColor;
}

void EditView::setBackgroundColor(const Color& color)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		_JAndroidEditView::setBackgroundColor.callBoolean(sl_null, handle, color.getARGB());
	}
	m_backgroundColor = color;
}

void EditView::setFont(const Ref<Font>& font)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		Ref<FontInstance> fontInstance;
		jobject jfont = UIPlatform::getNativeFont(font.ptr, fontInstance);
		if (jfont) {
			_JAndroidEditView::setFont.callBoolean(sl_null, handle, jfont);
		}
	}
	m_font = font;
}

SLIB_UI_NAMESPACE_END

#endif
