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

#include "slib/web/controller.h"

#include "slib/web/service.h"
#include "slib/core/xml.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(WebController, Object)

	WebController::WebController()
	{
	}

	Ref<WebController> WebController::create()
	{
		return new WebController;
	}

	void WebController::registerHandler(HttpMethod method, const String& path, const WebHandler& handler)
	{
		if (handler.isNotNull()) {
			String sig = _getHandlerSignature(method, path);
			m_handlers.put(sig, handler);
		}
	}

	sl_bool WebController::onHttpRequest(const Ref<HttpServiceContext>& context)
	{
		HttpMethod method = context->getMethod();
		String path = context->getPath();
		String sig = _getHandlerSignature(method, path);
		WebHandler handler;
		if (m_handlers.get(sig, &handler)) {
			Variant ret(handler(context, method, path));
			if (ret.isNotNull()) {
				if (ret.isObject()) {
					Ref<Referable> obj = ret.getObject();
					if (obj.isNotNull()) {
						if (XmlDocument* xml = CastInstance<XmlDocument>(obj.get())) {
							context->write(xml->toString());
						} else if (CMemory* mem = CastInstance<CMemory>(obj.get())) {
							context->write(mem);
						} else {
							context->write(ret.toJsonString());
						}
					}
				} else {
					context->write(ret.getString());
				}
				return sl_true;
			}
		}
		return sl_false;
	}

	String WebController::_getHandlerSignature(HttpMethod method, const String& path)
	{
		return HttpMethods::toString(method) + " " + path;
	}


	WebModule::WebModule(const String& path)
	: m_path(path)
	{
	}

	void WebModule::registerToController()
	{
		Ref<WebService> app = WebService::getApp();
		if (app.isNotNull()) {
			Ref<WebController> controller = app->getController();
			if (controller.isNotNull()) {
				ListLocker<Handler> list(m_handlers);
				for (sl_size i = 0; i < list.count; i++) {
					controller->registerHandler(list[i].method, m_path + list[i].path, list[i].handler);
				}
			}
		}
	}

	void WebModule::addHandler(HttpMethod method, const String& path, const WebHandler& handler)
	{
		if (handler.isNotNull()) {
			Handler h;
			h.method = method;
			h.path = path;
			h.handler = handler;
			m_handlers.add_NoLock(h);
		}
	}

}
