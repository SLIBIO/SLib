#include "../../../inc/slib/network/ethernet.h"

#include "../../../inc/slib/network/tcpip.h"

SLIB_NETWORK_NAMESPACE_BEGIN

EthernetMacTable::EthernetMacTable()
{
}

EthernetMacTable::~EthernetMacTable()
{
}

void EthernetMacTable::add(const IPv4Address& ip, const MacAddress& mac)
{
	m_table.put(ip, mac);
}

sl_bool EthernetMacTable::getMacAddress(const IPv4Address& ip, MacAddress* out)
{
	if (ip.isBroadcast()) {
		if (out) {
			out->setBroadcast();
		}
		return sl_true;
	}
	if (ip.isMulticast()) {
		if (out) {
			out->makeMulticast(ip);
		}
		return sl_true;
	}
	return m_table.get(ip, out);
}

void EthernetMacTable::parseEthernetFrame(const void* _frame, sl_uint32 sizeFrame, sl_bool flagUseSource, sl_bool flagUseDestination)
{
	if (sizeFrame > EthernetFrame::HeaderSize) {
		EthernetFrame* frame = (EthernetFrame*)_frame;
		if (frame->getProtocol() == NetworkLinkProtocol::IPv4) {
			IPv4Packet* ip = (IPv4Packet*)(frame->getContent());
			sl_uint32 sizeIP = sizeFrame - EthernetFrame::HeaderSize;
			if (IPv4Packet::check(ip, sizeIP)) {
				if (flagUseSource) {
					MacAddress mac = frame->getSourceAddress();
					if (mac.isNotMulticast()) {
						IPv4Address addr = ip->getSourceAddress();
						if (addr.isHost()) {
							add(addr, mac);
						}
					}
				}
				if (flagUseDestination) {
					MacAddress mac = frame->getDestinationAddress();
					if (mac.isNotMulticast()) {
						IPv4Address addr = ip->getDestinationAddress();
						if (addr.isHost()) {
							add(addr, mac);
						}
					}
				}
			}
		}
	}
}

SLIB_NETWORK_NAMESPACE_END
