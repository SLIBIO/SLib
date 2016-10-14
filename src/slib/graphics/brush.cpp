#include "../../../inc/slib/graphics/brush.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

BrushDesc::BrushDesc()
{
    style = BrushStyle::Solid;
    color = Color::White;
}

SLIB_DEFINE_ROOT_OBJECT(Brush)

Brush::Brush()
{
}

Ref<Brush> Brush::getDefault()
{
	SLIB_SAFE_STATIC(Ref<Brush>, defaultBrush, create(BrushDesc()))
	if (SLIB_SAFE_STATIC_CHECK_FREED(defaultBrush)) {
		return Ref<Brush>::null();
	}
	return defaultBrush;
}

Ref<Brush> Brush::create(const BrushDesc& desc)
{
	Ref<Brush> ret = new Brush;
	if (ret.isNotNull()) {
		ret->m_desc = desc;
	}
	return ret;
}

Ref<Brush> Brush::createSolidBrush(const Color& color)
{
	BrushDesc desc;
	desc.color = color;
	return create(desc);
}

void Brush::getDesc(BrushDesc& desc)
{
	desc = m_desc;
}

BrushStyle Brush::getStyle()
{
	return m_desc.style;
}

Color Brush::getColor()
{
	return m_desc.color;
}

SLIB_GRAPHICS_NAMESPACE_END
