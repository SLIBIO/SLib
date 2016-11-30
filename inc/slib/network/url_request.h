#ifndef CHECKHEADER_SLIB_NETWORK_URL_REQUEST
#define CHECKHEADER_SLIB_NETWORK_URL_REQUEST

#include "definition.h"

#include "http_common.h"

#include "../core/string.h"
#include "../core/variant.h"
#include "../core/xml.h"

SLIB_NAMESPACE_BEGIN

class UrlRequest;

class SLIB_EXPORT IUrlRequestListener
{
public:
	virtual void onComplete(UrlRequest* request);
	
	virtual void onError(UrlRequest* request);
	
	virtual void onResponse(UrlRequest* request, HttpStatus status);
	
	virtual void onReceiveContent(UrlRequest* request, const void* data, sl_size len);
	
	virtual void onDownloadContent(UrlRequest* request, sl_uint64 len);
	
	virtual void onUploadBody(UrlRequest* request, sl_uint64 len);
	
};

class SLIB_EXPORT UrlRequestParam
{
public:
	HttpMethod method;
	Map<String, String> requestHeaders;
	Map<String, String> additionalRequestHeaders;
	Memory requestBody;
	
	Ptr<IUrlRequestListener> listener;
	Function<void(UrlRequest*)> onComplete;
	Function<void(UrlRequest*)> onError;
	Function<void(UrlRequest*, const void*, sl_size)> onReceiveContent;
	Ref<Dispatcher> dispatcher;
	
	sl_bool flagUseBackgroundSession;
	sl_bool flagKeepReference;
	sl_bool flagStoreResponseContent;

public:
	UrlRequestParam();
	
};

class SLIB_EXPORT UrlRequest : public Object
{
	SLIB_DECLARE_OBJECT
	
protected:
	UrlRequest();
		
public:
	static Ref<UrlRequest> send(const String& url, const UrlRequestParam& param);
	
	static Ref<UrlRequest> downloadToFile(const String& filePath, const String& url, const UrlRequestParam& param);
	
	static Ref<UrlRequest> sendGet(const String& url, const Function<void(UrlRequest*)>& onComplete);
	
	static Ref<UrlRequest> sendGet(const String& url, const Function<void(UrlRequest*)>& onComplete, const Function<void(UrlRequest*)>& onError);
	
	static Ref<UrlRequest> sendPost(const String& url, const String& body, const Function<void(UrlRequest*)>& onComplete);
	
	static Ref<UrlRequest> sendPost(const String& url, const String& body, const Function<void(UrlRequest*)>& onComplete, const Function<void(UrlRequest*)>& onError);
	
public:
	const String& getUrl();
	
	const String& getDownloadingFilePath();
	
	
	HttpMethod getMethod();
	
	const Memory& getRequestBody();

	sl_size getRequestBodySize();
	
	Map<String, String> getRequestHeaders();
	
	Map<String, String> getAdditionalRequestHeaders();
	
	
	Memory getResponseContent();
	
	String getResponseContentAsString();
	
	Variant getResponseContentAsJson();
	
	Ref<XmlDocument> getResponseContentAsXml();
	
	
	HttpStatus getResponseStatus();
	
	String getResponseMessage();
	
	Map<String, String> getResponseHeaders();
	
	String getResponseHeader(const String& name);
	
	
	const Ptr<IUrlRequestListener>& getListener();
	
	const Function<void(UrlRequest*)>& getOnComplete();
	
	const Function<void(UrlRequest*)>& getOnError();

	const Function<void(UrlRequest*, const void*, sl_size)>& getOnReceiveContent();
	
	const Ref<Dispatcher>& getDispatcher();
	
	sl_bool isUsingBackgroundSession();
	
	sl_bool isKeepingReference();

	sl_bool isStoringResponseContent();
	
	
	sl_uint64 getSentRequestBodySize();
	
	sl_uint64 getResponseContentSize();
	
	sl_uint64 getExpectedResponseContentSize();
	
	
	void cancel();
	
protected:
	static Ref<UrlRequest> _create(const UrlRequestParam& param, const String& url, const String& downloadFilePath);
	
	void _init(const UrlRequestParam& param, const String& url, const String& downloadFilePath);
	
	void _removeFromMap();
	
	virtual void _cancel() = 0;
	
	void onComplete();
	
	void onError();
	
	void onResponse();
	
	void onReceiveContent(const void* data, sl_size len, const Memory& mem);
	
	void onDownloadContent(sl_uint64 size);
	
	void onUploadBody(sl_uint64 size);

	void _runCallback(const Function<void(UrlRequest*)>& callback);
	
protected:
	String m_url;
	String m_downloadFilePath;
	
	HttpMethod m_method;
	Memory m_requestBody;
	Map<String, String> m_requestHeaders;
	Map<String, String> m_additionalRequestHeaders;

	MemoryBuffer m_bufResponseContent;
	HttpStatus m_responseStatus;
	SafeString m_responseMessage;
	SafeMap<String, String> m_responseHeaders;
	
	Ptr<IUrlRequestListener> m_listener;
	Function<void(UrlRequest*)> m_onComplete;
	Function<void(UrlRequest*)> m_onError;
	Function<void(UrlRequest*, const void*, sl_size)> m_onReceiveContent;
	Ref<Dispatcher> m_dispatcher;
	sl_bool m_flagUseBackgroundSession;
	sl_bool m_flagKeepReference;
	sl_bool m_flagStoreResponseContent;
	
	sl_uint64 m_sizeBodySent;
	sl_uint64 m_sizeContentTotal;
	sl_uint64 m_sizeContentReceived;
	
	sl_bool m_flagClosed;
	
};

SLIB_NAMESPACE_END

#endif
