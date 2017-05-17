/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_GRAPHICS_COLOR
#define CHECKHEADER_SLIB_GRAPHICS_COLOR

#include "definition.h"

#include "../math/vector3.h"
#include "../math/vector4.h"

#include "../core/string.h"
#include "../core/parse.h"
#include "../core/math.h"

namespace slib
{
	
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
		constexpr Color(): r(0), g(0), b(0), a(0) {}

		constexpr Color(const Color& other):
		 r(other.r), g(other.g), b(other.b), a(other.a)
		{}

		constexpr Color(sl_uint32 _r, sl_uint32 _g, sl_uint32 _b, sl_uint32 _a = 255):
		 r(_r), g(_g), b(_b), a(_a)
		{}

		constexpr Color(sl_uint32 argb):
		 r((sl_uint8)((argb >> 16) & 0xFF)),
		 g((sl_uint8)((argb >> 8) & 0xFF)),
		 b((sl_uint8)(argb & 0xFF)),
		 a((sl_uint8)((argb >> 24) & 0xFF))
		{}

		constexpr Color(const Color3f& v):
		 r((sl_uint8)(Math::clamp0_255((sl_int32)(v.x * 255)))),
		 g((sl_uint8)(Math::clamp0_255((sl_int32)(v.y * 255)))),
		 b((sl_uint8)(Math::clamp0_255((sl_int32)(v.z * 255)))),
		 a(255)
		{}

		constexpr Color(const Color4f& v):
		 r((sl_uint8)(Math::clamp0_255((sl_int32)(v.x * 255)))),
		 g((sl_uint8)(Math::clamp0_255((sl_int32)(v.y * 255)))),
		 b((sl_uint8)(Math::clamp0_255((sl_int32)(v.z * 255)))),
		 a((sl_uint8)(Math::clamp0_255((sl_int32)(v.w * 255))))
		{}
	
	public:
		static const Color& zero();

		constexpr sl_bool isZero() const
		{
			return r == 0 && g == 0 && b == 0 && a == 0;
		}

		constexpr sl_bool isNotZero() const
		{
			return r != 0 || g != 0 || b != 0 || a != 0;
		}

		void setZero();
	
		constexpr float getBlueF() const
		{
			return (float)(b) / 255.0f;
		}

		void setBlueF(float v);

		constexpr float getGreenF() const
		{
			return (float)(g) / 255.0f;
		}

		void setGreenF(float v);

		constexpr float getRedF() const
		{
			return (float)(r) / 255.0f;
		}

		void setRedF(float v);

		constexpr float getAlphaF() const
		{
			return (float)(a) / 255.0f;
		}

		void setAlphaF(float v);

		void setRGBA(sl_uint8 r, sl_uint8 g, sl_uint8 b, sl_uint8 a);
	
		constexpr sl_uint32 getARGB() const
		{
			return ((sl_uint32)(a) << 24) | ((sl_uint32)(r) << 16) | ((sl_uint32)(g) << 8) | ((sl_uint32)(b));
		}
	
		void setARGB(sl_uint32 v);

		constexpr sl_uint32 getABGR() const
		{
			return ((sl_uint32)(a) << 24) | ((sl_uint32)(b) << 16) | ((sl_uint32)(g) << 8) | ((sl_uint32)(r));
		}

		void setABGR(sl_uint32 v);

		constexpr sl_uint32 getRGB() const
		{
			return ((sl_uint32)(r) << 16) | ((sl_uint32)(g) << 8) | ((sl_uint32)(b));
		}
	
		void setRGB(sl_uint8 r, sl_uint8 g, sl_uint8 b);

		void setRGB(sl_uint32 v);
	
		constexpr sl_uint32 getBGR() const
		{
			return ((sl_uint32)(b) << 16) | ((sl_uint32)(g) << 8) | ((sl_uint32)(r));
		}
	
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


		template <class ST>
		static sl_bool parse(const ST& str, Color* _out)
		{
			return Parse(str, _out);
		}

		template <class ST>
		sl_bool parse(const ST& str)
		{
			return Parse(str, this);
		}

	public:
		SLIB_INLINE Color& operator=(const Color& other) = default;

		constexpr sl_bool operator==(const Color& other) const
		{
			return r == other.r && g == other.g && b == other.b && a == other.a;
		}

		constexpr sl_bool operator!=(const Color& other) const
		{
			return r != other.r || g != other.g || b != other.b || a != other.a;
		}

		constexpr operator Color3f() const
		{
			return Color3f((sl_real)(r) / 255, (sl_real)(g) / 255, (sl_real)(b) / 255);
		}

		constexpr operator Color4f() const
		{
			return Color4f((sl_real)(r) / 255, (sl_real)(g) / 255, (sl_real)(b) / 255, (sl_real)(a) / 255);
		}

	private:
		static sl_uint8 _zero[4];

	};
	
	template <>
	sl_reg Parser<Color, sl_char8>::parse(Color* _out, const sl_char8 *sz, sl_size posBegin, sl_size posEnd);
	
	template <>
	sl_reg Parser<Color, sl_char16>::parse(Color* _out, const sl_char16 *sz, sl_size posBegin, sl_size posEnd);
	
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

}

#define SLIB_STATIC_COLOR(name, R, G, B, A) \
	SLIB_ALIGN(8) static sl_uint8 _static_color_buf_##name[] = {R, G, B, A}; \
	Color& name = *(reinterpret_cast<Color*>(_static_color_buf_##name));

#include "detail/color.inc"

#endif
