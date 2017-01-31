#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_APPLE)

#include "../../../inc/slib/graphics/bitmap.h"
#include "../../../inc/slib/graphics/platform.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

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
					
					CGColorSpaceRelease(colorSpace);

					if (bitmap) {
						
						ret = new _Quartz_Bitmap();
						
						if (ret.isNotNull()) {
						
							ret->m_bitmap = bitmap;
							ret->m_width = width;
							ret->m_height = height;
							ret->m_mem = mem;
							ret->m_buf = mem.getData();
							
							return ret;
						}
						
						CGContextRelease(bitmap);
					}
					
				}
				
			}
		}
		return ret;
	}
	
	static Ref<_Quartz_Bitmap> loadFromMemory(const void* mem, sl_size size)
	{
		Ref<_Quartz_Bitmap> ret;
		CGImageRef image = GraphicsPlatform::loadCGImageFromMemory(mem, size);
		if (image) {
			CGImageRetain(image);
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
				CGContextTranslateCTM(ret->m_bitmap, 0, height);
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
		source.data = ((sl_uint32*)m_buf) + m_width * (m_height - 1 - y) + x;
		source.pitch = -(m_width << 2);
		
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
		target.data = ((sl_uint32*)m_buf) + m_width * (m_height - 1 - y) + x;
		target.pitch = -(m_width << 2);
		
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
		sl_uint32* buf = ((sl_uint32*)m_buf) + m_width * (m_height - 1 - y) + x;
		Color color = _color;
		color.convertNPAtoPA();
		sl_uint32 pattern = *(sl_uint32*)((void*)&color);
		for (sl_uint32 j = 0; j < height; j++) {
			sl_uint32* t = buf;
			for (sl_uint32 i = 0; i < width; i++) {
				*t = pattern;
				t++;
			}
			buf -= m_width;
		}
		return sl_true;
	}
	
	// override
	Ref<Canvas> getCanvas()
	{
		return GraphicsPlatform::createCanvas(CanvasType::Bitmap, m_bitmap, m_width, m_height);
	}
	
	// override
	void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param)
	{
		sl_int32 sx = (sl_int32)(rectSrc.left);
		sl_int32 sy = (sl_int32)(rectSrc.top);
		sl_int32 sx2 = (sl_int32)(rectSrc.right);
		sl_int32 sy2 = (sl_int32)(rectSrc.bottom);
		if (sx < 0) {
			sx = 0;
		}
		if (sy < 0) {
			sy = 0;
		}
		sl_int32 w = (sl_int32)m_width;
		if (sx2 > w) {
			sx2 = w;
		}
		sl_int32 h = (sl_int32)m_height;
		if (sy2 > h) {
			sy2 = h;
		}
		sl_int32 sw = sx2 - sx;
		if (sw <= 0) {
			return;
		}
		sl_int32 sh = sy2 - sy;
		if (sh <= 0) {
			return;
		}

		CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
		if (colorSpace) {
			char* buf = (char*)m_buf + ((sx + (m_height - sy - sh) * w) << 2);
			CGContextRef bitmap = CGBitmapContextCreate(buf, sw, sh, 8, w << 2, colorSpace, kCGBitmapByteOrder32Big | kCGImageAlphaPremultipliedLast);
			if (bitmap) {
				CGImageRef image = CGBitmapContextCreateImage(bitmap);
				if (image) {
					GraphicsPlatform::drawCGImage(canvas, rectDst, image, sl_true, param);
					CGImageRelease(image);
				}
				CGContextRelease(bitmap);
			}
			CGColorSpaceRelease(colorSpace);
		}
	}
	
	// override
	void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param)
	{
		CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
		if (colorSpace) {
			CGContextRef bitmap = CGBitmapContextCreate(m_buf, m_width, m_height, 8, m_width << 2, colorSpace, kCGBitmapByteOrder32Big | kCGImageAlphaPremultipliedLast);
			if (bitmap) {
				CGImageRef image = CGBitmapContextCreateImage(bitmap);
				if (image) {
					GraphicsPlatform::drawCGImage(canvas, rectDst, image, sl_true, param);
					CGImageRelease(image);
				}
				CGContextRelease(bitmap);
			}
			CGColorSpaceRelease(colorSpace);
		}
	}
	
};

SLIB_DEFINE_OBJECT(_Quartz_Bitmap, Bitmap)

Ref<Bitmap> Bitmap::create(sl_uint32 width, sl_uint32 height)
{
	return _Quartz_Bitmap::create(width, height);
}

Ref<Bitmap> Bitmap::loadFromMemory(const void* mem, sl_size size)
{
	return _Quartz_Bitmap::loadFromMemory(mem, size);
}


#if defined(SLIB_PLATFORM_IS_OSX)
NSImage* GraphicsPlatform::createNSImageFromBitmap(const Ref<Bitmap>& bitmap)
{
	if (bitmap.isNotNull() && bitmap->isNotEmpty()) {
		sl_uint32 width = bitmap->getWidth();
		sl_uint32 height = bitmap->getHeight();
		NSBitmapImageRep* rep = [[NSBitmapImageRep alloc] initWithBitmapDataPlanes:NULL pixelsWide:width pixelsHigh:height bitsPerSample:8 samplesPerPixel:4 hasAlpha:YES isPlanar:FALSE colorSpaceName:NSDeviceRGBColorSpace bitmapFormat:0 bytesPerRow:(width*4) bitsPerPixel:32];
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

SLIB_GRAPHICS_NAMESPACE_END

#endif
