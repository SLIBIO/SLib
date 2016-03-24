#ifndef CHECKHEADER_SLIB_NETWORK_DATAGRAM
#define CHECKHEADER_SLIB_NETWORK_DATAGRAM

#include "definition.h"

#include "async.h"
#include "../core/io.h"

SLIB_NETWORK_NAMESPACE_BEGIN

class TcpDatagramClient;
class TcpDatagramServer;

class SLIB_EXPORT ITcpDatagramListener
{
public:
	virtual void onReceiveFrom(TcpDatagramClient* client, void* data, sl_uint32 sizeReceived) = 0;
	
	virtual void onError(TcpDatagramClient* client);
	
	virtual void onConnect(TcpDatagramClient* client, sl_bool flagError);
	
};

class SLIB_EXPORT TcpDatagramParam
{
public:
	SocketAddress bindAddress;
	Ref<AsyncIoLoop> ioLoop;
	Ptr<ITcpDatagramListener> listener;
	sl_uint32 maxWaitingBytesForSending = 1024000;
	
};

class SLIB_EXPORT TcpDatagramClientParam : public TcpDatagramParam
{
public:
	SocketAddress serverAddress;
	sl_bool flagAutoConnect = sl_true;
	sl_bool flagAutoReconnect = sl_false;
	sl_uint32 autoReconnectIntervalSeconds = 5;
	
};

class SLIB_EXPORT TcpDatagramClient : public Object, public IAsyncTcpSocketListener
{
public:
	TcpDatagramClient();

	~TcpDatagramClient();
	
public:
	static Ref<TcpDatagramClient> create(const TcpDatagramClientParam& param);
	
	void close();
	
	void connect();
	
	sl_bool isOpened();
	
	sl_bool isConnecting();

	sl_bool isConnected();
	
	Ref<TcpDatagramServer> getServer();
	
	sl_bool send(const void* data, sl_uint32 size);
	
	sl_bool send(const Memory& mem);
	
protected:
	// override
	void onConnect(AsyncTcpSocket* socket, const SocketAddress& address, sl_bool flagError);
	
	// override
	void onReceive(AsyncTcpSocket* socket, void* data, sl_uint32 sizeReceive, const Referable* refData, sl_bool flagError);
	
	// override
	void onSend(AsyncTcpSocket* socket, void* data, sl_uint32 sizeSent, const Referable* refData, sl_bool flagError);
	
	void onMessageError(AsyncTcpSocket* socket);
	
protected:
	static Ref<TcpDatagramClient> _createForServer(TcpDatagramServer* server, const Ref<Socket>& socket);

	void _reconnect();
	
	void _close();
	
protected:
	sl_bool m_flagOpened;
	Ref<AsyncTcpSocket> m_socketConnect;
	Ref<AsyncTcpSocket> m_socketMessage;

	Memory m_bufReceive;
	DatagramStream m_datagram;
	sl_uint32 m_maxWaitingBytesForSending;
	
	WeakRef<TcpDatagramServer> m_server;
	Ptr<ITcpDatagramListener> m_listener;
	Ref<AsyncIoLoop> m_ioLoop;
	SocketAddress m_addressBind;
	SocketAddress m_addressServer;
	sl_bool m_flagAutoReconnect;
	sl_uint32 m_autoReconnectIntervalSeconds;
	
	friend class TcpDatagramServer;
	
};

class SLIB_EXPORT TcpDatagramServerParam : public TcpDatagramParam
{
public:
	sl_bool flagAutoStart = sl_true;

};

class SLIB_EXPORT TcpDatagramServer : public Object, public IAsyncTcpServerListener
{
protected:
	TcpDatagramServer();
	
	~TcpDatagramServer();
	
public:
	static Ref<TcpDatagramServer> create(const TcpDatagramServerParam& param);
	
	void close();
	
	void start();
	
	sl_bool isRunning();
	
	Ptr<ITcpDatagramListener> getListener();
	
	Ref<AsyncIoLoop> getIoLoop();
	
protected:
	// override
	void onAccept(AsyncTcpServer* socketListen, const Ref<Socket>& socketAccept, const SocketAddress& address);
	
protected:
	sl_bool _initialize(const TcpDatagramServerParam& param);
	
	void _removeClient(TcpDatagramClient* client);
	
protected:
	Ref<AsyncTcpServer> m_server;
	HashMap< TcpDatagramClient*, Ref<TcpDatagramClient> > m_clients;
	
	Ptr<ITcpDatagramListener> m_listener;
	Ref<AsyncIoLoop> m_ioLoop;
	
	sl_uint32 m_maxWaitingBytesForSending;

	friend class TcpDatagramClient;
	
};

SLIB_NETWORK_NAMESPACE_END

#endif
