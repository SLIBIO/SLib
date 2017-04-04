/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_NETWORK_ETHERNET
#define CHECKHEADER_SLIB_NETWORK_ETHERNET

#include "definition.h"

#include "constants.h"
#include "mac_address.h"

#include "../core/map.h"

/*
	Ethernet II Frame (Layer 2)

	https://en.wikipedia.org/wiki/Ethernet_frame#Ethernet_II

	Header (14 bytes)
		0~5: Destination MAC Address
		6~11: Source MAC Address
		12~13: EtherType (Protocol)
		14~: Payload (46~1500 bytes)
*/

namespace slib
{

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
	
}

#include "detail/ethernet.h"

#endif
