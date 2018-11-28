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

#ifndef CHECKHEADER_SLIB_GRAPHICS_BITMAP
#define CHECKHEADER_SLIB_GRAPHICS_BITMAP

#include "definition.h"

#include "color.h"
#include "bitmap_data.h"

#include "drawable.h"
#include "canvas.h"

namespace slib
{
	
	class ImageDesc;
	class Image;
	
	class BitmapCache : public Object
	{
		SLIB_DECLARE_OBJECT

	protected:
		BitmapCache();

		~BitmapCache();

	public:
		virtual void update(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height) = 0;

	};
	
	class SLIB_EXPORT Bitmap : public Drawable
	{
		SLIB_DECLARE_OBJECT
	
	protected:
		Bitmap();

		~Bitmap();

	public:
		static Ref<Bitmap> create(sl_uint32 width, sl_uint32 height);

		static Ref<Bitmap> create(const ImageDesc& desc);

		static Ref<Bitmap> create(const Ref<Image>& image);

		static Ref<Bitmap> loadFromMemory(const void* mem, sl_size size);

		static Ref<Bitmap> loadFromMemory(const Memory& mem);

		static Ref<Bitmap> loadFromFile(const String& filePath);

		static Ref<Bitmap> loadFromAsset(const String& path);
	
	public:
		virtual sl_uint32 getBitmapWidth() = 0;

		virtual sl_uint32 getBitmapHeight() = 0;


		virtual sl_bool readPixels(sl_uint32 x, sl_uint32 y, BitmapData& data) = 0;

		virtual sl_bool writePixels(sl_uint32 x, sl_uint32 y, const BitmapData& data) = 0;

		virtual sl_bool resetPixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, const Color& color) = 0;


		virtual Ref<Canvas> getCanvas() = 0;

	public:
		sl_real getDrawableWidth() override;

		sl_real getDrawableHeight() override;

	public:
		sl_uint32 getWidth();

		sl_uint32 getHeight();
	
		sl_bool isEmpty();
	
		sl_bool isNotEmpty();

		void update(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height);

		void update();

	public:
		sl_bool readPixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, Color* colors, sl_int32 stride = 0);
	
		sl_bool writePixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, const Color* colors, sl_int32 stride = 0);

		sl_bool resetPixels(const Color& color);

	protected:
		AtomicRef<Drawable> m_drawableCached;
		AtomicRef<BitmapCache> m_renderingTextureCached;

	};

}

#endif
