#include "../../../inc/slib/network/socket.h"
#include "../../../inc/slib/network/event.h"

#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/event.h"
#include "../../../inc/slib/core/file.h"

#if defined(SLIB_PLATFORM_IS_WINDOWS)
#	include <winsock2.h>
#	include <ws2tcpip.h>
#	pragma comment(lib, "ws2_32.lib")
#else
#	include <unistd.h>
#	include <sys/socket.h>
#	if defined(SLIB_PLATFORM_IS_LINUX)
#		include <linux/tcp.h>
#		include <linux/if.h>
#		include <linux/if_packet.h>
#		include <sys/ioctl.h>
#	else
#		include <netinet/tcp.h>
#	endif
#	include <netinet/in.h>
#	include <signal.h>
#	include <errno.h>
typedef int SOCKET;
#	define SOCKET_ERROR -1
#endif

SLIB_NETWORK_NAMESPACE_BEGIN

static sl_uint32 _Socket_apply_address(SocketType type, sockaddr_storage& addr, SocketAddress in)
{
	if (in.ip.isIPv4() && (type == socketType_Tcp || type == socketType_Udp || type == socketType_Raw)) {
		return in.getSystemSocketAddress(&addr);
	} else if ((in.ip.isIPv4() || in.ip.isIPv6()) && (type == socketType_TcpIPv6 || type == socketType_UdpIPv6 || type == socketType_RawIPv6)) {
		if (in.ip.isIPv4()) {
			in.ip = IPv6Address(in.ip.getIPv4());
		}
		return in.getSystemSocketAddress(&addr);
	}
	return 0;
}

static void _Socket_close(sl_socket socket)
{
#if defined(SLIB_PLATFORM_IS_WINDOWS)
	::closesocket((SOCKET)socket);
#else
	::close((SOCKET)socket);
#endif
}

Socket::Socket()
{
	m_socket = SLIB_SOCKET_INVALID_HANDLE;
	m_type = socketType_None;
	m_lastError = socketError_None;
}

Socket::~Socket()
{
	close();
}

void Socket::close()
{
	if (m_socket != SLIB_SOCKET_INVALID_HANDLE) {
		_Socket_close(m_socket);
		m_socket = SLIB_SOCKET_INVALID_HANDLE;
	}
	m_type = socketType_None;
	m_lastError = socketError_None;
}

void Socket::initializeSocket()
{
#if defined(SLIB_PLATFORM_IS_WINDOWS)
	static sl_bool flagInit = sl_false;
	if (!flagInit) {
		WSADATA wsaData;
		int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (err != 0) {
			SLIB_LOG_ERROR("SOCKET", "WSA Startup failed");
		}
		flagInit = sl_true;
	}
#else
	//signal(SIGPIPE, SIG_IGN);
#endif
}

Ref<Socket> Socket::open(SocketType type, sl_uint32 _protocol)
{
	initializeSocket();

	int af = 0;
	int st = 0;
	int protocol = 0;
	if (type == socketType_Tcp) {
		af = AF_INET;
		st = SOCK_STREAM;
	} else if (type == socketType_Udp) {
		af = AF_INET;
		st = SOCK_DGRAM;
	} else if (type == socketType_Raw) {
		af = AF_INET;
		st = SOCK_RAW;
		protocol = _protocol;
	} else if (type == socketType_TcpIPv6) {
		af = AF_INET6;
		st = SOCK_STREAM;
	} else if (type == socketType_UdpIPv6) {
		af = AF_INET6;
		st = SOCK_DGRAM;
	} else if (type == socketType_RawIPv6) {
		af = AF_INET6;
		st = SOCK_RAW;
		protocol = _protocol;
	}
#if defined(SLIB_PLATFORM_IS_LINUX)
	else if (type == socketType_PacketRaw) {
		af = AF_PACKET;
		st = SOCK_RAW;
		protocol = htons(_protocol);
	} else if (type == socketType_PacketDatagram) {
		af = AF_PACKET;
		st = SOCK_DGRAM;
		protocol = htons(_protocol);
	}
#endif
	else {
		return Ref<Socket>::null();
	}

#if defined(SLIB_PLATFORM_IS_WINDOWS)
	sl_socket handle = (sl_socket)(::WSASocketW(af, st, protocol, NULL, 0, WSA_FLAG_OVERLAPPED));
#else
	sl_socket handle = (sl_socket)(::socket(af, st, protocol));
#endif
	if (handle != SLIB_SOCKET_INVALID_HANDLE) {
		Ref<Socket> ret = new Socket();
		if (ret.isNotNull()) {
			ret->m_socket = handle;
			ret->m_type = type;
			if (type == socketType_TcpIPv6 || type == socketType_UdpIPv6 || type == socketType_RawIPv6) {
				ret->setOption_IPv6Only(sl_false);
			}
#if defined(SLIB_PLATFORM_IS_APPLE)
			ret->setOption(SOL_SOCKET, SO_NOSIGPIPE, 1);
#endif
			return ret;
		}
		_Socket_close(handle);
	}
	return Ref<Socket>::null();
}

String Socket::getTypeText() const
{
	switch (getType()) {
	case socketType_Tcp:
		{
			SLIB_STATIC_STRING(s, "TCP/IP");
			return s;
		}
	case socketType_Udp:
		{
			SLIB_STATIC_STRING(s, "UDP/IP");
			return s;
		}
	case socketType_Raw:
		{
			SLIB_STATIC_STRING(s, "RAW/IP");
			return s;
		}
	case socketType_TcpIPv6:
		{
			SLIB_STATIC_STRING(s, "TCP/IPv6");
			return s;
		}
	case socketType_UdpIPv6:
		{
			SLIB_STATIC_STRING(s, "UDP/IPv6");
			return s;
		}
	case socketType_RawIPv6:
		{
			SLIB_STATIC_STRING(s, "RAW/IPv6");
			return s;
		}
	case socketType_PacketRaw:
		{
			SLIB_STATIC_STRING(s, "PACKET/RAW");
			return s;
		}
	case socketType_PacketDatagram:
		{
			SLIB_STATIC_STRING(s, "PACKET/DATAGRAM");
			return s;
		}
	default:
		{
			SLIB_STATIC_STRING(s, "None");
			return s;
		}
	}
}

sl_bool Socket::shutdown(SocketShutdownMode mode)
{
	if (isOpened()) {
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		int ret = ::shutdown((SOCKET)(m_socket), mode == socketShutdownMode_Receive ? SD_RECEIVE : mode == socketShutdownMode_Send ? SD_SEND : SD_BOTH);
#else
		int ret = ::shutdown((SOCKET)(m_socket), mode == socketShutdownMode_Receive ? SHUT_RD : mode == socketShutdownMode_Send ? SHUT_WR : SHUT_RDWR);
#endif
		if (ret == 0) {
			return sl_true;
		} else {
			_checkError();
			return sl_false;
		}
	} else {
		_setClosedError();
		return sl_false;
	}
}

sl_bool Socket::bind(const SocketAddress& address)
{
	if (isOpened()) {
		sockaddr_storage addr;
        sl_uint32 size_addr = 0;
		if (m_type == socketType_Tcp || m_type == socketType_Udp || m_type == socketType_Raw) {
			if (address.ip.isIPv4()) {
				size_addr = _Socket_apply_address(m_type, addr, address);
			} else if (address.ip.isNone()) {
				SocketAddress addrAny;
				addrAny.ip = IPv4Address::any();
				addrAny.port = address.port;
				size_addr = _Socket_apply_address(m_type, addr, addrAny);
			} else {
				_setError(socketError_BindInvalidAddress);
				return sl_false;
			}
		} else if (m_type == socketType_TcpIPv6 || m_type == socketType_UdpIPv6 || m_type == socketType_RawIPv6) {
			if (address.ip.isIPv4()) {
				size_addr = _Socket_apply_address(m_type, addr, address);
			} else if (address.ip.isIPv6()) {
				size_addr = _Socket_apply_address(m_type, addr, address);
			} else if (address.ip.isNone()) {
				SocketAddress addrAny;
				addrAny.ip = IPv6Address::any();
				addrAny.port = address.port;
				size_addr = _Socket_apply_address(m_type, addr, addrAny);
			} else {
				_setError(socketError_BindInvalidAddress);
				return sl_false;
			}
		} else {
			_setError(socketError_BindInvalidType);
			return sl_false;
		}
		int ret = ::bind((SOCKET)(m_socket), (sockaddr*)&addr, (int)size_addr);
		if (ret != SOCKET_ERROR) {
			return sl_true;
		} else {
			_checkError();
			return sl_false;
		}
	} else {
		_setClosedError();
		return sl_false;
	}
}

sl_bool Socket::listen()
{
	if (isOpened()) {
		if (m_type != socketType_Tcp && m_type != socketType_TcpIPv6) {
			_setError(socketError_ListenIsNotSupported);
			return sl_false;
		}
		int ret = ::listen((SOCKET)(m_socket), SOMAXCONN);
		if (ret != SOCKET_ERROR) {
			return sl_true;
		} else {
			_checkError();
			return sl_false;
		}
	} else {
		_setClosedError();
		return sl_false;
	}
}

sl_bool Socket::accept(Ref<Socket>& socketClient, SocketAddress& address)
{
	if (isOpened()) {
		if (m_type != socketType_Tcp && m_type != socketType_TcpIPv6) {
			_setError(socketError_AcceptIsNotSupported);
			return sl_false;
		}
		sockaddr_storage addr;
		Base::resetMemory(&addr, 0, sizeof(addr));
		int len = sizeof(addr);
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		sl_socket client = (sl_socket)(::accept((SOCKET)(m_socket), (sockaddr*)&addr, &len));
#else
		sl_socket client = (sl_socket)(::accept((SOCKET)(m_socket), (sockaddr*)&addr, (socklen_t*)&len));
#endif
		if (client != SLIB_SOCKET_INVALID_HANDLE) {
			address.setSystemSocketAddress(&addr);
			Ref<Socket> socket = new Socket();
			if (socket.isNotNull()) {
				socket->m_type = m_type;
				socket->m_socket = client;
				socketClient = socket;
			} else {
				_Socket_close(client);
			}
			return sl_true;
		} else {
			_checkError();
			return sl_false;
		}
	} else {
		_setClosedError();
		return sl_false;
	}
}

sl_bool Socket::connect(const SocketAddress& address)
{
	if (isOpened()) {
		if (m_type != socketType_Tcp && m_type != socketType_TcpIPv6) {
			_setError(socketError_ConnectIsNotSupported);
			return sl_false;
		}
		sockaddr_storage addr;
		if (sl_uint32 addr_size = _Socket_apply_address(m_type, addr, address)) {
			int ret = ::connect((SOCKET)(m_socket), (sockaddr*)&addr, (int)addr_size);
			if (ret != SOCKET_ERROR) {
				return sl_true;
			} else {
				sl_uint32 e = _checkError();
#if defined(SLIB_PLATFORM_IS_WINDOWS)
				if (e == socketError_WouldBlock)
#else
				if (e == socketError_InProgress)
#endif
				{
					return sl_true;
				} else {
					return sl_false;
				}
			}
		} else {
			_setError(socketError_ConnectToInvalidAddress);
			return sl_false;
		}
	} else {
		_setClosedError();
		return sl_false;
	}
}

sl_bool Socket::connectAndWait(const SocketAddress& address, sl_int32 timeout)
{
	setNonBlockingMode(sl_true);
	if (connect(address)) {
		Ref<SocketEvent> ev = SocketEvent::createWrite(this);
		if (ev.isNotNull()) {
			if (ev->waitEvents(timeout) & socketEventType_Write) {
				if (getOption_Error() == 0) {
					return sl_true;
				}
			}
		}
	}
	return sl_false;
}

sl_int32 Socket::send(const void* buf, sl_uint32 size)
{
	if (isOpened()) {
		if (size == 0) {
			return 0;
		}
		if (m_type != socketType_Tcp && m_type != socketType_TcpIPv6) {
			_setError(socketError_SendIsNotSupported);
			return -1;
		}
#if	defined(SLIB_PLATFORM_IS_LINUX)
		sl_int32 ret = (sl_int32)(::send((SOCKET)(m_socket), (const char*)buf, size, MSG_NOSIGNAL));
#else
		sl_int32 ret = (sl_int32)(::send((SOCKET)(m_socket), (const char*)buf, size, 0));
#endif
		if (ret >= 0) {
			if (ret == 0) {
				ret = -1;
			}
			return ret;
		} else {
			if (_checkError() == socketError_WouldBlock) {
				return 0;
			} else {
				return -1;
			}
		}
	} else {
		_setClosedError();
		return -1;
	}
}

sl_int32 Socket::receive(void* buf, sl_uint32 size)
{
	if (isOpened()) {
		if (size == 0) {
			return 0;
		}
		if (m_type != socketType_Tcp && m_type != socketType_TcpIPv6) {
			_setError(socketError_ReceiveIsNotSupported);
			return -1;
		}
		sl_int32 ret = (sl_int32)(::recv((SOCKET)(m_socket), (char*)buf, size, 0));
		if (ret >= 0) {
			if (ret == 0) {
				return -1;
			}
			return ret;
		} else {
			if (_checkError() == socketError_WouldBlock) {
				return 0;
			} else {
				return -1;
			}
		}
	} else {
		_setClosedError();
		return -1;
	}
}

sl_int32 Socket::sendTo(const SocketAddress& address, const void* buf, sl_uint32 size)
{
	if (isOpened()) {
		if (size == 0) {
			return 0;
		}
		if (m_type != socketType_Udp && m_type != socketType_UdpIPv6 && m_type != socketType_Raw && m_type != socketType_RawIPv6) {
			_setError(socketError_SendToIsNotSupported);
			return -1;
		}
		sockaddr_storage addr;
		if (sl_uint32 addr_size = _Socket_apply_address(m_type, addr, address)) {
			sl_int32 ret = (sl_int32)(::sendto((SOCKET)(m_socket), (char*)buf, size, 0, (sockaddr*)&addr, (int)addr_size));
			if (ret >= 0) {
				if (ret == 0) {
					ret = -1;
				}
				return ret;
			} else {
				if (_checkError() == socketError_WouldBlock) {
					return 0;
				} else {
					return -1;
				}
			}
		} else {
			_setError(socketError_SendToInvalidAddress);
			return -1;
		}
	} else {
		_setClosedError();
		return -1;
	}
}

sl_int32 Socket::receiveFrom(SocketAddress& address, void* buf, sl_uint32 size)
{
	if (isOpened()) {
		if (size == 0) {
			return 0;
		}
		if (m_type != socketType_Udp && m_type != socketType_UdpIPv6 && m_type != socketType_Raw && m_type != socketType_RawIPv6) {
			_setError(socketError_ReceiveFromIsNotSupported);
			return -1;
		}
		sockaddr_storage addr;
		Base::resetMemory(&addr, 0, sizeof(addr));
		int lenAddr = sizeof(addr);
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		sl_int32 ret = ::recvfrom((SOCKET)(m_socket), (char*)buf, size, 0, (sockaddr*)&addr, &lenAddr);
#else
		sl_int32 ret = (sl_int32)(::recvfrom((SOCKET)(m_socket), (char*)buf, size, 0, (sockaddr*)&addr, (socklen_t*)&lenAddr));
#endif
		if (ret >= 0) {
			address.setSystemSocketAddress(&addr);
			if (ret == 0) {
				ret = -1;
			}
			return ret;
		} else {
			if (_checkError() == socketError_WouldBlock) {
				return 0;
			} else {
				return -1;
			}
		}
	} else {
		_setClosedError();
		return -1;
	}
}

sl_int32 Socket::sendPacket(const void* buf, sl_uint32 size, const L2PacketInfo& info)
{
#if defined(SLIB_PLATFORM_IS_LINUX)
	if (isOpened()) {
		if (size == 0) {
			return 0;
		}
		if (m_type != socketType_PacketRaw && m_type != socketType_PacketDatagram) {
			_setError(socketError_SendPacketIsNotSupported);
			return -1;
		}
		
		sockaddr_ll addr;
		addr.sll_family = AF_PACKET;
		addr.sll_protocol = htons(info.protocol);
		addr.sll_ifindex = info.iface;
		addr.sll_hatype = 0;
		addr.sll_pkttype = (unsigned char)(info.type);
		sl_uint32 na = info.lenHardwareAddress;
		if (na > 8) {
			na = 8;
		}
		addr.sll_halen = na;
		Base::zeroMemory(addr.sll_addr, 8);
		Base::copyMemory(addr.sll_addr, info.hardwareAddress, na);
		sl_int32 ret = (sl_int32)(::sendto((SOCKET)(m_socket), (char*)buf, size, 0, (sockaddr*)&addr, sizeof(addr)));
		if (ret >= 0) {
			if (ret == 0) {
				ret = -1;
			}
			return ret;
		} else {
			if (_checkError() == socketError_WouldBlock) {
				return 0;
			} else {
				return -1;
			}
		}
	} else {
		_setClosedError();
	}
#endif
	return -1;
}

sl_int32 Socket::receivePacket(const void* buf, sl_uint32 size, L2PacketInfo& info)
{
#if defined(SLIB_PLATFORM_IS_LINUX)
	if (isOpened()) {
		if (size == 0) {
			return 0;
		}
		if (m_type != socketType_PacketRaw && m_type != socketType_PacketDatagram) {
			_setError(socketError_ReceivePacketIsNotSupported);
			return -1;
		}
		sockaddr_ll addr;
		Base::resetMemory(&addr, 0, sizeof(addr));
		int lenAddr = sizeof(addr);
		sl_int32 ret = (sl_int32)(::recvfrom((SOCKET)(m_socket), (char*)buf, size, 0, (sockaddr*)&addr, (socklen_t*)&lenAddr));
		if (ret >= 0) {
			info.iface = addr.sll_ifindex;
			info.protocol = ntohs(addr.sll_protocol);
			info.type = (L2PacketType)(addr.sll_pkttype);
			sl_uint32 na = addr.sll_halen;
			if (na > 8) {
				na = 8;
			}
			Base::copyMemory(info.hardwareAddress, addr.sll_addr, na);
			info.lenHardwareAddress = na;
			if (ret == 0) {
				ret = -1;
			}
			return ret;
		} else {
			if (_checkError() == socketError_WouldBlock) {
				return 0;
			} else {
				return -1;
			}
		}
	} else {
		_setClosedError();
	}
#endif
	return -1;
}

sl_bool _Socket_setNonBlocking(SOCKET fd, sl_bool flagEnable)
{
#if defined(SLIB_PLATFORM_IS_WINDOWS)
	u_long flag = flagEnable ? 1 : 0;
	return ioctlsocket(fd, FIONBIO, &flag) == 0;
#else
#	if defined(FIONBIO)
	sl_int32 flag = flagEnable ? 1 : 0;
	return ioctl(fd, FIONBIO, &flag) == 0;
#	else
	return File::setNonBlocking(fd, flagEnable);
#	endif
#endif
}

sl_bool Socket::setNonBlockingMode(sl_bool flagEnable)
{
	if (isOpened()) {
		if (_Socket_setNonBlocking((SOCKET)(m_socket), flagEnable)) {
			return sl_true;
		}
	}
	return sl_false;
}

sl_bool _Socket_setPromiscuousMode(SOCKET fd, const char* deviceName, sl_bool flagEnable)
{
#if defined(SLIB_PLATFORM_IS_LINUX)
	ifreq ifopts;
	Base::copyString(ifopts.ifr_name, deviceName, IFNAMSIZ-1);
	int ret;
	ret = ioctl(fd, SIOCGIFFLAGS, &ifopts);
	if (ret == 0) {
		if (flagEnable) {
			ifopts.ifr_flags |= IFF_PROMISC;
		} else {
			ifopts.ifr_flags &= (~IFF_PROMISC);
		}
		ret = ioctl(fd, SIOCSIFFLAGS, &ifopts);
		if (ret == 0) {
			return sl_true;
		}
	}
	return sl_false;
#else
	return sl_false;
#endif
}

sl_bool Socket::setPromiscuousMode(const String& _deviceName, sl_bool flagEnable)
{
	if (isOpened()) {
		String8 deviceName = _deviceName;
		if (deviceName.isNotEmpty()) {
			if (_Socket_setPromiscuousMode((SOCKET)(m_socket), deviceName.getBuf(), flagEnable)) {
				return sl_true;
			}
		}
	}
	return sl_false;
}

sl_bool Socket::getLocalAddress(SocketAddress& out)
{
	if (isOpened()) {
		sockaddr_storage addr;
		int size = sizeof(addr);
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		if (0 == getsockname((SOCKET)(m_socket), (sockaddr*)(&addr), &size)) {
#else
		if (0 == getsockname((SOCKET)(m_socket), (sockaddr*)(&addr), (socklen_t*)&size)) {
#endif
			return out.setSystemSocketAddress(&addr);
		} else {
			_checkError();
			return sl_false;
		}
	} else {
		_setClosedError();
		return sl_false;
	}
}

sl_bool Socket::getRemoteAddress(SocketAddress& out)
{
	if (isOpened()) {
		sockaddr_storage addr;
		int size = sizeof(addr);
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		if (0 == getpeername((SOCKET)(m_socket), (sockaddr*)(&addr), &size)) {
#else
		if (0 == getpeername((SOCKET)(m_socket), (sockaddr*)(&addr), (socklen_t*)&size)) {
#endif
			return out.setSystemSocketAddress(&addr);
		} else {
			_checkError();
			return sl_false;
		}
	} else {
		_setClosedError();
		return sl_false;
	}
}

sl_bool Socket::setOption(int level, int option, const void* buf, sl_uint32 bufSize)
{
	if (isOpened()) {
		int ret = ::setsockopt((SOCKET)(m_socket), level, option, (char*)buf, (int)bufSize);
		if (ret != SOCKET_ERROR) {
			return sl_true;
		} else {
			return sl_false;
		}
	} else {
		return sl_false;
	}
}
sl_bool Socket::getOption(int level, int option, void* buf, sl_uint32 bufSize) const
{
	if (isOpened()) {
		int len = (int)bufSize;
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		int ret = ::getsockopt((SOCKET)(m_socket), level, option, (char*)buf, &len);
#else
		int ret = ::getsockopt((SOCKET)(m_socket), level, option, (char*)buf, (socklen_t*)&len);
#endif
		if (ret != SOCKET_ERROR) {
			return sl_true;
		} else {
			return sl_false;
		}
	} else {
		return sl_false;
	}
}

sl_bool Socket::setOption(int level, int option, sl_uint32 value)
{
	return setOption(level, option, &value, 4);
}

sl_uint32 Socket::getOption(int level, int option) const
{
	sl_uint32 v;
	if (getOption(level, option, &v, 4)) {
		return v != 0;
	} else {
		return 0;
	}
}


sl_uint32 Socket::getOption_Error() const
{
	return getOption(SOL_SOCKET, SO_ERROR);
}


sl_bool Socket::setOption_Broadcast(sl_bool flagEnable)
{
	return setOption(SOL_SOCKET, SO_BROADCAST, flagEnable ? 1 : 0);
}

sl_bool Socket::getOption_Broadcast() const
{
	return getOption(SOL_SOCKET, SO_BROADCAST) != 0;
}


sl_bool Socket::setOption_ReuseAddress(sl_bool flagEnable)
{
	return setOption(SOL_SOCKET, SO_REUSEADDR, flagEnable ? 1 : 0);
}

sl_bool Socket::getOption_ReuseAddress() const
{
	return getOption(SOL_SOCKET, SO_REUSEADDR) != 0;
}


sl_bool Socket::setOption_SendBufferSize(sl_uint32 size)
{
	return setOption(SOL_SOCKET, SO_SNDBUF, size);
}

sl_uint32 Socket::getOption_SendBufferSize() const
{
	return getOption(SOL_SOCKET, SO_SNDBUF);
}


sl_bool Socket::setOption_ReceiveBufferSize(sl_uint32 size)
{
	return setOption(SOL_SOCKET, SO_RCVBUF, size);
}

sl_uint32 Socket::getOption_ReceiveBufferSize() const
{
	return getOption(SOL_SOCKET, SO_RCVBUF);
}


sl_bool Socket::setOption_SendTimeout(sl_uint32 size)
{
	// get is not supported
	return setOption(SOL_SOCKET, SO_SNDTIMEO, size);
}

sl_bool Socket::setOption_ReceiveTimeout(sl_uint32 size)
{
	// get is not supported
	return setOption(SOL_SOCKET, SO_RCVTIMEO, size);
}


sl_bool Socket::setOption_IPv6Only(sl_bool flagEnable)
{
	return setOption(IPPROTO_IPV6, IPV6_V6ONLY, flagEnable ? 1 : 0);
}

sl_bool Socket::getOption_IPv6Only() const
{
	return getOption(IPPROTO_IPV6, IPV6_V6ONLY) != 0;
}


sl_bool Socket::setOption_TcpNoDelay(sl_bool flagEnable)
{
	return setOption(IPPROTO_TCP, TCP_NODELAY, flagEnable ? 1 : 0);
}

sl_bool Socket::getOption_TcpNoDelay() const
{
	return getOption(IPPROTO_TCP, TCP_NODELAY) != 0;
}


sl_bool Socket::setOption_IpTTL(sl_uint32 ttl)
{
	if (ttl > 255) {
		return sl_false;
	}
	return setOption(IPPROTO_IP, IP_TTL, ttl);
}

sl_uint32 Socket::getOption_IpTTL() const
{
	return getOption(IPPROTO_IP, IP_TTL);
}


sl_bool Socket::getOption_IsListening() const
{
	return getOption(SOL_SOCKET, SO_ACCEPTCONN) != 0;
}


sl_bool Socket::setOption_IncludeIpHeader(sl_bool flagEnable)
{
	return setOption(IPPROTO_IP, IP_HDRINCL, flagEnable ? 1 : 0);
}

sl_bool Socket::getOption_IncludeIpHeader() const
{
	return getOption(IPPROTO_IP, IP_HDRINCL) != 0;
}


sl_bool Socket::setOption_bindToDevice(const String& ifname)
{
#if defined(SLIB_PLATFORM_IS_LINUX)
	return setOption(SOL_SOCKET, SO_BINDTODEVICE, ifname.getBuf(), ifname.getLength());
#else
	return sl_false;
#endif
}


sl_bool Socket::setOption_IpAddMembership(const IPv4Address& ipMulticast, const IPv4Address& ipInterface)
{
	ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = htonl(ipMulticast.toInt());
	mreq.imr_interface.s_addr = htonl(ipInterface.toInt());
	return setOption(IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
}

sl_bool Socket::setOption_IpDropMembership(const IPv4Address& ipMulticast, const IPv4Address& ipInterface)
{
	ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = htonl(ipMulticast.toInt());
	mreq.imr_interface.s_addr = htonl(ipInterface.toInt());
	return setOption(IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));
}

sl_bool Socket::setOption_IpMulticastLoop(sl_bool flag)
{
	return setOption(IPPROTO_IP, IP_MULTICAST_LOOP, flag ? 1 : 0);
}

sl_bool Socket::getOption_IpMulticastLoop() const
{
	return getOption(IPPROTO_IP, IP_MULTICAST_LOOP) != 0;
}

sl_bool Socket::setOption_IpMulticastTTL(sl_uint32 ttl)
{
	if (ttl > 255) {
		return sl_false;
	}
	return setOption(IPPROTO_IP, IP_MULTICAST_TTL, ttl);
}

sl_uint32 Socket::getOption_IpMulticastTTL() const
{
	return getOption(IPPROTO_IP, IP_MULTICAST_TTL);
}


sl_uint32 Socket::_setError(sl_uint32 code)
{
	m_lastError = code;
	return code;
}

sl_uint32 Socket::_checkError()
{
#if defined(SLIB_PLATFORM_IS_WINDOWS)
	int err = WSAGetLastError();
#else
	int err = errno;
#endif
	sl_uint32 ret = 0;

	switch (err) {
#if defined(SLIB_PLATFORM_IS_WINDOWS)
	case WSAEWOULDBLOCK:
#else
	case EWOULDBLOCK:
#	if (EWOULDBLOCK != EAGAIN)
	case EAGAIN:
#	endif
#endif
		ret = _setError(socketError_WouldBlock);
		break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
	case WSAENETDOWN:
		ret = _setError(socketError_NetworkDown);
		break;
#endif

#if defined(SLIB_PLATFORM_IS_WINDOWS)
	case WSAENETRESET:
		ret = _setError(socketError_NetworkReset);
		break;
#endif

#if defined(SLIB_PLATFORM_IS_WINDOWS)
	case WSAECONNRESET:
#else
	case ECONNRESET:
#endif
		ret = _setError(socketError_ConnectionReset);
		break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
	case WSAECONNABORTED:
#else
	case ECONNABORTED:
#endif
		ret = _setError(socketError_ConnectionAbort);
		break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
	case WSAECONNREFUSED:
#else
	case ECONNREFUSED:
#endif
		ret = _setError(socketError_ConnectionRefused);
		break;


#if defined(SLIB_PLATFORM_IS_WINDOWS)
	case WSAETIMEDOUT:
#else
	case ETIMEDOUT:
#endif
		ret = _setError(socketError_Timeout);
		break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
	case WSAENOTSOCK:
#else
	case ENOTSOCK:
	case EBADF:
#endif
		ret = _setError(socketError_NotSocket);
		break;


#if defined(SLIB_PLATFORM_IS_WINDOWS)
	case WSAEADDRINUSE:
#else
	case EADDRINUSE:
#endif
		ret = _setError(socketError_AddressAlreadyInUse);
		break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
	case WSAENOBUFS:
#else
	case ENOBUFS:
#endif
		ret = _setError(socketError_NoBufs);
		break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
	case WSA_NOT_ENOUGH_MEMORY:
#else
	case ENOMEM:
#endif
		ret = _setError(socketError_NoMem);
		break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
	case WSAEINPROGRESS:
#else
	case EINPROGRESS:
#endif
		ret = _setError(socketError_InProgress);
		break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
	case WSAEDESTADDRREQ:
#else
	case EDESTADDRREQ:
#endif
		ret = _setError(socketError_DestinationAddressRequired);
		break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
	case WSAEPFNOSUPPORT:
#else
	case EPFNOSUPPORT:
#endif
		ret = _setError(socketError_ProtocolFamilyNotSupported);
		break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
	case WSAEAFNOSUPPORT:
#else
	case EAFNOSUPPORT:
#endif
		ret = _setError(socketError_AddressFamilyNotSupported);
		break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
	case WSAEADDRNOTAVAIL:
#else
	case EADDRNOTAVAIL:
#endif
		ret = _setError(socketError_AddressNotAvailable);
		break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
	case WSAENOTCONN:
#else
	case ENOTCONN:
#endif
		ret = _setError(socketError_NotConnected);
		break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
	case WSAESHUTDOWN:
#else
	case ESHUTDOWN:
#endif
		ret = _setError(socketError_Shutdown);
		break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
	case WSAEACCES:
#else
	case EACCES:
#endif
		ret = _setError(socketError_Access);
		break;

	default:
		ret = _setError(socketError_Unknown + err);
		break;
	}
	return ret;
}

void Socket::_setClosedError()
{
	_setError(socketError_Closed);
}

String Socket::getErrorMessage(sl_uint32 error)
{
	switch (error) {
	case socketError_None:
		return String::null();
	case socketError_NetworkDown:
		return "NETDOWN - Network is down";
	case socketError_NetworkReset:
		return "NETRESET - Network dropped connection on reset";
	case socketError_ConnectionReset:
		return "CONNRESET - Connection reset by peer";
	case socketError_ConnectionAbort:
		return "CONNABORTED - Software caused connection abort";
	case socketError_ConnectionRefused:
		return "CONNREFUSED - Connection refused";
	case socketError_Timeout:
		return "TIMEOUT - Connection timed out";
	case socketError_NotSocket:
		return "NOTSOCK - Socket operation on nonsocket";
	case socketError_AddressAlreadyInUse:
		return "ADDRINUSE - Address already in use";
	case socketError_NoBufs:
		return "NOBUFS - No buffer space available";
	case socketError_NoMem:
		return "NOMEM - Insufficient memory available";
	case socketError_InProgress:
		return "INPROGRESS - Operation now in progress";
	case socketError_DestinationAddressRequired:
		return "DESTADDRREQ - Destination address required";
	case socketError_ProtocolFamilyNotSupported:
		return "PFNOSUPPORT - Protocol family not supported";
	case socketError_AddressNotAvailable:
		return "AFNOSUPPORT - Address family not supported by protocol family";
	case socketError_NotConnected:
		return "NOTCONN - Socket is not connected";
	case socketError_Shutdown:
		return "SHUTDOWN - Cannot send after socket shutdown";
	case socketError_Access:
		return "ACCESS - Permission denied";

	case socketError_Closed:
		return "Socket is closed";
	case socketError_BindInvalidAddress:
		return "Bind to invalid address";
	case socketError_BindInvalidType:
		return "Bind invalid socket type";
	case socketError_ListenIsNotSupported:
		return "Listen is not supported";
	case socketError_AcceptIsNotSupported:
		return "Accept is not supported";
	case socketError_ConnectIsNotSupported:
		return "Connect is not supported";
	case socketError_ConnectToInvalidAddress:
		return "Connect to invalid address";
	case socketError_SendIsNotSupported:
		return "Send is not supported";
	case socketError_ReceiveIsNotSupported:
		return "Receive is not supported";
	case socketError_SendToIsNotSupported:
		return "SendTo is not supported";
	case socketError_SendToInvalidAddress:
		return "SendTo to invalid address";
	case socketError_ReceiveFromIsNotSupported:
		return "ReceiveFrom is not supported";
	case socketError_SendPacketIsNotSupported:
		return "SendPacket is not supported";
	case socketError_SendPacketInvalidAddress:
		return "SendPacket to invalid address";
	case socketError_ReceivePacketIsNotSupported:
		return "ReceivePacket is not supported";
	}
	if (error > socketError_Unknown) {
		return String("Unknown System Error: ") + (error - socketError_Unknown);
	}
	return String("Not Defined Error: ") + error;
}
SLIB_NETWORK_NAMESPACE_END

