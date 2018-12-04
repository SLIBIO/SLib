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

#include "../../core/string_buffer.h"
#include "../../core/cast.h"

namespace slib
{
	
	template <class KT, class VT, class KEY_COMPARE>
	String HttpRequest::buildFormUrlEncodedFromMap(const Map<KT, VT, KEY_COMPARE>& params)
	{
		StringBuffer sb;
		sl_bool flagFirst = sl_true;
		for (auto& pair : params) {
			if (!flagFirst) {
				sb.addStatic("&", 1);
			}
			flagFirst = sl_false;
			sb.add(Cast<KT, String>()(pair.key));
			sb.addStatic("=", 1);
			sb.add(Url::encodeUriComponentByUTF8(Cast<VT, String>()(pair.value)));
		}
		return sb.merge();
	}
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	String HttpRequest::buildFormUrlEncodedFromHashMap(const HashMap<KT, VT, HASH, KEY_COMPARE>& params)
	{
		StringBuffer sb;
		sl_bool flagFirst = sl_true;
		for (auto& pair : params) {
			if (!flagFirst) {
				sb.addStatic("&", 1);
			}
			flagFirst = sl_false;
			sb.add(Cast<KT, String>()(pair.key));
			sb.addStatic("=", 1);
			sb.add(Url::encodeUriComponentByUTF8(Cast<VT, String>()(pair.value)));
		}
		return sb.merge();
	}

}
