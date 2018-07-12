/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/network/url_request.h"

#include "slib/network/url.h"
#include "slib/core/json.h"
#include "slib/core/string_buffer.h"
#include "slib/core/thread_pool.h"
#include "slib/core/safe_static.h"
#include "slib/core/event.h"
#include "slib/core/system.h"

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
		flagSynchronous = sl_false;
	}
	
	UrlRequestParam::UrlRequestParam(const UrlRequestParam& other) = default;
	
	UrlRequestParam::~UrlRequestParam()
	{
	}
	
	void UrlRequestParam::setRequestBodyAsString(const String& str)
	{
		requestBody = str.toMemory();
	}
	
	void UrlRequestParam::setRequestBodyAsJson(const Json& json)
	{
		requestBody = json.toJsonString().toMemory();
	}
	
	void UrlRequestParam::setRequestBodyAsXml(const Ref<XmlDocument>& xml)
	{
		if (xml.isNotNull()) {
			requestBody = xml->toString().toMemory();
		} else {
			requestBody.setNull();
		}
	}
	
	SLIB_DEFINE_OBJECT(UrlRequest, Object)
	
	typedef HashMap< UrlRequest*, Ref<UrlRequest> > _priv_UrlRequestMap;
	SLIB_SAFE_STATIC_GETTER(_priv_UrlRequestMap, _getUrlRequestMap)
	
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
	
	HttpHeaderMap UrlRequest::getAdditionalRequestHeaders()
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
		m_additionalRequestHeaders = param.additionalRequestHeaders;
		
		m_listener = param.listener;
		m_onComplete = param.onComplete;
		m_onReceiveContent = param.onReceiveContent;
		m_dispatcher = param.dispatcher;
		m_flagUseBackgroundSession = param.flagUseBackgroundSession;
		m_flagSelfAlive = param.flagSelfAlive && !(param.flagSynchronous);
		m_flagStoreResponseContent = param.flagStoreResponseContent;
		
		if (m_flagSelfAlive) {
			_priv_UrlRequestMap* map = _getUrlRequestMap();
			if (map) {
				map->put(this, this);
			}
		}

	}
	
	void UrlRequest::_removeFromMap()
	{
		if (m_flagSelfAlive) {
			_priv_UrlRequestMap* map = _getUrlRequestMap();
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
	
	String UrlRequest::_buildParameters(const Map<String, Variant>& params)
	{
		StringBuffer sb;
		sl_bool flagFirst = sl_true;
		for (auto& pair : params) {
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
	
	String UrlRequest::_buildParameters(const HashMap<String, Variant>& params)
	{
		StringBuffer sb;
		sl_bool flagFirst = sl_true;
		for (auto& pair : params) {
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
					if (CMap<String, Variant>* map = CastInstance< CMap<String, Variant> >(obj.get())) {
						body = _buildParameters(map).toMemory();
					} else if (CHashMap<String, Variant>* hashMap = CastInstance< CHashMap<String, Variant> >(obj.get())) {
						body = _buildParameters(hashMap).toMemory();
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
