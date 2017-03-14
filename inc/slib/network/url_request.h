#ifndef CHECKHEADER_SLIB_NETWORK_URL_REQUEST
#define CHECKHEADER_SLIB_NETWORK_URL_REQUEST

#include "definition.h"

#include "http_common.h"

#include "../core/string.h"
#include "../core/variant.h"
#include "../core/json.h"
#include "../core/xml.h"

namespace slib
{

	class UrlRequest;
	
	class SLIB_EXPORT IUrlRequestListener
	{
	public:
		IUrlRequestListener();

		virtual ~IUrlRequestListener();

	public:
		virtual void onComplete(UrlRequest* request);
		
		virtual void onResponse(UrlRequest* request, HttpStatus status);
		
		virtual void onReceiveContent(UrlRequest* request, const void* data, sl_size len);
		
		virtual void onDownloadContent(UrlRequest* request, sl_uint64 len);
		
		virtual void onUploadBody(UrlRequest* request, sl_uint64 len);
		
	};
	
	class SLIB_EXPORT UrlRequestParam
	{
	public:
		String url;
		HttpMethod method;
		Map<String, Variant> parameters;
		Map<String, String> requestHeaders;
		Map<String, String> additionalRequestHeaders;
		Memory requestBody;
		
		Ptr<IUrlRequestListener> listener;
		Function<void(UrlRequest*)> onComplete;
		Function<void(UrlRequest*, const void*, sl_size)> onReceiveContent;
		Ref<Dispatcher> dispatcher;
		
		sl_bool flagUseBackgroundSession;
		sl_bool flagSelfAlive;
		sl_bool flagStoreResponseContent;
		
	public:
		UrlRequestParam();
		
		UrlRequestParam(const UrlRequestParam& param);
		
		~UrlRequestParam();
		
	};
	
	class SLIB_EXPORT UrlRequest : public Object
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		UrlRequest();
		
		~UrlRequest();
		
	public:
		static Ref<UrlRequest> send(const UrlRequestParam& param);
		
		static Ref<UrlRequest> downloadToFile(const String& filePath, const UrlRequestParam& param);
		
		static Ref<UrlRequest> send(const String& url, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> send(const String& url, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> send(const String& url, const Map<String, Variant>& params, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> send(const String& url, const Map<String, Variant>& params, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> send(HttpMethod method, const String& url, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> send(HttpMethod method, const String& url, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> send(HttpMethod method, const String& url, const Map<String, Variant>& params, const Variant& body, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> send(HttpMethod method, const String& url, const Map<String, Variant>& params, const Variant& body, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> sendJson(HttpMethod method, const String& url, const Map<String, Variant>& params, const Json& json, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> sendJson(HttpMethod method, const String& url, const Map<String, Variant>& params, const Json& json, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> post(const String& url, const Variant& body, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> post(const String& url, const Variant& body, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> post(const String& url, const Map<String, Variant>& params, const Variant& body, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> post(const String& url, const Map<String, Variant>& params, const Variant& body, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> postJson(const String& url, const Json& json, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> postJson(const String& url, const Json& json, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
		static Ref<UrlRequest> postJson(const String& url, const Map<String, Variant>& params, const Json& json, const Function<void(UrlRequest*)>& onComplete);
		
		static Ref<UrlRequest> postJson(const String& url, const Map<String, Variant>& params, const Json& json, const Function<void(UrlRequest*)>& onComplete, const Ref<Dispatcher>& dispatcher);
		
	public:
		const String& getUrl();
		
		const String& getDownloadingFilePath();
		
		
		HttpMethod getMethod();
		
		const Memory& getRequestBody();
		
		sl_size getRequestBodySize();
		
		Map<String, Variant> getParameters();
		
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
		
		const Function<void(UrlRequest*, const void*, sl_size)>& getOnReceiveContent();
		
		const Ref<Dispatcher>& getDispatcher();
		
		sl_bool isUsingBackgroundSession();
		
		sl_bool isSelfAlive();
		
		sl_bool isStoringResponseContent();
		
		
		sl_uint64 getSentRequestBodySize();
		
		sl_uint64 getResponseContentSize();
		
		sl_uint64 getExpectedResponseContentSize();
		
		
		void cancel();
		
		sl_bool isError();
		
	protected:
		static Ref<UrlRequest> _send(const UrlRequestParam& param, const String& downloadFilePath);
		
		static Ref<UrlRequest> _create(const UrlRequestParam& param, const String& url, const String& downloadFilePath);
		
		void _init(const UrlRequestParam& param, const String& url, const String& downloadFilePath);
		
		void _removeFromMap();
		
		virtual void _cancel();
		
		void onComplete();
		
		void onError();
		
		void onResponse();
		
		void onReceiveContent(const void* data, sl_size len, const Memory& mem);
		
		void onDownloadContent(sl_uint64 size);
		
		void onUploadBody(sl_uint64 size);
		
		void _runCallback(const Function<void(UrlRequest*)>& callback);
		
		static void _onCreateError(const UrlRequestParam& param, const String& url, const String& downloadFilePath);
		
		static String _buildParameters(const Map<String, Variant>& params);
		
		static Memory _buildRequestBody(const Variant& varBody);
		
	protected:
		String m_url;
		String m_downloadFilePath;
		
		HttpMethod m_method;
		Memory m_requestBody;
		Map<String, Variant> m_parameters;
		Map<String, String> m_requestHeaders;
		Map<String, String> m_additionalRequestHeaders;
		
		MemoryQueue m_bufResponseContent;
		HttpStatus m_responseStatus;
		AtomicString m_responseMessage;
		AtomicMap<String, String> m_responseHeaders;
		
		Ptr<IUrlRequestListener> m_listener;
		Function<void(UrlRequest*)> m_onComplete;
		Function<void(UrlRequest*, const void*, sl_size)> m_onReceiveContent;
		Ref<Dispatcher> m_dispatcher;
		sl_bool m_flagUseBackgroundSession;
		sl_bool m_flagSelfAlive;
		sl_bool m_flagStoreResponseContent;
		
		sl_uint64 m_sizeBodySent;
		sl_uint64 m_sizeContentTotal;
		sl_uint64 m_sizeContentReceived;
		
		sl_bool m_flagClosed;
		sl_bool m_flagError;
		
	};

}

#endif
