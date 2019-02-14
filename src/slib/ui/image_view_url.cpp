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

#include "slib/ui/image_view.h"

#include "slib/network/url_request.h"

#include "slib/graphics/image.h"

namespace slib
{

	void ImageView::loadUrl(const String& url)
	{
		UrlRequestParam param;
		param.url = url;
		loadUrl(param);
	}
	
	void ImageView::loadUrl(const UrlRequestParam& _param)
	{
		if (_param.url.isEmpty()) {
			setSource(sl_null);
			return;
		}
		UrlRequestParam param = _param;
		param.flagSelfAlive = sl_false;
		auto weak = ToWeakRef(this);
		param.onComplete += [weak](UrlRequest* request) {
			auto view = ToRef(weak);
			if (view.isNull()) {
				return;
			}
			view->m_request.setNull();
			if (request->isError()) {
				return;
			}
			Memory mem = request->getResponseContent();
			view->setSource(Image::loadFromMemory(mem));
		};
		m_request = UrlRequest::send(param);
	}

}
