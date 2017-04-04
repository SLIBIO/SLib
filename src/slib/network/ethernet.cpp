/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/network/ethernet.h"

#include "slib/network/tcpip.h"

namespace slib
{
	
	EthernetMacTable::EthernetMacTable()
	{
	}

	EthernetMacTable::~EthernetMacTable()
	{
	}

	void EthernetMacTable::add(const IPv4Address& ip, const MacAddress& mac)
	{
		m_table.put(ip, mac);
	}

	sl_bool EthernetMacTable::getMacAddress(const IPv4Address& ip, MacAddress* out)
	{
		if (ip.isBroadcast()) {
			if (out) {
				out->setBroadcast();
			}
			return sl_true;
		}
		if (ip.isMulticast()) {
			if (out) {
				out->makeMulticast(ip);
			}
			return sl_true;
		}
		return m_table.get(ip, out);
	}

	void EthernetMacTable::parseEthernetFrame(const void* _frame, sl_uint32 sizeFrame, sl_bool flagUseSource, sl_bool flagUseDestination)
	{
		if (sizeFrame > EthernetFrame::HeaderSize) {
			EthernetFrame* frame = (EthernetFrame*)_frame;
			if (frame->getProtocol() == NetworkLinkProtocol::IPv4) {
				IPv4Packet* ip = (IPv4Packet*)(frame->getContent());
				sl_uint32 sizeIP = sizeFrame - EthernetFrame::HeaderSize;
				if (IPv4Packet::check(ip, sizeIP)) {
					if (flagUseSource) {
						MacAddress mac = frame->getSourceAddress();
						if (mac.isNotMulticast()) {
							IPv4Address addr = ip->getSourceAddress();
							if (addr.isHost()) {
								add(addr, mac);
							}
						}
					}
					if (flagUseDestination) {
						MacAddress mac = frame->getDestinationAddress();
						if (mac.isNotMulticast()) {
							IPv4Address addr = ip->getDestinationAddress();
							if (addr.isHost()) {
								add(addr, mac);
							}
						}
					}
				}
			}
		}
	}

}
