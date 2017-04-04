/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_NETWORK_ARP
#define CHECKHEADER_SLIB_NETWORK_ARP

#include "definition.h"

#include "ethernet.h"
#include "mac_address.h"
#include "ip_address.h"

/*
	ARP - Address Resolution Protocol

	ARP Packet (28 bytes for IPv4)

	0~1: Hardware Type (1 for Ethernet)
	2~3: Protocol Type (EtherType, 0x0800 for IPv4)
	3: Hardware Address Length (6 for Ethernet)
	4: Protocol Address Length (4 for IPv4)
	6~7: Operation (1: Request, 2: Reply)
 
	// For IPv4
	8~13: Sender Hardware Address
	14~17: Sender Protocol Address
	18~23: Target Hardware Address
	24~27: Target Protocol Address
 
*/

namespace slib
{

	enum class ArpOperation
	{
		Request = 1,
		Reply = 2
	};
	
	class SLIB_EXPORT ArpPacket
	{
	public:
		enum
		{
			SizeForIPv4 = 28
		};
		
	public:
		sl_uint16 getHardwareType() const;
		
		void setHardwareType(sl_uint16 hardwareType);
		
		NetworkLinkProtocol getProtocolType() const;
		
		void setProtocolType(NetworkLinkProtocol protocolType);
		
		sl_uint8 getHardwareAddressLength() const;
		
		void setHardwareAddressLength(sl_uint8 length);
		
		sl_uint8 getProtocolAddressLength() const;
		
		void setProtocolAddressLength(sl_uint8 length);
		
		ArpOperation getOperation() const;
		
		void setOperation(ArpOperation operation);
		
		const sl_uint8* getSenderHardwareAddress() const;
		
		sl_uint8* getSenderHardwareAddress();
		
		MacAddress getSenderMacAddress() const;
		
		void setSenderMacAddress(const MacAddress& address);
		
		const sl_uint8* getSenderProtocolAddress() const;
		
		sl_uint8* getSenderProtocolAddress();
		
		IPv4Address getSenderIPv4Address() const;
		
		void setSenderIPv4Address(const IPv4Address& address);
		
		const sl_uint8* getTargetHardwareAddress() const;
		
		sl_uint8* getTargetHardwareAddress();
		
		MacAddress getTargetMacAddress() const;
		
		void setTargetMacAddress(const MacAddress& address);
		
		const sl_uint8* getTargetProtocolAddress() const;
		
		sl_uint8* getTargetProtocolAddress();
		
		IPv4Address getTargetIPv4Address() const;
		
		void setTargetIPv4Address(const IPv4Address& address);
		
		sl_bool isValidEthernetIPv4() const;
		
	private:
		sl_uint8 _hardwareType[2];
		sl_uint8 _protocolType[2];
		sl_uint8 _hardwareAddressLength;
		sl_uint8 _protocolAddressLength;
		sl_uint8 _operation[2];
		
	};

}

#endif
