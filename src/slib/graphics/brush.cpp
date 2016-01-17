#include "../../../inc/slib/graphics/brush.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

BrushDesc::BrushDesc()
{
    style = brushStyle_Solid;
    color = Color::white();
}

Ref<Brush> Brush::getDefault()
{
	SLIB_SAFE_STATIC(Ref<Brush>, defaultBrush, create(BrushDesc()));
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

SLIB_GRAPHICS_NAMESPACE_END
