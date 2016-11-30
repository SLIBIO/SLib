#include "../../../inc/slib/web/controller.h"

#include "../../../inc/slib/web/service.h"
#include "../../../inc/slib/core/xml.h"

SLIB_WEB_NAMESPACE_BEGIN

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
					if (IMap<String, Variant>::checkInstance(obj.ptr)) {
						context->write(ret.toJsonString());
					} else if (XmlDocument::checkInstance(obj.ptr)) {
						context->write(((XmlDocument*)(obj.ptr))->toString());
					} else if (CMemory::checkInstance(obj.ptr)) {
						context->write((CMemory*)(obj.ptr));
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


void WebModule::registerToController()
{
	Ref<WebService> app = WebService::getApp();
	if (app.isNotNull()) {
		app->getController()->m_handlers.putAll(&m_handlers);
	}
}

void WebModule::addHandler(HttpMethod method, const String& path, const WebHandler& handler)
{
	if (handler.isNotNull()) {
		String sig = WebController::_getHandlerSignature(method, path);
		m_handlers.put(sig, handler);
	}
}

SLIB_WEB_NAMESPACE_END

