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

#include <slib.h>

using namespace slib;

/*
 
 On Linux, you have run following command in build output directory
 
		sudo setcap cap_net_raw=eip ExamplePing
 
 */

int main(int argc, const char * argv[])
{
	Println("Please input host address:");
	
	String hostName = Console::readLine().trim();
	IPv4Address hostAddress;
	if (! hostAddress.setHostName(hostName)) {
		Println("Can't resolve the host name: %s", hostName);
		return -1;
	}
	
#ifdef SLIB_PLATFORM_IS_MACOS
	Ref<Socket> socket = Socket::openDatagram(NetworkInternetProtocol::ICMP);
#else
	Ref<Socket> socket = Socket::openRaw(NetworkInternetProtocol::ICMP);
#endif
	if (socket.isNull()) {
		Println("Can't open socket.");
		return -1;
	}
	
	socket->setNonBlockingMode(sl_true);
	
	Ref<SocketEvent> event = SocketEvent::createRead(socket);
	if (event.isNull()) {
		Println("Can't create event.");
		return -1;
	}
	
	Ref<Thread> threadReceive = Thread::start([socket, event]() {
		while (Thread::isNotStoppingCurrent()) {
			event->waitEvents(100);
			sl_uint8 buf[4096];
			SocketAddress receivedHostAddress;
			sl_int32 n = socket->receiveFrom(receivedHostAddress, buf, 4096);
			if (n > 0) {
				if (IPv4Packet::checkHeaderSize(buf, n)) {
					IPv4Packet* packet = (IPv4Packet*)buf;
					if (packet->getProtocol() == NetworkInternetProtocol::ICMP) {
						IcmpHeaderFormat* icmp = (IcmpHeaderFormat*)(packet->getContent());
						if (icmp->check(n - packet->getHeaderSize())) {
							if (icmp->getType() == IcmpType::EchoReply) {
								Println("Received ICMP Echo Reply: Host=%s Sequence=%d", receivedHostAddress.ip.toString(), icmp->getEchoSequenceNumber());
							}
						}
					}
				}
			}
		}
	});
	if (threadReceive.isNull()) {
		Println("Can't start receiving thread.");
		return -1;
	}
	
	Ref<Thread> threadSend = Thread::start([socket, hostAddress]() {
		sl_uint16 seq = 1;
		while (Thread::isNotStoppingCurrent()) {
			sl_uint8 buf[sizeof(IcmpHeaderFormat) + 100] = {0};
			IcmpHeaderFormat* icmp = (IcmpHeaderFormat*)(buf);
			icmp->setType(IcmpType::Echo);
			icmp->setEchoSequenceNumber(seq);
			icmp->updateChecksum(sizeof(buf));
			sl_int32 n = socket->sendTo(SocketAddress(hostAddress, 0), buf, sizeof(buf));
			if (n > 0) {
				Println("Sent ICMP Echo: host=%s Sequence=%d", hostAddress.toString(), seq);
			} else {
				Println("Failed to send ICMP Echo: host=%s Sequence=%d", hostAddress.toString(), seq);
			}
			seq++;
			Thread::sleep(1000);
		}
	});
	if (threadReceive.isNull()) {
		Println("Can't start sending thread.");
		return -1;
	}

	for (;;) {
		Println("Input q to exit:");
		String s = Console::readLine().trim();
		if (s == "q") {
			break;
		}
	}
	
	threadReceive->finishAndWait();
	threadSend->finishAndWait();
	
	return 0;
}
