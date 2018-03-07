/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/definition.h"

#if defined(SLIB_PLATFORM_IS_WIN32)

#include "slib/network/url_request.h"

#include "slib/core/platform_windows.h"

#include "slib/network/url.h"
#include "slib/core/string_buffer.h"
#include "slib/core/scoped.h"
#include "slib/core/log.h"
#include "slib/core/safe_static.h"
#include "slib/core/async.h"

#include <wininet.h>
#pragma comment(lib, "wininet.lib")

#define MAX_CONNECTION_POOL_SIZE 32
#define READ_BUFFER_SIZE 65536

namespace slib
{

	class UrlRequest_Connection : public Referable
	{
	public:
		HINTERNET hConnect;
		sl_int32 id;
		String16 address;
		sl_bool flagHttps;

	public:
		UrlRequest_Connection(HINTERNET _hConnect, sl_int32 _id, const String16& _address, sl_bool _flagHttps) : hConnect(_hConnect), id(_id), address(_address), flagHttps(_flagHttps)
		{
		}

		~UrlRequest_Connection();

	};

	class UrlRequest_Impl;

	class UrlRequest_Session
	{
	public:
		HINTERNET hInternet;
		LinkedQueue< Ref<UrlRequest_Connection> > connectionPool;
		sl_int32 lastConnectionId;
		HashMap< sl_int32, WeakRef<UrlRequest_Connection> > connections;
		sl_int32 lastTaskId;
		HashMap< sl_int32, WeakRef<UrlRequest_Impl> > requests;

	public:
		UrlRequest_Session(INTERNET_STATUS_CALLBACK callback)
		{
			lastConnectionId = 0;
			lastTaskId = 0;
			hInternet = ::InternetOpenW(L"Windows Client", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, INTERNET_FLAG_ASYNC);
			::InternetSetStatusCallbackA(hInternet, callback);
		}

		~UrlRequest_Session()
		{
			::InternetSetStatusCallbackA(hInternet, NULL);
			::InternetCloseHandle(hInternet);
		}

		Ref<UrlRequest_Connection> getConnection(const String16& url, LPCWSTR& path)
		{
			DWORD lenURL = (DWORD)(url.getLength());
			URL_COMPONENTSW comps = { 0 };
			comps.dwStructSize = sizeof(comps);
			comps.dwHostNameLength = lenURL;
			comps.dwUserNameLength = lenURL;
			comps.dwPasswordLength = lenURL;
			comps.dwUrlPathLength = lenURL;
			if (::InternetCrackUrlW((LPCWSTR)(url.getData()), lenURL, 0, &comps)) {
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
						Link< Ref<UrlRequest_Connection> >* link = connectionPool.getBack();
						while (link) {
							Ref<UrlRequest_Connection>& r = link->value;
							if (r->address.getHashCode() == url.getHashCode() && r->address == url) {
								Ref<UrlRequest_Connection> connection = link->value;
								Link< Ref<UrlRequest_Connection> >* before = link->before;
								connectionPool.removeAt(link);
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
						Ref<UrlRequest_Connection> ret = new UrlRequest_Connection(hConnect, connectionId, address, comps.nScheme == INTERNET_SCHEME_HTTPS);
						if (ret.isNotNull()) {
							connections.put(connectionId, ret);
							return ret;
						}
						::InternetCloseHandle(hConnect);
					}
				}
			}
			return sl_null;
		}

	};

	class UrlRequest_DownloadStream : public AsyncStream, public IAsyncCopyListener
	{
	public:
		WeakRef<UrlRequest_Impl> m_request;
		LinkedQueue< Ref<AsyncStreamRequest> > m_queueBuffers;
		Ref<AsyncStreamRequest> m_currentBuffer;
		DWORD m_currentReadSize;
		sl_bool m_flagCompleteRead;

	public:
		UrlRequest_DownloadStream(UrlRequest_Impl* request);

	public:
		void close() override;

		sl_bool isOpened() override;

		sl_bool read(void* data, sl_uint32 size, const Function<void(AsyncStreamResult*)>& callback, Referable* ref) override;

		sl_bool write(void* data, sl_uint32 size, const Function<void(AsyncStreamResult*)>& callback, Referable* ref) override
		{
			return sl_false;
		}

		sl_bool addTask(const Function<void()>& callback) override
		{
			return sl_false;
		}

		void onReadBuffer(UrlRequest_Impl* request, HINTERNET hRequest);

		sl_bool readBuffer(UrlRequest_Impl* request, HINTERNET hRequest, AsyncStreamRequest* buffer);

		void onAsyncCopyExit(AsyncCopy* task) override;

	};

	class UrlRequest_Impl : public UrlRequest
	{
	public:
		Ref<UrlRequest_Connection> m_connection;
		HINTERNET m_hRequest;
		sl_int32 m_taskId;
		sl_int32 m_step;

		Memory m_bufPacket;
		DWORD m_sizeReadPacket;

		AtomicRef<AsyncFile> m_downloadFile;
		AtomicRef<UrlRequest_DownloadStream> m_downloadStream;
		AtomicRef<AsyncCopy> m_downloadCopy;

		enum {
			STEP_CONNECT, STEP_SEND_REQUEST, STEP_RECEIVE_DATA, STEP_COMPLETE, STEP_ERROR
		};

	public:
		UrlRequest_Impl()
		{
			m_hRequest = NULL;
			m_step = STEP_CONNECT;
			m_sizeReadPacket = 0;
		}

		~UrlRequest_Impl()
		{
			clean();
		}

		static UrlRequest_Session* getSession()
		{
			SLIB_SAFE_STATIC(UrlRequest_Session, session, callbackStatus);
			if (SLIB_SAFE_STATIC_CHECK_FREED(session)) {
				return sl_null;
			}
			return &session;
		}

		static Ref<UrlRequest_Impl> create(const UrlRequestParam& param, const String& url)
		{
			UrlRequest_Session* session = getSession();
			if (session) {
				Memory bufPacket = Memory::create(READ_BUFFER_SIZE);
				if (bufPacket.isNull()) {
					return sl_null;
				}
				LPCWSTR path;
				Ref<UrlRequest_Connection> connection = session->getConnection(url, path);
				if (connection.isNotNull()) {
					Ref<UrlRequest_Impl> ret = new UrlRequest_Impl;
					if (ret.isNotNull()) {
						Ref<AsyncFile> fileDownload;
						if (param.downloadFilePath.isNotEmpty()) {
							Ref<File> file = File::openForWrite(param.downloadFilePath);
							if (file.isNull()) {
								return sl_null;
							}
							fileDownload = AsyncFile::create(file);
							if (fileDownload.isNull()) {
								return sl_null;
							}
						}
						sl_int32 taskId = Base::interlockedIncrement32(&(session->lastTaskId)) & 0x7FFFFFFF;
						String16 verb = HttpMethods::toString(param.method);
						DWORD flags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE;
						if (connection->flagHttps) {
							flags |= INTERNET_FLAG_SECURE;
						}
						ret->_init(param, url);
						ret->m_connection = connection;
						ret->m_downloadFile = fileDownload;
						ret->m_bufPacket = bufPacket;
						ret->m_taskId = taskId;
						session->requests.put(taskId, ret);
						HINTERNET hRequest = ::HttpOpenRequestW(connection->hConnect, (LPCWSTR)(verb.getData()), path, NULL, NULL, NULL, flags, (DWORD_PTR)taskId);
						if (hRequest) {
							ret->m_hRequest = hRequest;
							return ret;
						}
						session->requests.remove(taskId);
					}
				}
			}
			return sl_null;
		}

		void _cancel() override
		{
			clean();
		}

		void _sendAsync() override
		{
			if (m_hRequest) {
				processAsync(m_hRequest, ERROR_SUCCESS);
			}
		}

		void clean()
		{
			if (m_hRequest) {
				::InternetCloseHandle(m_hRequest);
				m_hRequest = NULL;
				UrlRequest_Session* session = getSession();
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
			UrlRequest_Session* session = getSession();
			if (!session) {
				return;
			}
			sl_int32 id = (sl_int32)(dwContext);
			switch (dwInternetStatus) {
			case INTERNET_STATUS_REQUEST_COMPLETE:
				if (!(id & 0x80000000)) {
					Ref<UrlRequest_Impl> request = session->requests.getValue(id, WeakRef<UrlRequest_Impl>::null());
					if (request.isNotNull()) {
						INTERNET_ASYNC_RESULT* result = (LPINTERNET_ASYNC_RESULT)lpvStatusInformation;
						request->processAsync(hInternet, result->dwError);
					}
				}
				break;
			case INTERNET_STATUS_REQUEST_SENT:
				if (!(id & 0x80000000)) {
					Ref<UrlRequest_Impl> request = session->requests.getValue(id, WeakRef<UrlRequest_Impl>::null());
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
							for (auto& pair : m_requestHeaders) {
								String line = String16::format("%s: %s\r\n", pair.key, pair.value);
								::HttpAddRequestHeadersA(hRequest, line.getData(), (DWORD)(line.getLength()), HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
							}
						}
						StringBuffer sb;
						{
							for (auto& pair : m_additionalRequestHeaders) {
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
								Ref<UrlRequest_DownloadStream> stream = new UrlRequest_DownloadStream(this);
								if (stream.isNotNull()) {
									m_downloadStream = stream;
									AsyncCopyParam cp;
									cp.source = stream;
									cp.target = m_downloadFile;
									cp.listener.setRef(stream);
									cp.bufferSize = READ_BUFFER_SIZE;
									m_downloadCopy = AsyncCopy::create(cp);
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
							Ref<UrlRequest_DownloadStream> stream = m_downloadStream;
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
						LogError("UrlRequest", String(buf, len));
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
			UrlRequest_Session* session = getSession();
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

		friend class UrlRequest_DownloadStream;
	};

	UrlRequest_DownloadStream::UrlRequest_DownloadStream(UrlRequest_Impl* request) : m_request(request)
	{
		m_flagCompleteRead = sl_false;
	}

	void UrlRequest_DownloadStream::close()
	{
		ObjectLocker lock(this);
		m_request.setNull();
	}

	sl_bool UrlRequest_DownloadStream::isOpened()
	{
		return m_request.isNotNull();
	}

	sl_bool UrlRequest_DownloadStream::read(void* data, sl_uint32 size, const Function<void(AsyncStreamResult*)>& callback, Referable* ref)
	{
		Ref<UrlRequest_Impl> request(m_request);
		if (request.isNotNull()) {
			Ref<AsyncStreamRequest> buf = AsyncStreamRequest::createRead(data, size, ref, callback);
			if (buf.isNotNull()) {
				ObjectLocker lock(this);
				if (m_currentBuffer.isNotNull()) {
					if (m_queueBuffers.push(buf)) {
						return sl_true;
					}
				} else {
					return readBuffer(request.get(), request->m_hRequest, buf.get());
				}
			}
		}
		return sl_false;
	}

	void UrlRequest_DownloadStream::onReadBuffer(UrlRequest_Impl* request, HINTERNET hRequest)
	{
		if (m_currentReadSize) {
			request->onDownloadContent(m_currentReadSize);
		}
		ObjectLocker lock(this);
		if (m_currentBuffer.isNotNull()) {
			m_currentBuffer->runCallback(this, m_currentReadSize, sl_false);
			m_currentBuffer.setNull();
		}
		while (m_currentBuffer.isNull()) {
			Ref<AsyncStreamRequest> buf;
			if (m_queueBuffers.pop(&buf)) {
				if (!(readBuffer(request, hRequest, buf.get()))) {
					m_currentBuffer->runCallback(this, 0, sl_true);
					close();
					return;
				}
			} else {
				break;
			}
		}
	}

	sl_bool UrlRequest_DownloadStream::readBuffer(UrlRequest_Impl* request, HINTERNET hRequest, AsyncStreamRequest* buffer)
	{
		m_currentReadSize = 0;
		if (::InternetReadFile(hRequest, buffer->data, buffer->size, &m_currentReadSize)) {
			if (m_currentReadSize) {
				request->onDownloadContent(m_currentReadSize);
				buffer->runCallback(this, m_currentReadSize, sl_false);
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

	void UrlRequest_DownloadStream::onAsyncCopyExit(AsyncCopy* task)
	{
		Ref<UrlRequest_Impl> request(m_request);
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

	UrlRequest_Connection::~UrlRequest_Connection()
	{
		::InternetCloseHandle(hConnect);
		UrlRequest_Session* session = UrlRequest_Impl::getSession();
		if (session) {
			session->connections.remove(id);
		}
	}


	Ref<UrlRequest> UrlRequest::_create(const UrlRequestParam& param, const String& url)
	{
		return Ref<UrlRequest>::from(UrlRequest_Impl::create(param, url));
	}

}

#endif
