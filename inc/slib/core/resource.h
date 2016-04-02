#ifndef CHECKHEADER_SLIB_CORE_RESOURCE
#define CHECKHEADER_SLIB_CORE_RESOURCE

#include "definition.h"

#include "locale.h"
#include "map.h"
#include "string.h"
#include "memory.h"

SLIB_NAMESPACE_BEGIN

class Resources
{
public:
	static Locale getCurrentLocale();
	
	static void setCurrentLocale(const Locale& locale);

};

#define SLIB_DECLARE_STRING_RESOURCE(NAME) \
	namespace NAME { \
		slib::String get(const slib::Locale& locale); \
		slib::String get(); \
	} \

#define SLIB_BEGIN_STRING_RESOURCE(NAME, DEFAULT) \
	namespace NAME { \
		SLIB_STATIC_STRING(def, DEFAULT); \
		slib::String get(const slib::Locale& locale) { \

#define SLIB_DEFINE_STRING_RESOURCE_VALUE(LOCALE, VALUE) \
			if (locale == SLIB_LOCALE(LOCALE) || (sl_uint32)(locale.getLanguage()) == SLIB_LOCALE(LOCALE)) { \
				SLIB_STATIC_STRING(str, VALUE); \
				return str; \
			}

#define SLIB_END_STRING_RESOURCE \
			return def; \
		} \
		slib::String get() { return get(slib::Resources::getCurrentLocale()); } \
	} \

SLIB_NAMESPACE_END

#endif
