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

#include "slib/core/definition.h"

#ifdef SLIB_PLATFORM_IS_WIN32

#include "slib/core/preference.h"

#include "slib/core/json.h"
#include "slib/core/platform_windows.h"

namespace slib
{

	void Preference::setValue(const String& key, const Json& value)
	{
		if (key.isEmpty()) {
			return;
		}
		String appName = getApplicationKeyName();
		if (appName.isEmpty()) {
			return;
		}
		Windows::setRegistryValue(HKEY_CURRENT_USER, "Software\\" + appName, key, value.toJsonString());
	}

	Json Preference::getValue(const String& key)
	{
		if (key.isEmpty()) {
			return sl_null;
		}
		String appName = getApplicationKeyName();
		if (appName.isEmpty()) {
			return sl_null;
		}
		Variant v;
		if (Windows::getRegistryValue(HKEY_CURRENT_USER, "Software\\" + appName, key, &v)) {
			String s = v.getString();
			if (s.isNotEmpty()) {
				return Json::parseJson(s);
			}
		}
		return sl_null;
	}

}

#endif