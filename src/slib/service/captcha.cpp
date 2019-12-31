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
		length = 4;
		width = 130;
		height = 50;
	}

	sl_bool Captcha::generate()
	{
		Ref<Bitmap> bitmap = Bitmap::create(width, height);
		if (bitmap.isNull()) {
			return sl_false;
		}
		bitmap->resetPixels(Color(GetRandom8(157,255), GetRandom8(157,255), GetRandom8(157,255)));
		
		Ref<Canvas> canvas = bitmap->getCanvas();
		if (canvas.isNull()) {
			return sl_false;
		}
		
		Ref<Font> font = this->font;
		if (font.isNull()) {
			font = Font::create("Arial", (sl_real)(height / 2), sl_true);
			if (font.isNull()) {
				return sl_false;
			}
		}
		
		// draw line
		{
			for (int i = 0; i < 6; i++) {
				Ref<Pen> pen = Pen::createSolidPen(1, Color(GetRandom8(0,156), GetRandom8(0,156), GetRandom8(0,156)));
				canvas->drawLine((sl_real)(GetRandom(0, width)), (sl_real)(GetRandom(0, height)), (sl_real)(GetRandom(0, width)), (sl_real)(GetRandom(0, height)), pen);
			}
		}
		// draw garbages
		{
			Ref<Font> fonts[6];
			for (int k = 0; k < 6; k++) {
				fonts[k] = Font::create(font->getFamilyName(), font->getSize() * (sl_real)k / 10);
			}
			for (int i = 0; i < 100; i++) {
				sl_int32 indexFont = GetRandom(0, 5);
				Ref<Font>& f = fonts[indexFont];
				Color c(GetRandom8(200,255), GetRandom8(200,255), GetRandom8(200,255));
				canvas->drawText("*", (sl_real)(GetRandom(0, width)), (sl_real)(GetRandom(0, height)), f, c);
			}
		}

		SLIB_SCOPED_BUFFER(sl_char8, 32, codes, length)
		// generate code
		{
			static const char codeset[] = "abcdefghkmnprstuvwxyzABCDEFGHKMNPRSTUVWXYZ23456789";
			for (sl_uint32 i = 0; i < length; i++) {
				sl_char8 code = codeset[GetRandom(0, sizeof(codeset) - 1)];
				sl_char8 s[2] = {code, 0};
				Color color(GetRandom8(0,156), GetRandom8(0,156), GetRandom8(0,156));
				CanvasStateScope scope(canvas);
				Size size = font->measureText(s);
				sl_real x = (sl_real)(width * (i + 0.5f) / length + GetRandom(1, 5) - size.x / 2);
				sl_real y = (sl_real)(height - font->getFontHeight()) / 2;
				canvas->rotate(x + size.x / 2, y + size.y / 2, (sl_real)(GetRandom(-30, 30) * SLIB_PI / 180));
				canvas->drawText(s, x, y, font, color);
				codes[i] = code;
			}
		}
		this->code = String::create(codes, length);
		
		canvas.setNull();
		
		image = bitmap->toImage();
		if (image.isNull()) {
			return sl_false;
		}
		
		return sl_true;
	}

}

