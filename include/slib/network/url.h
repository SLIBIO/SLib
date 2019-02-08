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
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(Url)
		
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
