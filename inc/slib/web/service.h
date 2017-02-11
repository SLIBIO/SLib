#ifndef CHECKHEADER_SLIB_WEB_SERVICE
#define CHECKHEADER_SLIB_WEB_SERVICE

#include "definition.h"

#include "controller.h"

#include "../core/service.h"
#include "../network/http_service.h"

SLIB_WEB_NAMESPACE_BEGIN

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
	// override
	sl_bool dispatchStartService();
	
	// override
	void dispatchStopService();
	
protected:
	Ref<HttpService> m_http;
	HttpServiceParam m_httpParam;
	Ref<WebController> m_controller;
	
};

SLIB_WEB_NAMESPACE_END

#endif
