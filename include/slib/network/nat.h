/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

#ifndef CHECKHEADER_SLIB_NETWORK_NAT
#define CHECKHEADER_SLIB_NETWORK_NAT

#include "definition.h"

#include "socket.h"
#include "tcpip.h"
#include "icmp.h"
#include "socket_address.h"

#include "../core/object.h"
#include "../core/hash_map.h"

/*
	If you are usiing kernel-mode NAT on linux (for example on port range 40000~60000), following configuration will avoid to conflict with kernel-networking.
		
		iptables -A INPUT -p tcp --dport 40000:60000 -j DROP
		sysctl -w net.ipv4.ip_local_port_range="30000 39000"
*/ 

namespace slib
{

	class _priv_NatTablePort
	{
	public:
		sl_bool flagActive;
		SocketAddress addressSource;
		Time timeLastAccess;
		
	public:
		_priv_NatTablePort();
		
		~_priv_NatTablePort();
		
	};
	
	class _priv_NatTableMapping : public Object
	{
	public:
		_priv_NatTableMapping();
		
		~_priv_NatTableMapping();
		
	public:
		void setup(sl_uint16 portBegin, sl_uint16 portEnd);
		
		sl_bool mapToExternalPort(const SocketAddress& address, sl_uint16& port);
		
		sl_bool mapToInternalAddress(sl_uint16 port, SocketAddress& address);
		
	protected:
		CHashMap< SocketAddress, sl_uint16 > m_mapPorts;
		
		_priv_NatTablePort* m_ports;
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
		
		_priv_NatTableMapping m_mappingTcp;
		
		_priv_NatTableMapping m_mappingUdp;
		
		sl_uint16 m_icmpEchoSequenceCurrent;
		
		struct IcmpEchoElement
		{
			IcmpEchoAddress addressSource;
			sl_uint16 sequenceNumberTarget;
		};
		
		CHashMap<IcmpEchoAddress, IcmpEchoElement> m_mapIcmpEchoOutgoing;
		CHashMap<sl_uint32, IcmpEchoElement> m_mapIcmpEchoIncoming;
		
	};

}

#endif
