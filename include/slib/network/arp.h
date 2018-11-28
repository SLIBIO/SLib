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
