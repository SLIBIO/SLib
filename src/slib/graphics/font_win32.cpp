#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/graphics/font.h"
#include "../../../inc/slib/graphics/platform.h"
#include "../../../inc/slib/core/safe_static.h"

#pragma comment (lib, "gdiplus.lib")

SLIB_GRAPHICS_NAMESPACE_BEGIN

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

Size Font::getTextSize(const String& _text)
{
	Gdiplus::Font* handle = GraphicsPlatform::getGdiplusFont(this);
	if (!handle) {
		return Size::zero();
	}

	SLIB_SAFE_STATIC(_Win32_FontStatic, fs)
	if (SLIB_SAFE_STATIC_CHECK_FREED(fs)) {
		return Size::zero();
	}

	Size ret(0, 0);
	if (fs.graphics) {
		String16 text = _text;
		Gdiplus::StringFormat format(Gdiplus::StringFormatFlagsNoWrap | Gdiplus::StringFormatFlagsNoClip);
		Gdiplus::RectF bound;
		Gdiplus::PointF origin(0, 0);
		Gdiplus::Status result = fs.graphics->MeasureString((WCHAR*)(text.getData()), (INT)(text.getLength()), handle, origin, &format, &bound);
		if (result == Gdiplus::Ok) {
			ret.x = bound.Width;
			ret.y = bound.Height;
		}
	}
	return ret;

}

sl_bool Font::getFontMetrics(FontMetrics& _out)
{
	Gdiplus::Font* handle = GraphicsPlatform::getGdiplusFont(this);
	if (handle) {
		Gdiplus::FontFamily family;
		if (handle->GetFamily(&family) == Gdiplus::Ok) {
			int style = handle->GetStyle();
			sl_real ratio = (sl_real)(handle->GetSize()) / (sl_real)(family.GetEmHeight(style));
			_out.ascent = (sl_real)(family.GetCellAscent(style)) * ratio;
			_out.descent = (sl_real)(family.GetCellDescent(style)) * ratio;
			_out.leading = (sl_real)(family.GetLineSpacing(style)) * ratio - _out.ascent - _out.descent;
			return sl_true;
		}
	}
	return sl_false;
}

class _Win32_FontObject : public Referable
{
public:
	Gdiplus::Font* m_fontGdiplus;
	sl_bool m_flagCreatedGdiplus;

	HFONT m_fontGDI;
	sl_bool m_flagCreatedGDI;

	SpinLock m_lock;

public:
	_Win32_FontObject()
	{
		m_fontGdiplus = sl_null;
		m_flagCreatedGdiplus = sl_false;

		m_fontGDI = NULL;
		m_flagCreatedGDI = sl_false;
	}

	~_Win32_FontObject()
	{
		if (m_fontGdiplus) {
			delete m_fontGdiplus;
		}
		if (m_fontGDI) {
			::DeleteObject(m_fontGDI);
		}
	}
	
public:
	void _createGdiplus(const FontDesc& desc)
	{
		if (m_flagCreatedGdiplus) {
			return;
		}

		SpinLocker lock(&m_lock);

		if (m_flagCreatedGdiplus) {
			return;
		}

		m_flagCreatedGdiplus = sl_true;

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

	void _createGDI(const FontDesc& desc)
	{
		if (m_flagCreatedGDI) {
			return;
		}

		SpinLocker lock(&m_lock);

		if (m_flagCreatedGDI) {
			return;
		}

		m_flagCreatedGDI = sl_true;

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

};

class _Font : public Font
{
public:
	_Win32_FontObject* getPlatformObject()
	{
		if (m_platformObject.isNull()) {
			SpinLocker lock(&m_lock);
			if (m_platformObject.isNull()) {
				m_platformObject = new _Win32_FontObject;
			}
		}
		return (_Win32_FontObject*)(m_platformObject.get());;
	}

	Gdiplus::Font* getGdiplus()
	{
		_Win32_FontObject* po = getPlatformObject();
		if (po) {
			po->_createGdiplus(m_desc);
			return po->m_fontGdiplus;
		}
		return sl_null;
	}

	HFONT getGDI()
	{
		_Win32_FontObject* po = getPlatformObject();
		if (po) {
			po->_createGDI(m_desc);
			return po->m_fontGDI;
		}
		return sl_null;
	}

};

Gdiplus::Font* GraphicsPlatform::getGdiplusFont(Font* _font)
{
	if (_font) {
		_Font* font = (_Font*)_font;
		return font->getGdiplus();
	}
	return NULL;
}

HFONT GraphicsPlatform::getGdiFont(Font* _font)
{
	if (_font) {
		_Font* font = (_Font*)_font;
		return font->getGDI();
	}
	return NULL;
}

SLIB_GRAPHICS_NAMESPACE_END

#endif
