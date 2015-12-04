#ifndef CHECKHEADER_SLIB_NETWORK_ARP
#define CHECKHEADER_SLIB_NETWORK_ARP

#include "definition.h"

#include "ethernet.h"
#include "mac_address.h"
#include "ip_address.h"

/*
	ARP - Address Resolution Protocol

	ARP Packet (28 bytes)

	0~1: Hardware Type (1 for Ethernet)
	2~3: Protocol Type (EtherType, 0x0800 for IPv4)
	3: Hardware Address Length (6 for Ethernet)
	4: Protocol Address Length (4 for IPv4)
	6~7: Operation (1: Request, 2: Reply)
	8~13: Sender Hardware Address
	14~17: Sender Protocol Address
	18~23: Target Hardware Address
	24~27: Target Protocol Address
*/

SLIB_NETWORK_NAMESPACE_BEGIN
class SLIB_EXPORT ArpPacketFormat
{
public:
	SLIB_INLINE sl_uint16 getHardwareType() const
	{
		return MIO::readUint16BE(_hardwareType);
	}
	SLIB_INLINE void setHardwareType(sl_uint16 hardwareType)
	{
		MIO::writeUint16BE(_hardwareType, hardwareType);
	}

	SLIB_INLINE sl_uint16 getProtocolType() const
	{
		return MIO::readUint16BE(_protocolType);
	}
	SLIB_INLINE void setProtocolType(sl_uint16 protocolType)
	{
		MIO::writeUint16BE(_protocolType, protocolType);
	}

	SLIB_INLINE sl_uint8 getHardwareAddressLength() const
	{
		return _hardwareAddressLength;
	}
	SLIB_INLINE void setHardwareAddressLength(sl_uint8 length)
	{
		_hardwareAddressLength = length;
	}

	SLIB_INLINE sl_uint8 getProtocolAddressLength() const
	{
		return _protocolAddressLength;
	}
	SLIB_INLINE void setProtocolAddressLength(sl_uint8 length)
	{
		_protocolAddressLength = length;
	}

	enum Operation
	{
		operationRequest = 1,
		operationReply = 2
	};
	SLIB_INLINE Operation getOperation() const
	{
		return (Operation)(MIO::readUint16BE(_operation));
	}
	SLIB_INLINE void setOperation(Operation operation)
	{
		MIO::writeUint16BE(_operation, operation);
	}

	SLIB_INLINE const sl_uint8* getSenderHardwareAddress() const
	{
		return ((const sl_uint8*)this) + 8;
	}
	SLIB_INLINE sl_uint8* getSenderHardwareAddress()
	{
		return ((sl_uint8*)this) + 8;
	}
	SLIB_INLINE MacAddress getSenderMacAddress() const
	{
		return MacAddress(((sl_uint8*)this) + 8);
	}
	SLIB_INLINE void setSenderMacAddress(const MacAddress& address)
	{
		Base::copyMemory(((sl_uint8*)this) + 8, address.m, 6);
	}

	SLIB_INLINE const sl_uint8* getSenderProtocolAddress() const
	{
		return ((const sl_uint8*)this) + 8 + (sl_uint32)(getHardwareAddressLength());
	}
	SLIB_INLINE sl_uint8* getSenderProtocolAddress()
	{
		return ((sl_uint8*)this) + 8 + (sl_uint32)(getHardwareAddressLength());
	}
	SLIB_INLINE IPv4Address getSenderIPv4Address() const
	{
		return IPv4Address(((sl_uint8*)this) + 14);
	}
	SLIB_INLINE void setSenderIPv4Address(const IPv4Address& address)
	{
		address.get(((sl_uint8*)this) + 14);
	}

	SLIB_INLINE const sl_uint8* getTargetHardwareAddress() const
	{
		return ((const sl_uint8*)this) + 8 + (sl_uint32)(getHardwareAddressLength()) + (sl_uint32)(getProtocolAddressLength());
	}
	SLIB_INLINE sl_uint8* getTargetHardwareAddress()
	{
		return ((sl_uint8*)this) + 8 + (sl_uint32)(getHardwareAddressLength()) + (sl_uint32)(getProtocolAddressLength());
	}
	SLIB_INLINE MacAddress getTargetMacAddress() const
	{
		return MacAddress(((sl_uint8*)this) + 18);
	}
	SLIB_INLINE void setTargetMacAddress(const MacAddress& address)
	{
		Base::copyMemory(((sl_uint8*)this) + 18, address.m, 6);
	}

	SLIB_INLINE const sl_uint8* getTargetProtocolAddress() const
	{
		return ((const sl_uint8*)this) + 8 + (sl_uint32)(getHardwareAddressLength()) * 2 + (sl_uint32)(getProtocolAddressLength());
	}
	SLIB_INLINE sl_uint8* getTargetProtocolAddress()
	{
		return ((sl_uint8*)this) + 8 + (sl_uint32)(getHardwareAddressLength()) * 2 + (sl_uint32)(getProtocolAddressLength());
	}
	SLIB_INLINE IPv4Address getTargetIPv4Address() const
	{
		return IPv4Address(((sl_uint8*)this) + 24);
	}
	SLIB_INLINE void setTargetIPv4Address(const IPv4Address& address)
	{
		address.get(((sl_uint8*)this) + 24);
	}

	SLIB_INLINE static sl_uint32 getPacketSizeForEthernetIPv4()
	{
		return 28;
	}
	SLIB_INLINE sl_bool isValidEthernetIPv4() const
	{
		return getHardwareType() == 1 && getProtocolType() == networkLinkProtocol_IPv4
			&& getHardwareAddressLength() == 6 && getProtocolAddressLength() == 4;
	}

private:
	sl_uint8 _hardwareType[2];
	sl_uint8 _protocolType[2];
	sl_uint8 _hardwareAddressLength;
	sl_uint8 _protocolAddressLength;
	sl_uint8 _operation[2];
};
SLIB_NETWORK_NAMESPACE_END

#endif
