#ifndef CHECKHEADER_SLIB_NETWORK_ASYNC
#define CHECKHEADER_SLIB_NETWORK_ASYNC

#include "definition.h"

#include "socket.h"
#include "../core/async.h"

SLIB_NETWORK_NAMESPACE_BEGIN

class AsyncTcpSocket;

class SLIB_EXPORT IAsyncTcpSocketListener : public IAsyncStreamListener
{
public:
	virtual void onConnect(AsyncTcpSocket* socket, const SocketAddress& address, sl_bool flagError);
	
	virtual void onReceive(AsyncTcpSocket* socket, void* data, sl_uint32 sizeReceive, Referable* refData, sl_bool flagError);
	
	virtual void onSend(AsyncTcpSocket* socket, void* data, sl_uint32 sizeSent, Referable* refData, sl_bool flagError);

public:
	// override
	void onRead(AsyncStream* stream, void* data, sl_uint32 sizeRead, Referable* ref, sl_bool flagError);
	
	// override
	void onWrite(AsyncStream* stream, void* data, sl_uint32 sizeWritten, Referable* ref, sl_bool flagError);
	
};

class SLIB_EXPORT AsyncTcpSocketInstance : public AsyncStreamInstance
{
protected:
	AsyncTcpSocketInstance();

public:
	Ref<Socket> getSocket();
	
	sl_bool isSupportingConnect();
	
public:
	sl_bool connect(const SocketAddress& address, const Ptr<IAsyncTcpSocketListener>& listener);

	sl_bool receive(void* data, sl_uint32 size, const Ptr<IAsyncTcpSocketListener>& listener, Referable* refData);
	
	sl_bool send(void* data, sl_uint32 size, const Ptr<IAsyncTcpSocketListener>& listener, Referable* refData);

protected:
	void _onReceive(AsyncStreamRequest* req, sl_uint32 size, sl_bool flagError);
	
	void _onSend(AsyncStreamRequest* req, sl_uint32 size, sl_bool flagError);
	
	void _onConnect(sl_bool flagError);

protected:
	AtomicRef<Socket> m_socket;

	sl_bool m_flagSupportingConnect;
	sl_bool m_flagRequestConnect;
	SocketAddress m_addressRequestConnect;
	AtomicPtr<IAsyncTcpSocketListener> m_listenerConnect;
	
};

class SLIB_EXPORT AsyncTcpSocket : public AsyncStreamBase
{
public:
	static Ref<AsyncTcpSocket> create(const Ref<Socket>& socket, const Ref<AsyncIoLoop>& loop);
	
	static Ref<AsyncTcpSocket> create(const Ref<Socket>& socket);
	
	static Ref<AsyncTcpSocket> create(const SocketAddress& addressBind, const Ref<AsyncIoLoop>& loop);
	
	static Ref<AsyncTcpSocket> create(const SocketAddress& addressBind);
	
	static Ref<AsyncTcpSocket> create(sl_uint32 port, const Ref<AsyncIoLoop>& loop);
	
	static Ref<AsyncTcpSocket> create(sl_uint32 port);
	
	static Ref<AsyncTcpSocket> createIPv6(sl_uint32 port, const Ref<AsyncIoLoop>& loop);
	
	static Ref<AsyncTcpSocket> createIPv6(sl_uint32 port);

	static Ref<AsyncTcpSocket> create(const Ref<AsyncIoLoop>& loop);
	
	static Ref<AsyncTcpSocket> create();
	
	static Ref<AsyncTcpSocket> createIPv6(const Ref<AsyncIoLoop>& loop);
	
	static Ref<AsyncTcpSocket> createIPv6();
	
	static Ref<AsyncTcpSocket> createAndConnect(const SocketAddress& addressBind, const SocketAddress& addressConnect, const Ptr<IAsyncTcpSocketListener>& listener, const Ref<AsyncIoLoop>& loop);
	
	static Ref<AsyncTcpSocket> createAndConnect(const SocketAddress& addressBind, const SocketAddress& addressConnect, const Ptr<IAsyncTcpSocketListener>& listener);
	
	static Ref<AsyncTcpSocket> createAndConnect(const SocketAddress& addressConnect, const Ptr<IAsyncTcpSocketListener>& listener, const Ref<AsyncIoLoop>& loop);
	
	static Ref<AsyncTcpSocket> createAndConnect(const SocketAddress& addressConnect, const Ptr<IAsyncTcpSocketListener>& listener);
	
public:
	Ref<Socket> getSocket();

public:
	sl_bool connect(const SocketAddress& address, const Ptr<IAsyncTcpSocketListener>& listener);

	sl_bool receive(void* data, sl_uint32 size, const Ptr<IAsyncTcpSocketListener>& listener, Referable* refData = sl_null);
	
	sl_bool receive(const Memory& mem, const Ptr<IAsyncTcpSocketListener>& listener);

	sl_bool send(void* data, sl_uint32 size, const Ptr<IAsyncTcpSocketListener>& listener, Referable* refData = sl_null);
	
	sl_bool send(const Memory& mem, const Ptr<IAsyncTcpSocketListener>& listener);

protected:
	Ref<AsyncTcpSocketInstance> getIoInstance();

private:
	static Ref<AsyncTcpSocket> create(AsyncTcpSocketInstance* instance, const Ref<AsyncIoLoop>& loop);
	
};



class AsyncTcpServer;

class SLIB_EXPORT IAsyncTcpServerListener
{
public:
	virtual void onAccept(AsyncTcpServer* socketListen, const Ref<Socket>& socketAccept, const SocketAddress& address) = 0;
	
	virtual void onError(AsyncTcpServer* socketListen);
	
};

class SLIB_EXPORT AsyncTcpServerInstance : public AsyncIoInstance
{
public:
	AsyncTcpServerInstance();

public:
	// override
	void close();

	void start();

	sl_bool isRunning();

	Ref<Socket> getSocket();
	
	Ptr<IAsyncTcpServerListener> getListener();
	
protected:
	void _onAccept(const Ref<Socket>& socketAccept, const SocketAddress& address);
	
	void _onError();

protected:
	sl_bool m_flagRunning;

	AtomicRef<Socket> m_socket;
	Ptr<IAsyncTcpServerListener> m_listener;
	
};

class SLIB_EXPORT AsyncTcpServerParam
{
public:
	SocketAddress bindAddress;
	Ptr<IAsyncTcpServerListener> listener;
	Ref<AsyncIoLoop> ioLoop;
	sl_bool flagAutoStart = sl_true;
	
};

class SLIB_EXPORT AsyncTcpServer : public AsyncIoObject
{
public:
	static Ref<AsyncTcpServer> create(const Ref<Socket>& socket, const Ptr<IAsyncTcpServerListener>& listener, const Ref<AsyncIoLoop>& loop, sl_bool flagAutoStart = sl_true);
	
	static Ref<AsyncTcpServer> create(const Ref<Socket>& socket, const Ptr<IAsyncTcpServerListener>& listener, sl_bool flagAutoStart = sl_true);
	
	static Ref<AsyncTcpServer> create(const AsyncTcpServerParam& param);
	
	static Ref<AsyncTcpServer> create(const SocketAddress& addressListen, const Ptr<IAsyncTcpServerListener>& listener, const Ref<AsyncIoLoop>& loop, sl_bool flagAutoStart = sl_true);
	
	static Ref<AsyncTcpServer> create(const SocketAddress& addressListen, const Ptr<IAsyncTcpServerListener>& listener, sl_bool flagAutoStart = sl_true);
	
	static Ref<AsyncTcpServer> create(sl_uint32 portListen, const Ptr<IAsyncTcpServerListener>& listener, const Ref<AsyncIoLoop>& loop, sl_bool flagAutoStart = sl_true);
	
	static Ref<AsyncTcpServer> create(sl_uint32 portListen, const Ptr<IAsyncTcpServerListener>& listener, sl_bool flagAutoStart = sl_true);
	
	static Ref<AsyncTcpServer> createIPv6(sl_uint32 portListen, const Ptr<IAsyncTcpServerListener>& listener, const Ref<AsyncIoLoop>& loop, sl_bool flagAutoStart = sl_true);
	
	static Ref<AsyncTcpServer> createIPv6(sl_uint32 portListen, const Ptr<IAsyncTcpServerListener>& listener, sl_bool flagAutoStart = sl_true);

public:
	void close();

	sl_bool isOpened();

	void start();

	sl_bool isRunning();

public:
	Ref<Socket> getSocket();

protected:
	Ref<AsyncTcpServerInstance> getIoInstance();

private:
	static Ref<AsyncTcpServer> create(AsyncTcpServerInstance* instance, const Ref<AsyncIoLoop>& loop, sl_bool flagAutoStart);
	
};


class AsyncUdpSocket;

class SLIB_EXPORT IAsyncUdpSocketListener
{
public:
	virtual void onReceiveFrom(AsyncUdpSocket* socket, const SocketAddress& address, void* data, sl_uint32 sizeReceived) = 0;

};

class SLIB_EXPORT AsyncUdpSocketInstance : public AsyncIoInstance
{
public:
	AsyncUdpSocketInstance();

public:
	// override
	void close();

	void start();

	sl_bool isRunning();

	Ref<Socket> getSocket();
	
	Ptr<IAsyncUdpSocketListener> getListener();

public:
	sl_bool sendTo(const SocketAddress& address, const Memory& data);
	
protected:
	void _onReceive(const SocketAddress& address, sl_uint32 size);
	
protected:
	sl_bool m_flagRunning;

	AtomicRef<Socket> m_socket;
	Memory m_buffer;
	
	struct SendRequest
	{
		SocketAddress addressTo;
		Memory data;
	};
	LinkedQueue<SendRequest> m_queueSendRequests;
	
	Ptr<IAsyncUdpSocketListener> m_listener;

};

class SLIB_EXPORT AsyncUdpSocketParam
{
public:
	SocketAddress bindAddress;
	Ptr<IAsyncUdpSocketListener> listener;
	sl_uint32 packetSize;
	Ref<AsyncIoLoop> ioLoop;
	sl_bool flagAutoStart = sl_true;
	
};

class SLIB_EXPORT AsyncUdpSocket : public AsyncIoObject
{
public:
	static Ref<AsyncUdpSocket> create(const Ref<Socket>& socket, const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, const Ref<AsyncIoLoop>& loop, sl_bool flagAutoStart = sl_true);
	
	static Ref<AsyncUdpSocket> create(const Ref<Socket>& socket, const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, sl_bool flagAutoStart = sl_true);
	
	static Ref<AsyncUdpSocket> create(const AsyncUdpSocketParam& param);
	
	static Ref<AsyncUdpSocket> create(const SocketAddress& addressBind, const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, const Ref<AsyncIoLoop>& loop, sl_bool flagAutoStart = sl_true);
	
	static Ref<AsyncUdpSocket> create(const SocketAddress& addressBind, const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, sl_bool flagAutoStart = sl_true);
	
	static Ref<AsyncUdpSocket> create(sl_uint32 portBind, const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, const Ref<AsyncIoLoop>& loop, sl_bool flagAutoStart = sl_true);
	
	static Ref<AsyncUdpSocket> create(sl_uint32 portBind, const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, sl_bool flagAutoStart = sl_true);
	
	static Ref<AsyncUdpSocket> createIPv6(sl_uint32 portBind, const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, const Ref<AsyncIoLoop>& loop, sl_bool flagAutoStart = sl_true);
	
	static Ref<AsyncUdpSocket> createIPv6(sl_uint32 portBind, const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, sl_bool flagAutoStart = sl_true);
	
	static Ref<AsyncUdpSocket> create(const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, const Ref<AsyncIoLoop>& loop, sl_bool flagAutoStart = sl_true);
	
	static Ref<AsyncUdpSocket> create(const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, sl_bool flagAutoStart = sl_true);
	
	static Ref<AsyncUdpSocket> createIPv6(const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, const Ref<AsyncIoLoop>& loop, sl_bool flagAutoStart = sl_true);
	
	static Ref<AsyncUdpSocket> createIPv6(const Ptr<IAsyncUdpSocketListener>& listener, sl_uint32 packetSize, sl_bool flagAutoStart = sl_true);
	
public:
	void close();

	sl_bool isOpened();

	void start();

	sl_bool isRunning();

public:
	Ref<Socket> getSocket();

	void setBroadcast(sl_bool flag);

	void setSendBufferSize(sl_uint32 size);

	void setReceiveBufferSize(sl_uint32 size);

	sl_bool sendTo(const SocketAddress& addressTo, void* data, sl_uint32 size);

	sl_bool sendTo(const SocketAddress& addressTo, const Memory& mem);
	
protected:
	Ref<AsyncUdpSocketInstance> getIoInstance();

private:
	static Ref<AsyncUdpSocket> create(AsyncUdpSocketInstance* instance, const Ref<AsyncIoLoop>& loop, sl_bool flagAutoStart);

};

SLIB_NETWORK_NAMESPACE_END

#endif
