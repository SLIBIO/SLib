#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_APPLE)

#include "../../../inc/slib/graphics/image.h"
#include "../../../inc/slib/graphics/platform.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

void _Drawable_DataProviderRelease(void *info, const void *data, size_t size)
{
	Referable* ref = (Referable*)info;
	ref->decreaseReference();
}

Ref<Drawable> PlatformDrawable::create(const ImageDesc& desc)
{
	sl_uint32 width = desc.width;
	if (width == 0) {
		return sl_null;
	}
	sl_uint32 height = desc.height;
	if (height == 0) {
		return sl_null;
	}
	sl_uint32 stride = desc.stride;
	Ref<Drawable> ret;
	Ref<Referable> refData = desc.ref;
	refData->increaseReference();
	CGDataProviderRef provider = CGDataProviderCreateWithData(refData.get(), desc.colors, (stride * height) << 2, _Drawable_DataProviderRelease);
	if (provider) {
		CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
		if (colorSpaceRef) {
			CGImageRef imageRef = CGImageCreate(width, height, 8, 32, stride << 2, colorSpaceRef
												, kCGBitmapByteOrder32Big | kCGImageAlphaLast
												, provider, NULL, YES, kCGRenderingIntentDefault);
			if (imageRef) {
				ret = GraphicsPlatform::createImageDrawable(imageRef);
				CGImageRelease(imageRef);
			}
			CGColorSpaceRelease(colorSpaceRef);
		}
		CGDataProviderRelease(provider);
	} else {
		refData->decreaseReference();
	}
	return ret;
}

Ref<Drawable> PlatformDrawable::loadFromMemory(const void* mem, sl_size size)
{
	CGImageRef image = GraphicsPlatform::loadCGImageFromMemory(mem, size);
	if (image) {
		CGImageRetain(image);
		Ref<Drawable> ret = GraphicsPlatform::createImageDrawable(image);
		CGImageRelease(image);
		return ret;
	}
	return sl_null;
}


class _Quartz_ImageDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT
	
public:
	CGImageRef m_image;
	sl_uint32 m_width;
	sl_uint32 m_height;
	sl_bool m_flagFlipped;
	
public:
	_Quartz_ImageDrawable()
	{
	}
	
	~_Quartz_ImageDrawable()
	{
		CGImageRelease(m_image);
	}
	
public:
	static Ref<_Quartz_ImageDrawable> create(CGImageRef image, sl_bool flagFlipped)
	{
		if (image) {
			CGImageRetain(image);
			sl_int32 width = (sl_int32)(CGImageGetWidth(image));
			if (width > 0) {
				sl_int32 height = (sl_int32)(CGImageGetHeight(image));
				if (height > 0) {
					Ref<_Quartz_ImageDrawable> ret = new _Quartz_ImageDrawable();
					if (ret.isNotNull()) {
						ret->m_image = image;
						ret->m_width = width;
						ret->m_height = height;
						ret->m_flagFlipped = flagFlipped;
						return ret;
					}
				}
			}
			CGImageRelease(image);
		}
		return sl_null;
	}
	
	// override
	void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& _rectSrc, const DrawParam& param)
	{
		CGRect rectSrc;
		rectSrc.origin.x = _rectSrc.left;
		rectSrc.origin.y = _rectSrc.top;
		rectSrc.size.width = _rectSrc.getWidth();
		rectSrc.size.height = _rectSrc.getHeight();
		CGImageRef subImage = CGImageCreateWithImageInRect(m_image, rectSrc);
		if (subImage) {
			GraphicsPlatform::drawCGImage(canvas, rectDst, subImage, m_flagFlipped, param);
			CGImageRelease(subImage);
		}
	}
	
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		GraphicsPlatform::drawCGImage(canvas, rectDst, m_image, m_flagFlipped, param);
	}

	// override
	sl_real getDrawableWidth()
	{
		return (sl_real)m_width;
	}
	
	// override
	sl_real getDrawableHeight()
	{
		return (sl_real)m_height;
	}
	
};

SLIB_DEFINE_OBJECT(_Quartz_ImageDrawable, Drawable)

Ref<Drawable> GraphicsPlatform::createImageDrawable(CGImageRef image, sl_bool flagFlipped)
{
	return _Quartz_ImageDrawable::create(image, flagFlipped);
}

CGImageRef GraphicsPlatform::getImageDrawableHandle(Drawable* _drawable)
{
	if (_Quartz_ImageDrawable* drawable = CastInstance<_Quartz_ImageDrawable>(_drawable)) {
		return drawable->m_image;
	}
	return NULL;
}

SLIB_GRAPHICS_NAMESPACE_END

#endif
