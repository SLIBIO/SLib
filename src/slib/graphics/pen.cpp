#include "../../../inc/slib/graphics/pen.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

PenDesc::PenDesc()
{
    width = 1;
    color = Color::black();

    style = penStyle_Solid;
    cap = lineCap_Flat;
    join = lineJoin_Miter;
    miterLimit = 10;
}

Ref<Pen> Pen::getDefault()
{
	SLIB_SAFE_STATIC(Ref<Pen>, defaultPen, create(PenDesc()));
	return defaultPen;
}

Ref<Pen> Pen::create(const PenDesc& desc)
{
	Ref<Pen> ret = new Pen;
	if (ret.isNotNull()) {
		ret->m_desc = desc;
	}
	return ret;
}

Ref<Pen> Pen::create(PenStyle style, sl_real width, Color color)
{
	PenDesc desc;
	desc.style = style;
	desc.width = width;
	desc.color = color;
	return create(desc);
}

Ref<Pen> Pen::createSolidPen(sl_real width, Color color)
{
	PenDesc desc;
	desc.width = width;
	desc.color = color;
	return create(desc);
}

SLIB_GRAPHICS_NAMESPACE_END


