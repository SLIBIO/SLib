#include "../../../inc/slib/graphics/color.h"

#include "../../../inc/slib/core/math.h"

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

