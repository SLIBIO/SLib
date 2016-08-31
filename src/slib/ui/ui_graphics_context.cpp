#include "../../../inc/slib/graphics/context.h"
#include "../../../inc/slib/graphics/image.h"

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/platform.h"

#include "../../../inc/slib/core/file.h"
#include "../../../inc/slib/core/asset.h"

SLIB_UI_NAMESPACE_BEGIN

class _UI_GraphicsContext : public GraphicsContext
{
public:
    // override
	Size getFontTextSize(const Ref<Font>& font, const String& text)
	{
		return UI::getFontTextSize(font, text);
	}
	
    // override
	Rectangle getPathBounds(const Ref<GraphicsPath>& path)
	{
		return UI::getPathBounds(path);
	}
	
    // override
	sl_bool checkPointInPath(const Ref<GraphicsPath>& path, const Point& pt)
	{
		return UI::checkPointInPath(path, pt);
	}

    // override
	Ref<Drawable> createDrawableFromImage(const ImageDesc& desc)
	{
		return UI::createDrawableFromImage(desc);
	}
	
    // override
	Ref<Drawable> loadDrawableFromMemory(const void* mem, sl_size size)
	{
		return UI::loadDrawableFromMemory(mem, size);
	}
	
    // override
	Ref<Bitmap> createBitmap(sl_uint32 width, sl_uint32 height)
	{
		return UI::createBitmap(width, height);
	}
	
    // override
	Ref<Drawable> createDrawableFromBitmap(const Ref<Bitmap>& bitmap)
	{
		return UI::createDrawableFromBitmap(bitmap);
	}
	
    // override
	Ref<Bitmap> loadBitmapFromMemory(const void* mem, sl_size size)
	{
		return UI::loadBitmapFromMemory(mem, size);
	}
    
};

Ref<GraphicsContext> UI::getGraphicsContext()
{
	SLIB_SAFE_STATIC(Ref<GraphicsContext>, ret, new _UI_GraphicsContext)
	if (SLIB_SAFE_STATIC_CHECK_FREED(ret)) {
		return Ref<GraphicsContext>::null();
	}
	return ret;
}

Ref<Drawable> UI::createDrawableFromImage(const Ref<Image>& image)
{
	if (image.isNotNull()) {
		ImageDesc desc;
		image->getDesc(desc);
		return createDrawableFromImage(desc);
	}
	return Ref<Drawable>::null();
}

Ref<Drawable> UI::loadDrawableFromMemory(const Memory& mem)
{
	if (mem.isNotEmpty()) {
		return loadDrawableFromMemory(mem.getData(), mem.getSize());
	}
	return Ref<Drawable>::null();
}

Ref<Drawable> UI::loadDrawableFromFile(const String& filePath)
{
	Ref<Drawable> ret;
	Memory mem = File::readAllBytes(filePath);
	if (mem.isNotEmpty()) {
		ret = loadDrawableFromMemory(mem);
	}
	return ret;
}

Ref<Bitmap> UI::createBitmapFromImage(const ImageDesc& desc)
{
	Ref<Bitmap> ret = createBitmap(desc.width, desc.height);
	if (ret.isNotNull()) {
		ret->writePixels(0, 0, desc.width, desc.height, desc.colors, desc.stride);
		return ret;
	}
	return Ref<Bitmap>::null();
}

Ref<Bitmap> UI::createBitmapFromImage(const Ref<Image>& image)
{
	if (image.isNotNull()) {
		ImageDesc desc;
		image->getDesc(desc);
		return createBitmapFromImage(desc);
	}
	return Ref<Bitmap>::null();
}

Ref<Drawable> UI::loadDrawableFromAsset(const String& path)
{
	Ref<Drawable> ret;
	Memory mem = Assets::readAllBytes(path);
	if (mem.isNotEmpty()) {
		ret = loadDrawableFromMemory(mem);
	}
	return ret;
}

Ref<Bitmap> UI::loadBitmapFromMemory(const Memory& mem)
{
	if (mem.isNotEmpty()) {
		return loadBitmapFromMemory(mem.getData(), mem.getSize());
	}
	return Ref<Bitmap>::null();
}

Ref<Bitmap> UI::loadBitmapFromFile(const String& filePath)
{
	Ref<Bitmap> ret;
	Memory mem = File::readAllBytes(filePath);
	if (mem.isNotEmpty()) {
		ret = loadBitmapFromMemory(mem);
	}
	return ret;
}

Ref<Bitmap> UI::loadBitmapFromAsset(const String& path)
{
	Ref<Bitmap> ret;
	Memory mem = Assets::readAllBytes(path);
	if (mem.isNotEmpty()) {
		ret = loadBitmapFromMemory(mem);
	}
	return ret;
}

SLIB_UI_NAMESPACE_END

