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

#include "slib/graphics/bitmap.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(BitmapCache, Object)

	BitmapCache::BitmapCache()
	{
	}

	BitmapCache::~BitmapCache()
	{
	}

	SLIB_DEFINE_OBJECT(Bitmap, Drawable)

	Bitmap::Bitmap()
	{
	}

	Bitmap::~Bitmap()
	{
	}

	sl_real Bitmap::getDrawableWidth()
	{
		return (sl_real)(getBitmapWidth());
	}

	sl_real Bitmap::getDrawableHeight()
	{
		return (sl_real)(getBitmapHeight());
	}

	sl_uint32 Bitmap::getWidth()
	{
		return getBitmapWidth();
	}

	sl_uint32 Bitmap::getHeight()
	{
		return getBitmapHeight();
	}

	sl_bool Bitmap::isEmpty()
	{
		return getBitmapWidth() == 0 || getBitmapHeight() == 0;
	}

	sl_bool Bitmap::isNotEmpty()
	{
		return getBitmapWidth() != 0 && getBitmapHeight() != 0;
	}

	void Bitmap::update(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height)
	{
		m_drawableCached.setNull();
		{
			Ref<BitmapCache> cache = m_renderingTextureCached;
			if (cache.isNotNull()) {
				cache->update(x, y, width, height);
			}
		}
	}

	void Bitmap::update()
	{
		m_drawableCached.setNull();
		{
			Ref<BitmapCache> cache = m_renderingTextureCached;
			if (cache.isNotNull()) {
				cache->update(0, 0, getBitmapWidth(), getBitmapHeight());
			}		
		}
	}

	sl_bool Bitmap::readPixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, Color* colors, sl_int32 stride)
	{
		BitmapData bitmapData(width, height, colors, stride);
		return readPixels(x, y, bitmapData);
	}

	sl_bool Bitmap::writePixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, const Color* colors, sl_int32 stride)
	{
		BitmapData bitmapData(width, height, colors, stride);
		return writePixels(x, y, bitmapData);
	}

	sl_bool Bitmap::resetPixels(const Color& color)
	{
		return resetPixels(0, 0, getWidth(), getHeight(), color);
	}

}
