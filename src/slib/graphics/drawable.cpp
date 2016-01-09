#include "../../../inc/slib/graphics/drawable.h"
#include "../../../inc/slib/graphics/canvas.h"
#include "../../../inc/slib/graphics/bitmap.h"
#include "../../../inc/slib/graphics/image.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN
Drawable::Drawable()
{
}

sl_bool Drawable::isBitmap()
{
	return Bitmap::checkInstance(this);
}

sl_bool Drawable::isImage()
{
	return Image::checkInstance(this);
}

Ref<BrushDrawable> Drawable::createBrushDrawable(const Ref<Brush>& brush)
{
	return BrushDrawable::create(brush);
}

Ref<ColorDrawable> Drawable::createColorDrawable(const Color& color)
{
	return ColorDrawable::create(color);
}

BrushDrawable::BrushDrawable()
{
}

Ref<BrushDrawable> BrushDrawable::create(const Ref<Brush>& brush)
{
	Ref<BrushDrawable> ret;
	if (brush.isNull()) {
		return ret;
	}
	ret = new BrushDrawable();
	if (ret.isNotNull()) {
		ret->m_brush = brush;
	}
	return ret;
}

sl_real BrushDrawable::getDrawableWidth()
{
	return 1;
}

sl_real BrushDrawable::getDrawableHeight()
{
	return 1;
}

void BrushDrawable::onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc)
{
	Ref<Brush> brush = m_brush;
	if (brush.isNotNull()) {
		canvas->drawRectangle(rectDst, Ref<Pen>::null(), brush);
	}
}

ColorDrawable::ColorDrawable()
{
}

Ref<ColorDrawable> ColorDrawable::create(const Color& color)
{
	Ref<ColorDrawable> ret;
	Ref<Brush> brush = Brush::createSolidBrush(color);
	if (brush.isNull()) {
		return ret;
	}
	ret = new ColorDrawable();
	if (ret.isNotNull()) {
		ret->m_brush = brush;
	}
	return ret;
}

EmptyDrawable::EmptyDrawable()
{
}

Ref<EmptyDrawable> EmptyDrawable::create()
{
	Ref<EmptyDrawable> ret;
	ret = new EmptyDrawable();
	return ret;
}

sl_real EmptyDrawable::getDrawableWidth()
{
	return 1;
}

sl_real EmptyDrawable::getDrawableHeight()
{
	return 1;
}

void EmptyDrawable::onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc)
{
}

Ref<Drawable> Drawable::createEmptyDrawable()
{
	return EmptyDrawable::create();
}
SLIB_GRAPHICS_NAMESPACE_END
