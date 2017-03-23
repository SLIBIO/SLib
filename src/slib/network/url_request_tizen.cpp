#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_TIZEN)

#include "../../../inc/slib/network/url_request.h"
#include <curl/curl.h>

namespace slib
{

	class _UrlRequest : public UrlRequest
	{
		public:
			Ref<Thread> m_task;
			sl_reg m_taskId;


		public:
			_UrlRequest()
			{
				m_task = nullptr;
				m_taskId = -1;
			}

			~_UrlRequest()
			{
				m_task = nullptr;
			}

			void create(const UrlRequestParam& param, const String& url, const String& downloadFilePath) {
				_init(param, url, downloadFilePath);

				m_task = Thread::start(SLIB_FUNCTION_CLASS(_UrlRequest, run, this));
				m_taskId = Math::randomByTime() * 10000000;
			}



			void run() {
				CURL *curl;
				CURLcode curl_err;

				/* init the curl session */
				curl = ::curl_easy_init();

				::curl_easy_setopt(curl, CURLOPT_URL, getUrl().getData());

				// Set http method
				switch(m_method) {
				case HttpMethod::GET:
					curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
					break;
				case HttpMethod::HEAD:
					curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "HEAD");
					break;
				case HttpMethod::POST:
					curl_easy_setopt(curl, CURLOPT_POST, 1L);
					break;
				case HttpMethod::PUT:
					curl_easy_setopt(curl, CURLOPT_PUT, 1L);
					//TODO CURLOPT_INFILE and CURLOPT_INFILESIZE.
					break;
				case HttpMethod::DELETE:
					curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
					break;
				case HttpMethod::CONNECT:
					curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "CONNECT");
					break;
				case HttpMethod::OPTIONS:
					curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "OPTIONS");
					break;
				case HttpMethod::TRACE:
					curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "TRACE");
					break;
				default:
					break;
				}

				struct curl_slist *headerChunk = NULL;
				// HTTP headers and additional headers
				if(m_requestHeaders.isNotEmpty())
				{
					Pair<String, String> pair;
					Iterator< Pair<String, String> > iterator = m_requestHeaders.toIterator();
					while (iterator.next(&pair)) {
						headerChunk = curl_slist_append(headerChunk, (pair.key + ": " + pair.value).getData());
					}
				}
				if(m_additionalRequestHeaders.isNotEmpty())
				{
					Pair<String, String> pair;
					Iterator< Pair<String, String> > iterator = m_additionalRequestHeaders.toIterator();
					while (iterator.next(&pair)) {
						headerChunk = curl_slist_append(headerChunk, (pair.key + ": " + pair.value).getData());
					}
				}
				if(headerChunk != NULL) {
					::curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerChunk);
				}

				/* post data */
				::curl_easy_setopt(curl, CURLOPT_POSTFIELDS, m_requestBody.getData());
				/* set the size of the postfields data */
				::curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, m_requestBody.getSize());

				/* send all received data to _urlrequest_write_cb  */
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, _UrlRequest::_urlrequest_write_cb);

				/* pass the _UrlRequest to the callback function */
				::curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)this);

				/* getting data */
				curl_err = curl_easy_perform(curl);

				if (curl_err != CURLE_OK) {
					/* Error handling */
					//error
					onError();
					return;
				}
				//success
				onComplete();

				m_task = nullptr;
			}

			size_t dispatchReceiveContent(const void* data, size_t size)
			{
				if(m_downloadFilePath.isNotEmpty()) {
					Ref<File> file = File::openForAppend(m_downloadFilePath);
					sl_reg ret = file->write(data, size);
					file->close();
					return ret;
				}
				else {
					onReceiveContent(data, size, nullptr);
					return size;
				}
			}


			static size_t _urlrequest_write_cb(void *contents, size_t size, size_t nmemb, void *userp)
			{
				_UrlRequest *req = (_UrlRequest*)userp;
				size_t realsize = size * nmemb;
				return req->dispatchReceiveContent(contents, realsize);
			}
	};


	Ref<UrlRequest> UrlRequest::_create(const UrlRequestParam& param, const String& url, const String& downloadFilePath)
	{
		Ref<_UrlRequest> request = new _UrlRequest;
		request->create(param, url, downloadFilePath);
		return request;
	}

}

#endif
