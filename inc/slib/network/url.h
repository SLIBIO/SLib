/*
	https://tools.ietf.org/html/rfc3986

	Uniform Resource Identifier (URI): Generic Syntax
*/

#ifndef CHECKHEADER_SLIB_NETWORK_URL
#define CHECKHEADER_SLIB_NETWORK_URL

#include "definition.h"

#include "../core/string.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT Url
{
public:
	AtomicString scheme;
	AtomicString host;
	AtomicString path;
	AtomicString query;

public:
	void parse(const String& url);

public:
	static String encodePercentByUTF8(const String& value);
	
	static String decodePercentByUTF8(const String& value);

	
	static String encodeUriComponentByUTF8(const String& value);
	
	static String decodeUriComponentByUTF8(const String& value);

	
	static String encodeUriByUTF8(const String& value);
	
	static String decodeUriByUTF8(const String& value);

};

SLIB_NAMESPACE_END

#endif
