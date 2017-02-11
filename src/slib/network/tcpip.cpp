#include "../../../inc/slib/network/tcpip.h"

#include "../../../inc/slib/network/icmp.h"
#include "../../../inc/slib/core/mio.h"

SLIB_NETWORK_NAMESPACE_BEGIN

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


sl_uint32 IPv4Packet::getVersion() const
{
	return (_versionAndHeaderLength >> 4) & 0x0F;
}

void IPv4Packet::setVersion(sl_uint32 version)
{
	_versionAndHeaderLength = (sl_uint8)((_versionAndHeaderLength & 0x0F) | (version << 4));
}

sl_uint8 IPv4Packet::getTypeOfService() const
{
	return _TOS_DSCP_ECN;
}

void IPv4Packet::setTypeOfService(sl_uint8 TOS)
{
	_TOS_DSCP_ECN = TOS;
}

sl_uint32 IPv4Packet::getDSCP() const
{
	return ((_TOS_DSCP_ECN >> 2) & 0x3F);
}

void IPv4Packet::setDSCP(sl_uint32 DSCP)
{
	_TOS_DSCP_ECN = (sl_uint8)((_TOS_DSCP_ECN & 3) | ((DSCP & 0x3F) << 2));
}

sl_uint32 IPv4Packet::getECN() const
{
	return (_TOS_DSCP_ECN & 3);
}

void IPv4Packet::setECN(sl_uint32 ECN)
{
	_TOS_DSCP_ECN = (sl_uint8)((_TOS_DSCP_ECN & 0xFC) | (ECN & 3));
}

sl_uint16 IPv4Packet::getIdentification() const
{
	return MIO::readUint16BE(_identification);
}

void IPv4Packet::setIdentification(sl_uint16 identification)
{
	MIO::writeUint16BE(_identification, identification);
}

sl_bool IPv4Packet::isDF() const
{
	return (_flagsAndFlagmentOffset[0] & 0x40) != 0;
}

void IPv4Packet::setDF(sl_bool flag)
{
	_flagsAndFlagmentOffset[0] = (sl_uint8)((_flagsAndFlagmentOffset[0] & 0xBF) | (flag ? 0x40 : 0));
}

sl_bool IPv4Packet::isMF() const
{
	return (_flagsAndFlagmentOffset[0] & 0x20) != 0;
}

void IPv4Packet::setMF(sl_bool flag)
{
	_flagsAndFlagmentOffset[0] = (sl_uint8)((_flagsAndFlagmentOffset[0] & 0xDF) | (flag ? 0x20 : 0));
}

sl_uint32 IPv4Packet::getFragmentOffset() const
{
	return (((sl_uint32)(_flagsAndFlagmentOffset[0] & 0x1F)) << 8) | _flagsAndFlagmentOffset[1];
}

void IPv4Packet::setFragmentOffset(sl_uint32 offset)
{
	_flagsAndFlagmentOffset[1] = (sl_uint8)offset;
	_flagsAndFlagmentOffset[0] = (sl_uint8)((_flagsAndFlagmentOffset[0] & 0xE0) | ((offset >> 8) & 0x1F));
}

sl_uint8 IPv4Packet::getTTL() const
{
	return _timeToLive;
}

void IPv4Packet::setTTL(sl_uint8 TTL)
{
	_timeToLive = TTL;
}

sl_uint16 IPv4Packet::getChecksum() const
{
	return MIO::readUint16BE(_headerChecksum);
}

void IPv4Packet::setChecksum(sl_uint16 checksum)
{
	MIO::writeUint16BE(_headerChecksum, checksum);
}

void IPv4Packet::updateChecksum()
{
	_headerChecksum[0] = 0;
	_headerChecksum[1] = 0;
	MIO::writeUint16BE(_headerChecksum, TCP_IP::calculateChecksum(this, getHeaderSize()));
}

sl_bool IPv4Packet::checkChecksum() const
{
	sl_uint16 checksum = TCP_IP::calculateChecksum(this, getHeaderSize());
	return checksum == 0;
}

const sl_uint8* IPv4Packet::getOptions() const
{
	return (const sl_uint8*)(this) + sizeof(IPv4Packet);
}

sl_uint8* IPv4Packet::getOptions()
{
	return (sl_uint8*)(this) + sizeof(IPv4Packet);
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

sl_uint16 IPv4Packet::getChecksumForContent() const
{
	sl_uint16 sizeTotal = getTotalSize();
	sl_uint16 sizeHeader = getHeaderSize();
	sl_uint16 sizeContent = sizeTotal - sizeHeader;
	const char* content = (const char*)(this) + sizeHeader;
	return getChecksumForContent(content, sizeContent);
}

sl_bool IPv4Packet::check(const void* packet, sl_uint32 sizeTotal)
{
	if (!(checkHeader(packet, sizeTotal))) {
		return sl_false;
	}
	const IPv4Packet* header = (const IPv4Packet*)(packet);
	if (header->getTotalSize() > sizeTotal) {
		return sl_false;
	}
	return sl_true;
}

sl_bool IPv4Packet::checkHeader(const void* packet, sl_uint32 sizeTotal)
{
	if (!packet) {
		return sl_false;
	}
	if (sizeTotal < sizeof(IPv4Packet)) {
		return sl_false;
	}
	const IPv4Packet* header = (const IPv4Packet*)(packet);
	if (header->getVersion() != 4) {
		return sl_false;
	}
	if (sizeTotal < header->getHeaderSize()) {
		return sl_false;
	}
	if (!(header->checkChecksum())) {
		return sl_false;
	}
	return sl_true;
}

sl_bool IPv4Packet::isFirstFragment() const
{
	return getFragmentOffset() == 0;
}

sl_bool IPv4Packet::isLastFragment() const
{
	return !(isMF());
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


sl_uint32 TcpSegment::getSequenceNumber() const
{
	return MIO::readUint32BE(_sequenceNumber);
}

void TcpSegment::setSequenceNumber(sl_uint32 num)
{
	MIO::writeUint32BE(_sequenceNumber, num);
}

sl_uint32 TcpSegment::getAcknowledgmentNumber() const
{
	return MIO::readUint32BE(_acknowledgmentNumber);
}

void TcpSegment::setAcknowledgmentNumber(sl_uint32 num)
{
	MIO::writeUint32BE(_acknowledgmentNumber, num);
}

sl_bool TcpSegment::isNS() const
{
	return (_dataOffsetAndFlags[0] & 1) != 0;
}

void TcpSegment::setNS(sl_bool flag)
{
	_dataOffsetAndFlags[0] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xFE) | (flag ? 1 : 0));
}

sl_bool TcpSegment::isCWR() const
{
	return (_dataOffsetAndFlags[1] & 0x80) != 0;
}

void TcpSegment::setCWR(sl_bool flag)
{
	_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0x7F) | (flag ? 0x80 : 0));
}

sl_bool TcpSegment::isECE() const
{
	return (_dataOffsetAndFlags[1] & 0x40) != 0;
}

void TcpSegment::setECE(sl_bool flag)
{
	_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xBF) | (flag ? 0x40 : 0));
}

sl_bool TcpSegment::isURG() const
{
	return (_dataOffsetAndFlags[1] & 0x20) != 0;
}

void TcpSegment::setURG(sl_bool flag)
{
	_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xDF) | (flag ? 0x20 : 0));
}

sl_bool TcpSegment::isACK() const
{
	return (_dataOffsetAndFlags[1] & 0x10) != 0;
}

void TcpSegment::setACK(sl_bool flag)
{
	_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xEF) | (flag ? 0x10 : 0));
}

sl_bool TcpSegment::isPSH() const
{
	return (_dataOffsetAndFlags[1] & 0x08) != 0;
}

void TcpSegment::setPSH(sl_bool flag)
{
	_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xF7) | (flag ? 0x08 : 0));
}

sl_bool TcpSegment::isRST() const
{
	return (_dataOffsetAndFlags[1] & 0x04) != 0;
}

void TcpSegment::setRST(sl_bool flag)
{
	_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xFB) | (flag ? 0x04 : 0));
}

sl_bool TcpSegment::isSYN() const
{
	return (_dataOffsetAndFlags[1] & 0x02) != 0;
}

void TcpSegment::setSYN(sl_bool flag)
{
	_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xFD) | (flag ? 0x02 : 0));
}

sl_bool TcpSegment::isFIN() const
{
	return (_dataOffsetAndFlags[1] & 0x01) != 0;
}

void TcpSegment::setFIN(sl_bool flag)
{
	_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xFE) | (flag ? 0x01 : 0));
}

sl_uint16 TcpSegment::getWindowSize() const
{
	return MIO::readUint16BE(_windowSize);
}

void TcpSegment::setWindowSize(sl_uint16 size)
{
	MIO::writeUint16BE(_windowSize, size);
}

sl_uint16 TcpSegment::getChecksum() const
{
	return MIO::readUint16BE(_checksum);
}

void TcpSegment::setChecksum(sl_uint16 checksum)
{
	MIO::writeUint16BE(_checksum, checksum);
}

void TcpSegment::updateChecksum(const IPv4Packet* ipv4, sl_uint32 sizeContent)
{
	_checksum[0] = 0;
	_checksum[1] = 0;
	sl_uint16 checksum = ipv4->getChecksumForContent(this, sizeContent);
	MIO::writeUint16BE(_checksum, checksum);
}

sl_bool TcpSegment::checkChecksum(const IPv4Packet* ipv4, sl_uint32 sizeContent) const
{
	sl_uint16 checksum = ipv4->getChecksumForContent(this, sizeContent);
	return checksum == 0;
}

sl_bool TcpSegment::check(IPv4Packet* ip, sl_uint32 sizeTcp) const
{
	if (sizeTcp < sizeof(TcpSegment)) {
		return sl_false;
	}
	if (sizeTcp < getHeaderSize()) {
		return sl_false;
	}
	if (!(checkChecksum(ip, sizeTcp))) {
		return sl_false;
	}
	return sl_true;
}

sl_uint16 TcpSegment::getUrgentPointer() const
{
	return MIO::readUint16BE(_urgentPointer);
}

void TcpSegment::setUrgentPointer(sl_uint16 urgentPointer)
{
	MIO::writeUint16BE(_urgentPointer, urgentPointer);
}

const sl_uint8* TcpSegment::getOptions() const
{
	return (const sl_uint8*)(this) + sizeof(TcpSegment);
}

sl_uint8* TcpSegment::getOptions()
{
	return (sl_uint8*)(this) + sizeof(TcpSegment);
}


sl_uint16 UdpDatagram::getChecksum() const
{
	return MIO::readUint16BE(_checksum);
}

void UdpDatagram::setChecksum(sl_uint16 checksum)
{
	MIO::writeUint16BE(_checksum, checksum);
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
	if (getChecksum() == 0) {
		return sl_true;
	}
	sl_uint16 checksum = ipv4->getChecksumForContent(this, getTotalSize());
	return checksum == 0 || checksum == 0xFFFF;
}

sl_bool UdpDatagram::check(IPv4Packet* ip, sl_uint32 sizeUdp) const
{
	if (sizeUdp < HeaderSize) {
		return sl_false;
	}
	if (sizeUdp != getTotalSize()) {
		return sl_false;
	}
	if (!(checkChecksum(ip))) {
		return sl_false;
	}
	return sl_true;
}


sl_bool IPv4PacketIdentifier::operator==(const IPv4PacketIdentifier& other) const
{
	return source == other.source && destination == other.destination && identification == other.identification && protocol == other.protocol;
}

sl_uint32 IPv4PacketIdentifier::hashCode() const
{
	sl_uint64 t = source.hashCode();
	t *= 961;
	t += destination.hashCode();
	t *= 31;
	t += SLIB_MAKE_DWORD2(identification, protocol);
	return Hash64(t);
}

sl_uint32 Hash<IPv4PacketIdentifier>::operator()(const IPv4PacketIdentifier& v) const
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
	m_currentIdentifier = 0;
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

sl_bool IPv4Fragmentation::isNeededCombine(const void* _ip, sl_uint32 size, sl_bool flagCheckedHeader)
{
	IPv4Packet* ip = (IPv4Packet*)(_ip);
	if (!flagCheckedHeader) {
		if (!(IPv4Packet::check(ip, size))) {
			return sl_false;
		}
	}
	if (ip->getFragmentOffset() == 0 && !(ip->isMF())) {
		return sl_false;
	}
	return sl_true;
}

Memory IPv4Fragmentation::combineFragment(const void* _ip, sl_uint32 size, sl_bool flagCheckedHeader)
{
	IPv4Packet* ip = (IPv4Packet*)(_ip);
	if (!flagCheckedHeader) {
		if (!(IPv4Packet::check(ip, size))) {
			return sl_null;
		}
	}

	if (ip->getFragmentOffset() == 0 && !(ip->isMF())) {
		return Memory::create(ip, ip->getTotalSize());
	}

	sl_uint32 offset = ip->getFragmentOffset() * 8;

	IPv4PacketIdentifier id;
	id.source = ip->getSourceAddress();
	id.destination = ip->getDestinationAddress();
	id.identification = ip->getIdentification();
	id.protocol = ip->getProtocol();

	// get packet
	ObjectLocker lock(this);
	Ref<IPv4FragmentedPacket> packet;
	if (offset == 0) {
		packet = new IPv4FragmentedPacket;
		if (packet.isNull()) {
			return sl_null;
		}
		packet->header = Memory::create(ip, ip->getHeaderSize());
		if (packet->header.isEmpty()) {
			return sl_null;
		}
		IPv4Packet* headerNew = (IPv4Packet*)(packet->header.getData());
		headerNew->setMF(sl_false);
		headerNew->updateChecksum();
		packet->sizeAccumulated = 0;
		packet->sizeContent = 0;
		packet->fragments.removeAll();
		if (!(m_packets.put(id, packet))) {
			return sl_null;
		}
	} else {
		m_packets.get(id, &packet);
		if (packet.isNull()) {
			return sl_null;
		}
	}

	sl_uint8* data = (sl_uint8*)(ip->getContent());
	sl_uint32 sizeContent = ip->getContentSize();
	sl_uint32 start = offset;
	sl_uint32 end = start + sizeContent;

	// last fragment, then we can know the size of content
	if (!(ip->isMF())) {
		packet->sizeContent = end;
		if (end == 0) {
			m_packets.remove(id);
			return sl_null;
		}
	}

	// check overlapping
	if (sizeContent > 0) {
		ListLocker<IPv4Fragment> fragments(packet->fragments);
		for (sl_size i = 0; i < fragments.count; i++) {
			if (fragments[i].start <= start && start < fragments[i].end) {
				start = fragments[i].end;
			}
			if (fragments[i].start < end && end <= fragments[i].end) {
				end = fragments[i].start;
			}
			if (start >= end) {
				return sl_null;
			}
		}
	} else {
		return sl_null;
	}
	IPv4Fragment fragment;
	fragment.start = start;
	fragment.end = end;
	fragment.data = Memory::create(data + (start - offset), end - start);
	packet->fragments.add(fragment);
	packet->sizeAccumulated += (end - start);
	// check completed
	if (packet->sizeContent > 0 && packet->sizeAccumulated == packet->sizeContent) {
		m_packets.remove(id);
		Memory mem = Memory::create(packet->header.getSize() + packet->sizeContent);
		if (mem.isNotEmpty()) {
			Base::copyMemory(mem.getData(), packet->header.getData(), packet->header.getSize());
			data = (sl_uint8*)(mem.getData()) + packet->header.getSize();
			ListLocker<IPv4Fragment> fragments(packet->fragments);
			for (sl_size i = 0; i < fragments.count; i++) {
				Base::copyMemory(data + fragments[i].start, fragments[i].data.getData(), fragments[i].end - fragments[i].start);
			}
			return mem;
		}
	}
	return sl_null;
}

List<Memory> IPv4Fragmentation::makeFragments(const IPv4Packet* header, const void* ipContent, sl_uint32 sizeContent, sl_uint32 mtu)
{
	sl_int32 id = Base::interlockedIncrement32(&m_currentIdentifier);
	return makeFragments(header, (sl_uint16)id, ipContent, sizeContent, mtu);
}

List<Memory> IPv4Fragmentation::makeFragments(const IPv4Packet* header, sl_uint16 identifier, const void* ipContent, sl_uint32 sizeContent, sl_uint32 mtu)
{
	List<Memory> ret;
	sl_uint32 sizeHeader = header->getHeaderSize();
	if (mtu < sizeHeader + 8) {
		return ret;
	}
	if (sizeContent == 0) {
		return ret;
	}
	if (sizeContent > SLIB_UINT16_MAX) {
		return ret;
	}
	sl_uint8* data = (sl_uint8*)(ipContent);
	sl_uint32 sizeFragment = mtu - sizeHeader;
	sizeFragment = (sizeFragment & 0xFFF8);
	sl_uint32 offset = 0;
	while (offset < sizeContent) {
		sl_uint32 n = sizeFragment;
		if (offset + n > sizeContent) {
			n = sizeContent - offset;
		}
		Memory mem = Memory::create(sizeHeader + n);
		if (mem.isEmpty()) {
			return sl_null;
		}
		sl_uint8* buf = (sl_uint8*)(mem.getData());
		Base::copyMemory(buf, header, sizeHeader);
		Base::copyMemory(buf + sizeHeader, data + offset, n);
		IPv4Packet* h = (IPv4Packet*)buf;
		h->setIdentification(identifier);
		h->setTotalSize(sizeHeader + n);
		h->setDF(sl_false);
		h->setFragmentOffset(offset >> 3);
		offset += sizeFragment;
		h->setMF(offset < sizeContent);
		h->updateChecksum();
		
		ret.add_NoLock(mem);
	}
	return ret;
}

SLIB_NETWORK_NAMESPACE_END
