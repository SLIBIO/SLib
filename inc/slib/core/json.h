#ifndef CHECKHEADER_SLIB_CORE_JSON
#define CHECKHEADER_SLIB_CORE_JSON

#include "definition.h"

#include "variant.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT JsonParseParam
{
public:
	// in
	sl_bool flagSupportComments;
	
	// out
	sl_bool flagError;
	// out
	sl_size errorPosition;
	// out
	sl_size errorLine;
	// out
	sl_size errorColumn;
	// out
	String errorMessage;
	
public:
	JsonParseParam();
	
public:
	String getErrorText();
	
};

class SLIB_EXPORT Json
{
public:
	static Variant parseJson(const sl_char8* sz, sl_size len, JsonParseParam& param);
	
	static Variant parseJson(const sl_char8* sz, sl_size len);
	
	static Variant parseJson(const String& json, JsonParseParam& param);
	
	static Variant parseJson(const String& json);
	
	
	static Variant parseJson16(const sl_char16* sz, sl_size len, JsonParseParam& param);
	
	static Variant parseJson16(const sl_char16* sz, sl_size len);
	
	static Variant parseJson16(const String16& json, JsonParseParam& param);
	
	static Variant parseJson16(const String16& json);
	
	
	static Variant parseJsonFromUtf8TextFile(const String& filePath, JsonParseParam& param);
	
	static Variant parseJsonFromUtf8TextFile(const String& filePath);
	
	
	static String toJson(const Variant& var);
	
};

SLIB_NAMESPACE_END

#endif