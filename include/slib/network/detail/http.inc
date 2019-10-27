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
	
	template <class MAP>
	String HttpRequest::buildQuery(const MAP& params)
	{
		StringBuffer sb;
		sl_bool flagFirst = sl_true;
		for (auto& pair : params) {
			if (!flagFirst) {
				sb.addStatic("&", 1);
			}
			flagFirst = sl_false;
			sb.add(Cast<typename MAP::KEY_TYPE, String>()(pair.key));
			sb.addStatic("=", 1);
			sb.add(Url::encodePercent(Cast<typename MAP::VALUE_TYPE, String>()(pair.value)));
		}
		return sb.merge();
	}
	
	template <class MAP>
	String HttpRequest::buildFormUrlEncoded(const MAP& params)
	{
		StringBuffer sb;
		sl_bool flagFirst = sl_true;
		for (auto& pair : params) {
			if (!flagFirst) {
				sb.addStatic("&", 1);
			}
			flagFirst = sl_false;
			sb.add(Cast<typename MAP::KEY_TYPE, String>()(pair.key));
			sb.addStatic("=", 1);
			sb.add(Url::encodeForm(Cast<typename MAP::VALUE_TYPE, String>()(pair.value)));
		}
		return sb.merge();
	}
	
	template <class MAP>
	void HttpResponse::setResponseRedirect(const String& location, const MAP& map, HttpStatus status)
	{
		String url = location;
		String query = HttpRequest::buildQuery(map);
		if (query.isNotEmpty()) {
			if (url.indexOf('?') >= 0) {
				url += "&";
			} else {
				url += "?";
			}
			url += query;
		}
		setResponseRedirect(url, status);
	}
	
}
