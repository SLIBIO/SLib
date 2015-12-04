#include "../../../inc/slib/network/tcpip.h"
#include "../../../inc/slib/network/icmp.h"

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

void IPv4HeaderFormat::updateChecksum()
{
	_headerChecksum[0] = 0;
	_headerChecksum[1] = 0;
	setChecksum(TCP_IP::calculateChecksum(this, getHeaderSize()));
}

sl_bool IPv4HeaderFormat::checkChecksum() const
{
	sl_uint16 checksum = TCP_IP::calculateChecksum(this, getHeaderSize());
	return checksum == 0;
}

sl_uint16 IPv4HeaderFormat::getChecksumForContent(const void* content, sl_uint16 sizeContent) const
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

sl_uint16 IPv4HeaderFormat::getChecksumForContent() const
{
	sl_uint16 sizeTotal = getTotalSize();
	sl_uint16 sizeHeader = getHeaderSize();
	sl_uint16 sizeContent = sizeTotal - sizeHeader;
	const char* content = (const char*)(this) + sizeHeader;
	return getChecksumForContent(content, sizeContent);
}

sl_bool IPv4HeaderFormat::check(const void* packet, sl_uint32 sizeTotal)
{
	if (!(checkHeader(packet, sizeTotal))) {
		return sl_false;
	}
	const IPv4HeaderFormat* header = (const IPv4HeaderFormat*)(packet);
	if (header->getTotalSize() > sizeTotal) {
		return sl_false;
	}
	return sl_true;
}

sl_bool IPv4HeaderFormat::checkHeader(const void* packet, sl_uint32 sizeTotal)
{
	if (!packet) {
		return sl_false;
	}
	if (sizeTotal < sizeof(IPv4HeaderFormat)) {
		return sl_false;
	}
	const IPv4HeaderFormat* header = (const IPv4HeaderFormat*)(packet);
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

void TCP_HeaderFormat::updateChecksum(const IPv4HeaderFormat* ipv4, sl_uint32 sizeContent)
{
	_checksum[0] = 0;
	_checksum[1] = 0;
	sl_uint16 checksum = ipv4->getChecksumForContent(this, sizeContent);
	setChecksum(checksum);
}

sl_bool TCP_HeaderFormat::checkChecksum(const IPv4HeaderFormat* ipv4, sl_uint32 sizeContent) const
{
	sl_uint16 checksum = ipv4->getChecksumForContent(this, sizeContent);
	return checksum == 0;
}

sl_bool TCP_HeaderFormat::check(IPv4HeaderFormat* ip, sl_uint32 sizeTcp) const
{
	if (sizeTcp < sizeof(TCP_HeaderFormat)) {
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

void UDP_HeaderFormat::updateChecksum(const IPv4HeaderFormat* ipv4)
{
	_checksum[0] = 0;
	_checksum[1] = 0;
	sl_uint16 checksum = ipv4->getChecksumForContent(this, getTotalSize());
	if (checksum == 0) {
		checksum = 0xFFFF;
	}
	setChecksum(checksum);
}

sl_bool UDP_HeaderFormat::checkChecksum(const IPv4HeaderFormat* ipv4) const
{
	if (getChecksum() == 0) {
		return sl_true;
	}
	sl_uint16 checksum = ipv4->getChecksumForContent(this, getTotalSize());
	return checksum == 0 || checksum == 0xFFFF;
}

sl_bool UDP_HeaderFormat::check(IPv4HeaderFormat* ip, sl_uint32 sizeUdp) const
{
	if (sizeUdp < sizeof(UDP_HeaderFormat)) {
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
	return sl_hash(t);
}

IPv4Fragmentation::IPv4Fragmentation()
{
	m_currentIdentifier = 0;
}

IPv4Fragmentation::~IPv4Fragmentation()
{
}

void IPv4Fragmentation::setupExpiringDuration(sl_uint32 ms, const Ref<AsyncLoop>& loop)
{
	m_packets.setupTimer(ms, loop);
}

void IPv4Fragmentation::setupExpiringDuration(sl_uint32 ms)
{
	m_packets.setupTimer(ms);
}

Memory IPv4Fragmentation::combineFragment(const void* _ip, sl_uint32 size)
{
	IPv4HeaderFormat* ip = (IPv4HeaderFormat*)(_ip);
	if (!(IPv4HeaderFormat::check(ip, size))) {
		return Memory::null();
	}
	IPv4PacketIdentifier id;
	id.source = ip->getSourceAddress();
	id.destination = ip->getDestinationAddress();
	id.identification = ip->getIdentification();
	id.protocol = ip->getProtocol();

	if (ip->getFragmentOffset() == 0 && !(ip->isMF())) {
		m_packets.remove(id);
		return Memory::create(ip, ip->getTotalSize());
	}

	sl_uint32 offset = ip->getFragmentOffset() * 8;

	// get packet
	MutexLocker lock(getLocker());
	Ref<IPv4FragmentedPacket> packet;
	if (offset == 0) {
		packet = new IPv4FragmentedPacket;
		if (packet.isNull()) {
			return Memory::null();
		}
		packet->header = Memory::create(ip, ip->getHeaderSize());
		if (packet->header.isEmpty()) {
			return Memory::null();
		}
		IPv4HeaderFormat* headerNew = (IPv4HeaderFormat*)(packet->header.getBuf());
		headerNew->setMF(sl_false);
		headerNew->updateChecksum();
		packet->sizeAccumulated = 0;
		packet->sizeContent = 0;
		packet->fragments.clear();
		if (!(m_packets.put(id, packet))) {
			return Memory::null();
		}
	} else {
		m_packets.get(id, &packet);
		if (packet.isNull()) {
			return Memory::null();
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
			return Memory::null();
		}
	}

	// check overlapping
	if (sizeContent > 0) {
		ListLocker<IPv4Fragment> fragments(packet->fragments);
		for (sl_size i = 0; i < fragments.count(); i++) {
			if (fragments[i].start <= start && start < fragments[i].end) {
				start = fragments[i].end;
			}
			if (fragments[i].start < end && end <= fragments[i].end) {
				end = fragments[i].start;
			}
			if (start >= end) {
				return Memory::null();
			}
		}
	} else {
		return Memory::null();
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
			Base::copyMemory(mem.getBuf(), packet->header.getBuf(), packet->header.getSize());
			data = (sl_uint8*)(mem.getBuf()) + packet->header.getSize();
			ListLocker<IPv4Fragment> fragments(packet->fragments);
			for (sl_size i = 0; i < fragments.count(); i++) {
				Base::copyMemory(data + fragments[i].start, fragments[i].data.getBuf(), fragments[i].end - fragments[i].start);
			}
			return mem;
		}
	}
	return Memory::null();
}

List<Memory> IPv4Fragmentation::makeFragments(const IPv4HeaderFormat* header, const void* ipContent, sl_uint32 sizeContent, sl_uint32 mtu)
{
	sl_int32 id = Base::interlockedIncrement32(&m_currentIdentifier);
	return makeFragments(header, (sl_uint16)id, ipContent, sizeContent, mtu);
}

List<Memory> IPv4Fragmentation::makeFragments(const IPv4HeaderFormat* header, sl_uint16 identifier, const void* ipContent, sl_uint32 sizeContent, sl_uint32 mtu)
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
			return List<Memory>::null();
		}
		sl_uint8* buf = (sl_uint8*)(mem.getBuf());
		Base::copyMemory(buf, header, sizeHeader);
		Base::copyMemory(buf + sizeHeader, data + offset, n);
		IPv4HeaderFormat* h = (IPv4HeaderFormat*)buf;
		h->setIdentification(identifier);
		h->setContentSize(n);
		h->setDF(sl_false);
		h->setFragmentOffset(offset >> 3);
		offset += sizeFragment;
		h->setMF(offset < sizeContent);
		h->updateChecksum();
		ret.add(mem);
	}
	return ret;
}
SLIB_NETWORK_NAMESPACE_END
