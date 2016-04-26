#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_APPLE)

#include "../../../inc/slib/graphics/bitmap.h"
#include "../../../inc/slib/graphics/image.h"

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/platform.h"

SLIB_UI_NAMESPACE_BEGIN

class _Quartz_ImageDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT
	
public:
	CGImageRef m_image;
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
		Ref<_Quartz_ImageDrawable> ret;
		if (image) {
			CGImageRetain(image);
			ret = new _Quartz_ImageDrawable();
			if (ret.isNotNull()) {
				ret->m_image = image;
				ret->m_flagFlipped = flagFlipped;
				return ret;
			}
			CGImageRelease(image);
		}
		return ret;
	}
	
	// override
	void onDraw(Canvas* _canvas, const Rectangle& _rectDst, const Rectangle& _rectSrc)
	{
		CGContextRef graphics = UIPlatform::getCanvasHandle(_canvas);
		if (!graphics) {
			return;
		}
		CGRect rectDst;
		rectDst.origin.x = _rectDst.left;
		rectDst.origin.y = _rectDst.top;
		rectDst.size.width = _rectDst.getWidth();
		rectDst.size.height = _rectDst.getHeight();
		CGRect rectSrc;
		rectSrc.origin.x = _rectSrc.left;
		rectSrc.origin.y = _rectSrc.top;
		rectSrc.size.width = _rectSrc.getWidth();
		rectSrc.size.height = _rectSrc.getHeight();
		CGImageRef subImage = CGImageCreateWithImageInRect(m_image, rectSrc);
		if (subImage) {
			if (m_flagFlipped) {
				CGContextDrawImage(graphics, rectDst, subImage);
			} else {
				CGContextSaveGState(graphics);
				CGContextTranslateCTM(graphics, 0, rectDst.origin.y + rectDst.size.height);
				CGContextScaleCTM(graphics, 1, -1);
				CGContextTranslateCTM(graphics, 0, -rectDst.origin.y);
				CGContextDrawImage(graphics, rectDst, subImage);
				CGContextRestoreGState(graphics);
			}
			CGImageRelease(subImage);
		}
	}
	
	// override
	sl_real getDrawableWidth()
	{
		return (sl_real)(CGImageGetWidth(m_image));
	}
	
	// override
	sl_real getDrawableHeight()
	{
		return (sl_real)(CGImageGetHeight(m_image));
	}
	
};

SLIB_DEFINE_OBJECT(_Quartz_ImageDrawable, Drawable)


Ref<Drawable> UIPlatform::createImageDrawable(CGImageRef image, sl_bool flagFlipped)
{
	return _Quartz_ImageDrawable::create(image, flagFlipped);
}

CGImageRef UIPlatform::getImageDrawableHandle(Drawable* _drawable)
{
	if (_Quartz_ImageDrawable::checkInstance(_drawable)) {
		_Quartz_ImageDrawable* drawable = (_Quartz_ImageDrawable*)_drawable;
		return drawable->m_image;
	}
	return NULL;
}


void _Drawable_DataProviderRelease(void *info, const void *data, size_t size)
{
	Referable* ref = (Referable*)info;
	ref->decreaseReference();
}

Ref<Drawable> UI::createDrawableFromImage(const ImageDesc& desc)
{
	sl_uint32 width = desc.width;
	sl_uint32 height = desc.height;
	sl_uint32 stride = desc.stride;
	Ref<Drawable> ret;
	Ref<Referable> refData = desc.ref;
	refData->increaseReference();
	CGDataProviderRef provider = CGDataProviderCreateWithData(refData.ptr, desc.colors, (stride * height) << 2, _Drawable_DataProviderRelease);
	if (provider) {
		CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
		if (colorSpaceRef) {
			CGImageRef imageRef = CGImageCreate(width, height, 8, 32, stride << 2, colorSpaceRef
												, kCGBitmapByteOrder32Big | kCGImageAlphaLast
												, provider, NULL, NO, kCGRenderingIntentDefault);
			if (imageRef) {
				ret = UIPlatform::createImageDrawable(imageRef);
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

Ref<Drawable> UI::loadDrawableFromMemory(const void* mem, sl_size size)
{
	CGImageRef image = Apple::loadCGImage(mem, size);
	if (image) {
		Ref<Drawable> ret = UIPlatform::createImageDrawable(image);
		CGImageRelease(image);
		return ret;
	}
	return Ref<Drawable>::null();
}

class _Quartz_Bitmap : public Bitmap
{
	SLIB_DECLARE_OBJECT
	
public:
	CGContextRef m_bitmap;
	Memory m_mem;
	void* m_buf;
	sl_uint32 m_width;
	sl_uint32 m_height;
	
public:
	_Quartz_Bitmap()
	{
	}
	
	~_Quartz_Bitmap()
	{
		CGContextRelease(m_bitmap);
	}
	
public:
	static Ref<_Quartz_Bitmap> create(sl_uint32 width, sl_uint32 height)
	{
		Ref<_Quartz_Bitmap> ret;
		
		if (width > 0 && height > 0) {
			
			sl_uint32 size = (width * height) << 2;
			
			Memory mem = Memory::create(size);
			
			if (mem.isNotNull()) {
				
				Base::resetMemory(mem.getData(), 0, mem.getSize());
				
				CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
				
				if (colorSpace) {
					CGContextRef bitmap = CGBitmapContextCreate(mem.getData(), width, height, 8, width << 2, colorSpace, kCGBitmapByteOrder32Big | kCGImageAlphaPremultipliedLast);
					
					if (bitmap) {
						
						ret = new _Quartz_Bitmap();
						
						if (ret.isNotNull()) {
						
							ret->m_bitmap = bitmap;
							ret->m_width = width;
							ret->m_height = height;
							ret->m_mem = mem;
							ret->m_buf = mem.getData();
							
							CGContextTranslateCTM(ret->m_bitmap, 0, height - 1);
							CGContextScaleCTM(ret->m_bitmap, 1, -1);
							
							return ret;
						}
						
						CGContextRelease(bitmap);
					}
					
					CGColorSpaceRelease(colorSpace);
					
				}
				
			}
		}
		return ret;
	}
	
	static Ref<_Quartz_Bitmap> loadFromMemory(const void* mem, sl_size size)
	{
		Ref<_Quartz_Bitmap> ret;
		CGImageRef image = Apple::loadCGImage(mem, size);
		if (image) {
			sl_uint32 width = (sl_uint32)(CGImageGetWidth(image));
			sl_uint32 height = (sl_uint32)(CGImageGetHeight(image));
			ret = create(width, height);
			if (ret.isNotNull()) {
				CGRect rect;
				rect.origin.x = 0;
				rect.origin.y = 0;
				rect.size.width = width;
				rect.size.height = height;
				
				CGContextSaveGState(ret->m_bitmap);
				CGContextTranslateCTM(ret->m_bitmap, 0, height - 1);
				CGContextScaleCTM(ret->m_bitmap, 1, -1);
				CGContextDrawImage(ret->m_bitmap, rect, image);
				CGContextRestoreGState(ret->m_bitmap);
			}
			CGImageRelease(image);
		}
		return ret;
	}
	
	// override
	sl_uint32 getBitmapWidth()
	{
		return m_width;
	}
	
	// override
	sl_uint32 getBitmapHeight()
	{
		return m_height;
	}
	
	// override
	sl_bool readPixels(sl_uint32 x, sl_uint32 y, BitmapData& bitmapData)
	{
		if (x > m_width) {
			return sl_false;
		}
		if (y > m_height) {
			return sl_false;
		}
		BitmapData source;
		source.width = m_width - x;
		source.height = m_height - y;
		source.format = BitmapFormat::RGBA_PA;
		source.data = ((sl_uint32*)m_buf) + m_width * y + x;
		source.pitch = m_width << 2;
		
		bitmapData.copyPixelsFrom(source);
		
		return sl_true;
	}
	
	// override
	sl_bool writePixels(sl_uint32 x, sl_uint32 y, const BitmapData& bitmapData)
	{
		if (x > m_width) {
			return sl_false;
		}
		if (y > m_height) {
			return sl_false;
		}
		
		BitmapData target;
		target.width = m_width - x;
		target.height = m_height - y;
		target.format = BitmapFormat::RGBA_PA;
		target.data = ((sl_uint32*)m_buf) + m_width * y + x;
		target.pitch = m_width << 2;
		
		target.copyPixelsFrom(bitmapData);

		return sl_true;
	}
	
	// override
	sl_bool resetPixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, const Color& _color)
	{
		if (x > m_width) {
			return sl_false;
		}
		if (y > m_height) {
			return sl_false;
		}
		if (width > m_width - x) {
			width = m_width - x;
		}
		if (height > m_height - y) {
			height = m_height - y;
		}
		if (width == 0 || height == 0) {
			return sl_true;
		}
		sl_uint32* buf = ((sl_uint32*)m_buf) + m_width * y + x;
		Color color = _color;
		color.convertNPAtoPA();
		sl_uint32 pattern = *(sl_uint32*)((void*)&color);
		for (sl_uint32 j = 0; j < height; j++) {
			sl_uint32* t = buf;
			for (sl_uint32 i = 0; i < width; i++) {
				*t = pattern;
				t++;
			}
			buf += m_width;
		}
		return sl_true;
	}
	
	// override
	Ref<Canvas> getCanvas()
	{
		return UIPlatform::createCanvas(m_bitmap, m_width, m_height);
	}
	
	
	Ref<Drawable> getDrawable()
	{
		CGImageRef image = CGBitmapContextCreateImage(m_bitmap);
		if (image) {
			Ref<Drawable> ret = UIPlatform::createImageDrawable(image);
			CGImageRelease(image);
			return ret;
		}
		return Ref<Drawable>::null();
	}
};

SLIB_DEFINE_OBJECT(_Quartz_Bitmap, Bitmap)


Ref<Bitmap> UI::createBitmap(sl_uint32 width, sl_uint32 height)
{
	return _Quartz_Bitmap::create(width, height);
}

Ref<Drawable> UI::createDrawableFromBitmap(const Ref<Bitmap>& bitmap)
{
	if (bitmap.isNull()) {
		return Ref<Drawable>::null();
	}
	if (_Quartz_Bitmap::checkInstance(bitmap.ptr)) {
		_Quartz_Bitmap* p = (_Quartz_Bitmap*)(bitmap.ptr);
		return p->getDrawable();
	}
	return Ref<Drawable>::null();
}

Ref<Bitmap> UI::loadBitmapFromMemory(const void* mem, sl_size size)
{
	return _Quartz_Bitmap::loadFromMemory(mem, size);
}


#if defined(SLIB_PLATFORM_IS_OSX)

NSImage* UIPlatform::getNSImageFromBitmap(const Ref<Bitmap>& bitmap)
{
	if (bitmap.isNotNull() && bitmap->isNotEmpty()) {
		sl_uint32 width = bitmap->getWidth();
		sl_uint32 height = bitmap->getHeight();
		NSBitmapImageRep* rep = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:NULL pixelsWide:width pixelsHigh:height bitsPerSample:8 samplesPerPixel:4 hasAlpha:YES isPlanar:FALSE colorSpaceName:NSDeviceRGBColorSpace bitmapFormat:0 bytesPerRow:0 bitsPerPixel:0];
		if (rep != nil) {
			BitmapData bd;
			bd.data = rep.bitmapData;
			bd.format = BitmapFormat::RGBA;
			bd.width = width;
			bd.height = height;
			if (bitmap->readPixels(0, 0, bd)) {
				NSSize size;
				size.width = width;
				size.height = height;
				NSImage* image = [[NSImage alloc] initWithSize:size];
				if (image != nil) {
					[image addRepresentation:rep];
					return image;
				}
			}
		}
	}
	return nil;
}

#endif

SLIB_UI_NAMESPACE_END

#endif
