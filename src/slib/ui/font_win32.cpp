#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/graphics/font.h"

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/platform.h"

#pragma comment (lib, "gdiplus.lib")

SLIB_UI_NAMESPACE_BEGIN

class _Win32_FontInstance : public FontInstance
{
	SLIB_DECLARE_OBJECT

public:
	FontDesc m_desc;

	Gdiplus::Font* m_fontGdiplus;
	sl_bool m_flagCreatedGdiplus;

	HFONT m_fontGDI;
	sl_bool m_flagCreatedGDI;
	
public:
	_Win32_FontInstance()
	{
		m_fontGdiplus = sl_null;
		m_flagCreatedGdiplus = sl_false;

		m_fontGDI = NULL;
		m_flagCreatedGDI = sl_false;
	}

	~_Win32_FontInstance()
	{
		if (m_fontGdiplus) {
			delete m_fontGdiplus;
		}
		if (m_fontGDI) {
			::DeleteObject(m_fontGDI);
		}
	}
	
public:
	static Ref<_Win32_FontInstance> _create(const FontDesc& desc)
	{
		Ref<_Win32_FontInstance> ret = new _Win32_FontInstance();
		if (ret.isNotNull()) {
			ret->m_desc = desc;
		}
		return ret;
	}

	void _createGdiplus()
	{
		ObjectLocker lock(this);
		if (m_flagCreatedGdiplus) {
			return;
		}
		m_flagCreatedGdiplus = sl_true;

		const FontDesc& desc = m_desc;

		int style = 0;
		if (desc.flagBold) {
			style |= Gdiplus::FontStyleBold;
		}
		if (desc.flagItalic) {
			style |= Gdiplus::FontStyleItalic;
		}
		if (desc.flagUnderline) {
			style |= Gdiplus::FontStyleUnderline;
		}
		String16 fontName = desc.familyName;
		Gdiplus::Font* font = new Gdiplus::Font(
			(LPCWSTR)(fontName.getData())
			, desc.size
			, style
			, Gdiplus::UnitPixel);
		m_fontGdiplus = font;
	}

	void _createGDI()
	{
		ObjectLocker lock(this);
		if (m_flagCreatedGDI) {
			return;
		}
		m_flagCreatedGDI = sl_true;

		const FontDesc& desc = m_desc;

		int height = (int)(desc.size);
		int weight;
		if (desc.flagBold) {
			weight = 400;
		} else {
			weight = 700;
		}
		DWORD bItalic;
		if (desc.flagItalic) {
			bItalic = TRUE;
		} else {
			bItalic = FALSE;
		}
		DWORD bUnderline;
		if (desc.flagUnderline) {
			bUnderline = TRUE;
		} else {
			bUnderline = FALSE;
		}
		String16 fontName = desc.familyName;
		HFONT hFont = ::CreateFontW(height, 0, 0, 0, weight, bItalic, bUnderline
			, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS
			, ANTIALIASED_QUALITY
			, DEFAULT_PITCH
			, (LPCWSTR)(fontName.getData()));
		m_fontGDI = hFont;
	}

	static Ref<_Win32_FontInstance> getInstance(Font* font)
	{
		Ref<_Win32_FontInstance> ret;
		if (font) {
			Ref<FontInstance> _instance = font->getInstance();
			if (_Win32_FontInstance::checkInstance(_instance.ptr)) {
				ret = Ref<_Win32_FontInstance>::from(_instance);
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

SLIB_DEFINE_OBJECT(_Win32_FontInstance, FontInstance)

Gdiplus::Font* UIPlatform::getGdiplusFont(Font* font, Ref<FontInstance>& instanceOut)
{
	Ref<_Win32_FontInstance> instance = _Win32_FontInstance::getInstance(font);
	instanceOut = instance;
	if (instance.isNotNull()) {
		instance->_createGdiplus();
		return instance->m_fontGdiplus;
	}
	return NULL;
}

HFONT UIPlatform::getGdiFont(Font* font, Ref<FontInstance>& instanceOut)
{
	Ref<_Win32_FontInstance> instance = _Win32_FontInstance::getInstance(font);
	instanceOut = instance;
	if (instance.isNotNull()) {
		instance->_createGDI();
		return instance->m_fontGDI;
	}
	return NULL;
}

class _Win32_FontStatic
{
public:
	Gdiplus::Bitmap* image;
	Gdiplus::Graphics* graphics;

public:
	_Win32_FontStatic()
	{
		graphics = sl_null;
		image = new Gdiplus::Bitmap(1, 1, PixelFormat24bppRGB);
		if (image) {
			graphics = new Gdiplus::Graphics(image);
		}
	}

	~_Win32_FontStatic()
	{
		if (graphics) {
			delete graphics;
		}
		if (image) {
			delete image;
		}
	}
};

Size UI::getFontTextSize(const Ref<Font>& font, const String& _text)
{
	Size ret(0, 0);

	if (font.isNull()) {
		return ret;
	}

	Ref<FontInstance> instance;
	Gdiplus::Font* handle = UIPlatform::getGdiplusFont(font.ptr, instance);
	if (!handle) {
		return ret;
	}

	SLIB_SAFE_STATIC(_Win32_FontStatic, fs);
	if (fs.graphics) {
		String16 text = _text;
		Gdiplus::StringFormat format(Gdiplus::StringFormatFlagsNoWrap | Gdiplus::StringFormatFlagsNoClip);
		Gdiplus::RectF bound;
		Gdiplus::PointF origin(0, 0);
		Gdiplus::Status result = fs.graphics->MeasureString((WCHAR*)(text.getData()), text.getLength(), handle, origin, &format, &bound);
		if (result == Gdiplus::Ok) {
			ret.x = bound.Width;
			ret.y = bound.Height;
		}
	}
	return ret;
}

SLIB_UI_NAMESPACE_END

#endif
