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

#ifndef CHECKHEADER_SLIB_WEB_SERVICE
#define CHECKHEADER_SLIB_WEB_SERVICE

#include "definition.h"

#include "controller.h"

#include "../core/service.h"
#include "../network/http_service.h"

namespace slib
{
	
	class SLIB_EXPORT WebService : public Service
	{
		SLIB_DECLARE_OBJECT

	public:
		WebService();

		~WebService();

	public:
		static Ref<WebService> getApp();

	public:
		HttpServiceParam& getHttpParam();

		sl_uint16 getHttpPort();

		void setHttpPort(sl_uint16 port);
	
		void useAsset(const String& prefixForAssetPath);

		const Ref<WebController>& getController();

	protected:
		sl_bool dispatchStartService() override;

		void dispatchStopService() override;

	protected:
		Ref<HttpService> m_http;
		HttpServiceParam m_httpParam;
		Ref<WebController> m_controller;

	};

}

#endif
