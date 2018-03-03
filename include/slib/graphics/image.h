/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

		~ImageDesc();

	};
	
	class SLIB_EXPORT Image : public Bitmap
	{
		SLIB_DECLARE_OBJECT

	protected:
		Image();

		~Image();

	public:
		static Ref<Image> createStatic(const ImageDesc& desc);

		static Ref<Image> createStatic(sl_uint32 width, sl_uint32 height, const Color* pixels, sl_int32 stride = 0);

		static Ref<Image> createStatic(sl_uint32 width, sl_uint32 height, const Color* pixels, sl_int32 stride, Referable* ref);

		static Ref<Image> create(const ImageDesc& desc);

		static Ref<Image> create(sl_uint32 width, sl_uint32 height, const Color* pixels = sl_null, sl_int32 strideSource = 0);

		static Ref<Image> create(const BitmapData& bitmapData);
	
		static Ref<Image> create(const Ref<Bitmap>& bitmap);

		static Ref<Image> create(const Ref<Bitmap>& bitmap, sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height);

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


		Ref<Drawable> subDrawable(sl_real x, sl_real y, sl_real width, sl_real height) override;

		Ref<Image> sub(sl_uint32 x, sl_uint32 y, sl_uint32 width, sl_uint32 height) const;

		Ref<Image> scale(sl_uint32 width, sl_uint32 height, StretchMode stretch = StretchMode::Default) const;

		Ref<Image> scaleToSmall(sl_uint32 requiredWidth, sl_uint32 requiredHeight, StretchMode stretch = StretchMode::Default) const;


		static ImageFileType getFileType(const void* mem, sl_size size);

		static ImageFileType getFileType(Memory mem);

		static Ref<Image> loadFromMemory(const void* mem, sl_size size, sl_uint32 width = 0, sl_uint32 height = 0);

		static Ref<Image> loadFromMemory(Memory mem, sl_uint32 width = 0, sl_uint32 height = 0);

		static Ref<Image> loadFromFile(const String& filePath, sl_uint32 width = 0, sl_uint32 height = 0);

		static Ref<Image> loadFromAsset(const String& path, sl_uint32 width = 0, sl_uint32 height = 0);
	
		
		static Ref<Image> loadFromPNG(const void* content, sl_size size);

		static Memory saveToPNG(const Ref<Image>& image);

		Memory saveToPNG();

		static sl_bool saveToPNG(const String& filePath, const Ref<Image>& image);

		sl_bool saveToPNG(const String& filePath);
		

		static Ref<Image> loadFromJPEG(const void* content, sl_size size);

		static Memory saveToJPEG(const Ref<Image>& image, float quality = 0.5f);

		Memory saveToJPEG(float quality = 0.5f);

		static sl_bool saveToJPEG(const String& filePath, const Ref<Image>& image, float quality = 0.5f);

		sl_bool saveToJPEG(const String& filePath, float quality = 0.5f);

		
	public:
		Ref<Drawable> getDrawableCache();
	
		void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param) override;

		void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param) override;

	protected:
		ImageDesc m_desc;
		
	};

}

#endif
