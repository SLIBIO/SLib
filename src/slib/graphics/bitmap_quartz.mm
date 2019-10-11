/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#include "slib/core/definition.h"

#if defined(SLIB_GRAPHICS_IS_QUARTZ)

#include "slib/graphics/bitmap.h"

#include "slib/graphics/platform.h"

namespace slib
{

	namespace priv
	{
		namespace quartz
		{

			class BitmapImpl : public Bitmap
			{
				SLIB_DECLARE_OBJECT
				
			public:
				CGContextRef m_bitmap;
				Memory m_mem;
				void* m_buf;
				sl_uint32 m_width;
				sl_uint32 m_height;
				
			public:
				BitmapImpl()
				{
				}
				
				~BitmapImpl()
				{
					CFRelease(m_bitmap);
				}
				
			public:
				static Ref<BitmapImpl> create(sl_uint32 width, sl_uint32 height)
				{
					
					if (width > 0 && height > 0) {
						
						sl_uint32 size = (width * height) << 2;
						
						Memory mem = Memory::create(size);
						
						if (mem.isNotNull()) {
							
							Base::resetMemory(mem.getData(), 0, mem.getSize());
							
							CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
							
							if (colorSpace) {
								
								CGContextRef bitmap = CGBitmapContextCreate(mem.getData(), width, height, 8, width << 2, colorSpace, kCGBitmapByteOrder32Big | kCGImageAlphaPremultipliedLast);
								
								CFRelease(colorSpace);

								if (bitmap) {
									
									Ref<BitmapImpl> ret = new BitmapImpl();
									
									if (ret.isNotNull()) {
									
										ret->m_bitmap = bitmap;
										ret->m_width = width;
										ret->m_height = height;
										ret->m_mem = mem;
										ret->m_buf = mem.getData();
										
										return ret;
									}
									
									CFRelease(bitmap);
								}
								
							}
							
						}
					}
					return sl_null;
				}
				
				static Ref<BitmapImpl> create(CGImageRef image)
				{
					if (!image) {
						return sl_null;
					}
					
					sl_uint32 width = (sl_uint32)(CGImageGetWidth(image));
					sl_uint32 height = (sl_uint32)(CGImageGetHeight(image));

					Ref<BitmapImpl> ret = create(width, height);
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
						
						return ret;
					}
					
					return sl_null;
				}
				
				static Ref<BitmapImpl> loadFromMemory(const void* mem, sl_size size)
				{
					CGImageRef image = GraphicsPlatform::loadCGImageFromMemory(mem, size);
					if (image) {
						Ref<BitmapImpl> ret = create(image);
						CGImageRelease(image);
						return ret;
					}
					return sl_null;
				}
				
				sl_uint32 getBitmapWidth() override
				{
					return m_width;
				}
				
				sl_uint32 getBitmapHeight() override
				{
					return m_height;
				}
				
				sl_bool readPixels(sl_uint32 x, sl_uint32 y, BitmapData& bitmapData) override
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
				
				sl_bool writePixels(sl_uint32 x, sl_uint32 y, const BitmapData& bitmapData) override
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
				
				sl_bool resetPixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, const Color& _color) override
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
				
				Ref<Canvas> getCanvas() override
				{
					return GraphicsPlatform::createCanvas(CanvasType::Bitmap, m_bitmap, m_width, m_height);
				}
				
				void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param) override
				{
					CGImageRef image = CGBitmapContextCreateImage(m_bitmap);
					if (image) {
						CGImageRef imageCut = CGImageCreateWithImageInRect(image, CGRectMake((CGFloat)(rectSrc.left), (CGFloat)(m_height - rectSrc.bottom), (CGFloat)(rectSrc.getWidth()), (CGFloat)(rectSrc.getHeight())));
						if (imageCut) {
							GraphicsPlatform::drawCGImage(canvas, rectDst, imageCut, sl_true, param);
							CFRelease(imageCut);
						}
						CFRelease(image);
					}
				}
				
				void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param) override
				{
					CGImageRef image = CGBitmapContextCreateImage(m_bitmap);
					if (image) {
						GraphicsPlatform::drawCGImage(canvas, rectDst, image, sl_true, param);
						CFRelease(image);
					}
				}
				
			};

			SLIB_DEFINE_OBJECT(BitmapImpl, Bitmap)

		}
	}

	using namespace priv::quartz;

	Ref<Bitmap> Bitmap::create(sl_uint32 width, sl_uint32 height)
	{
		return BitmapImpl::create(width, height);
	}

	Ref<Bitmap> Bitmap::loadFromMemory(const void* mem, sl_size size)
	{
		return BitmapImpl::loadFromMemory(mem, size);
	}

	Ref<Bitmap> GraphicsPlatform::createImageBitmap(CGImageRef image)
	{
		return BitmapImpl::create(image);
	}
	
	CGContextRef GraphicsPlatform::getBitmapHandle(Bitmap* _bitmap)
	{
		if (BitmapImpl* bitmap = CastInstance<BitmapImpl>(_bitmap)) {
			return bitmap->m_bitmap;
		}
		return sl_null;
	}

}

#endif
