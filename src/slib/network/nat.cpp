#include "../../../inc/slib/network/nat.h"

SLIB_NETWORK_NAMESPACE_BEGIN
NAT_Table::NAT_Table()
{
	m_addressTarget.setZero();
	m_portStart = 40000;
	m_portEnd = 65000;
	m_portTcpCurrent = 0;
	m_portUdpCurrent = 0;

	m_icmpEchoIdentifierTarget = 30000;
	m_icmpEchoSequenceCurrent = 0;
}

NAT_Table::~NAT_Table()
{
}

sl_bool NAT_Table::translateOutgoingPacket(IPv4HeaderFormat* ipHeader, void* ipContent, sl_uint32 sizeContent)
{
	IPv4Address addressTarget = m_addressTarget;
	if (addressTarget.isAny()) {
		return sl_false;
	}
	if (ipHeader->isTCP()) {
		TCP_HeaderFormat* tcp = (TCP_HeaderFormat*)(ipContent);
		if (tcp->check(ipHeader, sizeContent)) {
			sl_uint32 targetPort = getMappedTcpTargetPort(SocketAddress(ipHeader->getSourceAddress(), tcp->getSourcePort()));
			tcp->setSourcePort(targetPort);
			ipHeader->setSourceAddress(addressTarget);
			tcp->updateChecksum(ipHeader, sizeContent);
			ipHeader->updateChecksum();
			return sl_true;
		}
	} else if (ipHeader->isUDP()) {
		UDP_HeaderFormat* udp = (UDP_HeaderFormat*)(ipContent);
		if (udp->check(ipHeader, sizeContent)) {
			sl_uint32 targetPort = getMappedUdpTargetPort(SocketAddress(ipHeader->getSourceAddress(), udp->getSourcePort()));
			udp->setSourcePort(targetPort);
			ipHeader->setSourceAddress(addressTarget);
			udp->updateChecksum(ipHeader);
			ipHeader->updateChecksum();
			return sl_true;
		}
	} else if (ipHeader->isICMP()) {
		ICMP_HeaderFormat* icmp = (ICMP_HeaderFormat*)(ipContent);
		if (icmp->check(sizeContent)) {
			if (icmp->getType() == ICMP_Type_Echo) {
				ICMP_EchoAddress address;
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

sl_bool NAT_Table::translateIncomingPacket(IPv4HeaderFormat* ipHeader, void* ipContent, sl_uint32 sizeContent)
{
	IPv4Address addressTarget = m_addressTarget;
	if (addressTarget.isAny()) {
		return sl_false;
	}
	if (ipHeader->getDestinationAddress() != addressTarget) {
		return sl_false;
	}
	if (ipHeader->isTCP()) {
		TCP_HeaderFormat* tcp = (TCP_HeaderFormat*)(ipContent);
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
		UDP_HeaderFormat* udp = (UDP_HeaderFormat*)(ipHeader->getContent());
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
		ICMP_HeaderFormat* icmp = (ICMP_HeaderFormat*)(ipContent);
		if (icmp->check(sizeContent)) {
			sl_uint8 type = icmp->getType();
			if (type == ICMP_Type_EchoReply) {
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
			} else if (type == ICMP_Type_DestinationUnreachable || type == ICMP_Type_TimeExceeded) {
				IPv4HeaderFormat* ipOrig = (IPv4HeaderFormat*)(icmp->getContent());
				sl_uint32 sizeOrig = sizeContent - sizeof(ICMP_HeaderFormat);
				if (sizeOrig == sizeof(IPv4HeaderFormat)+8 && IPv4HeaderFormat::checkHeader(ipOrig, sizeOrig) && ipOrig->getDestinationAddress() == addressTarget) {
					if (ipOrig->isTCP()) {
						TCP_HeaderFormat* tcp = (TCP_HeaderFormat*)(ipOrig->getContent());
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
						UDP_HeaderFormat* udp = (UDP_HeaderFormat*)(ipOrig->getContent());
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

sl_uint32 NAT_Table::getMappedTcpTargetPort(const SocketAddress& address)
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
		m_mapTcpOutgoing.removeAllKeys(element.addressSource);
	}
	element.addressSource = address;
	element.portTarget = port;
	m_mapTcpOutgoing.put(address, element);
	m_mapTcpIncoming.put(port, element);
	return port;
}

sl_uint32 NAT_Table::getMappedUdpTargetPort(const SocketAddress& address)
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
		m_mapUdpOutgoing.removeAllKeys(element.addressSource);
	}
	element.addressSource = address;
	element.portTarget = port;
	m_mapUdpOutgoing.put(address, element);
	m_mapUdpIncoming.put(port, element);
	return port;
}

sl_uint16 NAT_Table::getMappedIcmpEchoSequenceNumber(const ICMP_EchoAddress& address)
{
	IcmpEchoElement element;
	if (m_mapIcmpEchoOutgoing.get(address, &element)) {
		return element.sequenceNumberTarget;
	}
	sl_uint16 sn = ++ m_icmpEchoSequenceCurrent;
	if (m_mapIcmpEchoIncoming.get(sn, &element)) {
		m_mapIcmpEchoOutgoing.removeAllKeys(element.addressSource);
	}
	element.addressSource = address;
	element.sequenceNumberTarget = sn;
	m_mapIcmpEchoOutgoing.put(address, element);
	m_mapIcmpEchoIncoming.put(sn, element);
	return sn;
}
SLIB_NETWORK_NAMESPACE_END
