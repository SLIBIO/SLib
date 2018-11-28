/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
 */

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
		IPv6_HopOption = 0, // IPv6 Hop-by-Hop Option, RFC 8200
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
		IPv6_Route = 43, // Routing Header for IPv6, RFC 8200
		IPv6_Fragment = 44, // Fragment Header for IPv6, RFC 8200
		IPv6_ICMP = 58, // ICMP for IPv6, RFC 4443, RFC 4884
		IPv6_NoNext = 59, // No Next Header for IPv6, RFC 8200
		IPv6_Options = 60, // Destination Options for IPv6, RFC 8200
		Raw = 0xFF // Reserved for Socket api, for sendind the raw ip packets of any kind of protocols (sending only, not receiving)
	};

}

#endif
