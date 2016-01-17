#ifndef CHECKHEADER_SLIB_CORE_JSON
#define CHECKHEADER_SLIB_CORE_JSON

#include "definition.h"

#include "variant.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT Json
{
public:
	static Variant parseJsonFromUtf8TextFile(const String& filePath, sl_bool flagSupportComments = sl_true);
	
	static Variant parseJson(const String& json, sl_bool flagSupportComments = sl_true);
	
	static Variant parseJson16(const String16& json, sl_bool flagSupportComments = sl_true);
	
	static String toJson(const Variant& var);
};

SLIB_NAMESPACE_END

#endif