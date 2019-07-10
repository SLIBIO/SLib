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

#include "slib/network/url_request.h"

#include "slib/network/url.h"
#include "slib/core/json.h"
#include "slib/core/thread_pool.h"
#include "slib/core/safe_static.h"
#include "slib/core/event.h"
#include "slib/core/system.h"

namespace slib
{

	namespace priv
	{
		namespace url_request
		{
			typedef HashMap< UrlRequest*, Ref<UrlRequest> > UrlRequestMap;
			SLIB_SAFE_STATIC_GETTER(UrlRequestMap, getUrlRequestMap)

		}
	}
	
	using namespace priv::url_request;
	
	SLIB_DEFINE_OBJECT(UrlRequest, Object)
	
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
	
#define URL_REQUEST UrlRequest
#include "url_request_common.inc"
	
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
	
	HashMap<String, Variant> UrlRequest::getParameters()
	{
		return m_parameters;
	}
	
	HttpHeaderMap UrlRequest::getRequestHeaders()
	{
		return m_requestHeaders;
	}
	
	Memory UrlRequest::getResponseContent()
	{
		return m_bufResponseContent.merge();
	}
	
	String UrlRequest::getResponseContentAsString()
	{
		return String::fromUtf(m_bufResponseContent.merge());
	}
	
	Json UrlRequest::getResponseContentAsJson()
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
	
	HttpHeaderMap UrlRequest::getResponseHeaders()
	{
		return m_responseHeaders;
	}
	
	String UrlRequest::getResponseHeader(const String& name)
	{
		return m_responseHeaders.getValue(name, String::null());
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

	String UrlRequest::getLastErrorMessage()
	{
		return m_lastErrorMessage;
	}
	
	sl_bool UrlRequest::isClosed()
	{
		return m_flagClosed;
	}
	
	void UrlRequest::_sendSync()
	{
		Ref<Event> ev = Event::create();
		if (ev.isNotNull()) {
			m_eventSync = ev;
			_sendAsync();
			ev->wait();
			return;
		}
		onError();
	}
	
	void UrlRequest::_sendSync_call()
	{
		_sendSync();
	}

	class UrlRequest_AsyncPool
	{
	public:
		Ref<ThreadPool> threadPool;
		
	public:
		UrlRequest_AsyncPool()
		{
			threadPool = ThreadPool::create();
		}
		
	};
	
	SLIB_SAFE_STATIC_GETTER(UrlRequest_AsyncPool, Get_UrlRequestAsyncPool)
	
	void UrlRequest::_sendAsync()
	{
		UrlRequest_AsyncPool* pool = Get_UrlRequestAsyncPool();
		if (pool) {
			if (pool->threadPool->addTask(SLIB_FUNCTION_WEAKREF(UrlRequest, _sendSync_call, this))) {
				return;
			}
		}
		onError();
	}
		
	void UrlRequest::_init(const UrlRequestParam& param, const String& url)
	{
		m_url = url;
		m_downloadFilePath = param.downloadFilePath;
		
		m_method = param.method;
		m_requestBody = param.requestBody;
		m_parameters = param.parameters;
		m_requestHeaders = param.requestHeaders;

		m_onComplete = param.onComplete;
		m_onResponse = param.onResponse;
		m_onReceiveContent = param.onReceiveContent;
		m_onDownloadContent = param.onDownloadContent;
		m_onUploadBody = param.onUploadBody;
		m_dispatcher = param.dispatcher;
		
		m_flagUseBackgroundSession = param.flagUseBackgroundSession;
		m_flagSelfAlive = param.flagSelfAlive && !(param.flagSynchronous);
		m_flagStoreResponseContent = param.flagStoreResponseContent;
		
		m_timeout = param.timeout;
		m_flagAllowInsecureConnection = param.flagAllowInsecureConnection;
		
		if (m_flagSelfAlive) {
			UrlRequestMap* map = getUrlRequestMap();
			if (map) {
				map->put(this, this);
			}
		}

	}
	
	void UrlRequest::_removeFromMap()
	{
		if (m_flagSelfAlive) {
			UrlRequestMap* map = getUrlRequestMap();
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
		if (m_onComplete.isNotNull()) {
			if (m_dispatcher.isNotNull()) {
				m_dispatcher->dispatch(SLIB_BIND_REF(void(), UrlRequest, _runCallback, this, m_onComplete));
			} else {
				m_onComplete(this);
			}
		}
		_removeFromMap();
		
		if (m_eventSync.isNotNull()) {
			m_eventSync->set();
		}
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
		m_onResponse(this, m_responseStatus);
	}
	
	void UrlRequest::onReceiveContent(const void* data, sl_size len, const Memory& mem)
	{
		if (m_flagClosed) {
			return;
		}
		if (m_flagStoreResponseContent) {
			if (mem.isNotNull()) {
				m_bufResponseContent.add(mem);
			} else {
				m_bufResponseContent.add(Memory::create(data, len));
			}
			m_sizeContentReceived = m_bufResponseContent.getSize();
		} else {
			m_sizeContentReceived += len;
		}
		m_onReceiveContent(this, data, len);
	}
	
	void UrlRequest::onDownloadContent(sl_uint64 size)
	{
		if (m_flagClosed) {
			return;
		}
		m_sizeContentReceived += size;
		m_onDownloadContent(this, size);
	}
	
	void UrlRequest::onUploadBody(sl_uint64 size)
	{
		if (m_flagClosed) {
			return;
		}
		m_onUploadBody(this, size);
	}
	
	void UrlRequest::_runCallback(const Function<void(UrlRequest*)>& callback)
	{
		callback(this);
	}
	
}
