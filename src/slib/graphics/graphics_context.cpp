#include "../../../inc/slib/graphics/context.h"
#include "../../../inc/slib/graphics/image.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(GraphicsContext, Object)

Ref<Drawable> GraphicsContext::createDrawableFromImage(const Ref<Image>& image)
{
	if (image.isNotNull()) {
		ImageDesc desc;
		image->getDesc(desc);
		return createDrawableFromImage(desc);
	}
	return Ref<Drawable>::null();
}

Ref<Drawable> GraphicsContext::loadDrawableFromMemory(const Memory& mem)
{
	if (mem.isNotEmpty()) {
		return loadDrawableFromMemory(mem.getData(), mem.getSize());
	}
	return Ref<Drawable>::null();
}


Ref<Bitmap> GraphicsContext::createBitmapFromImage(const ImageDesc& desc)
{
	Ref<Bitmap> ret = createBitmap(desc.width, desc.height);
	if (ret.isNotNull()) {
		ret->writePixels(0, 0, desc.width, desc.height, desc.colors, desc.stride);
		return ret;
	}
	return Ref<Bitmap>::null();
}

Ref<Bitmap> GraphicsContext::createBitmapFromImage(const Ref<Image>& image)
{
	if (image.isNotNull()) {
		ImageDesc desc;
		image->getDesc(desc);
		return createBitmapFromImage(desc);
	}
	return Ref<Bitmap>::null();
}

Ref<Bitmap> GraphicsContext::loadBitmapFromMemory(const Memory& mem)
{
	if (mem.isNotEmpty()) {
		return loadBitmapFromMemory(mem.getData(), mem.getSize());
	}
	return Ref<Bitmap>::null();
}

SLIB_GRAPHICS_NAMESPACE_END


