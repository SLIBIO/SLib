/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
	class  _priv_ResourceMap { \
	public: \
		slib::CHashMap< slib::String, TYPE > map; \
	public: \
		 _priv_ResourceMap() { \

#define SLIB_DEFINE_RESOURCE_MAP_ITEM(NAME) \
			SLIB_STATIC_STRING(_key_##NAME, #NAME); \
			map.put_NoLock(_key_##NAME, NAME::get());

#define SLIB_DEFINE_RESOURCE_MAP_END(TYPE, DEFAULT_VALUE) \
		} \
	}; \
	SLIB_SAFE_STATIC_GETTER( _priv_ResourceMap, _get_resource_map); \
	TYPE get(const slib::String& name) { \
		 _priv_ResourceMap* map = _get_resource_map(); \
		if (map) { \
			return map->map.getValue_NoLock(name, DEFAULT_VALUE); \
		} \
		return DEFAULT_VALUE; \
	}


#define SLIB_DECLARE_STRING_RESOURCE_MAP SLIB_DECLARE_RESOURCE_MAP(slib::String)
#define SLIB_DEFINE_STRING_RESOURCE_MAP_BEGIN SLIB_DEFINE_RESOURCE_MAP_BEGIN(slib::String)
#define SLIB_DEFINE_STRING_RESOURCE_MAP_END SLIB_DEFINE_RESOURCE_MAP_END(slib::String, sl_null)

#define SLIB_DECLARE_RAW_RESOURCE_MAP SLIB_DECLARE_RESOURCE_MAP(slib::Memory)
#define SLIB_DEFINE_RAW_RESOURCE_MAP_BEGIN SLIB_DEFINE_RESOURCE_MAP_BEGIN(slib::Memory)
#define SLIB_DEFINE_RAW_RESOURCE_MAP_END SLIB_DEFINE_RESOURCE_MAP_END(slib::Memory, sl_null)

#endif
