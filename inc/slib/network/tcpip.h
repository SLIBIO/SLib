#ifndef CHECKHEADER_SLIB_NETWORK_TCPIP
#define CHECKHEADER_SLIB_NETWORK_TCPIP

#include "definition.h"

#include "constants.h"
#include "ip_address.h"

#include "../core/expire.h"

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

namespace slib
{
	class SLIB_EXPORT TCP_IP
	{
	public:
		static sl_uint16 calculateOneComplementSum(const void* data, sl_size size, sl_uint32 add = 0);

		static sl_uint16 calculateChecksum(const void* data, sl_size size);

	};

	class SLIB_EXPORT IPv4Packet
	{
	public:
		static sl_bool isIPv4Header(const void* header);
		
		// 4 bits, version is 4 for IPv4
		sl_uint32 getVersion() const;
		
		// 4 bits, version is 4 for IPv4
		void setVersion(sl_uint32 version = 4);

		// 4 bits, get the count of 32bit words for the header including options and padding
		sl_uint32 getHeaderLength() const;
		
		// 4 bits, set the count of 32bit words for the header including options and padding (5 if no option is included)
		void setHeaderLength(sl_uint32 length = 5);
		
		// header size in bytes
		sl_uint32 getHeaderSize() const;
		
		// header size in bytes
		void setHeaderSize(sl_uint32 size);
		
		// 8 bits, TOS is deprecated and replaced with DSCP&ECN
		sl_uint8 getTypeOfService() const;
		
		// 8 bits, TOS is deprecated and replaced with DSCP&ECN
		void setTypeOfService(sl_uint8 TOS);
		
		// 6 bits
		sl_uint32 getDSCP() const;
		
		// 6 bits
		void setDSCP(sl_uint32 DSCP);
		
		// 2 bits
		sl_uint32 getECN() const;
		
		// 2 bits
		void setECN(sl_uint32 ECN);
		
		// 16 bits, total size (including header and data) in bytes
		sl_uint16 getTotalSize() const;
		
		// 16 bits, total size (including header and data) in bytes
		void setTotalSize(sl_uint16 size);

		// 16 bits
		sl_uint16 getIdentification() const;
		
		// 16 bits
		void setIdentification(sl_uint16 identification);

		// true = Do not fragment, false = Fragment
		sl_bool isDF() const;
		
		// true = Do not fragment, false = Fragment
		void setDF(sl_bool flag);

		// true = More Fragment, false = Last Fragment
		sl_bool isMF() const;
		
		// true = More Fragment, false = Last Fragment
		void setMF(sl_bool flag);
		
		// 13 bits, the fragment offset measured in units of 8 octets (64 bits)
		sl_uint32 getFragmentOffset() const;
		
		// 13 bits, the fragment offset measured in units of 8 octets (64 bits)
		void setFragmentOffset(sl_uint32 offset);
		
		// Time To Live
		sl_uint8 getTTL() const;
		
		// Time To Live
		void setTTL(sl_uint8 TTL);
		
		NetworkInternetProtocol getProtocol() const;
		
		void setProtocol(NetworkInternetProtocol protocol);
		
		sl_uint16 getChecksum() const;
		
		void setChecksum(sl_uint16 checksum);
		
		void updateChecksum();
		
		sl_bool checkChecksum() const;

		IPv4Address getSourceAddress() const;
		
		void setSourceAddress(const IPv4Address& address);
		
		IPv4Address getDestinationAddress() const;
		
		void setDestinationAddress(const IPv4Address& address);
		
		const sl_uint8* getOptions() const;
		
		sl_uint8* getOptions();
		
		const sl_uint8* getContent() const;
		
		sl_uint8* getContent();
		
		sl_uint16 getContentSize() const;
		
		// used in TCP/UDP protocol
		sl_uint16 getChecksumForContent(const void* content, sl_uint16 size) const;
		
		sl_uint16 getChecksumForContent() const;

#ifdef check
#undef check
#endif
		static sl_bool check(const void* packetIP, sl_uint32 sizeTotal);
		
		static sl_bool checkHeader(const void* packetIP, sl_uint32 sizeTotal);

		sl_bool isTCP() const;
		
		sl_bool isUDP() const;
		
		sl_bool isICMP() const;

		sl_bool isFirstFragment() const;
		
		sl_bool isLastFragment() const;
		
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


	class SLIB_EXPORT TcpSegment
	{
	public:
		sl_uint16 getSourcePort() const;
		
		void setSourcePort(sl_uint16 port);
		
		sl_uint16 getDestinationPort() const;
		
		void setDestinationPort(sl_uint16 port);
		
		sl_uint32 getSequenceNumber() const;
		
		void setSequenceNumber(sl_uint32 num);

		sl_uint32 getAcknowledgmentNumber() const;
		
		void setAcknowledgmentNumber(sl_uint32 num);
		
		// 4 bits, the size of the TCP header in 32-bit words
		sl_uint32 getHeaderLength() const;
		
		// 4 bits, the size of the TCP header in 32-bit words
		void setHeaderLength(sl_uint32 length = 5);

		// header size in bytes
		sl_uint32 getHeaderSize() const;
		
		// header size in bytes
		void setHeaderSize(sl_uint32 size);

		sl_bool isNS() const;
		
		void setNS(sl_bool flag);
		
		sl_bool isCWR() const;
		
		void setCWR(sl_bool flag);
		
		sl_bool isECE() const;
		
		void setECE(sl_bool flag);
		
		sl_bool isURG() const;
		
		void setURG(sl_bool flag);
		
		sl_bool isACK() const;
		
		void setACK(sl_bool flag);

		sl_bool isPSH() const;
		
		void setPSH(sl_bool flag);
		
		sl_bool isRST() const;
		
		void setRST(sl_bool flag);
		
		sl_bool isSYN() const;
		
		void setSYN(sl_bool flag);

		sl_bool isFIN() const;
		
		void setFIN(sl_bool flag);
		
		sl_uint16 getWindowSize() const;
		
		void setWindowSize(sl_uint16 size);
		
		sl_uint16 getChecksum() const;
		
		void setChecksum(sl_uint16 checksum);

		void updateChecksum(const IPv4Packet* ipv4, sl_uint32 sizeContent);
		
		sl_bool checkChecksum(const IPv4Packet* ipv4, sl_uint32 sizeContent) const;

		sl_bool check(IPv4Packet* ip, sl_uint32 sizeContent) const;

		sl_uint16 getUrgentPointer() const;
		
		void setUrgentPointer(sl_uint16 urgentPointer);
		
		const sl_uint8* getOptions() const;
		
		sl_uint8* getOptions();
		
		const sl_uint8* getContent() const;
		
		sl_uint8* getContent();

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


	class SLIB_EXPORT UdpDatagram
	{
	public:
		enum
		{
			HeaderSize = 8
		};
		
	public:
		sl_uint16 getSourcePort() const;
		
		void setSourcePort(sl_uint16 port);
		
		sl_uint16 getDestinationPort() const;
		
		void setDestinationPort(sl_uint16 port);
		
		// including header and data
		sl_uint16 getTotalSize() const;
		
		// including header and data
		void setTotalSize(sl_uint16 size);

		sl_uint16 getChecksum() const;
		
		void setChecksum(sl_uint16 checksum);
		
		void updateChecksum(const IPv4Packet* ipv4);
		
		sl_bool checkChecksum(const IPv4Packet* ipv4) const;

		sl_bool check(IPv4Packet* ip, sl_uint32 sizeContent) const;
		
		const sl_uint8* getContent() const;
		
		sl_uint8* getContent();
		
		sl_uint16 getContentSize() const;
		
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
		NetworkInternetProtocol protocol;

	public:
		sl_bool operator==(const IPv4PacketIdentifier& other) const;

	public:
		sl_uint32 hashCode() const;
		
	};

	template <>
	class Hash<IPv4PacketIdentifier>
	{
	public:
		sl_uint32 operator()(const IPv4PacketIdentifier& v) const;
		
	};

	class SLIB_EXPORT IPv4Fragment
	{
	public:
		sl_uint32 start;
		sl_uint32 end;
		Memory data;
		
	public:
		IPv4Fragment();
		
		~IPv4Fragment();
		
	};

	class SLIB_EXPORT IPv4FragmentedPacket : public Referable
	{
	public:
		Memory header;
		List<IPv4Fragment> fragments;
		sl_uint32 sizeAccumulated;
		sl_uint32 sizeContent;
		
	public:
		IPv4FragmentedPacket();
		
		~IPv4FragmentedPacket();
		
	};

	class SLIB_EXPORT IPv4Fragmentation : public Object
	{
	public:
		IPv4Fragmentation();
		
		~IPv4Fragmentation();

	public:
		void setupExpiringDuration(sl_uint32 ms, const Ref<DispatchLoop>& loop);
		
		void setupExpiringDuration(sl_uint32 ms);

		static sl_bool isNeededCombine(const void* ip, sl_uint32 size, sl_bool flagCheckedHeader = sl_false);

		// returns a combined IP packet
		Memory combineFragment(const void* ip, sl_uint32 size, sl_bool flagCheckedHeader = sl_false);

		List<Memory> makeFragments(const IPv4Packet* header, const void* ipContent, sl_uint32 sizeContent, sl_uint32 mtu = 1500);

		static List<Memory> makeFragments(const IPv4Packet* header, sl_uint16 identifier, const void* ipContent, sl_uint32 sizeContent, sl_uint32 mtu = 1500);
		
	protected:
		ExpiringMap<IPv4PacketIdentifier, Ref<IPv4FragmentedPacket> > m_packets;
		sl_int32 m_currentIdentifier;
		
	};

}

#include "detail/tcpip.h"

#endif
