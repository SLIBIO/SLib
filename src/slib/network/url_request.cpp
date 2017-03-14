#include "../../../inc/slib/network/url_request.h"

#include "../../../inc/slib/network/url.h"
#include "../../../inc/slib/core/json.h"
#include "../../../inc/slib/core/string_buffer.h"
#include "../../../inc/slib/core/safe_static.h"
#include "../../../inc/slib/core/log.h"

namespace slib
{

	IUrlRequestListener::IUrlRequestListener()
	{
	}

	IUrlRequestListener::~IUrlRequestListener()
	{
	}

	void IUrlRequestListener::onComplete(UrlRequest *request)
	{
	}
	
	void IUrlRequestListener::onResponse(UrlRequest* request, HttpStatus status)
	{
	}
	
	void IUrlRequestListener::onReceiveContent(UrlRequest* request, const void* data, sl_size len)
	{
	}
	
	void IUrlRequestListener::onDownloadContent(UrlRequest* request, sl_uint64 len)
	{
	}
	
	void IUrlRequestListener::onUploadBody(UrlRequest* request, sl_uint64 len)
	{
	}
	
	UrlRequestParam::UrlRequestParam()
	{
		method = HttpMethod::GET;
		flagUseBackgroundSession = sl_false;
		flagSelfAlive = sl_true;
		flagStoreResponseContent = sl_true;
	}
	
	UrlRequestParam::UrlRequestParam(const UrlRequestParam& other) = default;
	
	UrlRequestParam::~UrlRequestParam()
	{
	}
	
	SLIB_DEFINE_OBJECT(UrlRequest, Object)
	
	typedef HashMap< UrlRequest*, Ref<UrlRequest> > _UrlRequestMap;
	SLIB_SAFE_STATIC_GETTER(_UrlRequestMap, _getUrlRequestMap)
	
	UrlRequest::UrlRequest()
	{
		m_sizeBodySent = 0;
		m_sizeContentTotal = 0;
		m_sizeContentReceived = 0;
		m_flagClosed = sl_false;
		m_flagError = sl_false;
		
		m_responseStatus = HttpStatus::Unknown;
		
		m_method = HttpMethod::GET;
		m_flagSelfAlive = sl_false;
		m_flagStoreResponseContent = sl_true;
		m_flagUseBackgroundSession = sl_false;

	}
	
	UrlRequest::~UrlRequest()
	{
	}
	
	Ref<UrlRequest> UrlRequest::send(const UrlRequestParam& param)
	{
		return _send(param, String::null());
	}
	
	Ref<UrlRequest> UrlRequest::downloadToFile(const String& filePath, const UrlRequestParam& param)
	{
		return _send(param, filePath);
	}
	
	Ref<UrlRequest> UrlRequest::send(const String& url, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::GET;
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> UrlRequest::send(const String& url, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::GET;
		rp.onComplete = onComplete;
		rp.dispatcher = dispatcher;
		return send(rp);
	}
	
	Ref<UrlRequest> UrlRequest::send(const String& url, const Map<String, Variant>& params, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::GET;
		rp.parameters = params;
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> UrlRequest::send(const String& url, const Map<String, Variant>& params, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::GET;
		rp.parameters = params;
		rp.onComplete = onComplete;
		rp.dispatcher = dispatcher;
		return send(rp);
	}
	
	Ref<UrlRequest> UrlRequest::send(HttpMethod method, const String& url, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> UrlRequest::send(HttpMethod method, const String& url, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.onComplete = onComplete;
		rp.dispatcher = dispatcher;
		return send(rp);
	}
	
	Ref<UrlRequest> UrlRequest::send(HttpMethod method, const String& url, const Map<String, Variant>& params, const Variant& body, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.parameters = params;
		rp.requestBody = _buildRequestBody(body);
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> UrlRequest::send(HttpMethod method, const String& url, const Map<String, Variant>& params, const Variant& body, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.parameters = params;
		rp.requestBody = _buildRequestBody(body);
		rp.onComplete = onComplete;
		rp.dispatcher = dispatcher;
		return send(rp);
	}
	
	Ref<UrlRequest> UrlRequest::sendJson(HttpMethod method, const String& url, const Map<String, Variant>& params, const Json& json, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.parameters = params;
		rp.requestBody = json.toJsonString().toMemory();
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> UrlRequest::sendJson(HttpMethod method, const String& url, const Map<String, Variant>& params, const Json& json, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = method;
		rp.parameters = params;
		rp.requestBody = json.toJsonString().toMemory();
		rp.onComplete = onComplete;
		rp.dispatcher = dispatcher;
		return send(rp);
	}
	
	Ref<UrlRequest> UrlRequest::post(const String& url, const Variant& body, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.requestBody = _buildRequestBody(body);
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> UrlRequest::post(const String& url, const Variant& body, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.requestBody = _buildRequestBody(body);
		rp.onComplete = onComplete;
		rp.dispatcher = dispatcher;
		return send(rp);
	}
	
	Ref<UrlRequest> UrlRequest::post(const String& url, const Map<String, Variant>& params, const Variant& body, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.parameters = params;
		rp.requestBody = _buildRequestBody(body);
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> UrlRequest::post(const String& url, const Map<String, Variant>& params, const Variant& body, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.parameters = params;
		rp.requestBody = _buildRequestBody(body);
		rp.onComplete = onComplete;
		rp.dispatcher = dispatcher;
		return send(rp);
	}
	
	Ref<UrlRequest> UrlRequest::postJson(const String& url, const Json& json, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.requestBody = json.toJsonString().toMemory();
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> UrlRequest::postJson(const String& url, const Json& json, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.requestBody = json.toJsonString().toMemory();
		rp.onComplete = onComplete;
		rp.dispatcher = dispatcher;
		return send(rp);
	}
	
	Ref<UrlRequest> UrlRequest::postJson(const String& url, const Map<String, Variant>& params, const Json& json, const Function<void(UrlRequest*)>& onComplete)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.parameters = params;
		rp.requestBody = json.toJsonString().toMemory();
		rp.onComplete = onComplete;
		return send(rp);
	}
	
	Ref<UrlRequest> UrlRequest::postJson(const String& url, const Map<String, Variant>& params, const Json& json, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher)
	{
		UrlRequestParam rp;
		rp.url = url;
		rp.method = HttpMethod::POST;
		rp.parameters = params;
		rp.requestBody = json.toJsonString().toMemory();
		rp.onComplete = onComplete;
		rp.dispatcher = dispatcher;
		return send(rp);
	}
	
	const String& UrlRequest::getUrl()
	{
		return m_url;
	}
	
	const String& UrlRequest::getDownloadingFilePath()
	{
		return m_downloadFilePath;
	}
	
	HttpMethod UrlRequest::getMethod()
	{
		return m_method;
	}
	
	const Memory& UrlRequest::getRequestBody()
	{
		return m_requestBody;
	}
	
	sl_size UrlRequest::getRequestBodySize()
	{
		return m_requestBody.getSize();
	}
	
	Map<String, Variant> UrlRequest::getParameters()
	{
		return m_parameters;
	}
	
	Map<String, String> UrlRequest::getRequestHeaders()
	{
		return m_requestHeaders;
	}
	
	Map<String, String> UrlRequest::getAdditionalRequestHeaders()
	{
		return m_additionalRequestHeaders;
	}
	
	Memory UrlRequest::getResponseContent()
	{
		return m_bufResponseContent.merge();
	}
	
	String UrlRequest::getResponseContentAsString()
	{
		return String::fromUtf(m_bufResponseContent.merge());
	}
	
	Variant UrlRequest::getResponseContentAsJson()
	{
		Memory mem = m_bufResponseContent.merge();
		String16 s = String16::fromUtf(mem);
		return Json::parseJson(s);
	}
	
	Ref<XmlDocument> UrlRequest::getResponseContentAsXml()
	{
		Memory mem = m_bufResponseContent.merge();
		String16 s = String16::fromUtf(mem);
		return Xml::parseXml(s);
	}
	
	HttpStatus UrlRequest::getResponseStatus()
	{
		return m_responseStatus;
	}
	
	String UrlRequest::getResponseMessage()
	{
		return m_responseMessage;
	}
	
	Map<String, String> UrlRequest::getResponseHeaders()
	{
		return m_responseHeaders;
	}
	
	String UrlRequest::getResponseHeader(const String& name)
	{
		return m_responseHeaders.getValue(name, String::null());
	}
	
	const Ptr<IUrlRequestListener>& UrlRequest::getListener()
	{
		return m_listener;
	}
	
	const Function<void(UrlRequest*)>& UrlRequest::getOnComplete()
	{
		return m_onComplete;
	}
	
	const Function<void(UrlRequest*, const void*, sl_size)>& UrlRequest::getOnReceiveContent()
	{
		return m_onReceiveContent;
	}
	
	const Ref<Dispatcher>& UrlRequest::getDispatcher()
	{
		return m_dispatcher;
	}
	
	sl_bool UrlRequest::isUsingBackgroundSession()
	{
		return m_flagUseBackgroundSession;
	}
	
	sl_bool UrlRequest::isSelfAlive()
	{
		return m_flagSelfAlive;
	}
	
	sl_bool UrlRequest::isStoringResponseContent()
	{
		return m_flagStoreResponseContent;
	}
	
	sl_uint64 UrlRequest::getSentRequestBodySize()
	{
		return m_sizeBodySent;
	}
	
	sl_uint64 UrlRequest::getResponseContentSize()
	{
		return m_sizeContentReceived;
	}
	
	sl_uint64 UrlRequest::getExpectedResponseContentSize()
	{
		return m_sizeContentTotal;
	}
	
	void UrlRequest::cancel()
	{
		if (m_flagClosed) {
			return;
		}
		ObjectLocker lock(this);
		if (m_flagClosed) {
			return;
		}
		m_flagClosed = sl_true;
		_removeFromMap();
		_cancel();
	}
	
	sl_bool UrlRequest::isError()
	{
		if (m_responseStatus >= HttpStatus::BadRequest) {
			return sl_true;
		}
		return m_flagError;
	}
	
	Ref<UrlRequest> UrlRequest::_send(const UrlRequestParam& param, const String& downloadFilePath)
	{
		String url = param.url;
		if (url.isNotEmpty()) {
			String url = param.url;
			if (param.parameters.isNotEmpty()) {
				if (url.contains('?')) {
					url += "&";
				} else {
					url += "?";
				}
				url += _buildParameters(param.parameters);
			}
			Ref<UrlRequest> request = _create(param, url, String::null());
			if (request.isNotNull()) {
				return request;
			}
		}
		_onCreateError(param, param.url, String::null());
		return sl_null;
	}
	
	void UrlRequest::_init(const UrlRequestParam& param, const String& url, const String& downloadFilePath)
	{
		m_url = url;
		m_downloadFilePath = downloadFilePath;
		
		m_method = param.method;
		m_requestBody = param.requestBody;
		m_parameters = param.parameters;
		m_requestHeaders = param.requestHeaders;
		m_additionalRequestHeaders = param.additionalRequestHeaders;
		
		m_listener = param.listener;
		m_onComplete = param.onComplete;
		m_onReceiveContent = param.onReceiveContent;
		m_dispatcher = param.dispatcher;
		m_flagUseBackgroundSession = param.flagUseBackgroundSession;
		m_flagSelfAlive = param.flagSelfAlive;
		m_flagStoreResponseContent = param.flagStoreResponseContent;
		
		if (param.flagSelfAlive) {
			_UrlRequestMap* map = _getUrlRequestMap();
			if (map) {
				map->put(this, this);
			}
		}
		
	}
	
	void UrlRequest::_removeFromMap()
	{
		if (m_flagSelfAlive) {
			_UrlRequestMap* map = _getUrlRequestMap();
			if (map) {
				map->remove(this);
			}
		}
	}
	
	void UrlRequest::_cancel()
	{
	}
	
	void UrlRequest::onComplete()
	{
		if (m_flagClosed) {
			return;
		}
		m_flagClosed = sl_true;
		PtrLocker<IUrlRequestListener> listener(m_listener);
		if (listener.isNotNull()) {
			listener->onComplete(this);
		}
		if (m_onComplete.isNotNull()) {
			if (m_dispatcher.isNotNull()) {
				m_dispatcher->dispatch(SLIB_BIND_REF(void(), UrlRequest, _runCallback, this, m_onComplete));
			} else {
				m_onComplete(this);
			}
		}
		_removeFromMap();
	}
	
	void UrlRequest::onError()
	{
		m_flagError = sl_true;
		onComplete();
	}
	
	void UrlRequest::onResponse()
	{
		if (m_flagClosed) {
			return;
		}
		m_sizeContentReceived = 0;
		m_bufResponseContent.clear();
		PtrLocker<IUrlRequestListener> listener(m_listener);
		if (listener.isNotNull()) {
			listener->onResponse(this, m_responseStatus);
		}
	}
	
	void UrlRequest::onReceiveContent(const void* data, sl_size len, const Memory& mem)
	{
		if (m_flagClosed) {
			return;
		}
		if (m_flagStoreResponseContent) {
			if (mem.isNotEmpty()) {
				m_bufResponseContent.add(mem);
			} else {
				m_bufResponseContent.add(Memory::create(data, len));
			}
			m_sizeContentReceived = m_bufResponseContent.getSize();
		} else {
			m_sizeContentReceived += len;
		}
		m_onReceiveContent(this, data, len);
		PtrLocker<IUrlRequestListener> listener(m_listener);
		if (listener.isNotNull()) {
			listener->onReceiveContent(this, data, len);
		}
	}
	
	void UrlRequest::onDownloadContent(sl_uint64 size)
	{
		if (m_flagClosed) {
			return;
		}
		m_sizeContentReceived += size;
		PtrLocker<IUrlRequestListener> listener(m_listener);
		if (listener.isNotNull()) {
			listener->onDownloadContent(this, size);
		}
	}
	
	void UrlRequest::onUploadBody(sl_uint64 size)
	{
		if (m_flagClosed) {
			return;
		}
		PtrLocker<IUrlRequestListener> listener(m_listener);
		if (listener.isNotNull()) {
			listener->onUploadBody(this, size);
		}
	}
	
	void UrlRequest::_runCallback(const Function<void(UrlRequest*)>& callback)
	{
		callback(this);
	}
	
	void UrlRequest::_onCreateError(const UrlRequestParam& param, const String& url, const String& downloadFilePath)
	{
		Ref<UrlRequest> req = new UrlRequest;
		if (req.isNotNull()) {
			LogError("UrlRequest", "Failed to create request on %s", url);
			req->_init(param, url, downloadFilePath);
			req->onError();
		}
	}
	
	String UrlRequest::_buildParameters(const Map<String, Variant>& params)
	{
		StringBuffer sb;
		sl_bool flagFirst = sl_true;
		for (auto pair : params) {
			if (!flagFirst) {
				sb.addStatic("&", 1);
			}
			flagFirst = sl_false;
			sb.add(pair.key);
			sb.addStatic("=", 1);
			sb.add(Url::encodeUriComponentByUTF8(pair.value.getString()));
		}
		return sb.merge();
	}
	
	Memory UrlRequest::_buildRequestBody(const Variant& varBody)
	{
		Memory body;
		if (varBody.isNotNull()) {
			if (varBody.isObject()) {
				Ref<Referable> obj = varBody.getObject();
				if (obj.isNotNull()) {
					if (IMap<String, Variant>* map = CastInstance< IMap<String, Variant> >(obj.get())) {
						body = _buildParameters(map).toMemory();
					} else if (XmlDocument* xml = CastInstance<XmlDocument>(obj.get())) {
						body = xml->toString().toMemory();
					} else if (CMemory* mem = CastInstance<CMemory>(obj.get())) {
						body = mem;
					}
				}
			} else {
				body = varBody.getString().toMemory();
			}
		}
		return body;
	}
	
}
