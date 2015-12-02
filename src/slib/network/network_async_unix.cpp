#include "network_async_config.h"

#if defined(SLIB_PLATFORM_IS_UNIX)

#include "../../../inc/slib/network/async.h"
#include "../../../inc/slib/core/log.h"

SLIB_NETWORK_NAMESPACE_BEGIN

class _Unix_AsyncTcpSocketInstance : public AsyncTcpSocketInstance
{
public:
	Ref<AsyncStreamRequest> m_requestReading;
	Ref<AsyncStreamRequest> m_requestWriting;
	sl_uint32 m_sizeWritten;
	
	sl_bool m_flagConnecting;
	
	_Unix_AsyncTcpSocketInstance()
	{
		m_sizeWritten = 0;
		m_flagConnecting = sl_false;
	}
	
	~_Unix_AsyncTcpSocketInstance()
	{
		close();
	}
	
	static Ref<_Unix_AsyncTcpSocketInstance> create(const Ref<Socket>& _socket)
	{
		Ref<_Unix_AsyncTcpSocketInstance> ret;
		Ref<Socket> socket = _socket;
		if (socket.isNotNull()) {
			if (socket->setNonBlockingMode(sl_true)) {
				sl_file handle = (sl_file)(socket->getHandle());
				if (handle != SLIB_FILE_INVALID_HANDLE) {
					ret = new _Unix_AsyncTcpSocketInstance();
					if (ret.isNotNull()) {
						ret->m_socket = socket;
						ret->setHandle(handle);
						return ret;
					}
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
	
	void processRead(sl_bool flagError)
	{
		Ref<Socket> socket = m_socket;
		if (socket.isNull()) {
			return;
		}
		Ref<AsyncStreamRequest> request = m_requestReading;
		m_requestReading.setNull();
		sl_size nQueue = m_requestsRead.count();
		if (Thread::isNotStoppingCurrent()) {
			if (request.isNull()) {
				if (nQueue > 0) {
					nQueue--;
					m_requestsRead.pop(&request);					
					if (request.isNull()) {
						return;
					}
				} else {
					return;
				}
			}
			sl_int32 n = socket->receive((char*)(request->data), request->size);
			if (n > 0) {
				_onReceive(request.get(), n, flagError);
			} else if (n < 0) {
				_onReceive(request.get(), 0, sl_true);
				return;
			} else {
				if (flagError) {
					_onReceive(request.get(), 0, sl_true);
				} else {
					m_requestReading = request;
				}
				return;
			}
			request.setNull();
		}
	}

	void processWrite(sl_bool flagError)
	{
		Ref<Socket> socket = m_socket;
		if (socket.isNull()) {
			return;
		}
		Ref<AsyncStreamRequest> request = m_requestWriting;
		m_requestWriting.setNull();
		sl_size nQueue = m_requestsWrite.count();
		if (Thread::isNotStoppingCurrent()) {
			if (request.isNull()) {
				if (nQueue > 0) {
					nQueue--;
					m_requestsWrite.pop(&request);					
					if (request.isNull()) {
						return;
					} else {
						m_sizeWritten = 0;
					}
				} else {
					return;
				}
			}
			sl_uint32 size = request->size - m_sizeWritten;
			sl_int32 n = socket->send((char*)(request->data) + m_sizeWritten, size);
			if (n > 0) {
				m_sizeWritten += n;
				if (m_sizeWritten >= request->size) {
					_onSend(request.get(), request->size, flagError);
				} else {
					m_requestWriting = request;
				}
			} else if (n < 0) {
				_onSend(request.get(), m_sizeWritten, sl_true);
				return;
			} else {
				if (flagError) {
					_onSend(request.get(), m_sizeWritten, sl_true);
				} else {
					m_requestWriting = request;
				}
				return;
			}
			request.setNull();
		}
	}
	
	void onOrder()
	{
		Ref<Socket> socket = m_socket;
		if (socket.isNull()) {
			return;
		}
		if (m_flagConnecting) {
			return;
		}
		if (m_flagRequestConnect) {
			m_flagRequestConnect = sl_false;
			if (socket->connect(m_addressRequestConnect)) {
				m_flagConnecting = sl_true;
			} else {
				_onConnect(sl_true);
			}
			return;
		}
		processRead(sl_false);
		processWrite(sl_false);
	}
	
	void onEvent(EventDesc* pev)
	{
		sl_bool flagProcessed = sl_false;
		if (pev->flagIn) {
			processRead(pev->flagError);
			flagProcessed = sl_true;
		}
		if (pev->flagOut) {
			if (m_flagConnecting) {
				m_flagConnecting = sl_false;
				if (pev->flagError) {
					_onConnect(sl_true);
				} else {
					_onConnect(sl_false);
				}
			} else {
				processWrite(pev->flagError);
			}
			flagProcessed = sl_true;
		}
		if (!flagProcessed) {
			if (pev->flagError) {
				if (m_flagConnecting) {
					m_flagConnecting = sl_false;
					_onConnect(sl_true);
				} else {
					processRead(sl_true);
					processWrite(sl_true);
				}
			}
		}
		requestOrder();
	}
};

Ref<AsyncTcpSocket> AsyncTcpSocket::create(const Ref<Socket>& socket, const Ref<AsyncLoop>& loop)
{
	Ref<_Unix_AsyncTcpSocketInstance> ret = _Unix_AsyncTcpSocketInstance::create(socket);
	return AsyncTcpSocket::create(ret.get(), loop);
}

class _Unix_AsyncTcpServerInstance : public AsyncTcpServerInstance
{
public:
	sl_bool m_flagListening;
	
	_Unix_AsyncTcpServerInstance()
	{
		m_flagListening = sl_false;
	}
	
	~_Unix_AsyncTcpServerInstance()
	{
		close();
	}
	
	static Ref<_Unix_AsyncTcpServerInstance> create(const Ref<Socket>& _socket)
	{
		Ref<_Unix_AsyncTcpServerInstance> ret;
		Ref<Socket> socket = _socket;
		if (socket.isNotNull()) {
			if (socket->setNonBlockingMode(sl_true)) {
				sl_file handle = (sl_file)(socket->getHandle());
				if (handle != SLIB_FILE_INVALID_HANDLE) {
					ret = new _Unix_AsyncTcpServerInstance();
					if (ret.isNotNull()) {
						ret->m_socket = socket;
						ret->setHandle(handle);
						return ret;
					}
				}				
			}
		}
		return ret;
	}
	
	void close()
	{
		m_socket.setNull();
		setHandle(SLIB_FILE_INVALID_HANDLE);
	}
	
	void onOrder()
	{
		Ref<Socket> socket = m_socket;
		if (socket.isNull()) {
			return;
		}
		while (Thread::isNotStoppingCurrent()) {
			Ref<Socket> socketAccept;
			SocketAddress addr;
			if (socket->accept(socketAccept, addr)) {
				_onAccept(socketAccept, addr);
			} else {
				sl_uint32 err = socket->getLastError();
				if (err != Socket::errorWouldBlock) {
					_onError();
				}
				return;
			}
		}
	}
	
	void onEvent(EventDesc* pev)
	{
		if (pev->flagIn) {
			onOrder();
		}
		if (pev->flagError) {
			_onError();
		}
	}
};

Ref<AsyncTcpServer> AsyncTcpServer::create(const Ref<Socket>& socket, const Ref<AsyncLoop>& loop)
{
	Ref<_Unix_AsyncTcpServerInstance> ret = _Unix_AsyncTcpServerInstance::create(socket);
	return AsyncTcpServer::create(ret.get(), loop);
}

class _Unix_AsyncUdpSocketInstance : public AsyncUdpSocketInstance
{
public:
	Ref<ReceiveRequest> m_requestReceiving;
	
	Ref<SendRequest> m_requestSending;
	
	_Unix_AsyncUdpSocketInstance()
	{
	}
	
	~_Unix_AsyncUdpSocketInstance()
	{
		close();
	}
	
	static Ref<_Unix_AsyncUdpSocketInstance> create(const Ref<Socket>& _socket)
	{
		Ref<_Unix_AsyncUdpSocketInstance> ret;
		Ref<Socket> socket = _socket;
		if (socket.isNotNull()) {
			if (socket->setNonBlockingMode(sl_true)) {
				sl_file handle = (sl_file)(socket->getHandle());
				if (handle != SLIB_FILE_INVALID_HANDLE) {
					ret = new _Unix_AsyncUdpSocketInstance();
					if (ret.isNotNull()) {
						ret->m_socket = socket;
						ret->setHandle(handle);
						return ret;
					}
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
	
	void processReceive(sl_bool flagError)
	{
		Ref<Socket> socket = m_socket;
		if (socket.isNull()) {
			return;
		}
		Ref<ReceiveRequest> request = m_requestReceiving;
		m_requestReceiving.setNull();
		sl_size nQueue = m_requestsReceive.count();
		if (Thread::isNotStoppingCurrent()) {
			if (request.isNull()) {
				if (nQueue > 0) {
					nQueue--;
					m_requestsReceive.pop(&request);					
					if (request.isNull()) {
						return;
					}
				} else {
					return;
				}
			}
			SocketAddress addr;
			sl_int32 n = socket->receiveFrom((char*)(request->data), request->size, addr);
			if (n > 0) {
				_onReceive(request.get(), n, addr, flagError);
			} else if (n < 0) {
				_onReceive(request.get(), 0, SocketAddress::none(), sl_true);
				return;
			} else {
				if (flagError) {
					_onReceive(request.get(), 0, SocketAddress::none(), sl_true);
				} else {
					m_requestReceiving = request;
				}
				return;
			}
			request.setNull();
		}
	}
	
	void processSend(sl_bool flagError)
	{
		Ref<Socket> socket = m_socket;
		if (socket.isNull()) {
			return;
		}
		Ref<SendRequest> request = m_requestSending;
		m_requestSending.setNull();
		sl_size nQueue = m_requestsSend.count();
		if (Thread::isNotStoppingCurrent()) {
			if (request.isNull()) {
				if (nQueue > 0) {
					nQueue--;
					m_requestsSend.pop(&request);					
					if (request.isNull()) {
						return;
					}
				} else {
					return;
				}
			}
			sl_int32 n = socket->sendTo((char*)(request->data), request->size, request->address);
			if (n > 0) {
				_onSend(request.get(), n, flagError);
			} else if (n < 0) {
				_onSend(request.get(), 0, sl_true);
				return;
			} else {
				if (flagError) {
					_onSend(request.get(), 0, sl_true);
				} else {
					m_requestSending = request;
				}
				return;
			}
			request.setNull();
		}
	}
	
	void onOrder()
	{
		processReceive(sl_false);
		processSend(sl_false);
	}
	
	void onEvent(EventDesc* pev)
	{
		sl_bool flagProcessed = sl_false;
		if (pev->flagIn) {
			processReceive(pev->flagError);
			flagProcessed = sl_true;
		}
		if (pev->flagOut) {
			processSend(pev->flagError);
			flagProcessed = sl_true;
		}
		if (!flagProcessed) {
			if (pev->flagError) {
				processReceive(sl_true);
				processSend(sl_true);
			}
		}
		requestOrder();
	}

};

Ref<AsyncUdpSocket> AsyncUdpSocket::create(const Ref<Socket>& socket, const Ref<AsyncLoop>& loop)
{
	Ref<_Unix_AsyncUdpSocketInstance> ret = _Unix_AsyncUdpSocketInstance::create(socket);
	return AsyncUdpSocket::create(ret.get(), loop);
}

SLIB_NETWORK_NAMESPACE_END

#endif
