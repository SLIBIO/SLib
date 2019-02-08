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

#include "slib/core/resource.h"

namespace slib
{

	Locale Resources::getCurrentLocale()
	{
		return Locale::getCurrent();
	}

	void Resources::setCurrentLocale(const Locale& locale)
	{
		Locale::setCurrent(locale);
	}

	String Resources::makeResourceName(const String& path)
	{
		String name = path.duplicate();
		sl_char8* szName = name.getData();
		sl_size lenName = name.getLength();
		for (sl_size i = 0; i < lenName; i++) {
			if (!(SLIB_CHAR_IS_C_NAME(szName[i]))) {
				szName[i] = '_';
			}
		}
		return name;
	}
	
	
	namespace res
	{
		namespace string
		{
			SLIB_DEFINE_STRING_RESOURCE_BEGIN(cancel, "Cancel")
				SLIB_DEFINE_STRING_RESOURCE_VALUE(ko, "\xec\xb7\xa8\xec\x86\x8c")
				SLIB_DEFINE_STRING_RESOURCE_VALUE(zh, "\xe5\x8f\x96\xe6\xb6\x88")
			SLIB_DEFINE_STRING_RESOURCE_END
			
			SLIB_DEFINE_STRING_RESOURCE_BEGIN(ok, "OK")
				SLIB_DEFINE_STRING_RESOURCE_VALUE(ko, "\xed\x99\x95\xec\x9d\xb8")
				SLIB_DEFINE_STRING_RESOURCE_VALUE(zh, "\xe5\xa5\xbd")
			SLIB_DEFINE_STRING_RESOURCE_END
			
		}
	}

}
