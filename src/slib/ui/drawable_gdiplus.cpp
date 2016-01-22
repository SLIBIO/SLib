#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/graphics/bitmap.h"
#include "../../../inc/slib/graphics/image.h"

#include "../../../inc/slib/ui/core.h"
#include "../../../inc/slib/ui/platform.h"
#include "../../../inc/slib/core/scoped_pointer.h"

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "gdiplus.lib")

SLIB_UI_NAMESPACE_BEGIN

class _Gdiplus_ImageDrawable : public Drawable
{
	SLIB_DECLARE_OBJECT(_Gdiplus_ImageDrawable, Drawable)
public:
	Gdiplus::Image* m_image;
	sl_bool m_flagFreeOnRelease;
	Ref<Referable> m_ref;
	
public:
	_Gdiplus_ImageDrawable()
	{
	}
	
	~_Gdiplus_ImageDrawable()
	{
		if (m_flagFreeOnRelease) {
			delete m_image;
		}
	}
	
public:
	static Ref<_Gdiplus_ImageDrawable> create(Gdiplus::Image* image, sl_bool flagFreeOnRelease, const Referable* ref)
	{
		Ref<_Gdiplus_ImageDrawable> ret;
		if (image) {
			ret = new _Gdiplus_ImageDrawable();
			if (ret.isNotNull()) {
				ret->m_image = image;
				ret->m_flagFreeOnRelease = flagFreeOnRelease;
				ret->m_ref = ref;
				return ret;
			}
			if (flagFreeOnRelease) {
				delete image;
			}
		}
		return ret;
	}
	
    // override
	void onDraw(Canvas* _canvas, const Rectangle& rectDst, const Rectangle& rectSrc)
	{
		Gdiplus::Graphics* graphics = UIPlatform::getCanvasHandle(_canvas);
		if (!graphics) {
			return;
		}
		Gdiplus::RectF rcDst(rectDst.left, rectDst.top, rectDst.getWidth(), rectDst.getHeight());
		graphics->DrawImage(m_image, rcDst
			, rectSrc.left, rectSrc.top, rectSrc.getWidth(), rectSrc.getHeight()
			, Gdiplus::UnitPixel);
	}
	
    // override
	sl_real getDrawableWidth()
	{
		return (sl_real)(m_image->GetWidth());
	}
	
    // override
	sl_real getDrawableHeight()
	{
		return (sl_real)(m_image->GetHeight());
	}
    
};

Ref<Drawable> UIPlatform::createImageDrawable(Gdiplus::Image* image, sl_bool flagFreeOnRelease, const Referable* ref)
{
	return _Gdiplus_ImageDrawable::create(image, flagFreeOnRelease, ref);
}

Gdiplus::Image* UIPlatform::getImageDrawableHandle(Drawable* _drawable)
{
	if (_Gdiplus_ImageDrawable::checkInstance(_drawable)) {
		_Gdiplus_ImageDrawable* drawable = (_Gdiplus_ImageDrawable*)_drawable;
		return drawable->m_image;
	}
	return NULL;
}

Ref<Drawable> UI::createDrawableFromImage(const ImageDesc& desc)
{
	return UI::createBitmapFromImage(desc);
}

Ref<Drawable> UI::loadDrawableFromMemory(const void* buf, sl_size size)
{
	Ref<Drawable> ret;
	IStream* stream = ::SHCreateMemStream((BYTE*)buf, (sl_uint32)size);
	if (stream) {
		Gdiplus::Image* image = new Gdiplus::Image(stream);
		stream->Release();
		if (image && image->GetWidth() > 0 && image->GetHeight() > 0) {
			ret = UIPlatform::createImageDrawable(image, sl_true);
		}
	}
	return ret;
}

class _Gdiplus_Bitmap : public Bitmap
{
	SLIB_DECLARE_OBJECT(_Gdiplus_Bitmap, Bitmap)
public:
	Gdiplus::Bitmap* m_bitmap;

public:
	_Gdiplus_Bitmap()
	{
	}
		
	~_Gdiplus_Bitmap()
	{
		delete m_bitmap;
	}
	
public:
	static Ref<_Gdiplus_Bitmap> create(sl_uint32 width, sl_uint32 height)
	{
		Ref<_Gdiplus_Bitmap> ret;
		if (width > 0 && height > 0) {
			Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(width, height, PixelFormat32bppARGB);
			if (bitmap) {
				if (bitmap->GetWidth() == width && bitmap->GetHeight() == height) {
					ret = new _Gdiplus_Bitmap();
					if (ret.isNotNull()) {
						ret->m_bitmap = bitmap;
						return ret;
					}
				}
				delete bitmap;
			}
		}
		return ret;
	}
	
	static Ref<_Gdiplus_Bitmap> loadFromMemory(const void* mem, sl_size size)
	{
		Ref<_Gdiplus_Bitmap> ret;
		IStream* stream = ::SHCreateMemStream((BYTE*)mem, (sl_uint32)size);
		if (stream) {
			Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(stream);
			stream->Release();
			if (bitmap && bitmap->GetWidth() > 0 && bitmap->GetHeight() > 0) {
				ret = new _Gdiplus_Bitmap();
				if (ret.isNotNull()) {
					ret->m_bitmap = bitmap;
					return ret;
				}
				delete bitmap;
			}
		}
		return ret;
	}
	
    // override
	sl_uint32 getBitmapWidth()
	{
		return m_bitmap->GetWidth();
	}
	
    // override
	sl_uint32 getBitmapHeight()
	{
		return m_bitmap->GetHeight();
	}
	
    // override
	sl_bool readPixels(sl_uint32 x, sl_uint32 y, BitmapData& _dst)
	{
		sl_uint32 w = getBitmapWidth();
		sl_uint32 h = getBitmapHeight();
		if (x >= w || y >= h) {
			return sl_false;
		}

		BitmapData dst(_dst);
		sl_uint32 width = dst.width;
		sl_uint32 height = dst.height;

		dst.fillDefaultValues();

		if (width > w - x) {
			width = w - x;
		}
		if (height > h - y) {
			height = h - y;
		}
		if (width == 0 || height == 0) {
			return sl_true;
		}

		Gdiplus::Rect rc(x, y, width, height);
		Gdiplus::Status result;

		if (dst.format.getBitsPerSample() == 32 && dst.format.getPlanesCount() == 1) {

			Gdiplus::BitmapData data;
			data.Width = width;
			data.Height = height;
			data.Scan0 = dst.data;
			data.Stride = dst.pitch;
			data.PixelFormat = PixelFormat32bppARGB;

			BitmapData src(dst);
			src.format = bitmapFormat_BGRA;

			result = m_bitmap->LockBits(&rc, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeUserInputBuf, PixelFormat32bppARGB, &data);
			if (result == Gdiplus::Ok) {
				m_bitmap->UnlockBits(&data);
				dst.copyPixelsFrom(src);
				return sl_true;
			}

			return sl_false;

		} else {
			
			SLIB_SCOPED_BUFFER(Color, 65536, buf, width*height);
			if (!buf) {
				return sl_false;
			}

			Gdiplus::BitmapData data;
			data.Width = width;
			data.Height = height;
			data.Scan0 = buf;
			data.Stride = width << 2;
			data.PixelFormat = PixelFormat32bppARGB;

			result = m_bitmap->LockBits(&rc, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeUserInputBuf, PixelFormat32bppARGB, &data);
			if (result == Gdiplus::Ok) {

				BitmapData src(width, height, buf);
				src.format = bitmapFormat_BGRA;
				dst.copyPixelsFrom(src);

				m_bitmap->UnlockBits(&data);

				return sl_true;
			}

			return sl_false;
		}
	}
	
    // override
	sl_bool writePixels(sl_uint32 x, sl_uint32 y, const BitmapData& _src)
	{
		sl_uint32 w = getBitmapWidth();
		sl_uint32 h = getBitmapHeight();
		if (x >= w || y >= h) {
			return sl_false;
		}

		BitmapData src(_src);
		sl_uint32 width = src.width;
		sl_uint32 height = src.height;

		src.fillDefaultValues();

		if (width > w - x) {
			width = w - x;
		}
		if (height > h - y) {
			height = h - y;
		}
		if (width == 0 || height == 0) {
			return sl_true;
		}

		Gdiplus::Rect rc(x, y, width, height);
		Gdiplus::Status result;

		if (src.format == bitmapFormat_BGRA) {

			Gdiplus::BitmapData data;
			data.Width = width;
			data.Height = height;
			data.PixelFormat = PixelFormat32bppARGB;
			data.Stride = src.pitch;
			data.Scan0 = src.data;

			result = m_bitmap->LockBits(&rc, Gdiplus::ImageLockModeWrite | Gdiplus::ImageLockModeUserInputBuf, PixelFormat32bppARGB, &data);
			if (result == Gdiplus::Ok) {
				result = m_bitmap->UnlockBits(&data);
				return result == Gdiplus::Ok;
			}
			
		} else {
			
			SLIB_SCOPED_BUFFER(Color, 65536, buf, width*height);
			if (!buf) {
				return sl_false;
			}

			Gdiplus::BitmapData data;
			data.Width = width;
			data.Height = height;
			data.Scan0 = buf;
			data.Stride = width << 2;
			data.PixelFormat = PixelFormat32bppARGB;

			result = m_bitmap->LockBits(&rc, Gdiplus::ImageLockModeWrite | Gdiplus::ImageLockModeUserInputBuf, PixelFormat32bppARGB, &data);

			if (result == Gdiplus::Ok) {

				BitmapData dst(width, height, buf);
				dst.format = bitmapFormat_BGRA;
				dst.copyPixelsFrom(src);

				m_bitmap->UnlockBits(&data);
			}
			return sl_true;
		}
		return sl_false;
	}
	
    // override
	sl_bool resetPixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, const Color& color)
	{
		sl_uint32 w = getBitmapWidth();
		sl_uint32 h = getBitmapHeight();
		if (x >= w || y >= h) {
			return sl_false;
		}
		if (width > w - x) {
			width = w - x;
		}
		if (height > h - y) {
			height = h - y;
		}
		if (width == 0 || height == 0) {
			return sl_true;
		}
		
		Gdiplus::Rect rc(x, y, width, height);
		
		Gdiplus::BitmapData data;
		data.Width = width;
		data.Height = height;
		data.PixelFormat = PixelFormat32bppARGB;
		data.Stride = width << 2;
		data.Scan0 = sl_null;
		Gdiplus::Status result = m_bitmap->LockBits(&rc, Gdiplus::ImageLockModeWrite, PixelFormat32bppARGB, &data);
		if (result == Gdiplus::Ok) {
			sl_uint32 size = width * height;
			Color* pixels = (Color*)(data.Scan0);
			for (sl_uint32 i = 0; i < size; i++) {
				pixels[i] = color;
			}
			result = m_bitmap->UnlockBits(&data);
			return result == Gdiplus::Ok;
		}
		return sl_false;
	}
	
    // override
	Ref<Canvas> getCanvas()
	{
		Ref<Canvas> ret;
		Gdiplus::Graphics* g = new Gdiplus::Graphics(m_bitmap);
		if (g) {
			sl_uint32 w = getBitmapWidth();
			sl_uint32 h = getBitmapHeight();
			Rectangle rc = { 0, 0, (sl_real)w, (sl_real)h };
			ret = UIPlatform::createCanvas(g, w, h, &rc, sl_true, this);
		}
		return ret;
	}
	
    // override
	void onDraw(Canvas* _canvas, const Rectangle& rectDst, const Rectangle& rectSrc)
	{
		Gdiplus::Graphics* graphics = UIPlatform::getCanvasHandle(_canvas);
		if (!graphics) {
			return;
		}
		Gdiplus::RectF rcDst(rectDst.left, rectDst.top, rectDst.getWidth(), rectDst.getHeight());
		graphics->DrawImage(m_bitmap, rcDst
			, rectSrc.left, rectSrc.top, rectSrc.getWidth(), rectSrc.getHeight()
			, Gdiplus::UnitPixel);
	}
};

Ref<Bitmap> UI::createBitmap(sl_uint32 width, sl_uint32 height)
{
	return _Gdiplus_Bitmap::create(width, height);
}

Ref<Drawable> UI::createDrawableFromBitmap(const Ref<Bitmap>& bitmap)
{
	return Ref<Drawable>::null();
}

Ref<Bitmap> UI::loadBitmapFromMemory(const void* mem, sl_size size)
{
	return _Gdiplus_Bitmap::loadFromMemory(mem, size);
}

SLIB_UI_NAMESPACE_END

#endif
