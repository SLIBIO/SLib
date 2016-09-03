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

Ref<Drawable> Drawable::createBrushDrawable(const Ref<Brush>& brush)
{
	return BrushDrawable::create(brush);
}

Ref<Drawable> Drawable::createColorDrawable(const Color& color)
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

Ref<Drawable> BrushDrawable::create(const Ref<Brush>& brush)
{
	if (brush.isNotNull()) {
		Ref<BrushDrawable> ret = new BrushDrawable;
		if (ret.isNotNull()) {
			ret->m_brush = brush;
			return ret;
		}
	}
	return Ref<Drawable>::null();
}

void BrushDrawable::onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc)
{
	canvas->drawRectangle(rectDst, Ref<Pen>::null(), m_brush);
}


SLIB_DEFINE_OBJECT(ColorDrawable, BrushDrawable)

Ref<Drawable> ColorDrawable::create(const Color& color)
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

Ref<Drawable> EmptyDrawable::create()
{
	return new EmptyDrawable();
}

void EmptyDrawable::onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc)
{
}


SLIB_DEFINE_OBJECT(RectangleDrawable, Drawable)

Ref<Drawable> RectangleDrawable::create(const Ref<Pen>& pen, const Ref<Brush>& brush)
{
	if (brush.isNotNull()) {
		Ref<RectangleDrawable> ret = new RectangleDrawable;
		if (ret.isNotNull()) {
			ret->m_pen = pen;
			ret->m_brush = brush;
			return ret;
		}
	}
	return Ref<Drawable>::null();
}

void RectangleDrawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst)
{
	canvas->drawRectangle(rectDst, m_pen, m_brush);
}


SLIB_DEFINE_OBJECT(SubDrawable, Drawable)

Ref<Drawable> SubDrawable::create(const Ref<Drawable>& src, sl_real x, sl_real y, sl_real width, sl_real height)
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
	return Ref<Drawable>::null();
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

Ref<Drawable> ScaledDrawable::create(const Ref<Drawable>& src, sl_real width, sl_real height)
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
	return Ref<Drawable>::null();
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

Ref<Drawable> ScaledSubDrawable::create(const Ref<Drawable>& src, const Rectangle& rectSrc, sl_real width, sl_real height)
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
	return Ref<Drawable>::null();
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


SLIB_DEFINE_OBJECT(NinePiecesDrawable, Drawable)

Ref<Drawable> NinePiecesDrawable::create(sl_real leftWidth, sl_real rightWidth, sl_real topHeight, sl_real bottomHeight,
									 const Ref<Drawable>& topLeft, const Ref<Drawable>& top, const Ref<Drawable>& topRight,
									 const Ref<Drawable>& left, const Ref<Drawable>& center, const Ref<Drawable>& right,
									 const Ref<Drawable>& bottomLeft, const Ref<Drawable>& bottom, const Ref<Drawable>& bottomRight)
{
	Ref<NinePiecesDrawable> ret = new NinePiecesDrawable;
	if (ret.isNotNull()) {
		if (leftWidth < 0) {
			leftWidth = 0;
		}
		if (rightWidth < 0) {
			rightWidth = 0;
		}
		if (topHeight < 0) {
			topHeight = 0;
		}
		if (bottomHeight < 0) {
			bottomHeight = 0;
		}
		ret->m_widthLeft = leftWidth;
		ret->m_widthRight = rightWidth;
		ret->m_heightTop = topHeight;
		ret->m_heightBottom = bottomHeight;
		ret->m_partTopLeft = topLeft;
		ret->m_partTop = top;
		ret->m_partTopRight = topRight;
		ret->m_partLeft = left;
		ret->m_partCenter = center;
		ret->m_partRight = right;
		ret->m_partBottomLeft = bottomLeft;
		ret->m_partBottom = bottom;
		ret->m_partTopRight = bottomRight;
		return ret;
	}
	return Ref<Drawable>::null();
}

void NinePiecesDrawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst)
{
	sl_real widthLeft = m_widthLeft;
	sl_real widthRight = m_widthRight;
	sl_real heightTop = m_heightTop;
	sl_real heightBottom = m_heightBottom;
	sl_real widthDst = rectDst.getWidth();
	sl_real heightDst = rectDst.getHeight();
	if (widthDst < widthLeft + widthRight || heightDst < heightTop + heightBottom) {
		canvas->draw(rectDst, m_partCenter);
		return;
	}
	if (heightTop > 0) {
		if (widthLeft > 0 && m_partTopLeft.isNotNull()) {
			canvas->draw(Rectangle(rectDst.left, rectDst.top, rectDst.left + widthLeft, rectDst.top + heightTop), m_partTopLeft);
		}
		if (m_partTop.isNotNull()) {
			canvas->draw(Rectangle(rectDst.left + widthLeft, rectDst.top, rectDst.right - widthRight, rectDst.top + heightTop), m_partTop);
		}
		if (widthRight > 0 && m_partTopRight.isNotNull()) {
			canvas->draw(Rectangle(rectDst.right - widthRight, rectDst.top, rectDst.right, rectDst.top + heightTop), m_partTopRight);
		}
	}
	if (heightBottom > 0) {
		if (widthLeft > 0 && m_partBottomLeft.isNotNull()) {
			canvas->draw(Rectangle(rectDst.left, rectDst.bottom - heightBottom, rectDst.left + widthLeft, rectDst.bottom), m_partBottomLeft);
		}
		if (m_partBottom.isNotNull()) {
			canvas->draw(Rectangle(rectDst.left + widthLeft, rectDst.bottom - heightBottom, rectDst.right - widthRight, rectDst.bottom), m_partBottom);
		}
		if (widthRight > 0 && m_partBottomRight.isNotNull()) {
			canvas->draw(Rectangle(rectDst.right - widthRight, rectDst.bottom - heightBottom, rectDst.right, rectDst.bottom), m_partBottomRight);
		}
	}
	if (widthLeft > 0 && m_partLeft.isNotNull()) {
		canvas->draw(Rectangle(rectDst.left, rectDst.top + heightTop, rectDst.left + widthLeft, rectDst.bottom - heightBottom), m_partLeft);
	}
	if (m_partCenter.isNotNull()) {
		canvas->draw(Rectangle(rectDst.left + widthLeft, rectDst.top + heightTop, rectDst.right - widthRight, rectDst.bottom - heightBottom), m_partCenter);
	}
	if (widthRight > 0 && m_partRight.isNotNull()) {
		canvas->draw(Rectangle(rectDst.right - widthRight, rectDst.top + heightTop, rectDst.right, rectDst.bottom - heightBottom), m_partRight);
	}
}


SLIB_DEFINE_OBJECT(NinePatchDrawable, Drawable)

Ref<Drawable> NinePatchDrawable::create(sl_real leftWidthDst, sl_real rightWidthDst, sl_real topHeightDst, sl_real bottomHeightDst,
				const Ref<Drawable>& src, sl_real leftWidthSrc, sl_real rightWidthSrc, sl_real topHeightSrc, sl_real bottomHeightSrc)
{
	if (src.isNull()) {
		return Ref<Drawable>::null();
	}
	
	if (leftWidthSrc + rightWidthSrc + SLIB_EPSILON > src->getDrawableWidth()) {
		return Ref<Drawable>::null();
	}
	if (topHeightSrc + bottomHeightSrc + SLIB_EPSILON > src->getDrawableWidth()) {
		return Ref<Drawable>::null();
	}
	
	Ref<NinePatchDrawable> ret = new NinePatchDrawable;
	
	if (ret.isNotNull()) {
		if (leftWidthDst < 0) {
			leftWidthDst = 0;
		}
		if (rightWidthDst < 0) {
			rightWidthDst = 0;
		}
		if (topHeightDst < 0) {
			topHeightDst = 0;
		}
		if (bottomHeightDst < 0) {
			bottomHeightDst = 0;
		}
		if (leftWidthSrc < 0) {
			leftWidthSrc = 0;
		}
		if (rightWidthSrc < 0) {
			rightWidthSrc = 0;
		}
		if (topHeightSrc < 0) {
			topHeightSrc = 0;
		}
		if (bottomHeightSrc < 0) {
			bottomHeightSrc = 0;
		}

		ret->m_widthLeftDst = leftWidthDst;
		ret->m_widthRightDst = rightWidthDst;
		ret->m_heightTopDst = topHeightDst;
		ret->m_heightBottomDst = bottomHeightDst;
		ret->m_src = src;
		ret->m_widthLeftSrc = leftWidthSrc;
		ret->m_widthRightSrc = rightWidthSrc;
		ret->m_heightTopSrc = topHeightSrc;
		ret->m_heightBottomSrc = bottomHeightSrc;
		
		return ret;
		
	}
	return Ref<Drawable>::null();
}

Ref<Drawable> NinePatchDrawable::create(const Ref<Drawable>& src, sl_real leftWidthSrc, sl_real rightWidthSrc, sl_real topHeightSrc, sl_real bottomHeightSrc)
{
	return create(leftWidthSrc, rightWidthSrc, topHeightSrc, bottomHeightSrc, src, leftWidthSrc, rightWidthSrc, topHeightSrc, bottomHeightSrc);
}

void NinePatchDrawable::onDrawAll(Canvas* canvas, const Rectangle& rectDst)
{
	Ref<Drawable> src = m_src;
	if (src.isNull()) {
		return;
	}
	sl_real widthDst = rectDst.getWidth();
	sl_real heightDst = rectDst.getHeight();
	sl_real widthLeftDst = m_widthLeftDst;
	sl_real widthRightDst = m_widthRightDst;
	sl_real heightTopDst = m_heightTopDst;
	sl_real heightBottomDst = m_heightBottomDst;
	if (widthDst < widthLeftDst + widthRightDst || heightDst < heightTopDst + heightBottomDst) {
		canvas->draw(rectDst, src);
		return;
	}
	sl_real widthLeftSrc = m_widthLeftSrc;
	sl_real widthRightSrc = m_widthRightSrc;
	sl_real heightTopSrc = m_heightTopSrc;
	sl_real heightBottomSrc = m_heightBottomSrc;
	sl_real widthSrc = src->getDrawableWidth();
	sl_real heightSrc = src->getDrawableHeight();
	if (widthSrc < widthLeftSrc + widthRightSrc || heightSrc < heightTopSrc + heightBottomSrc) {
		canvas->draw(rectDst, src);
		return;
	}
	if (heightTopDst > 0 && heightTopSrc > 0) {
		sl_real topDst = rectDst.top;
		sl_real bottomDst = rectDst.top + heightTopDst;
		sl_real topSrc = 0;
		sl_real bottomSrc = heightTopSrc;
		if (widthLeftDst > 0 && widthLeftSrc > 0) {
			canvas->draw(Rectangle(rectDst.left, topDst, rectDst.left + widthLeftDst, bottomDst), src, Rectangle(0, topSrc, widthLeftSrc, bottomSrc));
		}
		canvas->draw(Rectangle(rectDst.left + widthLeftDst, topDst, rectDst.right - widthRightDst, bottomDst), src, Rectangle(widthLeftSrc, topSrc, widthSrc - widthRightSrc, bottomSrc));
		if (widthRightDst > 0 && widthRightSrc > 0) {
			canvas->draw(Rectangle(rectDst.right - widthRightDst, topDst, rectDst.right, bottomDst), src, Rectangle(widthSrc - widthRightSrc, topSrc, widthSrc, bottomSrc));
		}
	}
	if (heightBottomDst > 0 && heightBottomSrc > 0) {
		sl_real topDst = rectDst.bottom - heightBottomDst;
		sl_real bottomDst = rectDst.bottom;
		sl_real topSrc = heightSrc - heightBottomSrc;
		sl_real bottomSrc = heightSrc;
		if (widthLeftDst > 0 && widthLeftSrc > 0) {
			canvas->draw(Rectangle(rectDst.left, topDst, rectDst.left + widthLeftDst, bottomDst), src, Rectangle(0, topSrc, widthLeftSrc, bottomSrc));
		}
		canvas->draw(Rectangle(rectDst.left + widthLeftDst, topDst, rectDst.right - widthRightDst, bottomDst), src, Rectangle(widthLeftSrc, topSrc, widthSrc - widthRightSrc, bottomSrc));
		if (widthRightDst > 0 && widthRightSrc > 0) {
			canvas->draw(Rectangle(rectDst.right - widthRightDst, topDst, rectDst.right, bottomDst), src, Rectangle(widthSrc - widthRightSrc, topSrc, widthSrc, bottomSrc));
		}
	}
	{
		sl_real topDst = rectDst.top + heightTopDst;
		sl_real bottomDst = rectDst.bottom - heightBottomDst;
		sl_real topSrc = heightTopSrc;
		sl_real bottomSrc = heightSrc - heightBottomSrc;
		if (widthLeftDst > 0 && widthLeftSrc > 0) {
			canvas->draw(Rectangle(rectDst.left, topDst, rectDst.left + widthLeftDst, bottomDst), src, Rectangle(0, topSrc, widthLeftSrc, bottomSrc));
		}
		canvas->draw(Rectangle(rectDst.left + widthLeftDst, topDst, rectDst.right - widthRightDst, bottomDst), src, Rectangle(widthLeftSrc, topSrc, widthSrc - widthRightSrc, bottomSrc));
		if (widthRightDst > 0 && widthRightSrc > 0) {
			canvas->draw(Rectangle(rectDst.right - widthRightDst, topDst, rectDst.right, bottomDst), src, Rectangle(widthSrc - widthRightSrc, topSrc, widthSrc, bottomSrc));
		}
	}
}

SLIB_GRAPHICS_NAMESPACE_END
