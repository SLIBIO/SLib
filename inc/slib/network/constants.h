#ifndef CHECKHEADER_SLIB_NETWORK_CONSTANTS
#define CHECKHEADER_SLIB_NETWORK_CONSTANTS

#include "definition.h"

SLIB_NETWORK_NAMESPACE_BEGIN

// EtherType
enum NetworkLinkProtocolType {
	networkLinkProtocol_All = 0x0003,
	networkLinkProtocol_IPv4 = 0x0800,
	networkLinkProtocol_ARP = 0x0806,
	networkLinkProtocol_RARP = 0x8035,
	networkLinkProtocol_IPv6 = 0x86DD
};

enum NetworkLinkDeviceType
{
	networkLinkDevice_LoopbackBSD = 0, //  BSD loopback encapsulation; the link layer header is a 4-byte field, in host byte order, containing a PF_ value from socket.h for the network-layer protocol of the packet. 
	networkLinkDevice_Ethernet = 1, // 6 bytes destination mac, 6 bytes source mac, 2 bytes Ethertype
	networkLinkDevice_PPP = 9, // Point-to-Point Protocol Datalink layer
	networkLinkDevice_PPPoE = 51, // PPP over Ethernet
	networkLinkDevice_Raw = 101, // Raw IP; the packet begins with an IPv4 or IPv6 header, with the "version" field of the header indicating whether it's an IPv4 or IPv6 header. 
	networkLinkDevice_802_11 = 105, // IEEE 802.11 wireless LAN
	networkLinkDevice_LoobackOpenBSD = 108, // OpenBSD loopback encapsulation; the link-layer header is a 4-byte field, in network byte order, containing a PF_ value from OpenBSD's socket.h for the network-layer protocol of the packet. 
	networkLinkDevice_Linux = 113 //  Linux "cooked" capture encapsulation. (for "any" or PPP devices)
};

enum NetworkInternetProtocolType
{
	networkInternetProtocol_ICMP = 1, // Internet Control Message Protocol, RFC 792
	networkInternetProtocol_IGMP = 2, // Internet Group Management Protocol, RFC 1112
	networkInternetProtocol_GGP = 3, // Gateway-to-Gateway Protocol, RFC 823
	networkInternetProtocol_IPinIP = 4, // IP in IP, RFC 2003
	networkInternetProtocol_ST = 5, // Internet Stream Protocol, RFC 1190
	networkInternetProtocol_TCP = 6, // Transmission Control Protocol, RFC 793
	networkInternetProtocol_NVP = 11, // Network Voice Protocol, RFC 741
	networkInternetProtocol_UDP = 17, // User Datagram, RFC 768
	networkInternetProtocol_RDP = 27, // Reliable Datagram Protocol, RFC 908
	networkInternetProtocol_IRTP = 28, // Internet Reliable Transaction Protocol, RFC 938
	networkInternetProtocol_Raw = 0xFF // Reserved for Socket api, for sendind the raw ip packets of any kind of protocols (sending only, not receiving)
};

SLIB_NETWORK_NAMESPACE_END

#endif
