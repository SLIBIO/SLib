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

#include "slib/graphics/brush.h"

#include "slib/graphics/image.h"
#include "slib/graphics/platform.h"
#include "slib/core/scoped.h"

namespace slib
{

	namespace priv
	{
		namespace quartz
		{
			
			class GradientBrushPlatformObject : public Referable
			{
			public:
				CGGradientRef hGradient;
				
			public:
				GradientBrushPlatformObject(BrushDesc& desc)
				{
					hGradient = sl_null;
					if (desc.style == BrushStyle::LinearGradient || desc.style == BrushStyle::RadialGradient) {
						GradientBrushDetail* detail = (GradientBrushDetail*)(desc.detail.get());
						if (detail) {
							CGColorSpaceRef colorSpace = CGColorSpaceCreateDeviceRGB();
							if (colorSpace) {
								NSMutableArray* array = [[NSMutableArray alloc] init];
								if (array) {
									sl_bool flagSuccess = sl_true;
									ListElements<Color> colors(detail->colors);
									sl_size n = colors.count;
									SLIB_SCOPED_BUFFER(CGFloat, 128, locations, n);
									{
										ListElements<sl_real> _locations(detail->locations);
										for (sl_size i = 0; i < n; i++) {
											locations[i] = _locations[i];
											Color& color = colors[i];
											CGFloat comps[4] = {color.getRedF(), color.getGreenF(), color.getBlueF(), color.getAlphaF()};
											CGColorRef colorRef = CGColorCreate(colorSpace, comps);
											if (colorRef) {
												[array addObject:(__bridge id)colorRef];
												CFRelease(colorRef);
											} else {
												flagSuccess = sl_false;
											}
										}
									}
									if (flagSuccess) {
										hGradient = CGGradientCreateWithColors(colorSpace, (CFArrayRef)array, locations);
									}
								}
								CFRelease(colorSpace);
							}
						}
					}
				}
				
				~GradientBrushPlatformObject()
				{
					if (hGradient) {
						CFRelease(hGradient);
					}
				}
				
			};
			
			class TextureBrushPlatformObject : public Referable
			{
			public:
				CGImageRef hImage;
				CGContextRef hBitmap;
				Ref<Drawable> drawableCache;
				
			public:
				TextureBrushPlatformObject(BrushDesc& desc)
				{
					hImage = sl_null;
					hBitmap = sl_null;
					if (desc.style == BrushStyle::Texture) {
						TextureBrushDetail* detail = (TextureBrushDetail*)(desc.detail.get());
						if (detail) {
							Bitmap* bitmap = detail->pattern.get();
							if (bitmap->isImage()) {
								drawableCache = PlatformDrawable::create((Image*)bitmap);
								if (drawableCache.isNotNull()) {
									hImage = GraphicsPlatform::getImageDrawableHandle(drawableCache.get());
								}
							} else {
								hBitmap = GraphicsPlatform::getBitmapHandle(bitmap);
							}
						}
					}
				}
				
			};

			class BrushHelper : public Brush
			{
			public:
				CGGradientRef getGradientBrushHandle()
				{
					if (m_platformObject.isNull()) {
						SpinLocker lock(&m_lock);
						if (m_platformObject.isNull()) {
							m_platformObject = new GradientBrushPlatformObject(m_desc);
						}
					}
					GradientBrushPlatformObject* obj = (GradientBrushPlatformObject*)(m_platformObject.get());
					if (obj) {
						return obj->hGradient;
					}
					return sl_null;
				}

				CGImageRef getTextureBrushRetainedHandle()
				{
					if (m_platformObject.isNull()) {
						SpinLocker lock(&m_lock);
						if (m_platformObject.isNull()) {
							m_platformObject = new TextureBrushPlatformObject(m_desc);
						}
					}
					TextureBrushPlatformObject* obj = (TextureBrushPlatformObject*)(m_platformObject.get());
					if (obj) {
						CGImageRef image = obj->hImage;
						if (image) {
							CFRetain(image);
							return image;
						} else {
							CGContextRef bitmap = obj->hBitmap;
							if (bitmap) {
								CGImageRef image = CGBitmapContextCreateImage(bitmap);
								if (image) {
									return image;
								}
							}
						}
					}
					return sl_null;
				}

			};

		}
	}

	using namespace priv::quartz;

	CGGradientRef GraphicsPlatform::getGradientBrushHandle(Brush* brush)
	{
		if (brush) {
			return ((BrushHelper*)brush)->getGradientBrushHandle();
		}
		return NULL;
	}
	
	CGImageRef GraphicsPlatform::getTextureBrushRetainedHandle(Brush* brush)
	{
		if (brush) {
			return ((BrushHelper*)brush)->getTextureBrushRetainedHandle();
		}
		return NULL;
	}
	
}

#endif
