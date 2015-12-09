#include "../../../inc/slib/graphics/color.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

sl_uint8 Color::_zero[4] = {0, 0, 0, 0};
sl_uint8 Color::_transparent[4] = {255, 255, 255, 0};
sl_uint8 Color::_black[4] = {0, 0, 0, 255};
sl_uint8 Color::_white[4] = {255, 255, 255, 255};
sl_uint8 Color::_gray[4] = {128, 128, 128, 255};
sl_uint8 Color::_red[4] = {255, 0, 0, 255};
sl_uint8 Color::_green[4] = {0, 255, 0, 255};
sl_uint8 Color::_blue[4] = {0, 0, 255, 255};
sl_uint8 Color::_yellow[4] = {255, 255, 0, 255};
sl_uint8 Color::_magenta[4] = {255, 0, 255, 255};
sl_uint8 Color::_cyan[4] = {0, 255, 255, 255};

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

SLIB_GRAPHICS_NAMESPACE_END

