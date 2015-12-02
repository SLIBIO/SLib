#include "../../../inc/slib/graphics/context.h"
#include "../../../inc/slib/graphics/image.h"

#include "../../../inc/slib/core/file.h"
#include "../../../inc/slib/core/resource.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN
GraphicsContext::GraphicsContext()
{
}

Ref<Drawable> GraphicsContext::createDrawableFromImage(const Ref<Image>& _image)
{
	Ref<Image> image = _image;
	if (image.isNotNull()) {
		ImageDesc desc;
		image->getDesc(desc);
		return createDrawableFromImage(desc);
	}
	return Ref<Drawable>::null();
}

Ref<Drawable> GraphicsContext::loadDrawableFromMemory(const Memory& _mem)
{
	Memory mem = _mem;
	if (mem.isNotEmpty()) {
		return loadDrawableFromMemory(mem.getBuf(), mem.getSize());
	}
	return Ref<Drawable>::null();
}

Ref<Drawable> GraphicsContext::loadDrawableFromFile(const String& filePath)
{
	Ref<Drawable> ret;
	Memory mem = File::readAllBytes(filePath);
	if (mem.isNotEmpty()) {
		ret = loadDrawableFromMemory(mem);
	}
	return ret;
}

Ref<Drawable> GraphicsContext::loadDrawableFromResource(const String& path)
{
	Ref<Drawable> ret;
	Memory mem = Resource::readAllBytes(path);
	if (mem.isNotEmpty()) {
		ret = loadDrawableFromMemory(mem);
	}
	return ret;
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

Ref<Bitmap> GraphicsContext::createBitmapFromImage(const Ref<Image>& _image)
{
	Ref<Image> image = _image;
	if (image.isNotNull()) {
		ImageDesc desc;
		image->getDesc(desc);
		return createBitmapFromImage(desc);
	}
	return Ref<Bitmap>::null();
}

Ref<Bitmap> GraphicsContext::loadBitmapFromMemory(const Memory& _mem)
{
	Memory mem = _mem;
	if (mem.isNotEmpty()) {
		return loadBitmapFromMemory(mem.getBuf(), mem.getSize());
	}
	return Ref<Bitmap>::null();
}

Ref<Bitmap> GraphicsContext::loadBitmapFromFile(const String& filePath)
{
	Ref<Bitmap> ret;
	Memory mem = File::readAllBytes(filePath);
	if (mem.isNotEmpty()) {
		ret = loadBitmapFromMemory(mem);
	}
	return ret;
}

Ref<Bitmap> GraphicsContext::loadBitmapFromResource(const String& path)
{
	Ref<Bitmap> ret;
	Memory mem = Resource::readAllBytes(path);
	if (mem.isNotEmpty()) {
		ret = loadBitmapFromMemory(mem);
	}
	return ret;
}
SLIB_GRAPHICS_NAMESPACE_END


