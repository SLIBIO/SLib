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
