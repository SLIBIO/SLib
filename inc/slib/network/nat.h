#ifndef CHECKHEADER_SLIB_NETWORK_NAT
#define CHECKHEADER_SLIB_NETWORK_NAT

#include "definition.h"

#include "tcpip.h"
#include "icmp.h"
#include "socket_address.h"

#include "../core/object.h"
#include "../core/map.h"

/*
	If you are usiing kernel-mode NAT on linux (for example on port range 40000~60000), following configuration will avoid to conflict with kernel-networking.
		
		iptables -A INPUT -p tcp --dport 40000:60000 -j DROP
		sysctl -w net.ipv4.ip_local_port_range="30000 39000"
*/ 

SLIB_NETWORK_NAMESPACE_BEGIN

class SLIB_EXPORT NatTable : public Object
{
public:
	NatTable();
	~NatTable();

public:
	SLIB_INLINE const IPv4Address& getTargetAddress() const
	{
		return m_addressTarget;
	}
	SLIB_INLINE void setTargetAddress(const IPv4Address& targetAddress)
	{
		m_addressTarget = targetAddress;
	}

	SLIB_INLINE sl_uint32 getTargetPortBegin() const
	{
		return m_portStart;
	}
	SLIB_INLINE void setTargetPortBegin(sl_uint32 port)
	{
		m_portStart = port;
	}

	SLIB_INLINE sl_uint32 getTargetPortEnd() const
	{
		return m_portEnd;
	}
	SLIB_INLINE void setTargetPortEnd(sl_uint32 port)
	{
		m_portEnd = port;
	}

	SLIB_INLINE sl_uint16 getTargetIcmpEchoIdentifier() const
	{
		return m_icmpEchoIdentifierTarget;
	}
	SLIB_INLINE void setTargetIcmpEchoIdentifier(sl_uint16 id)
	{
		m_icmpEchoIdentifierTarget = id;
	}

public:
	sl_bool translateOutgoingPacket(IPv4HeaderFormat* ipHeader, void* ipContent, sl_uint32 sizeContent);
	sl_bool translateIncomingPacket(IPv4HeaderFormat* ipHeader, void* ipContent, sl_uint32 sizeContent);

	sl_uint32 getMappedTcpTargetPort(const SocketAddress& address);
	sl_uint32 getMappedUdpTargetPort(const SocketAddress& address);
	sl_uint16 getMappedIcmpEchoSequenceNumber(const IcmpEchoAddress& address);

protected:
	IPv4Address m_addressTarget;
	sl_uint32 m_portStart;
	sl_uint32 m_portEnd;
	sl_uint32 m_portTcpCurrent;
	sl_uint32 m_portUdpCurrent;

	sl_uint16 m_icmpEchoIdentifierTarget;
	sl_uint16 m_icmpEchoSequenceCurrent;

	struct MappingElement
	{
		SocketAddress addressSource;
		sl_uint32 portTarget;
	};
	HashMap<SocketAddress, MappingElement> m_mapTcpOutgoing;
	HashMap<sl_uint32, MappingElement> m_mapTcpIncoming;
	HashMap<SocketAddress, MappingElement> m_mapUdpOutgoing;
	HashMap<sl_uint32, MappingElement> m_mapUdpIncoming;

	struct IcmpEchoElement
	{
		IcmpEchoAddress addressSource;
		sl_uint16 sequenceNumberTarget;
	};
	HashMap<IcmpEchoAddress, IcmpEchoElement> m_mapIcmpEchoOutgoing;
	HashMap<sl_uint32, IcmpEchoElement> m_mapIcmpEchoIncoming;
};

SLIB_NETWORK_NAMESPACE_END

#endif
