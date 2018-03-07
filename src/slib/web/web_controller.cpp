/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
				ListLocker<_Handler> list(m_handlers);
				for (sl_size i = 0; i < list.count; i++) {
					controller->registerHandler(list[i].method, m_path + list[i].path, list[i].handler);
				}
			}
		}
	}

	void WebModule::addHandler(HttpMethod method, const String& path, const WebHandler& handler)
	{
		if (handler.isNotNull()) {
			_Handler h;
			h.method = method;
			h.path = path;
			h.handler = handler;
			m_handlers.add_NoLock(h);
		}
	}

}
