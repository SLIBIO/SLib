#ifndef CHECKHEADER_SLIB_NETWORK_ETHERNET
#define CHECKHEADER_SLIB_NETWORK_ETHERNET

#include "definition.h"

#include "constants.h"
#include "mac_address.h"

#include "../core/mio.h"
#include "../core/map.h"

SLIB_NETWORK_NAMESPACE_BEGIN

#define SLIB_NETWORK_ETHERNET_FRAME_HEADER_SIZE 14

/*
	Ethernet II Frame (Layer 2)

	https://en.wikipedia.org/wiki/Ethernet_frame#Ethernet_II

	Header (14 bytes)
		0~5: Destination MAC Address
		6~11: Source MAC Address
		12~13: EtherType (Protocol)
		14~: Payload (46~1500 bytes)
*/

class SLIB_EXPORT EthernetFrameFormat
{
public:
	SLIB_INLINE MacAddress getDestinationAddress()
	{
		return MacAddress(_macDestination);
	}
	
	SLIB_INLINE void setDestinationAddress(const MacAddress& address)
	{
		Base::copyMemory(_macDestination, address.m, 6);
	}

	
	SLIB_INLINE MacAddress getSourceAddress()
	{
		return MacAddress(_macSource);
	}
	
	SLIB_INLINE void setSourceAddress(const MacAddress& address)
	{
		Base::copyMemory(_macSource, address.m, 6);
	}

	
	// NetworkLinkProtocol
	SLIB_INLINE sl_uint16 getProtocol() const
	{
		return MIO::readUint16BE(_etherType);
	}
	
	// NetworkLinkProtocol
	SLIB_INLINE void setProtocol(sl_uint16 type)
	{
		MIO::writeUint16BE(_etherType, type);
	}

	
	SLIB_INLINE const sl_uint8* getContent() const
	{
		return ((const sl_uint8*)this) + SLIB_NETWORK_ETHERNET_FRAME_HEADER_SIZE;
	}
	
	SLIB_INLINE sl_uint8* getContent()
	{
		return ((sl_uint8*)this) + SLIB_NETWORK_ETHERNET_FRAME_HEADER_SIZE;
	}

	
	static SLIB_INLINE sl_uint32 getHeaderSize()
	{
		return SLIB_NETWORK_ETHERNET_FRAME_HEADER_SIZE;
	}
	
private:
	sl_uint8 _macDestination[6];
	sl_uint8 _macSource[6];
	sl_uint8 _etherType[2];
};


class SLIB_EXPORT EthernetMacTable : public Object
{
public:
	void add(const IPv4Address& ip, const MacAddress& mac);
	
	sl_bool getMacAddress(const IPv4Address& ip, MacAddress* _out = sl_null);

	void parseEthernetFrame(const void* frame, sl_uint32 sizeFrame, sl_bool flagUseSource, sl_bool flagUseDestination);

protected:
	HashMap<IPv4Address, MacAddress> m_table;
};

SLIB_NETWORK_NAMESPACE_END

#endif
