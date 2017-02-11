#ifndef CHECKHEADER_SLIB_CORE_PREFERENCE
#define CHECKHEADER_SLIB_CORE_PREFERENCE

#include "definition.h"

#include "variant.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT Preference
{
public:
	static void setValue(const String& key, const Variant& value);
	
	static Variant getValue(const String& key);
	

	// used for Win32 applications
	static String getApplicationKeyName();

	static void setApplicationKeyName(const String& name);

};

SLIB_NAMESPACE_END

#endif
