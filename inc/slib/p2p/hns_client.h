#ifndef CHECKHEADER_SLIB_P2P_HNS_CLIENT
#define CHECKHEADER_SLIB_P2P_HNS_CLIENT

#include "definition.h"

#include "../core/object.h"
#include "../core/string.h"
#include "../core/time.h"
#include "../core/pointer.h"
#include "../crypto/rsa.h"

#include "../network/address.h"
#include "../network/socket.h"

SLIB_P2P_NAMESPACE_BEGIN

class SLIB_EXPORT HnsHostInfo
{
public:
	String clientId;
	SocketAddress clientAddress;
	sl_uint64 elapsedMillisecondsFromLastCommunicated;

	SLIB_INLINE HnsHostInfo()
	{
		elapsedMillisecondsFromLastCommunicated = -1;
	}
};

class HnsClient;
class SLIB_EXPORT IHnsClientListener
{
public:
	virtual void onConnected(HnsClient* client) {}

	// check "info.timeLastCommunicate"
	virtual void onQueryHost(HnsClient* client, const HnsHostInfo& info) {}

	virtual void onMessage(HnsClient* client, String senderId, sl_uint64 messageId, const void* message, sl_uint32 size) {}

	virtual void onBroadcastMessage(HnsClient* client, String senderId, sl_uint64 messageId, const void* message, sl_uint32 size) {}

	virtual void onKeepAlive(HnsClient* client, Time serviceTimeSent, sl_uint64 milliseconsdsEllapsed) {}
};

class SLIB_EXPORT HnsClientParam
{
public:
	Ref<Socket> socket;

	Ptr<IHnsClientListener> listener;

	String name;
	SocketAddress serviceAddress;
	RSAPublicKey servicePublicKey;
	String serviceSecret;

	String clientId;
	String clientCertificate;

	sl_uint32 connectionTimeoutMilliseconds;
	sl_uint32 keepAliveIntervalMilliseconds;

	sl_uint64 sessionId;
	String sessionSecret;

	SLIB_INLINE HnsClientParam()
	{
		connectionTimeoutMilliseconds = 60000;
		keepAliveIntervalMilliseconds = 5000;

		sessionId = 0;
	}
};

class SLIB_EXPORT HnsClient : public Object
{
protected:
	SLIB_INLINE HnsClient()
	{
	}

public:
	virtual void release() = 0;

	virtual sl_bool processReceivedPacket(const SocketAddress& address, sl_uint32 methodId, const void* data, sl_uint32 size) = 0;

	virtual void runStep() = 0;

	virtual sl_bool isRunning() = 0;

	virtual sl_bool isConnected() = 0;

	virtual void connect() = 0;

	virtual void reconnect() = 0;

	virtual void queryHost(String targetId) = 0;

	virtual void broadcastMessage(sl_uint64 messageId, const void* data, sl_uint32 n) = 0;

	virtual void sendMessage(String targetId, sl_uint64 messageId, const void* data, sl_uint32 n) = 0;

	virtual Time getServiceTime() = 0;

public:
	SLIB_PROPERTY_INLINE(String, name)

public:
	SLIB_INLINE const Ptr<IHnsClientListener>& getListener()
	{
		return m_listener;
	}
	SLIB_INLINE void setListener(const Ptr<IHnsClientListener>& listener)
	{
		m_listener = listener;
	}

	SLIB_INLINE const Ref<Socket>& getSocket()
	{
		return m_socket;
	}

	SLIB_INLINE const String& getServiceName()
	{
		return m_name;
	}
	SLIB_INLINE const SocketAddress& getServiceAddress()
	{
		return m_serviceAddress;
	}
	SLIB_INLINE const RSAPublicKey& getServicePublicKey()
	{
		return m_servicePublicKey;
	}

	SLIB_INLINE String getClientId()
	{
		return m_clientId;
	}
	SLIB_INLINE String getClientCertificate()
	{
		return m_clientCertificate;
	}

	SLIB_INLINE sl_uint32 getConnectionTimeoutMilliseconds()
	{
		return m_connectionTimeoutMilliseconds;
	}
	SLIB_INLINE void setConnectionTimeoutMilliseconds(sl_uint32 millis)
	{
		m_connectionTimeoutMilliseconds = millis;
	}
	SLIB_INLINE sl_uint32 getKeepAliveIntervalMilliseconds()
	{
		return m_keepAliveIntervalMilliseconds;
	}
	SLIB_INLINE void setKeepAliveIntervalMilliseconds(sl_uint32 millis)
	{
		m_keepAliveIntervalMilliseconds = millis;
	}

	SLIB_INLINE const SocketAddress& getPublicAddress()
	{
		return m_publicAddress;
	}

public:
	SLIB_INLINE Time getLastCommunicatedTime()
	{
		return m_timeLastCommunicated;
	}

protected:
	Ptr<IHnsClientListener> m_listener;
		
	Ref<Socket> m_socket;
	Time m_timeLastCommunicated;

	String m_name;
	SocketAddress m_serviceAddress;
	RSAPublicKey m_servicePublicKey;

	String m_clientId;
	String m_clientCertificate;

	sl_uint32 m_connectionTimeoutMilliseconds;
	sl_uint32 m_keepAliveIntervalMilliseconds;

	SocketAddress m_publicAddress;

public:
	static Ref<HnsClient> createUdp(const HnsClientParam& param);

};

SLIB_P2P_NAMESPACE_END

#endif

