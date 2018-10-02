/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/network/tcpip.h"

#include "slib/core/mio.h"

namespace slib
{

	sl_uint16 TCP_IP::calculateOneComplementSum(const void* data, sl_size size, sl_uint32 add)
	{
		sl_uint32 sum = add;
		sl_uint8* p = (sl_uint8*)data;
		while (size > 1) {
			sl_uint16 w = *((sl_uint8*)(p));
			w = (w << 8) | *((sl_uint8*)(p + 1));
			sum += w; // can be computed in either byte order
			p += 2;
			size -= 2;
		}
		if (size) {
			sl_uint16 w = *((sl_uint8*)p);
			w = w << 8;
			sum += w;
		}
		while (sum >> 16) {
			sum = (sum >> 16) + (sum & 0xffff); // 1's complement sum
		}
		return sum;
	}
	
	// Referenced from RFC 1071
	sl_uint16 TCP_IP::calculateChecksum(const void* data, sl_size size)
	{
		sl_uint16 sum = TCP_IP::calculateOneComplementSum(data, size);
		return (sl_uint16)(~sum); // 1's complement
	}
	
	
	void IPv4Packet::updateChecksum()
	{
		_headerChecksum[0] = 0;
		_headerChecksum[1] = 0;
		setChecksum(TCP_IP::calculateChecksum(this, getHeaderSize()));
	}
	
	sl_bool IPv4Packet::checkChecksum() const
	{
		if ((_headerChecksum[0] | _headerChecksum[1]) == 0) {
			return sl_true;
		}
		sl_uint16 checksum = TCP_IP::calculateChecksum(this, getHeaderSize());
		return checksum == 0;
	}
	
	sl_uint16 IPv4Packet::getChecksumForContent(const void* content, sl_uint16 sizeContent) const
	{
		sl_uint32 sum = 0;
		sum = MIO::readUint16BE(_sourceIp);
		sum += MIO::readUint16BE(_sourceIp + 2);
		sum += MIO::readUint16BE(_destinationIp);
		sum += MIO::readUint16BE(_destinationIp + 2);
		sum += _protocol;
		sum += sizeContent;
		sum = TCP_IP::calculateOneComplementSum(content, sizeContent, sum);
		return (sl_uint16)(~sum);
	}
	
	sl_bool IPv4Packet::check(const void* packet, sl_size sizePacket)
	{
		if (!(checkHeader(packet, sizePacket))) {
			return sl_false;
		}
		const IPv4Packet* header = (const IPv4Packet*)(packet);
		if (header->getTotalSize() > sizePacket) {
			return sl_false;
		}
		return sl_true;
	}
	
	sl_bool IPv4Packet::checkHeader(const void* packet, sl_size sizePacket)
	{
		if (!packet) {
			return sl_false;
		}
		if (sizePacket < HeaderSizeBeforeOptions) {
			return sl_false;
		}
		const IPv4Packet* header = (const IPv4Packet*)(packet);
		if (header->getVersion() != 4) {
			return sl_false;
		}
		sl_uint8 sizeHeader = header->getHeaderSize();
		if (sizeHeader > sizePacket) {
			return sl_false;
		}
		if (sizeHeader > header->getTotalSize()) {
			return sl_false;
		}
		if (!(header->checkChecksum())) {
			return sl_false;
		}
		return sl_true;
	}
	
	sl_bool IPv4Packet::getPortsForTcpUdp(sl_uint16& src, sl_uint16& dst) const
	{
		NetworkInternetProtocol protocol = getProtocol();
		if (protocol == NetworkInternetProtocol::TCP || protocol == NetworkInternetProtocol::UDP) {
			if (getContentSize() > 4) {
				const sl_uint8* p = getContent();
				src = MIO::readUint16BE(p);
				dst = MIO::readUint16BE(p + 2);
				return sl_true;
			}
		}
		return sl_false;
	}
	
	
	sl_uint16 IPv6Packet::getChecksumForContent(const void* content, sl_uint32 sizeContent) const
	{
		sl_uint32 sum = 0;
		int i;
		for (i = 0; i < 16; i += 2) {
			sum += MIO::readUint16BE(_sourceAddress + i);
		}
		for (i = 0; i < 16; i += 2) {
			sum += MIO::readUint16BE(_destinationAddress + i);
		}
		sum += (sl_uint16)(sizeContent >> 16);
		sum += (sl_uint16)(sizeContent);
		sum += _nextHeader;
		sum = TCP_IP::calculateOneComplementSum(content, sizeContent, sum);
		return (sl_uint16)(~sum);
	}
	
	sl_bool IPv6Packet::check(const void* packet, sl_size sizePacket)
	{
		if (!(checkHeader(packet, sizePacket))) {
			return sl_false;
		}
		const IPv6Packet* header = (const IPv6Packet*)(packet);
		if (HeaderSize + (sl_size)(header->getPayloadLength()) > sizePacket) {
			return sl_false;
		}
		return sl_true;
	}
	
	sl_bool IPv6Packet::checkHeader(const void* packet, sl_size sizePacket)
	{
		if (!packet) {
			return sl_false;
		}
		if (sizePacket < HeaderSize) {
			return sl_false;
		}
		const IPv6Packet* header = (const IPv6Packet*)(packet);
		if (header->getVersion() != 6) {
			return sl_false;
		}
		return sl_true;
	}
	
	
	sl_bool TcpSegment::checkSize(sl_size sizeTcp) const
	{
		if (sizeTcp < HeaderSizeBeforeOptions) {
			return sl_false;
		}
		if (sizeTcp < getHeaderSize()) {
			return sl_false;
		}
		return sl_true;
	}
	
	void TcpSegment::updateChecksum(const IPv4Packet* ipv4, sl_size sizeTcp)
	{
		_checksum[0] = 0;
		_checksum[1] = 0;
		sl_uint16 checksum = ipv4->getChecksumForContent(this, (sl_uint16)sizeTcp);
		MIO::writeUint16BE(_checksum, checksum);
	}
	
	sl_bool TcpSegment::checkChecksum(const IPv4Packet* ipv4, sl_size sizeTcp) const
	{
		sl_uint16 checksum = ipv4->getChecksumForContent(this, (sl_uint16)sizeTcp);
		return checksum == 0;
	}
	
	sl_bool TcpSegment::check(IPv4Packet* ip, sl_size sizeTcp) const
	{
		return checkSize(sizeTcp) && checkChecksum(ip, sizeTcp);
	}
	
	
	sl_bool UdpDatagram::checkSize(sl_size sizeUdp) const
	{
		if (sizeUdp < HeaderSize) {
			return sl_false;
		}
		if (sizeUdp != getTotalSize()) {
			return sl_false;
		}
		return sl_true;
	}
	
	void UdpDatagram::updateChecksum(const IPv4Packet* ipv4)
	{
		_checksum[0] = 0;
		_checksum[1] = 0;
		sl_uint16 checksum = ipv4->getChecksumForContent(this, getTotalSize());
		if (checksum == 0) {
			checksum = 0xFFFF;
		}
		setChecksum(checksum);
	}
	
	sl_bool UdpDatagram::checkChecksum(const IPv4Packet* ipv4) const
	{
		if ((_checksum[0] | _checksum[1]) == 0) {
			return sl_true;
		}
		sl_uint16 checksum = ipv4->getChecksumForContent(this, getTotalSize());
		return checksum == 0 || checksum == 0xFFFF;
	}
	
	sl_bool UdpDatagram::check(IPv4Packet* ip, sl_size sizeUdp) const
	{
		return checkSize(sizeUdp) && checkChecksum(ip);
	}
	
	
	sl_bool IPv4PacketIdentifier::operator==(const IPv4PacketIdentifier& other) const
	{
		return source == other.source && destination == other.destination && identification == other.identification && protocol == other.protocol;
	}
	
	sl_size IPv4PacketIdentifier::hashCode() const
	{
		sl_uint64 t = source.hashCode();
		t *= 961;
		t += destination.hashCode();
		t *= 31;
		t += SLIB_MAKE_DWORD2(identification, protocol);
		return Rehash64ToSize(t);
	}
	
	int Compare<IPv4PacketIdentifier>::operator()(const IPv4PacketIdentifier& a, const IPv4PacketIdentifier& b) const
	{
		sl_size _a = a.hashCode();
		sl_size _b = b.hashCode();
		return (_a < _b) ? -1 : (_a > _b);
	}

	sl_size Hash<IPv4PacketIdentifier>::operator()(const IPv4PacketIdentifier& v) const
	{
		return v.hashCode();
	}
	
	
	IPv4Fragment::IPv4Fragment()
	{
	}
	
	IPv4Fragment::~IPv4Fragment()
	{
	}
	
	IPv4FragmentedPacket::IPv4FragmentedPacket()
	{
	}
	
	IPv4FragmentedPacket::~IPv4FragmentedPacket()
	{
	}
	
	IPv4Fragmentation::IPv4Fragmentation()
	{
	}
	
	IPv4Fragmentation::~IPv4Fragmentation()
	{
	}
	
	void IPv4Fragmentation::setupExpiringDuration(sl_uint32 ms, const Ref<DispatchLoop>& loop)
	{
		m_packets.setupTimer(ms, loop);
	}
	
	void IPv4Fragmentation::setupExpiringDuration(sl_uint32 ms)
	{
		m_packets.setupTimer(ms);
	}
	
	sl_bool IPv4Fragmentation::isNeededReassembly(const IPv4Packet* ip)
	{
		if (ip->getFragmentOffset() == 0 && !(ip->isMF())) {
			return sl_false;
		}
		return sl_true;
	}
	
	Memory IPv4Fragmentation::reassemble(const IPv4Packet* ip)
	{
		if (ip->getFragmentOffset() == 0 && !(ip->isMF())) {
			return Memory::create(ip, ip->getTotalSize());
		}
		
		sl_uint8* data = (sl_uint8*)(ip->getContent());
		sl_uint16 sizeContent = ip->getContentSize();
		sl_uint16 offset = ip->getFragmentOffset() << 3;
		if ((sl_uint32)offset + (sl_uint32)sizeContent > 0xFFFF) {
			return sl_null;
		}

		IPv4PacketIdentifier id;
		id.source = ip->getSourceAddress();
		id.destination = ip->getDestinationAddress();
		id.identification = ip->getIdentification();
		id.protocol = ip->getProtocol();
		
		ObjectLocker lock(this);
		
		Ref<IPv4FragmentedPacket> packet;
		if (offset == 0) {
			packet = new IPv4FragmentedPacket;
			if (packet.isNull()) {
				return sl_null;
			}
			packet->header = Memory::create(ip, ip->getHeaderSize());
			if (packet->header.isNull()) {
				return sl_null;
			}
			if (!(m_packets.put(id, packet))) {
				return sl_null;
			}
		} else {
			m_packets.get(id, &packet);
			if (packet.isNull()) {
				return sl_null;
			}
		}
		
		if (ip->isMF()) {
			IPv4Fragment fragment;
			fragment.offset = offset;
			fragment.data = Memory::create(data, sizeContent);
			packet->fragments.add_NoLock(fragment);
			return sl_null;
		}
		
		m_packets.remove(id);
		
		sl_uint16 sizeHeader = (sl_uint16)(packet->header.getSize());
		sl_uint16 sizeTotal = offset + sizeContent;
		if ((sl_uint32)sizeHeader + (sl_uint32)sizeTotal > 0xFFFF) {
			return sl_null;
		}
		
		Memory mem = Memory::create(sizeHeader + sizeTotal);
		if (mem.isNull()) {
			return sl_null;
		}
		
		IPv4Packet* headerTotal = (IPv4Packet*)(mem.getData());
		sl_uint8* dataTotal = (sl_uint8*)(headerTotal) + sizeHeader;

		Base::copyMemory(headerTotal, packet->header.getData(), sizeHeader);
		
		ListElements<IPv4Fragment> fragments(packet->fragments);
		for (sl_size i = 0; i < fragments.count; i++) {
			sl_size sizeFragment = fragments[i].data.getSize();
			if (fragments[i].offset + sizeFragment <= sizeTotal) {
				Base::copyMemory(dataTotal + fragments[i].offset, fragments[i].data.getData(), sizeFragment);
			}
		}
		Base::copyMemory(dataTotal + offset, data, sizeContent);

		headerTotal->setMF(sl_false);
		headerTotal->setTotalSize(sizeHeader + sizeTotal);
		headerTotal->updateChecksum();

		return mem;
	}
	
	sl_bool IPv4Fragmentation::isNeededFragmentation(const IPv4Packet* header, sl_uint16 mtu)
	{
		if (header->isDF()) {
			return sl_false;
		}
		if (header->getTotalSize() <= mtu) {
			return sl_false;
		}
		return sl_true;
	}
	
	List<Memory> IPv4Fragmentation::makeFragments(const IPv4Packet* header, sl_uint16 mtu)
	{
		sl_uint8 sizeHeader = header->getHeaderSize();
		sl_uint16 sizeContent = header->getContentSize();
		
		if (sizeContent == 0) {
			return sl_null;
		}
		if (sizeHeader + 8 > mtu) {
			return sl_null;
		}
		
		sl_uint8* data = (sl_uint8*)(header->getContent());
		
		sl_uint16 sizeFragment = mtu - sizeHeader;
		sizeFragment = (sizeFragment & 0xFFF8);
		
		sl_uint16 offsetOriginal = header->getFragmentOffset();
		sl_bool flagMFOriginal = header->isMF();
		
		List<Memory> ret;
		sl_uint16 offset = 0;
		
		while (offset < sizeContent) {
			
			sl_uint16 n = sizeFragment;
			if ((sl_uint32)offset + (sl_uint32)n > sizeContent) {
				n = sizeContent - offset;
			}
			
			Memory mem = Memory::create(sizeHeader + n);
			if (mem.isNull()) {
				return sl_null;
			}
			sl_uint8* buf = (sl_uint8*)(mem.getData());
			Base::copyMemory(buf, header, sizeHeader);
			Base::copyMemory(buf + sizeHeader, data + offset, n);
			
			IPv4Packet* h = (IPv4Packet*)buf;
			h->setTotalSize(sizeHeader + n);
			h->setDF(sl_false);
			h->setFragmentOffset(offsetOriginal + (offset >> 3));
			offset += sizeFragment;
			h->setMF(offset < sizeContent ? sl_true : flagMFOriginal);
			h->updateChecksum();
			
			ret.add_NoLock(mem);
		}
		return ret;
	}
	
}
