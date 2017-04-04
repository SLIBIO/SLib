/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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

	sl_uint32 IcmpEchoAddress::hashCode() const
	{
		sl_uint64 t = ip.hashCode();
		t = t * 31 + SLIB_MAKE_DWORD2(identifier, sequenceNumber);
		return Hash64(t);
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

	sl_uint32 Hash<IcmpEchoAddress>::operator()(const IcmpEchoAddress& a) const
	{
		return a.hashCode();
	}

}
