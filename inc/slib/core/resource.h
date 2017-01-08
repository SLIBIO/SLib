#ifndef CHECKHEADER_SLIB_CORE_RESOURCE
#define CHECKHEADER_SLIB_CORE_RESOURCE

#include "definition.h"

#include "locale.h"
#include "map.h"
#include "string.h"
#include "memory.h"
#include "safe_static.h"

SLIB_NAMESPACE_BEGIN

class Resources
{
public:
	static Locale getCurrentLocale();
	
	static void setCurrentLocale(const Locale& locale);
	
	static String makeResourceName(const String& path);

};

#define SLIB_DECLARE_STRING_RESOURCE(NAME) \
	namespace NAME { \
		slib::String get(const slib::Locale& locale); \
		slib::String get(); \
	} \

#define SLIB_DEFINE_STRING_RESOURCE_BEGIN(NAME, DEFAULT) \
	namespace NAME { \
		SLIB_STATIC_STRING(def, DEFAULT); \
		slib::String get(const slib::Locale& locale) { \
			int lang = (int)(locale.getLanguage()); \
			SLIB_UNUSED(lang) \
			slib::Locale localeSource; \
			SLIB_UNUSED(localeSource)

#define SLIB_DEFINE_STRING_RESOURCE_VALUE(LOCALE, VALUE) \
			localeSource = SLIB_LOCALE(LOCALE); \
			if (locale == localeSource || lang == localeSource) { \
				SLIB_STATIC_STRING(str, VALUE); \
				return str; \
			}

#define SLIB_DEFINE_STRING_RESOURCE_END \
			return def; \
		} \
		slib::String get() { return get(slib::Resources::getCurrentLocale()); } \
	} \

#define SLIB_DEFINE_STRING_RESOURCE_SIMPLE(NAME, VALUE) \
	namespace NAME { \
		SLIB_STATIC_STRING(def, VALUE) \
		slib::String get(const slib::Locale& locale) { return def; } \
		slib::String get() { return def; } \
	}


#define SLIB_DECLARE_RAW_RESOURCE(NAME) \
	namespace NAME { \
		extern const sl_uint8 bytes[]; \
		extern const sl_size size; \
		slib::Memory get(); \
	} \

#define SLIB_DEFINE_RAW_RESOURCE(NAME, SIZE) \
	namespace NAME { \
		const sl_size size = SIZE; \
		slib::Memory get() { \
			return slib::Memory::createStatic(bytes, SIZE); \
		} \
	}

#define SLIB_DECLARE_RESOURCE_MAP(TYPE) \
	TYPE get(const slib::String& name);

#define SLIB_DEFINE_RESOURCE_MAP_BEGIN(TYPE) \
	class _ResourceMap { \
	public: \
		slib::HashMap< slib::String, TYPE > map; \
	public: \
		_ResourceMap() { \

#define SLIB_DEFINE_RESOURCE_MAP_ITEM(NAME) \
			SLIB_STATIC_STRING(_key_##NAME, #NAME); \
			map.put_NoLock(_key_##NAME, NAME::get());

#define SLIB_DEFINE_RESOURCE_MAP_END(TYPE) \
		} \
	}; \
	SLIB_SAFE_STATIC_GETTER(_ResourceMap, _get_resource_map); \
	TYPE get(const slib::String& name) { \
		_ResourceMap* map = _get_resource_map(); \
		if (map) { \
			return map->map.getValue_NoLock(name, TYPE::null()); \
		} \
		return sl_null; \
	}


#define SLIB_DECLARE_STRING_RESOURCE_MAP SLIB_DECLARE_RESOURCE_MAP(slib::String)
#define SLIB_DEFINE_STRING_RESOURCE_MAP_BEGIN SLIB_DEFINE_RESOURCE_MAP_BEGIN(slib::String)
#define SLIB_DEFINE_STRING_RESOURCE_MAP_END SLIB_DEFINE_RESOURCE_MAP_END(slib::String)

#define SLIB_DECLARE_RAW_RESOURCE_MAP SLIB_DECLARE_RESOURCE_MAP(slib::Memory)
#define SLIB_DEFINE_RAW_RESOURCE_MAP_BEGIN SLIB_DEFINE_RESOURCE_MAP_BEGIN(slib::Memory)
#define SLIB_DEFINE_RAW_RESOURCE_MAP_END SLIB_DEFINE_RESOURCE_MAP_END(slib::Memory)

SLIB_NAMESPACE_END

#endif
