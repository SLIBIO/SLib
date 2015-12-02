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
	Ref<AsyncStreamRequest> m_requestReading;

	WSAOVERLAPPED m_overlappedWrite;
	WSABUF m_bufWrite;
	Ref<AsyncStreamRequest> m_requestWriting;

	WSAOVERLAPPED m_overlappedConnect;
	LPFN_CONNECTEX m_funcConnectEx;

	_Win32AsyncTcpSocketInstance()
	{
	}

	~_Win32AsyncTcpSocketInstance()
	{
		close();
	}

	static Ref<_Win32AsyncTcpSocketInstance> create(const Ref<Socket>& _socket)
	{
		Ref<_Win32AsyncTcpSocketInstance> ret;
		Ref<Socket> socket = _socket;
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
			if (m_requestsRead.pop(&req)) {
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
							_onReceive(req.get(), 0, sl_true);
						}
					}
				}
			}
		}
		if (m_requestWriting.isNull()) {
			Ref<AsyncStreamRequest> req;
			if (m_requestsWrite.pop(&req)) {
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
							_onSend(req.get(), 0, sl_true);
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
							// ConnectEx requires the socket to be “initially bound”
							sockaddr_storage saBind;
							SocketAddress aBind;
							aBind.port = 0;
							if (socket->isIPv6()) {
								aBind.ip = IPv6Address::getAny();
							} else {
								aBind.ip = IPv4Address::getAny();
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
				flagError = sl_true;
			}
			Ref<AsyncStreamRequest> req = m_requestReading;
			m_requestReading.setNull();
			if (req.isNotNull()) {
				_onReceive(req.get(), dwSize, flagError);
			}
		} else if (pOverlapped == &m_overlappedWrite) {
			if (dwSize == 0) {
				flagError = sl_true;
			}
			Ref<AsyncStreamRequest> req = m_requestWriting;
			m_requestWriting.setNull();
			if (req.isNotNull()) {
				_onSend(req.get(), dwSize, flagError);
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

Ref<AsyncTcpSocket> AsyncTcpSocket::create(const Ref<Socket>& socket, const Ref<AsyncLoop>& loop)
{
	Ref<_Win32AsyncTcpSocketInstance> ret = _Win32AsyncTcpSocketInstance::create(socket);
	return AsyncTcpSocket::create(ret.get(), loop);
}


class _Win32AsyncTcpServerInstance : public AsyncTcpServerInstance
{
public:
	sl_bool m_flagListening;

	WSAOVERLAPPED m_overlapped;
	char m_bufferAccept[2 * (sizeof(SOCKADDR_IN)+16)];
	Ref<Socket> m_socketAccept;

	LPFN_ACCEPTEX m_funcAcceptEx;
	LPFN_GETACCEPTEXSOCKADDRS m_funcGetAcceptExSockaddrs;

	_Win32AsyncTcpServerInstance()
	{
		m_flagListening = sl_false;
	}

	~_Win32AsyncTcpServerInstance()
	{
		close();
	}

	static Ref<_Win32AsyncTcpServerInstance> create(const Ref<Socket>& _socket)
	{
		Ref<_Win32AsyncTcpServerInstance> ret;
		Ref<Socket> socket = _socket;
		if (socket.isNotNull()) {
			sl_file handle = (sl_file)(socket->getHandle());
			if (handle != SLIB_FILE_INVALID_HANDLE) {
				ret = new _Win32AsyncTcpServerInstance();
				if (ret.isNotNull()) {
					ret->m_socket = socket;
					ret->setHandle(handle);
					if (!(ret->initialize())) {
						ret.setNull();
					}
					return ret;
				}
			}
		}
		return ret;
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
		m_socket.setNull();
		setHandle(SLIB_FILE_INVALID_HANDLE);
	}

	void onOrder()
	{
		sl_file handle = getHandle();
		if (handle == SLIB_FILE_INVALID_HANDLE) {
			return;
		}
		do {
			Ref<Socket> socket = m_socket;
			if (socket.isNull()) {
				break;
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
						m_flagListening = sl_true;
					} else {
						processAccept(sl_true);
					}
				}
			} else {
				processAccept(sl_true);
			}
		} while (0);
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
			processAccept(sl_false);
		} else {
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

Ref<AsyncTcpServer> AsyncTcpServer::create(const Ref<Socket>& socket, const Ref<AsyncLoop>& loop)
{
	Ref<_Win32AsyncTcpServerInstance> ret = _Win32AsyncTcpServerInstance::create(socket);
	return AsyncTcpServer::create(ret.get(), loop);
}


class _Win32AsyncUdpSocketInstance : public AsyncUdpSocketInstance
{
public:
	WSAOVERLAPPED m_overlappedReceive;
	WSABUF m_bufReceive;
	DWORD m_flagsReceive;
	Ref<ReceiveRequest> m_requestReceiving;
	sockaddr_storage m_addrReceive;
	int m_lenAddrReceive;

	WSAOVERLAPPED m_overlappedSend;
	WSABUF m_bufSend;
	Ref<SendRequest> m_requestSending;

	_Win32AsyncUdpSocketInstance()
	{
	}

	~_Win32AsyncUdpSocketInstance()
	{
		close();
	}

	static Ref<_Win32AsyncUdpSocketInstance> create(const Ref<Socket>& _socket)
	{
		Ref<_Win32AsyncUdpSocketInstance> ret;
		Ref<Socket> socket = _socket;
		if (socket.isNotNull()) {
			sl_file handle = (sl_file)(socket->getHandle());
			if (handle != SLIB_FILE_INVALID_HANDLE) {
				ret = new _Win32AsyncUdpSocketInstance();
				if (ret.isNotNull()) {
					ret->m_socket = socket;
					ret->setHandle(handle);
					return ret;
				}
			}
		}
		return ret;
	}

	void close()
	{
		setHandle(SLIB_FILE_INVALID_HANDLE);
		m_socket.setNull();
	}

	void onOrder()
	{
		sl_file handle = getHandle();
		if (handle == SLIB_FILE_INVALID_HANDLE) {
			return;
		}
		if (m_requestReceiving.isNull()) {
			Ref<ReceiveRequest> req;
			if (m_requestsReceive.pop(&req)) {
				if (req.isNotNull()) {
					Base::zeroMemory(&m_overlappedReceive, sizeof(m_overlappedReceive));
					m_bufReceive.buf = (CHAR*)(req->data);
					m_bufReceive.len = req->size;
					m_flagsReceive = 0;
					DWORD dwRead = 0;
					m_lenAddrReceive = sizeof(sockaddr_storage);
					int ret = WSARecvFrom((SOCKET)handle, &m_bufReceive, 1, &dwRead, &m_flagsReceive
						, (sockaddr*)&m_addrReceive, &m_lenAddrReceive, &m_overlappedReceive, NULL);
					if (ret == 0) {
						m_requestReceiving = req;
						EventDesc desc;
						desc.pOverlapped = &m_overlappedReceive;
						onEvent(&desc);
					} else {
						DWORD dwErr = ::WSAGetLastError();
						if (dwErr == WSA_IO_PENDING) {
							m_requestReceiving = req;
						} else {
							_onReceive(req.get(), 0, SocketAddress::none(), sl_true);
						}
					}
				}
			}
		}
		if (m_requestSending.isNull()) {
			Ref<SendRequest> req;
			if (m_requestsSend.pop(&req)) {
				if (req.isNotNull()) {
					sockaddr_storage addr;
					int lenAddr = req->address.getSystemSocketAddress(&addr);
					if (lenAddr > 0) {
						Base::zeroMemory(&m_overlappedSend, sizeof(m_overlappedSend));
						m_bufSend.buf = (CHAR*)(req->data);
						m_bufSend.len = req->size;
						DWORD dwSent = 0;
						int ret = ::WSASendTo((SOCKET)handle, &m_bufSend, 1, &dwSent, 0, (sockaddr*)&addr, lenAddr, &m_overlappedSend, NULL);
						if (ret == 0) {
							m_requestSending = req;
							EventDesc desc;
							desc.pOverlapped = &m_overlappedSend;
							onEvent(&desc);
						} else {
							int dwErr = ::WSAGetLastError();
							if (dwErr == WSA_IO_PENDING) {
								m_requestSending = req;
							} else {
								_onSend(req.get(), 0, sl_true);
							}
						}
					} else {
						_onSend(req.get(), 0, sl_true);
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
			flagError = sl_true;
			close();
		}
		if (dwSize == 0) {
			flagError = sl_true;
		}
		if (pOverlapped == &m_overlappedReceive) {
			Ref<ReceiveRequest> req = m_requestReceiving;
			m_requestReceiving.setNull();
			if (req.isNotNull()) {
				SocketAddress addr;
				if (m_lenAddrReceive > 0) {
					addr.setSystemSocketAddress(&m_addrReceive, m_lenAddrReceive);
				}
				_onReceive(req.get(), dwSize, addr, flagError);
			}
		} else if (pOverlapped == &m_overlappedSend) {
			Ref<SendRequest> req = m_requestSending;
			m_requestSending.setNull();
			if (req.isNotNull()) {
				_onSend(req.get(), dwSize, flagError);
			}
		}
		requestOrder();
	}
};

Ref<AsyncUdpSocket> AsyncUdpSocket::create(const Ref<Socket>& socket, const Ref<AsyncLoop>& loop)
{
	Ref<_Win32AsyncUdpSocketInstance> ret = _Win32AsyncUdpSocketInstance::create(socket);
	return AsyncUdpSocket::create(ret.get(), loop);
}
SLIB_NETWORK_NAMESPACE_END

#endif
