#include "../../../inc/slib/graphics/brush.h"

#include "../../../inc/slib/core/safe_static.h"

namespace slib
{

	BrushDesc::BrushDesc()
	{
		style = BrushStyle::Solid;
		color = Color::White;
	}

	BrushDesc::~BrushDesc()
	{
	}

	SLIB_DEFINE_ROOT_OBJECT(Brush)

	Brush::Brush()
	{
	}

	Brush::~Brush()
	{
	}

	Ref<Brush> Brush::getDefault()
	{
		SLIB_SAFE_STATIC(Ref<Brush>, defaultBrush, create(BrushDesc()))
		if (SLIB_SAFE_STATIC_CHECK_FREED(defaultBrush)) {
			return sl_null;
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

}
