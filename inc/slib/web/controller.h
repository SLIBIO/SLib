#ifndef CHECKHEADER_SLIB_WEB_CONTROLLER
#define CHECKHEADER_SLIB_WEB_CONTROLLER

#include "definition.h"

#include "../core/function.h"
#include "../core/variant.h"
#include "../network/http_service.h"

SLIB_WEB_NAMESPACE_BEGIN

#define SWEB_HANDLER_PARAMS_LIST const slib::Ref<slib::HttpServiceContext>& context, HttpMethod method, const slib::String& path

typedef Function<Variant(SWEB_HANDLER_PARAMS_LIST)> WebHandler;

class WebController : public Object, public IHttpServiceProcessor
{
	SLIB_DECLARE_OBJECT

protected:
	WebController();
	
public:
	static Ref<WebController> create();
	
public:
	void registerHandler(HttpMethod method, const String& path, const WebHandler& handler);
	
protected:
	// override
	sl_bool onHttpRequest(const Ref<HttpServiceContext>& context);
	
protected:
	static String _getHandlerSignature(HttpMethod method, const String& path);
	
protected:
	TreeMap<String, WebHandler> m_handlers;
	
	friend class WebModule;
	
};

#define SWEB_DECALRE_MODULE(NAME) namespace SWEB_MODULE_##NAME { void registerModule(); }

#define SWEB_REGISTER_MODULE(NAME) SWEB_MODULE_##NAME::registerModule();

class WebModule
{
public:
	WebModule(const String& path);
	
public:
	void registerToController();
	
	void addHandler(HttpMethod method, const String& path, const WebHandler& handler);
	
protected:
	String m_path;
	struct _Handler
	{
		HttpMethod method;
		String path;
		WebHandler handler;
	};
	CList<_Handler> m_handlers;
	
};

#define SWEB_BEGIN_MODULE(NAME, PATH) \
namespace SWEB_MODULE_##NAME { \
	SLIB_SAFE_STATIC_GETTER(slib::WebModule, getModule, PATH) \
	void registerModule() { getModule()->registerToController(); }

#define SWEB_END_MODULE }

#define SWEB_HANDLER(METHOD, PATH, NAME) \
	slib::Variant NAME(SWEB_HANDLER_PARAMS_LIST); \
	class _SLIB_WEB_HANDLER_REGISTERER##NAME { public: _SLIB_WEB_HANDLER_REGISTERER##NAME() { getModule()->addHandler(slib::HttpMethod::METHOD, PATH, &NAME); } } _SLIB_WEB_HANDLER_REGISTERER_INSTANCE_##NAME; \
	slib::Variant NAME(SWEB_HANDLER_PARAMS_LIST)

#define SWEB_STRING_PARAM(NAME) slib::String NAME = context->getParameter(#NAME);
#define SWEB_INT_PARAM(NAME, ...) sl_int32 NAME = context->getParameter(#NAME).parseInt32(10, ##__VA_ARGS__);
#define SWEB_INT64_PARAM(NAME, ...) sl_int64 NAME = context->getParameter(#NAME).parseInt64(10, ##__VA_ARGS__);
#define SWEB_FLOAT_PARAM(NAME, ...) float NAME = context->getParameter(#NAME).parseFloat(##__VA_ARGS__);
#define SWEB_DOUBLE_PARAM(NAME, ...) double NAME = context->getParameter(#NAME).parseDouble(##__VA_ARGS__);

SLIB_WEB_NAMESPACE_END

#endif
