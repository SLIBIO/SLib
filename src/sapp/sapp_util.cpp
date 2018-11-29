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

#include "sapp_util.h"

#include "slib/core/file.h"

namespace slib
{

	template <class CT>
	sl_bool _SAppUtil_checkName(const CT* sz, sl_reg len)
	{
		if (len == 0) {
			return sl_false;
		}
		CT ch = sz[0];
		if (!(SLIB_CHAR_IS_ALPHA(ch)) && ch != '_') {
			return sl_false;
		}
		if (len < 0) {
			for (sl_size i = 1; ; i++) {
				ch = sz[i];
				if (ch == 0) {
					break;
				}
				if (!(SLIB_CHAR_IS_C_NAME(ch))) {
					return sl_false;
				}
			}
		} else {
			for (sl_reg i = 1; i < len; i++) {
				ch = sz[i];
				if (!(SLIB_CHAR_IS_C_NAME(ch))) {
					return sl_false;
				}
			}
		}
		return sl_true;
	}

	sl_bool SAppUtil::checkName(const sl_char8* sz, sl_reg len)
	{
		return _SAppUtil_checkName(sz, len);
	}

	sl_bool SAppUtil::checkName(const sl_char16* sz, sl_reg len)
	{
		return _SAppUtil_checkName(sz, len);
	}

	String SAppUtil::generateBytesArrayDefinition(const void* data, sl_size size, sl_size countPerLine, sl_size tabCount)
	{
		if (size == 0) {
			return String::getEmpty();
		}
		sl_size len = size * 5 - 1;
		sl_size nLines;
		if (countPerLine == 0) {
			countPerLine = size;
			nLines = 1;
		} else {
			nLines = size / countPerLine;
			if (size % countPerLine) {
				nLines++;
			}
			len += 2 * (nLines - 1);
		}
		len += tabCount * nLines;
		String s = String::allocate(len);
		if (s.isEmpty()) {
			return s;
		}
		sl_char8* sz = s.getData();
		sl_size pos = 0;
		sl_size col = 0;
		for (sl_size i = 0; i < size; i++) {
			if (i > 0) {
				sz[pos++] = ',';
			}
			if (col == countPerLine) {
				col = 0;
				sz[pos++] = '\r';
				sz[pos++] = '\n';
			}
			if (col == 0) {
				for (sl_size iTab = 0; iTab < tabCount; iTab++) {
					sz[pos++] = '\t';
				}
			}
			sz[pos++] = '0';
			sz[pos++] = 'x';
			sl_uint32 n = ((sl_uint8*)data)[i];
			sz[pos++] = _priv_StringConv_radixPatternLower[(n >> 4) & 15];
			sz[pos++] = _priv_StringConv_radixPatternLower[n & 15];
			col++;
		}
		sz[pos++] = 0;
		return s;
	}
	
}
