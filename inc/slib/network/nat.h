#ifndef CHECKHEADER_SLIB_NETWORK_NAT
#define CHECKHEADER_SLIB_NETWORK_NAT

#include "definition.h"

#include "socket.h"
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

namespace slib
{
	class _NatTablePort
	{
	public:
		sl_bool flagActive;
		SocketAddress addressSource;
		Time timeLastAccess;
		
	public:
		_NatTablePort();
		
		~_NatTablePort();
		
	};
	
	class _NatTableMapping : public Object
	{
	public:
		_NatTableMapping();
		
		~_NatTableMapping();
		
	public:
		void setup(sl_uint16 portBegin, sl_uint16 portEnd);
		
		sl_bool mapToExternalPort(const SocketAddress& address, sl_uint16& port);
		
		sl_bool mapToInternalAddress(sl_uint16 port, SocketAddress& address);
		
	protected:
		HashMap< SocketAddress, sl_uint16 > m_mapPorts;
		
		_NatTablePort* m_ports;
		sl_uint16 m_nPorts;
		sl_uint16 m_pos;
		
		sl_uint16 m_portBegin;
		sl_uint16 m_portEnd;
		
	};
	
	class SLIB_EXPORT NatTableParam
	{
	public:
		IPv4Address targetAddress;
		
		sl_uint16 tcpPortBegin;
		sl_uint16 tcpPortEnd;
		
		sl_uint16 udpPortBegin;
		sl_uint16 udpPortEnd;
		
		sl_uint16 icmpEchoIdentifier;
		
	public:
		NatTableParam();
		
		~NatTableParam();
		
	};
	
	class SLIB_EXPORT NatTable : public Object
	{
	public:
		NatTable();
		
		~NatTable();
		
	public:
		const NatTableParam& getParam() const;
		
		void setup(const NatTableParam& param);
		
	public:
		sl_bool translateOutgoingPacket(IPv4Packet* ipHeader, void* ipContent, sl_uint32 sizeContent);
		
		sl_bool translateIncomingPacket(IPv4Packet* ipHeader, void* ipContent, sl_uint32 sizeContent);
		
		sl_uint16 getMappedIcmpEchoSequenceNumber(const IcmpEchoAddress& address);
		
	protected:
		NatTableParam m_param;
		
		_NatTableMapping m_mappingTcp;
		
		_NatTableMapping m_mappingUdp;
		
		sl_uint16 m_icmpEchoSequenceCurrent;
		
		struct IcmpEchoElement
		{
			IcmpEchoAddress addressSource;
			sl_uint16 sequenceNumberTarget;
		};
		
		HashMap<IcmpEchoAddress, IcmpEchoElement> m_mapIcmpEchoOutgoing;
		HashMap<sl_uint32, IcmpEchoElement> m_mapIcmpEchoIncoming;
		
	};	
}

#endif
