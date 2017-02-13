#ifndef CHECKHEADER_SLIB_NETWORK_ETHERNET
#define CHECKHEADER_SLIB_NETWORK_ETHERNET

#include "definition.h"

#include "constants.h"
#include "mac_address.h"

#include "../core/map.h"

namespace slib{

/*
	Ethernet II Frame (Layer 2)

	https://en.wikipedia.org/wiki/Ethernet_frame#Ethernet_II

	Header (14 bytes)
		0~5: Destination MAC Address
		6~11: Source MAC Address
		12~13: EtherType (Protocol)
		14~: Payload (46~1500 bytes)
*/
	class SLIB_EXPORT EthernetFrame
	{
	public:
		enum
		{
			HeaderSize = 14
		};
		
	public:
		MacAddress getDestinationAddress();
		
		void setDestinationAddress(const MacAddress& address);
		
		
		MacAddress getSourceAddress();
		
		void setSourceAddress(const MacAddress& address);
		
		
		NetworkLinkProtocol getProtocol() const;
		
		void setProtocol(NetworkLinkProtocol type);
		
		
		const sl_uint8* getContent() const;
		
		sl_uint8* getContent();
		
	private:
		sl_uint8 _macDestination[6];
		sl_uint8 _macSource[6];
		sl_uint8 _etherType[2];
		
	};
	
	
	class SLIB_EXPORT EthernetMacTable : public Object
	{
	public:
		EthernetMacTable();
		
		~EthernetMacTable();
		
	public:
		void add(const IPv4Address& ip, const MacAddress& mac);
		
		sl_bool getMacAddress(const IPv4Address& ip, MacAddress* _out = sl_null);
		
		void parseEthernetFrame(const void* frame, sl_uint32 sizeFrame, sl_bool flagUseSource, sl_bool flagUseDestination);
		
	protected:
		HashMap<IPv4Address, MacAddress> m_table;
		
	};
	
	SLIB_INLINE MacAddress EthernetFrame::getDestinationAddress()
	{
		return {_macDestination[0], _macDestination[1], _macDestination[2], _macDestination[3], _macDestination[4], _macDestination[5]};
	}
	
	SLIB_INLINE void EthernetFrame::setDestinationAddress(const MacAddress& address)
	{
		_macDestination[0] = address.m[0];
		_macDestination[1] = address.m[1];
		_macDestination[2] = address.m[2];
		_macDestination[3] = address.m[3];
		_macDestination[4] = address.m[4];
		_macDestination[5] = address.m[5];
	}
	
	SLIB_INLINE MacAddress EthernetFrame::getSourceAddress()
	{
		return {_macSource[0], _macSource[1], _macSource[2], _macSource[3], _macSource[4], _macSource[5]};
	}
	
	SLIB_INLINE void EthernetFrame::setSourceAddress(const MacAddress& address)
	{
		_macSource[0] = address.m[0];
		_macSource[1] = address.m[1];
		_macSource[2] = address.m[2];
		_macSource[3] = address.m[3];
		_macSource[4] = address.m[4];
		_macSource[5] = address.m[5];
	}
	
	SLIB_INLINE NetworkLinkProtocol EthernetFrame::getProtocol() const
	{
		return (NetworkLinkProtocol)(((sl_uint16)(_etherType[0]) << 8) | ((sl_uint16)(_etherType[1])));
	}
	
	SLIB_INLINE void EthernetFrame::setProtocol(NetworkLinkProtocol _type)
	{
		sl_uint32 type = (sl_uint32)_type;
		_etherType[0] = (sl_uint8)(type >> 8);
		_etherType[1] = (sl_uint8)(type);
	}
	
	SLIB_INLINE const sl_uint8* EthernetFrame::getContent() const
	{
		return ((const sl_uint8*)this) + HeaderSize;
	}
	
	SLIB_INLINE sl_uint8* EthernetFrame::getContent()
	{
		return ((sl_uint8*)this) + HeaderSize;
	}
}

#endif
