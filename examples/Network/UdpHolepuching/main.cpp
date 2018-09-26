/*
 *  Copyright (c) 2018 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <slib.h>

using namespace slib;

int main(int argc, const char * argv[])
{
	Println("Welcome to UDP Holepunching test tool. Input q to exit.");

	SocketAddress stunAddress;
	for (;;) {
		Println("Please input STUN server address. Format: HOST[:PORT], default: stun.gnunet.org");
		String strStunAddress = Console::readLine().trim();
		if (strStunAddress == "q") {
			return 0;
		}
		if (strStunAddress.isEmpty()) {
			strStunAddress = "stun.gnunet.org";
		}
		if (stunAddress.setHostAddress(strStunAddress)) {
			if (stunAddress.port == 0) {
				stunAddress.port = SLIB_NETWORK_STUN_PORT;
			}
			Println("STUN Server: %s", stunAddress.toString());
			break;
		} else {
			Println("Invalid STUN server address.");
		}
	}
	
	AsyncUdpSocketParam param;
	param.onReceiveFrom = [](AsyncUdpSocket* socket, const SocketAddress& addressFrom, void* packet, sl_uint32 size) {
		StunPacket* stunPacket = (StunPacket*)packet;
		StunAttributes stunAttrs;
		if (stunPacket->parseAttributes(size, stunAttrs)) {
			if (stunPacket->getMessageClass() == StunMessageClass::Response) {
				if (stunPacket->getMethod() == StunMethod::Binding) {
					if (stunAttrs.mappedAddress.isValid() || stunAttrs.xorMappedAddress.isValid()) {
						SocketAddress address;
						if (stunAttrs.xorMappedAddress.isValid()) {
							address = stunAttrs.xorMappedAddress;
						} else {
							address = stunAttrs.mappedAddress;
						}
						static SocketAddress stunBindedAddress;
						if (stunBindedAddress != address) {
							Println("STUN Binding Response: %s", address.toString());
							stunBindedAddress = address;
						}
					}
				}
			} else if (stunPacket->getMessageClass() == StunMessageClass::ErrorResponse) {
				Println("STUN Error Response: %d %s", (sl_uint32)(stunAttrs.errorCode), stunAttrs.errorReasonPhase);
			}
		} else {
			Println("Received Packet, From: %s, Data: %s", addressFrom.toString(), String((sl_char8*)packet, size));
		}
	};
	
	Println("Input the UDP binding address. Format: [HOST:]PORT, Default: 0");
	String strAddressBind = Console::readLine().trim();
	sl_uint32 bindPort;
	if (strAddressBind.parseUint32(10, &bindPort)) {
		param.bindAddress.port = (sl_uint16)bindPort;
	} else {
		param.bindAddress.setString(strAddressBind);
	}
	
	for (;;) {
		Println("Is IPv6? Y/N, default: N");
		String v = Console::readLine().trim();
		if (v == "Y" || v == "y") {
			param.flagIPv6 = sl_true;
			break;
		}
		if (v == "N" || v == "n" || v.isEmpty()) {
			break;
		}
	}

	Println("Creating Async UDP Socket, %s, Bind: %s", param.flagIPv6 ? "IPv6" : "IPv4", param.bindAddress.toString());

	Ref<AsyncUdpSocket> socket = AsyncUdpSocket::create(param);
	if (socket.isNull()) {
		Println("Unable to create Async UDP Socket.");
		return -1;
	}
	
	Ref<Timer> timerSTUN = Timer::start([socket, stunAddress](Timer* timer) {
		static sl_uint32 transactionID[3] = {0};
		transactionID[0]++;
		StunAttributes attrs;
		attrs.flagHaveFingerprint = sl_true;
		Memory packet = StunPacket::buildPacket(StunMessageClass::Request, StunMethod::Binding, transactionID, attrs);
		if (packet.isNotNull()) {
			socket->sendTo(stunAddress, packet);
		}
	}, 2000);
	
	for (;;) {
		Println("Please input target address. Format: IP:PORT");
		String strAddress = Console::readLine().trim();
		if (strAddress == "q") {
			break;
		}
		SocketAddress address(strAddress);
		if (address.isInvalid()) {
			continue;
		}
		Println("Please input sending text");
		String text = Console::readLine();
		if (text.isEmpty()) {
			text = " ";
		}
		socket->sendTo(address, text.getData(), (sl_uint32)(text.getLength()));
	}
	
	return 0;
}
