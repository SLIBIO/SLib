#include "../../../inc/slib/network/nat.h"

SLIB_NETWORK_NAMESPACE_BEGIN

NatTable::NatTable()
{
	m_addressTarget.setZero();
	m_portStart = 40000;
	m_portEnd = 65000;
	m_portTcpCurrent = 0;
	m_portUdpCurrent = 0;

	m_icmpEchoIdentifierTarget = 30000;
	m_icmpEchoSequenceCurrent = 0;
}

sl_bool NatTable::translateOutgoingPacket(IPv4HeaderFormat* ipHeader, void* ipContent, sl_uint32 sizeContent)
{
	IPv4Address addressTarget = m_addressTarget;
	if (addressTarget.isAny()) {
		return sl_false;
	}
	if (ipHeader->isTCP()) {
		TcpHeaderFormat* tcp = (TcpHeaderFormat*)(ipContent);
		if (tcp->check(ipHeader, sizeContent)) {
			sl_uint32 targetPort = getMappedTcpTargetPort(SocketAddress(ipHeader->getSourceAddress(), tcp->getSourcePort()));
			tcp->setSourcePort(targetPort);
			ipHeader->setSourceAddress(addressTarget);
			tcp->updateChecksum(ipHeader, sizeContent);
			ipHeader->updateChecksum();
			return sl_true;
		}
	} else if (ipHeader->isUDP()) {
		UdpHeaderFormat* udp = (UdpHeaderFormat*)(ipContent);
		if (udp->check(ipHeader, sizeContent)) {
			sl_uint32 targetPort = getMappedUdpTargetPort(SocketAddress(ipHeader->getSourceAddress(), udp->getSourcePort()));
			udp->setSourcePort(targetPort);
			ipHeader->setSourceAddress(addressTarget);
			udp->updateChecksum(ipHeader);
			ipHeader->updateChecksum();
			return sl_true;
		}
	} else if (ipHeader->isICMP()) {
		IcmpHeaderFormat* icmp = (IcmpHeaderFormat*)(ipContent);
		if (icmp->check(sizeContent)) {
			if (icmp->getType() == icmpType_Echo) {
				IcmpEchoAddress address;
				address.ip = ipHeader->getSourceAddress();
				address.identifier = icmp->getEchoIdentifier();
				address.sequenceNumber = icmp->getEchoSequenceNumber();
				sl_uint16 sn = getMappedIcmpEchoSequenceNumber(address);
				icmp->setEchoIdentifier(m_icmpEchoIdentifierTarget);
				icmp->setEchoSequenceNumber(sn);
				ipHeader->setSourceAddress(addressTarget);
				icmp->updateChecksum(sizeContent);
				ipHeader->updateChecksum();
				return sl_true;
			}
		}
	}
	return sl_false;
}

sl_bool NatTable::translateIncomingPacket(IPv4HeaderFormat* ipHeader, void* ipContent, sl_uint32 sizeContent)
{
	IPv4Address addressTarget = m_addressTarget;
	if (addressTarget.isAny()) {
		return sl_false;
	}
	if (ipHeader->getDestinationAddress() != addressTarget) {
		return sl_false;
	}
	if (ipHeader->isTCP()) {
		TcpHeaderFormat* tcp = (TcpHeaderFormat*)(ipContent);
		if (tcp->check(ipHeader, sizeContent)) {
			MappingElement element;
			if (m_mapTcpIncoming.get(tcp->getDestinationPort(), &element)) {
				ipHeader->setDestinationAddress(element.addressSource.ip.getIPv4());
				tcp->setDestinationPort(element.addressSource.port);
				tcp->updateChecksum(ipHeader, sizeContent);
				ipHeader->updateChecksum();
				return sl_true;
			}
		}
	} else if (ipHeader->isUDP()) {
		UdpHeaderFormat* udp = (UdpHeaderFormat*)(ipHeader->getContent());
		if (udp->check(ipHeader, sizeContent)) {
			MappingElement element;
			if (m_mapUdpIncoming.get(udp->getDestinationPort(), &element)) {
				ipHeader->setDestinationAddress(element.addressSource.ip.getIPv4());
				udp->setDestinationPort(element.addressSource.port);
				udp->updateChecksum(ipHeader);
				ipHeader->updateChecksum();
				return sl_true;
			}
		}
	} else if (ipHeader->isICMP()) {
		IcmpHeaderFormat* icmp = (IcmpHeaderFormat*)(ipContent);
		if (icmp->check(sizeContent)) {
			sl_uint8 type = icmp->getType();
			if (type == icmpType_EchoReply) {
				if (icmp->getEchoIdentifier() == m_icmpEchoIdentifierTarget) {
					IcmpEchoElement element;
					if (m_mapIcmpEchoIncoming.get(icmp->getEchoSequenceNumber(), &element)) {
						ipHeader->setDestinationAddress(element.addressSource.ip);
						icmp->setEchoIdentifier(element.addressSource.identifier);
						icmp->setEchoSequenceNumber(element.addressSource.sequenceNumber);
						icmp->updateChecksum(sizeContent);
						ipHeader->updateChecksum();
						return sl_true;
					}
				}
			} else if (type == icmpType_DestinationUnreachable || type == icmpType_TimeExceeded) {
				IPv4HeaderFormat* ipOrig = (IPv4HeaderFormat*)(icmp->getContent());
				sl_uint32 sizeOrig = sizeContent - sizeof(IcmpHeaderFormat);
				if (sizeOrig == sizeof(IPv4HeaderFormat)+8 && IPv4HeaderFormat::checkHeader(ipOrig, sizeOrig) && ipOrig->getDestinationAddress() == addressTarget) {
					if (ipOrig->isTCP()) {
						TcpHeaderFormat* tcp = (TcpHeaderFormat*)(ipOrig->getContent());
						MappingElement element;
						if (m_mapTcpIncoming.get(tcp->getDestinationPort(), &element)) {
							ipOrig->setDestinationAddress(element.addressSource.ip.getIPv4());
							tcp->setDestinationPort(element.addressSource.port);
							ipOrig->updateChecksum();
							ipHeader->setDestinationAddress(element.addressSource.ip.getIPv4());
							icmp->updateChecksum(sizeContent);
							ipHeader->updateChecksum();
							return sl_true;
						}
					} else if (ipOrig->isUDP()) {
						UdpHeaderFormat* udp = (UdpHeaderFormat*)(ipOrig->getContent());
						MappingElement element;
						if (m_mapUdpIncoming.get(udp->getDestinationPort(), &element)) {
							ipOrig->setDestinationAddress(element.addressSource.ip.getIPv4());
							udp->setDestinationPort(element.addressSource.port);
							udp->setChecksum(0);
							ipOrig->updateChecksum();
							ipHeader->setDestinationAddress(element.addressSource.ip.getIPv4());
							icmp->updateChecksum(sizeContent);
							ipHeader->updateChecksum();
							return sl_true;
						}
					}
				}
			}
		}
	}
	return sl_false;
}

sl_uint32 NatTable::getMappedTcpTargetPort(const SocketAddress& address)
{
	MappingElement element;
	if (m_mapTcpOutgoing.get(address, &element)) {
		return element.portTarget;
	}
	sl_uint32 port = ++m_portTcpCurrent;
	if (port < m_portStart || port > m_portEnd) {
		port = m_portStart;
	}
	m_portTcpCurrent = port;
	if (m_mapTcpIncoming.get(port, &element)) {
		m_mapTcpOutgoing.removeAllMatchingKeys(element.addressSource);
	}
	element.addressSource = address;
	element.portTarget = port;
	m_mapTcpOutgoing.put(address, element);
	m_mapTcpIncoming.put(port, element);
	return port;
}

sl_uint32 NatTable::getMappedUdpTargetPort(const SocketAddress& address)
{
	MappingElement element;
	if (m_mapUdpOutgoing.get(address, &element)) {
		return element.portTarget;
	}
	sl_uint32 port = ++m_portUdpCurrent;
	if (port < m_portStart || port > m_portEnd) {
		port = m_portStart;
	}
	m_portUdpCurrent = port;
	if (m_mapUdpIncoming.get(port, &element)) {
		m_mapUdpOutgoing.removeAllMatchingKeys(element.addressSource);
	}
	element.addressSource = address;
	element.portTarget = port;
	m_mapUdpOutgoing.put(address, element);
	m_mapUdpIncoming.put(port, element);
	return port;
}

sl_uint16 NatTable::getMappedIcmpEchoSequenceNumber(const IcmpEchoAddress& address)
{
	IcmpEchoElement element;
	if (m_mapIcmpEchoOutgoing.get(address, &element)) {
		return element.sequenceNumberTarget;
	}
	sl_uint16 sn = ++ m_icmpEchoSequenceCurrent;
	if (m_mapIcmpEchoIncoming.get(sn, &element)) {
		m_mapIcmpEchoOutgoing.removeAllMatchingKeys(element.addressSource);
	}
	element.addressSource = address;
	element.sequenceNumberTarget = sn;
	m_mapIcmpEchoOutgoing.put(address, element);
	m_mapIcmpEchoIncoming.put(sn, element);
	return sn;
}

SLIB_NETWORK_NAMESPACE_END
