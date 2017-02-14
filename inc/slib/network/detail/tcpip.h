#ifndef CHECKHEADER_SLIB_NETWORK_DETAIL_TCPIP
#define CHECKHEADER_SLIB_NETWORK_DETAIL_TCPIP

#include "../tcpip.h"

namespace slib
{

	SLIB_INLINE sl_bool IPv4Packet::isIPv4Header(const void* header)
	{
		return ((IPv4Packet*)header)->getVersion() == 4;
	}

	SLIB_INLINE sl_uint32 IPv4Packet::getHeaderLength() const
	{
		return _versionAndHeaderLength & 0x0F;
	}

	SLIB_INLINE void IPv4Packet::setHeaderLength(sl_uint32 length)
	{
		_versionAndHeaderLength = (sl_uint8)((_versionAndHeaderLength & 0xF0) | (length & 0x0F));
	}

	SLIB_INLINE sl_uint32 IPv4Packet::getHeaderSize() const
	{
		return ((sl_uint32)(_versionAndHeaderLength & 0x0F)) << 2;
	}

	SLIB_INLINE void IPv4Packet::setHeaderSize(sl_uint32 size)
	{
		setHeaderLength((size + 3) >> 2);
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

	SLIB_INLINE NetworkInternetProtocol IPv4Packet::getProtocol() const
	{
		return (NetworkInternetProtocol)_protocol;
	}

	SLIB_INLINE void IPv4Packet::setProtocol(NetworkInternetProtocol protocol)
	{
		_protocol = (sl_uint8)(protocol);
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

	SLIB_INLINE sl_bool IPv4Packet::isTCP() const
	{
		return getProtocol() == NetworkInternetProtocol::TCP;
	}

	SLIB_INLINE sl_bool IPv4Packet::isUDP() const
	{
		return getProtocol() == NetworkInternetProtocol::UDP;
	}

	SLIB_INLINE sl_bool IPv4Packet::isICMP() const
	{
		return getProtocol() == NetworkInternetProtocol::ICMP;
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

	SLIB_INLINE sl_uint32 TcpSegment::getHeaderLength() const
	{
		return _dataOffsetAndFlags[0] >> 4;
	}

	SLIB_INLINE void TcpSegment::setHeaderLength(sl_uint32 length)
	{
		_dataOffsetAndFlags[0] = (sl_uint8)((_dataOffsetAndFlags[0] & 0x0F) | (length << 4));
	}

	SLIB_INLINE sl_uint32 TcpSegment::getHeaderSize() const
	{
		return (_dataOffsetAndFlags[0] >> 4) << 2;
	}

	SLIB_INLINE void TcpSegment::setHeaderSize(sl_uint32 size)
	{
		setHeaderLength((size + 3) >> 2);
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

#endif
