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

#include "slib/math/transform2d.h"
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
		
			static Ref<Image> GenerateCharImage(sl_char16 ch, const Ref<Font>& font)
			{
				StringParam str(&ch, 1);
				Sizei size = font->measureText(str);
				if (size.x < 1 || size.y < 1) {
					return sl_null;
				}
				Ref<Bitmap> bitmap = Bitmap::create(size.x, size.y);
				if (bitmap.isNull()) {
					return sl_null;
				}
				// draw
				{
					bitmap->resetPixels(Color::zero());
					Ref<Canvas> canvas = bitmap->getCanvas();
					if (canvas.isNull()) {
						return sl_null;
					}
					canvas->drawText(str, 0, 0, font, Color::Black);
				}
				Ref<Image> image = bitmap->toImage();
				if (image.isNotNull()) {
					Rectanglei bounds;
					if (image->getDrawnBounds(&bounds)) {
						return image->sub(bounds.left, bounds.top, bounds.getWidth(), bounds.getHeight());
					}
				}
				return sl_null;
			}
		
		}
	}

	using namespace priv::captcha;
	

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(Captcha)

	Captcha::Captcha()
	{
		m_length = 4;
		m_width = 100;
		m_height = 40;
		SLIB_STATIC_STRING16(codeset, "abcdefghkmnprstuvwxyzABCDEFGHKMNPRSTUVWXYZ23456789")
		m_codeset = codeset;
		m_interferingCount = 10;
		
		m_flagPrepared = sl_false;
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
		m_flagPrepared = sl_false;
	}

	String16 Captcha::getInterferingElements() const
	{
		return m_interferingElements;
	}

	void Captcha::setInterferingElements(const StringParam& elements)
	{
		m_interferingElements = elements.toString16();
		m_flagPrepared = sl_false;
	}

	sl_uint32 Captcha::getInterferingCount() const
	{
		return m_interferingCount;
	}

	void Captcha::setInterferingCount(sl_uint32 count)
	{
		m_interferingCount = count;
	}

	sl_uint32 Captcha::getWidth() const
	{
		return m_width;
	}

	void Captcha::setWidth(sl_uint32 width)
	{
		m_width = width;
	}

	sl_uint32 Captcha::getHeight() const
	{
		return m_height;
	}

	void Captcha::setHeight(sl_uint32 height)
	{
		m_height = height;
		m_flagPrepared = sl_false;
	}

	Ref<Font> Captcha::getFont() const
	{
		return m_font;
	}

	void Captcha::setFont(const Ref<Font>& font)
	{
		m_font = font;
		m_flagPrepared = sl_false;
	}

	Ref<Font> Captcha::getInterferingFont() const
	{
		return m_fontInterfering;
	}

	void Captcha::setInterferingFont(const Ref<Font>& font)
	{
		m_fontInterfering = font;
		m_flagPrepared = sl_false;
	}

	Color Captcha::getBackgroundColor() const
	{
		return m_backgroundColor;
	}

	void Captcha::setBackgroundColor(const Color& color)
	{
		m_backgroundColor = color;
	}

	sl_bool Captcha::prepare()
	{
		if (m_flagPrepared) {
			return sl_true;
		}
		MutexLocker locker(&m_lock);
		if (m_flagPrepared) {
			return sl_true;
		}
		
		m_imagesCodeset.setNull();
		m_imagesInterferingElements.setNull();
		
		sl_uint32 width = m_width;
		sl_uint32 height = m_height;
		if (!width) {
			return sl_false;
		}
		if (!height) {
			return sl_false;
		}
				
		if (m_interferingElements.isNotEmpty()) {
			Ref<Font> font = this->m_fontInterfering;
			if (font.isNull()) {
				font = Font::create("Arial", (sl_real)(height / 3), sl_true);
				if (font.isNull()) {
					return sl_false;
				}
			}
			sl_char16* elements = m_interferingElements.getData();
			sl_uint32 count = (sl_uint32)(m_interferingElements.getLength());
			for (sl_uint32 i = 0; i < count; i++) {
				Ref<Image> image = GenerateCharImage(elements[i], font);
				m_imagesInterferingElements.add_NoLock(Move(image));
			}
		}
		if (m_codeset.isNotEmpty()) {
			Ref<Font> font = this->m_font;
			if (font.isNull()) {
				font = Font::create("Arial", (sl_real)(height / 2), sl_true);
				if (font.isNull()) {
					return sl_false;
				}
			}
			sl_char16* codesets = m_codeset.getData();
			sl_uint32 count = (sl_uint32)(m_codeset.getLength());
			for (sl_uint32 i = 0; i < count; i++) {
				Ref<Image> image = GenerateCharImage(codesets[i], font);
				m_imagesCodeset.add_NoLock(Move(image));
			}
		}
		
		m_flagPrepared = sl_true;
		return sl_true;
	}

	Ref<Image> Captcha::generate(String& _code)
	{
		sl_uint32 width = m_width;
		sl_uint32 height = m_height;
		if (!width) {
			return sl_null;
		}
		if (!height) {
			return sl_null;
		}
		if (!(prepare())) {
			return sl_null;
		}
		Ref<Image> image = Image::create(width, height);
		if (image.isNull()) {
			return sl_null;
		}
		
		if (m_backgroundColor.isNotZero()) {
			image->resetPixels(m_backgroundColor);
		} else {
			image->resetPixels(Color(GetRandom8(157,255), GetRandom8(157,255), GetRandom8(157,255)));
		}
		
		// draw line
		{
			for (int i = 0; i < 6; i++) {
				image->drawSmoothLine(GetRandom(0, width), GetRandom(0, height), GetRandom(0, width), GetRandom(0, height), Color(GetRandom8(0,156), GetRandom8(0,156), GetRandom8(0,156)));
			}
		}
		// draw interfering elements
		if (m_imagesInterferingElements.isNotEmpty()) {
			ListElements< Ref<Image> > elements(m_imagesInterferingElements);
			for (int i = 0; i < m_interferingCount; i++) {
				sl_int32 index = GetRandom(0, (sl_int32)(elements.count - 1));
				Ref<Image>& element = elements[index];
				if (element.isNotNull()) {
					double f = (double)(GetRandom(5, 10)) / 10.0;
					Color color(GetRandom8(200,255), GetRandom8(200,255), GetRandom8(200,255), 0);
					sl_int32 sw = element->getWidth();
					sl_int32 sh = element->getHeight();
					image->drawImage(GetRandom(-sw, width), GetRandom(-sh, height), (sl_int32)(sw * f), (sl_int32)(sh * f), element, color, 0, 0, sw, sh, BlendMode::Over, StretchMode::Linear);
				}
			}
		}

		String16 code = String16::from(_code);
		// generate code
		if (code.isEmpty()) {
			sl_uint32 length = m_length;
			code = String16::allocate(length);
			if (code.isEmpty()) {
				return sl_null;
			}
			sl_char16* codes = code.getData();
			sl_uint32 lenCodeset = (sl_uint32)(m_codeset.getLength());
			if (!lenCodeset) {
				return sl_null;
			}
			sl_char16* codesets = m_codeset.getData();
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
				sl_reg index = m_codeset.indexOf(c);
				if (index >= 0) {
					Ref<Image> imageChar = m_imagesCodeset.getValueAt(index);
					if (imageChar.isNotNull()) {
						sl_real iw = (sl_real)(imageChar->getWidth());
						sl_real ih = (sl_real)(imageChar->getHeight());
						Color color(GetRandom8(0,156), GetRandom8(0,156), GetRandom8(0,156), 0);
						sl_real x = (sl_real)(width * (i + 0.5f) / length + GetRandom(1, 5) - iw / 2);
						sl_real y = (sl_real)(height - ih) / 2;
						Matrix3 mat;
						Transform2::setTranslation(mat, -iw/2, -ih/2);
						Transform2::rotate(mat, (sl_real)(GetRandom(-30, 30) * SLIB_PI / 180));
						Transform2::translate(mat, iw/2 + x, ih/2 + y);
						image->drawImage(imageChar, color, mat);
					}
				}
			}
		} else {
			return sl_null;
		}
		
		_code = String::from(code);
		return image;
	}

}

