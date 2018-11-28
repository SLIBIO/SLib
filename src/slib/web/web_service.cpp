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

#include "slib/web/service.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(WebService, Service)

	WebService::WebService()
	{
		m_controller = WebController::create();
	}

	WebService::~WebService()
	{
	}

	Ref<WebService> WebService::getApp()
	{
		Ref<Application> app(Application::getApp());
		return CastRef<WebService>(app);
	}

	HttpServiceParam& WebService::getHttpParam()
	{
		return m_httpParam;
	}

	sl_uint16 WebService::getHttpPort()
	{
		return m_httpParam.port;
	}

	void WebService::setHttpPort(sl_uint16 port)
	{
		m_httpParam.port = port;
	}

	void WebService::useAsset(const String& prefix)
	{
		m_httpParam.flagUseAsset = sl_true;
		m_httpParam.prefixAsset = prefix;
	}

	const Ref<WebController>& WebService::getController()
	{
		return m_controller;
	}

	sl_bool WebService::dispatchStartService()
	{
		if (Service::dispatchStartService()) {
			m_http = HttpService::create(m_httpParam);
			if (m_http.isNotNull()) {
				m_http->addProcessor(m_controller);
				return sl_true;
			}
		}
		return sl_false;
	}

	void WebService::dispatchStopService()
	{
		if (m_http.isNotNull()) {
			m_http->release();
			m_http.setNull();
		}
		m_controller.setNull();
		Service::dispatchStopService();
	}

}
