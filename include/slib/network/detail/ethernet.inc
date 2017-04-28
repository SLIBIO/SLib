/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

namespace slib
{

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
