#include "../../../inc/slib/graphics/brush.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN
BrushDesc::BrushDesc()
{
    style = brushStyleSolid;
    color = Color::white();
}
    
Brush::Brush()
{
}

Ref<Brush> Brush::getDefault()
{
	SLIB_SAFE_STATIC_REF(Ref<Brush>, defaultBrush);
	if (defaultBrush.isNull()) {
		BrushDesc desc;
		defaultBrush = create(desc);
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

Ref<Brush> Brush::createSolidBrush(Color color)
{
	BrushDesc desc;
	desc.color = color;
	return create(desc);
}

const Ref<BrushInstance>& Brush::getInstance()
{
	return m_instance;
}

void Brush::setInstance(const Ref<BrushInstance>& instance)
{
	m_instance = instance;
}

BrushInstance::BrushInstance()
{
}
SLIB_GRAPHICS_NAMESPACE_END
