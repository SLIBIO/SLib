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

#ifndef CHECKHEADER_SLIB_GRAPHICS_RESOURCE
#define CHECKHEADER_SLIB_GRAPHICS_RESOURCE

#include "definition.h"

#include "image.h"
#include "color.h"

#include "../core/locale.h"
#include "../core/list.h"
#include "../core/resource.h"

namespace slib
{
	
	class SLIB_EXPORT GraphicsResource
	{
	public:
		static sl_uint32 getScreenWidth();
		
		static void setScreenWidth(sl_uint32 width);
		
		static sl_uint32 getScreenHeight();
		
		static void setScreenHeight(sl_uint32 height);
		
	};
	
}

#define SLIB_DECLARE_COLOR_RESOURCE(NAME) \
	namespace NAME { \
		slib::Color get(); \
	}

#define SLIB_DEFINE_COLOR_RESOURCE(NAME, r, g, b, a) \
	namespace NAME { \
		slib::Color get() { \
			return slib::Color(r, g, b, a); \
		} \
	}

#define SLIB_DECLARE_COLOR_RESOURCE_MAP SLIB_DECLARE_RESOURCE_MAP(slib::Color)
#define SLIB_DEFINE_COLOR_RESOURCE_MAP_BEGIN SLIB_DEFINE_RESOURCE_MAP_BEGIN(slib::Color)
#define SLIB_DEFINE_COLOR_RESOURCE_MAP_ITEM(NAME) SLIB_DEFINE_RESOURCE_MAP_ITEM(NAME)
#define SLIB_DEFINE_COLOR_RESOURCE_MAP_END SLIB_DEFINE_RESOURCE_MAP_END(slib::Color, 0)


#define SLIB_DECLARE_IMAGE_RESOURCE(NAME) \
	namespace NAME { \
		slib::Ref<slib::Image> getImage(const slib::Locale& locale, sl_uint32 requiredWidth = 0, sl_uint32 requiredHeight = 0); \
		slib::Ref<slib::Image> getImage(sl_uint32 requiredWidth = 0, sl_uint32 requiredHeight = 0); \
		slib::List< slib::Ref<slib::Image> > getImages(const slib::Locale& locale); \
		slib::List< slib::Ref<slib::Image> > getImages(); \
		slib::Ref<slib::Drawable> get(const slib::Locale& locale); \
		slib::Ref<slib::Drawable> get(); \
	}

#define SLIB_DEFINE_IMAGE_RESOURCE_BEGIN(NAME, WIDTH, HEIGHT) \
	namespace NAME { \
		slib::_priv_ImageResourceEntry* _getEntries(const slib::Locale& locale); \
		slib::Ref<slib::Image> getImage(const slib::Locale& locale, sl_uint32 requiredWidth, sl_uint32 requiredHeight) { \
			return slib::_priv_ImageResource_getImage(_getEntries(locale), requiredWidth, requiredHeight); \
		} \
		slib::Ref<slib::Image> getImage(sl_uint32 requiredWidth, sl_uint32 requiredHeight) { \
			return slib::_priv_ImageResource_getImage(_getEntries(slib::Resources::getCurrentLocale()), requiredWidth, requiredHeight); \
		} \
		slib::List< slib::Ref<slib::Image> > getImages(const slib::Locale& locale) { \
			return slib::_priv_ImageResource_getImages(_getEntries(locale)); \
		} \
		slib::List< slib::Ref<slib::Image> > getImages() { \
			return slib::_priv_ImageResource_getImages(_getEntries(slib::Resources::getCurrentLocale())); \
		} \
		slib::Ref<slib::Drawable> get(const slib::Locale& locale) { \
			return slib::_priv_ImageResource_get(_getEntries(locale), WIDTH, HEIGHT); \
		} \
		slib::Ref<slib::Drawable> get() { \
			return slib::_priv_ImageResource_get(_getEntries(slib::Resources::getCurrentLocale()), WIDTH, HEIGHT); \
		} \
		slib::_priv_ImageResourceEntry* _getEntries(const slib::Locale& locale) { \
			slib::Locale localeSource; \
			SLIB_UNUSED(localeSource)

#define SLIB_DEFINE_IMAGE_RESOURCE_DEFAULT_LIST_BEGIN \
			{ \
				static slib::_priv_ImageResourceEntry entries[] = { \

#define SLIB_DEFINE_IMAGE_RESOURCE_LIST_BEGIN(LOCALE) \
			localeSource = slib::Locale(#LOCALE); \
			if (locale == localeSource || slib::Locale(locale.getLanguage()) == localeSource || slib::Locale(locale.getLanguage(), locale.getCountry()) == localeSource || slib::Locale(locale.getLanguage(), locale.getScript(), slib::Country::Unknown) == localeSource) { \
				static slib::_priv_ImageResourceEntry entries[] = {

#define SLIB_DEFINE_IMAGE_RESOURCE_ITEM(WIDTH, HEIGHT, SIZE, BYTES) \
					{sl_true, WIDTH, HEIGHT, BYTES, (sl_uint32)(SIZE), 0, 0, sl_false},

#define SLIB_DEFINE_IMAGE_RESOURCE_LIST_END \
					{sl_false, 0, 0, 0, 0, 0, 0, sl_false} \
				}; \
				static slib::_priv_ImageResourceFreeStatic free_entries(entries); \
				return entries; \
			}

#define SLIB_DEFINE_IMAGE_RESOURCE_END \
		} \
	}

#define SLIB_DEFINE_IMAGE_RESOURCE_SIMPLE(NAME, WIDTH, HEIGHT, SIZE, BYTES) \
	namespace NAME { \
		static slib::_priv_ImageResourceEntry entries[2] = { {sl_true, WIDTH, HEIGHT, BYTES, (sl_uint32)(SIZE), 0, 0, sl_false}, {sl_false, 0, 0, 0, 0, 0, 0, sl_false} }; \
		static slib::_priv_ImageResourceFreeStatic free_entries(entries); \
		slib::Ref<slib::Image> getImage(const slib::Locale& locale, sl_uint32 requiredWidth, sl_uint32 requiredHeight) { \
			return slib::_priv_ImageResource_getImage(entries, requiredWidth, requiredHeight); \
		} \
		slib::Ref<slib::Image> getImage(sl_uint32 requiredWidth, sl_uint32 requiredHeight) { \
			return slib::_priv_ImageResource_getImage(entries, requiredWidth, requiredHeight); \
		} \
		slib::List< slib::Ref<slib::Image> > getImages(const slib::Locale& locale) { \
			return slib::_priv_ImageResource_getImages(entries); \
		} \
		slib::List< slib::Ref<slib::Image> > getImages() { \
			return slib::_priv_ImageResource_getImages(entries); \
		} \
		slib::Ref<slib::Drawable> get(const slib::Locale& locale) { \
			return slib::_priv_ImageResource_get(entries, WIDTH, HEIGHT); \
		} \
		slib::Ref<slib::Drawable> get() { \
			SLIB_SAFE_STATIC(slib::Ref<slib::Drawable>, value, slib::_priv_ImageResource_get(entries, WIDTH, HEIGHT)); \
			if (SLIB_SAFE_STATIC_CHECK_FREED(value)) { \
				return sl_null; \
			} \
			return value; \
		} \
	}

#define SLIB_DECLARE_DRAWABLE_RESOURCE(NAME) \
	namespace NAME { \
		slib::Ref<slib::Drawable> get(const slib::Locale& locale); \
		slib::Ref<slib::Drawable> get(); \
	}

#define SLIB_DECLARE_NINEPIECES_RESOURCE(NAME) SLIB_DECLARE_DRAWABLE_RESOURCE(NAME)

#define SLIB_DEFINE_NINEPIECES_RESOURCE(NAME, LEFT_WIDTH, RIGHT_WIDTH, TOP_HEIGHT, BOTTOM_HEIGHT, TOP_LEFT, TOP, TOP_RIGHT, LEFT, CENTER, RIGHT, BOTTOM_LEFT, BOTTOM, BOTTOM_RIGHT) \
	namespace NAME { \
		SLIB_STATIC_ZERO_INITIALIZED(slib::AtomicRef<slib::Drawable>, value) \
		slib::Ref<slib::Drawable> get() { \
			if (SLIB_SAFE_STATIC_CHECK_FREED(value)) { \
				return sl_null; \
			} \
			if (value.isNull()) { \
				value = slib::NinePiecesDrawable::create(LEFT_WIDTH, RIGHT_WIDTH, TOP_HEIGHT, BOTTOM_HEIGHT, TOP_LEFT, TOP, TOP_RIGHT, LEFT, CENTER, RIGHT, BOTTOM_LEFT, BOTTOM, BOTTOM_RIGHT); \
			} \
			return value; \
		} \
		slib::Ref<slib::Drawable> get(const slib::Locale& locale) { \
			return get(); \
		} \
	}


#define SLIB_DECLARE_NINEPATCH_RESOURCE(NAME) SLIB_DECLARE_DRAWABLE_RESOURCE(NAME)

#define SLIB_DEFINE_NINEPATCH_RESOURCE(NAME, DST_LEFT_WIDTH, DST_RIGHT_WIDTH, DST_TOP_HEIGHT, DST_BOTTOM_HEIGHT, SRC, SRC_LEFT_WIDTH, SRC_RIGHT_WIDTH, SRC_TOP_HEIGHT, SRC_BOTTOM_HEIGHT) \
	namespace NAME { \
		SLIB_STATIC_ZERO_INITIALIZED(slib::AtomicRef<slib::Drawable>, value) \
		slib::Ref<slib::Drawable> get() { \
			if (SLIB_SAFE_STATIC_CHECK_FREED(value)) { \
				return sl_null; \
			} \
			if (value.isNull()) { \
				value = slib::NinePatchDrawable::create(DST_LEFT_WIDTH, DST_RIGHT_WIDTH, DST_TOP_HEIGHT, DST_BOTTOM_HEIGHT, SRC, SRC_LEFT_WIDTH, SRC_RIGHT_WIDTH, SRC_TOP_HEIGHT, SRC_BOTTOM_HEIGHT); \
			} \
			return value; \
		} \
		slib::Ref<slib::Drawable> get(const slib::Locale& locale) { \
			return get(); \
		} \
	}


#define SLIB_DECLARE_DRAWABLE_RESOURCE_MAP SLIB_DECLARE_LOCALIZED_RESOURCE_MAP(slib::Ref<slib::Drawable>)
#define SLIB_DEFINE_DRAWABLE_RESOURCE_MAP_BEGIN SLIB_DEFINE_LOCALIZED_RESOURCE_MAP_BEGIN(slib::Ref<slib::Drawable>)
#define SLIB_DEFINE_DRAWABLE_RESOURCE_MAP_ITEM(NAME) SLIB_DEFINE_LOCALIZED_RESOURCE_MAP_ITEM(NAME)
#define SLIB_DEFINE_DRAWABLE_RESOURCE_MAP_END SLIB_DEFINE_LOCALIZED_RESOURCE_MAP_END(slib::Ref<slib::Drawable>, sl_null)

#include "detail/resource.inc"

#endif
