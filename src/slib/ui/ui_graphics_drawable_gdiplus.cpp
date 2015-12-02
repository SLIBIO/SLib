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
	
	_Gdiplus_ImageDrawable()
	{
	}
	
	~_Gdiplus_ImageDrawable()
	{
		if (m_flagFreeOnRelease) {
			delete m_image;
		}
	}
	
	static Ref<_Gdiplus_ImageDrawable> create(Gdiplus::Image* image, sl_bool flagFreeOnRelease, Referable* ref)
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
	
	sl_real getDrawableWidth()
	{
		return (sl_real)(m_image->GetWidth());
	}
	
	sl_real getDrawableHeight()
	{
		return (sl_real)(m_image->GetHeight());
	}
};

Ref<Drawable> UIPlatform::createImageDrawable(Gdiplus::Image* image, sl_bool flagFreeOnRelease, Referable* ref)
{
	Ref<_Gdiplus_ImageDrawable> ret = _Gdiplus_ImageDrawable::create(image, flagFreeOnRelease, ref);
	return Ref<Drawable>::from(ret);
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

	_Gdiplus_Bitmap()
	{
	}
		
	~_Gdiplus_Bitmap()
	{
		delete m_bitmap;
	}
	
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
	
	sl_uint32 getBitmapWidth()
	{
		return m_bitmap->GetWidth();
	}
	
	sl_uint32 getBitmapHeight()
	{
		return m_bitmap->GetHeight();
	}
	
	sl_bool readPixels(sl_uint32 x, sl_uint32 y, BitmapDesc& desc)
	{
		sl_uint32 w = getBitmapWidth();
		sl_uint32 h = getBitmapHeight();
		if (x >= w || y >= h) {
			return sl_false;
		}
		sl_uint32 width = desc.width;
		sl_uint32 height = desc.height;
		sl_uint32 pitch = desc.pitch;
		if (width > w - x || height > h - y) {
			return sl_false;
		}
		sl_uint32 bpp = Color::getModelBits(desc.colorModel);
		if (pitch == 0) {
			pitch = (sl_uint32)(Color::calculatePitchAlign1(width, bpp));
		}
		sl_uint8* pixels = (sl_uint8*)desc.data;
		Gdiplus::BitmapData data;
		data.Width = width;
		data.Height = height;
		data.Stride = pitch;
		data.Scan0 = pixels;
		Gdiplus::Rect rc(x, y, width, height);
		Gdiplus::Status result;
		
		sl_bool flagUseLineCopy = sl_false;
		if (bpp == 32) {
			data.PixelFormat = PixelFormat32bppARGB;
			result = m_bitmap->LockBits(&rc, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeUserInputBuf, PixelFormat32bppARGB, &data);
			if (result == Gdiplus::Ok) {
				m_bitmap->UnlockBits(&data);
				Color::convert(width, height, Color::BGRA, pixels, pitch, desc.colorModel, pixels, pitch);
				return sl_true;
			}
		} else if (bpp == 24) {
			data.PixelFormat = PixelFormat24bppRGB;
			result = m_bitmap->LockBits(&rc, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeUserInputBuf, PixelFormat24bppRGB, &data);
			if (result == Gdiplus::Ok) {
				m_bitmap->UnlockBits(&data);
				Color::convert(width, height, Color::BGR, pixels, pitch, desc.colorModel, pixels, pitch);
				return sl_true;
			}
		} else {
			flagUseLineCopy = sl_true;
		}
		
		if (flagUseLineCopy) {
			sl_uint32 pitchIn = width * 4;
			sl_uint32 nRowsUnit = 0x100000 / pitchIn;
			if (nRowsUnit == 0) {
				return sl_false;
			}
			SLIB_SCOPED_ARRAY(sl_uint8, buf, nRowsUnit * pitchIn);
			data.Stride = pitchIn;
			data.Scan0 = buf;
			for (sl_uint32 i = 0; i < height; i += nRowsUnit) {
				sl_uint32 h = height - i;
				if (h > nRowsUnit) {
					h = nRowsUnit;
				}
				rc.Y = y;
				rc.Height = h;
				data.Height = h;
				result = m_bitmap->LockBits(&rc, Gdiplus::ImageLockModeRead | Gdiplus::ImageLockModeUserInputBuf, PixelFormat32bppARGB, &data);
				if (result == Gdiplus::Ok) {
					m_bitmap->UnlockBits(&data);
					Color::convert(width, h, Color::BGRA, buf, pitchIn, desc.colorModel, pixels, pitch);
				} else {
					return sl_false;
				}
				y += h;
				pixels += pitch * h;
			}
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool writePixels(sl_uint32 x, sl_uint32 y, const BitmapDesc& desc)
	{
		sl_uint32 w = getBitmapWidth();
		sl_uint32 h = getBitmapHeight();
		if (x >= w || y >= h) {
			return sl_false;
		}
		sl_uint32 width = desc.width;
		sl_uint32 height = desc.height;
		sl_uint32 pitch = desc.pitch;
		if (width > w - x || height > h - y) {
			return sl_false;
		}
		sl_uint32 bpp = Color::getModelBits(desc.colorModel);
		if (pitch == 0) {
			pitch = (sl_uint32)(Color::calculatePitchAlign1(width, bpp));
		}
		
		const sl_uint8* pixels = (const sl_uint8*)desc.data;
		Gdiplus::Rect rc(x, y, width, height);
		if (desc.colorModel == Color::BGRA) {
			Gdiplus::BitmapData data;
			data.Width = width;
			data.Height = height;
			data.PixelFormat = PixelFormat32bppARGB;
			data.Stride = pitch;
			data.Scan0 = (void*)pixels;
			Gdiplus::Status result = m_bitmap->LockBits(&rc, Gdiplus::ImageLockModeWrite | Gdiplus::ImageLockModeUserInputBuf, PixelFormat32bppARGB, &data);
			if (result == Gdiplus::Ok) {
				result = m_bitmap->UnlockBits(&data);
				return result == Gdiplus::Ok;
			}
		} else {
			sl_uint32 pitchOut = width * 4;
			sl_uint32 nRowsUnit = 0x100000 / pitchOut;
			if (nRowsUnit == 0) {
				return sl_false;
			}
			SLIB_SCOPED_ARRAY(sl_uint8, buf, nRowsUnit * pitchOut);
			Gdiplus::BitmapData data;
			data.Stride = pitchOut;
			data.Scan0 = buf;
			for (sl_uint32 i = 0; i < height; i += nRowsUnit) {
				sl_uint32 h = height - i;
				if (h > nRowsUnit) {
					h = nRowsUnit;
				}
				rc.Y = y;
				rc.Height = h;
				data.Height = h;
				Gdiplus::Status result = m_bitmap->LockBits(&rc, Gdiplus::ImageLockModeWrite | Gdiplus::ImageLockModeUserInputBuf, PixelFormat32bppARGB, &data);
				Color::convert(width, h, desc.colorModel, pixels, pitch, Color::BGRA, buf, pitchOut);
				if (result == Gdiplus::Ok) {
					m_bitmap->UnlockBits(&data);
				} else {
					return sl_false;
				}
				y += h;
				pixels += pitch * h;
			}
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool resetPixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, const Color& color)
	{
		sl_uint32 w = getBitmapWidth();
		sl_uint32 h = getBitmapHeight();
		if (x >= w || y >= h) {
			return sl_false;
		}
		if (width > w - x || height > h - y) {
			return sl_false;
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
	Ref<_Gdiplus_Bitmap> ret = _Gdiplus_Bitmap::create(width, height);
	return Ref<Bitmap>::from(ret);
}

Ref<Drawable> UI::createDrawableFromBitmap(const Ref<Bitmap>& bitmap)
{
	return Ref<Drawable>::null();
}

Ref<Bitmap> UI::loadBitmapFromMemory(const void* mem, sl_size size)
{
	Ref<_Gdiplus_Bitmap> ret = _Gdiplus_Bitmap::loadFromMemory(mem, size);
	return Ref<Bitmap>::from(ret);
}

SLIB_UI_NAMESPACE_END

#endif
