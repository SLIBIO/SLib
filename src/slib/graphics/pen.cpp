#include "../../../inc/slib/graphics/pen.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN
PenDesc::PenDesc()
{
    width = 1;
    color = Color::black();

    style = penStyleSolid;
    cap = lineCapFlat;
    join = lineJoinMiter;
    miterLimit = 10;
}

Pen::Pen()
{
}

Ref<Pen> Pen::getDefault()
{
	SLIB_SAFE_STATIC_REF(Ref<Pen>, defaultPen);
	if (defaultPen.isNull()) {
		PenDesc desc;
		defaultPen = create(desc);
	}
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

const Ref<PenInstance>& Pen::getInstance()
{
	return m_instance;
}

void Pen::setInstance(const Ref<PenInstance>& instance)
{
	m_instance = instance;
}

PenInstance::PenInstance()
{	
}
SLIB_GRAPHICS_NAMESPACE_END


