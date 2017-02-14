#ifndef CHECKHEADER_SLIB_NETWORK_CONSTANTS
#define CHECKHEADER_SLIB_NETWORK_CONSTANTS

#include "definition.h"

namespace slib
{

	// EtherType
	enum class NetworkLinkProtocol
	{
		All = 0x0003,
		IPv4 = 0x0800,
		ARP = 0x0806,
		RARP = 0x8035,
		IPv6 = 0x86DD
	};
	
	enum class NetworkLinkDeviceType
	{
		LoopbackBSD = 0, //  BSD loopback encapsulation; the link layer header is a 4-byte field, in host byte order, containing a PF_ value from socket.h for the network-layer protocol of the packet.
		Ethernet = 1, // 6 bytes destination mac, 6 bytes source mac, 2 bytes Ethertype
		PPP = 9, // Point-to-Point Protocol Datalink layer
		PPPoE = 51, // PPP over Ethernet
		Raw = 101, // Raw IP; the packet begins with an IPv4 or IPv6 header, with the "version" field of the header indicating whether it's an IPv4 or IPv6 header.
		IEEE802_11 = 105, // IEEE 802.11 wireless LAN
		LoobackOpenBSD = 108, // OpenBSD loopback encapsulation; the link-layer header is a 4-byte field, in network byte order, containing a PF_ value from OpenBSD's socket.h for the network-layer protocol of the packet.
		Linux = 113 //  Linux "cooked" capture encapsulation. (for "any" or PPP devices)
	};
	
	enum class NetworkInternetProtocol
	{
		ICMP = 1, // Internet Control Message Protocol, RFC 792
		IGMP = 2, // Internet Group Management Protocol, RFC 1112
		GGP = 3, // Gateway-to-Gateway Protocol, RFC 823
		IPinIP = 4, // IP in IP, RFC 2003
		ST = 5, // Internet Stream Protocol, RFC 1190
		TCP = 6, // Transmission Control Protocol, RFC 793
		NVP = 11, // Network Voice Protocol, RFC 741
		UDP = 17, // User Datagram, RFC 768
		RDP = 27, // Reliable Datagram Protocol, RFC 908
		IRTP = 28, // Internet Reliable Transaction Protocol, RFC 938
		Raw = 0xFF // Reserved for Socket api, for sendind the raw ip packets of any kind of protocols (sending only, not receiving)
	};

}

#endif
