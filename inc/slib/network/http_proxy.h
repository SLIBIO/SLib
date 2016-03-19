#ifndef CHECKHEADER_SLIB_NETWORK_HTTP_PROXY
#define CHECKHEADER_SLIB_NETWORK_HTTP_PROXY

#include "http_service.h"

SLIB_NETWORK_NAMESPACE_BEGIN

class SLIB_EXPORT HttpProxyParam : public HttpServiceParam
{
public:
};

class SLIB_EXPORT IHttpProxyConnectListener
{
public:
	virtual void onConnect(HttpServiceConnection* connection, AsyncStream* streamRemote, sl_bool flagError) = 0;
};

class SLIB_EXPORT HttpProxy : public HttpService
{
protected:
	HttpProxy();
	
	~HttpProxy();
	
public:
	static Ref<HttpProxy> create(const HttpProxyParam& param);

public:
	void release();

public:
	// override
	sl_bool preprocessRequest(HttpServiceContext* context);
	
public:
	virtual sl_bool connectTo(HttpServiceConnection* connection, const String& hostAddress, const Ptr<IHttpProxyConnectListener>& listener);

protected:
	sl_bool _init(const HttpProxyParam& param);
	
	void _processConnect(Ref<HttpServiceContext> context);
	
	void _processProxy(Ref<HttpServiceContext> context, String host, sl_uint32 port);

};

SLIB_NETWORK_NAMESPACE_END

#endif

