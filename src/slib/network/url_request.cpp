#include "../../../inc/slib/network/url_request.h"

#include "../../../inc/slib/core/json.h"

SLIB_NETWORK_NAMESPACE_BEGIN

void IUrlRequestListener::onComplete(UrlRequest *request)
{
}

void IUrlRequestListener::onError(UrlRequest* request)
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
	flagKeepReference = sl_true;
	flagStoreResponseContent = sl_true;
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
	
	m_responseStatus = HttpStatus::Unknown;
	
}

Ref<UrlRequest> UrlRequest::send(const String& url, const UrlRequestParam& param)
{
	if (url.isNotEmpty()) {
		Ref<UrlRequest> request = _create(param, url, String::null());
		if (request.isNotNull()) {
			return request;
		}
	}
	return Ref<UrlRequest>::null();
}

Ref<UrlRequest> UrlRequest::downloadToFile(const String& filePath, const String& url, const UrlRequestParam& param)
{
	if (url.isNotEmpty()) {
		Ref<UrlRequest> request = _create(param, url, filePath);
		if (request.isNotNull()) {
			return request;
		}
	}
	return Ref<UrlRequest>::null();
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

const Function<void(UrlRequest*)>& UrlRequest::getOnError()
{
	return m_onError;
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

sl_bool UrlRequest::isKeepingReference()
{
	return m_flagKeepReference;
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

void UrlRequest::_init(const UrlRequestParam& param, const String& url, const String& downloadFilePath)
{
	m_url = url;
	m_downloadFilePath = downloadFilePath;
	
	m_method = param.method;
	m_requestBody = param.requestBody;
	m_requestHeaders = param.requestHeaders;
	m_additionalRequestHeaders = param.additionalRequestHeaders;
	
	m_listener = param.listener;
	m_onComplete = param.onComplete;
	m_onError = param.onError;
	m_onReceiveContent = param.onReceiveContent;
	m_dispatcher = param.dispatcher;
	m_flagUseBackgroundSession = param.flagUseBackgroundSession;
	m_flagKeepReference = param.flagKeepReference;
	m_flagStoreResponseContent = param.flagStoreResponseContent;

	if (param.flagKeepReference) {
		_UrlRequestMap* map = _getUrlRequestMap();
		if (map) {
			map->put(this, this);
		}
	}
	
}

void UrlRequest::_removeFromMap()
{
	if (m_flagKeepReference) {
		_UrlRequestMap* map = _getUrlRequestMap();
		if (map) {
			map->remove(this);
		}
	}
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
			m_dispatcher->dispatch(SLIB_CALLBACK_REF(UrlRequest, _runCallback, this, m_onComplete));
		} else {
			m_onComplete(this);
		}
	}
	_removeFromMap();
}

void UrlRequest::onError()
{
	if (m_flagClosed) {
		return;
	}
	m_flagClosed = sl_true;
	PtrLocker<IUrlRequestListener> listener(m_listener);
	if (listener.isNotNull()) {
		listener->onError(this);
	}
	if (m_onError.isNotNull()) {
		if (m_dispatcher.isNotNull()) {
			m_dispatcher->dispatch(SLIB_CALLBACK_REF(UrlRequest, _runCallback, this, m_onError));
		} else {
			m_onError(this);
		}
	}
	_removeFromMap();
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

SLIB_NETWORK_NAMESPACE_END
