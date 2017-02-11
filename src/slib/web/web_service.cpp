#include "../../../inc/slib/web/service.h"

SLIB_WEB_NAMESPACE_BEGIN

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

SLIB_WEB_NAMESPACE_END

