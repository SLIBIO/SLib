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

#include "slib/graphics/color.h"

#include "slib/core/math.h"
#include "slib/core/variant.h"
#include "slib/core/safe_static.h"

namespace slib
{

	SLIB_ALIGN(8) sl_uint8 Color::_zero[4] = {0, 0, 0, 0};

	void Color::setBlueF(float v) noexcept
	{
		sl_int32 n = (sl_int32)(v * 255.0f);
		b = (sl_uint8)(Math::clamp0_255(n));
	}

	void Color::setGreenF(float v) noexcept
	{
		sl_int32 n = (sl_int32)(v * 255.0f);
		g = (sl_uint8)(Math::clamp0_255(n));
	}

	void Color::setRedF(float v) noexcept
	{
		sl_int32 n = (sl_int32)(v * 255.0f);
		r = (sl_uint8)(Math::clamp0_255(n));
	}

	void Color::setAlphaF(float v) noexcept
	{
		sl_int32 n = (sl_int32)(v * 255.0f);
		a = (sl_uint8)(Math::clamp0_255(n));
	}

	void Color::blend_PA_NPA(sl_uint32 _r, sl_uint32 _g, sl_uint32 _b, sl_uint32 _a) noexcept
	{
		sl_uint32 _or = r;
		sl_uint32 _og = g;
		sl_uint32 _ob = b;
		sl_uint32 _oa = a;
		sl_uint32 sa = _a;
		_or = (_or * (255 - sa) + _r * sa) / 255;
		_og = (_og * (255 - sa) + _g * sa) / 255;
		_ob = (_ob * (255 - sa) + _b * sa) / 255;
		_oa = (_oa * (255 - sa) + 255 * sa) / 255;
		r = (sl_uint8)(_or);
		g = (sl_uint8)(_og);
		b = (sl_uint8)(_ob);
		a = (sl_uint8)(_oa);
	}

	void Color::blend_PA_NPA(const Color& src) noexcept
	{
		blend_PA_NPA(src.r, src.g, src.b, src.a);
	}

	void Color::blend_PA_PA(sl_uint32 _r, sl_uint32 _g, sl_uint32 _b, sl_uint32 _a) noexcept
	{
		sl_uint32 _or = r;
		sl_uint32 _og = g;
		sl_uint32 _ob = b;
		sl_uint32 oa = a;
		sl_uint32 sa = _a;
		_or = Math::clamp0_255(_or * (255 - sa) / 255 + _r);
		_og = Math::clamp0_255(_og * (255 - sa) / 255 + _g);
		_ob = Math::clamp0_255(_ob * (255 - sa) / 255 + _b);
		oa = (oa * (255 - sa) + 255 * sa) / 255;
		r = (sl_uint8)(_or);
		g = (sl_uint8)(_og);
		b = (sl_uint8)(_ob);
		a = (sl_uint8)(oa);
	}

	void Color::blend_PA_PA(const Color& src) noexcept
	{
		blend_PA_NPA(src.r, src.g, src.b, src.a);
	}

	void Color::blend_NPA_NPA(sl_uint32 _r, sl_uint32 _g, sl_uint32 _b, sl_uint32 _a) noexcept
	{
		sl_uint32 _or = r;
		sl_uint32 _og = g;
		sl_uint32 _ob = b;
		sl_uint32 oa = a;
		sl_uint32 sa = _a;
		sl_uint32 _oa = (oa * (255 - sa) + 255 * sa) + 1;
		_or = Math::clamp0_255((_or * oa * (255 - sa) + _r * (255 * sa + 1)) / _oa);
		_og = Math::clamp0_255((_og * oa * (255 - sa) + _g * (255 * sa + 1)) / _oa);
		_ob = Math::clamp0_255((_ob * oa * (255 - sa) + _b * (255 * sa + 1)) / _oa);
		_oa = (_oa - 1) / 255;
		r = (sl_uint8)(_or);
		g = (sl_uint8)(_og);
		b = (sl_uint8)(_ob);
		a = (sl_uint8)(_oa);
	}

	void Color::blend_NPA_NPA(const Color& src) noexcept
	{
		blend_NPA_NPA(src.r, src.g, src.b, src.a);
	}

	void Color::blend_NPA_PA(sl_uint32 _r, sl_uint32 _g, sl_uint32 _b, sl_uint32 _a) noexcept
	{
		sl_uint32 _or = r;
		sl_uint32 _og = g;
		sl_uint32 _ob = b;
		sl_uint32 oa = a;
		sl_uint32 sa = _a;
		sl_uint32 _oa = (oa * (255 - sa) + 255 * sa) + 1;
		_or = Math::clamp0_255((_or * oa * (255 - sa) + (_r << 16)) / _oa);
		_og = Math::clamp0_255((_og * oa * (255 - sa) + (_g << 16)) / _oa);
		_ob = Math::clamp0_255((_ob * oa * (255 - sa) + (_b << 16)) / _oa);
		_oa = (_oa - 1) / 255;
		r = (sl_uint8)(_or);
		g = (sl_uint8)(_og);
		b = (sl_uint8)(_ob);
		a = (sl_uint8)(_oa);
	}

	void Color::blend_NPA_PA(const Color& src) noexcept
	{
		blend_NPA_PA(src.r, src.g, src.b, src.a);
	}

	void Color::convertNPAtoPA() noexcept
	{
		sl_uint32 _or = r;
		sl_uint32 _og = g;
		sl_uint32 _ob = b;
		sl_uint32 oa = a;
		oa++;
		_or = (_or * oa) >> 8;
		_og = (_og * oa) >> 8;
		_ob = (_ob * oa) >> 8;
		r = (sl_uint8)(_or & 255);
		g = (sl_uint8)(_og & 255);
		b = (sl_uint8)(_ob & 255);
	}

	void Color::convertPAtoNPA() noexcept
	{
		sl_uint32 _or = r;
		sl_uint32 _og = g;
		sl_uint32 _ob = b;
		sl_uint32 oa = a;
		oa++;
		_or = Math::clamp0_255((_or << 8) / oa);
		_og = Math::clamp0_255((_og << 8) / oa);
		_ob = Math::clamp0_255((_ob << 8) / oa);
		r = (sl_uint8)(_or);
		g = (sl_uint8)(_og);
		b = (sl_uint8)(_ob);
	}

	void Color::multiplyAlpha(float f) noexcept
	{
		a = (sl_uint8)(Math::clamp0_255((sl_int32)(a * f)));
	}

	void Color::multiplyRed(float f) noexcept
	{
		r = (sl_uint8)(Math::clamp0_255((sl_int32)(r * f)));
	}

	void Color::multiplyGreen(float f) noexcept
	{
		g = (sl_uint8)(Math::clamp0_255((sl_int32)(g * f)));
	}

	void Color::multiplyBlue(float f) noexcept
	{
		b = (sl_uint8)(Math::clamp0_255((sl_int32)(b * f)));
	}

	String Color::toString() const noexcept
	{
		return String::format("#%02x%02x%02x%02x", a, r, g, b);
	}

	namespace priv
	{
		namespace color
		{

			/*
			
			Color names supported by all web-browsers
			
			http://www.w3schools.com/colors/colors_names.asp
			
			*/
		
#define PRIV_MAP_COLOR(NAME) \
			{ \
				SLIB_STATIC_STRING(_s, #NAME); \
				mapName.put(_s.toLower(), (sl_uint32)(Color::NAME)); \
			}

			class NameMap
			{
			public:
				HashTable<String, Color> mapName;

				
				NameMap() noexcept
				{
					PRIV_MAP_COLOR(AliceBlue)
					PRIV_MAP_COLOR(AntiqueWhite)
					PRIV_MAP_COLOR(Aqua)
					PRIV_MAP_COLOR(Aquamarine)
					PRIV_MAP_COLOR(Azure)
					PRIV_MAP_COLOR(Beige)
					PRIV_MAP_COLOR(Bisque)
					PRIV_MAP_COLOR(Black)
					PRIV_MAP_COLOR(BlanchedAlmond)
					PRIV_MAP_COLOR(Blue)
					PRIV_MAP_COLOR(BlueViolet)
					PRIV_MAP_COLOR(Brown)
					PRIV_MAP_COLOR(BurlyWood)
					PRIV_MAP_COLOR(CadetBlue)
					PRIV_MAP_COLOR(Chartreuse)
					PRIV_MAP_COLOR(Chocolate)
					PRIV_MAP_COLOR(Coral)
					PRIV_MAP_COLOR(CornflowerBlue)
					PRIV_MAP_COLOR(Cornsilk)
					PRIV_MAP_COLOR(Crimson)
					PRIV_MAP_COLOR(Cyan)
					PRIV_MAP_COLOR(DarkBlue)
					PRIV_MAP_COLOR(DarkCyan)
					PRIV_MAP_COLOR(DarkGoldenrod)
					PRIV_MAP_COLOR(DarkGray)
					PRIV_MAP_COLOR(DarkGreen)
					PRIV_MAP_COLOR(DarkKhaki)
					PRIV_MAP_COLOR(DarkMagenta)
					PRIV_MAP_COLOR(DarkOliveGreen)
					PRIV_MAP_COLOR(DarkOrange)
					PRIV_MAP_COLOR(DarkOrchid)
					PRIV_MAP_COLOR(DarkRed)
					PRIV_MAP_COLOR(DarkSalmon)
					PRIV_MAP_COLOR(DarkSeaGreen)
					PRIV_MAP_COLOR(DarkSlateBlue)
					PRIV_MAP_COLOR(DarkSlateGray)
					PRIV_MAP_COLOR(DarkTurquoise)
					PRIV_MAP_COLOR(DarkViolet)
					PRIV_MAP_COLOR(DeepPink)
					PRIV_MAP_COLOR(DeepSkyBlue)
					PRIV_MAP_COLOR(DimGray)
					PRIV_MAP_COLOR(DodgerBlue)
					PRIV_MAP_COLOR(Firebrick)
					PRIV_MAP_COLOR(FloralWhite)
					PRIV_MAP_COLOR(ForestGreen)
					PRIV_MAP_COLOR(Fuchsia)
					PRIV_MAP_COLOR(Gainsboro)
					PRIV_MAP_COLOR(GhostWhite)
					PRIV_MAP_COLOR(Gold)
					PRIV_MAP_COLOR(Goldenrod)
					PRIV_MAP_COLOR(Gray)
					PRIV_MAP_COLOR(Green)
					PRIV_MAP_COLOR(GreenYellow)
					PRIV_MAP_COLOR(Honeydew)
					PRIV_MAP_COLOR(HotPink)
					PRIV_MAP_COLOR(IndianRed)
					PRIV_MAP_COLOR(Indigo)
					PRIV_MAP_COLOR(Ivory)
					PRIV_MAP_COLOR(Khaki)
					PRIV_MAP_COLOR(Lavender)
					PRIV_MAP_COLOR(LavenderBlush)
					PRIV_MAP_COLOR(LawnGreen)
					PRIV_MAP_COLOR(LemonChiffon)
					PRIV_MAP_COLOR(LightBlue)
					PRIV_MAP_COLOR(LightCoral)
					PRIV_MAP_COLOR(LightCyan)
					PRIV_MAP_COLOR(LightGoldenrodYellow)
					PRIV_MAP_COLOR(LightGray)
					PRIV_MAP_COLOR(LightGreen)
					PRIV_MAP_COLOR(LightPink)
					PRIV_MAP_COLOR(LightSalmon)
					PRIV_MAP_COLOR(LightSeaGreen)
					PRIV_MAP_COLOR(LightSkyBlue)
					PRIV_MAP_COLOR(LightSlateGray)
					PRIV_MAP_COLOR(LightSteelBlue)
					PRIV_MAP_COLOR(LightYellow)
					PRIV_MAP_COLOR(Lime)
					PRIV_MAP_COLOR(LimeGreen)
					PRIV_MAP_COLOR(Linen)
					PRIV_MAP_COLOR(Magenta)
					PRIV_MAP_COLOR(Maroon)
					PRIV_MAP_COLOR(MediumAquamarine)
					PRIV_MAP_COLOR(MediumBlue)
					PRIV_MAP_COLOR(MediumOrchid)
					PRIV_MAP_COLOR(MediumPurple)
					PRIV_MAP_COLOR(MediumSeaGreen)
					PRIV_MAP_COLOR(MediumSlateBlue)
					PRIV_MAP_COLOR(MediumSpringGreen)
					PRIV_MAP_COLOR(MediumTurquoise)
					PRIV_MAP_COLOR(MediumVioletRed)
					PRIV_MAP_COLOR(MidnightBlue)
					PRIV_MAP_COLOR(MintCream)
					PRIV_MAP_COLOR(MistyRose)
					PRIV_MAP_COLOR(Moccasin)
					PRIV_MAP_COLOR(NavajoWhite)
					PRIV_MAP_COLOR(Navy)
					PRIV_MAP_COLOR(OldLace)
					PRIV_MAP_COLOR(Olive)
					PRIV_MAP_COLOR(OliveDrab)
					PRIV_MAP_COLOR(Orange)
					PRIV_MAP_COLOR(OrangeRed)
					PRIV_MAP_COLOR(Orchid)
					PRIV_MAP_COLOR(PaleGoldenrod)
					PRIV_MAP_COLOR(PaleGreen)
					PRIV_MAP_COLOR(PaleTurquoise)
					PRIV_MAP_COLOR(PaleVioletRed)
					PRIV_MAP_COLOR(PapayaWhip)
					PRIV_MAP_COLOR(PeachPuff)
					PRIV_MAP_COLOR(Peru)
					PRIV_MAP_COLOR(Pink)
					PRIV_MAP_COLOR(Plum)
					PRIV_MAP_COLOR(PowderBlue)
					PRIV_MAP_COLOR(Purple)
					PRIV_MAP_COLOR(Red)
					PRIV_MAP_COLOR(RosyBrown)
					PRIV_MAP_COLOR(RoyalBlue)
					PRIV_MAP_COLOR(SaddleBrown)
					PRIV_MAP_COLOR(Salmon)
					PRIV_MAP_COLOR(SandyBrown)
					PRIV_MAP_COLOR(SeaGreen)
					PRIV_MAP_COLOR(SeaShell)
					PRIV_MAP_COLOR(Sienna)
					PRIV_MAP_COLOR(Silver)
					PRIV_MAP_COLOR(SkyBlue)
					PRIV_MAP_COLOR(SlateBlue)
					PRIV_MAP_COLOR(SlateGray)
					PRIV_MAP_COLOR(Snow)
					PRIV_MAP_COLOR(SpringGreen)
					PRIV_MAP_COLOR(SteelBlue)
					PRIV_MAP_COLOR(Tan)
					PRIV_MAP_COLOR(Teal)
					PRIV_MAP_COLOR(Thistle)
					PRIV_MAP_COLOR(Tomato)
					PRIV_MAP_COLOR(Transparent)
					PRIV_MAP_COLOR(Turquoise)
					PRIV_MAP_COLOR(Violet)
					PRIV_MAP_COLOR(Wheat)
					PRIV_MAP_COLOR(White)
					PRIV_MAP_COLOR(WhiteSmoke)
					PRIV_MAP_COLOR(Yellow)
					PRIV_MAP_COLOR(YellowGreen)
					PRIV_MAP_COLOR(Zero)
					PRIV_MAP_COLOR(None)
				}
				
				sl_bool getColorFromName(const String& nameLower, Color& color) noexcept
				{
					return mapName.get(nameLower, &color);
				}
				
			};

			SLIB_SAFE_STATIC_GETTER(NameMap, GetNameMap)

			template <class CT>
			SLIB_INLINE static sl_reg Parse(Color* _out, const CT* sz, sl_size pos, sl_size len) noexcept
			{
				if (pos >= len) {
					return SLIB_PARSE_ERROR;
				}
				
				sl_size start;
				
				if (sz[pos] == '#') {
					
					pos++;
					
					start = pos;
					
					for (; pos < len; pos++) {
						if (!(SLIB_CHAR_IS_HEX(sz[pos]))) {
							break;
						}
					}
					
					sl_size n = pos - start;
					
					if (n == 6 || n == 8) {
						if (_out) {
							sl_uint32 r, g, b, a;
							if (n == 6) {
								a = 255;
								r = (SLIB_CHAR_HEX_TO_INT(sz[start]) << 4) | SLIB_CHAR_HEX_TO_INT(sz[start + 1]);
								g = (SLIB_CHAR_HEX_TO_INT(sz[start + 2]) << 4) | SLIB_CHAR_HEX_TO_INT(sz[start + 3]);
								b = (SLIB_CHAR_HEX_TO_INT(sz[start + 4]) << 4) | SLIB_CHAR_HEX_TO_INT(sz[start + 5]);
							} else {
								a = (SLIB_CHAR_HEX_TO_INT(sz[start]) << 4) | SLIB_CHAR_HEX_TO_INT(sz[start + 1]);
								r = (SLIB_CHAR_HEX_TO_INT(sz[start + 2]) << 4) | SLIB_CHAR_HEX_TO_INT(sz[start + 3]);
								g = (SLIB_CHAR_HEX_TO_INT(sz[start + 4]) << 4) | SLIB_CHAR_HEX_TO_INT(sz[start + 5]);
								b = (SLIB_CHAR_HEX_TO_INT(sz[start + 6]) << 4) | SLIB_CHAR_HEX_TO_INT(sz[start + 7]);
							}
							*_out = Color(r, g, b, a);
						}
						return pos;
					}
					
				} else {
					
					start = pos;
					
					for (; pos < len; pos++) {
						if (!(SLIB_CHAR_IS_ALPHA(sz[pos]))) {
							break;
						}
					}
					
					sl_size n = pos - start;
					
					if (n > 0) {
						
						if ((n == 3 || n ==4) && (sz[start] == 'r' || sz[start] == 'R') && (sz[start + 1] == 'g' || sz[start + 1] == 'G') && (sz[start + 2] == 'b' || sz[start + 2] == 'B')) {
							
							if (n == 4) {
								if (sz[start + 3] != 'a' && sz[start + 3] != 'A') {
									return SLIB_PARSE_ERROR;
								}
							}
							
							for (; pos < len; pos++) {
								if (!(SLIB_CHAR_IS_SPACE_TAB(sz[pos]))) {
									break;
								}
							}
							if (pos >= len) {
								return SLIB_PARSE_ERROR;
							}
							
							if (sz[pos] != '(') {
								return SLIB_PARSE_ERROR;
							}
							pos++;
							
							sl_reg iRet;
							sl_uint32 comp[3];
							sl_real a = 1;
							
							for (sl_size i = 0; i < n; i++) {
								
								for (; pos < len; pos++) {
									if (!(SLIB_CHAR_IS_SPACE_TAB(sz[pos]))) {
										break;
									}
								}
								if (pos >= len) {
									return SLIB_PARSE_ERROR;
								}
								if (i == 3) {
									iRet = StringTypeFromCharType<CT>::Type::parseFloat(&a, sz, pos, len);
									if (iRet == SLIB_PARSE_ERROR) {
										return SLIB_PARSE_ERROR;
									}
									if (a > 1) {
										return SLIB_PARSE_ERROR;
									}
									if (a < 0) {
										return SLIB_PARSE_ERROR;
									}
								} else {
									iRet = StringTypeFromCharType<CT>::Type::parseUint32(10, comp + i, sz, pos, len);
									if (iRet == SLIB_PARSE_ERROR) {
										return SLIB_PARSE_ERROR;
									}
									if (comp[i] > 255) {
										return SLIB_PARSE_ERROR;
									}
								}
								pos = iRet;
								for (; pos < len; pos++) {
									if (!(SLIB_CHAR_IS_SPACE_TAB(sz[pos]))) {
										break;
									}
								}
								if (pos >= len) {
									return SLIB_PARSE_ERROR;
								}
								if (i < n - 1) {
									if (sz[pos] != ',') {
										return SLIB_PARSE_ERROR;
									}
								} else {
									if (sz[pos] != ')') {
										return SLIB_PARSE_ERROR;
									}
								}
								pos++;
							}
							
							if (_out) {
								if (n == 4) {
									*_out = Color(comp[0], comp[1], comp[2], (sl_uint32)(a * 255));
								} else {
									*_out = Color(comp[0], comp[1], comp[2]);
								}
							}
							
							return pos;
							
						} else if (n < 64) {
							
							sl_char8 s[64];
							for (sl_size i = 0; i < n; i++) {
								sl_char8 ch = (sl_char8)(sz[start + i]);
								s[i] = SLIB_CHAR_UPPER_TO_LOWER(ch);
							}
							
							NameMap* nm = GetNameMap();
							if (nm) {
								Color color;
								if (nm->getColorFromName(String(s, n), color)) {
									if (_out) {
										*_out = color;
									}
									return pos;
								}
							}
							
						}
					}
				}
				return SLIB_PARSE_ERROR;
			}

		}
	}

	template <>
	sl_reg Parser<Color, sl_char8>::parse(Color* _out, const sl_char8 *sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return priv::color::Parse(_out, sz, posBegin, posEnd);
	}

	template <>
	sl_reg Parser<Color, sl_char16>::parse(Color* _out, const sl_char16 *sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return priv::color::Parse(_out, sz, posBegin, posEnd);
	}


	Color4f ColorMatrix::transformColor(const Color4f& src) const noexcept
	{
		Color4f ret;
		ret.x = src.dot(red);
		ret.y = src.dot(green);
		ret.z = src.dot(blue);
		ret.w = src.dot(alpha);
		ret += bias;
		return ret;
	}

	Color4f ColorMatrix::transformColor(const Color& src) const noexcept
	{
		return transformColor((Color4f)src);
	}

	Color4f ColorMatrix::transformColor(const Color3f& src) const noexcept
	{
		return transformColor(Color4f(src.x, src.y, src.z, 1));
	}
	
	void ColorMatrix::setOverlay(const Color4f& c) noexcept
	{
		red = Color4f::zero();
		green = Color4f::zero();
		blue = Color4f::zero();
		alpha = Color4f(0, 0, 0, c.w);
		bias = Color4f(c.x, c.y, c.z, 0);
	}

	void ColorMatrix::setOverlay(const Color& c) noexcept
	{
		setOverlay((Color4f)c);
	}
	
	void ColorMatrix::setOverlay(const Color3f& c) noexcept
	{
		setOverlay(Color4f(c.x, c.y, c.z, 1));
	}
	
}
