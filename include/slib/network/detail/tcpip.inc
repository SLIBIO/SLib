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

	SLIB_INLINE sl_uint8 IPv4Packet::getVersion() const
	{
		return _versionAndHeaderLength >> 4;
	}
	
	SLIB_INLINE void IPv4Packet::setVersion(sl_uint8 version)
	{
		_versionAndHeaderLength = (_versionAndHeaderLength & 0x0F) | (version << 4);
	}
	
	SLIB_INLINE sl_uint8 IPv4Packet::getHeaderLength() const
	{
		return _versionAndHeaderLength & 0x0F;
	}

	SLIB_INLINE void IPv4Packet::setHeaderLength(sl_uint8 length)
	{
		_versionAndHeaderLength = (_versionAndHeaderLength & 0xF0) | (length & 0x0F);
	}

	SLIB_INLINE sl_uint8 IPv4Packet::getHeaderSize() const
	{
		return (_versionAndHeaderLength & 0x0F) << 2;
	}

	SLIB_INLINE void IPv4Packet::setHeaderSize(sl_uint8 size)
	{
		setHeaderLength((size + 3) >> 2);
	}
	
	SLIB_INLINE sl_uint8 IPv4Packet::getTypeOfService() const
	{
		return m_TOS_DSCP_ECN;
	}
	
	SLIB_INLINE void IPv4Packet::setTypeOfService(sl_uint8 TOS)
	{
		m_TOS_DSCP_ECN = TOS;
	}
	
	SLIB_INLINE sl_uint8 IPv4Packet::getDSCP() const
	{
		return ((m_TOS_DSCP_ECN >> 2) & 0x3F);
	}
	
	SLIB_INLINE void IPv4Packet::setDSCP(sl_uint8 DSCP)
	{
		m_TOS_DSCP_ECN = (sl_uint8)((m_TOS_DSCP_ECN & 3) | ((DSCP & 0x3F) << 2));
	}
	
	SLIB_INLINE sl_uint8 IPv4Packet::getECN() const
	{
		return (m_TOS_DSCP_ECN & 3);
	}
	
	SLIB_INLINE void IPv4Packet::setECN(sl_uint8 ECN)
	{
		m_TOS_DSCP_ECN = (sl_uint8)((m_TOS_DSCP_ECN & 0xFC) | (ECN & 3));
	}
	
	SLIB_INLINE sl_uint16 IPv4Packet::getTotalSize() const
	{
		return ((sl_uint16)(_totalLength[0]) << 8) | ((sl_uint16)(_totalLength[1]));
	}

	SLIB_INLINE void IPv4Packet::setTotalSize(sl_uint16 size)
	{
		_totalLength[0] = (sl_uint8)(size >> 8);
		_totalLength[1] = (sl_uint8)(size);
	}
	
	SLIB_INLINE sl_uint16 IPv4Packet::getIdentification() const
	{
		return ((sl_uint16)(_identification[0]) << 8) | ((sl_uint16)(_identification[1]));
	}
	
	SLIB_INLINE void IPv4Packet::setIdentification(sl_uint16 identification)
	{
		_identification[0] = (sl_uint8)(identification >> 8);
		_identification[1] = (sl_uint8)(identification);
	}
	
	SLIB_INLINE sl_bool IPv4Packet::isDF() const
	{
		return (_flagsAndFragmentOffset[0] & 0x40) != 0;
	}
	
	SLIB_INLINE void IPv4Packet::setDF(sl_bool flag)
	{
		_flagsAndFragmentOffset[0] = (sl_uint8)((_flagsAndFragmentOffset[0] & 0xBF) | (flag ? 0x40 : 0));
	}
	
	SLIB_INLINE sl_bool IPv4Packet::isMF() const
	{
		return (_flagsAndFragmentOffset[0] & 0x20) != 0;
	}
	
	SLIB_INLINE void IPv4Packet::setMF(sl_bool flag)
	{
		_flagsAndFragmentOffset[0] = (sl_uint8)((_flagsAndFragmentOffset[0] & 0xDF) | (flag ? 0x20 : 0));
	}
	
	SLIB_INLINE sl_uint16 IPv4Packet::getFragmentOffset() const
	{
		return (((sl_uint16)(_flagsAndFragmentOffset[0] & 0x1F)) << 8) | _flagsAndFragmentOffset[1];
	}
	
	SLIB_INLINE void IPv4Packet::setFragmentOffset(sl_uint16 offset)
	{
		_flagsAndFragmentOffset[1] = (sl_uint8)offset;
		_flagsAndFragmentOffset[0] = (sl_uint8)((_flagsAndFragmentOffset[0] & 0xE0) | ((offset >> 8) & 0x1F));
	}
	
	SLIB_INLINE sl_uint8 IPv4Packet::getTTL() const
	{
		return _timeToLive;
	}
	
	SLIB_INLINE void IPv4Packet::setTTL(sl_uint8 TTL)
	{
		_timeToLive = TTL;
	}
	
	SLIB_INLINE NetworkInternetProtocol IPv4Packet::getProtocol() const
	{
		return (NetworkInternetProtocol)_protocol;
	}

	SLIB_INLINE void IPv4Packet::setProtocol(NetworkInternetProtocol protocol)
	{
		_protocol = (sl_uint8)(protocol);
	}

	SLIB_INLINE sl_uint16 IPv4Packet::getChecksum() const
	{
		return ((sl_uint16)(_headerChecksum[0]) << 8) | ((sl_uint16)(_headerChecksum[1]));
	}
	
	SLIB_INLINE void IPv4Packet::setChecksum(sl_uint16 checksum)
	{
		_headerChecksum[0] = (sl_uint8)(checksum >> 8);
		_headerChecksum[1] = (sl_uint8)(checksum);
	}
	
	SLIB_INLINE IPv4Address IPv4Packet::getSourceAddress() const
	{
		return {_sourceIp[0], _sourceIp[1], _sourceIp[2], _sourceIp[3]};
	}

	SLIB_INLINE void IPv4Packet::setSourceAddress(const IPv4Address& address)
	{
		_sourceIp[0] = address.a;
		_sourceIp[1] = address.b;
		_sourceIp[2] = address.c;
		_sourceIp[3] = address.d;
	}

	SLIB_INLINE IPv4Address IPv4Packet::getDestinationAddress() const
	{
		return {_destinationIp[0], _destinationIp[1], _destinationIp[2], _destinationIp[3]};
	}

	SLIB_INLINE void IPv4Packet::setDestinationAddress(const IPv4Address& address)
	{
		_destinationIp[0] = address.a;
		_destinationIp[1] = address.b;
		_destinationIp[2] = address.c;
		_destinationIp[3] = address.d;
	}

	SLIB_INLINE const sl_uint8* IPv4Packet::getOptions() const
	{
		return (const sl_uint8*)(this) + sizeof(IPv4Packet);
	}
	
	SLIB_INLINE sl_uint8* IPv4Packet::getOptions()
	{
		return (sl_uint8*)(this) + sizeof(IPv4Packet);
	}
	
	SLIB_INLINE const sl_uint8* IPv4Packet::getContent() const
	{
		return (const sl_uint8*)(this) + getHeaderSize();
	}

	SLIB_INLINE sl_uint8* IPv4Packet::getContent()
	{
		return (sl_uint8*)(this) + getHeaderSize();
	}

	SLIB_INLINE sl_uint16 IPv4Packet::getContentSize() const
	{
		return getTotalSize() - getHeaderSize();
	}

	
	SLIB_INLINE sl_uint8 IPv6Packet::getVersion() const
	{
		return _version_TrafficClass_FlowLabel[0] >> 4;
	}
	
	SLIB_INLINE void IPv6Packet::setVersion(sl_uint8 version)
	{
		_version_TrafficClass_FlowLabel[0] = (_version_TrafficClass_FlowLabel[0] & 0x0F) | (version << 4);
	}
	
	SLIB_INLINE sl_uint8 IPv6Packet::getTrafficClass() const
	{
		return ((_version_TrafficClass_FlowLabel[0] & 0x0F) << 4) | (_version_TrafficClass_FlowLabel[1] >> 4);
	}
	
	SLIB_INLINE void IPv6Packet::setTrafficClass(sl_uint8 value)
	{
		_version_TrafficClass_FlowLabel[0] = (_version_TrafficClass_FlowLabel[0] & 0xF0) | (value >> 4);
		_version_TrafficClass_FlowLabel[1] = (_version_TrafficClass_FlowLabel[1] & 0x0F) | ((value & 0x0F) << 4);
	}
	
	SLIB_INLINE sl_uint32 IPv6Packet::getFlowLabel() const
	{
		return ((sl_uint32)(_version_TrafficClass_FlowLabel[1] & 0x0F) << 16) | ((sl_uint32)(_version_TrafficClass_FlowLabel[2]) << 8) | _version_TrafficClass_FlowLabel[3];
	}
	
	SLIB_INLINE void IPv6Packet::setFlowLabel(sl_uint32 value)
	{
		_version_TrafficClass_FlowLabel[1] = (_version_TrafficClass_FlowLabel[0] & 0xF0) | (sl_uint8)((value >> 16) & 0x0F);
		_version_TrafficClass_FlowLabel[2] = (sl_uint8)(value >> 8);
		_version_TrafficClass_FlowLabel[3] = (sl_uint8)(value);
	}
	
	SLIB_INLINE sl_uint16 IPv6Packet::getPayloadLength() const
	{
		return ((sl_uint16)(_payloadLength[0]) << 8) | ((sl_uint16)(_payloadLength[1]));
	}
	
	SLIB_INLINE void IPv6Packet::setPayloadLength(sl_uint16 length)
	{
		_payloadLength[0] = (sl_uint8)(length >> 8);
		_payloadLength[1] = (sl_uint8)(length);
	}
	
	SLIB_INLINE NetworkInternetProtocol IPv6Packet::getNextHeader() const
	{
		return (NetworkInternetProtocol)(_nextHeader);
	}
	
	SLIB_INLINE void IPv6Packet::setNextHeader(NetworkInternetProtocol protocol)
	{
		_nextHeader = (sl_uint8)(protocol);
	}
	
	SLIB_INLINE sl_uint8 IPv6Packet::getHopLimit() const
	{
		return _hopLimit;
	}
	
	SLIB_INLINE void IPv6Packet::setHopLimit(sl_uint8 limit)
	{
		_hopLimit = limit;
	}
	
	SLIB_INLINE IPv6Address IPv6Packet::getSourceAddress() const
	{
		return IPv6Address(_sourceAddress);
	}
	
	SLIB_INLINE void IPv6Packet::setSourceAddresss(const IPv6Address& address)
	{
		address.getBytes(_sourceAddress);
	}
	
	SLIB_INLINE IPv6Address IPv6Packet::getDestinationAddress() const
	{
		return IPv6Address(_destinationAddress);
	}
	
	SLIB_INLINE void IPv6Packet::setDestinationAddresss(const IPv6Address& address)
	{
		address.getBytes(_destinationAddress);
	}
	
	SLIB_INLINE sl_uint8* IPv6Packet::getContent()
	{
		return (sl_uint8*)(this) + HeaderSize;
	}
	

	SLIB_INLINE sl_uint16 TcpSegment::getSourcePort() const
	{
		return ((sl_uint16)(_sourcePort[0]) << 8) | ((sl_uint16)(_sourcePort[1]));
	}

	SLIB_INLINE void TcpSegment::setSourcePort(sl_uint16 port)
	{
		_sourcePort[0] = (sl_uint8)(port >> 8);
		_sourcePort[1] = (sl_uint8)(port);
	}

	SLIB_INLINE sl_uint16 TcpSegment::getDestinationPort() const
	{
		return ((sl_uint16)(_destinationPort[0]) << 8) | ((sl_uint16)(_destinationPort[1]));
	}

	SLIB_INLINE void TcpSegment::setDestinationPort(sl_uint16 port)
	{
		_destinationPort[0] = (sl_uint8)(port >> 8);
		_destinationPort[1] = (sl_uint8)(port);
	}

	SLIB_INLINE sl_uint32 TcpSegment::getSequenceNumber() const
	{
		return ((sl_uint32)(_sequenceNumber[0]) << 24) | ((sl_uint32)(_sequenceNumber[1]) << 16) | ((sl_uint32)(_sequenceNumber[2]) << 8) | ((sl_uint32)(_sequenceNumber[3]));
	}
	
	SLIB_INLINE void TcpSegment::setSequenceNumber(sl_uint32 num)
	{
		_sequenceNumber[0] = (sl_uint8)(num >> 24);
		_sequenceNumber[1] = (sl_uint8)(num >> 16);
		_sequenceNumber[2] = (sl_uint8)(num >> 8);
		_sequenceNumber[3] = (sl_uint8)(num);
	}
	
	SLIB_INLINE sl_uint32 TcpSegment::getAcknowledgmentNumber() const
	{
		return ((sl_uint32)(_acknowledgmentNumber[0]) << 24) | ((sl_uint32)(_acknowledgmentNumber[1]) << 16) | ((sl_uint32)(_acknowledgmentNumber[2]) << 8) | ((sl_uint32)(_acknowledgmentNumber[3]));
	}
	
	SLIB_INLINE void TcpSegment::setAcknowledgmentNumber(sl_uint32 num)
	{
		_acknowledgmentNumber[0] = (sl_uint8)(num >> 24);
		_acknowledgmentNumber[1] = (sl_uint8)(num >> 16);
		_acknowledgmentNumber[2] = (sl_uint8)(num >> 8);
		_acknowledgmentNumber[3] = (sl_uint8)(num);
	}
	
	SLIB_INLINE sl_uint8 TcpSegment::getHeaderLength() const
	{
		return _dataOffsetAndFlags[0] >> 4;
	}

	SLIB_INLINE void TcpSegment::setHeaderLength(sl_uint8 length)
	{
		_dataOffsetAndFlags[0] = (sl_uint8)((_dataOffsetAndFlags[0] & 0x0F) | (length << 4));
	}

	SLIB_INLINE sl_uint8 TcpSegment::getHeaderSize() const
	{
		return (_dataOffsetAndFlags[0] >> 4) << 2;
	}

	SLIB_INLINE void TcpSegment::setHeaderSize(sl_uint8 size)
	{
		setHeaderLength((size + 3) >> 2);
	}

	SLIB_INLINE sl_bool TcpSegment::isNS() const
	{
		return (_dataOffsetAndFlags[0] & 1) != 0;
	}
	
	SLIB_INLINE void TcpSegment::setNS(sl_bool flag)
	{
		_dataOffsetAndFlags[0] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xFE) | (flag ? 1 : 0));
	}
	
	SLIB_INLINE sl_bool TcpSegment::isCWR() const
	{
		return (_dataOffsetAndFlags[1] & 0x80) != 0;
	}
	
	SLIB_INLINE void TcpSegment::setCWR(sl_bool flag)
	{
		_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0x7F) | (flag ? 0x80 : 0));
	}
	
	SLIB_INLINE sl_bool TcpSegment::isECE() const
	{
		return (_dataOffsetAndFlags[1] & 0x40) != 0;
	}
	
	SLIB_INLINE void TcpSegment::setECE(sl_bool flag)
	{
		_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xBF) | (flag ? 0x40 : 0));
	}
	
	SLIB_INLINE sl_bool TcpSegment::isURG() const
	{
		return (_dataOffsetAndFlags[1] & 0x20) != 0;
	}
	
	SLIB_INLINE void TcpSegment::setURG(sl_bool flag)
	{
		_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xDF) | (flag ? 0x20 : 0));
	}
	
	SLIB_INLINE sl_bool TcpSegment::isACK() const
	{
		return (_dataOffsetAndFlags[1] & 0x10) != 0;
	}
	
	SLIB_INLINE void TcpSegment::setACK(sl_bool flag)
	{
		_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xEF) | (flag ? 0x10 : 0));
	}
	
	SLIB_INLINE sl_bool TcpSegment::isPSH() const
	{
		return (_dataOffsetAndFlags[1] & 0x08) != 0;
	}
	
	SLIB_INLINE void TcpSegment::setPSH(sl_bool flag)
	{
		_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xF7) | (flag ? 0x08 : 0));
	}
	
	SLIB_INLINE sl_bool TcpSegment::isRST() const
	{
		return (_dataOffsetAndFlags[1] & 0x04) != 0;
	}
	
	SLIB_INLINE void TcpSegment::setRST(sl_bool flag)
	{
		_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xFB) | (flag ? 0x04 : 0));
	}
	
	SLIB_INLINE sl_bool TcpSegment::isSYN() const
	{
		return (_dataOffsetAndFlags[1] & 0x02) != 0;
	}
	
	SLIB_INLINE void TcpSegment::setSYN(sl_bool flag)
	{
		_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xFD) | (flag ? 0x02 : 0));
	}
	
	SLIB_INLINE sl_bool TcpSegment::isFIN() const
	{
		return (_dataOffsetAndFlags[1] & 0x01) != 0;
	}
	
	SLIB_INLINE void TcpSegment::setFIN(sl_bool flag)
	{
		_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xFE) | (flag ? 0x01 : 0));
	}
	
	SLIB_INLINE sl_uint16 TcpSegment::getWindowSize() const
	{
		return ((sl_uint16)(_windowSize[0]) << 8) | ((sl_uint16)(_windowSize[1]));
	}
	
	SLIB_INLINE void TcpSegment::setWindowSize(sl_uint16 size)
	{
		_windowSize[0] = (sl_uint8)(size >> 8);
		_windowSize[1] = (sl_uint8)(size);
	}
	
	SLIB_INLINE sl_uint16 TcpSegment::getChecksum() const
	{
		return ((sl_uint16)(_checksum[0]) << 8) | ((sl_uint16)(_checksum[1]));
	}
	
	SLIB_INLINE void TcpSegment::setChecksum(sl_uint16 checksum)
	{
		_checksum[0] = (sl_uint8)(checksum >> 8);
		_checksum[1] = (sl_uint8)(checksum);
	}
	
	SLIB_INLINE sl_uint16 TcpSegment::getUrgentPointer() const
	{
		return ((sl_uint16)(_urgentPointer[0]) << 8) | ((sl_uint16)(_urgentPointer[1]));
	}
	
	SLIB_INLINE void TcpSegment::setUrgentPointer(sl_uint16 urgentPointer)
	{
		_urgentPointer[0] = (sl_uint8)(urgentPointer >> 8);
		_urgentPointer[1] = (sl_uint8)(urgentPointer);
	}
	
	SLIB_INLINE const sl_uint8* TcpSegment::getOptions() const
	{
		return (const sl_uint8*)(this) + HeaderSizeBeforeOptions;
	}
	
	SLIB_INLINE sl_uint8* TcpSegment::getOptions()
	{
		return (sl_uint8*)(this) + HeaderSizeBeforeOptions;
	}
	
	SLIB_INLINE const sl_uint8* TcpSegment::getContent() const
	{
		return (const sl_uint8*)(this) + getHeaderSize();
	}

	SLIB_INLINE sl_uint8* TcpSegment::getContent()
	{
		return (sl_uint8*)(this) + getHeaderSize();
	}


	SLIB_INLINE sl_uint16 UdpDatagram::getSourcePort() const
	{
		return ((sl_uint16)(_sourcePort[0]) << 8) | ((sl_uint16)(_sourcePort[1]));
	}

	SLIB_INLINE void UdpDatagram::setSourcePort(sl_uint16 port)
	{
		_sourcePort[0] = (sl_uint8)(port >> 8);
		_sourcePort[1] = (sl_uint8)(port);
	}

	SLIB_INLINE sl_uint16 UdpDatagram::getDestinationPort() const
	{
		return ((sl_uint16)(_destinationPort[0]) << 8) | ((sl_uint16)(_destinationPort[1]));
	}

	SLIB_INLINE void UdpDatagram::setDestinationPort(sl_uint16 port)
	{
		_destinationPort[0] = (sl_uint8)(port >> 8);
		_destinationPort[1] = (sl_uint8)(port);
	}

	SLIB_INLINE sl_uint16 UdpDatagram::getTotalSize() const
	{
		return ((sl_uint16)(_length[0]) << 8) | ((sl_uint16)(_length[1]));
	}

	SLIB_INLINE void UdpDatagram::setTotalSize(sl_uint16 size)
	{
		_length[0] = (sl_uint8)(size >> 8);
		_length[1] = (sl_uint8)(size);
	}
	
	SLIB_INLINE sl_uint16 UdpDatagram::getChecksum() const
	{
		return ((sl_uint16)(_checksum[0]) << 8) | ((sl_uint16)(_checksum[1]));
	}
	
	SLIB_INLINE void UdpDatagram::setChecksum(sl_uint16 checksum)
	{
		_checksum[0] = (sl_uint8)(checksum >> 8);
		_checksum[1] = (sl_uint8)(checksum);
	}
	
	SLIB_INLINE const sl_uint8* UdpDatagram::getContent() const
	{
		return (const sl_uint8*)(this) + HeaderSize;
	}

	SLIB_INLINE sl_uint8* UdpDatagram::getContent()
	{
		return (sl_uint8*)(this) + HeaderSize;
	}

	SLIB_INLINE sl_uint16 UdpDatagram::getContentSize() const
	{
		return getTotalSize() - HeaderSize;
	}
	
}
