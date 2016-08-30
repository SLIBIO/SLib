#include "network_async_config.h"

#if defined(SLIB_PLATFORM_IS_WINDOWS)

#include <winsock2.h>
#include <mswsock.h>
#pragma comment(lib, "mswsock.lib")

#include "../../../inc/slib/network/async.h"
#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/platform_windows.h"

SLIB_NETWORK_NAMESPACE_BEGIN

class _Win32AsyncTcpSocketInstance : public AsyncTcpSocketInstance
{
public:
	WSAOVERLAPPED m_overlappedRead;
	WSABUF m_bufRead;
	DWORD m_flagsRead;
	SafeRef<AsyncStreamRequest> m_requestReading;

	WSAOVERLAPPED m_overlappedWrite;
	WSABUF m_bufWrite;
	SafeRef<AsyncStreamRequest> m_requestWriting;

	WSAOVERLAPPED m_overlappedConnect;
	LPFN_CONNECTEX m_funcConnectEx;

public:
	_Win32AsyncTcpSocketInstance()
	{
	}

	~_Win32AsyncTcpSocketInstance()
	{
		close();
	}

public:
	static Ref<_Win32AsyncTcpSocketInstance> create(const Ref<Socket>& socket)
	{
		Ref<_Win32AsyncTcpSocketInstance> ret;
		if (socket.isNotNull()) {
			sl_file handle = (sl_file)(socket->getHandle());
			if (handle != SLIB_FILE_INVALID_HANDLE) {
				ret = new _Win32AsyncTcpSocketInstance();
				if (ret.isNotNull()) {
					ret->m_socket = socket;
					ret->setHandle(handle);
					ret->initializeConnectEx();
					return ret;
				}
			}
		}
		return ret;
	}

	void initializeConnectEx()
	{
		sl_file handle = getHandle();
		m_funcConnectEx = sl_null;
		// ConnectEx
		{
			GUID Guid = WSAID_CONNECTEX;
			DWORD dwBytes = 0;
			int ret = WSAIoctl(
				(SOCKET)(handle), SIO_GET_EXTENSION_FUNCTION_POINTER
				, &Guid, sizeof (Guid)
				, &m_funcConnectEx, sizeof(m_funcConnectEx)
				, &dwBytes, NULL, NULL);
			if (ret == SOCKET_ERROR) {
				m_funcConnectEx = sl_null;
			}
		}
		m_flagSupportingConnect = m_funcConnectEx != sl_null;
	}

	void close()
	{
		setHandle(SLIB_FILE_INVALID_HANDLE);			
		m_socket.setNull();
	}

	void onOrder()
	{
		Ref<Socket> socket = m_socket;
		if (socket.isNull()) {
			return;
		}
		sl_file handle = getHandle();
		if (handle == SLIB_FILE_INVALID_HANDLE) {
			return;
		}
		if (m_requestReading.isNull()) {
			Ref<AsyncStreamRequest> req;
			if (popReadRequest(req)) {
				if (req.isNotNull()) {
					Base::zeroMemory(&m_overlappedRead, sizeof(m_overlappedRead));
					m_bufRead.buf = (CHAR*)(req->data);
					m_bufRead.len = req->size;
					m_flagsRead = 0;
					DWORD dwRead = 0;
					int ret = ::WSARecv((SOCKET)handle, &m_bufRead, 1, &dwRead, &m_flagsRead, &m_overlappedRead, NULL);
					if (ret == 0) {
						m_requestReading = req;
						EventDesc desc;
						desc.pOverlapped = &m_overlappedRead;
						onEvent(&desc);
					} else {
						DWORD dwErr = ::WSAGetLastError();
						if (dwErr == WSA_IO_PENDING) {
							m_requestReading = req;
						} else {
							_onReceive(req.ptr, 0, sl_true);
						}
					}
				}
			}
		}
		if (m_requestWriting.isNull()) {
			Ref<AsyncStreamRequest> req;
			if (popWriteRequest(req)) {
				if (req.isNotNull()) {
					Base::zeroMemory(&m_overlappedWrite, sizeof(m_overlappedWrite));
					m_bufWrite.buf = (CHAR*)(req->data);
					m_bufWrite.len = req->size;
					DWORD dwWrite = 0;
					int ret = ::WSASend((SOCKET)handle, &m_bufWrite, 1, &dwWrite, 0, &m_overlappedWrite, NULL);
					if (ret == 0) {
						m_requestWriting = req;
						EventDesc desc;
						desc.pOverlapped = &m_overlappedWrite;
						onEvent(&desc);
					} else {
						int dwErr = ::WSAGetLastError();
						if (dwErr == WSA_IO_PENDING) {
							m_requestWriting = req;
						} else {
							_onSend(req.ptr, 0, sl_true);
						}
					}
				}
			}
		}
		if (m_flagRequestConnect) {
			m_flagRequestConnect = sl_false;
			if (m_funcConnectEx) {
				sockaddr_storage addr;
				sl_uint32 lenAddr = m_addressRequestConnect.getSystemSocketAddress(&addr);
				if (lenAddr) {
					Base::resetMemory(&m_overlappedConnect, 0, sizeof(m_overlappedConnect));
					BOOL ret = m_funcConnectEx((SOCKET)handle, (sockaddr*)&addr, lenAddr, NULL, 0, NULL, &m_overlappedConnect);
					if (ret) {
						_onConnect(sl_true);
					} else {
						int err = ::WSAGetLastError();
						if (err == WSAEINVAL) {
							// ConnectEx requires the socket to be 'initially bound'
							sockaddr_storage saBind;
							SocketAddress aBind;
							aBind.port = 0;
							if (socket->isIPv6()) {
								aBind.ip = IPv6Address::zero();
							} else {
								aBind.ip = IPv4Address::zero();
							}
							sl_uint32 nSaBind = aBind.getSystemSocketAddress(&saBind);
							::bind(handle, (SOCKADDR*)&saBind, nSaBind);
							BOOL ret = m_funcConnectEx((SOCKET)handle, (sockaddr*)&addr, lenAddr, NULL, 0, NULL, &m_overlappedConnect);
							if (ret) {
								_onConnect(sl_true);
							} else {
								int err = ::WSAGetLastError();
								if (err != ERROR_IO_PENDING) {
									_onConnect(sl_true);
								}
							}
						} else if (err != ERROR_IO_PENDING) {
							_onConnect(sl_true);
						}
					}
				}
			}
		}
	}

	void onEvent(EventDesc* pev)
	{
		sl_file handle = getHandle();
		if (handle == SLIB_FILE_INVALID_HANDLE) {
			return;
		}
		OVERLAPPED* pOverlapped = (OVERLAPPED*)(pev->pOverlapped);
		DWORD dwSize = 0;
		DWORD dwFlags = 0;
		sl_bool flagError = sl_false;
		if (!::WSAGetOverlappedResult((SOCKET)handle, pOverlapped, &dwSize, FALSE, &dwFlags)) {
			int err = ::WSAGetLastError();
			if (err == WSA_IO_INCOMPLETE) {
				return;
			}
			flagError = sl_true;
		}
		if (pOverlapped == &m_overlappedRead) {
			if (dwSize == 0) {
				//flagError = sl_true;
			}
			Ref<AsyncStreamRequest> req = m_requestReading;
			m_requestReading.setNull();
			if (req.isNotNull()) {
				_onReceive(req.ptr, dwSize, flagError);
			}
		} else if (pOverlapped == &m_overlappedWrite) {
			if (dwSize == 0) {
				flagError = sl_true;
			}
			Ref<AsyncStreamRequest> req = m_requestWriting;
			m_requestWriting.setNull();
			if (req.isNotNull()) {
				_onSend(req.ptr, dwSize, flagError);
			}
		} else if (pOverlapped == &m_overlappedConnect) {
			if (flagError) {
				_onConnect(sl_true);
			} else {
				_onConnect(sl_false);
			}
		}
		requestOrder();
	}
};

Ref<AsyncTcpSocket> AsyncTcpSocket::create(const Ref<Socket>& socket, const Ref<AsyncIoLoop>& loop)
{
	Ref<_Win32AsyncTcpSocketInstance> ret = _Win32AsyncTcpSocketInstance::create(socket);
	return AsyncTcpSocket::create(ret.ptr, loop);
}


class _Win32AsyncTcpServerInstance : public AsyncTcpServerInstance
{
public:
	sl_bool m_flagAccepting;

	WSAOVERLAPPED m_overlapped;
	char m_bufferAccept[2 * (sizeof(SOCKADDR_IN)+16)];
	SafeRef<Socket> m_socketAccept;

	LPFN_ACCEPTEX m_funcAcceptEx;
	LPFN_GETACCEPTEXSOCKADDRS m_funcGetAcceptExSockaddrs;

public:
	_Win32AsyncTcpServerInstance()
	{
		m_flagAccepting = sl_false;
	}

	~_Win32AsyncTcpServerInstance()
	{
		close();
	}

public:
	static Ref<_Win32AsyncTcpServerInstance> create(const Ref<Socket>& socket, const Ptr<IAsyncTcpServerListener>& listener)
	{
		if (socket.isNotNull()) {
			sl_file handle = (sl_file)(socket->getHandle());
			if (handle != SLIB_FILE_INVALID_HANDLE) {
				Ref<_Win32AsyncTcpServerInstance> ret = new _Win32AsyncTcpServerInstance();
				if (ret.isNotNull()) {
					ret->m_socket = socket;
					ret->setHandle(handle);
					if (ret->initialize()) {
						ret->m_listener = listener;
						return ret;
					}
				}
			}
		}
		return Ref<_Win32AsyncTcpServerInstance>::null();
	}

	sl_bool initialize()
	{
		sl_file handle = getHandle();
		m_funcAcceptEx = sl_null;
		m_funcGetAcceptExSockaddrs = sl_null;
		// AcceptEx
		{
			GUID Guid = WSAID_ACCEPTEX;
			DWORD dwBytes = 0;
			int ret = WSAIoctl(
				(SOCKET)(handle), SIO_GET_EXTENSION_FUNCTION_POINTER
				, &Guid, sizeof (Guid)
				, &m_funcAcceptEx, sizeof(m_funcAcceptEx)
				, &dwBytes, NULL, NULL);
			if (ret == SOCKET_ERROR) {
				m_funcAcceptEx = sl_null;
				LOG_ERROR("Get AcceptEx extension error");
			}
		}
		// GetAcceptExSockaddrs
		{
			GUID Guid = WSAID_GETACCEPTEXSOCKADDRS;
			DWORD dwBytes = 0;
			int ret = WSAIoctl(
				(SOCKET)(handle), SIO_GET_EXTENSION_FUNCTION_POINTER
				, &Guid, sizeof (Guid)
				, &m_funcGetAcceptExSockaddrs, sizeof(m_funcGetAcceptExSockaddrs)
				, &dwBytes, NULL, NULL);
			if (ret == SOCKET_ERROR) {
				m_funcGetAcceptExSockaddrs = sl_null;
				LOG_ERROR("Get GetAcceptExSockaddrs extension error");
			}
		}
		return m_funcAcceptEx != sl_null && m_funcGetAcceptExSockaddrs != sl_null;
	}

	void close()
	{
		AsyncTcpServerInstance::close();
		m_socket.setNull();
		setHandle(SLIB_FILE_INVALID_HANDLE);
	}

	void onOrder()
	{
		if (m_flagAccepting) {
			return;
		}
		sl_file handle = getHandle();
		if (handle == SLIB_FILE_INVALID_HANDLE) {
			return;
		}
		while (Thread::isNotStoppingCurrent()) {
			Ref<Socket> socket = m_socket;
			if (socket.isNull()) {
				return;
			}
			if (!(socket->isOpened())) {
				return;
			}
			Ref<Socket> socketAccept = Socket::open(socket->getType());
			if (socketAccept.isNotNull()) {
				m_socketAccept = socketAccept;
				Base::zeroMemory(&m_overlapped, sizeof(WSAOVERLAPPED));
				DWORD dwSize = 0;
				BOOL ret = m_funcAcceptEx(
					(SOCKET)(handle), (SOCKET)(socketAccept->getHandle())
					, m_bufferAccept, 0, sizeof(SOCKADDR_IN)+16, sizeof(SOCKADDR_IN)+16, &dwSize
					, &m_overlapped);
				if (ret) {
					processAccept(sl_false);
				} else {
					int err = ::WSAGetLastError();
					if (err = ERROR_IO_PENDING) {
						m_flagAccepting = sl_true;
					} else {
						processAccept(sl_true);
						requestOrder();
					}
					break;
				}
			} else {
				LOG_ERROR("Failed to create accept socket");
				processAccept(sl_true);
				break;
			}
		}
	}

	void onEvent(EventDesc* pev)
	{
		sl_file handle = getHandle();
		if (handle == SLIB_FILE_INVALID_HANDLE) {
			return;
		}
		OVERLAPPED* pOverlapped = (OVERLAPPED*)(pev->pOverlapped);
		DWORD dwSize = 0;
		DWORD dwFlags = 0;
		sl_bool flagError = sl_false;
		if (::WSAGetOverlappedResult((SOCKET)handle, pOverlapped, &dwSize, FALSE, &dwFlags)) {
			m_flagAccepting = sl_false;
			processAccept(sl_false);
		} else {
			int err = ::WSAGetLastError();
			if (err == WSA_IO_INCOMPLETE) {
				return;
			}
			m_flagAccepting = sl_false;
			processAccept(sl_true);
		}
		onOrder();
	}

	void processAccept(sl_bool flagError)
	{
		Ref<AsyncTcpServer> server = Ref<AsyncTcpServer>::from(getObject());
		if (server.isNull()) {
			return;
		}
		Ref<Socket> socketAccept = m_socketAccept;
		if (socketAccept.isNull()) {
			return;
		}
		if (flagError) {
			_onError();
		} else {
			SOCKADDR_IN *paddr_local, *paddr_remote;
			int lenaddr_local = 0;
			int lenaddr_remote = 0;
			m_funcGetAcceptExSockaddrs(m_bufferAccept, 0, sizeof(SOCKADDR_IN)+16, sizeof(SOCKADDR_IN)+16, (sockaddr**)&paddr_local, &lenaddr_local, (sockaddr**)&paddr_remote, &lenaddr_remote);
			if (paddr_remote) {
				SOCKET socketListen = (SOCKET)(getHandle());
				::setsockopt((SOCKET)(socketAccept->getHandle()), SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&socketListen, sizeof(void*));
				SocketAddress addressRemote;
				addressRemote.setSystemSocketAddress(paddr_remote);
				SocketAddress addressLocal;
				addressLocal.setSystemSocketAddress(paddr_local);
				_onAccept(socketAccept, addressRemote);
			}
		}
	}

};

Ref<AsyncTcpServer> AsyncTcpServer::create(const Ref<Socket>& socket, const Ptr<IAsyncTcpServerListener>& listener, const Ref<AsyncIoLoop>& loop, sl_bool flagAutoStart)
{
	Ref<_Win32AsyncTcpServerInstance> ret = _Win32AsyncTcpServerInstance::create(socket, listener);
	return AsyncTcpServer::create(ret.ptr, loop, flagAutoStart);
}


class _Win32AsyncUdpSocketInstance : public AsyncUdpSocketInstance
{
public:
	sl_bool m_flagReceiving;

	WSAOVERLAPPED m_overlappedReceive;
	WSABUF m_bufReceive;
	DWORD m_flagsReceive;
	sockaddr_storage m_addrReceive;
	int m_lenAddrReceive;

public:
	_Win32AsyncUdpSocketInstance()
	{
		m_flagReceiving = sl_false;
	}

	~_Win32AsyncUdpSocketInstance()
	{
		close();
	}

public:
	static Ref<_Win32AsyncUdpSocketInstance> create(const Ref<Socket>& socket, const Ptr<IAsyncUdpSocketListener>& listener, const Memory& buffer)
	{
		Ref<_Win32AsyncUdpSocketInstance> ret;
		if (socket.isNotNull()) {
			if (socket->setNonBlockingMode(sl_true)) {
				sl_file handle = (sl_file)(socket->getHandle());
				if (handle != SLIB_FILE_INVALID_HANDLE) {
					ret = new _Win32AsyncUdpSocketInstance();
					if (ret.isNotNull()) {
						ret->m_socket = socket;
						ret->setHandle(handle);
						ret->m_listener = listener;
						ret->m_buffer = buffer;
						return ret;
					}
				}
			}
		}
		return ret;
	}

	void close()
	{
		AsyncUdpSocketInstance::close();
		setHandle(SLIB_FILE_INVALID_HANDLE);
		m_socket.setNull();
	}

	void onOrder()
	{
		processSend();
		processReceive();
	}

	void onEvent(EventDesc* pev)
	{
		sl_file handle = getHandle();
		if (handle == SLIB_FILE_INVALID_HANDLE) {
			return;
		}
		OVERLAPPED* pOverlapped = (OVERLAPPED*)(pev->pOverlapped);
		if (pOverlapped == &m_overlappedReceive) {
			DWORD dwSize = 0;
			DWORD dwFlags = 0;
			if (::WSAGetOverlappedResult((SOCKET)handle, pOverlapped, &dwSize, FALSE, &dwFlags)) {
				m_flagReceiving = sl_false;
				if (dwSize > 0) {
					SocketAddress addr;
					if (m_lenAddrReceive > 0) {
						if (addr.setSystemSocketAddress(&m_addrReceive, m_lenAddrReceive)) {
							_onReceive(addr, dwSize);
						}
					}
				}
			} else {
				int err = ::WSAGetLastError();
				if (err == WSA_IO_INCOMPLETE) {
					return;
				}
				m_flagReceiving = sl_false;
			}
			processReceive();
		}

	}

	void processSend()
	{
		sl_file handle = getHandle();
		if (handle == SLIB_FILE_INVALID_HANDLE) {
			return;
		}
		Ref<Socket> socket = m_socket;
		if (socket.isNull()) {
			return;
		}
		if (!(socket->isOpened())) {
			return;
		}
		while (Thread::isNotStoppingCurrent()) {
			SendRequest request;
			if (m_queueSendRequests.pop(&request)) {
				socket->sendTo(request.addressTo, request.data.getData(), (sl_uint32)(request.data.getSize()));
			} else {
				break;
			}
		}
	}

	void processReceive()
	{
		if (m_flagReceiving) {
			return;
		}
		sl_file handle = getHandle();
		if (handle == SLIB_FILE_INVALID_HANDLE) {
			return;
		}
		Ref<Socket> socket = m_socket;
		if (socket.isNull()) {
			return;
		}
		if (!(socket->isOpened())) {
			return;
		}
		void* buf = m_buffer.getData();
		sl_uint32 sizeBuf = (sl_uint32)(m_buffer.getSize());

		Base::zeroMemory(&m_overlappedReceive, sizeof(m_overlappedReceive));
		m_bufReceive.buf = (CHAR*)(buf);
		m_bufReceive.len = sizeBuf;
		m_flagsReceive = 0;
		DWORD dwRead = 0;
		m_lenAddrReceive = sizeof(sockaddr_storage);
		int ret = WSARecvFrom((SOCKET)handle, &m_bufReceive, 1, &dwRead, &m_flagsReceive
			, (sockaddr*)&m_addrReceive, &m_lenAddrReceive, &m_overlappedReceive, NULL);
		if (ret == 0) {
			m_flagReceiving = sl_true;
		} else {
			DWORD dwErr = ::WSAGetLastError();
			if (dwErr == WSA_IO_PENDING) {
				m_flagReceiving = sl_true;
			} else {
				requestOrder();
			}
		}
	}

};

Ref<AsyncUdpSocket> AsyncUdpSocket::create(const Ref<Socket>& socket, const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, const Ref<AsyncIoLoop>& loop, sl_bool flagAutoStart)
{
	Memory buffer = Memory::create(packetSize);
	if (buffer.isNotEmpty()) {
		Ref<_Win32AsyncUdpSocketInstance> ret = _Win32AsyncUdpSocketInstance::create(socket, listener, buffer);
		return AsyncUdpSocket::create(ret.ptr, loop, flagAutoStart);
	}
	return Ref<AsyncUdpSocket>::null();
}

SLIB_NETWORK_NAMESPACE_END

#endif
