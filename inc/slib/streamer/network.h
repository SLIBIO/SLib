#ifndef CHECKHEADER_SLIB_STREAMER_NETWORK
#define CHECKHEADER_SLIB_STREAMER_NETWORK

#include "definition.h"

#include "graph.h"
#include "../network/socket.h"

SLIB_STREAMER_NAMESPACE_BEGIN

class SLIB_EXPORT NetworkUdpSource : public Source
{
protected:
	SLIB_INLINE NetworkUdpSource()
	{
	}

public:
	SLIB_PROPERTY_INLINE(Ref<Socket>, Socket)

public:
	static Ref<NetworkUdpSource> create(const Ref<Socket>& socket);
	static Ref<NetworkUdpSource> create(SocketAddress addressBind, sl_bool flagBroadcast = sl_false);
	static Ref<NetworkUdpSource> createMulticast(SocketAddress addressBind, const IPv4Address& group);
};

class SLIB_EXPORT NetworkUdpSink : public Sink
{
private:
	SLIB_INLINE NetworkUdpSink()
	{
	}

public:
	sl_bool sendPacket(const Packet& packet);

	virtual sl_bool resolveTarget(const Packet& packet, SocketAddress& target);

public:
	SLIB_PROPERTY_INLINE(Ref<Socket>, Socket)
	SLIB_PROPERTY_INLINE(SocketAddress, DefaultTarget)

public:
	static Ref<NetworkUdpSink> create(const Ref<Socket>& socket);
	static Ref<NetworkUdpSink> create(SocketAddress addressBind, SocketAddress defaultTarget, sl_bool flagBroadcast = sl_false);
	static Ref<NetworkUdpSink> createMulticast(SocketAddress addressBind, SocketAddress defaultTarget, const IPv4Address& group);

};

SLIB_STREAMER_NAMESPACE_END

#endif
