#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/graphics/font.h"

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/platform.h"

SLIB_UI_NAMESPACE_BEGIN

SLIB_JNI_BEGIN_CLASS(_JAndroidPointF, "android/graphics/PointF")
	SLIB_JNI_FLOAT_FIELD(x);
	SLIB_JNI_FLOAT_FIELD(y);
SLIB_JNI_END_CLASS

SLIB_JNI_BEGIN_CLASS(_JAndroidFont, "slib/platform/android/ui/UiFont")
	SLIB_JNI_STATIC_METHOD(create, "create", "(Ljava/lang/String;FI)Lslib/platform/android/ui/UiFont;");
	SLIB_JNI_METHOD(getTextSize, "getTextSize", "(Ljava/lang/String;)Landroid/graphics/PointF;");
SLIB_JNI_END_CLASS

class _Android_FontInstance : public FontInstance
{
	SLIB_DECLARE_OBJECT(_Android_FontInstance, FontInstance)
public:
	JniGlobal<jobject> m_font;
	
	_Android_FontInstance()
	{
	}
	
	static Ref<_Android_FontInstance> _create(const FontDesc& desc)
	{
		Ref<_Android_FontInstance> ret;
		
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
				ret = new _Android_FontInstance();
				if (ret.isNotNull()) {
					ret->m_font = gfont;
					return ret;
				}
			}
		}
		return ret;
	}
	
	Size getTextSize(const String& _text)
	{
		JniLocal<jstring> jtext = Jni::getJniString(_text);
		JniLocal<jobject> size = _JAndroidFont::getTextSize.callObject(m_font, jtext.get());
		Size ret;
		ret.x = _JAndroidPointF::x.get(size.get());
		ret.y = _JAndroidPointF::y.get(size.get());
		return ret;
	}

	static Ref<_Android_FontInstance> getInstance(Font* font)
	{
		Ref<_Android_FontInstance> ret;
		if (font) {
			Ref<FontInstance> _instance = font->getInstance();
			if (_Android_FontInstance::checkInstance(_instance)) {
				ret = Ref<_Android_FontInstance>::from(_instance);
			} else {
				FontDesc desc;
				font->getDesc(desc);
				ret = _create(desc);
				if (ret.isNotNull()) {
					font->setInstance(ret);
				}
			}
		}
		return ret;
	}
};

jobject UIPlatform::getNativeFont(Font* font, Ref<FontInstance>& instanceOut)
{
	Ref<_Android_FontInstance> instance = _Android_FontInstance::getInstance(font);
	instanceOut = instance;
	if (instance.isNotNull()) {
		return instance->m_font.get();
	}
	return 0;
}

Size UI::getFontTextSize(const Ref<Font>& _font, const String& text)
{
	Ref<Font> font = _font;
	Ref<_Android_FontInstance> instance = _Android_FontInstance::getInstance(font.get());
	if (instance.isNotNull()) {
		return instance->getTextSize(text);
	}
	return Size::zero();
}
SLIB_UI_NAMESPACE_END

#endif


