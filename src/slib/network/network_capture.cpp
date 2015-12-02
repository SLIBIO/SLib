#include "../../../inc/slib/core/definition.h"

#include "../../../inc/slib/network/capture.h"
#include "../../../inc/slib/core/thread.h"
#include "../../../inc/slib/core/log.h"

#define TAG "NetCapture"

#include "../../../inc/slib/network/socket.h"
#include "../../../inc/slib/network/event.h"
#include "../../../inc/slib/network/tcpip.h"
#include "../../../inc/slib/network/ethernet.h"

#define MAX_PACKET_SIZE 65535

SLIB_NETWORK_NAMESPACE_BEGIN
NetCapture::NetCapture()
{
}

NetCapture::~NetCapture()
{
}

void NetCapture::_onCapturePacket(NetCapturePacket* packet)
{
	PtrLocker<INetCaptureListener> listener(getListener());
	if (listener.isNotNull()) {
		listener->onCapturePacket(this, packet);
	}
}

sl_bool NetCapture::setLinkType(sl_uint32 type)
{
	return sl_false;
}

String NetCapture::getLastErrorMessage()
{
	return String::null();
}

class _NetRawPacketCapture : public NetCapture
{
public:
	Ref<Socket> m_socket;
	
	sl_uint32 m_deviceType;
	sl_uint32 m_ifaceIndex;
	Memory m_bufPacket;	
	Ref<Thread> m_thread;
	
	sl_bool m_flagRunning;
	
	_NetRawPacketCapture()
	{	
		m_flagRunning = sl_true;
	}
	
	~_NetRawPacketCapture()
	{
		release();
	}
	
	static Ref<_NetRawPacketCapture> create(const NetCaptureParam& param)
	{
		Ref<_NetRawPacketCapture> ret;
		sl_uint32 iface = 0;
		String deviceName = param.deviceName;
		if (deviceName.isNotEmpty()) {
			iface = NetworkDevice::getInterfaceIndexFromName(deviceName);
			if (iface == 0) {
				SLIB_LOG_ERROR(TAG, "Can not find the interface index of device: " + deviceName);
				return ret;
			}
		}
		Ref<Socket> socket;
		sl_uint32 deviceType = param.preferedLinkDeviceType;
		if (deviceType == networkLinkDevice_Raw) {
			socket = Socket::openPacketDatagram(networkLinkProtocol_All);
		} else {
			deviceType = networkLinkDevice_Ethernet;
			socket = Socket::openPacketRaw(networkLinkProtocol_All);
		}
		if (socket.isNotNull()) {
			if (iface > 0) {
				if (!(socket->setOption_bindToDevice(deviceName))) {
					SLIB_LOG(TAG, "Can not bind to the network device: " + deviceName);
				}
			}
			Memory mem = Memory::create(MAX_PACKET_SIZE);
			if (mem.isNotEmpty()) {
				ret = new _NetRawPacketCapture;
				if (ret.isNotNull()) {
					ret->m_bufPacket = mem;
					ret->m_socket = socket;
					ret->m_deviceType = deviceType;
					ret->m_ifaceIndex = iface;
					ret->setListener(param.listener);
					ret->m_thread = Thread::start(SLIB_CALLBACK_CLASS(_NetRawPacketCapture, _run, ret.get()));
					if (ret->m_thread.isNull()) {
						ret.setNull();
					}
				}
			}
		} else {
			SLIB_LOG_ERROR(TAG, "Failed to create Packet socket");
		}
		return ret;
	}
	
	void release()
	{
		MutexLocker lock(getLocker());
		if (!m_flagRunning) {
			return;
		}
		m_flagRunning = sl_false;
		
		if (m_thread.isNotNull()) {
			m_thread->finishAndWait();
			m_thread.setNull();
		}
		m_socket.setNull();
	}
	
	sl_bool isRunning()
	{
		return m_flagRunning;
	}
	
	void _run()
	{
		NetCapturePacket packet;

		Ref<Socket> socket = m_socket;
		if (socket.isNull()) {
			return;
		}
		socket->setNonBlockingMode(sl_true);
		Ref<SocketEvent> event = SocketEvent::createRead(socket);
		if (event.isNull()) {
			return;
		}
		
		sl_uint8* buf = (sl_uint8*)(m_bufPacket.getBuf());
		sl_uint32 sizeBuf = (sl_uint32)(m_bufPacket.getSize());
		
		while (Thread::isNotStoppingCurrent()) {
			while (1) {
				L2PacketInfo info;
				sl_int32 n = socket->receivePacket(buf, sizeBuf, info);
				if (n > 0) {
					packet.data = buf;
					packet.length = n;
					packet.time = 0;
					_onCapturePacket(&packet);
				} else {
					break;
				}
			}
			event->wait();
		}
	}

	sl_uint32 getLinkType()
	{
		return m_deviceType;
	}

	sl_bool sendPacket(const void* buf, sl_uint32 size)
	{
		if (m_ifaceIndex == 0) {
			return sl_false;
		}
		if (m_flagRunning) {
			L2PacketInfo info;
			info.type = L2PacketInfo::typeOutGoing;
			info.iface = m_ifaceIndex;
			if (m_deviceType == networkLinkDevice_Ethernet) {
				EthernetFrameFormat* frame = (EthernetFrameFormat*)buf;
				if (size < EthernetFrameFormat::getHeaderSize()) {
					return sl_false;
				}
				info.protocol = frame->getProtocol();
				info.setMacAddress(frame->getDestinationAddress());
			} else {
				info.protocol = networkLinkProtocol_IPv4;
				info.clearAddress();
			}
			Ref<Socket> socket = m_socket;
			if (socket.isNotNull()) {
				sl_int32 ret = socket->sendPacket(buf, size, info);
				if (ret == size) {
					return sl_true;
				}
			}
		}
		return sl_false;
	}
	
};

Ref<NetCapture> NetCapture::createRawPacket(const NetCaptureParam& param)
{
	Ref<_NetRawPacketCapture> ret = _NetRawPacketCapture::create(param);
	return Ref<NetCapture>::from(ret);
}

class _NetRawIPv4Capture : public NetCapture
{
public:
	Ref<Socket> m_socketTCP;
	Ref<Socket> m_socketUDP;
	Ref<Socket> m_socketICMP;
	
	Memory m_bufPacket;	
	Ref<Thread> m_thread;

	sl_bool m_flagRunning;
	
	_NetRawIPv4Capture()
	{	
		m_flagRunning = sl_true;
	}
	
	~_NetRawIPv4Capture()
	{
		release();
	}
	
	static Ref<_NetRawIPv4Capture> create(const NetCaptureParam& param)
	{
		Ref<_NetRawIPv4Capture> ret;
		Ref<Socket> socketTCP = Socket::openRaw_IPv4(networkInternetProtocol_TCP);
		Ref<Socket> socketUDP = Socket::openRaw_IPv4(networkInternetProtocol_UDP);
		Ref<Socket> socketICMP = Socket::openRaw_IPv4(networkInternetProtocol_ICMP);
		if (socketTCP.isNotNull() && socketUDP.isNotNull() && socketICMP.isNotNull()) {
			socketTCP->setOption_IncludeIpHeader(sl_true);
			socketUDP->setOption_IncludeIpHeader(sl_true);
			socketICMP->setOption_IncludeIpHeader(sl_true);
			Memory mem = Memory::create(MAX_PACKET_SIZE);
			if (mem.isNotEmpty()) {
				ret = new _NetRawIPv4Capture;
				if (ret.isNotNull()) {
					ret->m_bufPacket = mem;
					ret->m_socketTCP = socketTCP;
					ret->m_socketUDP = socketUDP;
					ret->m_socketICMP = socketICMP;
					ret->setListener(param.listener);
					ret->m_thread = Thread::start(SLIB_CALLBACK_CLASS(_NetRawIPv4Capture, _run, ret.get()));
					if (ret->m_thread.isNull()) {
						ret.setNull();
					}
				}
			}
		}
		return ret;
	}
	
	void release()
	{
		MutexLocker lock(getLocker());
		if (!m_flagRunning) {
			return;
		}
		m_flagRunning = sl_false;
		
		if (m_thread.isNotNull()) {
			m_thread->finishAndWait();
			m_thread.setNull();
		}
		m_socketTCP.setNull();
		m_socketUDP.setNull();
		m_socketICMP.setNull();
	}
	
	sl_bool isRunning()
	{
		return m_flagRunning;
	}
	
	void _run()
	{
		NetCapturePacket packet;

		Ref<Socket> socketTCP = m_socketTCP;
		if (socketTCP.isNull()) {
			return;
		}
		socketTCP->setNonBlockingMode(sl_true);
		Ref<SocketEvent> eventTCP = SocketEvent::createRead(socketTCP);
		if (eventTCP.isNull()) {
			return;
		}
		
		Ref<Socket> socketUDP = m_socketUDP;
		if (socketUDP.isNull()) {
			return;
		}
		socketUDP->setNonBlockingMode(sl_true);
		Ref<SocketEvent> eventUDP = SocketEvent::createRead(socketUDP);
		if (eventUDP.isNull()) {
			return;
		}
		
		Ref<Socket> socketICMP = m_socketICMP;
		if (socketICMP.isNull()) {
			return;
		}
		socketICMP->setNonBlockingMode(sl_true);
		Ref<SocketEvent> eventICMP = SocketEvent::createRead(socketICMP);
		if (eventICMP.isNull()) {
			return;
		}

		Ref<SocketEvent> events[3];
		events[0] = eventTCP;
		events[1] = eventUDP;
		events[2] = eventICMP;
		
		sl_uint8* buf = (sl_uint8*)(m_bufPacket.getBuf());
		sl_uint32 sizeBuf = (sl_uint32)(m_bufPacket.getSize());
		while (Thread::isNotStoppingCurrent()) {
			while (1) {
				SocketAddress address;
				sl_int32 n = socketTCP->receiveFrom(buf, sizeBuf, address);
				if (n > 0) {
					packet.data = buf;
					packet.length = n;
					packet.time = 0;
					_onCapturePacket(&packet);
				} else {
					break;
				}
			}
			while (1) {
				SocketAddress address;
				sl_int32 n = socketUDP->receiveFrom(buf, sizeBuf, address);
				if (n > 0) {
					packet.data = buf;
					packet.length = n;
					packet.time = 0;
					_onCapturePacket(&packet);
				} else {
					break;
				}
			}
			while (1) {
				SocketAddress address;
				sl_int32 n = socketICMP->receiveFrom(buf, sizeBuf, address);
				if (n > 0) {
					packet.data = buf;
					packet.length = n;
					packet.time = 0;
					_onCapturePacket(&packet);
				} else {
					break;
				}
			}
			SocketEvent::waitMultipleEvents(events, sl_null, 3);
		}
	}

	sl_uint32 getLinkType()
	{
		return networkLinkDevice_Raw;
	}

	sl_bool sendPacket(const void* buf, sl_uint32 size)
	{
		if (m_flagRunning) {
			SocketAddress address;
			if (IPv4HeaderFormat::check(buf, size)) {
				IPv4HeaderFormat* ip = (IPv4HeaderFormat*)buf;
				address.ip = ip->getDestinationAddress();
				address.port = 0;
				Ref<Socket> socket;
				if (ip->isTCP()) {
					socket = m_socketTCP;
				} else if (ip->isUDP()) {
					socket = m_socketUDP;
				} else if (ip->isICMP()) {
					socket = m_socketICMP;
				}
				if (socket.isNotNull()) {
					sl_int32 ret = socket->sendTo(buf, size, address);
					if (ret == size) {
						return sl_true;
					}
				}
			}
		}
		return sl_false;
	}
	
};

Ref<NetCapture> NetCapture::createRawIPv4(const NetCaptureParam& param)
{
	Ref<_NetRawIPv4Capture> ret = _NetRawIPv4Capture::create(param);
	return Ref<NetCapture>::from(ret);
}

SLIB_NETWORK_NAMESPACE_END
