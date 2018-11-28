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
		static void setValue(const String& key, const Json& value);
		
		static Json getValue(const String& key);

		template <class T>
		SLIB_INLINE static void getValue(const String& key, T& _out)
		{
			getValue(key).get(_out);
		}
		
		template <class T>
		SLIB_INLINE static void getValue(const String& key, T& _out, const T& _def)
		{
			getValue(key).get(_out, _def);
		}

		// used for Win32 applications
		static String getApplicationKeyName();

		static void setApplicationKeyName(const String& name);
	
	};

}

#endif
