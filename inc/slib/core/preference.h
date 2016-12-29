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
	
};

SLIB_NAMESPACE_END

#endif
