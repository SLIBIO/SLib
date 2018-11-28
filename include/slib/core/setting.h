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

#ifndef CHECKHEADER_SLIB_CORE_SETTING
#define CHECKHEADER_SLIB_CORE_SETTING

#include "definition.h"

#include "hash_map.h"
#include "variant.h"

namespace slib
{
	
	class SLIB_EXPORT IniSetting : public Object
	{
		SLIB_DECLARE_OBJECT

	public:
		IniSetting();

		~IniSetting();

	public:
		void initialize();

		sl_bool parseFromUtf8TextFile(const String& filePath);

		sl_bool parseFromText(const String& text);

		Variant getValue(const String& name);

	private:
		CHashMap<String, String> m_mapValues;

	};
	
	class SLIB_EXPORT SettingUtil
	{
	public:
		static sl_bool parseUint32Range(const String& str, sl_uint32* from = sl_null, sl_uint32* to = sl_null);
	
	};

}

#endif
