/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#include "slib/service/captcha.h"

#include "slib/core/scoped.h"

namespace slib
{

	namespace priv
	{
		namespace captcha
		{
		
			static sl_int32 GetRandom(sl_int32 vmin, sl_int32 vmax)
			{
				if (vmin >= vmax) {
					return vmin;
				}
				sl_int32 c;
				Math::randomMemory(&c, 4);
				return ((c & 0x7FFFFFFF) % (vmax - vmin + 1)) + vmin;
			}
		
			static sl_uint8 GetRandom8(sl_uint8 vmin, sl_uint8 vmax)
			{
				return (sl_uint8)(GetRandom(vmin, vmax));
			}
		
		}
	}

	using namespace priv::captcha;
	

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(Captcha)

	Captcha::Captcha()
	{
		m_length = 4;
		m_width = 130;
		m_height = 50;
		SLIB_STATIC_STRING16(codeset, "abcdefghkmnprstuvwxyzABCDEFGHKMNPRSTUVWXYZ23456789")
		m_codeset = codeset;
		SLIB_STATIC_STRING16(interferingElements, "*")
		m_interferingElements = interferingElements;
	}

	String Captcha::getCode() const
	{
		return String::from(m_code);
	}
	
	String16 Captcha::getCode16() const
	{
		return m_code;
	}

	void Captcha::setCode(const StringParam& code)
	{
		m_codeInput = code.toString16();
		m_code = m_codeInput;
	}

	sl_uint32 Captcha::getCodeLength() const
	{
		return m_length;
	}

	void Captcha::setCodeLength(sl_uint32 length)
	{
		m_length = length;
	}

	String16 Captcha::getCodeElements() const
	{
		return m_codeset;
	}

	void Captcha::setCodeElements(const StringParam& codeset)
	{
		m_codeset = codeset.toString16();
	}

	String16 Captcha::getInterferingElements() const
	{
		return m_interferingElements;
	}

	void Captcha::setInterferingElements(const StringParam& elements)
	{
		m_interferingElements = elements.toString16();
	}

	sl_uint32 Captcha::getWidth() const
	{
		return m_width;
	}

	void Captcha::setWidth(sl_uint32 width)
	{
		m_width = width;
		m_bitmap.setNull();
		m_image.setNull();
	}

	sl_uint32 Captcha::getHeight() const
	{
		return m_height;
	}

	void Captcha::setHeight(sl_uint32 height)
	{
		m_height = height;
		m_bitmap.setNull();
		m_image.setNull();
	}

	Ref<Font> Captcha::getFont() const
	{
		return m_font;
	}

	void Captcha::setFont(const Ref<Font>& font)
	{
		m_font = font;
	}

	void Captcha::setBackgroundColor(const Color& background)
	{
		m_background = background;
	}

	Ref<Image> Captcha::generate(String* outCode)
	{
		MutexLocker locker(&m_lock);
		
		Ref<Bitmap> bitmap = m_bitmap;
		if (bitmap.isNull()) {
			bitmap = Bitmap::create(m_width, m_height);
			if (bitmap.isNull()) {
				return sl_null;
			}
			m_bitmap = bitmap;
		}
		if (m_background.isNotZero()) {
			bitmap->resetPixels(m_background);
		} else {
			bitmap->resetPixels(Color(GetRandom8(157,255), GetRandom8(157,255), GetRandom8(157,255)));
		}
		Ref<Canvas> canvas = bitmap->getCanvas();
		if (canvas.isNull()) {
			return sl_null;
		}

		sl_uint32 width = m_width;
		sl_uint32 height = m_height;
		Ref<Font> font = this->m_font;
		if (font.isNull()) {
			font = Font::create("Arial", (sl_real)(m_height / 2), sl_true);
			if (font.isNull()) {
				return sl_null;
			}
			m_font = font;
		}
		
		// draw line
		{
			for (int i = 0; i < 6; i++) {
				Ref<Pen> pen = Pen::createSolidPen(1, Color(GetRandom8(0,156), GetRandom8(0,156), GetRandom8(0,156)));
				canvas->drawLine((sl_real)(GetRandom(0, width)), (sl_real)(GetRandom(0, height)), (sl_real)(GetRandom(0, width)), (sl_real)(GetRandom(0, height)), pen);
			}
		}
		// draw interfering elements
		String16 interferingElements = m_interferingElements;
		if (interferingElements.isNotEmpty()) {
			sl_char16* elements = interferingElements.getData();
			sl_uint32 countElements = (sl_uint32)(interferingElements.getLength());
			Ref<Font> fonts[6];
			for (int k = 0; k < 6; k++) {
				fonts[k] = Font::create(font->getFamilyName(), font->getSize() * (sl_real)k / 10);
			}
			for (int i = 0; i < 100; i++) {
				sl_int32 indexFont = GetRandom(0, 5);
				sl_char16 c;
				if (countElements <= 1) {
					c = elements[0];
				} else {
					sl_uint32 index = GetRandom(0, countElements - 1);
					c = elements[index];
				}
				Ref<Font>& font = fonts[indexFont];
				Color color(GetRandom8(200,255), GetRandom8(200,255), GetRandom8(200,255));
				canvas->drawText(StringParam(&c, 1), (sl_real)(GetRandom(0, width)), (sl_real)(GetRandom(0, height)), font, color);
			}
		}

		String16 code = m_codeInput;
		// generate code
		if (code.isEmpty()) {
			sl_uint32 length = m_length;
			code = String16::allocate(length);
			if (code.isEmpty()) {
				return sl_null;
			}
			sl_char16* codes = code.getData();
			String16 codeset = m_codeset;
			sl_uint32 lenCodeset = (sl_uint32)(codeset.getLength());
			if (!lenCodeset) {
				return sl_null;
			}
			sl_char16* codesets = codeset.getData();
			for (sl_uint32 i = 0; i < length; i++) {
				sl_uint32 index = GetRandom(0, lenCodeset - 1);
				sl_char16 code = codesets[index];
				codes[i] = code;
			}
		}
		
		// draw code
		if (code.isNotEmpty()) {
			sl_uint32 length = (sl_uint32)(code.getLength());
			sl_char16* codes = code.getData();
			for (sl_uint32 i = 0; i < length; i++) {
				sl_char16 c = codes[i];
				StringParam s(&c, 1);
				Color color(GetRandom8(0,156), GetRandom8(0,156), GetRandom8(0,156));
				CanvasStateScope scope(canvas);
				Size size = font->measureText(s);
				sl_real x = (sl_real)(width * (i + 0.5f) / length + GetRandom(1, 5) - size.x / 2);
				sl_real y = (sl_real)(height - font->getFontHeight()) / 2;
				canvas->rotate(x + size.x / 2, y + size.y / 2, (sl_real)(GetRandom(-30, 30) * SLIB_PI / 180));
				canvas->drawText(s, x, y, font, color);
			}
		} else {
			return sl_null;
		}
		
		canvas.setNull();
		
		Ref<Image> image = m_image;
		if (image.isNull()) {
			image = bitmap->toImage();
			if (image.isNull()) {
				return sl_null;
			}
			m_image = image;
		} else {
			image->copyBitmap(bitmap, 0, 0, width, height);
		}
		m_code = code;
		if (outCode) {
			*outCode = String::from(code);
		}
		return image;
	}

}

