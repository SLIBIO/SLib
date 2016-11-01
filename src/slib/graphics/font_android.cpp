#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/graphics/font.h"
#include "../../../inc/slib/graphics/platform.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

SLIB_JNI_BEGIN_CLASS(_JAndroidPointF, "android/graphics/PointF")
	SLIB_JNI_FLOAT_FIELD(x);
	SLIB_JNI_FLOAT_FIELD(y);
SLIB_JNI_END_CLASS

SLIB_JNI_BEGIN_CLASS(_JAndroidFontMetrics, "android/graphics/Paint$FontMetrics")
	SLIB_JNI_FLOAT_FIELD(ascent);
	SLIB_JNI_FLOAT_FIELD(descent);
	SLIB_JNI_FLOAT_FIELD(leading);
SLIB_JNI_END_CLASS

SLIB_JNI_BEGIN_CLASS(_JAndroidFont, "slib/platform/android/ui/UiFont")
	SLIB_JNI_STATIC_METHOD(create, "create", "(Ljava/lang/String;FI)Lslib/platform/android/ui/UiFont;");
	SLIB_JNI_METHOD(getTextSize, "getTextSize", "(Ljava/lang/String;)Landroid/graphics/PointF;");
	SLIB_JNI_METHOD(getFontMetrics, "getFontMetrics", "()Landroid/graphics/Paint$FontMetrics;");
SLIB_JNI_END_CLASS


Size Font::getTextSize(const String& text)
{
	jobject font = GraphicsPlatform::getNativeFont(this);
	if (font) {	
		JniLocal<jstring> jtext = Jni::getJniString(text);
		JniLocal<jobject> size = _JAndroidFont::getTextSize.callObject(font, jtext.get());
		Size ret;
		ret.x = _JAndroidPointF::x.get(size.get());
		ret.y = _JAndroidPointF::y.get(size.get());
		return ret;
	}
	return Size::zero();
}

sl_bool Font::getFontMetrics(FontMetrics& _out)
{
	jobject font = GraphicsPlatform::getNativeFont(this);
	if (font) {
		JniLocal<jobject> fm = _JAndroidFont::getFontMetrics.callObject(font);
		if (fm.isNotNull()) {
			_out.ascent = -(_JAndroidFontMetrics::ascent.get(fm.get()));
			_out.descent = _JAndroidFontMetrics::descent.get(fm.get());
			_out.leading = _JAndroidFontMetrics::leading.get(fm.get());
			return sl_true;
		}
	}
	return sl_false;
}

class _Android_FontObject : public Referable
{
public:
	JniGlobal<jobject> m_font;
	
public:	
	_Android_FontObject(const FontDesc& desc)
	{
		int style = 0;
		if (desc.flagBold) {
			style |= 1;
		}
		if (desc.flagItalic) {
			style |= 2;
		}
		if (desc.flagUnderline) {
			style |= 4;
		}
		float size = (float)(desc.size);
		JniLocal<jstring> fontName = Jni::getJniString(desc.familyName);
		JniLocal<jobject> jfont = _JAndroidFont::create.callObject(sl_null, fontName.value, size, style);
		if (jfont.isNotNull()) {
			JniGlobal<jobject> gfont = jfont;
			if (gfont.isNotNull()) {
				m_font = gfont;
			}
		}
	}

};

class _Font : public Font
{
public:
	_Android_FontObject* getPlatformObject()
	{
		if (m_platformObject.isNull()) {
			SpinLocker lock(&m_lock);
			if (m_platformObject.isNull()) {
				m_platformObject = new _Android_FontObject(m_desc);
			}
		}
		return (_Android_FontObject*)(m_platformObject.ptr);;
	}

	jobject getPlatformHandle()
	{
		_Android_FontObject* po = getPlatformObject();
		if (po) {
			return po->m_font.get();
		}
		return 0;
	}
};

jobject GraphicsPlatform::getNativeFont(Font* font)
{
	if (font) {
		return ((_Font*)font)->getPlatformHandle();
	}
	return 0;
}

SLIB_GRAPHICS_NAMESPACE_END

#endif
