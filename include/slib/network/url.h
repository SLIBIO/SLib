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
#include "../core/hash_map.h"

namespace slib
{

	class SLIB_EXPORT Url
	{
	public:
		String scheme;
		String host;
		String path;
		String query;
		String fragment;
		
	public:
		Url();
		
		Url(const String& url);
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(Url)
		
	public:
		void parse(const String& url);
		
		String toString() const;
		
		HashMap<String, String> getQueryParameters() const;
		
		void setQueryParameters(const HashMap<String, String>& params);
		
	public:
		static String encodePercent(const String& value);
		
		static String decodePercent(const String& value);
		
		
		static String encodeUriComponent(const String& value);
		
		static String decodeUriComponent(const String& value);
		
		
		static String encodeUri(const String& value);
		
		static String decodeUri(const String& value);
		
		
		static String encodeForm(const String& value);
		
		static String decodeForm(const String& value);


		static String getPhoneNumber(const String& url);
		
	};

}

#endif
