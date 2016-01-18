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
	
	virtual void onReceive(AsyncTcpSocket* socket, void* data, sl_uint32 sizeReceive, const Referable* refData, sl_bool flagError);
	
	virtual void onSend(AsyncTcpSocket* socket, void* data, sl_uint32 sizeSent, const Referable* refData, sl_bool flagError);

public:
	// override
	void onRead(AsyncStream* stream, void* data, sl_uint32 sizeRead, const Referable* ref, sl_bool flagError);
	
	// override
	void onWrite(AsyncStream* stream, void* data, sl_uint32 sizeWritten, const Referable* ref, sl_bool flagError);
	
};

class SLIB_EXPORT AsyncTcpSocketInstance : public AsyncStreamInstance
{
protected:
	AsyncTcpSocketInstance();

public:
	SLIB_INLINE Ref<Socket> getSocket()
	{
		return m_socket;
	}

	SLIB_INLINE sl_bool isSupportingConnect()
	{
		return m_flagSupportingConnect;
	}

	sl_bool connect(const SocketAddress& address, const Ptr<IAsyncTcpSocketListener>& listener);

	sl_bool receive(void* data, sl_uint32 size, const Ptr<IAsyncTcpSocketListener>& listener, const Referable* refData);
	
	sl_bool send(void* data, sl_uint32 size, const Ptr<IAsyncTcpSocketListener>& listener, const Referable* refData);

protected:
	void _onReceive(AsyncStreamRequest* req, sl_uint32 size, sl_bool flagError);
	
	void _onSend(AsyncStreamRequest* req, sl_uint32 size, sl_bool flagError);
	
	void _onConnect(sl_bool flagError);

protected:
	SafeRef<Socket> m_socket;

	sl_bool m_flagSupportingConnect;
	sl_bool m_flagRequestConnect;
	SocketAddress m_addressRequestConnect;
	SafePtr<IAsyncTcpSocketListener> m_listenerConnect;
};

class SLIB_EXPORT AsyncTcpSocket : public AsyncStreamBase
{
public:
	static Ref<AsyncTcpSocket> create(const Ref<Socket>& socket, const Ref<AsyncLoop>& loop);
	
	static Ref<AsyncTcpSocket> create(const Ref<Socket>& socket);
	
	static Ref<AsyncTcpSocket> create(const Ref<AsyncLoop>& loop, sl_bool flagIPv6 = sl_false);
	
	static Ref<AsyncTcpSocket> create(sl_bool flagIPv6 = sl_false);
	
	static Ref<AsyncTcpSocket> create(const SocketAddress& addressBind, const Ref<AsyncLoop>& loop);
	
	static Ref<AsyncTcpSocket> create(const SocketAddress& addressBind);
	
	static Ref<AsyncTcpSocket> createAndConnect(const SocketAddress& addressBind, const SocketAddress& addressConnect, const Ptr<IAsyncTcpSocketListener>& listener, const Ref<AsyncLoop>& loop);
	
	static Ref<AsyncTcpSocket> createAndConnect(const SocketAddress& addressBind, const SocketAddress& addressConnect, const Ptr<IAsyncTcpSocketListener>& listener);
	
	static Ref<AsyncTcpSocket> createAndConnect(const SocketAddress& addressConnect, const Ptr<IAsyncTcpSocketListener>& listener, const Ref<AsyncLoop>& loop);
	
	static Ref<AsyncTcpSocket> createAndConnect(const SocketAddress& addressConnect, const Ptr<IAsyncTcpSocketListener>& listener);
	
public:
	Ref<Socket> getSocket();

public:
	sl_bool connect(const SocketAddress& address, const Ptr<IAsyncTcpSocketListener>& listener);

	sl_bool receive(void* data, sl_uint32 size, const Ptr<IAsyncTcpSocketListener>& listener, const Referable* refData = sl_null);
	
	SLIB_INLINE sl_bool receive(const Memory& mem, const Ptr<IAsyncTcpSocketListener>& listener)
	{
		return receive(mem.getBuf(), (sl_uint32)(mem.getSize()), listener, mem.getObject());
	}

	sl_bool send(void* data, sl_uint32 size, const Ptr<IAsyncTcpSocketListener>& listener, const Referable* refData = sl_null);
	
	SLIB_INLINE sl_bool send(const Memory& mem, const Ptr<IAsyncTcpSocketListener>& listener)
	{
		return send(mem.getBuf(), (sl_uint32)(mem.getSize()), listener, mem.getObject());
	}

protected:
	SLIB_INLINE Ref<AsyncTcpSocketInstance> getInstance()
	{
		return Ref<AsyncTcpSocketInstance>::from(AsyncStreamBase::getInstance());
	}

private:
	void _connectCallback(SocketAddress address, Ptr<IAsyncTcpSocketListener> listener);

private:
	static Ref<AsyncTcpSocket> create(AsyncTcpSocketInstance* instance, const Ref<AsyncLoop>& loop);
};



class AsyncTcpServer;

class SLIB_EXPORT IAsyncTcpServerListener
{
public:
	virtual void onAccept(AsyncTcpServer* socketListen, const Ref<Socket>& socketAccept, const SocketAddress& address);
	
	virtual void onError(AsyncTcpServer* socketListen);
	
};

class SLIB_EXPORT AsyncTcpServerInstance : public AsyncInstance
{
public:
	SLIB_INLINE Ref<Socket> getSocket()
	{
		return m_socket;
	}
	
	SLIB_PTR_PROPERTY_INLINE(IAsyncTcpServerListener, Listener)

protected:
	SafeRef<Socket> m_socket;

protected:
	void _onAccept(const Ref<Socket>& socketAccept, const SocketAddress& address);
	void _onError();
};

class SLIB_EXPORT AsyncTcpServer : public AsyncObject
{
private:
	AsyncTcpServer();
	
public:
	static Ref<AsyncTcpServer> create(const Ref<Socket>& socket, const Ref<AsyncLoop>& loop);
	
	static Ref<AsyncTcpServer> create(const Ref<Socket>& socket);
	
	static Ref<AsyncTcpServer> create(const SocketAddress& addressListen, const Ref<AsyncLoop>& loop);
	
	static Ref<AsyncTcpServer> create(const SocketAddress& addressListen);
	
	static Ref<AsyncTcpServer> create(sl_uint32 portListen, const Ref<AsyncLoop>& loop, sl_bool flagIPv6 = sl_false);
	
	static Ref<AsyncTcpServer> create(sl_uint32 portListen, sl_bool flagIPv6 = sl_false);

public:
	Ref<Socket> getSocket();

	
	Ptr<IAsyncTcpServerListener> getListener();
	
	void setListener(const Ptr<IAsyncTcpServerListener>& listener);

public:
	void close();

	sl_bool isOpened();

	void start();

	SLIB_INLINE void start(const Ptr<IAsyncTcpServerListener>& listener)
	{
		setListener(listener);
		start();
	}

protected:
	SLIB_INLINE Ref<AsyncTcpServerInstance> getInstance()
	{
		return Ref<AsyncTcpServerInstance>::from(AsyncObject::getInstance());
	}

private:
	static Ref<AsyncTcpServer> create(AsyncTcpServerInstance* instance, const Ref<AsyncLoop>& loop);

private:
	sl_bool m_flagStarted;
};



class AsyncUdpSocket;

class SLIB_EXPORT IAsyncUdpSocketListener
{
public:
	virtual void onReceiveFrom(AsyncUdpSocket* socket, void* data, sl_uint32 sizeReceive, const SocketAddress& address, sl_bool flagError);

	virtual void onSendTo(AsyncUdpSocket* socket, void* data, sl_uint32 sizeSent, const SocketAddress& address, sl_bool flagError);

};

class SLIB_EXPORT AsyncUdpSocketInstance : public AsyncInstance
{
public:
	SLIB_INLINE Ref<Socket> getSocket()
	{
		return m_socket;
	}

	sl_bool receiveFrom(void* data, sl_uint32 size, const Ptr<IAsyncUdpSocketListener>& listener, const Referable* refData);

	sl_bool sendTo(const SocketAddress& address, void* data, sl_uint32 size, const Ptr<IAsyncUdpSocketListener>& listener, const Referable* refData);
	
protected:
	class ReceiveRequest : public Referable
	{
	public:
		void* data;
		sl_uint32 size;
		Ref<Referable> refData;
		Ptr<IAsyncUdpSocketListener> listener;
	};
	void _onReceive(ReceiveRequest* req, sl_uint32 size, const SocketAddress& address, sl_bool flagError);
	
	class SendRequest : public Referable
	{
	public:
		void* data;
		sl_uint32 size;
		Ref<Referable> refData;
		Ptr<IAsyncUdpSocketListener> listener;
		SocketAddress address;
	};
	void _onSend(SendRequest* req, sl_uint32 size, sl_bool flagError);

protected:
	SafeRef<Socket> m_socket;
	Queue< Ref<ReceiveRequest> > m_requestsReceive;
	Queue< Ref<SendRequest> > m_requestsSend;
};

class SLIB_EXPORT AsyncUdpSocket : public AsyncObject
{
public:
	static Ref<AsyncUdpSocket> create(const Ref<Socket>& socket, const Ref<AsyncLoop>& loop);
	
	static Ref<AsyncUdpSocket> create(const Ref<Socket>& socket);
	
	static Ref<AsyncUdpSocket> create(const SocketAddress& addressBind, const Ref<AsyncLoop>& loop, sl_bool flagBroadcast = sl_false);
	
	static Ref<AsyncUdpSocket> create(const SocketAddress& addressBind, sl_bool flagBroadcast = sl_false);
	
	static Ref<AsyncUdpSocket> create(sl_uint32 portBind, const Ref<AsyncLoop>& loop, sl_bool flagBroadcast = sl_false, sl_bool flagIPv6 = sl_false);
	
	static Ref<AsyncUdpSocket> create(sl_uint32 portBind = 0, sl_bool flagBroadcast = sl_false, sl_bool flagIPv6 = sl_false);
	
	static Ref<AsyncUdpSocket> create(const Ref<AsyncLoop>& loop, sl_bool flagBroadcast = sl_false, sl_bool flagIPv6 = sl_false);
	
public:
	Ref<Socket> getSocket();

public:
	void close();

	sl_bool isOpened();

	
	void setBroadcast(sl_bool flag);

	sl_bool receiveFrom(void* data, sl_uint32 size, const Ptr<IAsyncUdpSocketListener>& listener, const Referable* refData = sl_null);
	
	SLIB_INLINE sl_bool receiveFrom(const Memory& mem, const Ptr<IAsyncUdpSocketListener>& listener)
	{
		return receiveFrom(mem.getBuf(), (sl_uint32)(mem.getSize()), listener, mem.getObject());
	}

	sl_bool sendTo(const SocketAddress& addressTo, void* data, sl_uint32 size, const Ptr<IAsyncUdpSocketListener>& listener, const Referable* refData = sl_null);

	SLIB_INLINE sl_bool sendTo(const SocketAddress& addressTo, const Memory& mem, const Ptr<IAsyncUdpSocketListener>& listener)
	{
		return sendTo(addressTo, mem.getBuf(), (sl_uint32)(mem.getSize()), listener, mem.getObject());
	}

	SLIB_INLINE sl_bool sendBroadcast(sl_uint32 port, void* data, sl_uint32 size, const Ptr<IAsyncUdpSocketListener>& listener, const Referable* refData = sl_null)
	{
		SocketAddress address(IPv4Address::getBroadcast(), port);
		return sendTo(address, data, size, listener, refData);
	}
	
	SLIB_INLINE sl_bool sendBroadcast(sl_uint32 port, const Memory& mem, const Ptr<IAsyncUdpSocketListener>& listener)
	{
		return sendBroadcast(port, mem.getBuf(), (sl_uint32)(mem.getSize()), listener, mem.getObject());
	}

	SLIB_INLINE sl_bool sendTo(const SocketAddress& addressTo, void* data, sl_uint32 size, const Referable* refData = sl_null)
	{
		return sendTo(addressTo, data, size, Ptr<IAsyncUdpSocketListener>::null(), refData);
	}

	SLIB_INLINE sl_bool sendTo(const SocketAddress& addressTo, const Memory& mem)
	{
		return sendTo(addressTo, mem.getBuf(), (sl_uint32)(mem.getSize()), Ptr<IAsyncUdpSocketListener>::null(), mem.getObject());
	}

	SLIB_INLINE sl_bool sendBroadcast(sl_uint32 port, void* data, sl_uint32 size, const Referable* refData = sl_null)
	{
		SocketAddress address(IPv4Address::getBroadcast(), port);
		return sendTo(address, data, size, Ptr<IAsyncUdpSocketListener>::null(), refData);
	}

	SLIB_INLINE sl_bool sendBroadcast(sl_uint32 port, const Memory& mem)
	{
		return sendBroadcast(port, mem.getBuf(), (sl_uint32)(mem.getSize()), Ptr<IAsyncUdpSocketListener>::null(), mem.getObject());
	}

protected:
	SLIB_INLINE Ref<AsyncUdpSocketInstance> getInstance()
	{
		return Ref<AsyncUdpSocketInstance>::from(AsyncObject::getInstance());
	}

private:
	static Ref<AsyncUdpSocket> create(AsyncUdpSocketInstance* instance, const Ref<AsyncLoop>& loop);
};

SLIB_NETWORK_NAMESPACE_END

#endif
