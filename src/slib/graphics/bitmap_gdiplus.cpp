#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/graphics/bitmap.h"

#include "../../../inc/slib/graphics/platform.h"
#include "../../../inc/slib/core/scoped.h"

#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "gdiplus.lib")

namespace slib
{

	class _Gdiplus_Bitmap : public Bitmap
	{
		SLIB_DECLARE_OBJECT
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
			if (width > 0 && height > 0) {
				Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(width, height, PixelFormat32bppARGB);
				if (bitmap) {
					if (bitmap->GetWidth() == width && bitmap->GetHeight() == height) {
						Ref<_Gdiplus_Bitmap> ret = new _Gdiplus_Bitmap();
						if (ret.isNotNull()) {
							ret->m_bitmap = bitmap;
							return ret;
						}
					}
					delete bitmap;
				}
			}
			return sl_null;
		}
		
		static Ref<_Gdiplus_Bitmap> loadFromMemory(const void* mem, sl_size size)
		{
			IStream* stream = ::SHCreateMemStream((BYTE*)mem, (sl_uint32)size);
			if (stream) {
				Gdiplus::Bitmap* bitmap = new Gdiplus::Bitmap(stream);
				stream->Release();
				if (bitmap && bitmap->GetWidth() > 0 && bitmap->GetHeight() > 0) {
					Ref<_Gdiplus_Bitmap> ret = new _Gdiplus_Bitmap();
					if (ret.isNotNull()) {
						ret->m_bitmap = bitmap;
						return ret;
					}
					delete bitmap;
				}
			}
			return sl_null;
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

			if (BitmapFormats::getBitsPerSample(dst.format) == 32 && BitmapFormats::getPlanesCount(dst.format) == 1) {

				Gdiplus::BitmapData data;
				data.Width = width;
				data.Height = height;
				data.Scan0 = dst.data;
				data.Stride = dst.pitch;
				data.PixelFormat = PixelFormat32bppARGB;

				BitmapData src(dst);
				src.format = BitmapFormat::BGRA;

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
					src.format = BitmapFormat::BGRA;
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

			if (src.format == BitmapFormat::BGRA) {

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
					dst.format = BitmapFormat::BGRA;
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
				DWORD _color = color.getARGB();
				sl_uint8* row = (sl_uint8*)(data.Scan0);
				for (sl_uint32 i = 0; i < height; i++) {
					DWORD* pixels = (DWORD*)row;
					for (sl_uint32 j = 0; j < width; j++) {
						pixels[j] = _color;
					}
					row += data.Stride;
				}
				result = m_bitmap->UnlockBits(&data);
				return result == Gdiplus::Ok;
			}
			return sl_false;
		}
		
		// override
		Ref<Canvas> getCanvas()
		{
			Gdiplus::Graphics* g = new Gdiplus::Graphics(m_bitmap);
			if (g) {
				sl_uint32 w = getBitmapWidth();
				sl_uint32 h = getBitmapHeight();
				return GraphicsPlatform::createCanvas(CanvasType::Bitmap, g, w, h, sl_true, this);
			}
			return sl_null;
		}
		
		// override
		void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param)
		{
			GraphicsPlatform::drawImage(canvas, rectDst, m_bitmap, rectSrc, param);
		}
	};

	SLIB_DEFINE_OBJECT(_Gdiplus_Bitmap, Bitmap)

	Ref<Bitmap> Bitmap::create(sl_uint32 width, sl_uint32 height)
	{
		return _Gdiplus_Bitmap::create(width, height);
	}

	Ref<Bitmap> Bitmap::loadFromMemory(const void* mem, sl_size size)
	{
		return _Gdiplus_Bitmap::loadFromMemory(mem, size);
	}

	Gdiplus::Bitmap* GraphicsPlatform::getBitmapHandle(Bitmap* _bitmap)
	{
		if (_Gdiplus_Bitmap* bitmap = CastInstance<_Gdiplus_Bitmap>(_bitmap)) {
			return bitmap->m_bitmap;
		}
		return NULL;
	}

	HBITMAP GraphicsPlatform::createDIBFromBitmap(const Ref<Bitmap>& bitmap)
	{
		if (bitmap.isNotNull() && bitmap->isNotEmpty()) {

			BITMAPINFOHEADER bi;
			bi.biSize = sizeof(bi);
			bi.biWidth = bitmap->getWidth();
			bi.biHeight = bitmap->getHeight();
			bi.biPlanes = 1;
			bi.biBitCount = 32;
			bi.biCompression = BI_RGB;
			bi.biSizeImage = 0;
			bi.biXPelsPerMeter = 1000;
			bi.biYPelsPerMeter = 1000;
			bi.biClrImportant = 0;
			bi.biClrUsed = 0;

			void* buf;
			HBITMAP hbm = ::CreateDIBSection(NULL, (BITMAPINFO*)&bi, DIB_RGB_COLORS, &buf, NULL, 0);

			if (hbm) {
				BitmapData bd;
				bd.format = BitmapFormat::BGRA_PA;
				bd.width = bitmap->getWidth();
				bd.height = bitmap->getHeight();
				bd.data = (char*)buf + bd.width * 4 * (bd.height - 1);
				bd.pitch = -((int)bd.width * 4);
				if (bitmap->readPixels(0, 0, bd)) {
					return hbm;
				}
				::DeleteObject(hbm);
			}
		}
		return NULL;
	}

}

#endif
