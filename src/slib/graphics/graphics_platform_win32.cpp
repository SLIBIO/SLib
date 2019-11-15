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

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "slib/graphics/platform.h"

#include "slib/core/scoped.h"
#include "slib/core/safe_static.h"

#pragma comment(lib, "gdiplus.lib")

namespace slib
{

	void GraphicsPlatform::startGdiplus()
	{
		static sl_bool flagStarted = sl_false;
		if (flagStarted) {
			return;
		}
		SLIB_STATIC_SPINLOCKER(lock)
		if (flagStarted) {
			return;
		}
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		flagStarted = sl_true;
	}

	COLORREF GraphicsPlatform::getColorRef(const Color& color)
	{
		return (COLORREF)(color.getBGR());
	}

	Color GraphicsPlatform::getColorFromColorRef(COLORREF cr)
	{
		Color ret;
		ret.setBGR((sl_uint32)cr);
		ret.a = 255;
		return ret;
	}

	HBITMAP GraphicsPlatform::createDIB(const Ref<Drawable>& drawable)
	{
		if (drawable.isNull()) {
			return NULL;
		}

		Ref<Bitmap> bitmap = drawable->toBitmap();
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

	HICON GraphicsPlatform::createHICON(const Ref<Drawable>& drawable, sl_bool flagCursor, sl_uint32 xHotspot, sl_uint32 yHotspot)
	{
		if (drawable.isNull()) {
			return NULL;
		}
		Ref<Bitmap> bitmap = drawable->toBitmap();
		if (bitmap.isNull()) {
			return NULL;
		}
		sl_uint32 width = bitmap->getWidth();
		sl_uint32 height = bitmap->getHeight();
		if (width <= 0 || height <= 0) {
			return NULL;
		}
		HBITMAP dib = createDIB(bitmap);
		if (dib) {
			HICON hIcon = NULL;
			sl_uint32 maskWidth = BitmapData::calculatePitchAlign4(width, 1);
			sl_uint32 size = maskWidth * height;
			SLIB_SCOPED_BUFFER(sl_uint8, 4096, bits, size);
			Base::resetMemory(bits, 0xFF, size);
			HBITMAP mask = CreateBitmap(width, height, 1, 1, bits);
			if (mask) {
				ICONINFO info;
				info.fIcon = flagCursor ? FALSE : TRUE;
				info.xHotspot = xHotspot;
				info.yHotspot = yHotspot;
				info.hbmMask = mask;
				info.hbmColor = dib;
				hIcon = CreateIconIndirect(&info);
				DeleteObject(mask);
			}
			DeleteObject(dib);
			return hIcon;
		}
		return NULL;
	}

}

#endif
