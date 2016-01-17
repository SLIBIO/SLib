#include "../../../inc/slib/network/icmp.h"

SLIB_NETWORK_NAMESPACE_BEGIN
void IcmpHeaderFormat::updateChecksum(sl_uint32 sizeContent)
{
	_checksum[0] = 0;
	_checksum[1] = 0;
	sl_uint16 checksum = TCP_IP::calculateChecksum(this, sizeContent);
	setChecksum(checksum);
}

sl_bool IcmpHeaderFormat::checkChecksum(sl_uint32 sizeContent) const
{
	sl_uint16 checksum = TCP_IP::calculateChecksum(this, sizeContent);
	return checksum == 0;
}

sl_bool IcmpHeaderFormat::check(sl_uint32 sizeContent) const
{
	if (sizeContent < sizeof(IcmpHeaderFormat)) {
		return sl_false;
	}
	if (!(checkChecksum(sizeContent))) {
		return sl_false;
	}
	return sl_true;
}

sl_bool IcmpEchoAddress::operator == (const IcmpEchoAddress& other) const
{
	return ip == other.ip && identifier == other.identifier && sequenceNumber == other.sequenceNumber;
}

int IcmpEchoAddress::compare(const IcmpEchoAddress& other) const
{
	int c = ip.compare(other.ip);
	if (c == 0) {
		return Compare<sl_uint32>::compare(SLIB_MAKE_DWORD2(identifier, sequenceNumber), SLIB_MAKE_DWORD2(other.identifier, other.sequenceNumber));
	}
	return c;
}

sl_uint32 IcmpEchoAddress::hashCode() const
{
	sl_uint64 t = ip.hashCode();
	t = t * 31 + SLIB_MAKE_DWORD2(identifier, sequenceNumber);
	return sl_hash(t);
}
SLIB_NETWORK_NAMESPACE_END
