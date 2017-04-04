/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

/*
	https://tools.ietf.org/html/rfc3986

	Uniform Resource Identifier (URI): Generic Syntax
*/

#ifndef CHECKHEADER_SLIB_NETWORK_URL
#define CHECKHEADER_SLIB_NETWORK_URL

#include "definition.h"

#include "../core/string.h"

namespace slib
{

	class SLIB_EXPORT Url
	{
	public:
		AtomicString scheme;
		AtomicString host;
		AtomicString path;
		AtomicString query;
		
	public:
		Url();
		
		~Url();
		
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

}

#endif
