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

#include "slib/network/icmp.h"

#include "slib/core/mio.h"

namespace slib
{

	IcmpType IcmpHeaderFormat::getType() const
	{
		return (IcmpType)_type;
	}

	void IcmpHeaderFormat::setType(IcmpType type)
	{
		_type = (sl_uint8)type;
	}

	sl_uint8 IcmpHeaderFormat::getCode() const
	{
		return _code;
	}

	void IcmpHeaderFormat::setCode(sl_uint8 code)
	{
		_code = code;
	}

	sl_uint16 IcmpHeaderFormat::getChecksum() const
	{
		return MIO::readUint16BE(_checksum);
	}

	void IcmpHeaderFormat::setChecksum(sl_uint16 checksum)
	{
		MIO::writeUint16BE(_checksum, checksum);
	}

	void IcmpHeaderFormat::updateChecksum(sl_uint32 sizeICMP)
	{
		_checksum[0] = 0;
		_checksum[1] = 0;
		sl_uint16 checksum = TCP_IP::calculateChecksum(this, sizeICMP);
		setChecksum(checksum);
	}

	sl_bool IcmpHeaderFormat::checkChecksum(sl_uint32 sizeICMP) const
	{
		sl_uint16 checksum = TCP_IP::calculateChecksum(this, sizeICMP);
		return checksum == 0;
	}

	sl_bool IcmpHeaderFormat::check(sl_uint32 sizeICMP) const
	{
		if (sizeICMP < sizeof(IcmpHeaderFormat)) {
			return sl_false;
		}
		if (!(checkChecksum(sizeICMP))) {
			return sl_false;
		}
		return sl_true;
	}

	sl_uint16 IcmpHeaderFormat::getEchoIdentifier() const
	{
		return MIO::readUint16BE(_rest);
	}

	void IcmpHeaderFormat::setEchoIdentifier(sl_uint16 id)
	{
		MIO::writeUint16BE(_rest, id);
	}

	sl_uint16 IcmpHeaderFormat::getEchoSequenceNumber() const
	{
		return MIO::readUint16BE(_rest + 2);
	}

	void IcmpHeaderFormat::setEchoSequenceNumber(sl_uint16 sn)
	{
		MIO::writeUint16BE(_rest + 2, sn);
	}

	IPv4Address IcmpHeaderFormat::getRedirectGatewayAddress() const
	{
		return IPv4Address(_rest);
	}

	void IcmpHeaderFormat::setRedirectGatewayAddress(const IPv4Address& address)
	{
		address.getBytes(_rest);
	}

	sl_uint8 IcmpHeaderFormat::getParameterProblemPointer() const
	{
		return _rest[0];
	}

	void IcmpHeaderFormat::setParameterProblemPointer(sl_uint8 pointer)
	{
		_rest[0] = pointer;
	}

	sl_uint16 IcmpHeaderFormat::getTimestampIdentifier() const
	{
		return MIO::readUint16BE(_rest);
	}

	void IcmpHeaderFormat::setTimestampIdentifier(sl_uint16 id)
	{
		MIO::writeUint16BE(_rest, id);
	}

	sl_uint16 IcmpHeaderFormat::getTimestampSequenceNumber() const
	{
		return MIO::readUint16BE(_rest + 2);
	}

	void IcmpHeaderFormat::setTimestampSequenceNumber(sl_uint16 sn)
	{
		MIO::writeUint16BE(_rest + 2, sn);
	}

	sl_uint16 IcmpHeaderFormat::getAddressMaskIdentifier() const
	{
		return MIO::readUint16BE(_rest);
	}
	
	void IcmpHeaderFormat::setAddressMaskIdentifier(sl_uint16 id)
	{
		MIO::writeUint16BE(_rest, id);
	}
	
	sl_uint16 IcmpHeaderFormat::getAddressMaskSequenceNumber() const
	{
		return MIO::readUint16BE(_rest + 2);
	}
	
	void IcmpHeaderFormat::setAddressMaskSequenceNumber(sl_uint16 sn)
	{
		MIO::writeUint16BE(_rest + 2, sn);
	}

	sl_uint16 IcmpHeaderFormat::getNextHopMTU() const
	{
		return MIO::readUint16BE(_rest + 2);
	}
	
	void IcmpHeaderFormat::setNextHopMTU(sl_uint16 mtu)
	{
		MIO::writeUint16BE(_rest + 2, mtu);
	}

	const sl_uint8* IcmpHeaderFormat::getContent() const
	{
		return (const sl_uint8*)(this) + sizeof(sizeof(IcmpHeaderFormat));
	}

	sl_uint8* IcmpHeaderFormat::getContent()
	{
		return (sl_uint8*)(this) + sizeof(sizeof(IcmpHeaderFormat));
	}


	int IcmpEchoAddress::compare(const IcmpEchoAddress& other) const
	{
		int c = ip.compare(other.ip);
		if (c == 0) {
			return Compare<sl_uint32>()(SLIB_MAKE_DWORD2(identifier, sequenceNumber), SLIB_MAKE_DWORD2(other.identifier, other.sequenceNumber));
		}
		return c;
	}

	sl_size IcmpEchoAddress::hashCode() const
	{
		return Rehash64ToSize(SLIB_MAKE_QWORD4(SLIB_MAKE_DWORD2(identifier, sequenceNumber), ip.getInt()));
	}

	sl_bool IcmpEchoAddress::operator == (const IcmpEchoAddress& other) const
	{
		return ip == other.ip && identifier == other.identifier && sequenceNumber == other.sequenceNumber;
	}

	sl_bool IcmpEchoAddress::operator!=(const IcmpEchoAddress& other) const
	{
		return !(*this == other);
	}

	int Compare<IcmpEchoAddress>::operator()(const IcmpEchoAddress& a, const IcmpEchoAddress& b) const
	{
		return a.compare(b);
	}

	sl_bool Equals<IcmpEchoAddress>::operator()(const IcmpEchoAddress& a, const IcmpEchoAddress& b) const
	{
		return a == b;
	}

	sl_size Hash<IcmpEchoAddress>::operator()(const IcmpEchoAddress& a) const
	{
		return a.hashCode();
	}

}
