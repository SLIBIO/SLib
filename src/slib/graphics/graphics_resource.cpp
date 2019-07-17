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

#include "slib/graphics/resource.h"

#include "slib/graphics/util.h"
#include "slib/core/scoped.h"

namespace slib
{
	
	namespace priv
	{
		namespace graphics_resource
		{
			
			sl_uint32 g_screenWidth = 0;
			sl_uint32 g_screenHeight = 0;

			Ref<Image> ImageEntry::getImage()
			{
				Ref<Image>& s = *((Ref<Image>*)((void*)&image));
				if (flag_load) {
					return s;
				}
				SpinLocker locker((SpinLock*)((void*)&lock));
				if (flag_load) {
					return s;
				}
				s = Image::loadFromMemory(source_bytes, source_size);
				if (s.isNotNull()) {
					s->setCustomDrawable(Image::loadAnimationFromMemory(source_bytes, source_size));
				}
				flag_load = sl_true;
				return s;
			}

			Ref<Image> ImageEntry::getMatchingImage(sl_uint32 reqWidth, sl_uint32 reqHeight)
			{
				if (reqWidth == 0 || reqHeight == 0) {
					return sl_null;
				}
				Ref<Image>& s = *((Ref<Image>*)((void*)&image));
				if (flag_load) {
					return s;
				}
				SpinLocker locker((SpinLock*)((void*)&lock));
				if (flag_load) {
					return s;
				}
				if (s.isNotNull()) {
					if (s->getWidth() >= reqWidth && s->getHeight() >= reqHeight) {
						return s;
					}
				}
				sl_uint32 zoomLevel = 1;
				sl_uint32 widthZoom = width;
				sl_uint32 heightZoom = height;
				while (widthZoom >= reqWidth && heightZoom >= reqHeight) {
					widthZoom >>= 1;
					heightZoom >>= 1;
					zoomLevel <<= 1;
				}
				Ref<Drawable> animation = Image::loadAnimationFromMemory(source_bytes, source_size);
				if (animation.isNotNull() || zoomLevel <= 2) {
					s = Image::loadFromMemory(source_bytes, source_size);
					if (s.isNotNull()) {
						s->setCustomDrawable(animation);
					}
					flag_load = sl_true;
				} else {
					Ref<Image> t = Image::loadFromMemory(source_bytes, source_size);
					if (t.isNotNull()) {
						zoomLevel >>= 1;
						s = t->stretchToSmall(zoomLevel);
					}
				}
				return s;
			}

			
			FreeImageContext::FreeImageContext(ImageEntry* entries)
			{
				m_entries = entries;
			}

			FreeImageContext::~FreeImageContext()
			{
				ImageEntry* entry;
				entry = m_entries;
				while (entry->flagValid) {
					(*((Ref<Image>*)((void*)&(entry->image)))).Ref<Image>::~Ref();
					entry->flagValid = sl_false;
					entry++;
				}
			}

			Ref<Image> GetImage(ImageEntry* entries, sl_uint32 requiredWidth, sl_uint32 requiredHeight)
			{
				ImageEntry* entry;
				if (requiredWidth == 0 && requiredHeight == 0) {
					entry = entries;
					if (entry->flagValid) {
						return entry->getImage();
					}
					return sl_null;
				}
				
				sl_uint32 minSize = 0;
				ImageEntry* minEntry = sl_null;
				entry = entries;
				while (entry->flagValid) {
					sl_uint32 width = entry->width;
					sl_uint32 height = entry->height;
					if (width >= requiredWidth && height >= requiredHeight) {
						sl_uint32 size = width * height;
						if (minSize == 0 || size < minSize) {
							minSize = size;
							minEntry = entry;
						}
					}
					entry++;
				}
				if (minEntry) {
					return minEntry->getMatchingImage(requiredWidth, requiredHeight);
				}
				sl_uint32 maxSize = 0;
				ImageEntry* maxEntry = sl_null;
				entry = entries;
				while (entry->flagValid) {
					sl_uint32 width = entry->width;
					sl_uint32 height = entry->height;
					if (width < requiredWidth || height < requiredHeight) {
						sl_uint32 size = width * height;
						if (maxSize == 0 || size > maxSize) {
							maxSize = size;
							maxEntry = entry;
						}
					}
					entry++;
				}
				if (maxEntry) {
					return maxEntry->getMatchingImage(requiredWidth, requiredHeight);
				}
				return sl_null;
			}

			List< Ref<Image> > GetImages(ImageEntry* entries)
			{
				List< Ref<Image> > ret;
				ImageEntry* entry;
				entry = entries;
				while (entry->flagValid) {
					Ref<Image> image = entry->getImage();
					if (image.isNotNull()) {
						ret.add_NoLock(image);
					}
					entry++;
				}
				return ret;
			}


			class ImageDrawable : public Drawable
			{
				SLIB_DECLARE_OBJECT
				
			private:
				ImageEntry* m_entries;
				sl_uint32 m_width;
				sl_uint32 m_height;

			public:
				ImageDrawable(ImageEntry* entries, sl_uint32 width, sl_uint32 height)
				{
					m_entries = entries;
					m_width = width;
					m_height = height;
				}
				
			public:
				sl_real getDrawableWidth() override
				{
					return (sl_real)m_width;
				}
				
				sl_real getDrawableHeight() override
				{
					return (sl_real)m_height;
				}
				
				void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param) override
				{
					Rectangle rectDstWhole = GraphicsUtil::transformRectangle(rectDst, rectSrc, Rectangle(0, 0, (float)m_width, (float)m_height));
					sl_int32 width = (sl_int32)(rectDstWhole.getWidth());
					sl_int32 height = (sl_int32)(rectDstWhole.getHeight());
					if (width > 0 && height > 0) {
						Ref<Image> image = GetImage(m_entries, width, height);
						if (image.isNotNull()) {
							float fx = (float)(image->getWidth()) / (float)(m_width);
							float fy = (float)(image->getHeight()) / (float)(m_height);
							Rectangle r;
							r.left = rectSrc.left * fx;
							r.top = rectSrc.top * fy;
							r.right = rectSrc.right * fx;
							r.bottom = rectSrc.bottom * fy;
							canvas->draw(rectDst, image, r, param);
						}
					}
				}

				void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param) override
				{
					sl_int32 width = (sl_int32)(rectDst.getWidth());
					sl_int32 height = (sl_int32)(rectDst.getHeight());
					if (width > 0 && height > 0) {
						Ref<Image> image = GetImage(m_entries, width, height);
						if (image.isNotNull()) {
							canvas->draw(rectDst, image, param);
						}
					}
				}
				
				sl_bool getAnimationInfo(DrawableAnimationInfo* info) override
				{
					if (m_entries->flagValid) {
						Ref<Image> image = m_entries->getImage();
						if (image.isNotNull()) {
							return image->getAnimationInfo(info);
						}
					}
					return sl_false;
				}
				
			};

			SLIB_DEFINE_OBJECT(ImageDrawable, Drawable)


			class SimpleImageDrawable : public Drawable
			{
				SLIB_DECLARE_OBJECT
				
			private:
				ImageEntry* m_entry;
				sl_uint32 m_width;
				sl_uint32 m_height;
				
			public:
				SimpleImageDrawable(ImageEntry* entry, sl_uint32 width, sl_uint32 height)
				{
					m_entry = entry;
					m_width = width;
					m_height = height;
				}
				
			public:
				sl_real getDrawableWidth() override
				{
					return (sl_real)m_width;
				}
				
				sl_real getDrawableHeight() override
				{
					return (sl_real)m_height;
				}
				
				void onDraw(Canvas* canvas, const Rectangle& rectDst, const Rectangle& rectSrc, const DrawParam& param) override
				{
					Rectangle rectDstWhole = GraphicsUtil::transformRectangle(rectDst, rectSrc, Rectangle(0, 0, (float)m_width, (float)m_height));
					sl_int32 width = (sl_int32)(rectDstWhole.getWidth());
					sl_int32 height = (sl_int32)(rectDstWhole.getHeight());
					if (width > 0 && height > 0) {
						Ref<Image> image = m_entry->getMatchingImage(width, height);
						if (image.isNotNull()) {
							float fx = (float)(image->getWidth()) / (float)(m_width);
							float fy = (float)(image->getHeight()) / (float)(m_height);
							Rectangle r;
							r.left = rectSrc.left * fx;
							r.top = rectSrc.top * fy;
							r.right = rectSrc.right * fx;
							r.bottom = rectSrc.bottom * fy;
							canvas->draw(rectDst, image, r, param);
						}
					}
				}
				
				void onDrawAll(Canvas* canvas, const Rectangle& rectDst, const DrawParam& param) override
				{
					sl_int32 width = (sl_int32)(rectDst.getWidth());
					sl_int32 height = (sl_int32)(rectDst.getHeight());
					if (width > 0 && height > 0) {
						Ref<Image> image = m_entry->getMatchingImage(width, height);
						if (image.isNotNull()) {
							canvas->draw(rectDst, image, param);
						}
					}
				}
				
				sl_bool getAnimationInfo(DrawableAnimationInfo* info) override
				{
					Ref<Image> image = m_entry->getImage();
					if (image.isNotNull()) {
						return image->getAnimationInfo(info);
					}
					return sl_false;
				}
				
			};

			SLIB_DEFINE_OBJECT(SimpleImageDrawable, Drawable)

			Ref<Drawable> GetDrawable(ImageEntry* entries, sl_uint32 width, sl_uint32 height)
			{
				if (entries->flagValid) {
					if (!(entries[1].flagValid)) {
						return new SimpleImageDrawable(entries, width, height);
					} else {
						return new ImageDrawable(entries, width, height);
					}
				}
				return sl_null;
			}
			
		}
	}

	using namespace priv::graphics_resource;

	sl_uint32 GraphicsResource::getScreenWidth()
	{
		return g_screenWidth;
	}
	
	void GraphicsResource::setScreenWidth(sl_uint32 width)
	{
		g_screenWidth = width;
	}
	
	sl_uint32 GraphicsResource::getScreenHeight()
	{
		return g_screenHeight;
	}
	
	void GraphicsResource::setScreenHeight(sl_uint32 height)
	{
		g_screenHeight = height;
	}
	
}
