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

#ifndef CHECKHEADER_SLIB_CORE_RESOURCE
#define CHECKHEADER_SLIB_CORE_RESOURCE

#include "definition.h"

#include "locale.h"
#include "hash_map.h"
#include "string.h"
#include "memory.h"
#include "safe_static.h"

namespace slib
{
	
	class Resources
	{
	public:
		static Locale getCurrentLocale();

		static void setCurrentLocale(const Locale& locale);

		static String makeResourceName(const String& path);
	
	};

}

#define SLIB_DECLARE_STRING_RESOURCE(NAME) \
	namespace NAME { \
		slib::String get(const slib::Locale& locale); \
		slib::String get(); \
	} \

#define SLIB_DEFINE_STRING_RESOURCE_BEGIN(NAME, DEFAULT) \
	namespace NAME { \
		SLIB_STATIC_STRING(def, DEFAULT); \
		slib::String get(const slib::Locale& locale) { \
			slib::Locale localeSource; \
			SLIB_UNUSED(localeSource)

#define SLIB_DEFINE_STRING_RESOURCE_VALUE(LOCALE, VALUE) \
			localeSource = slib::Locale(#LOCALE); \
if (locale == localeSource || slib::Locale(locale.getLanguage()) == localeSource || slib::Locale(locale.getLanguage(), locale.getCountry()) == localeSource || slib::Locale(locale.getLanguage(), locale.getScript(), slib::Country::Unknown) == localeSource) { \
				SLIB_RETURN_STRING(VALUE); \
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
	TYPE get(const slib::String& name); \
	slib::List<slib::String> getAllNames();

#define SLIB_DEFINE_RESOURCE_MAP_BEGIN(TYPE) \
	typedef TYPE (*_getter_type)(); \
	class  _priv_ResourceMap { \
	public: \
		slib::CHashMap< slib::String, _getter_type > map; \
	public: \
		 _priv_ResourceMap() {

#define SLIB_DEFINE_RESOURCE_MAP_ITEM(NAME) \
			{ \
				SLIB_STATIC_STRING(_key_##NAME, #NAME); \
				_getter_type f = &(NAME::get); \
				map.put_NoLock(_key_##NAME, f); \
			}

#define SLIB_DEFINE_RESOURCE_MAP_END(TYPE, DEFAULT_VALUE) \
		} \
	}; \
	SLIB_SAFE_STATIC_GETTER(_priv_ResourceMap, _get_resource_map); \
	TYPE get(const slib::String& name) { \
		 _priv_ResourceMap* map = _get_resource_map(); \
		if (map) { \
			_getter_type getter; \
			if (map->map.get_NoLock(name, &getter)) { \
				return getter(); \
			} \
		} \
		return DEFAULT_VALUE; \
	} \
	slib::List<slib::String> getAllNames() { \
		_priv_ResourceMap* map = _get_resource_map(); \
		if (map) { \
			return map->map.getAllKeys_NoLock(); \
		} \
		return sl_null; \
	}

#define SLIB_DECLARE_LOCALIZED_RESOURCE_MAP(TYPE) \
	TYPE get(const slib::String& name, const slib::Locale& locale); \
	TYPE get(const slib::String& name); \
	slib::List<slib::String> getAllNames();

#define SLIB_DEFINE_LOCALIZED_RESOURCE_MAP_BEGIN(TYPE) \
	typedef TYPE (*_getter_type)(); \
	typedef TYPE (*_getter_locale_type)(const slib::Locale& locale); \
	class  _priv_ResourceMap { \
	public: \
		slib::CHashMap< slib::String, _getter_type > map; \
		slib::CHashMap< slib::String, _getter_locale_type > map_locale; \
	public: \
		_priv_ResourceMap() {

#define SLIB_DEFINE_LOCALIZED_RESOURCE_MAP_ITEM(NAME) \
			{ \
				SLIB_STATIC_STRING(_key_##NAME, #NAME); \
				_getter_type f1 = &(NAME::get); \
				map.put_NoLock(_key_##NAME, f1); \
				_getter_locale_type f2 = &(NAME::get); \
				map_locale.put_NoLock(_key_##NAME, f2); \
			}

#define SLIB_DEFINE_LOCALIZED_RESOURCE_MAP_END(TYPE, DEFAULT_VALUE) \
		} \
	}; \
	SLIB_SAFE_STATIC_GETTER(_priv_ResourceMap, _get_resource_map); \
	TYPE get(const slib::String& name, const slib::Locale& locale) { \
		_priv_ResourceMap* map = _get_resource_map(); \
		if (map) { \
			_getter_locale_type getter; \
			if (map->map_locale.get_NoLock(name, &getter)) { \
				return getter(locale); \
			} \
		} \
		return DEFAULT_VALUE; \
	} \
	TYPE get(const slib::String& name) { \
		_priv_ResourceMap* map = _get_resource_map(); \
		if (map) { \
			_getter_type getter; \
			if (map->map.get_NoLock(name, &getter)) { \
				return getter(); \
			} \
		} \
		return DEFAULT_VALUE; \
	} \
	slib::List<slib::String> getAllNames() { \
		_priv_ResourceMap* map = _get_resource_map(); \
		if (map) { \
			return map->map.getAllKeys_NoLock(); \
		} \
		return sl_null; \
	}

#define SLIB_DECLARE_STRING_RESOURCE_MAP SLIB_DECLARE_LOCALIZED_RESOURCE_MAP(slib::String)
#define SLIB_DEFINE_STRING_RESOURCE_MAP_BEGIN SLIB_DEFINE_LOCALIZED_RESOURCE_MAP_BEGIN(slib::String)
#define SLIB_DEFINE_STRING_RESOURCE_MAP_ITEM(NAME) SLIB_DEFINE_LOCALIZED_RESOURCE_MAP_ITEM(NAME)
#define SLIB_DEFINE_STRING_RESOURCE_MAP_END SLIB_DEFINE_LOCALIZED_RESOURCE_MAP_END(slib::String, sl_null)

#define SLIB_DECLARE_RAW_RESOURCE_MAP SLIB_DECLARE_RESOURCE_MAP(slib::Memory)
#define SLIB_DEFINE_RAW_RESOURCE_MAP_BEGIN SLIB_DEFINE_RESOURCE_MAP_BEGIN(slib::Memory)
#define SLIB_DEFINE_RAW_RESOURCE_MAP_ITEM(NAME) SLIB_DEFINE_RESOURCE_MAP_ITEM(NAME)
#define SLIB_DEFINE_RAW_RESOURCE_MAP_END SLIB_DEFINE_RESOURCE_MAP_END(slib::Memory, sl_null)

#endif
