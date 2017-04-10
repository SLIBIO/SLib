/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "slib/graphics/font.h"

#include "slib/graphics/platform.h"
#include "slib/core/safe_static.h"

#pragma comment (lib, "gdiplus.lib")

namespace slib
{

	class Win32_FontStatic
	{
	public:
		Gdiplus::Bitmap* image;
		Gdiplus::Graphics* graphics;

	public:
		Win32_FontStatic()
		{
			graphics = sl_null;
			image = new Gdiplus::Bitmap(1, 1, PixelFormat24bppRGB);
			if (image) {
				graphics = new Gdiplus::Graphics(image);
			}
		}

		~Win32_FontStatic()
		{
			delete graphics;
			delete image;
		}
	};

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

	Size Font::_measureText_PO(const String& _text)
	{
		Gdiplus::Font* handle = GraphicsPlatform::getGdiplusFont(this);
		if (!handle) {
			return Size::zero();
		}

		SLIB_SAFE_STATIC(Win32_FontStatic, fs)
		if (SLIB_SAFE_STATIC_CHECK_FREED(fs)) {
			return Size::zero();
		}

		Size ret(0, 0);
		if (fs.graphics) {
			String16 text = _text;
			Gdiplus::StringFormat format(Gdiplus::StringFormatFlagsNoWrap | Gdiplus::StringFormatFlagsNoClip | Gdiplus::StringFormatFlagsNoFitBlackBox);
			Gdiplus::RectF bound;
			Gdiplus::PointF origin(0, 0);
			Gdiplus::Status result = fs.graphics->MeasureString((WCHAR*)(text.getData()), (INT)(text.getLength()), handle, origin, Gdiplus::StringFormat::GenericTypographic(), &bound);
			if (result == Gdiplus::Ok) {
				ret.x = bound.Width;
				ret.y = bound.Height;
			}
		}
		return ret;

	}

	class Win32_FontObject : public Referable
	{
	public:
		Gdiplus::Font* m_fontGdiplus;
		sl_bool m_flagCreatedGdiplus;

		HFONT m_fontGDI;
		sl_bool m_flagCreatedGDI;

		SpinLock m_lock;

	public:
		Win32_FontObject()
		{
			m_fontGdiplus = sl_null;
			m_flagCreatedGdiplus = sl_false;

			m_fontGDI = NULL;
			m_flagCreatedGDI = sl_false;
		}

		~Win32_FontObject()
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
			String16 fontName = desc.familyName;
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
			DWORD bStrikeout;
			if (desc.flagStrikeout) {
				bStrikeout = TRUE;
			} else {
				bStrikeout = FALSE;
			}
			String16 fontName = desc.familyName;
			HFONT hFont = ::CreateFontW(height, 0, 0, 0, weight, bItalic, bUnderline, bStrikeout,
				DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				ANTIALIASED_QUALITY,
				DEFAULT_PITCH,
				(LPCWSTR)(fontName.getData()));

			m_fontGDI = hFont;

		}

	};

	class Font_Ext : public Font
	{
	public:
		Win32_FontObject* getPlatformObject()
		{
			if (m_platformObject.isNull()) {
				SpinLocker lock(&m_lock);
				if (m_platformObject.isNull()) {
					m_platformObject = new Win32_FontObject;
				}
			}
			return (Win32_FontObject*)(m_platformObject.get());;
		}

		Gdiplus::Font* getGdiplus()
		{
			Win32_FontObject* po = getPlatformObject();
			if (po) {
				po->_createGdiplus(m_desc);
				return po->m_fontGdiplus;
			}
			return sl_null;
		}

		HFONT getGDI()
		{
			Win32_FontObject* po = getPlatformObject();
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
			Font_Ext* font = (Font_Ext*)_font;
			return font->getGdiplus();
		}
		return NULL;
	}

	HFONT GraphicsPlatform::getGdiFont(Font* _font)
	{
		if (_font) {
			Font_Ext* font = (Font_Ext*)_font;
			return font->getGDI();
		}
		return NULL;
	}

}

#endif
