#ifndef CHECKHEADER_SLIB_GRAPHICS_COLOR
#define CHECKHEADER_SLIB_GRAPHICS_COLOR

#include "definition.h"
#include "../core/math.h"
#include "../math/geometry.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

typedef Vector3f Color3f;
typedef Vector4f Color4f;

class SLIB_EXPORT Color
{
public:
	union {
		sl_uint32 value;
		sl_uint8 c[4]; // b, g, r, a
	};
	
	SLIB_INLINE Color()
	{
	}
	
	SLIB_INLINE Color(const Color& other)
	{
		value = other.value;
	}
	
	SLIB_INLINE Color(sl_uint32 _r, sl_uint32 _g, sl_uint32 _b, sl_uint32 _a = 255)
	{
		b() = (sl_uint8)_b;
		g() = (sl_uint8)_g;
		r() = (sl_uint8)_r;
		a() = (sl_uint8)_a;
	}
	
	SLIB_INLINE Color(sl_uint32 argb)
	{
		setARGB(argb);
	}
	

	SLIB_INLINE Color(const Color3f& v)
	{
		sl_int32 _r = Math::clamp0_255((sl_int32)(v.x * 255));
		sl_int32 _g = Math::clamp0_255((sl_int32)(v.y * 255));
		sl_int32 _b = Math::clamp0_255((sl_int32)(v.z * 255));
		r() = (sl_uint8)(_r);
		g() = (sl_uint8)(_g);
		b() = (sl_uint8)(_b);
	}

	SLIB_INLINE Color(const Color4f& v)
	{
		sl_int32 _r = Math::clamp0_255((sl_int32)(v.x * 255));
		sl_int32 _g = Math::clamp0_255((sl_int32)(v.y * 255));
		sl_int32 _b = Math::clamp0_255((sl_int32)(v.z * 255));
		sl_int32 _a = Math::clamp0_255((sl_int32)(v.w * 255));
		a() = (sl_uint8)(_a);
		r() = (sl_uint8)(_r);
		g() = (sl_uint8)(_g);
		b() = (sl_uint8)(_b);
	}
	
	SLIB_INLINE Color& operator=(const Color& other)
	{
		value = other.value;
		return *this;
	}
	SLIB_INLINE sl_bool operator==(const Color& other) const
	{
		return value == other.value;
	}
	SLIB_INLINE sl_bool operator!=(const Color& other) const
	{
		return value != other.value;
	}

	SLIB_INLINE operator Color3f() const
	{
		sl_real _r = getRed();
		sl_real _g = getGreen();
		sl_real _b = getBlue();
		return Color3f(_r / 255, _g / 255, _b / 255);
	}

	SLIB_INLINE operator Color4f() const
	{
		sl_real _r = getRed();
		sl_real _g = getGreen();
		sl_real _b = getBlue();
		sl_real _a = getAlpha();
		return Color4f(_r / 255, _g / 255, _b / 255, _a / 255);
	}

	SLIB_INLINE sl_bool isZero() const
	{
		return value == 0;
	}
	
	SLIB_INLINE sl_bool isNotZero() const
	{
		return value != 0;
	}
	
	SLIB_INLINE void setZero()
	{
		value = 0;
	}
	
private:
	static sl_uint8 _zero[4];
	static sl_uint8 _transparent[4];
	static sl_uint8 _black[4];
	static sl_uint8 _white[4];
	static sl_uint8 _gray[4];
	static sl_uint8 _red[4];
	static sl_uint8 _green[4];
	static sl_uint8 _blue[4];
	static sl_uint8 _yellow[4];
	static sl_uint8 _magenta[4];
	static sl_uint8 _cyan[4];
	
public:
	SLIB_INLINE static const Color& zero()
	{
		return *((Color*)((void*)(&_zero)));
	}
	
	SLIB_INLINE static const Color& transparent()
	{
		return *((Color*)((void*)(&_transparent)));
	}
	
	SLIB_INLINE static const Color& black()
	{
		return *((Color*)((void*)(&_black)));
	}
	
	SLIB_INLINE static const Color& white()
	{
		return *((Color*)((void*)(&_white)));
	}
	
	SLIB_INLINE static const Color& gray()
	{
		return *((Color*)((void*)(&_gray)));
	}
	
	SLIB_INLINE static Color gray(sl_uint8 g)
	{
		return Color(g, g, g);
	}

	SLIB_INLINE static const Color& red()
	{
		return *((Color*)((void*)(&_red)));
	}
	
	SLIB_INLINE static const Color& green()
	{
		return *((Color*)((void*)(&_green)));
	}
	
	SLIB_INLINE static const Color& blue()
	{
		return *((Color*)((void*)(&_blue)));
	}
	
	SLIB_INLINE static const Color& yellow()
	{
		return *((Color*)((void*)(&_yellow)));
	}
	
	SLIB_INLINE static const Color& magenta()
	{
		return *((Color*)((void*)(&_magenta)));
	}
	
	SLIB_INLINE static const Color& cyan()
	{
		return *((Color*)((void*)(&_cyan)));
	}
	
public:
	SLIB_INLINE sl_uint8& b()
	{
		return c[0];
	}
	SLIB_INLINE const sl_uint8 b() const
	{
		return c[0];
	}
	SLIB_INLINE sl_uint8& g()
	{
		return c[1];
	}
	SLIB_INLINE sl_uint8 g() const
	{
		return c[1];
	}
	SLIB_INLINE sl_uint8& r()
	{
		return c[2];
	}
	SLIB_INLINE sl_uint8 r() const
	{
		return c[2];
	}
	SLIB_INLINE sl_uint8& a()
	{
		return c[3];
	}
	SLIB_INLINE sl_uint8 a() const
	{
		return c[3];
	}

	SLIB_INLINE sl_uint8 getBlue() const
	{
		return c[0];
	}
	SLIB_INLINE sl_uint8 getGreen() const
	{
		return c[1];
	}
	SLIB_INLINE sl_uint8 getRed() const
	{
		return c[2];
	}
	SLIB_INLINE sl_uint8 getAlpha() const
	{
		return c[3];
	}

	SLIB_INLINE void setBlue(sl_uint8 v)
	{
		c[0] = v;
	}
	SLIB_INLINE void setGreen(sl_uint8 v)
	{
		c[1] = v;
	}
	SLIB_INLINE void setRed(sl_uint8 v)
	{
		c[2] = v;
	}
	SLIB_INLINE void setAlpha(sl_uint8 v)
	{
		c[3] = v;
	}
	SLIB_INLINE void setRGB(sl_uint8 R, sl_uint8 G, sl_uint8 B)
	{
		c[0] = R;
		c[1] = G;
		c[2] = B;
	}
	SLIB_INLINE void setRGBA(sl_uint8 R, sl_uint8 G, sl_uint8 B, sl_uint8 A)
	{
		c[0] = R;
		c[1] = G;
		c[2] = B;
		c[3] = A;
	}

	SLIB_INLINE float getBlueF() const
	{
		return (float)(c[0]) / 255.0f;
	}
	SLIB_INLINE float getGreenF() const
	{
		return (float)(c[1]) / 255.0f;
	}
	SLIB_INLINE float getRedF() const
	{
		return (float)(c[2]) / 255.0f;
	}
	SLIB_INLINE float getAlphaF() const
	{
		return (float)(c[3]) / 255.0f;
	}

	SLIB_INLINE void setBlueF(float v)
	{
		sl_int32 n = (sl_int32)(v * 255.0f);
		if (n > 255) {
			n = 255;
		}
		if (n < 0) {
			n = 0;
		}
		c[0] = (sl_uint8)n;
	}
	SLIB_INLINE void setGreenF(float v)
	{
		sl_int32 n = (sl_int32)(v * 255.0f);
		if (n > 255) {
			n = 255;
		}
		if (n < 0) {
			n = 0;
		}
		c[1] = (sl_uint8)n;
	}
	SLIB_INLINE void setRedF(float v)
	{
		sl_int32 n = (sl_int32)(v * 255.0f);
		if (n > 255) {
			n = 255;
		}
		if (n < 0) {
			n = 0;
		}
		c[2] = (sl_uint8)n;
	}
	SLIB_INLINE void setAlphaF(float v)
	{
		sl_int32 n = (sl_int32)(v * 255.0f);
		if (n > 255) {
			n = 255;
		}
		if (n < 0) {
			n = 0;
		}
		c[3] = (sl_uint8)n;
	}
	
	SLIB_INLINE sl_uint32 getARGB() const
	{
		return ((int)(c[3]) << 24) | ((int)(c[2]) << 16) | ((int)(c[1]) << 8) | ((int)(c[0]));
	}
	SLIB_INLINE void setARGB(sl_uint32 v)
	{
		sl_uint8 _b = v & 0xFF;
		sl_uint8 _g = (v >> 8) & 0xFF;
		sl_uint8 _r = (v >> 16) & 0xFF;
		sl_uint8 _a = (v >> 24) & 0xFF;
		b() = _b;
		g() = _g;
		r() = _r;
		a() = _a;
	}

	SLIB_INLINE sl_uint32 getABGR() const
	{
		return ((int)(c[3]) << 24) | ((int)(c[0]) << 16) | ((int)(c[1]) << 8) | ((int)(c[2]));
	}
	SLIB_INLINE void setABGR(sl_uint32 v)
	{
		sl_uint8 _r = v & 0xFF;
		sl_uint8 _g = (v >> 8) & 0xFF;
		sl_uint8 _b = (v >> 16) & 0xFF;
		sl_uint8 _a = (v >> 24) & 0xFF;
		b() = _b;
		g() = _g;
		r() = _r;
		a() = _a;
	}

	SLIB_INLINE sl_uint32 getRGBA() const
	{
		return ((int)(c[3])) | ((int)(c[2]) << 24) | ((int)(c[1]) << 16) | ((int)(c[0]) << 8);
	}
	SLIB_INLINE void setRGBA(sl_uint32 v)
	{
		sl_uint8 _a = v & 0xFF;
		sl_uint8 _b = (v >> 8) & 0xFF;
		sl_uint8 _g = (v >> 16) & 0xFF;
		sl_uint8 _r = (v >> 24) & 0xFF;
		b() = _b;
		g() = _g;
		r() = _r;
		a() = _a;
	}

	SLIB_INLINE sl_uint32 getBGRA() const
	{
		return ((int)(c[3])) | ((int)(c[0]) << 24) | ((int)(c[1]) << 16) | ((int)(c[2]) << 8);
	}
	SLIB_INLINE void setBGRA(sl_uint32 v)
	{
		sl_uint8 _a = v & 0xFF;
		sl_uint8 _r = (v >> 8) & 0xFF;
		sl_uint8 _g = (v >> 16) & 0xFF;
		sl_uint8 _b = (v >> 24) & 0xFF;
		b() = _b;
		g() = _g;
		r() = _r;
		a() = _a;
	}

	SLIB_INLINE sl_uint32 getRGB() const
	{
		return ((int)(c[2]) << 16) | ((int)(c[1]) << 8) | ((int)(c[0]));
	}
	SLIB_INLINE void setRGB(sl_uint32 v)
	{
		sl_uint8 _b = v & 0xFF;
		sl_uint8 _g = (v >> 8) & 0xFF;
		sl_uint8 _r = (v >> 16) & 0xFF;
		b() = _b;
		g() = _g;
		r() = _r;
	}

	SLIB_INLINE sl_uint32 getBGR() const
	{
		return ((int)(c[0]) << 16) | ((int)(c[1]) << 8) | ((int)(c[2]));
	}
	SLIB_INLINE void setBGR(sl_uint32 v)
	{
		sl_uint8 _r = v & 0xFF;
		sl_uint8 _g = (v >> 8) & 0xFF;
		sl_uint8 _b = (v >> 16) & 0xFF;
		b() = _b;
		g() = _g;
		r() = _r;
	}
	
	enum
	{
		AliceBlue = 0xFFF0F8FF,
		AntiqueWhite = 0xFFFAEBD7,
		Aqua = 0xFF00FFFF,
		Aquamarine = 0xFF7FFFD4,
		Azure = 0xFFF0FFFF,
		Beige = 0xFFF5F5DC,
		Bisque = 0xFFFFE4C4,
		Black = 0xFF000000,
		BlanchedAlmond = 0xFFFFEBCD,
		Blue = 0xFF0000FF,
		BlueViolet = 0xFF8A2BE2,
		Brown = 0xFFA52A2A,
		BurlyWood = 0xFFDEB887,
		CadetBlue = 0xFF5F9EA0,
		Chartreuse = 0xFF7FFF00,
		Chocolate = 0xFFD2691E,
		Coral = 0xFFFF7F50,
		CornflowerBlue = 0xFF6495ED,
		Cornsilk = 0xFFFFF8DC,
		Crimson = 0xFFDC143C,
		Cyan = 0xFF00FFFF,
		DarkBlue = 0xFF00008B,
		DarkCyan = 0xFF008B8B,
		DarkGoldenrod = 0xFFB8860B,
		DarkGray = 0xFFA9A9A9,
		DarkGreen = 0xFF006400,
		DarkKhaki = 0xFFBDB76B,
		DarkMagenta = 0xFF8B008B,
		DarkOliveGreen = 0xFF556B2F,
		DarkOrange = 0xFFFF8C00,
		DarkOrchid = 0xFF9932CC,
		DarkRed = 0xFF8B0000,
		DarkSalmon = 0xFFE9967A,
		DarkSeaGreen = 0xFF8FBC8B,
		DarkSlateBlue = 0xFF483D8B,
		DarkSlateGray = 0xFF2F4F4F,
		DarkTurquoise = 0xFF00CED1,
		DarkViolet = 0xFF9400D3,
		DeepPink = 0xFFFF1493,
		DeepSkyBlue = 0xFF00BFFF,
		DimGray = 0xFF696969,
		DodgerBlue = 0xFF1E90FF,
		Firebrick = 0xFFB22222,
		FloralWhite = 0xFFFFFAF0,
		ForestGreen = 0xFF228B22,
		Fuchsia = 0xFFFF00FF,
		Gainsboro = 0xFFDCDCDC,
		GhostWhite = 0xFFF8F8FF,
		Gold = 0xFFFFD700,
		Goldenrod = 0xFFDAA520,
		Gray = 0xFF808080,
		Green = 0xFF008000,
		GreenYellow = 0xFFADFF2F,
		Honeydew = 0xFFF0FFF0,
		HotPink = 0xFFFF69B4,
		IndianRed = 0xFFCD5C5C,
		Indigo = 0xFF4B0082,
		Ivory = 0xFFFFFFF0,
		Khaki = 0xFFF0E68C,
		Lavender = 0xFFE6E6FA,
		LavenderBlush = 0xFFFFF0F5,
		LawnGreen = 0xFF7CFC00,
		LemonChiffon = 0xFFFFFACD,
		LightBlue = 0xFFADD8E6,
		LightCoral = 0xFFF08080,
		LightCyan = 0xFFE0FFFF,
		LightGoldenrodYellow = 0xFFFAFAD2,
		LightGray = 0xFFD3D3D3,
		LightGreen = 0xFF90EE90,
		LightPink = 0xFFFFB6C1,
		LightSalmon = 0xFFFFA07A,
		LightSeaGreen = 0xFF20B2AA,
		LightSkyBlue = 0xFF87CEFA,
		LightSlateGray = 0xFF778899,
		LightSteelBlue = 0xFFB0C4DE,
		LightYellow = 0xFFFFFFE0,
		Lime = 0xFF00FF00,
		LimeGreen = 0xFF32CD32,
		Linen = 0xFFFAF0E6,
		Magenta = 0xFFFF00FF,
		Maroon = 0xFF800000,
		MediumAquamarine = 0xFF66CDAA,
		MediumBlue = 0xFF0000CD,
		MediumOrchid = 0xFFBA55D3,
		MediumPurple = 0xFF9370DB,
		MediumSeaGreen = 0xFF3CB371,
		MediumSlateBlue = 0xFF7B68EE,
		MediumSpringGreen = 0xFF00FA9A,
		MediumTurquoise = 0xFF48D1CC,
		MediumVioletRed = 0xFFC71585,
		MidnightBlue = 0xFF191970,
		MintCream = 0xFFF5FFFA,
		MistyRose = 0xFFFFE4E1,
		Moccasin = 0xFFFFE4B5,
		NavajoWhite = 0xFFFFDEAD,
		Navy = 0xFF000080,
		OldLace = 0xFFFDF5E6,
		Olive = 0xFF808000,
		OliveDrab = 0xFF6B8E23,
		Orange = 0xFFFFA500,
		OrangeRed = 0xFFFF4500,
		Orchid = 0xFFDA70D6,
		PaleGoldenrod = 0xFFEEE8AA,
		PaleGreen = 0xFF98FB98,
		PaleTurquoise = 0xFFAFEEEE,
		PaleVioletRed = 0xFFDB7093,
		PapayaWhip = 0xFFFFEFD5,
		PeachPuff = 0xFFFFDAB9,
		Peru = 0xFFCD853F,
		Pink = 0xFFFFC0CB,
		Plum = 0xFFDDA0DD,
		PowderBlue = 0xFFB0E0E6,
		Purple = 0xFF800080,
		Red = 0xFFFF0000,
		RosyBrown = 0xFFBC8F8F,
		RoyalBlue = 0xFF4169E1,
		SaddleBrown = 0xFF8B4513,
		Salmon = 0xFFFA8072,
		SandyBrown = 0xFFF4A460,
		SeaGreen = 0xFF2E8B57,
		SeaShell = 0xFFFFF5EE,
		Sienna = 0xFFA0522D,
		Silver = 0xFFC0C0C0,
		SkyBlue = 0xFF87CEEB,
		SlateBlue = 0xFF6A5ACD,
		SlateGray = 0xFF708090,
		Snow = 0xFFFFFAFA,
		SpringGreen = 0xFF00FF7F,
		SteelBlue = 0xFF4682B4,
		Tan = 0xFFD2B48C,
		Teal = 0xFF008080,
		Thistle = 0xFFD8BFD8,
		Tomato = 0xFFFF6347,
		Transparent = 0x00FFFFFF,
		Turquoise = 0xFF40E0D0,
		Violet = 0xFFEE82EE,
		Wheat = 0xFFF5DEB3,
		White = 0xFFFFFFFF,
		WhiteSmoke = 0xFFF5F5F5,
		Yellow = 0xFFFFFF00,
		YellowGreen = 0xFF9ACD32
	};
	
	// assume that` dst is premultiplied alpha, and src is non-premultiplied alpha
	SLIB_INLINE void blend_PA_NPA(sl_uint32 _r, sl_uint32 _g, sl_uint32 _b, sl_uint32 _a)
	{
		sl_uint32 _or = r();
		sl_uint32 _og = g();
		sl_uint32 _ob = b();
		sl_uint32 _oa = a();
		sl_uint32 sa = _a;
		_or = (_or * (255 - sa) + _r * sa) / 255;
		_og = (_og * (255 - sa) + _g * sa) / 255;
		_ob = (_ob * (255 - sa) + _b * sa) / 255;
		_oa = (_oa * (255 - sa) + 255 * sa) / 255;
		r() = (sl_uint8)(_or);
		g() = (sl_uint8)(_og);
		b() = (sl_uint8)(_ob);
		a() = (sl_uint8)(_oa);
	}

	// assume that dst is premultiplied alpha, and src is non-premultiplied alpha
	SLIB_INLINE void blend_PA_NPA(const Color& src)
	{
		blend_PA_NPA(src.r(), src.g(), src.b(), src.a());
	}

	// assume that dst is premultiplied alpha, and src is premultiplied alpha
	SLIB_INLINE void blend_PA_PA(sl_uint32 _r, sl_uint32 _g, sl_uint32 _b, sl_uint32 _a)
	{
		sl_uint32 _or = r();
		sl_uint32 _og = g();
		sl_uint32 _ob = b();
		sl_uint32 oa = a();
		sl_uint32 sa = _a;
		_or = Math::clamp0_255(_or * (255 - sa) / 255 + _r);
		_og = Math::clamp0_255(_og * (255 - sa) / 255 + _g);
		_ob = Math::clamp0_255(_ob * (255 - sa) / 255 + _b);
		oa = (oa * (255 - sa) + 255 * sa) / 255;
		r() = (sl_uint8)(_or);
		g() = (sl_uint8)(_og);
		b() = (sl_uint8)(_ob);
		a() = (sl_uint8)(oa);
	}

	// assume that dst is premultiplied alpha, and src is premultiplied alpha
	SLIB_INLINE void blend_PA_PA(const Color& src)
	{
		blend_PA_NPA(src.r(), src.g(), src.b(), src.a());
	}

	// assume that dst is non-premultiplied alpha, and src is non-premultiplied alpha
	SLIB_INLINE void blend_NPA_NPA(sl_uint32 _r, sl_uint32 _g, sl_uint32 _b, sl_uint32 _a)
	{
		sl_uint32 _or = r();
		sl_uint32 _og = g();
		sl_uint32 _ob = b();
		sl_uint32 oa = a();
		sl_uint32 sa = _a;
		sl_uint32 _oa = (oa * (255 - sa) + 255 * sa) + 1;
		_or = Math::clamp0_255((_or * oa * (255 - sa) + _r * (255 * sa + 1)) / _oa);
		_og = Math::clamp0_255((_og * oa * (255 - sa) + _g * (255 * sa + 1)) / _oa);
		_ob = Math::clamp0_255((_ob * oa * (255 - sa) + _b * (255 * sa + 1)) / _oa);
		_oa = (_oa - 1) / 255;
		r() = (sl_uint8)(_or);
		g() = (sl_uint8)(_og);
		b() = (sl_uint8)(_ob);
		a() = (sl_uint8)(_oa);
	}

	// assume that dst is non-premultiplied alpha, and src is non-premultiplied alpha
	SLIB_INLINE void blend_NPA_NPA(const Color& src)
	{
		blend_NPA_NPA(src.r(), src.g(), src.b(), src.a());
	}


	// assume that dst is non-premultiplied alpha, and src is premultiplied alpha
	SLIB_INLINE void blend_NPA_PA(sl_uint32 _r, sl_uint32 _g, sl_uint32 _b, sl_uint32 _a)
	{
		sl_uint32 _or = r();
		sl_uint32 _og = g();
		sl_uint32 _ob = b();
		sl_uint32 oa = a();
		sl_uint32 sa = _a;
		sl_uint32 _oa = (oa * (255 - sa) + 255 * sa) + 1;
		_or = Math::clamp0_255((_or * oa * (255 - sa) + (_r << 16)) / _oa);
		_og = Math::clamp0_255((_og * oa * (255 - sa) + (_g << 16)) / _oa);
		_ob = Math::clamp0_255((_ob * oa * (255 - sa) + (_b << 16)) / _oa);
		_oa = (_oa - 1) / 255;
		r() = (sl_uint8)(_or);
		g() = (sl_uint8)(_og);
		b() = (sl_uint8)(_ob);
		a() = (sl_uint8)(_oa);
	}

	// assume that dst is non-premultiplied alpha, and src is premultiplied alpha
	SLIB_INLINE void blend_NPA_PA(const Color& src)
	{
		blend_NPA_PA(src.r(), src.g(), src.b(), src.a());
	}


	SLIB_INLINE void convertNPAtoPA()
	{
		sl_uint32 _or = r();
		sl_uint32 _og = g();
		sl_uint32 _ob = b();
		sl_uint32 oa = a();
		oa++;
		_or = (_or * oa) >> 8;
		_og = (_og * oa) >> 8;
		_ob = (_ob * oa) >> 8;
		r() = (sl_uint8)(_or & 255);
		g() = (sl_uint8)(_og & 255);
		b() = (sl_uint8)(_ob & 255);
	}

	SLIB_INLINE void convertPAtoNPA()
	{
		sl_uint32 _or = r();
		sl_uint32 _og = g();
		sl_uint32 _ob = b();
		sl_uint32 oa = a();
		oa++;
		_or = Math::clamp0_255((_or << 8) / oa);
		_og = Math::clamp0_255((_og << 8) / oa);
		_ob = Math::clamp0_255((_ob << 8) / oa);
		r() = (sl_uint8)(_or);
		g() = (sl_uint8)(_og);
		b() = (sl_uint8)(_ob);
	}

	enum Model
	{
		// RGB
		// 32 bit models
		RGBA = 0x2001,
		BGRA = 0x2002,
		ARGB = 0x2003,
		ABGR = 0x2004,
		// 24 bit models
		RGB = 0x1801,
		BGR = 0x1802,
		// 16 bit models
		RGB565BE = 0x1001,
		RGB565LE = 0x1002,
		BGR565BE = 0x1003,
		BGR565LE = 0x1004,
		// 8 bit models
		GRAY8 = 0x0801,
		ALPHA8 = 0x0802,

		// YUV
		YUV444 = 0x100000,
		// YUV420 planar formats
		YUV_I420 = 0x100001, // 8 bit Y plane followed by 8 bit 2x2 subsampled U and V planes
		YUV_YV12 = 0x100002, // 8 bit Y plane followed by 8 bit 2x2 subsampled V and U planes
		YUV_NV21 = 0x100003, // 8-bit Y plane followed by an interleaved V/U plane with 2x2 subsampling (Android camera standard)
		YUV_NV12 = 0x100004, // 8-bit Y plane followed by an interleaved U/V plane with 2x2 subsampling
		
		defaultModel = BGRA
	};
	
	SLIB_INLINE static sl_bool is32bitRGBModel(Model model)
	{
		return model >= RGBA && model <= ABGR;
	}
	
	SLIB_INLINE static sl_bool isYUV420Model(Model model)
	{
		return model >= YUV_I420 && model <= YUV_NV12;
	}

	SLIB_INLINE static sl_uint32 getModelBits(Model model)
	{
		switch (model) {
			case RGBA:
			case BGRA:
			case ARGB:
			case ABGR:
				return 32;
			case RGB:
			case BGR:
				return 24;
			case RGB565BE:
			case RGB565LE:
			case BGR565BE:
			case BGR565LE:
				return 16;
			case GRAY8:
			case ALPHA8:
				return 8;

			case YUV444:
				return 24;
				
			default:
				return 0;
		}
		return 0;
	}

	SLIB_INLINE static sl_size calculatePitchAlign1(sl_size width, sl_uint32 bpp)
	{
		return (width * bpp + 7) >> 3;
	}
	SLIB_INLINE static sl_size calculatePitchAlign2(sl_size width, sl_uint32 bpp)
	{
		return (width * bpp + 15) >> 4;
	}
	SLIB_INLINE static sl_size calculatePitchAlign4(sl_size width, sl_uint32 bpp)
	{
		return (width * bpp + 31) >> 5;
	}

	// RGBA
	SLIB_INLINE void readRGBA(const void* rgba)
	{
		const sl_uint8* p = (const sl_uint8*)rgba;
		r() = p[0];
		g() = p[1];
		b() = p[2];
		a() = p[3];
	}
	SLIB_INLINE void readRGBA_PAtoNPA(const void* rgba)
	{
		readRGBA(rgba);
		convertPAtoNPA();
	}
	SLIB_INLINE void readRGBA_NPAtoPA(const void* rgba)
	{
		readRGBA(rgba);
		convertNPAtoPA();
	}
	SLIB_INLINE void writeRGBA(void* rgba) const
	{
		sl_uint8* p = (sl_uint8*)rgba;
		p[0] = r();
		p[1] = g();
		p[2] = b();
		p[3] = a();
	}
	SLIB_INLINE void writeRGBA_PAtoNPA(void* rgba) const
	{
		Color c = *this;
		c.convertPAtoNPA();
		c.writeRGBA(rgba);
	}
	SLIB_INLINE void writeRGBA_NPAtoPA(void* rgba) const
	{
		Color c = *this;
		c.convertNPAtoPA();
		c.writeRGBA(rgba);
	}

	// BGRA
	SLIB_INLINE void readBGRA(const void* bgra)
	{
		const sl_uint8* p = (const sl_uint8*)bgra;
		b() = p[0];
		g() = p[1];
		r() = p[2];
		a() = p[3];
	}
	SLIB_INLINE void readBGRA_PAtoNPA(const void* bgra)
	{
		readBGRA(bgra);
		convertPAtoNPA();
	}
	SLIB_INLINE void readBGRA_NPAtoPA(const void* bgra)
	{
		readBGRA(bgra);
		convertNPAtoPA();
	}
	SLIB_INLINE void writeBGRA(void* bgra) const
	{
		sl_uint8* p = (sl_uint8*)bgra;
		p[0] = b();
		p[1] = g();
		p[2] = r();
		p[3] = a();
	}
	SLIB_INLINE void writeBGRA_PAtoNPA(void* bgra) const
	{
		Color c = *this;
		c.convertPAtoNPA();
		c.writeBGRA(bgra);
	}
	SLIB_INLINE void writeBGRA_NPAtoPA(void* bgra) const
	{
		Color c = *this;
		c.convertNPAtoPA();
		c.writeBGRA(bgra);
	}
	
	// ARGB
	SLIB_INLINE void readARGB(const void* argb)
	{
		const sl_uint8* p = (const sl_uint8*)argb;
		a() = p[0];
		r() = p[1];
		g() = p[2];
		b() = p[3];
	}
	SLIB_INLINE void readARGB_PAtoNPA(const void* argb)
	{
		readARGB(argb);
		convertPAtoNPA();
	}
	SLIB_INLINE void readARGB_NPAtoPA(const void* argb)
	{
		readARGB(argb);
		convertNPAtoPA();
	}
	SLIB_INLINE void writeARGB(void* argb) const
	{
		sl_uint8* p = (sl_uint8*)argb;
		p[0] = a();
		p[1] = r();
		p[2] = g();
		p[3] = b();
	}
	SLIB_INLINE void writeARGB_PAtoNPA(void* argb) const
	{
		Color c = *this;
		c.convertPAtoNPA();
		c.writeARGB(argb);
	}
	SLIB_INLINE void writeARGB_NPAtoPA(void* argb) const
	{
		Color c = *this;
		c.convertNPAtoPA();
		c.writeARGB(argb);
	}

	// ABGR
	SLIB_INLINE void readABGR(const void* abgr)
	{
		const sl_uint8* p = (const sl_uint8*)abgr;
		a() = p[0];
		b() = p[1];
		g() = p[2];
		r() = p[3];
	}
	SLIB_INLINE void readABGR_PAtoNPA(const void* abgr)
	{
		readABGR(abgr);
		convertPAtoNPA();
	}
	SLIB_INLINE void readABGR_NPAtoPA(const void* abgr)
	{
		readABGR(abgr);
		convertNPAtoPA();
	}
	SLIB_INLINE void writeABGR(void* abgr) const
	{
		sl_uint8* p = (sl_uint8*)abgr;
		p[0] = a();
		p[1] = b();
		p[2] = g();
		p[3] = r();
	}
	SLIB_INLINE void writeABGR_PAtoNPA(void* abgr) const
	{
		Color c = *this;
		c.convertPAtoNPA();
		c.writeABGR(abgr);
	}
	SLIB_INLINE void writeABGR_NPAtoPA(void* abgr) const
	{
		Color c = *this;
		c.convertNPAtoPA();
		c.writeABGR(abgr);
	}

	// RGB
	SLIB_INLINE void readRGB(const void* rgb)
	{
		const sl_uint8* p = (const sl_uint8*)rgb;
		r() = p[0];
		g() = p[1];
		b() = p[2];
		a() = 255;
	}
	SLIB_INLINE void writeRGB(void* rgb) const
	{
		sl_uint8* p = (sl_uint8*)rgb;
		p[0] = r();
		p[1] = g();
		p[2] = b();
	}

	// BGR
	SLIB_INLINE void readBGR(const void* bgr)
	{
		const sl_uint8* p = (const sl_uint8*)bgr;
		b() = p[0];
		g() = p[1];
		r() = p[2];
		a() = 255;
	}
	SLIB_INLINE void writeBGR(void* bgr) const
	{
		sl_uint8* p = (sl_uint8*)bgr;
		p[0] = b();
		p[1] = g();
		p[2] = r();
	}

	// RGB565
	SLIB_INLINE void readRGB565BE(const void* c)
	{
		const sl_uint8* p = (const sl_uint8*)c;
		sl_uint32 s = p[0];
		s = (s << 8) | p[1];
		r() = (sl_uint8)((s & 0xF800) >> 8);
		g() = (sl_uint8)((s & 0x07E0) >> 3);
		b() = (sl_uint8)((s & 0x001F) << 3);
		a() = 255;
	}
	SLIB_INLINE void writeRGB565BE(void* c) const
	{
		sl_uint8* p = (sl_uint8*)c;
		sl_uint32 s = r() >> 3;
		s = (s << 5) | (g() >> 2);
		s = (s << 6) | (b() >> 3);
		p[0] = (sl_uint8)(s >> 8);
		p[1] = (sl_uint8)(s);
	}
	SLIB_INLINE void readRGB565LE(const void* c)
	{
		const sl_uint8* p = (const sl_uint8*)c;
		sl_uint16 s = p[1];
		s = (s << 8) | p[0];
		r() = (sl_uint8)((s & 0xF800) >> 8);
		g() = (sl_uint8)((s & 0x07E0) >> 3);
		b() = (sl_uint8)((s & 0x001F) << 3);
		a() = 255;
	}
	SLIB_INLINE void writeRGB565LE(void* c) const
	{
		sl_uint8* p = (sl_uint8*)c;
		sl_uint32 s = r() >> 3;
		s = (s << 5) | (g() >> 2);
		s = (s << 6) | (b() >> 3);
		p[1] = (sl_uint8)(s >> 8);
		p[0] = (sl_uint8)(s);
	}

	// BGR565
	SLIB_INLINE void readBGR565BE(const void* c)
	{
		const sl_uint8* p = (const sl_uint8*)c;
		sl_uint32 s = p[0];
		s = (s << 8) | p[1];
		b() = (sl_uint8)((s & 0xF800) >> 8);
		g() = (sl_uint8)((s & 0x07E0) >> 3);
		r() = (sl_uint8)((s & 0x001F) << 3);
		a() = 255;
	}
	SLIB_INLINE void writeBGR565BE(void* c) const
	{
		sl_uint8* p = (sl_uint8*)c;
		sl_uint32 s = b() >> 3;
		s = (s << 5) | (g() >> 2);
		s = (s << 6) | (r() >> 3);
		p[0] = (sl_uint8)(s >> 8);
		p[1] = (sl_uint8)(s);
	}
	SLIB_INLINE void readBGR565LE(const void* c)
	{
		const sl_uint8* p = (const sl_uint8*)c;
		sl_uint16 s = p[1];
		s = (s << 8) | p[0];
		b() = (sl_uint8)((s & 0xF800) >> 8);
		g() = (sl_uint8)((s & 0x07E0) >> 3);
		r() = (sl_uint8)((s & 0x001F) << 3);
		a() = 255;
	}
	SLIB_INLINE void writeBGR565LE(void* c) const
	{
		sl_uint8* p = (sl_uint8*)c;
		sl_uint32 s = b() >> 3;
		s = (s << 5) | (g() >> 2);
		s = (s << 6) | (r() >> 3);
		p[1] = (sl_uint8)(s >> 8);
		p[0] = (sl_uint8)(s);
	}

	// GRAY8
	SLIB_INLINE void readGRAY8(const void* c)
	{
		sl_uint8 p = *(sl_uint8*)c;
		b() = p;
		g() = p;
		r() = p;
		a() = 255;
	}
	SLIB_INLINE void writeGRAY8(void* c) const
	{
		sl_uint32 p = b();
		p += g();
		p += r();
		p /= 3;
		*((sl_uint8*)c) = (sl_uint8)(p);
	}

	// ALPHA8
	SLIB_INLINE void readALPHA8(const void* c)
	{
		a() = *(sl_uint8*)c;
	}
	SLIB_INLINE void writeALPHA8(void* c) const
	{
		*((sl_uint8*)c) = a();
	}

	// YUV444
#define _SLIB_IMAGE_COLOR_YUV_YG 18997 /* round(1.164 * 64 * 256 * 256 / 257) */
#define _SLIB_IMAGE_COLOR_YUV_YGB 1160 /* 1.164 * 64 * 16 - adjusted for even error distribution */
#define _SLIB_IMAGE_COLOR_YUV_UB -128 /* -min(128, round(2.018 * 64)) */
#define _SLIB_IMAGE_COLOR_YUV_UG 25 /* -round(-0.391 * 64) */
#define _SLIB_IMAGE_COLOR_YUV_VG 52 /* -round(-0.813 * 64) */
#define _SLIB_IMAGE_COLOR_YUV_VR -102 /* -round(1.596 * 64) */
#define _SLIB_IMAGE_COLOR_YUV_BB (_SLIB_IMAGE_COLOR_YUV_UB * 128 - _SLIB_IMAGE_COLOR_YUV_YGB)
#define _SLIB_IMAGE_COLOR_YUV_BG (_SLIB_IMAGE_COLOR_YUV_UG * 128 + _SLIB_IMAGE_COLOR_YUV_VG * 128 - _SLIB_IMAGE_COLOR_YUV_YGB)
#define _SLIB_IMAGE_COLOR_YUV_BR (_SLIB_IMAGE_COLOR_YUV_VR * 128 - _SLIB_IMAGE_COLOR_YUV_YGB)
	SLIB_INLINE static void convertRGBToYUV(sl_uint8 R, sl_uint8 G, sl_uint8 B, sl_uint8& Y, sl_uint8& U, sl_uint8& V)
	{
		sl_int32 _r = R;
		sl_int32 _g = G;
		sl_int32 _b = B;
		Y = (sl_uint8)(Math::clamp0_255((66 * _r + 129 * _g + 25 * _b + 0x1080) >> 8)); // y
		U = (sl_uint8)(Math::clamp0_255((112 * _b - 74 * _g - 38 * _r + 0x8080) >> 8)); // u
		V = (sl_uint8)(Math::clamp0_255((112 * _r - 94 * _g - 18 * _b + 0x8080) >> 8)); // v
	}
	SLIB_INLINE static void convertYUVToRGB(sl_uint8 Y, sl_uint8 U, sl_uint8 V, sl_uint8& R, sl_uint8& G, sl_uint8& B)
	{
		sl_int32 _y = Y;
		sl_int32 _u = U;
		sl_int32 _v = V;
		sl_int32 y1 = (sl_uint32)(_y * 0x0101 * _SLIB_IMAGE_COLOR_YUV_YG) >> 16;
		B = (sl_uint8)(Math::clamp0_255((sl_int32)(_SLIB_IMAGE_COLOR_YUV_BB - (_u * _SLIB_IMAGE_COLOR_YUV_UB) + y1) >> 6));
		G = (sl_uint8)(Math::clamp0_255((sl_int32)(_SLIB_IMAGE_COLOR_YUV_BG - (_v * _SLIB_IMAGE_COLOR_YUV_VG + _u * _SLIB_IMAGE_COLOR_YUV_UG) + y1) >> 6));
		R = (sl_uint8)(Math::clamp0_255((sl_int32)(_SLIB_IMAGE_COLOR_YUV_BR - (_v * _SLIB_IMAGE_COLOR_YUV_VR) + y1) >> 6));
	}

	SLIB_INLINE void readYUV444(const void* yuv)
	{
		const sl_uint8* p = (const sl_uint8*)yuv;
		convertYUVToRGB(p[0], p[1], p[2], r(), g(), b());
		a() = 255;
	}
	SLIB_INLINE void writeYUV444(void* yuv) const
	{
		sl_uint8* p = (sl_uint8*)yuv;
		convertRGBToYUV(r(), g(), b(), p[0], p[1], p[2]);
	}

	SLIB_INLINE void setYUV(sl_uint8 y, sl_uint8 u, sl_uint8 v)
	{
		convertYUVToRGB(y, u, v, r(), g(), b());
	}
	SLIB_INLINE void getYUV(sl_uint8& y, sl_uint8& u, sl_uint8& v) const
	{
		convertRGBToYUV(r(), g(), b(), y, u, v);
	}

	void read(Model model, const void* data);
	void read_PAtoNPA(Model model, const void* data);
	void read_NPAtoPA(Model model, const void* data);
	void write(Model model, void* data) const;
	void write_PAtoNPA(Model model, void* data) const;
	void write_NPAtoPA(Model model, void* data) const;
	
	static void copy(sl_size count, const Color* _in, Color* _out);
	static void convert(sl_size count, Model modelIn, const void* _in, Color* _out);
	static void convert(sl_size count, const Color* _in, Model modelOut, void* _out);
	static void convert(sl_size count, Model modelIn, const void* _in, Model modelOut, void* _out);
	
	static void convertPAtoNPA(sl_size count, const Color* _in, Color* _out);
	static void convertPAtoNPA(sl_size count, Model modelIn, const void* _in, Color* _out);
	static void convertPAtoNPA(sl_size count, const Color* _in, Model modelOut, void* _out);
	static void convertPAtoNPA(sl_size count, Model modelIn, const void* _in, Model modelOut, void* _out);
	
	static void convertNPAtoPA(sl_size count, const Color* _in, Color* _out);
	static void convertNPAtoPA(sl_size count, Model modelIn, const void* _in, Color* _out);
	static void convertNPAtoPA(sl_size count, const Color* _in, Model modelOut, void* _out);
	static void convertNPAtoPA(sl_size count, Model modelIn, const void* _in, Model modelOut, void* _out);
	
	static void copy(sl_size width, sl_size height, const Color* _in, sl_reg strideIn, Color* _out, sl_reg strideOut);
	static void convert(sl_size width, sl_size height, Model modelIn, const void* _in, sl_reg pitchIn, Color* _out, sl_reg strideOut);
	static void convert(sl_size width, sl_size height, const Color* _in, sl_reg strideIn, Model modelOut, void* _out, sl_reg pitchOut);
	static void convert(sl_size width, sl_size height, Model modelIn, const void* _in, sl_reg pitchIn, Model modelOut, void* _out, sl_reg pitchOut);
	
	static void convertPAtoNPA(sl_size width, sl_size height, const Color* _in, sl_reg strideIn, Color* _out, sl_reg strideOut);
	static void convertPAtoNPA(sl_size width, sl_size height, Model modelIn, const void* _in, sl_reg pitchIn, Color* _out, sl_reg strideOut);
	static void convertPAtoNPA(sl_size width, sl_size height, const Color* _in, sl_reg strideIn, Model modelOut, void* _out, sl_reg pitchOut);
	static void convertPAtoNPA(sl_size width, sl_size height, Model modelIn, const void* _in, sl_reg pitchIn, Model modelOut, void* _out, sl_reg pitchOut);
	
	static void convertNPAtoPA(sl_size width, sl_size height, const Color* _in, sl_reg strideIn, Color* _out, sl_reg strideOut);
	static void convertNPAtoPA(sl_size width, sl_size height, Model modelIn, const void* _in, sl_reg pitchIn, Color* _out, sl_reg strideOut);
	static void convertNPAtoPA(sl_size width, sl_size height, const Color* _in, sl_reg strideIn, Model modelOut, void* _out, sl_reg pitchOut);
	static void convertNPAtoPA(sl_size width, sl_size height, Model modelIn, const void* _in, sl_reg pitchIn, Model modelOut, void* _out, sl_reg pitchOut);
};

typedef Color::Model ColorModel;

SLIB_GRAPHICS_NAMESPACE_END

#endif
