#include "../../../inc/slib/graphics/bitmap.h"
#include "../../../inc/slib/graphics/context.h"
#include "../../../inc/slib/graphics/image.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN
Bitmap::Bitmap()
{
}

sl_real Bitmap::getDrawableWidth()
{
	return (sl_real)(getBitmapWidth());
}

sl_real Bitmap::getDrawableHeight()
{
	return (sl_real)(getBitmapHeight());
}

sl_uint32 Bitmap::getWidth()
{
	return getBitmapWidth();
}

sl_uint32 Bitmap::getHeight()
{
	return getBitmapHeight();
}

void Bitmap::onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc)
{
	Ref<GraphicsContext> context = canvas->getGraphicsContext();
	if (context.isNull()) {
		return;
	}
	Ref<GraphicsContext> contextCached = m_contextCached;
	Ref<Drawable> drawableCached = m_drawableCached;
	if (contextCached == context && drawableCached.isNotNull()) {
		drawableCached->onDraw(canvas, rectDst, rectSrc);
	} else {
		if (Image::checkInstance(this)) {
			drawableCached = context->createDrawableFromImage((Image*)this);
		} else {
			drawableCached = context->createDrawableFromBitmap(this);
		}
		if (drawableCached.isNotNull()) {
			drawableCached->onDraw(canvas, rectDst, rectSrc);
			m_drawableCached = drawableCached;
			m_contextCached = context;
		}
	}
}

void Bitmap::update()
{
	m_contextCached.setNull();
	m_drawableCached.setNull();
}

sl_bool Bitmap::readPixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, Color* colors, sl_int32 stride)
{
	BitmapData bitmapData(width, height, colors, stride);
	return readPixels(x, y, bitmapData);
}

sl_bool Bitmap::writePixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, const Color* colors, sl_int32 stride)
{
	BitmapData bitmapData(width, height, colors, stride);
	return writePixels(x, y, bitmapData);
}

sl_bool Bitmap::resetPixels(const Color& color)
{
	return resetPixels(0, 0, getWidth(), getHeight(), color);
}
SLIB_GRAPHICS_NAMESPACE_END

