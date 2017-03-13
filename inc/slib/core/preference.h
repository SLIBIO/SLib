#ifndef CHECKHEADER_SLIB_CORE_PREFERENCE
#define CHECKHEADER_SLIB_CORE_PREFERENCE

#include "definition.h"

#include "variant.h"
#include "json.h"

namespace slib
{
	
	class SLIB_EXPORT Preference
	{
	public:
		static void setValue(const String& key, const Variant& value);
		
		template <class T>
		SLIB_INLINE static void setValue(const String& key, const T& value)
		{
			setValue(key, Json::toJson(value));
		}

		static Variant getValue(const String& key);

		template <class T>
		SLIB_INLINE static void getValue(const String& key, T& _out)
		{
			Json::fromJson(getValue(key), _out);
		}
		
		template <class T>
		SLIB_INLINE static void getValue(const String& key, T& _out, const T& _def)
		{
			Json::fromJson(getValue(key), _out, _def);
		}

		// used for Win32 applications
		static String getApplicationKeyName();

		static void setApplicationKeyName(const String& name);
	
	};

}

#endif
