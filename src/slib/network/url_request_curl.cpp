#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_TIZEN)

#include "slib/network/url_request.h"

#include "slib/core/file.h"
#include "slib/core/log.h"

#include <curl/curl.h>
#include <net_connection.h>
#include <stdlib.h>

namespace slib
{

	class UrlRequest_Impl : public UrlRequest
	{
	public:
		CURL* m_curl;
		sl_bool m_flagClosed;
		sl_bool m_flagProcessResponse;

	public:
		UrlRequest_Impl()
		{
			m_curl = sl_null;
			m_flagClosed = sl_false;
			m_flagProcessResponse = sl_false;
		}

		~UrlRequest_Impl()
		{
		}

	public:
		static Ref<UrlRequest_Impl> create(const UrlRequestParam& param, const String& url) {
			Ref<UrlRequest_Impl> ret = new UrlRequest_Impl;
			if (ret.isNotNull()) {
				ret->_init(param, url);
				return ret;
			}
			return sl_null;
		}

		void _cancel() override
		{
			m_flagClosed = sl_true;
		}

		void _sendSync() override
		{
			connection_h connection;
			if (::connection_create(&connection) != CONNECTION_ERROR_NONE) {
				onError();
				return;
			}

			CURL* curl = ::curl_easy_init();
			if (!curl) {
				::connection_destroy(connection);
				onError();
				return;
			}

			m_curl = curl;

			char* proxy_address;
			sl_bool flagSetProxy = sl_false;
			int conn_err = ::connection_get_proxy(connection, CONNECTION_ADDRESS_FAMILY_IPV4, &proxy_address);
			if (conn_err == CONNECTION_ERROR_NONE && proxy_address) {
				if (proxy_address[0]) {
					::curl_easy_setopt(curl, CURLOPT_PROXY, proxy_address);
					flagSetProxy = sl_true;
				}
				::free(proxy_address);
			}
			if (!flagSetProxy) {
				conn_err = ::connection_get_proxy(connection, CONNECTION_ADDRESS_FAMILY_IPV6, &proxy_address);
				if (conn_err == CONNECTION_ERROR_NONE && proxy_address) {
					if (proxy_address[0]) {
						::curl_easy_setopt(curl, CURLOPT_PROXY, proxy_address);
					}
					::free(proxy_address);
				}
			}
			::connection_set_proxy_address_changed_cb(connection, UrlRequest_Impl::callbackProxyChanged, (void*)this);

			String url = m_url;
			::curl_easy_setopt(curl, CURLOPT_URL, url.getData());

			::curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
			::curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 10L);

			// Set http method
			switch(m_method) {
			case HttpMethod::GET:
				::curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
				break;
			case HttpMethod::HEAD:
				::curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "HEAD");
				break;
			case HttpMethod::POST:
				::curl_easy_setopt(curl, CURLOPT_POST, 1L);
				break;
			case HttpMethod::PUT:
				::curl_easy_setopt(curl, CURLOPT_PUT, 1L);
				break;
			case HttpMethod::DELETE:
				::curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
				break;
			case HttpMethod::CONNECT:
				::curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "CONNECT");
				break;
			case HttpMethod::OPTIONS:
				::curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "OPTIONS");
				break;
			case HttpMethod::TRACE:
				::curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "TRACE");
				break;
			default:
				break;
			}

			curl_slist *headerChunk = sl_null;

			// HTTP headers and additional headers
			if(m_requestHeaders.isNotEmpty())
			{
				for (auto& pair : m_requestHeaders) {
					String s = pair.key + ": " + pair.value;
					headerChunk = ::curl_slist_append(headerChunk, s.getData());
				}
			}
			if(m_additionalRequestHeaders.isNotEmpty())
			{
				for (auto& pair : m_additionalRequestHeaders) {
					String s = pair.key + ": " + pair.value;
					headerChunk = ::curl_slist_append(headerChunk, s.getData());
				}
			}
			if (headerChunk) {
				::curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerChunk);
			}

			// post data
			Memory requestBody = m_requestBody;
			if (m_method == HttpMethod::POST) {
				::curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestBody.getData());
				::curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, requestBody.getSize());
			} else {
				if (requestBody.isNotEmpty()) {
					::curl_easy_setopt(curl, CURLOPT_READFUNCTION, UrlRequest_Impl::callbackRead);
					::curl_easy_setopt(curl, CURLOPT_READDATA, (void*)this);
				}
			}

			// received header
			::curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, UrlRequest_Impl::callbackHeader);
			::curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void*)this);

			// received data
			::curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, UrlRequest_Impl::callbackWrite);
			::curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)this);

			/* getting data */
			CURLcode err = ::curl_easy_perform(curl);

			processResponse();

			if (err == CURLE_OK) {
				onComplete();
			} else {
				String strError = ::curl_easy_strerror(err);
				LogError("UrlRequest", "Error: %s", strError);
				m_lastErrorMessage = strError;
				onError();
			}

			if (headerChunk) {
				::curl_slist_free_all(headerChunk);
			}

			::curl_easy_cleanup(curl);
			::connection_destroy(connection);

		}

		void processResponse()
		{
			if (m_flagClosed) {
				return;
			}
			if (m_flagProcessResponse) {
				return;
			}
			m_flagProcessResponse = sl_true;

			long response_code = 0;
			::curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &response_code);
			m_responseStatus = (HttpStatus)((int)response_code);

			String strLength = m_responseHeaders.getValue("Content-Length", String::null());
			if (strLength.isNotEmpty()) {
				m_sizeContentTotal = strLength.parseUint64();
			}

			onResponse();
		}

		sl_size onRead(void* data, sl_size size)
		{
			if (m_flagClosed) {
				return 0;
			}
			Memory body = m_requestBody;
			sl_size total = body.getSize();
			if (m_sizeBodySent + size > total) {
				size = total - m_sizeBodySent;
			}
			if (size > 0) {
				body.read(m_sizeBodySent, size, data);
				m_sizeBodySent += size;
				onUploadBody(size);
			}
			return size;
		}

		static size_t callbackRead(void *contents, size_t size, size_t nmemb, void *user_data)
		{
			UrlRequest_Impl* req = (UrlRequest_Impl*)user_data;
			size_t n = size * nmemb;
			if (n) {
				return (size_t)(req->onRead(contents, (sl_size)n));
			}
			return 0;
		}

		sl_size onWrite(const void* data, sl_size size)
		{
			if (m_flagClosed) {
				return 0;
			}
			processResponse();
			if (m_downloadFilePath.isNotEmpty()) {
				Ref<File> file = File::openForAppend(m_downloadFilePath);
				if (file.isNotNull()) {
					sl_reg ret = file->write(data, size);
					if (ret > 0) {
						size = ret;
					} else {
						size = 0;
					}
				}
				onDownloadContent(size);
			} else {
				onReceiveContent(data, size, sl_null);
			}
			return size;
		}

		static size_t callbackWrite(void *contents, size_t size, size_t nmemb, void *user_data)
		{
			UrlRequest_Impl* req = (UrlRequest_Impl*)user_data;
			size_t n = size * nmemb;
			if (n) {
				return (size_t)(req->onWrite(contents, (sl_size)n));
			}
			return 0;
		}

		sl_bool onHeader(char* header, sl_size size)
		{
			if (m_flagClosed) {
				return sl_false;
			}
			if (size >= 2 && header[size-2] == '\r' && header[size-1] == '\n') {
				size -= 2;
			}
			if (size > 0) {
				sl_size index1 = size;
				sl_size index2 = size;
				for (sl_size i = 0; i < size; i++) {
					if (header[i] == ':') {
						index1 = i;
						if (i + 1 < size && header[i + 1] == ' ') {
							index2 = i + 2;
						} else {
							index2 = i + 1;
						}
						break;
					}
				}
				String name(header, index1);
				String value(header + index2, size - index2);
				if (value.isEmpty() && name.startsWith("HTTP")) {
					m_responseHeaders.setNull();
				} else {
					HttpHeaderMap headers = m_responseHeaders;
					if (headers.isNull()) {
						headers.init();
						m_responseHeaders = headers;
					}
					headers.add(name, value);
				}
			}
			return sl_true;
		}

		static size_t callbackHeader(char *buffer, size_t size, size_t nitems, void *user_data)
		{
			UrlRequest_Impl* req = (UrlRequest_Impl*)user_data;
			size_t n = size * nitems;
			if (n) {
				if (req->onHeader(buffer, (sl_size)n)) {
					return n;
				}
			}
			return 0;
		}

		void onProxyChanged(const char *ipv4_address, const char *ipv6_address)
		{
			if (m_flagClosed) {
				return;
			}
			if (ipv4_address) {
				::curl_easy_setopt(m_curl, CURLOPT_PROXY, ipv4_address);
			} else if (ipv6_address) {
				::curl_easy_setopt(m_curl, CURLOPT_PROXY, ipv6_address);
			}
		}

		static void callbackProxyChanged(const char *ipv4_address, const char *ipv6_address, void *user_data)
		{
			UrlRequest_Impl* req = (UrlRequest_Impl*)user_data;
			req->onProxyChanged(ipv4_address, ipv6_address);
		}

	};


	Ref<UrlRequest> UrlRequest::_create(const UrlRequestParam& param, const String& url)
	{
		return Ref<UrlRequest>::from(UrlRequest_Impl::create(param, url));
	}

}

#endif
