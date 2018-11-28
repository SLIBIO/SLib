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

#include "slib/graphics/drawable.h"

#include "slib/graphics/image.h"
#include "slib/graphics/platform.h"

namespace slib
{

	void _priv_Drawable_DataProviderRelease(void *info, const void *data, size_t size)
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
		CGDataProviderRef provider = CGDataProviderCreateWithData(refData.get(), desc.colors, (stride * height) << 2, _priv_Drawable_DataProviderRelease);
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
			Ref<Drawable> ret = GraphicsPlatform::createImageDrawable(image);
			CGImageRelease(image);
			return ret;
		}
		return sl_null;
	}


	class _priv_Quartz_ImageDrawable : public Drawable
	{
		SLIB_DECLARE_OBJECT
		
	public:
		CGImageRef m_image;
		sl_uint32 m_width;
		sl_uint32 m_height;
		sl_bool m_flagFlipped;
		
	public:
		_priv_Quartz_ImageDrawable()
		{
		}
		
		~_priv_Quartz_ImageDrawable()
		{
			CGImageRelease(m_image);
		}
		
	public:
		static Ref<_priv_Quartz_ImageDrawable> create(CGImageRef image, sl_bool flagFlipped)
		{
			if (image) {
				CGImageRetain(image);
				sl_int32 width = (sl_int32)(CGImageGetWidth(image));
				if (width > 0) {
					sl_int32 height = (sl_int32)(CGImageGetHeight(image));
					if (height > 0) {
						Ref<_priv_Quartz_ImageDrawable> ret = new _priv_Quartz_ImageDrawable();
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
		
		void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& _rectSrc, const DrawParam& param) override
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
		
		void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param) override
		{
			GraphicsPlatform::drawCGImage(canvas, rectDst, m_image, m_flagFlipped, param);
		}

		sl_real getDrawableWidth() override
		{
			return (sl_real)m_width;
		}
		
		sl_real getDrawableHeight() override
		{
			return (sl_real)m_height;
		}
		
	};

	SLIB_DEFINE_OBJECT(_priv_Quartz_ImageDrawable, Drawable)

	Ref<Drawable> GraphicsPlatform::createImageDrawable(CGImageRef image, sl_bool flagFlipped)
	{
		return _priv_Quartz_ImageDrawable::create(image, flagFlipped);
	}

	CGImageRef GraphicsPlatform::getImageDrawableHandle(Drawable* _drawable)
	{
		if (_priv_Quartz_ImageDrawable* drawable = CastInstance<_priv_Quartz_ImageDrawable>(_drawable)) {
			return drawable->m_image;
		}
		return NULL;
	}

}

#endif
