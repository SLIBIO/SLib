#include "../../../inc/slib/network/capture.h"

#include "../../../inc/slib/core/thread.h"
#include "../../../inc/slib/core/mio.h"
#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/network/os.h"
#include "../../../inc/slib/network/socket.h"
#include "../../../inc/slib/network/event.h"
#include "../../../inc/slib/network/tcpip.h"
#include "../../../inc/slib/network/ethernet.h"

#define TAG "NetCapture"

#define MAX_PACKET_SIZE 65535

namespace slib
{
	NetCapturePacket::NetCapturePacket(): data(sl_null), length(0)
	{
	}
	
	NetCapturePacket::~NetCapturePacket()
	{
	}
	
	NetCaptureDeviceInfo::NetCaptureDeviceInfo(): flagLoopback(sl_false)
	{
	}
	
	NetCaptureDeviceInfo::~NetCaptureDeviceInfo()
	{
	}
	
	INetCaptureListener::INetCaptureListener()
	{
	}

	INetCaptureListener::~INetCaptureListener()
	{
	}

	NetCaptureParam::NetCaptureParam()
	{
		flagPromiscuous = sl_false;
		timeoutRead = 100;
		sizeBuffer = 0x200000; // 2MB (16Mb)
		
		preferedLinkDeviceType = NetworkLinkDeviceType::Ethernet;
		
		flagAutoStart = sl_true;
	}
	
	NetCaptureParam::~NetCaptureParam()
	{
	}
	
	
	SLIB_DEFINE_OBJECT(NetCapture, Object)
	
	NetCapture::NetCapture()
	{
	}
	
	NetCapture::~NetCapture()
	{
	}
	
	sl_bool NetCapture::setLinkType(sl_uint32 type)
	{
		return sl_false;
	}
	
	String NetCapture::getLastErrorMessage()
	{
		return sl_null;
	}
	
	void NetCapture::_initWithParam(const NetCaptureParam& param)
	{
		m_listener = param.listener;
		m_onCapturePacket = param.onCapturePacket;
	}
	
	void NetCapture::_onCapturePacket(NetCapturePacket* packet)
	{
		PtrLocker<INetCaptureListener> listener(m_listener);
		if (listener.isNotNull()) {
			listener->onCapturePacket(this, packet);
		}
		m_onCapturePacket(this, packet);
	}
	
	
	class _NetRawPacketCapture : public NetCapture
	{
	public:
		AtomicRef<Socket> m_socket;
		
		NetworkLinkDeviceType m_deviceType;
		sl_uint32 m_ifaceIndex;
		Memory m_bufPacket;
		Ref<Thread> m_thread;
		
		sl_bool m_flagInit;
		sl_bool m_flagRunning;
		
	public:
		_NetRawPacketCapture()
		{
			m_deviceType = NetworkLinkDeviceType::Ethernet;
			m_ifaceIndex = 0;

			m_flagInit = sl_false;
			m_flagRunning = sl_false;
		}
		
		~_NetRawPacketCapture()
		{
			release();
		}
		
	public:
		static Ref<_NetRawPacketCapture> create(const NetCaptureParam& param)
		{
			
			sl_uint32 iface = 0;
			String deviceName = param.deviceName;
			if (deviceName.isNotEmpty()) {
				iface = Network::getInterfaceIndexFromName(deviceName);
				if (iface == 0) {
					LogError(TAG, "Failed to find the interface index of device: %s", deviceName);
					return sl_null;
				}
			}
			Ref<Socket> socket;
			NetworkLinkDeviceType deviceType = param.preferedLinkDeviceType;
			if (deviceType == NetworkLinkDeviceType::Raw) {
				socket = Socket::openPacketDatagram(NetworkLinkProtocol::All);
			} else {
				deviceType = NetworkLinkDeviceType::Ethernet;
				socket = Socket::openPacketRaw(NetworkLinkProtocol::All);
			}
			if (socket.isNotNull()) {
				if (iface > 0) {
					if (param.flagPromiscuous) {
						if (!(socket->setPromiscuousMode(deviceName, sl_true))) {
							Log(TAG, "Failed to set promiscuous mode to the network device: %s", deviceName);
						}
					}
					if (!(socket->setOption_bindToDevice(deviceName))) {
						Log(TAG, "Failed to bind the network device: %s", deviceName);
					}
				}
				Memory mem = Memory::create(MAX_PACKET_SIZE);
				if (mem.isNotEmpty()) {
					Ref<_NetRawPacketCapture> ret = new _NetRawPacketCapture;
					if (ret.isNotNull()) {
						ret->_initWithParam(param);
						ret->m_bufPacket = mem;
						ret->m_socket = socket;
						ret->m_deviceType = deviceType;
						ret->m_ifaceIndex = iface;
						ret->m_thread = Thread::create(SLIB_FUNCTION_CLASS(_NetRawPacketCapture, _run, ret.get()));
						if (ret->m_thread.isNotNull()) {
							ret->m_flagInit = sl_true;
							if (param.flagAutoStart) {
								ret->start();
							}
							return ret;
						} else {
							LogError(TAG, "Failed to create thread");
						}
					}
				}
			} else {
				LogError(TAG, "Failed to create Packet socket");
			}
			return sl_null;
		}
		
		void release()
		{
			ObjectLocker lock(this);
			if (!m_flagInit) {
				return;
			}
			m_flagInit = sl_false;
			
			m_flagRunning = sl_false;
			if (m_thread.isNotNull()) {
				m_thread->finishAndWait();
				m_thread.setNull();
			}
			m_socket.setNull();
		}
		
		void start()
		{
			ObjectLocker lock(this);
			if (!m_flagInit) {
				return;
			}
			
			if (m_flagRunning) {
				return;
			}
			if (m_thread.isNotNull()) {
				if (m_thread->start()) {
					m_flagRunning = sl_true;
				}
			}
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
			
			sl_uint8* buf = (sl_uint8*)(m_bufPacket.getData());
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
		
		NetworkLinkDeviceType getLinkType()
		{
			return m_deviceType;
		}
		
		sl_bool sendPacket(const void* buf, sl_uint32 size)
		{
			if (m_ifaceIndex == 0) {
				return sl_false;
			}
			if (m_flagInit) {
				L2PacketInfo info;
				info.type = L2PacketType::OutGoing;
				info.iface = m_ifaceIndex;
				if (m_deviceType == NetworkLinkDeviceType::Ethernet) {
					EthernetFrame* frame = (EthernetFrame*)buf;
					if (size < EthernetFrame::HeaderSize) {
						return sl_false;
					}
					info.protocol = frame->getProtocol();
					info.setMacAddress(frame->getDestinationAddress());
				} else {
					info.protocol = NetworkLinkProtocol::IPv4;
					info.clearAddress();
				}
				Ref<Socket> socket = m_socket;
				if (socket.isNotNull()) {
					sl_uint32 ret = socket->sendPacket(buf, size, info);
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
		return _NetRawPacketCapture::create(param);
	}
	
	class _NetRawIPv4Capture : public NetCapture
	{
	public:
		AtomicRef<Socket> m_socketTCP;
		AtomicRef<Socket> m_socketUDP;
		AtomicRef<Socket> m_socketICMP;
		
		Memory m_bufPacket;
		Ref<Thread> m_thread;
		
		sl_bool m_flagInit;
		sl_bool m_flagRunning;
		
	public:
		_NetRawIPv4Capture()
		{
			m_flagInit = sl_false;
			m_flagRunning = sl_false;
		}
		
		~_NetRawIPv4Capture()
		{
			release();
		}
		
	public:
		static Ref<_NetRawIPv4Capture> create(const NetCaptureParam& param)
		{
			Ref<Socket> socketTCP = Socket::openRaw(NetworkInternetProtocol::TCP);
			Ref<Socket> socketUDP = Socket::openRaw(NetworkInternetProtocol::UDP);
			Ref<Socket> socketICMP = Socket::openRaw(NetworkInternetProtocol::ICMP);
			if (socketTCP.isNotNull() && socketUDP.isNotNull() && socketICMP.isNotNull()) {
				socketTCP->setOption_IncludeIpHeader(sl_true);
				socketUDP->setOption_IncludeIpHeader(sl_true);
				socketICMP->setOption_IncludeIpHeader(sl_true);
				Memory mem = Memory::create(MAX_PACKET_SIZE);
				if (mem.isNotEmpty()) {
					Ref<_NetRawIPv4Capture> ret = new _NetRawIPv4Capture;
					if (ret.isNotNull()) {
						ret->_initWithParam(param);
						ret->m_bufPacket = mem;
						ret->m_socketTCP = socketTCP;
						ret->m_socketUDP = socketUDP;
						ret->m_socketICMP = socketICMP;
						ret->m_thread = Thread::create(SLIB_FUNCTION_CLASS(_NetRawIPv4Capture, _run, ret.get()));
						if (ret->m_thread.isNotNull()) {
							ret->m_flagInit = sl_true;
							if (param.flagAutoStart) {
								ret->start();
							}
							return ret;
						} else {
							LogError(TAG, "Failed to create thread");
						}
					}
				}
			}
			return sl_null;
		}
		
		void release()
		{
			ObjectLocker lock(this);
			if (!m_flagInit) {
				return;
			}
			m_flagInit = sl_false;
			
			m_flagRunning = sl_false;
			if (m_thread.isNotNull()) {
				m_thread->finishAndWait();
				m_thread.setNull();
			}
			m_socketTCP.setNull();
			m_socketUDP.setNull();
			m_socketICMP.setNull();
		}
		
		void start()
		{
			ObjectLocker lock(this);
			if (!m_flagInit) {
				return;
			}
			
			if (m_flagRunning) {
				return;
			}
			
			if (m_thread.isNotNull()) {
				if (m_thread->start()) {
					m_flagRunning = sl_true;
				}
			}
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
			
			sl_uint8* buf = (sl_uint8*)(m_bufPacket.getData());
			sl_uint32 sizeBuf = (sl_uint32)(m_bufPacket.getSize());
			while (Thread::isNotStoppingCurrent()) {
				while (1) {
					SocketAddress address;
					sl_int32 n = socketTCP->receiveFrom(address, buf, sizeBuf);
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
					sl_int32 n = socketUDP->receiveFrom(address, buf, sizeBuf);
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
					sl_int32 n = socketICMP->receiveFrom(address, buf, sizeBuf);
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
		
		NetworkLinkDeviceType getLinkType()
		{
			return NetworkLinkDeviceType::Raw;
		}
		
		sl_bool sendPacket(const void* buf, sl_uint32 size)
		{
			if (m_flagInit) {
				SocketAddress address;
				if (IPv4Packet::check(buf, size)) {
					IPv4Packet* ip = (IPv4Packet*)buf;
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
						sl_uint32 ret = socket->sendTo(address, buf, size);
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
		return _NetRawIPv4Capture::create(param);
	}
	
	
	LinuxCookedPacketType LinuxCookedFrame::getPacketType() const
	{
		return (LinuxCookedPacketType)(MIO::readUint16BE(m_packetType));
	}
	
	void LinuxCookedFrame::setPacketType(LinuxCookedPacketType type)
	{
		MIO::writeUint16BE(m_packetType, (sl_uint32)type);
	}
	
	NetworkLinkDeviceType LinuxCookedFrame::getDeviceType() const
	{
		return (NetworkLinkDeviceType)(MIO::readUint16BE(m_deviceType));
	}
	
	void LinuxCookedFrame::setDeviceType(NetworkLinkDeviceType type)
	{
		MIO::writeUint16BE(m_deviceType, (sl_uint32)type);
	}
	
	sl_uint16 LinuxCookedFrame::getAddressLength() const
	{
		return MIO::readUint16BE(m_lenAddress);
	}
	
	void LinuxCookedFrame::setAddressLength(sl_uint16 len)
	{
		MIO::writeUint16BE(m_lenAddress, len);
	}
	
	const sl_uint8* LinuxCookedFrame::getAddress() const
	{
		return m_address;
	}
	
	sl_uint8* LinuxCookedFrame::getAddress()
	{
		return m_address;
	}
	
	NetworkLinkProtocol LinuxCookedFrame::getProtocolType() const
	{
		return (NetworkLinkProtocol)(MIO::readUint16BE(m_protocol));
	}
	
	void LinuxCookedFrame::setProtocolType(NetworkLinkProtocol type)
	{
		MIO::writeUint16BE(m_protocol, (sl_uint32)type);
	}
	
	const sl_uint8* LinuxCookedFrame::getContent() const
	{
		return ((const sl_uint8*)this) + HeaderSize;
	}
	
	sl_uint8* LinuxCookedFrame::getContent()
	{
		return ((sl_uint8*)this) + HeaderSize;
	}
	
}
