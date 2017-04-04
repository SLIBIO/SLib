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

#if defined(SLIB_PLATFORM_IS_TIZEN)

#include "slib/graphics/bitmap.h"
#include "slib/graphics/image.h"

#include "slib/graphics/platform.h"

namespace slib
{

	class _Cairo_Bitmap : public Bitmap
	{
		SLIB_DECLARE_OBJECT

	public:
		cairo_surface_t* m_bitmap;

	public:
		_Cairo_Bitmap()
		{
			m_bitmap = sl_null;
		}

		~_Cairo_Bitmap()
		{
			if (m_bitmap) {
				::cairo_surface_destroy(m_bitmap);
			}
		}

	public:
		static Ref<_Cairo_Bitmap> create(sl_uint32 width, sl_uint32 height)
		{
			if (width > 0 && height > 0) {
				cairo_surface_t* bitmap = ::cairo_image_surface_create(CAIRO_FORMAT_ARGB32, width, height);
				if (bitmap) {
					Ref<_Cairo_Bitmap> ret = new _Cairo_Bitmap();
					if (ret.isNotNull()) {
						ret->m_bitmap = bitmap;
						return ret;
					}
				}
			}
			return sl_null;
		}

		// override
		sl_uint32 getBitmapWidth()
		{
			return ::cairo_image_surface_get_width(m_bitmap);
		}

		// override
		sl_uint32 getBitmapHeight()
		{
			return ::cairo_image_surface_get_height(m_bitmap);
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

			::cairo_surface_flush(m_bitmap);

			int pitch = ::cairo_image_surface_get_stride(m_bitmap);
			char* buf = (char*)(::cairo_image_surface_get_data(m_bitmap)) + pitch * y + (x << 2);

			BitmapData src;
			src.width = width;
			src.height = height;
			src.pitch = pitch;
			src.format = BitmapFormat::BGRA_PA;
			src.data = buf;

			dst.copyPixelsFrom(src);

			return sl_true;

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

			int pitch = ::cairo_image_surface_get_stride(m_bitmap);
			char* buf = (char*)(::cairo_image_surface_get_data(m_bitmap)) + pitch * y + (x << 2);

			BitmapData dst;
			dst.width = width;
			dst.height = height;
			dst.pitch = pitch;
			dst.format = BitmapFormat::BGRA_PA;
			dst.data = buf;

			dst.copyPixelsFrom(src);

			::cairo_surface_mark_dirty(m_bitmap);

			return sl_true;

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

			int pitch = ::cairo_image_surface_get_stride(m_bitmap);
			char* buf = (char*)(::cairo_image_surface_get_data(m_bitmap)) + pitch * y + (x << 2);

			Color _color = color;
			_color.convertNPAtoPA();
			sl_uint32 c = _color.getARGB();

			char* row = buf;
			for (sl_uint32 i = 0; i < height; i++) {
				sl_uint32* pixels = (sl_uint32*)row;
				for (sl_uint32 j = 0; j < width; j++) {
					pixels[j] = c;
				}
				row += pitch;
			}

			::cairo_surface_mark_dirty(m_bitmap);

			return sl_true;

		}

		// override
		Ref<Canvas> getCanvas()
		{
			cairo_t* g = ::cairo_create(m_bitmap);
			if (g) {
				sl_uint32 w = getBitmapWidth();
				sl_uint32 h = getBitmapHeight();
				return GraphicsPlatform::createCanvas(CanvasType::Bitmap, g, w, h);
			}
			return sl_null;
		}

		// override
		void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param)
		{
			::cairo_surface_flush(m_bitmap);
			GraphicsPlatform::drawImage(canvas, rectDst, m_bitmap, rectSrc, param);
		}

	};

	SLIB_DEFINE_OBJECT(_Cairo_Bitmap, Bitmap)

	Ref<Bitmap> Bitmap::create(sl_uint32 width, sl_uint32 height)
	{
		return _Cairo_Bitmap::create(width, height);
	}

	Ref<Bitmap> Bitmap::loadFromMemory(const void* mem, sl_size size)
	{
		return Bitmap::create(Image::loadFromMemory(mem, size));
	}

}

#endif
