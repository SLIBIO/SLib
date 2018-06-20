/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "slib/graphics/platform.h"

namespace slib
{

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
