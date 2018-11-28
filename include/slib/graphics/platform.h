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

#ifndef CHECKHEADER_SLIB_GRAPHICS_PLATFORM
#define CHECKHEADER_SLIB_GRAPHICS_PLATFORM

#include "definition.h"

#include "color.h"
#include "canvas.h"
#include "bitmap.h"

#include "../core/ref.h"
#include "../math/matrix3.h"
#include "../math/matrix4.h"

#if defined(SLIB_GRAPHICS_IS_GDI) || defined(SLIB_PLATFORM_IS_WIN32)
#	include "../core/platform_windows.h"
#	include <GdiPlus.h>
#endif
#if defined(SLIB_GRAPHICS_IS_QUARTZ) || defined(SLIB_PLATFORM_IS_APPLE)
#	include "../core/platform_apple.h"
#	include <CoreText/CoreText.h>
#	include <CoreGraphics/CoreGraphics.h>
#endif
#if defined(SLIB_GRAPHICS_IS_ANDROID)
#	include "../core/platform_android.h"
#endif
#if defined(SLIB_GRAPHICS_IS_CAIRO)
#	if	defined(SLIB_PLATFORM_IS_TIZEN)
#		include <cairo.h>
#	else
#		include "cairo/cairo.h"
#	endif
#endif

namespace slib
{

	class SLIB_EXPORT GraphicsPlatform
	{
	public:
#if defined(SLIB_GRAPHICS_IS_GDI)

		static Gdiplus::Brush* getBrushHandle(Brush* brush);
		static Gdiplus::Pen* getPenHandle(Pen* pen);
		static Gdiplus::GraphicsPath* getGraphicsPath(GraphicsPath* path);

		static Gdiplus::Font* getGdiplusFont(Font* font);
		static HFONT getGdiFont(Font* font);

		static Ref<Canvas> createCanvas(CanvasType type, Gdiplus::Graphics* graphics, sl_uint32 width, sl_uint32 height, sl_bool flagFreeOnRelease = sl_true, Referable* ref = sl_null);
		static Gdiplus::Graphics* getCanvasHandle(Canvas* canvas);
		static void drawImage(Canvas* canvas, const Rectangle& rectDst, Gdiplus::Image* image, const Rectangle& rectSrc, const DrawParam& param);

		static Ref<Drawable> createImageDrawable(Gdiplus::Image* image, sl_bool flagFreeOnRelease = sl_true, Referable* ref = sl_null);
		static Gdiplus::Image* getImageDrawableHandle(Drawable* drawable);
		static Gdiplus::Bitmap* getBitmapHandle(Bitmap* bitmap);
	
#elif defined(SLIB_GRAPHICS_IS_ANDROID)

		static jobject getBrushHandle(Brush* brush);
		static jobject getPenHandle(Pen* pen);
		static jobject getGraphicsPath(GraphicsPath* path);
		static jobject getNativeFont(Font* font);

		static Ref<Canvas> createCanvas(CanvasType type, jobject canvas);
		static jobject getCanvasHandle(Canvas* canvas);

		static Ref<Drawable> createImageDrawable(jobject bitmap, sl_bool flagRecycleOnRelease = sl_true, Referable* ref = sl_null);
		static jobject getImageDrawableHandle(Drawable* drawable);

#elif defined(SLIB_GRAPHICS_IS_QUARTZ)

		static CGPathRef getGraphicsPath(GraphicsPath* path);
		static CTFontRef getCoreTextFont(Font* font);

		static Ref<Canvas> createCanvas(CanvasType type, CGContextRef graphics, sl_uint32 width, sl_uint32 height);
		static CGContextRef getCanvasHandle(Canvas* canvas);
		static void drawCGImage(Canvas* canvas, const Rectangle& rectDst, CGImageRef image, sl_bool flagFlipY, const DrawParam& param);

		static Ref<Drawable> createImageDrawable(CGImageRef image, sl_bool flagFlipped = sl_false);
		static CGImageRef getImageDrawableHandle(Drawable* drawable);
		static Ref<Bitmap> createImageBitmap(CGImageRef image);

#	if defined(__OBJC__)
#		if defined(SLIB_PLATFORM_IS_MACOS)

		static NSFont* getNSFont(Font* font, CGFloat scaleFactor = 1);

#		elif defined(SLIB_PLATFORM_IS_IOS)

		static UIFont* getUIFont(Font* font, CGFloat scaleFactor);

#		endif
#	endif

#elif defined(SLIB_GRAPHICS_IS_CAIRO)

		static cairo_scaled_font_t* getCairoFont(Font* font);

		static Ref<Canvas> createCanvas(CanvasType type, cairo_t* graphics, sl_uint32 width, sl_uint32 height);
		static cairo_t* getCanvasHandle(Canvas* canvas);

		static void drawImage(Canvas* canvas, const Rectangle& rectDst, cairo_surface_t* image, const Rectangle& rectSrc, const DrawParam& param);

#endif

#if defined(SLIB_PLATFORM_IS_WIN32)
		static COLORREF getColorRef(const Color& color);
		static Color getColorFromColorRef(COLORREF cr);

		static HBITMAP createDIBFromBitmap(const Ref<Bitmap>& bitmap);
#endif

#if defined(SLIB_PLATFORM_IS_APPLE)
		static CGImageRef loadCGImageFromMemory(const void* buf, sl_size size);
		static CGImageRef loadCGImageFromApp(const String& name);

#	if defined(__OBJC__)
#		if defined(SLIB_PLATFORM_IS_MACOS)

		static NSImage* loadNSImageFromMemory(const void* buf, sl_size size);
		static NSColor* getNSColorFromColor(const Color& color);
		static Color getColorFromNSColor(NSColor* color);
		static NSImage* createNSImageFromBitmap(const Ref<Bitmap>& bitmap);

#		elif defined(SLIB_PLATFORM_IS_IOS)

		static UIImage* loadUIImageFromMemory(const void* buf, sl_size size);
		static UIColor* getUIColorFromColor(const Color& color);
		static Color getColorFromUIColor(UIColor* color);

#		endif

		static void getCATransform(CATransform3D& _out, const Matrix3& mat);
		static void getCATransform(CATransform3D& _out, const Matrix4& mat);
		static void getCGAffineTransform(CGAffineTransform& _out, const Matrix3& mat);
		static void getCGAffineTransform(CGAffineTransform& _out, const Matrix3& mat, CGFloat scaleFactor, CGFloat anchorX, CGFloat anchorY);
		static void getMatrix3FromCGAffineTransform(Matrix3& _out, const CGAffineTransform& transform);
#	endif
		
#endif

	};

}

#endif
