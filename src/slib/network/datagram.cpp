#include "../../../inc/slib/network/datagram.h"

SLIB_NETWORK_NAMESPACE_BEGIN

#define DATAGRAM_BUF_SIZE 4096

void ITcpDatagramListener::onConnect(TcpDatagramClient* client)
{
}

void ITcpDatagramListener::onError(TcpDatagramClient* client)
{
}

TcpDatagramParam::TcpDatagramParam()
{
	maxWaitingBytesForSending = 1024000;
}

TcpDatagramParam::~TcpDatagramParam()
{
}

/***************************************
		TcpDatagramClient
***************************************/

TcpDatagramClientParam::TcpDatagramClientParam()
{
	flagAutoConnect = sl_true;
	flagAutoReconnect = sl_false;
	autoReconnectIntervalSeconds = 5;
}

TcpDatagramClientParam::~TcpDatagramClientParam()
{
}


SLIB_DEFINE_OBJECT(TcpDatagramClient, Object)

TcpDatagramClient::TcpDatagramClient()
{
	m_flagOpened = sl_false;
	m_flagAutoReconnect = sl_false;
}

TcpDatagramClient::~TcpDatagramClient()
{
	close();
}

Ref<TcpDatagramClient> TcpDatagramClient::create(const TcpDatagramClientParam& param)
{
	Memory memReceive = Memory::create(DATAGRAM_BUF_SIZE);
	
	if (memReceive.isNotEmpty()) {
		
		if (param.serverAddress.isValid()) {
			
			Ref<TcpDatagramClient> ret = new TcpDatagramClient;
			
			if (ret.isNotNull()) {
				
				ret->m_flagOpened = sl_true;
				ret->m_ioLoop = param.ioLoop;
				ret->m_addressBind = param.bindAddress;
				ret->m_addressServer = param.serverAddress;
				ret->m_flagAutoReconnect = param.flagAutoReconnect;
				ret->m_autoReconnectIntervalSeconds = param.autoReconnectIntervalSeconds;
				ret->m_bufReceive = memReceive;
				ret->m_maxWaitingBytesForSending = param.maxWaitingBytesForSending;
				ret->m_datagram.setMaxDatagramSize(param.maxWaitingBytesForSending);
				
				ret->m_listener = param.listener;
				ret->m_onConnect = param.onConnect;
				ret->m_onReceiveFrom = param.onReceiveFrom;
				ret->m_onError = param.onError;

				if (param.flagAutoConnect) {
					ret->connect();
				}
				
				return ret;
			}
		}
	}
	return sl_null;
}

void TcpDatagramClient::close()
{
	ObjectLocker lock(this);
	if (m_flagOpened) {
		_close();
		m_flagOpened = sl_false;
		Ref<TcpDatagramServer> server = m_server;
		if (server.isNotNull()) {
			server->_removeClient(this);
		}
	}
}

void TcpDatagramClient::connect()
{
	ObjectLocker lock(this);
	if (m_flagOpened) {
		if (m_socketMessage.isNotNull()) {
			m_socketMessage->close();
			m_socketMessage.setNull();
		}
		AsyncTcpSocketParam param;
		param.bindAddress = m_addressBind;
		param.connectAddress = m_addressServer;
		param.listener.setPointer(this);
		param.ioLoop = m_ioLoop;
		m_socketConnect = AsyncTcpSocket::create(param);
		if (m_socketConnect.isNull()) {
			if (m_flagAutoReconnect) {
				_reconnect();
			}
		}
	}
}

sl_bool TcpDatagramClient::isOpened()
{
	return m_flagOpened;
}

sl_bool TcpDatagramClient::isConnecting()
{
	return m_socketConnect.isNotNull();
}

sl_bool TcpDatagramClient::isConnected()
{
	return m_socketMessage.isNotNull();
}

Ref<TcpDatagramServer> TcpDatagramClient::getServer()
{
	return m_server;
}

sl_bool TcpDatagramClient::send(const void* data, sl_uint32 size)
{
	ObjectLocker lock(this);
	if (m_flagOpened) {
		if (m_socketMessage.isNotNull()) {
			if (m_socketMessage->getWaitingSizeForWrite() < m_maxWaitingBytesForSending) {
				Memory packet = m_datagram.build(data, size);
				if (m_socketMessage->send(packet, SLIB_FUNCTION_WEAKREF(TcpDatagramClient, onSendStream, this))) {
					return sl_true;
				}
			}
		}
	}
	return sl_false;
}

sl_bool TcpDatagramClient::send(const Memory& mem)
{
	return send(mem.getData(), (sl_uint32)(mem.getSize()));
}

void TcpDatagramClient::onConnect(AsyncTcpSocket* socket, const SocketAddress& address, sl_bool flagError)
{
	m_socketConnect.setNull();
	if (flagError) {
		PtrLocker<ITcpDatagramListener> listener(m_listener);
		if (listener.isNotNull()) {
			listener->onError(this);
		}
		m_onError(this);
		if (m_flagAutoReconnect) {
			_reconnect();
		}
	} else {
		m_socketMessage = socket;
		PtrLocker<ITcpDatagramListener> listener(m_listener);
		if (listener.isNotNull()) {
			listener->onConnect(this);
		}
		m_onConnect(this);
		ObjectLocker lock(this);
		if (m_socketMessage.isNotNull()) {
			m_socketMessage->receive(m_bufReceive, SLIB_FUNCTION_WEAKREF(TcpDatagramClient, onReceiveStream, this));
		}
	}
}

void TcpDatagramClient::onReceiveStream(AsyncStreamResult* result)
{
	if (result->flagError) {
		onMessageError(static_cast<AsyncTcpSocket*>(result->stream));
	} else {
		LinkedQueue<Memory> queue;
		if (m_datagram.parse(result->data, result->size, queue)) {
			PtrLocker<ITcpDatagramListener> listener(m_listener);
			if (listener.isNotNull()) {
				Memory packet;
				while (queue.pop(&packet)) {
					listener->onReceiveFrom(this, packet.getData(), (sl_uint32)(packet.getSize()));
					m_onReceiveFrom(this, packet.getData(), (sl_uint32)(packet.getSize()));
				}
			}
		} else {
			onMessageError(static_cast<AsyncTcpSocket*>(result->stream));
		}
		ObjectLocker lock(this);
		if (m_socketMessage.isNotNull()) {
			m_socketMessage->receive(m_bufReceive, SLIB_FUNCTION_WEAKREF(TcpDatagramClient, onReceiveStream, this));
		}
	}
}

void TcpDatagramClient::onSendStream(AsyncStreamResult* result)
{
	if (result->flagError) {
		onMessageError(static_cast<AsyncTcpSocket*>(result->stream));
	}
}

void TcpDatagramClient::onMessageError(AsyncTcpSocket* socket)
{
	ObjectLocker lock(this);
	if (m_socketMessage.get() == socket) {
		PtrLocker<ITcpDatagramListener> listener(m_listener);
		if (listener.isNotNull()) {
			listener->onError(this);
		}
		m_onError(this);
		if (m_flagAutoReconnect) {
			_reconnect();
		} else {
			close();
		}
	}
}

Ref<TcpDatagramClient> TcpDatagramClient::_createForServer(TcpDatagramServer* server, const Ref<Socket>& socketAccepted)
{
	if (server) {
		
		Memory memReceive = Memory::create(DATAGRAM_BUF_SIZE);
		
		if (memReceive.isNotEmpty()) {
		
			Ref<TcpDatagramClient> ret = new TcpDatagramClient;
			
			if (ret.isNotNull()) {
				
				Ref<AsyncIoLoop> loop = server->m_ioLoop;
				
				AsyncTcpSocketParam param;
				param.socket = socketAccepted;
				param.ioLoop = loop;
				
				Ref<AsyncTcpSocket> socket = AsyncTcpSocket::create(param);
				
				if (socket.isNotNull()) {
					
					ret->m_flagOpened = sl_true;
					ret->m_socketMessage = socket;
					ret->m_server = server;
					ret->m_bufReceive = memReceive;
					ret->m_maxWaitingBytesForSending = server->m_maxWaitingBytesForSending;
					ret->m_datagram.setMaxDatagramSize(ret->m_maxWaitingBytesForSending);
					
					ret->m_listener = server->m_listener;
					ret->m_onConnect = server->m_onConnect;
					ret->m_onReceiveFrom = server->m_onReceiveFrom;
					ret->m_onError = server->m_onError;
					
					return ret;
				}
			}
		}
	}
	return sl_null;
}

void TcpDatagramClient::_reconnect()
{
	ObjectLocker lock(this);
	if (m_flagOpened) {
		_close();
		Dispatch::setTimeout(SLIB_FUNCTION_WEAKREF(TcpDatagramClient, connect, this), m_autoReconnectIntervalSeconds*1000);
	}
}

void TcpDatagramClient::_close()
{
	if (m_socketConnect.isNotNull()) {
		m_socketConnect->close();
		m_socketConnect.setNull();
	}
	if (m_socketMessage.isNotNull()) {
		m_socketMessage->close();
		m_socketMessage.setNull();
	}
}

/***************************************
		TcpDatagramServer
***************************************/

TcpDatagramServerParam::TcpDatagramServerParam()
{
	flagAutoStart = sl_true;
}

TcpDatagramServerParam::~TcpDatagramServerParam()
{
}

SLIB_DEFINE_OBJECT(TcpDatagramServer, Object)

TcpDatagramServer::TcpDatagramServer()
{
}

TcpDatagramServer::~TcpDatagramServer()
{
	close();
}

Ref<TcpDatagramServer> TcpDatagramServer::create(const TcpDatagramServerParam& param)
{
	Ref<TcpDatagramServer> server = new TcpDatagramServer;
	if (server.isNotNull()) {
		if (server->_initialize(param)) {
			return server;
		}
	}
	return sl_null;
}

void TcpDatagramServer::close()
{
	m_clients.removeAll();
	ObjectLocker lock(this);
	if (m_server.isNotNull()) {
		m_server->close();
		m_server.setNull();
	}
}

void TcpDatagramServer::start()
{
	ObjectLocker lock(this);
	if (m_server.isNotNull()) {
		m_server->start();
	}
}

sl_bool TcpDatagramServer::isRunning()
{
	ObjectLocker lock(this);
	if (m_server.isNotNull()) {
		return m_server->isRunning();
	}
	return sl_false;
}

Ptr<ITcpDatagramListener> TcpDatagramServer::getListener()
{
	return m_listener;
}

Ref<AsyncIoLoop> TcpDatagramServer::getIoLoop()
{
	return m_ioLoop;
}

void TcpDatagramServer::onAccept(AsyncTcpServer* socketListen, const Ref<Socket>& socketAccept, const SocketAddress& address)
{
	Ref<TcpDatagramClient> client = TcpDatagramClient::_createForServer(this, socketAccept);
	if (client.isNotNull()) {
		m_clients.put(client.get(), client);
		client->onConnect(client->m_socketMessage.get(), address, sl_false);
	}
}

sl_bool TcpDatagramServer::_initialize(const TcpDatagramServerParam& param)
{
	m_listener = param.listener;
	m_onConnect = param.onConnect;
	m_onReceiveFrom = param.onReceiveFrom;
	m_onError = param.onError;
	
	m_ioLoop = param.ioLoop;
	m_maxWaitingBytesForSending = param.maxWaitingBytesForSending;
	
	AsyncTcpServerParam sp;
	sp.bindAddress = param.bindAddress;
	sp.listener.setPointer(this);
	sp.ioLoop = param.ioLoop;
	sp.flagAutoStart = sl_false;
	
	Ref<AsyncTcpServer> server = AsyncTcpServer::create(sp);
	if (server.isNotNull()) {
		m_server = server;
		if (param.flagAutoStart) {
			server->start();
		}
		return sl_true;
	}
	return sl_false;
}

void TcpDatagramServer::_removeClient(TcpDatagramClient* client)
{
	m_clients.remove(client);
}

SLIB_NETWORK_NAMESPACE_END
