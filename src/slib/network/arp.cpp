/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/network/arp.h"

#include "slib/core/mio.h"

namespace slib
{

	sl_uint16 ArpPacket::getHardwareType() const
	{
		return MIO::readUint16BE(_hardwareType);
	}

	void ArpPacket::setHardwareType(sl_uint16 hardwareType)
	{
		MIO::writeUint16BE(_hardwareType, hardwareType);
	}

	NetworkLinkProtocol ArpPacket::getProtocolType() const
	{
		return (NetworkLinkProtocol)(MIO::readUint16BE(_protocolType));
	}

	void ArpPacket::setProtocolType(NetworkLinkProtocol protocolType)
	{
		MIO::writeUint16BE(_protocolType, (sl_uint32)protocolType);
	}

	sl_uint8 ArpPacket::getHardwareAddressLength() const
	{
		return _hardwareAddressLength;
	}

	void ArpPacket::setHardwareAddressLength(sl_uint8 length)
	{
		_hardwareAddressLength = length;
	}

	sl_uint8 ArpPacket::getProtocolAddressLength() const
	{
		return _protocolAddressLength;
	}

	void ArpPacket::setProtocolAddressLength(sl_uint8 length)
	{
		_protocolAddressLength = length;
	}

	ArpOperation ArpPacket::getOperation() const
	{
		return (ArpOperation)(MIO::readUint16BE(_operation));
	}

	void ArpPacket::setOperation(ArpOperation operation)
	{
		MIO::writeUint16BE(_operation, (sl_uint32)operation);
	}

	const sl_uint8* ArpPacket::getSenderHardwareAddress() const
	{
		return ((const sl_uint8*)this) + 8;
	}

	sl_uint8* ArpPacket::getSenderHardwareAddress()
	{
		return ((sl_uint8*)this) + 8;
	}

	MacAddress ArpPacket::getSenderMacAddress() const
	{
		return MacAddress(((sl_uint8*)this) + 8);
	}

	void ArpPacket::setSenderMacAddress(const MacAddress& address)
	{
		Base::copyMemory(((sl_uint8*)this) + 8, address.m, 6);
	}

	const sl_uint8* ArpPacket::getSenderProtocolAddress() const
	{
		return ((const sl_uint8*)this) + 8 + (sl_uint32)(getHardwareAddressLength());
	}

	sl_uint8* ArpPacket::getSenderProtocolAddress()
	{
		return ((sl_uint8*)this) + 8 + (sl_uint32)(getHardwareAddressLength());
	}

	IPv4Address ArpPacket::getSenderIPv4Address() const
	{
		return IPv4Address(((sl_uint8*)this) + 14);
	}

	void ArpPacket::setSenderIPv4Address(const IPv4Address& address)
	{
		address.getBytes(((sl_uint8*)this) + 14);
	}

	const sl_uint8* ArpPacket::getTargetHardwareAddress() const
	{
		return ((const sl_uint8*)this) + 8 + (sl_uint32)(getHardwareAddressLength()) + (sl_uint32)(getProtocolAddressLength());
	}

	sl_uint8* ArpPacket::getTargetHardwareAddress()
	{
		return ((sl_uint8*)this) + 8 + (sl_uint32)(getHardwareAddressLength()) + (sl_uint32)(getProtocolAddressLength());
	}

	MacAddress ArpPacket::getTargetMacAddress() const
	{
		return MacAddress(((sl_uint8*)this) + 18);
	}

	void ArpPacket::setTargetMacAddress(const MacAddress& address)
	{
		Base::copyMemory(((sl_uint8*)this) + 18, address.m, 6);
	}

	const sl_uint8* ArpPacket::getTargetProtocolAddress() const
	{
		return ((const sl_uint8*)this) + 8 + (sl_uint32)(getHardwareAddressLength()) * 2 + (sl_uint32)(getProtocolAddressLength());
	}

	sl_uint8* ArpPacket::getTargetProtocolAddress()
	{
		return ((sl_uint8*)this) + 8 + (sl_uint32)(getHardwareAddressLength()) * 2 + (sl_uint32)(getProtocolAddressLength());
	}

	IPv4Address ArpPacket::getTargetIPv4Address() const
	{
		return IPv4Address(((sl_uint8*)this) + 24);
	}

	void ArpPacket::setTargetIPv4Address(const IPv4Address& address)
	{
		address.getBytes(((sl_uint8*)this) + 24);
	}

	sl_bool ArpPacket::isValidEthernetIPv4() const
	{
		return getHardwareType() == 1 && getProtocolType() == NetworkLinkProtocol::IPv4 &&
				getHardwareAddressLength() == 6 && getProtocolAddressLength() == 4;
	}

}
