#ifndef CHECKHEADER_SLIB_GRAPHICS_RESOURCE
#define CHECKHEADER_SLIB_GRAPHICS_RESOURCE

#include "definition.h"

#include "image.h"
#include "../core/locale.h"
#include "../core/list.h"
#include "../core/resource.h"

SLIB_GRAPHICS_NAMESPACE_BEGIN

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
		slib::_ImageResourceEntry* _getEntries(const slib::Locale& locale); \
		slib::Ref<slib::Image> getImage(const slib::Locale& locale, sl_uint32 requiredWidth, sl_uint32 requiredHeight) { \
			return slib::_ImageResource_getImage(_getEntries(locale), requiredWidth, requiredHeight); \
		} \
		slib::Ref<slib::Image> getImage(sl_uint32 requiredWidth, sl_uint32 requiredHeight) { \
			return slib::_ImageResource_getImage(_getEntries(slib::Resources::getCurrentLocale()), requiredWidth, requiredHeight); \
		} \
		slib::List< slib::Ref<slib::Image> > getImages(const slib::Locale& locale) { \
			return slib::_ImageResource_getImages(_getEntries(locale)); \
		} \
		slib::List< slib::Ref<slib::Image> > getImages() { \
			return slib::_ImageResource_getImages(_getEntries(slib::Resources::getCurrentLocale())); \
		} \
		slib::Ref<slib::Drawable> get(const slib::Locale& locale) { \
			return slib::_ImageResource_get(_getEntries(locale), WIDTH, HEIGHT); \
		} \
		slib::Ref<slib::Drawable> get() { \
			return slib::_ImageResource_get(_getEntries(slib::Resources::getCurrentLocale()), WIDTH, HEIGHT); \
		} \
		slib::_ImageResourceEntry* _getEntries(const slib::Locale& locale) { \
			int lang = (int)(locale.getLanguage()); \
			SLIB_UNUSED(lang) \
			slib::Locale localeSource; \
			SLIB_UNUSED(localeSource)

#define SLIB_DEFINE_IMAGE_RESOURCE_DEFAULT_LIST_BEGIN \
			{ \
				static slib::_ImageResourceEntry entries[] = { \

#define SLIB_DEFINE_IMAGE_RESOURCE_LIST_BEGIN(LOCALE) \
			localeSource = SLIB_LOCALE(LOCALE); \
			if (locale == localeSource || lang == localeSource) { \
				static slib::_ImageResourceEntry entries[] = {

#define SLIB_DEFINE_IMAGE_RESOURCE_ITEM(WIDTH, HEIGHT, SIZE, BYTES) \
					{sl_true, WIDTH, HEIGHT, BYTES, (sl_uint32)(SIZE), 0, 0, sl_false},

#define SLIB_DEFINE_IMAGE_RESOURCE_LIST_END \
					{sl_false, 0, 0, 0, 0, 0, 0, sl_false} \
				}; \
				static slib::_ImageResourceFreeStatic free_entries(entries); \
				return entries; \
			}

#define SLIB_DEFINE_IMAGE_RESOURCE_END \
		} \
	}

#define SLIB_DEFINE_IMAGE_RESOURCE_SIMPLE(NAME, WIDTH, HEIGHT, SIZE, BYTES) \
	namespace NAME { \
		static slib::_ImageResourceEntry entries[2] = { {sl_true, WIDTH, HEIGHT, BYTES, (sl_uint32)(SIZE), 0, 0, sl_false}, {sl_false, 0, 0, 0, 0, 0, 0, sl_false} }; \
		static slib::_ImageResourceFreeStatic free_entries(entries); \
		slib::Ref<slib::Image> getImage(const slib::Locale& locale, sl_uint32 requiredWidth, sl_uint32 requiredHeight) { \
			return slib::_ImageResource_getImage(entries, requiredWidth, requiredHeight); \
		} \
		slib::Ref<slib::Image> getImage(sl_uint32 requiredWidth, sl_uint32 requiredHeight) { \
			return slib::_ImageResource_getImage(entries, requiredWidth, requiredHeight); \
		} \
		slib::List< slib::Ref<slib::Image> > getImages(const slib::Locale& locale) { \
			return slib::_ImageResource_getImages(entries); \
		} \
		slib::List< slib::Ref<slib::Image> > getImages() { \
			return slib::_ImageResource_getImages(entries); \
		} \
		slib::Ref<slib::Drawable> get(const slib::Locale& locale) { \
			return slib::_ImageResource_get(entries, WIDTH, HEIGHT); \
		} \
		slib::Ref<slib::Drawable> get() { \
			return slib::_ImageResource_get(entries, WIDTH, HEIGHT); \
		} \
	}


#define SLIB_DECLARE_NINEPIECES_RESOURCE(NAME) \
	namespace NAME { \
		slib::Ref<slib::Drawable> get(); \
	}

#define SLIB_DEFINE_NINEPIECES_RESOURCE(NAME, LEFT_WIDTH, RIGHT_WIDTH, TOP_HEIGHT, BOTTOM_HEIGHT, TOP_LEFT, TOP, TOP_RIGHT, LEFT, CENTER, RIGHT, BOTTOM_LEFT, BOTTOM, BOTTOM_RIGHT) \
	namespace NAME { \
		SLIB_STATIC_ZERO_INITIALIZED(SafeRef<Drawable>, value) \
		slib::Ref<slib::Drawable> get() { \
			if (SLIB_SAFE_STATIC_CHECK_FREED(value)) { \
				return slib::Ref<slib::Drawable>::null(); \
			} \
			if (value.isNull()) { \
				value = NinePiecesDrawable::create(LEFT_WIDTH, RIGHT_WIDTH, TOP_HEIGHT, BOTTOM_HEIGHT, TOP_LEFT, TOP, TOP_RIGHT, LEFT, CENTER, RIGHT, BOTTOM_LEFT, BOTTOM, BOTTOM_RIGHT); \
			} \
			return value; \
		} \
	}


#define SLIB_DECLARE_NINEPATCH_RESOURCE(NAME) \
	namespace NAME { \
		slib::Ref<slib::Drawable> get(); \
	}

#define SLIB_DEFINE_NINEPATCH_RESOURCE(NAME, DST_LEFT_WIDTH, DST_RIGHT_WIDTH, DST_TOP_HEIGHT, DST_BOTTOM_HEIGHT, SRC, SRC_LEFT_WIDTH, SRC_RIGHT_WIDTH, SRC_TOP_HEIGHT, SRC_BOTTOM_HEIGHT) \
	namespace NAME { \
		SLIB_STATIC_ZERO_INITIALIZED(SafeRef<Drawable>, value) \
		slib::Ref<slib::Drawable> get() { \
			if (SLIB_SAFE_STATIC_CHECK_FREED(value)) { \
				return slib::Ref<slib::Drawable>::null(); \
			} \
			if (value.isNull()) { \
				value = NinePatchDrawable::create(DST_LEFT_WIDTH, DST_RIGHT_WIDTH, DST_TOP_HEIGHT, DST_BOTTOM_HEIGHT, SRC, SRC_LEFT_WIDTH, SRC_RIGHT_WIDTH, SRC_TOP_HEIGHT, SRC_BOTTOM_HEIGHT); \
			} \
			return value; \
		} \
	}

#define SLIB_DECLARE_DRAWABLE_RESOURCE_MAP SLIB_DECLARE_RESOURCE_MAP(slib::Ref<slib::Drawable>)
#define SLIB_DEFINE_DRAWABLE_RESOURCE_MAP_BEGIN SLIB_DEFINE_RESOURCE_MAP_BEGIN(slib::Ref<slib::Drawable>)
#define SLIB_DEFINE_DRAWABLE_RESOURCE_MAP_END SLIB_DEFINE_RESOURCE_MAP_END(slib::Ref<slib::Drawable>)


struct _ImageResourceEntry
{
	sl_bool flagValid;
	
	sl_uint32 width;
	sl_uint32 height;
	
	const sl_uint8* source_bytes;
	sl_uint32 source_size;
	
	sl_int32 lock;
	void* image;
	sl_bool flag_load;
	
	Ref<Image> getImage();
	Ref<Image> getMatchingImage(sl_uint32 width, sl_uint32 height);
	
};

class _ImageResourceFreeStatic
{
public:
	_ImageResourceFreeStatic(_ImageResourceEntry* entries);
	
	~_ImageResourceFreeStatic();
	
private:
	_ImageResourceEntry* m_entries;
	
};

Ref<Image> _ImageResource_getImage(_ImageResourceEntry* entries, sl_uint32 requiredWidth, sl_uint32 requiredHeight);

List< Ref<Image> > _ImageResource_getImages(_ImageResourceEntry* entries);

Ref<Drawable> _ImageResource_get(_ImageResourceEntry* entries, sl_uint32 width, sl_uint32 height);

SLIB_GRAPHICS_NAMESPACE_END

#endif
