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

Ref<Drawable> Drawable::subDrawable(sl_real x, sl_real y, sl_real width, sl_real height)
{
	return SubDrawable::create(this, x, y, width, height);
}

Ref<Drawable> Drawable::scaleDrawable(sl_real width, sl_real height)
{
	return ScaledDrawable::create(this, width, height);
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

Ref<Drawable> Drawable::createSubDrawable(const Ref<Drawable>& src, sl_real x, sl_real y, sl_real width, sl_real height)
{
	if (src.isNotNull()) {
		return src->subDrawable(x, y, width, height);
	}
	return Ref<Drawable>::null();
}

Ref<Drawable> Drawable::createSubDrawable(const Ref<Drawable>& src, const Rectangle& rectSrc)
{
	if (src.isNotNull()) {
		return src->subDrawable(rectSrc.left, rectSrc.top, rectSrc.getWidth(), rectSrc.getHeight());
	}
	return Ref<Drawable>::null();
}

Ref<Drawable> Drawable::createScaledDrawable(const Ref<Drawable>& src, sl_real width, sl_real height)
{
	if (src.isNotNull()) {
		return src->scaleDrawable(width, height);
	}
	return Ref<Drawable>::null();
}

Ref<Drawable> Drawable::createScaledDrawable(const Ref<Drawable>& src, const Size& size)
{
	if (src.isNotNull()) {
		return src->scaleDrawable(size.x, size.y);
	}
	return Ref<Drawable>::null();
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


SLIB_DEFINE_OBJECT(SubDrawable, Drawable)

Ref<SubDrawable> SubDrawable::create(const Ref<Drawable>& src, sl_real x, sl_real y, sl_real width, sl_real height)
{
	if (src.isNotNull() && width > 0 && height > 0) {
		Ref<SubDrawable> sub = new SubDrawable;
		if (sub.isNotNull()) {
			sub->m_src = src;
			sub->m_x = x;
			sub->m_y = y;
			sub->m_width = width;
			sub->m_height = height;
			return sub;
		}
	}
	return Ref<SubDrawable>::null();
}

sl_real SubDrawable::getDrawableWidth()
{
	return m_width;
}

sl_real SubDrawable::getDrawableHeight()
{
	return m_height;
}

Ref<Drawable> SubDrawable::subDrawable(sl_real x, sl_real y, sl_real width, sl_real height)
{
	return SubDrawable::create(m_src, m_x + x, m_y + y, width, height);
}

Ref<Drawable> SubDrawable::scaleDrawable(sl_real width, sl_real height)
{
	return ScaledSubDrawable::create(m_src, Rectangle(m_x, m_y, m_x + m_width, m_y + m_height), width, height);
}

void SubDrawable::onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& _rectSrc)
{
	Rectangle rectSrc;
	rectSrc.left = m_x + _rectSrc.left;
	rectSrc.top = m_y + _rectSrc.top;
	rectSrc.right = m_x + _rectSrc.right;
	rectSrc.bottom = m_y + _rectSrc.bottom;
	canvas->draw(rectDst, m_src, rectSrc);
}

void SubDrawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst)
{
	canvas->draw(rectDst, m_src, Rectangle(m_x, m_y, m_x + m_width, m_y + m_height));
}


SLIB_DEFINE_OBJECT(ScaledDrawable, Drawable)

Ref<ScaledDrawable> ScaledDrawable::create(const Ref<Drawable>& src, sl_real width, sl_real height)
{
	if (src.isNotNull() && width > 0 && height > 0) {
		Ref<ScaledDrawable> ret = new ScaledDrawable;
		if (ret.isNotNull()) {
			ret->m_src = src;
			ret->m_width = width;
			ret->m_height = height;
			return ret;
		}
	}
	return Ref<ScaledDrawable>::null();
}

sl_real ScaledDrawable::getDrawableWidth()
{
	return m_width;
}

sl_real ScaledDrawable::getDrawableHeight()
{
	return m_height;
}

Ref<Drawable> ScaledDrawable::subDrawable(sl_real x, sl_real y, sl_real width, sl_real height)
{
	sl_real rsx = m_src->getDrawableWidth() / m_width;
	sl_real rsy = m_src->getDrawableHeight() / m_height;
	return ScaledSubDrawable::create(m_src, Rectangle(rsx * x, rsy * y, rsx * (x + width), rsy * (y + height)), width, height);
}

Ref<Drawable> ScaledDrawable::scaleDrawable(sl_real width, sl_real height)
{
	return ScaledDrawable::create(m_src, width, height);
}

void ScaledDrawable::onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& _rectSrc)
{
	Rectangle rectSrc;
	sl_real rsx = m_src->getDrawableWidth() / m_width;
	sl_real rsy = m_src->getDrawableHeight() / m_height;
	rectSrc.left = rsx * _rectSrc.left;
	rectSrc.top = rsy * _rectSrc.top;
	rectSrc.right = rsx * _rectSrc.right;
	rectSrc.bottom = rsy * _rectSrc.bottom;
	canvas->draw(rectDst, m_src, rectSrc);
}

void ScaledDrawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst)
{
	canvas->draw(rectDst, m_src);
}


SLIB_DEFINE_OBJECT(ScaledSubDrawable, Drawable)

Ref<ScaledSubDrawable> ScaledSubDrawable::create(const Ref<Drawable>& src, const Rectangle& rectSrc, sl_real width, sl_real height)
{
	if (src.isNotNull() && width > 0 && height > 0 && rectSrc.getWidth() > 0 && rectSrc.getHeight() > 0) {
		Ref<ScaledSubDrawable> ret = new ScaledSubDrawable;
		if (ret.isNotNull()) {
			ret->m_src = src;
			ret->m_rectSrc = rectSrc;
			ret->m_width = width;
			ret->m_height = height;
			return ret;
		}
	}
	return Ref<ScaledSubDrawable>::null();
}

sl_real ScaledSubDrawable::getDrawableWidth()
{
	return m_width;
}

sl_real ScaledSubDrawable::getDrawableHeight()
{
	return m_height;
}

Ref<Drawable> ScaledSubDrawable::subDrawable(sl_real x, sl_real y, sl_real width, sl_real height)
{
	sl_real rsx = m_rectSrc.getWidth() / m_width;
	sl_real rsy = m_rectSrc.getHeight() / m_height;
	Rectangle rectSrc;
	rectSrc.left = rsx * x + m_rectSrc.left;
	rectSrc.top = rsy * y + m_rectSrc.top;
	rectSrc.right = rectSrc.left + rsx * width;
	rectSrc.bottom = rectSrc.top + rsy * height;
	return ScaledSubDrawable::create(m_src, rectSrc, width, height);
}

Ref<Drawable> ScaledSubDrawable::scaleDrawable(sl_real width, sl_real height)
{
	return ScaledSubDrawable::create(m_src, m_rectSrc, width, height);
}

void ScaledSubDrawable::onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& _rectSrc)
{
	sl_real rsx = m_rectSrc.getWidth() / m_width;
	sl_real rsy = m_rectSrc.getHeight() / m_height;
	Rectangle rectSrc;
	rectSrc.left = rsx * _rectSrc.left + m_rectSrc.left;
	rectSrc.top = rsy * _rectSrc.top + m_rectSrc.top;
	rectSrc.right = rsx * _rectSrc.right + m_rectSrc.left;
	rectSrc.bottom = rsy * _rectSrc.bottom + m_rectSrc.top;
	canvas->draw(rectDst, m_src, rectSrc);
}

void ScaledSubDrawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst)
{
	canvas->draw(rectDst, m_src, m_rectSrc);
}

SLIB_GRAPHICS_NAMESPACE_END
