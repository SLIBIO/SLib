#include "../../../inc/slib/graphics/drawable.h"
#include "../../../inc/slib/graphics/canvas.h"
#include "../../../inc/slib/graphics/bitmap.h"
#include "../../../inc/slib/graphics/image.h"
#include "../../../inc/slib/graphics/util.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(Drawable, Object)

sl_real Drawable::getDrawableWidth()
{
	return 1;
}

sl_real Drawable::getDrawableHeight()
{
	return 1;
}

void Drawable::onDraw(Canvas* canvas, const Rectangle& _rectDst, const Rectangle& rectSrc)
{
	Rectangle rectDst = GraphicsUtil::transformRectangle(_rectDst, rectSrc, Rectangle(0, 0, getDrawableWidth(), getDrawableHeight()));
	onDrawAll(canvas, rectDst);
}

void Drawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst)
{
	onDraw(canvas, rectDst, Rectangle(0, 0, getDrawableWidth(), getDrawableHeight()));
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

Ref<Drawable> Drawable::createEmptyDrawable()
{
	return EmptyDrawable::create();
}


SLIB_DEFINE_OBJECT(BrushDrawable, Drawable)

Ref<BrushDrawable> BrushDrawable::create(const Ref<Brush>& brush)
{
	if (brush.isNotNull()) {
		Ref<BrushDrawable> ret = new BrushDrawable;
		if (ret.isNotNull()) {
			ret->m_brush = brush;
			return ret;
		}
	}
	return Ref<BrushDrawable>::null();
}

void BrushDrawable::onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc)
{
	canvas->drawRectangle(rectDst, Ref<Pen>::null(), m_brush);
}


SLIB_DEFINE_OBJECT(ColorDrawable, BrushDrawable)

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


SLIB_DEFINE_OBJECT(EmptyDrawable, Drawable)

Ref<EmptyDrawable> EmptyDrawable::create()
{
	return new EmptyDrawable();
}

void EmptyDrawable::onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc)
{
}


SLIB_DEFINE_OBJECT(RectangleDrawable, Drawable)

Ref<RectangleDrawable> RectangleDrawable::create(const Ref<Pen>& pen, const Ref<Brush>& brush)
{
	if (brush.isNotNull()) {
		Ref<RectangleDrawable> ret = new RectangleDrawable;
		if (ret.isNotNull()) {
			ret->m_pen = pen;
			ret->m_brush = brush;
			return ret;
		}
	}
	return Ref<RectangleDrawable>::null();
}

void RectangleDrawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst)
{
	canvas->drawRectangle(rectDst, m_pen, m_brush);
}


SLIB_GRAPHICS_NAMESPACE_END
