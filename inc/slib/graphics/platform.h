#ifndef CHECKHEADER_SLIB_GRAPHICS_PLATFORM
#define CHECKHEADER_SLIB_GRAPHICS_PLATFORM

#include "definition.h"

#include "color.h"
#include "canvas.h"
#include "bitmap.h"

#include "../core/ref.h"
#include "../math/matrix3.h"
#include "../math/matrix4.h"

#if defined(SLIB_PLATFORM_IS_WIN32)
#include "../core/platform_windows.h"
#include <GdiPlus.h>
#elif defined(SLIB_PLATFORM_IS_ANDROID)
#include "../core/platform_android.h"
#elif defined(SLIB_PLATFORM_IS_APPLE)
#include "../core/platform_apple.h"
#include <CoreText/CoreText.h>
#include <CoreGraphics/CoreGraphics.h>
#endif

SLIB_GRAPHICS_NAMESPACE_BEGIN

class SLIB_EXPORT GraphicsPlatform
{
public:
#if defined(SLIB_PLATFORM_IS_WIN32)
	
	static Gdiplus::Brush* getBrushHandle(Brush* brush);
	static Gdiplus::Pen* getPenHandle(Pen* pen);
	static Gdiplus::GraphicsPath* getGraphicsPath(GraphicsPath* path);
	
	static Gdiplus::Font* getGdiplusFont(Font* font);
	static HFONT getGdiFont(Font* font);
	
	static Ref<Canvas> createCanvas(CanvasType type, Gdiplus::Graphics* graphics, sl_uint32 width, sl_uint32 height, sl_bool flagFreeOnRelease = sl_true, const Referable* ref = sl_null);
	static Gdiplus::Graphics* getCanvasHandle(Canvas* canvas);
	static void drawImage(Canvas* canvas, const Rectangle& rectDst, Gdiplus::Image* image, const Rectangle& rectSrc, const DrawParam& param);
	
	static Ref<Drawable> createImageDrawable(Gdiplus::Image* image, sl_bool flagFreeOnRelease = sl_true, const Referable* ref = sl_null);
	static Gdiplus::Image* getImageHandle(Drawable* drawable);
	
	static Gdiplus::Bitmap* getBitmapHandle(Bitmap* bitmap);
	static HBITMAP createDIBFromBitmap(const Ref<Bitmap>& bitmap);

	static COLORREF getColorRef(const Color& color);
	static Color getColorFromColorRef(COLORREF cr);
	
#elif defined(SLIB_PLATFORM_IS_ANDROID)
	
	static jobject getBrushHandle(Brush* brush);
	static jobject getPenHandle(Pen* pen);
	static jobject getGraphicsPath(GraphicsPath* path);
	static jobject getNativeFont(Font* font);
	
	static Ref<Canvas> createCanvas(CanvasType type, jobject canvas);
	static jobject getCanvasHandle(Canvas* canvas);
	
	static Ref<Drawable> createImageDrawable(jobject bitmap, sl_bool flagRecycleOnRelease = sl_true, const Referable* ref = sl_null);
	static jobject getImageDrawableHandle(Drawable* drawable);
	
#elif defined(SLIB_PLATFORM_IS_APPLE)
	
	static CGPathRef getGraphicsPath(GraphicsPath* path);
	static CTFontRef getCoreTextFont(Font* font);
	
	static Ref<Canvas> createCanvas(CanvasType type, CGContextRef graphics, sl_uint32 width, sl_uint32 height);
	static CGContextRef getCanvasHandle(Canvas* canvas);
	static void drawCGImage(Canvas* canvas, const Rectangle& rectDst, CGImageRef image, sl_bool flagFlipY, const DrawParam& param);
	
	static Ref<Drawable> createImageDrawable(CGImageRef image, sl_bool flagFlipped = sl_false);
	static CGImageRef getImageDrawableHandle(Drawable* drawable);
	
	static CGImageRef loadCGImageFromMemory(const void* buf, sl_size size);

#	if defined(__OBJC__)
#		if defined(SLIB_PLATFORM_IS_OSX)
	
	static NSFont* getNSFont(Font* font, CGFloat scaleFactor = 1);
	
	static NSImage* loadNSImageFromMemory(const void* buf, sl_size size);

	static NSColor* getNSColorFromColor(const Color& color);
	static Color getColorFromNSColor(NSColor* color);
	
	static NSImage* createNSImageFromBitmap(const Ref<Bitmap>& bitmap);
	
#		elif defined(SLIB_PLATFORM_IS_IOS)
	
	static UIFont* getUIFont(Font* font, CGFloat scaleFactor);
	
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

SLIB_GRAPHICS_NAMESPACE_END

#endif
