#include "../../../inc/slib/graphics/pen.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

PenDesc::PenDesc()
{
    width = 1;
    color = Color::Black;

    style = PenStyle::Solid;
    cap = LineCap::Flat;
    join = LineJoin::Miter;
    miterLimit = 10;
}


SLIB_DEFINE_OBJECT(Pen, Object)

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

void Pen::getDesc(PenDesc& desc)
{
	desc = m_desc;
}

PenStyle Pen::getStyle()
{
	return m_desc.style;
}

sl_real Pen::getWidth()
{
	return m_desc.width;
}

Color Pen::getColor()
{
	return m_desc.color;
}

LineCap Pen::getCap()
{
	return m_desc.cap;
}

LineJoin Pen::getJoin()
{
	return m_desc.join;
}

sl_real Pen::getMiterLimit()
{
	return m_desc.miterLimit;
}

SLIB_DEFINE_OBJECT(PenInstance, Object)

SLIB_GRAPHICS_NAMESPACE_END


