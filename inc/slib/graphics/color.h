#ifndef CHECKHEADER_SLIB_GRAPHICS_COLOR
#define CHECKHEADER_SLIB_GRAPHICS_COLOR

#include "definition.h"

#include "../math/vector3.h"
#include "../math/vector4.h"

#include "../core/string.h"
#include "../core/parse.h"

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
		YellowGreen = 0xFF9ACD32,
		Zero = 0,
		None = 0
	};

public:
	SLIB_INLINE Color() = default;
	
	SLIB_INLINE Color(const Color& other) = default;
	
	Color(sl_uint32 r, sl_uint32 g, sl_uint32 b, sl_uint32 a = 255);
	
	Color(sl_uint32 argb);

	Color(const Color3f& v);
	
	Color(const Color4f& v);
	
public:
	static const Color& zero();
	
	sl_bool isZero() const;
	
	sl_bool isNotZero() const;
	
	void setZero();

	float getBlueF() const;
	
	void setBlueF(float v);
	
	float getGreenF() const;
	
	void setGreenF(float v);
	
	float getRedF() const;
	
	void setRedF(float v);
	
	float getAlphaF() const;
	
	void setAlphaF(float v);
	
	void setRGBA(sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a);

	sl_uint32 getARGB() const;
	
	void setARGB(sl_uint32 v);

	sl_uint32 getABGR() const;
	
	void setABGR(sl_uint32 v);

	void setRGB(sl_uint8 r, sl_uint8 g, sl_uint8 b);
	
	sl_uint32 getRGB() const;
	
	void setRGB(sl_uint32 v);

	sl_uint32 getBGR() const;
	
	void setBGR(sl_uint32 v);
	
	
	// assume that` dst is premultiplied alpha, and src is non-premultiplied alpha
	void blend_PA_NPA(sl_uint32 r, sl_uint32 g, sl_uint32 b, sl_uint32 a);

	// assume that dst is premultiplied alpha, and src is non-premultiplied alpha
	void blend_PA_NPA(const Color& src);

	// assume that dst is premultiplied alpha, and src is premultiplied alpha
	void blend_PA_PA(sl_uint32 r, sl_uint32 g, sl_uint32 b, sl_uint32 a);

	// assume that dst is premultiplied alpha, and src is premultiplied alpha
	void blend_PA_PA(const Color& src);

	// assume that dst is non-premultiplied alpha, and src is non-premultiplied alpha
	void blend_NPA_NPA(sl_uint32 r, sl_uint32 g, sl_uint32 b, sl_uint32 a);
	
	// assume that dst is non-premultiplied alpha, and src is non-premultiplied alpha
	void blend_NPA_NPA(const Color& src);

	// assume that dst is non-premultiplied alpha, and src is premultiplied alpha
	void blend_NPA_PA(sl_uint32 r, sl_uint32 g, sl_uint32 b, sl_uint32 a);

	// assume that dst is non-premultiplied alpha, and src is premultiplied alpha
	void blend_NPA_PA(const Color& src);


	void convertNPAtoPA();

	void convertPAtoNPA();
	
	
	String toString() const;
	
	SLIB_DECLARE_PARSE_FUNCTIONS(Color)
	
public:
	SLIB_INLINE Color& operator=(const Color& other) = default;
	
	sl_bool operator==(const Color& other) const;
	
	sl_bool operator!=(const Color& other) const;

	operator Color3f() const;
	
	operator Color4f() const;
	
private:
	static sl_uint8 _zero[4];
	
};

#define SLIB_STATIC_COLOR(name, R, G, B, A) \
	SLIB_ALIGN(8) static sl_uint8 _static_color_buf_##name[] = {R, G, B, A}; \
	Color& name = *((Color*)((void*)_static_color_buf_##name));

class SLIB_EXPORT ColorMatrix
{
public:
	Color4f red;
	Color4f green;
	Color4f blue;
	Color4f alpha;
	Color4f bias;
	
public:
	/*
	 d.r = dot(s, red)
	 d.g = dot(s, green)
	 d.b = dot(s, blue)
	 d.a = dot(s, alpha)
	 d = d + bias
	 */
	Color4f transformColor(const Color4f& s) const;
	Color4f transformColor(const Color& s) const;	
	Color4f transformColor(const Color3f& s) const;
	
};

SLIB_GRAPHICS_NAMESPACE_END


SLIB_GRAPHICS_NAMESPACE_BEGIN

SLIB_INLINE Color::Color(sl_uint32 _r, sl_uint32 _g, sl_uint32 _b, sl_uint32 _a) : r(_r), g(_g), b(_b), a(_a)
{
}

SLIB_INLINE Color::Color(sl_uint32 argb)
{
	b = (sl_uint8)(argb & 0xFF);
	g = (sl_uint8)((argb >> 8) & 0xFF);
	r = (sl_uint8)((argb >> 16) & 0xFF);
	a = (sl_uint8)((argb >> 24) & 0xFF);
}

SLIB_INLINE const Color& Color::zero()
{
	return *((Color*)((void*)(&_zero)));
}

SLIB_INLINE sl_bool Color::isZero() const
{
	return r == 0 && g == 0 && b == 0 && a == 0;
}

SLIB_INLINE sl_bool Color::isNotZero() const
{
	return r != 0 || g != 0 || b != 0 || a != 0;
}

SLIB_INLINE void Color::setZero()
{
	r = 0;
	g = 0;
	b = 0;
	a = 0;
}

SLIB_INLINE void Color::setRGBA(sl_uint8 _r, sl_uint8 _g, sl_uint8 _b, sl_uint8 _a)
{
	r = _r;
	g = _g;
	b = _b;
	a = _a;
}

SLIB_INLINE sl_uint32 Color::getARGB() const
{
	return ((sl_uint32)(a) << 24) | ((sl_uint32)(r) << 16) | ((sl_uint32)(g) << 8) | ((sl_uint32)(b));
}

SLIB_INLINE void Color::setARGB(sl_uint32 v)
{
	b = (sl_uint8)(v & 0xFF);
	g = (sl_uint8)((v >> 8) & 0xFF);
	r = (sl_uint8)((v >> 16) & 0xFF);
	a = (sl_uint8)((v >> 24) & 0xFF);
}

SLIB_INLINE sl_uint32 Color::getABGR() const
{
	return ((sl_uint32)(a) << 24) | ((sl_uint32)(b) << 16) | ((sl_uint32)(g) << 8) | ((sl_uint32)(r));
}

SLIB_INLINE void Color::setABGR(sl_uint32 v)
{
	r = (sl_uint8)(v & 0xFF);
	g = (sl_uint8)((v >> 8) & 0xFF);
	b = (sl_uint8)((v >> 16) & 0xFF);
	a = (sl_uint8)((v >> 24) & 0xFF);
}

SLIB_INLINE void Color::setRGB(sl_uint8 _r, sl_uint8 _g, sl_uint8 _b)
{
	r = _r;
	g = _g;
	b = _b;
}

SLIB_INLINE sl_uint32 Color::getRGB() const
{
	return ((sl_uint32)(r) << 16) | ((sl_uint32)(g) << 8) | ((sl_uint32)(b));
}

SLIB_INLINE void Color::setRGB(sl_uint32 v)
{
	b = (sl_uint8)(v & 0xFF);
	g = (sl_uint8)((v >> 8) & 0xFF);
	r = (sl_uint8)((v >> 16) & 0xFF);
}

SLIB_INLINE sl_uint32 Color::getBGR() const
{
	return ((sl_uint32)(b) << 16) | ((sl_uint32)(g) << 8) | ((sl_uint32)(r));
}

SLIB_INLINE void Color::setBGR(sl_uint32 v)
{
	r = (sl_uint8)(v & 0xFF);
	g = (sl_uint8)((v >> 8) & 0xFF);
	b = (sl_uint8)((v >> 16) & 0xFF);
}

SLIB_INLINE sl_bool Color::operator==(const Color& other) const
{
	return r == other.r && g == other.g && b == other.b && a == other.a;
}

SLIB_INLINE sl_bool Color::operator!=(const Color& other) const
{
	return r != other.r || g != other.g || b != other.b || a != other.a;
}

SLIB_GRAPHICS_NAMESPACE_END

#endif
