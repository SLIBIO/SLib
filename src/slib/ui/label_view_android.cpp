#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/ui/label_view.h"

#include "view_android.h"


SLIB_JNI_BEGIN_CLASS(_JAndroidEditView, "slib/platform/android/ui/view/UiEditView")

	SLIB_JNI_STATIC_METHOD(create, "_create", "(Landroid/content/Context;I)Landroid/view/View;");

	SLIB_JNI_STATIC_METHOD(getText, "_getText", "(Landroid/view/View;)Ljava/lang/String;");
	SLIB_JNI_STATIC_METHOD(setText, "_setText", "(Landroid/view/View;Ljava/lang/String;)Z");
	SLIB_JNI_STATIC_METHOD(isBorder, "_isBorder", "(Landroid/view/View;)Z");
	SLIB_JNI_STATIC_METHOD(setBorder, "_setBorder", "(Landroid/view/View;Z)Z");
	SLIB_JNI_STATIC_METHOD(getAlignment, "_getAlignment", "(Landroid/view/View;)I");
	SLIB_JNI_STATIC_METHOD(setAlignment, "_setAlignment", "(Landroid/view/View;I)Z");
	SLIB_JNI_STATIC_METHOD(setTextColor, "_setTextColor", "(Landroid/view/View;I)Z");
	SLIB_JNI_STATIC_METHOD(setBackgroundColor, "_setBackgroundColor", "(Landroid/view/View;I)Z");
	SLIB_JNI_STATIC_METHOD(setFont, "_setFont", "(Landroid/view/View;Lslib/platform/android/ui/UiFont;)Z");

SLIB_JNI_END_CLASS


SLIB_UI_NAMESPACE_BEGIN
class _LabelView : public LabelView
{
public:
	void applyParameters(jobject handle)
	{
		JniLocal<jstring> jtext = Jni::getJniString(m_text);
		_JAndroidEditView::setText.callBoolean(sl_null, handle, jtext.get());
		_JAndroidEditView::setBorder.callBoolean(sl_null, handle, m_flagBorder);
		_JAndroidEditView::setAlignment.callBoolean(sl_null, handle, m_textAlignment);
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

Ref<ViewInstance> LabelView::createInstance(ViewInstance* _parent)
{
	Ref<Android_ViewInstance> ret;
	Android_ViewInstance* parent = (Android_ViewInstance*)_parent;
	if (parent) {
		JniLocal<jobject> handle = _JAndroidEditView::create.callObject(sl_null, parent->getContext(), 3);
		ret = Android_ViewInstance::create<Android_ViewInstance>(this, parent, handle.get());
		if (ret.isNotNull()) {
			jobject handle = ret->getHandle();
			((_LabelView*)this)->applyParameters(handle);
		}
	}
	return ret;
}

String LabelView::getText()
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		m_text = _JAndroidEditView::getText.callString(sl_null, handle);
	}
	return m_text;
}

void LabelView::setText(const String& text)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		JniLocal<jstring> jstr = Jni::getJniString(text);
		_JAndroidEditView::setText.callBoolean(sl_null, handle, jstr.get());
	}
	m_text = text;
}

sl_bool LabelView::isBorder()
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		m_flagBorder = _JAndroidEditView::isBorder.callBoolean(sl_null, handle);
	}
	return m_flagBorder;
}

void LabelView::setBorder(sl_bool flag)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		_JAndroidEditView::setBorder.callBoolean(sl_null, handle, flag);
	}
	m_flagBorder = flag;
}

Alignments LabelView::getTextAlignment()
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		m_textAlignment = _JAndroidEditView::getAlignment.callInt(sl_null, handle);
	}
	return m_textAlignment;
}

void LabelView::setTextAlignment(Alignments align)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		_JAndroidEditView::setAlignment.callBoolean(sl_null, handle, align);
	}
	m_textAlignment = align;
}

Color LabelView::getTextColor()
{
	return m_textColor;
}

void LabelView::setTextColor(const Color& color)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		_JAndroidEditView::setTextColor.callBoolean(sl_null, handle, color.getARGB());
	}
	m_textColor = color;
}

Color LabelView::getBackgroundColor()
{
	return m_backgroundColor;
}

void LabelView::setBackgroundColor(const Color& color)
{
	jobject handle = UIPlatform::getViewHandle(this);
	if (handle) {
		_JAndroidEditView::setBackgroundColor.callBoolean(sl_null, handle, color.getARGB());
	}
	m_backgroundColor = color;
}

void LabelView::setFont(const Ref<Font>& font)
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
