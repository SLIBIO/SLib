#ifndef CHECKHEADER_SLIB_GRAPHICS_COLOR
#define CHECKHEADER_SLIB_GRAPHICS_COLOR

#include "definition.h"

#include "../core/math.h"
#include "../math/vector3.h"
#include "../math/vector4.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

typedef Vector3f Color3f;
typedef Vector4f Color4f;

class SLIB_EXPORT Color
{
public:
	sl_uint8 r;
	sl_uint8 g;
	sl_uint8 b;
	sl_uint8 a;
	
public:
	SLIB_INLINE Color()
	{
	}
	
	SLIB_INLINE Color(const Color& other)
	{
		r = other.r;
		g = other.g;
		b = other.b;
		a = other.a;
	}
	
	SLIB_INLINE Color(sl_uint32 _r, sl_uint32 _g, sl_uint32 _b, sl_uint32 _a = 255)
	{
		r = (sl_uint8)_r;
		g = (sl_uint8)_g;
		b = (sl_uint8)_b;
		a = (sl_uint8)_a;
	}
	
	SLIB_INLINE Color(sl_uint32 argb)
	{
		setARGB(argb);
	}
	

	Color(const Color3f& v);
	
	Color(const Color4f& v);
	
	SLIB_INLINE Color& operator=(const Color& other)
	{
		*((sl_uint32*)((void*)this)) = *((sl_uint32*)((void*)&other));
		return *this;
	}
	
	SLIB_INLINE sl_bool operator==(const Color& other) const
	{
		return *((sl_uint32*)((void*)this)) == *((sl_uint32*)((void*)&other));
	}
	
	SLIB_INLINE sl_bool operator!=(const Color& other) const
	{
		return *((sl_uint32*)((void*)this)) != *((sl_uint32*)((void*)&other));
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
		return *((sl_uint32*)((void*)this)) == 0;
	}
	
	SLIB_INLINE sl_bool isNotZero() const
	{
		return *((sl_uint32*)((void*)this)) != 0;
	}
	
	SLIB_INLINE void setZero()
	{
		*((sl_uint32*)((void*)this)) = 0;
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
	SLIB_INLINE sl_uint8 getBlue() const
	{
		return b;
	}
	
	SLIB_INLINE sl_uint8 getGreen() const
	{
		return g;
	}
	
	SLIB_INLINE sl_uint8 getRed() const
	{
		return r;
	}
	
	SLIB_INLINE sl_uint8 getAlpha() const
	{
		return a;
	}

	SLIB_INLINE void setBlue(sl_uint8 v)
	{
		b = v;
	}
	
	SLIB_INLINE void setGreen(sl_uint8 v)
	{
		g = v;
	}
	
	SLIB_INLINE void setRed(sl_uint8 v)
	{
		r = v;
	}
	
	SLIB_INLINE void setAlpha(sl_uint8 v)
	{
		a = v;
	}
	
	SLIB_INLINE void setRGB(sl_uint8 R, sl_uint8 G, sl_uint8 B)
	{
		r = R;
		g = G;
		b = B;
	}
	
	SLIB_INLINE void setRGBA(sl_uint8 R, sl_uint8 G, sl_uint8 B, sl_uint8 A)
	{
		r = R;
		g = G;
		b = B;
		a = A;
	}

	SLIB_INLINE float getBlueF() const
	{
		return (float)(b) / 255.0f;
	}
	
	SLIB_INLINE float getGreenF() const
	{
		return (float)(g) / 255.0f;
	}
	
	SLIB_INLINE float getRedF() const
	{
		return (float)(r) / 255.0f;
	}
	
	SLIB_INLINE float getAlphaF() const
	{
		return (float)(a) / 255.0f;
	}

	SLIB_INLINE void setBlueF(float v)
	{
		sl_int32 n = (sl_int32)(v * 255.0f);
		b = (sl_uint8)(Math::clamp0_255(n));
	}
	
	SLIB_INLINE void setGreenF(float v)
	{
		sl_int32 n = (sl_int32)(v * 255.0f);
		g = (sl_uint8)(Math::clamp0_255(n));
	}
	
	SLIB_INLINE void setRedF(float v)
	{
		sl_int32 n = (sl_int32)(v * 255.0f);
		r = (sl_uint8)(Math::clamp0_255(n));
	}
	
	SLIB_INLINE void setAlphaF(float v)
	{
		sl_int32 n = (sl_int32)(v * 255.0f);
		a = (sl_uint8)(Math::clamp0_255(n));
	}
	
	SLIB_INLINE sl_uint32 getARGB() const
	{
		return ((sl_uint32)(a) << 24) | ((sl_uint32)(r) << 16) | ((sl_uint32)(g) << 8) | ((sl_uint32)(b));
	}
	
	SLIB_INLINE void setARGB(sl_uint32 v)
	{
		sl_uint8 _b = v & 0xFF;
		sl_uint8 _g = (v >> 8) & 0xFF;
		sl_uint8 _r = (v >> 16) & 0xFF;
		sl_uint8 _a = (v >> 24) & 0xFF;
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	SLIB_INLINE sl_uint32 getABGR() const
	{
		return ((sl_uint32)(a) << 24) | ((sl_uint32)(b) << 16) | ((sl_uint32)(g) << 8) | ((sl_uint32)(r));
	}
	
	SLIB_INLINE void setABGR(sl_uint32 v)
	{
		sl_uint8 _r = v & 0xFF;
		sl_uint8 _g = (v >> 8) & 0xFF;
		sl_uint8 _b = (v >> 16) & 0xFF;
		sl_uint8 _a = (v >> 24) & 0xFF;
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	SLIB_INLINE sl_uint32 getRGBA() const
	{
		return ((sl_uint32)(a)) | ((sl_uint32)(r) << 24) | ((sl_uint32)(g) << 16) | ((sl_uint32)(b) << 8);
	}
	
	SLIB_INLINE void setRGBA(sl_uint32 v)
	{
		sl_uint8 _a = v & 0xFF;
		sl_uint8 _b = (v >> 8) & 0xFF;
		sl_uint8 _g = (v >> 16) & 0xFF;
		sl_uint8 _r = (v >> 24) & 0xFF;
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	SLIB_INLINE sl_uint32 getBGRA() const
	{
		return ((sl_uint32)(a)) | ((sl_uint32)(b) << 24) | ((sl_uint32)(g) << 16) | ((sl_uint32)(r) << 8);
	}
	
	SLIB_INLINE void setBGRA(sl_uint32 v)
	{
		sl_uint8 _a = v & 0xFF;
		sl_uint8 _r = (v >> 8) & 0xFF;
		sl_uint8 _g = (v >> 16) & 0xFF;
		sl_uint8 _b = (v >> 24) & 0xFF;
		r = _r;
		g = _g;
		b = _b;
		a = _a;
	}

	SLIB_INLINE sl_uint32 getRGB() const
	{
		return ((sl_uint32)(r) << 16) | ((sl_uint32)(g) << 8) | ((sl_uint32)(b));
	}
	
	SLIB_INLINE void setRGB(sl_uint32 v)
	{
		sl_uint8 _b = v & 0xFF;
		sl_uint8 _g = (v >> 8) & 0xFF;
		sl_uint8 _r = (v >> 16) & 0xFF;
		r = _r;
		g = _g;
		b = _b;
	}

	SLIB_INLINE sl_uint32 getBGR() const
	{
		return ((sl_uint32)(b) << 16) | ((sl_uint32)(g) << 8) | ((sl_uint32)(r));
	}
	
	SLIB_INLINE void setBGR(sl_uint32 v)
	{
		sl_uint8 _r = v & 0xFF;
		sl_uint8 _g = (v >> 8) & 0xFF;
		sl_uint8 _b = (v >> 16) & 0xFF;
		r = _r;
		g = _g;
		b = _b;
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

	// assume that dst is premultiplied alpha, and src is non-premultiplied alpha
	SLIB_INLINE void blend_PA_NPA(const Color& src)
	{
		blend_PA_NPA(src.r, src.g, src.b, src.a);
	}

	// assume that dst is premultiplied alpha, and src is premultiplied alpha
	SLIB_INLINE void blend_PA_PA(sl_uint32 _r, sl_uint32 _g, sl_uint32 _b, sl_uint32 _a)
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

	// assume that dst is premultiplied alpha, and src is premultiplied alpha
	SLIB_INLINE void blend_PA_PA(const Color& src)
	{
		blend_PA_NPA(src.r, src.g, src.b, src.a);
	}

	// assume that dst is non-premultiplied alpha, and src is non-premultiplied alpha
	SLIB_INLINE void blend_NPA_NPA(sl_uint32 _r, sl_uint32 _g, sl_uint32 _b, sl_uint32 _a)
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

	// assume that dst is non-premultiplied alpha, and src is non-premultiplied alpha
	SLIB_INLINE void blend_NPA_NPA(const Color& src)
	{
		blend_NPA_NPA(src.r, src.g, src.b, src.a);
	}


	// assume that dst is non-premultiplied alpha, and src is premultiplied alpha
	SLIB_INLINE void blend_NPA_PA(sl_uint32 _r, sl_uint32 _g, sl_uint32 _b, sl_uint32 _a)
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

	// assume that dst is non-premultiplied alpha, and src is premultiplied alpha
	SLIB_INLINE void blend_NPA_PA(const Color& src)
	{
		blend_NPA_PA(src.r, src.g, src.b, src.a);
	}


	SLIB_INLINE void convertNPAtoPA()
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

	SLIB_INLINE void convertPAtoNPA()
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

};

SLIB_GRAPHICS_NAMESPACE_END

#endif
