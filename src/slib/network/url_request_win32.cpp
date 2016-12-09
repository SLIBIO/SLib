#include "../../../inc/slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "../../../inc/slib/network/url_request.h"

#include "../../../inc/slib/network/url.h"
#include "../../../inc/slib/core/scoped.h"
#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/safe_static.h"
#include "../../../inc/slib/core/platform_windows.h"

#include <wininet.h>
#pragma comment(lib, "wininet.lib")

#define MAX_CONNECTION_POOL_SIZE 32
#define READ_BUFFER_SIZE 65536

SLIB_NETWORK_NAMESPACE_BEGIN

class _UrlRequestConnection : public Referable
{
public:
	HINTERNET hConnect;
	sl_int32 id;
	String16 address;
	sl_bool flagHttps;

public:
	_UrlRequestConnection(HINTERNET _hConnect, sl_int32 _id, const String16& _address, sl_bool _flagHttps) : hConnect(_hConnect), id(_id), address(_address), flagHttps(_flagHttps)
	{
	}

	~_UrlRequestConnection();

};

class _UrlRequest;

class _UrlRequestSession
{
public:
	HINTERNET hInternet;
	Queue< Ref<_UrlRequestConnection> > connectionPool;
	sl_int32 lastConnectionId;
	HashMap< sl_int32, WeakRef<_UrlRequestConnection> > connections;
	sl_int32 lastTaskId;
	HashMap< sl_int32, WeakRef<_UrlRequest> > requests;

public:
	_UrlRequestSession(INTERNET_STATUS_CALLBACK callback)
	{
		lastConnectionId = 0;
		lastTaskId = 0;
		hInternet = ::InternetOpenW(L"Windows Client", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_ASYNC);
		::InternetSetStatusCallbackA(hInternet, callback);
	}

	~_UrlRequestSession()
	{
		::InternetSetStatusCallbackA(hInternet, NULL);
		::InternetCloseHandle(hInternet);
	}

	Ref<_UrlRequestConnection> getConnection(const String16& url, LPCWSTR& path)
	{
		DWORD lenURL = (DWORD)(url.getLength());
		URL_COMPONENTSW comps = { 0 };
		comps.dwStructSize = sizeof(comps);
		comps.dwHostNameLength = lenURL;
		comps.dwUserNameLength = lenURL;
		comps.dwPasswordLength = lenURL;
		comps.dwUrlPathLength = lenURL;
		if (::InternetCrackUrlW(url.getData(), lenURL, 0, &comps)) {
			if (comps.lpszHostName && (comps.nScheme == INTERNET_SCHEME_HTTP || comps.nScheme == INTERNET_SCHEME_HTTPS)) {
				if (comps.lpszUrlPath && comps.lpszUrlPath[0]) {
					path = comps.lpszUrlPath;
					*(comps.lpszUrlPath - 1) = 0;
				} else {
					path = L"/";
				}
				String16 address = url.getData();
				{
					ObjectLocker lock(&connectionPool);
					Link< Ref<_UrlRequestConnection> >* link = connectionPool.getBack();
					while (link) {
						Ref<_UrlRequestConnection>& r = link->value;
						if (r->address.getHashCode() == url.getHashCode() && r->address == url) {
							Ref<_UrlRequestConnection> connection = link->value;
							Link< Ref<_UrlRequestConnection> >* before = link->before;
							connectionPool.removeItem_NoLock(link);
							return connection;
						}
						link = link->before;
					}
				}
				comps.lpszHostName[comps.dwHostNameLength] = 0;
				if (comps.lpszUserName) {
					comps.lpszUserName[comps.dwUserNameLength] = 0;
				}
				if (comps.lpszPassword) {
					comps.lpszPassword[comps.dwPasswordLength] = 0;
				}
				sl_int32 connectionId = Base::interlockedIncrement32(&(lastConnectionId)) & 0x7FFFFFFF;
				HINTERNET hConnect = ::InternetConnectW(hInternet, comps.lpszHostName, comps.nPort, comps.lpszUserName, comps.lpszPassword, INTERNET_SERVICE_HTTP, 0, (DWORD_PTR)(connectionId | 0x80000000));
				if (hConnect) {
					Ref<_UrlRequestConnection> ret = new _UrlRequestConnection(hConnect, connectionId, address, comps.nScheme == INTERNET_SCHEME_HTTPS);
					if (ret.isNotNull()) {
						connections.put(connectionId, ret);
						return ret;
					}
					::InternetCloseHandle(hConnect);
				}
			}
		}
		return Ref<_UrlRequestConnection>::null();
	}

};

class _UrlRequest;

class _UrlRequestDownloadStream : public AsyncStream, public IAsyncCopyListener
{
public:
	WeakRef<_UrlRequest> m_request;
	Queue< Ref<AsyncStreamRequest> > m_queueBuffers;
	Ref<AsyncStreamRequest> m_currentBuffer;
	DWORD m_currentReadSize;
	sl_bool m_flagCompleteRead;

public:
	_UrlRequestDownloadStream(_UrlRequest* request);

public:
	// override
	void close();

	// override
	sl_bool isOpened();

	// override
	sl_bool read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, const Referable* ref);

	// override
	sl_bool write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, const Referable* ref)
	{
		return sl_false;
	}

	// override
	sl_bool addTask(const Callback& callback)
	{
		return sl_false;
	}

	void onReadBuffer(_UrlRequest* request, HINTERNET hRequest);

	sl_bool readBuffer(_UrlRequest* request, HINTERNET hRequest, AsyncStreamRequest* buffer);

	// override
	void onAsyncCopyExit(AsyncCopy* task);

};

class _UrlRequest : public UrlRequest
{
public:
	Ref<_UrlRequestConnection> m_connection;
	HINTERNET m_hRequest;
	sl_int32 m_taskId;
	sl_int32 m_step;

	Memory m_bufPacket;
	DWORD m_sizeReadPacket;

	SafeRef<AsyncFile> m_downloadFile;
	SafeRef<_UrlRequestDownloadStream> m_downloadStream;
	SafeRef<AsyncCopy> m_downloadCopy;

	enum {
		STEP_CONNECT, STEP_SEND_REQUEST, STEP_RECEIVE_DATA, STEP_COMPLETE, STEP_ERROR
	};

public:
	_UrlRequest()
	{
		m_hRequest = NULL;
		m_step = STEP_CONNECT;
		m_sizeReadPacket = 0;
	}

	~_UrlRequest()
	{
		clean();
	}

	static _UrlRequestSession* getSession()
	{
		SLIB_SAFE_STATIC(_UrlRequestSession, session, callbackStatus);
		if (SLIB_SAFE_STATIC_CHECK_FREED(session)) {
			return sl_null;
		}
		return &session;
	}

	static Ref<_UrlRequest> create(const UrlRequestParam& param, const String& url, const String& downloadFilePath)
	{
		_UrlRequestSession* session = getSession();
		if (session) {
			Memory bufPacket = Memory::create(READ_BUFFER_SIZE);
			if (bufPacket.isNull()) {
				return Ref<_UrlRequest>::null();
			}
			LPCWSTR path;
			Ref<_UrlRequestConnection> connection = session->getConnection(url, path);
			if (connection.isNotNull()) {
				Ref<_UrlRequest> ret = new _UrlRequest;
				if (ret.isNotNull()) {
					Ref<AsyncFile> fileDownload;
					if (downloadFilePath.isNotEmpty()) {
						Ref<File> file = File::openForWrite(downloadFilePath);
						if (file.isNull()) {
							return Ref<_UrlRequest>::null();
						}
						fileDownload = AsyncFile::create(file);
						if (fileDownload.isNull()) {
							return Ref<_UrlRequest>::null();
						}
					}
					sl_int32 taskId = Base::interlockedIncrement32(&(session->lastTaskId)) & 0x7FFFFFFF;
					String16 verb = HttpMethods::toString(param.method);
					DWORD flags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE;
					if (connection->flagHttps) {
						flags |= INTERNET_FLAG_SECURE;
					}
					ret->_init(param, url, downloadFilePath);
					ret->m_connection = connection;
					ret->m_downloadFile = fileDownload;
					ret->m_bufPacket = bufPacket;
					ret->m_taskId = taskId;
					session->requests.put(taskId, ret);
					HINTERNET hRequest = ::HttpOpenRequestW(connection->hConnect, verb.getData(), path, NULL, NULL, NULL, flags, (DWORD_PTR)taskId);
					if (hRequest) {
						ret->m_hRequest = hRequest;
						ret->processAsync(hRequest, ERROR_SUCCESS);
						return ret;
					}
					session->requests.remove(taskId);
				}
			}
		}
		return Ref<_UrlRequest>::null();
	}

	// override
	void _cancel()
	{
		clean();
	}

	void clean()
	{
		if (m_hRequest) {
			::InternetCloseHandle(m_hRequest);
			m_hRequest = NULL;
			_UrlRequestSession* session = getSession();
			if (session) {
				session->requests.remove(m_taskId);
			}
			m_connection.setNull();
		}
		m_downloadCopy.setNull();
		m_downloadStream.setNull();
		m_downloadFile.setNull();
	}

	static void CALLBACK callbackStatus(HINTERNET hInternet, DWORD_PTR dwContext, DWORD dwInternetStatus, LPVOID lpvStatusInformation, DWORD dwStatusInformationLength)
	{
		if (dwContext == 0) {
			return;
		}
		_UrlRequestSession* session = getSession();
		if (!session) {
			return;
		}
		sl_int32 id = (sl_int32)(dwContext);
		switch (dwInternetStatus) {
		case INTERNET_STATUS_REQUEST_COMPLETE:
			if (!(id & 0x80000000)) {
				Ref<_UrlRequest> request = session->requests.getValue(id, WeakRef<_UrlRequest>::null());
				if (request.isNotNull()) {
					INTERNET_ASYNC_RESULT* result = (LPINTERNET_ASYNC_RESULT)lpvStatusInformation;
					request->processAsync(hInternet, result->dwError);
				}
			}
			break;
		case INTERNET_STATUS_REQUEST_SENT:
			if (!(id & 0x80000000)) {
				Ref<_UrlRequest> request = session->requests.getValue(id, WeakRef<_UrlRequest>::null());
				if (request.isNotNull()) {
					DWORD dwSize = *((DWORD*)lpvStatusInformation);
					request->m_sizeBodySent += dwSize;
					request->onUploadBody(dwSize);
				}
			}
			break;
		}
	}
	
	void processAsync(HINTERNET hRequest, DWORD dwError)
	{
		while (dwError == ERROR_SUCCESS && m_step != STEP_COMPLETE) {
			switch (m_step) {
			case STEP_CONNECT:
				{
					{
						Iterator< Pair<String, String> > iterator = m_requestHeaders.iterator();
						Pair<String, String> pair;
						if (iterator.next(&pair)) {
							String line = String16::format("%s: %s\r\n", pair.key, pair.value);
							::HttpAddRequestHeadersA(hRequest, line.getData(), (DWORD)(line.getLength()), HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
						}
					}
					StringBuffer sb;
					{
						Iterator< Pair<String, String> > iterator = m_additionalRequestHeaders.iterator();
						Pair<String, String> pair;
						while (iterator.next(&pair)) {
							String str = pair.key;
							sb.addStatic(str.getData(), str.getLength());
							sb.addStatic(": ", 2);
							str = pair.value;
							sb.addStatic(str.getData(), str.getLength());
							sb.addStatic("\r\n", 2);
						}
					}
					String s = sb.merge();
					Memory body = m_requestBody;

					m_step = STEP_SEND_REQUEST;
					if (!(::HttpSendRequestA(hRequest, s.getData(), (DWORD)(s.getLength()), body.getData(), (DWORD)(body.getSize())))) {
						dwError = ::GetLastError();
					}
				}
				break;
			case STEP_SEND_REQUEST:
				{
					DWORD status;
					DWORD len = 4;
					::HttpQueryInfoA(hRequest, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &status, &len, NULL);
					m_responseStatus = (HttpStatus)status;

					m_sizeContentTotal = (sl_uint64)-1;

					len = 0;
					::HttpQueryInfoA(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, NULL, &len, NULL);

					SLIB_SCOPED_BUFFER(char, 1024, bufHeaders, len);
					::HttpQueryInfoA(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, bufHeaders, &len, NULL);
					if (len) {
						HttpResponse response;
						sl_reg iRet = response.parseResponsePacket(bufHeaders, len);
						if (iRet > 0) {
							m_responseMessage = response.getResponseMessage();
							m_responseHeaders = response.getResponseHeaders();
							String strLength = response.getResponseHeader("Content-Length");
							if (strLength.isNotEmpty()) {
								m_sizeContentTotal = strLength.parseUint64();
							}
						}
					}

					onResponse();

					if (m_sizeContentTotal == 0) {
						processComplete();
					} else {
						m_step = STEP_RECEIVE_DATA;
						if (m_downloadFilePath.isNotEmpty()) {
							Ref<_UrlRequestDownloadStream> stream = new _UrlRequestDownloadStream(this);
							if (stream.isNotNull()) {
								m_downloadStream = stream;
								m_downloadCopy = AsyncCopy::create(stream, m_downloadFile, -1, stream, READ_BUFFER_SIZE);
								if (m_downloadCopy.isNotNull()) {
									return;
								}
							}
							processError();
							return;
						} else {
							dwError = receiveData(hRequest);
						}
					}
				}
				break;
			case STEP_RECEIVE_DATA:
				{
					if (m_downloadFilePath.isNotNull()) {
						Ref<_UrlRequestDownloadStream> stream = m_downloadStream;
						if (stream.isNotNull()) {
							stream->onReadBuffer(this, hRequest);
						}
						return;
					} else {
						if (m_sizeReadPacket) {
							onReceiveContent(m_bufPacket.getData(), m_sizeReadPacket, Memory::null());
						}
						dwError = receiveData(hRequest);
					}
				}
				break;
			}
		}
		if (dwError != ERROR_SUCCESS && dwError != ERROR_IO_PENDING) {
			if (dwError == ERROR_INTERNET_EXTENDED_ERROR) {
				DWORD err = dwError;
				WCHAR buf[512];
				DWORD len = 512;
				if (::InternetGetLastResponseInfoW(&err, buf, &len)) {
					SLIB_LOG_ERROR("UrlRequest", String(buf, len));
				}
			}
			processError();
		}
	}

	DWORD receiveData(HINTERNET hRequest)
	{
		void* buf = m_bufPacket.getData();
		DWORD sizeBuf = (DWORD)(m_bufPacket.getSize());
		while (1) {
			m_sizeReadPacket = 0;
			if (::InternetReadFile(hRequest, buf, sizeBuf, &m_sizeReadPacket)) {
				if (m_sizeReadPacket) {
					onReceiveContent(buf, m_sizeReadPacket, Memory::null());
				} else {
					processComplete();
					return ERROR_SUCCESS;
				}
			} else {
				DWORD dwError = ::GetLastError();
				return dwError;
			}
		}
		return ERROR_SUCCESS;
	}

	void processComplete()
	{
		ObjectLocker lock(this);
		if (m_step == STEP_COMPLETE || m_step == STEP_ERROR) {
			return;
		}
		m_step = STEP_COMPLETE;
		onComplete();
		_UrlRequestSession* session = getSession();
		if (session) {
			session->connectionPool.push(m_connection, MAX_CONNECTION_POOL_SIZE);
		}
		clean();
	}

	void processError()
	{
		ObjectLocker lock(this);
		if (m_step == STEP_COMPLETE || m_step == STEP_ERROR) {
			return;
		}
		m_step = STEP_ERROR;
		onError();
		clean();
	}

	friend class _UrlRequestDownloadStream;
};

_UrlRequestDownloadStream::_UrlRequestDownloadStream(_UrlRequest* request) : m_request(request)
{
	m_flagCompleteRead = sl_false;
}

void _UrlRequestDownloadStream::close()
{
	ObjectLocker lock(this);
	m_request.setNull();
}

sl_bool _UrlRequestDownloadStream::isOpened()
{
	return m_request.isNotNull();
}

sl_bool _UrlRequestDownloadStream::read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, const Referable* ref)
{
	Ref<_UrlRequest> request(m_request);
	if (request.isNotNull()) {
		Ref<AsyncStreamRequest> buf = AsyncStreamRequest::createRead(data, size, ref, listener);
		if (buf.isNotNull()) {
			ObjectLocker lock(this);
			if (m_currentBuffer.isNotNull()) {
				if (m_queueBuffers.push(buf)) {
					return sl_true;
				}
			} else {
				return readBuffer(request.ptr, request->m_hRequest, buf.ptr);
			}
		}
	}
	return sl_false;
}

void _UrlRequestDownloadStream::onReadBuffer(_UrlRequest* request, HINTERNET hRequest)
{
	if (m_currentReadSize) {
		request->onDownloadContent(m_currentReadSize);
	}
	ObjectLocker lock(this);
	if (m_currentBuffer.isNotNull()) {
		PtrLocker<IAsyncStreamListener> listener(m_currentBuffer->listener);
		if (listener.isNotNull()) {
			listener->onRead(this, m_currentBuffer->data, m_currentReadSize, m_currentBuffer->refData.ptr, sl_false);
		}
		m_currentBuffer.setNull();
	}
	while (m_currentBuffer.isNull()) {
		Ref<AsyncStreamRequest> buf;
		if (m_queueBuffers.pop(&buf)) {
			if (!(readBuffer(request, hRequest, buf.ptr))) {
				PtrLocker<IAsyncStreamListener> listener(buf->listener);
				if (listener.isNotNull()) {
					listener->onRead(this, buf->data, 0, buf->refData.ptr, sl_true);
				}
				close();
				return;
			}
		} else {
			break;
		}
	}
}

sl_bool _UrlRequestDownloadStream::readBuffer(_UrlRequest* request, HINTERNET hRequest, AsyncStreamRequest* buffer)
{
	m_currentReadSize = 0;
	if (::InternetReadFile(hRequest, buffer->data, buffer->size, &m_currentReadSize)) {
		if (m_currentReadSize) {
			request->onDownloadContent(m_currentReadSize);
			PtrLocker<IAsyncStreamListener> listener(buffer->listener);
			if (listener.isNotNull()) {
				listener->onRead(this, buffer->data, m_currentReadSize, buffer->refData.ptr, sl_false);
			}
			return sl_true;
		} else {
			m_flagCompleteRead = sl_true;
			return sl_false;
		}
	} else {
		DWORD dwError = ::GetLastError();
		if (dwError == ERROR_IO_PENDING) {
			m_currentBuffer = buffer;
			return sl_true;
		}
		return sl_false;
	}
}

void _UrlRequestDownloadStream::onAsyncCopyExit(AsyncCopy* task)
{
	Ref<_UrlRequest> request(m_request);
	if (request.isNotNull()) {
		if (task->isWritingErrorOccured()) {
			request->processError();
			return;
		}
		if (m_flagCompleteRead) {
			request->processComplete();
			return;
		}
		request->processError();
	}
}

_UrlRequestConnection::~_UrlRequestConnection()
{
	::InternetCloseHandle(hConnect);
	_UrlRequestSession* session = _UrlRequest::getSession();
	if (session) {
		session->connections.remove(id);
	}
}


Ref<UrlRequest> UrlRequest::_create(const UrlRequestParam& param, const String& url, const String& downloadFilePath)
{
	return Ref<UrlRequest>::from(_UrlRequest::create(param, url, downloadFilePath));
}

SLIB_NETWORK_NAMESPACE_END

#endif
