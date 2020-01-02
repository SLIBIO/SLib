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

#ifndef CHECKHEADER_SLIB_GRAPHICS_IMAGE
#define CHECKHEADER_SLIB_GRAPHICS_IMAGE

#include "definition.h"

#include "constants.h"
#include "color.h"
#include "bitmap.h"

#include "../core/object.h"
#include "../core/memory.h"
#include "../core/string.h"

namespace slib
{
	
	class SLIB_EXPORT ImageDesc
	{
	public:
		sl_uint32 width;
		sl_uint32 height;
		sl_int32 stride;
		Color* colors;

		Ref<Referable> ref;

	public:
		ImageDesc();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(ImageDesc)

	};
	
	class SLIB_EXPORT Image : public Bitmap
	{
		SLIB_DECLARE_OBJECT

	protected:
		Image();

		~Image();

	public:
		static Ref<Image> createStatic(const ImageDesc& desc);

		static Ref<Image> createStatic(sl_uint32 width, sl_uint32 height, const Color* pixels, sl_int32 stride = 0, Referable* ref = sl_null);

		static Ref<Image> create(sl_uint32 width, sl_uint32 height, const Color* pixels = sl_null, sl_int32 strideSource = 0);

		static Ref<Image> create(const ImageDesc& desc);

		static Ref<Image> create(const ImageDesc& desc, RotationMode rotate, FlipMode flip = FlipMode::None);
		
		static Ref<Image> create(const BitmapData& bitmapData);

		static Ref<Image> createCopy(const Ref<Image>& image);
		
		static Ref<Image> createCopy(const Ref<Image>& image, RotationMode rotate, FlipMode flip = FlipMode::None);

		static Ref<Image> createCopyBitmap(const Ref<Bitmap>& bitmap);

		static Ref<Image> createCopyBitmap(const Ref<Bitmap>& bitmap, sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height);

	public:
		sl_uint32 getWidth() const;

		sl_uint32 getHeight() const;
	
		sl_bool isEmpty() const;
	
		sl_bool isNotEmpty() const;

		sl_int32 getStride() const;

		Color* getColors() const;

		Color* getColorsAt(sl_int32 x, sl_int32 y) const;

		Color& getPixel(sl_int32 x, sl_int32 y) const;

		void getDesc(ImageDesc& desc) const;
	

		sl_uint32 getBitmapWidth() override;

		sl_uint32 getBitmapHeight() override;
	
		sl_bool readPixels(sl_uint32 x, sl_uint32 y, BitmapData& desc) override;

		sl_bool writePixels(sl_uint32 x, sl_uint32 y, const BitmapData& desc) override;

		sl_bool resetPixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, const Color& color) override;

		sl_bool readPixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, Color* colors, sl_int32 stride = 0);

		sl_bool writePixels(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height, const Color* colors, sl_int32 stride = 0);

		sl_bool resetPixels(const Color& color);

		Ref<Canvas> getCanvas() override;

		void fillColor(const Color& color);

		void tintColor(const Color& color);


		static void draw(ImageDesc& dst, const ImageDesc& src, BlendMode blend = BlendMode::Copy, StretchMode stretch = StretchMode::Default);

		void drawImage(sl_int32 dx, sl_int32 dy, sl_int32 dw, sl_int32 dh,
					   const Ref<Image>& src, sl_int32 sx, sl_int32 sy, sl_int32 sw, sl_int32 sh,
					   BlendMode blend = BlendMode::Copy, StretchMode stretch = StretchMode::Default);

		void drawImage(const Rectanglei& rectDst,
					   const Ref<Image>& src, const Rectanglei& rectSrc,
					   BlendMode blend = BlendMode::Copy, StretchMode stretch = StretchMode::Default);

		void drawImage(sl_int32 dx, sl_int32 dy, sl_int32 dw, sl_int32 dh,
					   const Ref<Image>& src, sl_int32 sx, sl_int32 sy,
					   BlendMode blend = BlendMode::Copy, StretchMode stretch = StretchMode::Default);

		void copyBitmap(const Ref<Bitmap>& bitmap, sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height);

		
		Ref<Drawable> subDrawable(sl_real x, sl_real y, sl_real width, sl_real height) override;

		Ref<Image> sub(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height) const;

		Ref<Image> stretch(sl_uint32 width, sl_uint32 height, StretchMode stretch = StretchMode::Default) const;

		Ref<Image> stretchToSmall(sl_uint32 requiredWidth, sl_uint32 requiredHeight, sl_bool flagKeepAspectRatio = sl_true, StretchMode stretch = StretchMode::Default) const;

		Ref<Image> stretchToSmall(sl_uint32 sampleSize) const;

		Ref<Image> rotateImage(RotationMode rotate, FlipMode flip = FlipMode::None) const;
		
		Ref<Image> flipImage(FlipMode flip) const;

		Ref<Image> duplicate() const;

		Ref<Image> duplicate(RotationMode rotate, FlipMode flip = FlipMode::None) const;
		
		Ref<Image> duplicate(FlipMode flip) const;
		
		Ref<Image> duplicate(sl_uint32 newWidth, sl_uint32 newHeight, StretchMode stretch = StretchMode::Default) const;
		

		static ImageFileType getFileType(const void* mem, sl_size size);

		static ImageFileType getFileType(Memory mem);

		static Ref<Image> loadFromMemory(const void* mem, sl_size size);

		static Ref<Image> loadFromMemory(Memory mem);

		static Ref<Image> loadFromFile(const String& filePath);

		static Ref<Image> loadFromAsset(const String& path);
		
		static Ref<AnimationDrawable> loadAnimationFromMemory(const void* mem, sl_size size);
		
		static Ref<AnimationDrawable> loadAnimationFromMemory(Memory mem);
		
		static Ref<AnimationDrawable> loadAnimationFromFile(const String& filePath);
		
		static Ref<AnimationDrawable> loadAnimationFromAsset(const String& path);
		
		
		static Ref<Image> loadSTB(const void* content, sl_size size);
		
		static Ref<AnimationDrawable> loadSTB_GIF(const void* content, sl_size size);
	
		
		static Ref<Image> loadPNG(const void* content, sl_size size);

		static Memory savePNG(const Ref<Image>& image);

		Memory savePNG();

		static sl_bool savePNG(const String& filePath, const Ref<Image>& image);

		sl_bool savePNG(const String& filePath);
		

		static Ref<Image> loadJPEG(const void* content, sl_size size);

		static Memory saveJPEG(const Ref<Image>& image, float quality = 0.5f);

		Memory saveJPEG(float quality = 0.5f);

		static sl_bool saveJPEG(const String& filePath, const Ref<Image>& image, float quality = 0.5f);

		sl_bool saveJPEG(const String& filePath, float quality = 0.5f);

		
		Ref<Drawable> getCustomDrawable();
		
		void setCustomDrawable(const Ref<Drawable>& drawable);
		
		sl_real getDrawableWidth() override;
		
		sl_real getDrawableHeight() override;
		
		sl_bool getAnimationInfo(DrawableAnimationInfo* info) override;


	protected:
		Ref<Drawable> getDrawableCache(Canvas* canvas);
	
		void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param) override;

		void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param) override;
		
	protected:
		ImageDesc m_desc;
		AtomicRef<Drawable> m_customDrawable;
		
	};

}

#endif
