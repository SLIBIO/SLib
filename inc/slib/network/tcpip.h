#ifndef CHECKHEADER_SLIB_NETWORK_TCPIP
#define CHECKHEADER_SLIB_NETWORK_TCPIP

#include "definition.h"

#include "constants.h"
#include "ip_address.h"

#include "../core/mio.h"
#include "../core/expire.h"

SLIB_NETWORK_NAMESPACE_BEGIN

class SLIB_EXPORT TCP_IP
{
public:
	static sl_uint16 calculateOneComplementSum(const void* data, sl_size size, sl_uint32 add = 0);
	
	static sl_uint16 calculateChecksum(const void* data, sl_size size);
	
};

/********************************************************************
	IPv4 Header from RFC 791

	0                   1                   2                   3
	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|Version|  IHL  |Type of Service|          Total Length         |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|         Identification        |Flags|      Fragment Offset    |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|  Time to Live |    Protocol   |         Header Checksum       |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                       Source Address                          |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                    Destination Address                        |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                    Options                    |    Padding    |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+


- by RFC 2474 and RFC 3168, TOS(Type Of Service) field is redefined as following
	0   1   2   3   4   5   6   7
	+---+---+---+---+---+---+---+---+
	|         DSCP          |  ECN  |


- Protocol is defined in RFC 790

********************************************************************/

class SLIB_EXPORT IPv4HeaderFormat
{
public:
	SLIB_INLINE static sl_bool isIPv4Header(const void* header)
	{
		return ((IPv4HeaderFormat*)header)->getVersion() == 4;
	}

	
	// 4 bits, version is 4 for IPv4
	SLIB_INLINE sl_uint32 getVersion() const
	{
		return (_versionAndHeaderLength >> 4) & 0x0F;
	}
	
	// 4 bits, version is 4 for IPv4
	SLIB_INLINE void setVersion(sl_uint32 version = 4)
	{
		_versionAndHeaderLength = (sl_uint8)((_versionAndHeaderLength & 0x0F) | (version << 4));
	}

	
	// 4 bits, get the count of 32bit words for the header including options and padding
	SLIB_INLINE sl_uint32 getHeaderLength() const
	{
		return _versionAndHeaderLength & 0x0F;
	}
	
	// 4 bits, set the count of 32bit words for the header including options and padding (5 if no option is included)
	SLIB_INLINE void setHeaderLength(sl_uint32 length = 5)
	{
		_versionAndHeaderLength = (sl_uint8)((_versionAndHeaderLength & 0xF0) | (length & 0x0F));
	}

	
	// header size in bytes
	SLIB_INLINE sl_uint32 getHeaderSize() const
	{
		return getHeaderLength() << 2;
	}
	
	// header size in bytes
	SLIB_INLINE void setHeaderSize(sl_uint32 size)
	{
		setHeaderLength((size + 3) >> 2);
	}

	
	// 8 bits, TOS is deprecated and replaced with DSCP&ECN
	SLIB_INLINE sl_uint8 getTypeOfService() const
	{
		return _TOS_DSCP_ECN;
	}
	
	// 8 bits, TOS is deprecated and replaced with DSCP&ECN
	SLIB_INLINE void setTypeOfService(sl_uint8 TOS)
	{
		_TOS_DSCP_ECN = TOS;
	}

	
	// 6 bits
	SLIB_INLINE sl_uint32 getDSCP() const
	{
		return ((_TOS_DSCP_ECN >> 2) & 0x3F);
	}
	
	// 6 bits
	SLIB_INLINE void setDSCP(sl_uint32 DSCP)
	{
		_TOS_DSCP_ECN = (sl_uint8)((_TOS_DSCP_ECN & 3) | ((DSCP & 0x3F) << 2));
	}

	
	// 2 bits
	SLIB_INLINE sl_uint32 getECN() const
	{
		return (_TOS_DSCP_ECN & 3);
	}
	
	// 2 bits
	SLIB_INLINE void setECN(sl_uint32 ECN)
	{
		_TOS_DSCP_ECN = (sl_uint8)((_TOS_DSCP_ECN & 0xFC) | (ECN & 3));
	}

	
	// 16 bits, total size (including header and data) in bytes
	SLIB_INLINE sl_uint16 getTotalSize() const
	{
		return MIO::readUint16BE(_totalLength);
	}
	
	// 16 bits, total size (including header and data) in bytes
	SLIB_INLINE void setTotalSize(sl_uint16 size)
	{
		MIO::writeUint16BE(_totalLength, size);
	}

	
	// 16 bits
	SLIB_INLINE sl_uint16 getIdentification() const
	{
		return MIO::readUint16BE(_identification);
	}
	
	// 16 bits
	SLIB_INLINE void setIdentification(sl_uint16 identification)
	{
		MIO::writeUint16BE(_identification, identification);
	}

	
	// true = Do not fragment, false = Fragment
	SLIB_INLINE sl_bool isDF() const
	{
		return (_flagsAndFlagmentOffset[0] & 0x40) != 0;
	}
	
	// true = Do not fragment, false = Fragment
	SLIB_INLINE void setDF(sl_bool flag)
	{
		_flagsAndFlagmentOffset[0] = (sl_uint8)((_flagsAndFlagmentOffset[0] & 0xBF) | (flag ? 0x40 : 0));
	}

	
	// true = More Fragment, false = Last Fragment
	SLIB_INLINE sl_bool isMF() const
	{
		return (_flagsAndFlagmentOffset[0] & 0x20) != 0;
	}
	
	// true = More Fragment, false = Last Fragment
	SLIB_INLINE void setMF(sl_bool flag)
	{
		_flagsAndFlagmentOffset[0] = (sl_uint8)((_flagsAndFlagmentOffset[0] & 0xDF) | (flag ? 0x20 : 0));
	}

	
	// 13 bits, the fragment offset measured in units of 8 octets (64 bits)
	SLIB_INLINE sl_uint32 getFragmentOffset() const
	{
		return (((sl_uint32)(_flagsAndFlagmentOffset[0] & 0x1F)) << 8) | _flagsAndFlagmentOffset[1];
	}
	
	// 13 bits, the fragment offset measured in units of 8 octets (64 bits)
	SLIB_INLINE void setFragmentOffset(sl_uint32 offset)
	{
		_flagsAndFlagmentOffset[1] = (sl_uint8)offset;
		_flagsAndFlagmentOffset[0] = (sl_uint8)((_flagsAndFlagmentOffset[0] & 0xE0) | ((offset >> 8) & 0x1F));
	}

	
	SLIB_INLINE sl_uint8 getTTL() const
	{
		return _timeToLive;
	}
	
	SLIB_INLINE sl_uint8 getTimeToLive() const
	{
		return _timeToLive;
	}
	
	SLIB_INLINE void setTTL(sl_uint8 TTL)
	{
		_timeToLive = TTL;
	}
	
	SLIB_INLINE void setTimeToLive(sl_uint8 TTL)
	{
		_timeToLive = TTL;
	}

	
	// NetworkInternetProtocol
	SLIB_INLINE sl_uint8 getProtocol() const
	{
		return _protocol;
	}
	
	// NetworkInternetProtocol
	SLIB_INLINE void setProtocol(sl_uint8 protocol)
	{
		_protocol = protocol;
	}

	
	SLIB_INLINE sl_uint16 getChecksum() const
	{
		return MIO::readUint16BE(_headerChecksum);
	}
	
	SLIB_INLINE void setChecksum(sl_uint16 checksum)
	{
		MIO::writeUint16BE(_headerChecksum, checksum);
	}

	
	void updateChecksum();
	
	sl_bool checkChecksum() const;

	
	SLIB_INLINE IPv4Address getSourceAddress() const
	{
		return IPv4Address(_sourceIp);
	}
	
	SLIB_INLINE void setSourceAddress(const IPv4Address& address)
	{
		address.get(_sourceIp);
	}

	
	SLIB_INLINE IPv4Address getDestinationAddress() const
	{
		return IPv4Address(_destinationIp);
	}
	
	SLIB_INLINE void setDestinationAddress(const IPv4Address& address)
	{
		address.get(_destinationIp);
	}

	
	SLIB_INLINE const sl_uint8* getOptions() const
	{
		return (const sl_uint8*)(this) + sizeof(IPv4HeaderFormat);
	}
	
	SLIB_INLINE sl_uint8* getOptions()
	{
		return (sl_uint8*)(this) + sizeof(IPv4HeaderFormat);
	}

	
	SLIB_INLINE const sl_uint8* getContent() const
	{
		return (const sl_uint8*)(this) + getHeaderSize();
	}
	
	SLIB_INLINE sl_uint8* getContent()
	{
		return (sl_uint8*)(this) + getHeaderSize();
	}
	
	SLIB_INLINE sl_uint16 getContentSize() const
	{
		return getTotalSize() - getHeaderSize();
	}
	
	SLIB_INLINE void setContentSize(sl_uint16 size)
	{
		setTotalSize(size + getHeaderSize());
	}

	
	// used in TCP/UDP protocol
	sl_uint16 getChecksumForContent(const void* content, sl_uint16 size) const;
	
	sl_uint16 getChecksumForContent() const;

#ifdef check
#undef check
#endif
	static sl_bool check(const void* packetIP, sl_uint32 sizeTotal);
	
	static sl_bool checkHeader(const void* packetIP, sl_uint32 sizeTotal);

	SLIB_INLINE sl_bool isTCP() const
	{
		return getProtocol() == networkInternetProtocol_TCP;
	}
	
	SLIB_INLINE sl_bool isUDP() const
	{
		return getProtocol() == networkInternetProtocol_UDP;
	}
	
	SLIB_INLINE sl_bool isICMP() const
	{
		return getProtocol() == networkInternetProtocol_ICMP;
	}

	SLIB_INLINE sl_bool isFirstFragment() const
	{
		return getFragmentOffset() == 0;
	}

	SLIB_INLINE sl_bool isLastFragment() const
	{
		return !(isMF());
	}

	sl_bool getPortsForTcpUdp(sl_uint16& src, sl_uint16& dst) const;

private:
	sl_uint8 _versionAndHeaderLength;
	sl_uint8 _TOS_DSCP_ECN;
	sl_uint8 _totalLength[2];
	sl_uint8 _identification[2];
	sl_uint8 _flagsAndFlagmentOffset[2];
	sl_uint8 _timeToLive;
	sl_uint8 _protocol;
	sl_uint8 _headerChecksum[2];
	sl_uint8 _sourceIp[4];
	sl_uint8 _destinationIp[4];
	// options and padding
};

/********************************************************************
	TCP Header from RFC 793, RFC 3168, RFC 3540

	0                   1                   2                   3
	0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|          Source Port          |       Destination Port        |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                        Sequence Number                        |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                    Acknowledgment Number                      |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|  Data |Reser|N|C|E|U|A|P|R|S|F|                               |
	| Offset| ved |S|W|C|R|C|S|S|Y|I|            Window             |
	|       |     | |R|E|G|K|H|T|N|N|                               |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|           Checksum            |         Urgent Pointer        |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                    Options                    |    Padding    |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
	|                             data                              |
	+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

********************************************************************/
class SLIB_EXPORT TcpHeaderFormat
{
public:
	SLIB_INLINE sl_uint16 getSourcePort() const
	{
		return MIO::readUint16BE(_sourcePort);
	}
	
	SLIB_INLINE void setSourcePort(sl_uint16 port)
	{
		MIO::writeUint16BE(_sourcePort, port);
	}

	
	SLIB_INLINE sl_uint16 getDestinationPort() const
	{
		return MIO::readUint16BE(_destinationPort);
	}
	
	SLIB_INLINE void setDestinationPort(sl_uint16 port)
	{
		MIO::writeUint16BE(_destinationPort, port);
	}

	
	SLIB_INLINE sl_uint32 getSequenceNumber() const
	{
		return MIO::readUint32BE(_sequenceNumber);
	}
	
	SLIB_INLINE void setSequenceNumber(sl_uint32 num)
	{
		MIO::writeUint32BE(_sequenceNumber, num);
	}

	
	SLIB_INLINE sl_uint32 getAcknowledgmentNumber() const
	{
		return MIO::readUint32BE(_acknowledgmentNumber);
	}
	
	SLIB_INLINE void setAcknowledgmentNumber(sl_uint32 num)
	{
		MIO::writeUint32BE(_acknowledgmentNumber, num);
	}

	
	// 4 bits, the size of the TCP header in 32-bit words
	SLIB_INLINE sl_uint32 getHeaderLength() const
	{
		return _dataOffsetAndFlags[0] >> 4;
	}
	
	// 4 bits, the size of the TCP header in 32-bit words
	SLIB_INLINE void setHeaderLength(sl_uint32 length = 5)
	{
		_dataOffsetAndFlags[0] = (sl_uint8)((_dataOffsetAndFlags[0] & 0x0F) | (length << 4));
	}

	
	// header size in bytes
	SLIB_INLINE sl_uint32 getHeaderSize() const
	{
		return getHeaderLength() << 2;
	}
	
	// header size in bytes
	SLIB_INLINE void setHeaderSize(sl_uint32 size)
	{
		setHeaderLength((size + 3) >> 2);
	}

	
	SLIB_INLINE sl_bool isNS() const
	{
		return (_dataOffsetAndFlags[0] & 1) != 0;
	}
	
	SLIB_INLINE void setNS(sl_bool flag)
	{
		_dataOffsetAndFlags[0] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xFE) | (flag ? 1 : 0));
	}

	
	SLIB_INLINE sl_bool isCWR() const
	{
		return (_dataOffsetAndFlags[1] & 0x80) != 0;
	}
	
	SLIB_INLINE void setCWR(sl_bool flag)
	{
		_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0x7F) | (flag ? 0x80 : 0));
	}

	
	SLIB_INLINE sl_bool isECE() const
	{
		return (_dataOffsetAndFlags[1] & 0x40) != 0;
	}
	
	SLIB_INLINE void setECE(sl_bool flag)
	{
		_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xBF) | (flag ? 0x40 : 0));
	}

	
	SLIB_INLINE sl_bool isURG() const
	{
		return (_dataOffsetAndFlags[1] & 0x20) != 0;
	}
	
	SLIB_INLINE void setURG(sl_bool flag)
	{
		_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xDF) | (flag ? 0x20 : 0));
	}

	
	SLIB_INLINE sl_bool isACK() const
	{
		return (_dataOffsetAndFlags[1] & 0x10) != 0;
	}
	
	SLIB_INLINE void setACK(sl_bool flag)
	{
		_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xEF) | (flag ? 0x10 : 0));
	}

	
	SLIB_INLINE sl_bool isPSH() const
	{
		return (_dataOffsetAndFlags[1] & 0x08) != 0;
	}
	
	SLIB_INLINE void setPSH(sl_bool flag)
	{
		_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xF7) | (flag ? 0x08 : 0));
	}

	
	SLIB_INLINE sl_bool isRST() const
	{
		return (_dataOffsetAndFlags[1] & 0x04) != 0;
	}
	
	SLIB_INLINE void setRST(sl_bool flag)
	{
		_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xFB) | (flag ? 0x04 : 0));
	}

	
	SLIB_INLINE sl_bool isSYN() const
	{
		return (_dataOffsetAndFlags[1] & 0x02) != 0;
	}
	
	SLIB_INLINE void setSYN(sl_bool flag)
	{
		_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xFD) | (flag ? 0x02 : 0));
	}

	
	SLIB_INLINE sl_bool isFIN() const
	{
		return (_dataOffsetAndFlags[1] & 0x01) != 0;
	}
	
	SLIB_INLINE void setFIN(sl_bool flag)
	{
		_dataOffsetAndFlags[1] = (sl_uint8)((_dataOffsetAndFlags[0] & 0xFE) | (flag ? 0x01 : 0));
	}

	
	SLIB_INLINE sl_uint16 getWindowSize() const
	{
		return MIO::readUint16BE(_windowSize);
	}
	
	SLIB_INLINE void setWindowSize(sl_uint16 size)
	{
		MIO::writeUint16BE(_windowSize, size);
	}

	
	SLIB_INLINE sl_uint16 getChecksum() const
	{
		return MIO::readUint16BE(_checksum);
	}
	
	SLIB_INLINE void setChecksum(sl_uint16 checksum)
	{
		MIO::writeUint16BE(_checksum, checksum);
	}
	

	void updateChecksum(const IPv4HeaderFormat* ipv4, sl_uint32 sizeContent);
	
	sl_bool checkChecksum(const IPv4HeaderFormat* ipv4, sl_uint32 sizeContent) const;

	
	SLIB_INLINE sl_uint16 getUrgentPointer() const
	{
		return MIO::readUint16BE(_urgentPointer);
	}
	
	SLIB_INLINE void setUrgentPointer(sl_uint16 urgentPointer)
	{
		MIO::writeUint16BE(_urgentPointer, urgentPointer);
	}

	
	SLIB_INLINE const sl_uint8* getOptions() const
	{
		return (const sl_uint8*)(this) + sizeof(TcpHeaderFormat);
	}
	
	SLIB_INLINE sl_uint8* getOptions()
	{
		return (sl_uint8*)(this) + sizeof(TcpHeaderFormat);
	}

	
	SLIB_INLINE const sl_uint8* getContent() const
	{
		return (const sl_uint8*)(this) + getHeaderSize();
	}
	
	SLIB_INLINE sl_uint8* getContent()
	{
		return (sl_uint8*)(this) + getHeaderSize();
	}

	
	sl_bool check(IPv4HeaderFormat* ip, sl_uint32 sizeContent) const;

private:
	sl_uint8 _sourcePort[2];
	sl_uint8 _destinationPort[2];
	sl_uint8 _sequenceNumber[4];
	sl_uint8 _acknowledgmentNumber[4];
	sl_uint8 _dataOffsetAndFlags[2];
	sl_uint8 _windowSize[2];
	sl_uint8 _checksum[2];
	sl_uint8 _urgentPointer[2];
	// options and padding
};


/********************************************************************
	UDP Header from RFC 768

	0      7 8     15 16    23 24    31
	+--------+--------+--------+--------+
	|     Source      |   Destination   |
	|      Port       |      Port       |
	+--------+--------+--------+--------+
	|                 |                 |
	|     Length      |    Checksum     |
	+--------+--------+--------+--------+
	|
	|          data octets ...
	+---------------- ...

********************************************************************/
class SLIB_EXPORT UdpHeaderFormat
{
public:
	SLIB_INLINE sl_uint16 getSourcePort() const
	{
		return MIO::readUint16BE(_sourcePort);
	}
	
	SLIB_INLINE void setSourcePort(sl_uint16 port)
	{
		MIO::writeUint16BE(_sourcePort, port);
	}

	
	SLIB_INLINE sl_uint16 getDestinationPort() const
	{
		return MIO::readUint16BE(_destinationPort);
	}
	
	SLIB_INLINE void setDestinationPort(sl_uint16 port)
	{
		MIO::writeUint16BE(_destinationPort, port);
	}

	
	// including header and data
	SLIB_INLINE sl_uint16 getTotalSize() const
	{
		return MIO::readUint16BE(_length);
	}
	
	// including header and data
	SLIB_INLINE void setTotalSize(sl_uint16 size)
	{
		MIO::writeUint16BE(_length, size);
	}

	
	SLIB_INLINE sl_uint16 getChecksum() const
	{
		return MIO::readUint16BE(_checksum);
	}
	
	SLIB_INLINE void setChecksum(sl_uint16 checksum)
	{
		MIO::writeUint16BE(_checksum, checksum);
	}

	
	void updateChecksum(const IPv4HeaderFormat* ipv4);
	
	sl_bool checkChecksum(const IPv4HeaderFormat* ipv4) const;

	
	SLIB_INLINE const sl_uint8* getContent() const
	{
		return (const sl_uint8*)(this) + sizeof(sizeof(UdpHeaderFormat));
	}
	
	SLIB_INLINE sl_uint8* getContent()
	{
		return (sl_uint8*)(this) + sizeof(sizeof(UdpHeaderFormat));
	}

	
	SLIB_INLINE sl_uint16 getContentSize() const
	{
		return getTotalSize() - sizeof(UdpHeaderFormat);
	}
	
	SLIB_INLINE void setContentSize(sl_uint16 size)
	{
		setTotalSize(size + sizeof(UdpHeaderFormat));
	}

	
	sl_bool check(IPv4HeaderFormat* ip, sl_uint32 sizeContent) const;

private:
	sl_uint8 _sourcePort[2];
	sl_uint8 _destinationPort[2];
	sl_uint8 _length[2];
	sl_uint8 _checksum[2];
};

class SLIB_EXPORT IPv4PacketIdentifier
{
public:
	IPv4Address source;
	IPv4Address destination;
	sl_uint16 identification;
	sl_uint8 protocol;

public:
	sl_bool operator==(const IPv4PacketIdentifier& other) const;

public:
	sl_uint32 hashCode() const;
	
};

template <>
SLIB_INLINE sl_uint32 Hash<IPv4PacketIdentifier>::hash(const IPv4PacketIdentifier& v)
{
	return v.hashCode();
}

struct SLIB_EXPORT IPv4Fragment
{
	sl_uint32 start;
	sl_uint32 end;
	Memory data;
};

class SLIB_EXPORT IPv4FragmentedPacket : public Referable
{
public:
	Memory header;
	List<IPv4Fragment> fragments;
	sl_uint32 sizeAccumulated;
	sl_uint32 sizeContent;
};

class SLIB_EXPORT IPv4Fragmentation : public Object
{
public:
	IPv4Fragmentation();
	~IPv4Fragmentation();

public:
	void setupExpiringDuration(sl_uint32 ms, const Ref<AsyncLoop>& loop);
	
	void setupExpiringDuration(sl_uint32 ms);

	static sl_bool isNeededCombine(const void* ip, sl_uint32 size, sl_bool flagCheckedHeader = sl_false);

	// returns a combined IP packet
	Memory combineFragment(const void* ip, sl_uint32 size, sl_bool flagCheckedHeader = sl_false);

	List<Memory> makeFragments(const IPv4HeaderFormat* header, const void* ipContent, sl_uint32 sizeContent, sl_uint32 mtu = 1500);

	static List<Memory> makeFragments(const IPv4HeaderFormat* header, sl_uint16 identifier, const void* ipContent, sl_uint32 sizeContent, sl_uint32 mtu = 1500);
	
protected:
	ExpiringMap<IPv4PacketIdentifier, Ref<IPv4FragmentedPacket> > m_packets;
	sl_int32 m_currentIdentifier;
};

SLIB_NETWORK_NAMESPACE_END

#endif
