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

#if defined(SLIB_GRAPHICS_IS_GDI)

#include "slib/graphics/font.h"

#include "slib/graphics/platform.h"
#include "slib/core/safe_static.h"

namespace slib
{

	namespace priv
	{
		namespace gdi
		{

			class FontStaticContext
			{
			public:
				Gdiplus::Bitmap* image;
				Gdiplus::Graphics* graphics;

			public:
				FontStaticContext()
				{
					graphics = sl_null;

					GraphicsPlatform::startGdiplus();
					image = new Gdiplus::Bitmap(1, 1, PixelFormat24bppRGB);
					if (image) {
						graphics = new Gdiplus::Graphics(image);
					}
				}

				~FontStaticContext()
				{
					delete graphics;
					delete image;
				}
			};

			SLIB_SAFE_STATIC_GETTER(FontStaticContext, GetFontStaticContext)

			class FontPlatformObject : public Referable
			{
			public:
				Gdiplus::Font* m_fontGdiplus;
				sl_bool m_flagCreatedGdiplus;

				HFONT m_fontGDI;
				sl_bool m_flagCreatedGDI;

				SpinLock m_lock;

			public:
				FontPlatformObject()
				{
					m_fontGdiplus = sl_null;
					m_flagCreatedGdiplus = sl_false;

					m_fontGDI = NULL;
					m_flagCreatedGDI = sl_false;
				}

				~FontPlatformObject()
				{
					delete m_fontGdiplus;
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
					if (desc.flagStrikeout) {
						style |= Gdiplus::FontStyleStrikeout;
					}
					String16 fontName = String16::from(desc.familyName);

					GraphicsPlatform::startGdiplus();
					Gdiplus::Font* font = new Gdiplus::Font(
						(LPCWSTR)(fontName.getData()),
						desc.size,
						style,
						Gdiplus::UnitPixel);
					
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
					
					int height = -(int)(desc.size);
					int weight;
					if (desc.flagBold) {
						weight = 700;
					} else {
						weight = 400;
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
					DWORD bStrikeout;
					if (desc.flagStrikeout) {
						bStrikeout = TRUE;
					} else {
						bStrikeout = FALSE;
					}
					String16 fontName = String16::from(desc.familyName);
					HFONT hFont = CreateFontW(height, 0, 0, 0, weight, bItalic, bUnderline, bStrikeout,
						DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
						ANTIALIASED_QUALITY,
						DEFAULT_PITCH,
						(LPCWSTR)(fontName.getData()));

					m_fontGDI = hFont;

				}

			};

			class FontHelper : public Font
			{
			public:
				FontPlatformObject* getPlatformObject()
				{
					if (m_platformObject.isNull()) {
						SpinLocker lock(&m_lock);
						if (m_platformObject.isNull()) {
							m_platformObject = new FontPlatformObject;
						}
					}
					return (FontPlatformObject*)(m_platformObject.get());;
				}

				Gdiplus::Font* getGdiplus()
				{
					FontPlatformObject* po = getPlatformObject();
					if (po) {
						po->_createGdiplus(m_desc);
						return po->m_fontGdiplus;
					}
					return sl_null;
				}

				HFONT getGDI()
				{
					FontPlatformObject* po = getPlatformObject();
					if (po) {
						po->_createGDI(m_desc);
						return po->m_fontGDI;
					}
					return sl_null;
				}

			};

		}
	}

	using namespace priv::gdi;

	sl_bool Font::_getFontMetrics_PO(FontMetrics& _out)
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

	Size Font::_measureText_PO(const StringParam& _text)
	{
		Gdiplus::Font* handle = GraphicsPlatform::getGdiplusFont(this);
		if (!handle) {
			return Size::zero();
		}

		FontStaticContext* fs = GetFontStaticContext();
		if (!fs) {
			return Size::zero();
		}

		Size ret(0, 0);
		if (fs->graphics) {
			StringData16 text(_text);
			Gdiplus::StringFormat format(Gdiplus::StringFormatFlagsNoWrap | Gdiplus::StringFormatFlagsNoClip | Gdiplus::StringFormatFlagsNoFitBlackBox);
			Gdiplus::RectF bound;
			Gdiplus::PointF origin(0, 0);
			Gdiplus::Status result = fs->graphics->MeasureString((WCHAR*)(text.getData()), (INT)(text.getLength()), handle, origin, Gdiplus::StringFormat::GenericTypographic(), &bound);
			if (result == Gdiplus::Ok) {
				ret.x = bound.Width;
				ret.y = bound.Height;
			}
		}
		return ret;

	}

	Gdiplus::Font* GraphicsPlatform::getGdiplusFont(Font* _font)
	{
		if (_font) {
			FontHelper* font = (FontHelper*)_font;
			return font->getGdiplus();
		}
		return NULL;
	}

	HFONT GraphicsPlatform::getGdiFont(Font* _font)
	{
		if (_font) {
			FontHelper* font = (FontHelper*)_font;
			return font->getGDI();
		}
		return NULL;
	}

}

#endif
