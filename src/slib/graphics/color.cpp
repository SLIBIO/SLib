#include "../../../inc/slib/graphics/color.h"

#include "../../../inc/slib/core/math.h"
#include "../../../inc/slib/core/variant.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

sl_uint8 Color::_zero[4] = {0, 0, 0, 0};

Color::Color(const Color3f& v)
{
	sl_int32 _r = Math::clamp0_255((sl_int32)(v.x * 255));
	sl_int32 _g = Math::clamp0_255((sl_int32)(v.y * 255));
	sl_int32 _b = Math::clamp0_255((sl_int32)(v.z * 255));
	r = (sl_uint8)(_r);
	g = (sl_uint8)(_g);
	b = (sl_uint8)(_b);
}

Color::Color(const Color4f& v)
{
	sl_int32 _r = Math::clamp0_255((sl_int32)(v.x * 255));
	sl_int32 _g = Math::clamp0_255((sl_int32)(v.y * 255));
	sl_int32 _b = Math::clamp0_255((sl_int32)(v.z * 255));
	sl_int32 _a = Math::clamp0_255((sl_int32)(v.w * 255));
	a = (sl_uint8)(_a);
	r = (sl_uint8)(_r);
	g = (sl_uint8)(_g);
	b = (sl_uint8)(_b);
}

float Color::getBlueF() const
{
	return (float)(b) / 255.0f;
}

void Color::setBlueF(float v)
{
	sl_int32 n = (sl_int32)(v * 255.0f);
	b = (sl_uint8)(Math::clamp0_255(n));
}

float Color::getGreenF() const
{
	return (float)(g) / 255.0f;
}

void Color::setGreenF(float v)
{
	sl_int32 n = (sl_int32)(v * 255.0f);
	g = (sl_uint8)(Math::clamp0_255(n));
}

float Color::getRedF() const
{
	return (float)(r) / 255.0f;
}

void Color::setRedF(float v)
{
	sl_int32 n = (sl_int32)(v * 255.0f);
	r = (sl_uint8)(Math::clamp0_255(n));
}

float Color::getAlphaF() const
{
	return (float)(a) / 255.0f;
}

void Color::setAlphaF(float v)
{
	sl_int32 n = (sl_int32)(v * 255.0f);
	a = (sl_uint8)(Math::clamp0_255(n));
}

void Color::blend_PA_NPA(sl_uint32 _r, sl_uint32 _g, sl_uint32 _b, sl_uint32 _a)
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

void Color::blend_PA_NPA(const Color& src)
{
	blend_PA_NPA(src.r, src.g, src.b, src.a);
}

void Color::blend_PA_PA(sl_uint32 _r, sl_uint32 _g, sl_uint32 _b, sl_uint32 _a)
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

void Color::blend_PA_PA(const Color& src)
{
	blend_PA_NPA(src.r, src.g, src.b, src.a);
}

void Color::blend_NPA_NPA(sl_uint32 _r, sl_uint32 _g, sl_uint32 _b, sl_uint32 _a)
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

void Color::blend_NPA_NPA(const Color& src)
{
	blend_NPA_NPA(src.r, src.g, src.b, src.a);
}

void Color::blend_NPA_PA(sl_uint32 _r, sl_uint32 _g, sl_uint32 _b, sl_uint32 _a)
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

void Color::blend_NPA_PA(const Color& src)
{
	blend_NPA_PA(src.r, src.g, src.b, src.a);
}

void Color::convertNPAtoPA()
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

void Color::convertPAtoNPA()
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


String Color::toString() const
{
	return String::format("#%02x%02x%02x%02x", a, r, g, b);
}

/*
 
 Color names supported by all web-browsers
 
 http://www.w3schools.com/colors/colors_names.asp
 
*/

class _Color_Name_Map
{
public:
	HashTable<String8, Color> mapName;
	
#define _MAP_COLOR(NAME) \
	{ \
		SLIB_STATIC_STRING(_s, #NAME); \
		mapName.put(_s.toLower(), (sl_uint32)(Color::NAME)); \
	}
	
	_Color_Name_Map()
	{
		_MAP_COLOR(AliceBlue)
		_MAP_COLOR(AntiqueWhite)
		_MAP_COLOR(Aqua)
		_MAP_COLOR(Aquamarine)
		_MAP_COLOR(Azure)
		_MAP_COLOR(Beige)
		_MAP_COLOR(Bisque)
		_MAP_COLOR(Black)
		_MAP_COLOR(BlanchedAlmond)
		_MAP_COLOR(Blue)
		_MAP_COLOR(BlueViolet)
		_MAP_COLOR(Brown)
		_MAP_COLOR(BurlyWood)
		_MAP_COLOR(CadetBlue)
		_MAP_COLOR(Chartreuse)
		_MAP_COLOR(Chocolate)
		_MAP_COLOR(Coral)
		_MAP_COLOR(CornflowerBlue)
		_MAP_COLOR(Cornsilk)
		_MAP_COLOR(Crimson)
		_MAP_COLOR(Cyan)
		_MAP_COLOR(DarkBlue)
		_MAP_COLOR(DarkCyan)
		_MAP_COLOR(DarkGoldenrod)
		_MAP_COLOR(DarkGray)
		_MAP_COLOR(DarkGreen)
		_MAP_COLOR(DarkKhaki)
		_MAP_COLOR(DarkMagenta)
		_MAP_COLOR(DarkOliveGreen)
		_MAP_COLOR(DarkOrange)
		_MAP_COLOR(DarkOrchid)
		_MAP_COLOR(DarkRed)
		_MAP_COLOR(DarkSalmon)
		_MAP_COLOR(DarkSeaGreen)
		_MAP_COLOR(DarkSlateBlue)
		_MAP_COLOR(DarkSlateGray)
		_MAP_COLOR(DarkTurquoise)
		_MAP_COLOR(DarkViolet)
		_MAP_COLOR(DeepPink)
		_MAP_COLOR(DeepSkyBlue)
		_MAP_COLOR(DimGray)
		_MAP_COLOR(DodgerBlue)
		_MAP_COLOR(Firebrick)
		_MAP_COLOR(FloralWhite)
		_MAP_COLOR(ForestGreen)
		_MAP_COLOR(Fuchsia)
		_MAP_COLOR(Gainsboro)
		_MAP_COLOR(GhostWhite)
		_MAP_COLOR(Gold)
		_MAP_COLOR(Goldenrod)
		_MAP_COLOR(Gray)
		_MAP_COLOR(Green)
		_MAP_COLOR(GreenYellow)
		_MAP_COLOR(Honeydew)
		_MAP_COLOR(HotPink)
		_MAP_COLOR(IndianRed)
		_MAP_COLOR(Indigo)
		_MAP_COLOR(Ivory)
		_MAP_COLOR(Khaki)
		_MAP_COLOR(Lavender)
		_MAP_COLOR(LavenderBlush)
		_MAP_COLOR(LawnGreen)
		_MAP_COLOR(LemonChiffon)
		_MAP_COLOR(LightBlue)
		_MAP_COLOR(LightCoral)
		_MAP_COLOR(LightCyan)
		_MAP_COLOR(LightGoldenrodYellow)
		_MAP_COLOR(LightGray)
		_MAP_COLOR(LightGreen)
		_MAP_COLOR(LightPink)
		_MAP_COLOR(LightSalmon)
		_MAP_COLOR(LightSeaGreen)
		_MAP_COLOR(LightSkyBlue)
		_MAP_COLOR(LightSlateGray)
		_MAP_COLOR(LightSteelBlue)
		_MAP_COLOR(LightYellow)
		_MAP_COLOR(Lime)
		_MAP_COLOR(LimeGreen)
		_MAP_COLOR(Linen)
		_MAP_COLOR(Magenta)
		_MAP_COLOR(Maroon)
		_MAP_COLOR(MediumAquamarine)
		_MAP_COLOR(MediumBlue)
		_MAP_COLOR(MediumOrchid)
		_MAP_COLOR(MediumPurple)
		_MAP_COLOR(MediumSeaGreen)
		_MAP_COLOR(MediumSlateBlue)
		_MAP_COLOR(MediumSpringGreen)
		_MAP_COLOR(MediumTurquoise)
		_MAP_COLOR(MediumVioletRed)
		_MAP_COLOR(MidnightBlue)
		_MAP_COLOR(MintCream)
		_MAP_COLOR(MistyRose)
		_MAP_COLOR(Moccasin)
		_MAP_COLOR(NavajoWhite)
		_MAP_COLOR(Navy)
		_MAP_COLOR(OldLace)
		_MAP_COLOR(Olive)
		_MAP_COLOR(OliveDrab)
		_MAP_COLOR(Orange)
		_MAP_COLOR(OrangeRed)
		_MAP_COLOR(Orchid)
		_MAP_COLOR(PaleGoldenrod)
		_MAP_COLOR(PaleGreen)
		_MAP_COLOR(PaleTurquoise)
		_MAP_COLOR(PaleVioletRed)
		_MAP_COLOR(PapayaWhip)
		_MAP_COLOR(PeachPuff)
		_MAP_COLOR(Peru)
		_MAP_COLOR(Pink)
		_MAP_COLOR(Plum)
		_MAP_COLOR(PowderBlue)
		_MAP_COLOR(Purple)
		_MAP_COLOR(Red)
		_MAP_COLOR(RosyBrown)
		_MAP_COLOR(RoyalBlue)
		_MAP_COLOR(SaddleBrown)
		_MAP_COLOR(Salmon)
		_MAP_COLOR(SandyBrown)
		_MAP_COLOR(SeaGreen)
		_MAP_COLOR(SeaShell)
		_MAP_COLOR(Sienna)
		_MAP_COLOR(Silver)
		_MAP_COLOR(SkyBlue)
		_MAP_COLOR(SlateBlue)
		_MAP_COLOR(SlateGray)
		_MAP_COLOR(Snow)
		_MAP_COLOR(SpringGreen)
		_MAP_COLOR(SteelBlue)
		_MAP_COLOR(Tan)
		_MAP_COLOR(Teal)
		_MAP_COLOR(Thistle)
		_MAP_COLOR(Tomato)
		_MAP_COLOR(Transparent)
		_MAP_COLOR(Turquoise)
		_MAP_COLOR(Violet)
		_MAP_COLOR(Wheat)
		_MAP_COLOR(White)
		_MAP_COLOR(WhiteSmoke)
		_MAP_COLOR(Yellow)
		_MAP_COLOR(YellowGreen)
	}
	
	Color getColorFromName(const String& nameLower)
	{
		Color ret;
		if (mapName.get(nameLower, &ret)) {
			return ret;
		}
		return Color::zero();
	}
	
};

SLIB_SAFE_STATIC_GETTER(_Color_Name_Map, _Color_getNameMap)

template <class CT, class ST>
static sl_reg _Color_parse(Color* _out, const CT* sz, sl_size pos, sl_size len)
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
						iRet = ST::parseFloat(&a, sz, pos, len);
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
						iRet = ST::parseUint32(10, comp + i, sz, pos, len);
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
				
				_Color_Name_Map& nm = _Color_getNameMap();
				if ((void*)&nm) {
					Color color = nm.getColorFromName(String(s, n));
					if (color.isNotZero()) {
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

SLIB_DEFINE_PARSE_FUNCTIONS(Color, _Color_parse)


Color::operator Color3f() const
{
	sl_real _r = r;
	sl_real _g = g;
	sl_real _b = b;
	return {_r / 255, _g / 255, _b / 255};
}

Color::operator Color4f() const
{
	sl_real _r = r;
	sl_real _g = g;
	sl_real _b = b;
	sl_real _a = a;
	return {_r / 255, _g / 255, _b / 255, _a / 255};
}

SLIB_GRAPHICS_NAMESPACE_END

