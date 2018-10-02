/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/network/socket.h"

#include "slib/network/event.h"
#include "slib/core/log.h"
#include "slib/core/event.h"
#include "slib/core/file.h"

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

namespace slib
{

	void L2PacketInfo::setMacAddress(const MacAddress& address)
	{
		lenHardwareAddress = 6;
		Base::copyMemory(hardwareAddress, address.m, 6);
		hardwareAddress[6] = 0;
		hardwareAddress[7] = 0;
	}

	sl_bool L2PacketInfo::getMacAddress(MacAddress* address)
	{
		if (lenHardwareAddress == 6) {
			if (address) {
				address->setBytes(hardwareAddress);
			}
			return sl_true;
		} else {
			return sl_false;
		}
	}

	void L2PacketInfo::clearAddress()
	{
		lenHardwareAddress = 0;
		Base::zeroMemory(hardwareAddress, 8);
	}


	SLIB_INLINE static sl_uint32 _priv_Socket_apply_address(SocketType type, sockaddr_storage& addr, SocketAddress in)
	{
		if (in.ip.isIPv4() && Socket::isIPv4(type)) {
			return in.getSystemSocketAddress(&addr);
		} else if ((in.ip.isIPv4() || in.ip.isIPv6()) && Socket::isIPv6(type)) {
			if (in.ip.isIPv4()) {
				in.ip = IPv6Address(in.ip.getIPv4());
			}
			return in.getSystemSocketAddress(&addr);
		}
		return 0;
	}

	void Socket::initializeSocket()
	{
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		static sl_bool flagInit = sl_false;
		if (!flagInit) {
			WSADATA wsaData;
			int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
			if (err != 0) {
				LogError("SOCKET", "WSA Startup failed");
			}
			flagInit = sl_true;
		}
#else
		//signal(SIGPIPE, SIG_IGN);
#endif
	}

	SLIB_INLINE static void _priv_Socket_close(sl_socket socket)
	{
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		::closesocket((SOCKET)socket);
#else
		::close((SOCKET)socket);
#endif
	}

	SLIB_DEFINE_ROOT_OBJECT(Socket)

	Socket::Socket()
	{
		m_socket = SLIB_SOCKET_INVALID_HANDLE;
		m_type = SocketType::None;
		m_lastError = SocketError::None;
	}

	Socket::~Socket()
	{
		close();
	}

	Ref<Socket> Socket::open(SocketType type, sl_uint32 _protocol)
	{
		initializeSocket();

		int af = 0;
		int st = 0;
		int protocol = _protocol;
		switch (type) {
			case SocketType::Stream:
				af = AF_INET;
				st = SOCK_STREAM;
				break;
			case SocketType::Datagram:
				af = AF_INET;
				st = SOCK_DGRAM;
				break;
			case SocketType::Raw:
				af = AF_INET;
				st = SOCK_RAW;
				break;
			case SocketType::StreamIPv6:
				af = AF_INET6;
				st = SOCK_STREAM;
				break;
			case SocketType::DatagramIPv6:
				af = AF_INET6;
				st = SOCK_DGRAM;
				break;
			case SocketType::RawIPv6:
				af = AF_INET6;
				st = SOCK_RAW;
				break;
#if defined(SLIB_PLATFORM_IS_LINUX)
			case SocketType::PacketRaw:
				af = AF_PACKET;
				st = SOCK_RAW;
				protocol = htons(_protocol);
				break;
			case SocketType::PacketDatagram:
				af = AF_PACKET;
				st = SOCK_DGRAM;
				protocol = htons(_protocol);
				break;
#endif
			default:
				return sl_null;
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
				if (isIPv6(type)) {
					ret->setOption_IPv6Only(sl_false);
				}
#if defined(SLIB_PLATFORM_IS_APPLE)
				ret->setOption(SOL_SOCKET, SO_NOSIGPIPE, 1);
#endif
				return ret;
			}
			_priv_Socket_close(handle);
		}
		return sl_null;
	}

	Ref<Socket> Socket::openStream(NetworkInternetProtocol internetProtocol)
	{
		return open(SocketType::Stream, (sl_uint32)internetProtocol);
	}

	Ref<Socket> Socket::openTcp()
	{
		return open(SocketType::Stream);
	}

	Ref<Socket> Socket::openDatagram(NetworkInternetProtocol internetProtocol)
	{
		return open(SocketType::Datagram, (sl_uint32)internetProtocol);
	}
	
	Ref<Socket> Socket::openUdp()
	{
		return open(SocketType::Datagram);
	}

	Ref<Socket> Socket::openRaw(NetworkInternetProtocol internetProtocol)
	{
		return open(SocketType::Raw, (sl_uint32)internetProtocol);
	}

	Ref<Socket> Socket::openStream_IPv6(NetworkInternetProtocol internetProtocol)
	{
		return open(SocketType::StreamIPv6, (sl_uint32)internetProtocol);
	}

	Ref<Socket> Socket::openTcp_IPv6()
	{
		return open(SocketType::StreamIPv6);
	}

	Ref<Socket> Socket::openDatagram_IPv6(NetworkInternetProtocol internetProtocol)
	{
		return open(SocketType::DatagramIPv6, (sl_uint32)internetProtocol);
	}

	Ref<Socket> Socket::openUdp_IPv6()
	{
		return open(SocketType::DatagramIPv6);
	}

	Ref<Socket> Socket::openRaw_IPv6(NetworkInternetProtocol internetProtocol)
	{
		return open(SocketType::RawIPv6, (sl_uint32)internetProtocol);
	}

	Ref<Socket> Socket::openPacketRaw(NetworkLinkProtocol linkProtocol)
	{
		return open(SocketType::PacketRaw, (sl_uint32)linkProtocol);
	}

	Ref<Socket> Socket::openPacketDatagram(NetworkLinkProtocol linkProtocol)
	{
		return open(SocketType::PacketDatagram, (sl_uint32)linkProtocol);
	}

	void Socket::close()
	{
		if (m_socket != SLIB_SOCKET_INVALID_HANDLE) {
			_priv_Socket_close(m_socket);
			m_socket = SLIB_SOCKET_INVALID_HANDLE;
		}
		m_type = SocketType::None;
		m_lastError = SocketError::None;
	}

	sl_bool Socket::isOpened() const
	{
		return m_socket != SLIB_SOCKET_INVALID_HANDLE;
	}

	sl_socket Socket::getHandle() const
	{
		return m_socket;
	}

	SocketType Socket::getType() const
	{
		return m_type;
	}

	String Socket::getTypeText() const
	{
		switch (getType()) {
			case SocketType::Stream:
			{
				SLIB_STATIC_STRING(s, "Stream/IPv4");
				return s;
			}
			case SocketType::Datagram:
			{
				SLIB_STATIC_STRING(s, "Datagram/IPv4");
				return s;
			}
			case SocketType::Raw:
			{
				SLIB_STATIC_STRING(s, "Raw/IPv4");
				return s;
			}
			case SocketType::StreamIPv6:
			{
				SLIB_STATIC_STRING(s, "Stream/IPv6");
				return s;
			}
			case SocketType::DatagramIPv6:
			{
				SLIB_STATIC_STRING(s, "Datagram/IPv6");
				return s;
			}
			case SocketType::RawIPv6:
			{
				SLIB_STATIC_STRING(s, "Raw/IPv6");
				return s;
			}
			case SocketType::PacketRaw:
			{
				SLIB_STATIC_STRING(s, "Raw/Packet");
				return s;
			}
			case SocketType::PacketDatagram:
			{
				SLIB_STATIC_STRING(s, "Datagram/Packet");
				return s;
			}
			default:
			{
				SLIB_STATIC_STRING(s, "None");
				return s;
			}
		}
	}

	sl_bool Socket::isStream() const
	{
		return m_type == SocketType::Stream || m_type == SocketType::StreamIPv6;
	}

	sl_bool Socket::isDatagram() const
	{
		return m_type == SocketType::Datagram || m_type == SocketType::DatagramIPv6;
	}

	sl_bool Socket::isRaw() const
	{
		return m_type == SocketType::Raw || m_type == SocketType::RawIPv6;
	}

	sl_bool Socket::isPacket() const
	{
		return m_type == SocketType::PacketRaw || m_type == SocketType::PacketDatagram;
	}

	sl_bool Socket::isIPv4(SocketType type)
	{
		return ((int)type & (int)(SocketType::MASK_ADDRESS_FAMILY)) == (int)(SocketType::ADDRESS_FAMILY_IPv4);
	}
	
	sl_bool Socket::isIPv4() const
	{
		return isIPv4(m_type);
	}
	
	sl_bool Socket::isIPv6(SocketType type)
	{
		return ((int)type & (int)(SocketType::MASK_ADDRESS_FAMILY)) == (int)(SocketType::ADDRESS_FAMILY_IPv6);
	}

	sl_bool Socket::isIPv6() const
	{
		return isIPv6(m_type);
	}

	SocketError Socket::getLastError() const
	{
		return m_lastError;
	}

	String Socket::getLastErrorMessage() const
	{
		return getErrorMessage(m_lastError);
	}

	sl_bool Socket::shutdown(SocketShutdownMode mode)
	{
		if (isOpened()) {
#if defined(SLIB_PLATFORM_IS_WINDOWS)
			int ret = ::shutdown((SOCKET)(m_socket), mode == SocketShutdownMode::Receive ? SD_RECEIVE : mode == SocketShutdownMode::Send ? SD_SEND : SD_BOTH);
#else
			int ret = ::shutdown((SOCKET)(m_socket), mode == SocketShutdownMode::Receive ? SHUT_RD : mode == SocketShutdownMode::Send ? SHUT_WR : SHUT_RDWR);
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
			if (isIPv4()) {
				if (address.ip.isIPv4()) {
					size_addr = _priv_Socket_apply_address(m_type, addr, address);
				} else if (address.ip.isNone()) {
					SocketAddress addrAny;
					addrAny.ip = IPv4Address::zero();
					addrAny.port = address.port;
					size_addr = _priv_Socket_apply_address(m_type, addr, addrAny);
				} else {
					_setError(SocketError::BindInvalidAddress);
					return sl_false;
				}
			} else if (isIPv6()) {
				if (address.ip.isIPv4()) {
					size_addr = _priv_Socket_apply_address(m_type, addr, address);
				} else if (address.ip.isIPv6()) {
					size_addr = _priv_Socket_apply_address(m_type, addr, address);
				} else if (address.ip.isNone()) {
					SocketAddress addrAny;
					addrAny.ip = IPv6Address::zero();
					addrAny.port = address.port;
					size_addr = _priv_Socket_apply_address(m_type, addr, addrAny);
				} else {
					_setError(SocketError::BindInvalidAddress);
					return sl_false;
				}
			} else {
				_setError(SocketError::BindInvalidType);
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
			if (!(isStream())) {
				_setError(SocketError::ListenIsNotSupported);
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
			if (!(isStream())) {
				_setError(SocketError::AcceptIsNotSupported);
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
					_priv_Socket_close(client);
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
			if (!(isStream())) {
				_setError(SocketError::ConnectIsNotSupported);
				return sl_false;
			}
			sockaddr_storage addr;
			sl_uint32 addr_size = _priv_Socket_apply_address(m_type, addr, address);
			if (addr_size) {
				int ret = ::connect((SOCKET)(m_socket), (sockaddr*)&addr, (int)addr_size);
				if (ret != SOCKET_ERROR) {
					return sl_true;
				} else {
					SocketError e = _checkError();
#if defined(SLIB_PLATFORM_IS_WINDOWS)
					return (e == SocketError::WouldBlock);
#else
					return (e == SocketError::InProgress);
#endif
				}
			} else {
				_setError(SocketError::ConnectToInvalidAddress);
				return sl_false;
			}
		} else {
			_setClosedError();
			return sl_false;
		}
	}

	sl_int32 Socket::send(const void* buf, sl_uint32 size)
	{
		if (isOpened()) {
			if (size == 0) {
				return 0;
			}
			if (!(isStream())) {
				_setError(SocketError::SendIsNotSupported);
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
				if (_checkError() == SocketError::WouldBlock) {
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
			if (!(isStream())) {
				_setError(SocketError::ReceiveIsNotSupported);
				return -1;
			}
			sl_int32 ret = (sl_int32)(::recv((SOCKET)(m_socket), (char*)buf, size, 0));
			if (ret >= 0) {
				if (ret == 0) {
					return -1;
				}
				return ret;
			} else {
				if (_checkError() == SocketError::WouldBlock) {
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
			if (!(isDatagram() || isRaw())) {
				_setError(SocketError::SendToIsNotSupported);
				return -1;
			}
			sockaddr_storage addr;
			if (sl_uint32 addr_size = _priv_Socket_apply_address(m_type, addr, address)) {
				sl_int32 ret = (sl_int32)(::sendto((SOCKET)(m_socket), (char*)buf, size, 0, (sockaddr*)&addr, (int)addr_size));
				if (ret >= 0) {
					if (ret == 0) {
						ret = -1;
					}
					return ret;
				} else {
					if (_checkError() == SocketError::WouldBlock) {
						return 0;
					} else {
						return -1;
					}
				}
			} else {
				_setError(SocketError::SendToInvalidAddress);
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
			if (!(isDatagram() || isRaw())) {
				_setError(SocketError::ReceiveFromIsNotSupported);
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
				if (_checkError() == SocketError::WouldBlock) {
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
			if (m_type != SocketType::PacketRaw && m_type != SocketType::PacketDatagram) {
				_setError(SocketError::SendPacketIsNotSupported);
				return -1;
			}

			sockaddr_ll addr;
			addr.sll_family = AF_PACKET;
			addr.sll_protocol = htons((sl_uint16)(info.protocol));
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
				if (_checkError() == SocketError::WouldBlock) {
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
			if (m_type != SocketType::PacketRaw && m_type != SocketType::PacketDatagram) {
				_setError(SocketError::ReceivePacketIsNotSupported);
				return -1;
			}
			sockaddr_ll addr;
			Base::resetMemory(&addr, 0, sizeof(addr));
			int lenAddr = sizeof(addr);
			sl_int32 ret = (sl_int32)(::recvfrom((SOCKET)(m_socket), (char*)buf, size, 0, (sockaddr*)&addr, (socklen_t*)&lenAddr));
			if (ret >= 0) {
				info.iface = addr.sll_ifindex;
				info.protocol = (NetworkLinkProtocol)(ntohs(addr.sll_protocol));
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
				if (_checkError() == SocketError::WouldBlock) {
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

	sl_bool _priv_Socket_setNonBlocking(SOCKET fd, sl_bool flagEnable)
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
			if (_priv_Socket_setNonBlocking((SOCKET)(m_socket), flagEnable)) {
				return sl_true;
			}
		}
		return sl_false;
	}

	sl_bool _priv_Socket_setPromiscuousMode(SOCKET fd, const char* deviceName, sl_bool flagEnable)
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
			String deviceName = _deviceName;
			if (deviceName.isNotEmpty()) {
				if (_priv_Socket_setPromiscuousMode((SOCKET)(m_socket), deviceName.getData(), flagEnable)) {
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

		
	sl_bool Socket::setOption_ReusePort(sl_bool flagEnable)
	{
#if defined(SLIB_PLATFORM_IS_WIN32) || defined(SLIB_PLATFORM_IS_ANDROID) || defined(SLIB_PLATFORM_IS_TIZEN)
		return setOption(SOL_SOCKET, SO_REUSEADDR, flagEnable ? 1 : 0);
#else
		return setOption(SOL_SOCKET, SO_REUSEPORT, flagEnable ? 1 : 0);
#endif
	}
	
	sl_bool Socket::getOption_ReusePort() const
	{
#if defined(SLIB_PLATFORM_IS_WIN32) || defined(SLIB_PLATFORM_IS_ANDROID) || defined(SLIB_PLATFORM_IS_TIZEN)
		return getOption(SOL_SOCKET, SO_REUSEADDR) != 0;
#else
		return getOption(SOL_SOCKET, SO_REUSEPORT) != 0;
#endif
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
		return setOption(SOL_SOCKET, SO_BINDTODEVICE, ifname.getData(), ifname.getLength());
#else
		return sl_false;
#endif
	}

	sl_bool Socket::setOption_IpAddMembership(const IPv4Address& ipMulticast, const IPv4Address& ipInterface)
	{
		ip_mreq mreq;
		mreq.imr_multiaddr.s_addr = htonl(ipMulticast.getInt());
		mreq.imr_interface.s_addr = htonl(ipInterface.getInt());
		return setOption(IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
	}

	sl_bool Socket::setOption_IpDropMembership(const IPv4Address& ipMulticast, const IPv4Address& ipInterface)
	{
		ip_mreq mreq;
		mreq.imr_multiaddr.s_addr = htonl(ipMulticast.getInt());
		mreq.imr_interface.s_addr = htonl(ipInterface.getInt());
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

	SocketError Socket::_setError(SocketError code)
	{
		m_lastError = code;
		return code;
	}

	SocketError Socket::_checkError()
	{
#if defined(SLIB_PLATFORM_IS_WINDOWS)
		int err = WSAGetLastError();
#else
		int err = errno;
#endif
		SocketError ret = SocketError::None;

		switch (err) {
#if defined(SLIB_PLATFORM_IS_WINDOWS)
			case WSAEWOULDBLOCK:
#else
			case EWOULDBLOCK:
#	if (EWOULDBLOCK != EAGAIN)
			case EAGAIN:
#	endif
#endif
				ret = _setError(SocketError::WouldBlock);
				break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
			case WSAENETDOWN:
				ret = _setError(SocketError::NetworkDown);
				break;
#endif

#if defined(SLIB_PLATFORM_IS_WINDOWS)
			case WSAENETRESET:
				ret = _setError(SocketError::NetworkReset);
				break;
#endif

#if defined(SLIB_PLATFORM_IS_WINDOWS)
			case WSAECONNRESET:
#else
			case ECONNRESET:
#endif
				ret = _setError(SocketError::ConnectionReset);
				break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
			case WSAECONNABORTED:
#else
			case ECONNABORTED:
#endif
				ret = _setError(SocketError::ConnectionAbort);
				break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
			case WSAECONNREFUSED:
#else
			case ECONNREFUSED:
#endif
				ret = _setError(SocketError::ConnectionRefused);
				break;


#if defined(SLIB_PLATFORM_IS_WINDOWS)
			case WSAETIMEDOUT:
#else
			case ETIMEDOUT:
#endif
				ret = _setError(SocketError::Timeout);
				break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
			case WSAENOTSOCK:
#else
			case ENOTSOCK:
			case EBADF:
#endif
				ret = _setError(SocketError::NotSocket);
				break;


#if defined(SLIB_PLATFORM_IS_WINDOWS)
			case WSAEADDRINUSE:
#else
			case EADDRINUSE:
#endif
				ret = _setError(SocketError::AddressAlreadyInUse);
				break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
			case WSAENOBUFS:
#else
			case ENOBUFS:
#endif
				ret = _setError(SocketError::NoBufs);
				break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
			case WSA_NOT_ENOUGH_MEMORY:
#else
			case ENOMEM:
#endif
				ret = _setError(SocketError::NoMem);
				break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
			case WSAEINPROGRESS:
#else
			case EINPROGRESS:
#endif
				ret = _setError(SocketError::InProgress);
				break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
			case WSAEDESTADDRREQ:
#else
			case EDESTADDRREQ:
#endif
				ret = _setError(SocketError::DestinationAddressRequired);
				break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
			case WSAEPFNOSUPPORT:
#else
			case EPFNOSUPPORT:
#endif
				ret = _setError(SocketError::ProtocolFamilyNotSupported);
				break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
			case WSAEAFNOSUPPORT:
#else
			case EAFNOSUPPORT:
#endif
				ret = _setError(SocketError::AddressFamilyNotSupported);
				break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
			case WSAEADDRNOTAVAIL:
#else
			case EADDRNOTAVAIL:
#endif
				ret = _setError(SocketError::AddressNotAvailable);
				break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
			case WSAENOTCONN:
#else
			case ENOTCONN:
#endif
				ret = _setError(SocketError::NotConnected);
				break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
			case WSAESHUTDOWN:
#else
			case ESHUTDOWN:
#endif
				ret = _setError(SocketError::Shutdown);
				break;

#if defined(SLIB_PLATFORM_IS_WINDOWS)
			case WSAEACCES:
#else
			case EACCES:
#endif
				ret = _setError(SocketError::Access);
				break;
				
#if defined(SLIB_PLATFORM_IS_UNIX)
			case EPERM:
				ret = _setError(SocketError::NotPermitted);
				break;
#endif

			default:
				ret = _setError((SocketError)((sl_uint32)(SocketError::Unknown) + err));
				break;
		}
		return ret;
	}

	void Socket::_setClosedError()
	{
		_setError(SocketError::Closed);
	}

	String Socket::getErrorMessage(SocketError error)
	{
		switch (error) {
			case SocketError::None:
				return sl_null;
			case SocketError::NetworkDown:
				return "NETDOWN - Network is down";
			case SocketError::NetworkReset:
				return "NETRESET - Network dropped connection on reset";
			case SocketError::ConnectionReset:
				return "CONNRESET - Connection reset by peer";
			case SocketError::ConnectionAbort:
				return "CONNABORTED - Software caused connection abort";
			case SocketError::ConnectionRefused:
				return "CONNREFUSED - Connection refused";
			case SocketError::Timeout:
				return "TIMEOUT - Connection timed out";
			case SocketError::NotSocket:
				return "NOTSOCK - Socket operation on nonsocket";
			case SocketError::AddressAlreadyInUse:
				return "ADDRINUSE - Address already in use";
			case SocketError::NoBufs:
				return "NOBUFS - No buffer space available";
			case SocketError::NoMem:
				return "NOMEM - Insufficient memory available";
			case SocketError::InProgress:
				return "INPROGRESS - Operation now in progress";
			case SocketError::DestinationAddressRequired:
				return "DESTADDRREQ - Destination address required";
			case SocketError::ProtocolFamilyNotSupported:
				return "PFNOSUPPORT - Protocol family not supported";
			case SocketError::AddressNotAvailable:
				return "AFNOSUPPORT - Address family not supported by protocol family";
			case SocketError::NotConnected:
				return "NOTCONN - Socket is not connected";
			case SocketError::Shutdown:
				return "SHUTDOWN - Cannot send after socket shutdown";
			case SocketError::Access:
				return "ACCESS - Permission denied";
			case SocketError::NotPermitted:
				return "EPERM - Operation not permitted";
				
			case SocketError::Closed:
				return "Socket is closed";
			case SocketError::BindInvalidAddress:
				return "Bind to invalid address";
			case SocketError::BindInvalidType:
				return "Bind invalid socket type";
			case SocketError::ListenIsNotSupported:
				return "Listen is not supported";
			case SocketError::AcceptIsNotSupported:
				return "Accept is not supported";
			case SocketError::ConnectIsNotSupported:
				return "Connect is not supported";
			case SocketError::ConnectToInvalidAddress:
				return "Connect to invalid address";
			case SocketError::SendIsNotSupported:
				return "Send is not supported";
			case SocketError::ReceiveIsNotSupported:
				return "Receive is not supported";
			case SocketError::SendToIsNotSupported:
				return "SendTo is not supported";
			case SocketError::SendToInvalidAddress:
				return "SendTo to invalid address";
			case SocketError::ReceiveFromIsNotSupported:
				return "ReceiveFrom is not supported";
			case SocketError::SendPacketIsNotSupported:
				return "SendPacket is not supported";
			case SocketError::SendPacketInvalidAddress:
				return "SendPacket to invalid address";
			case SocketError::ReceivePacketIsNotSupported:
				return "ReceivePacket is not supported";
			default:
				break;
		}
		if (error > SocketError::Unknown) {
			return String("Unknown System Error: ") + ((sl_uint32)error - (sl_uint32)(SocketError::Unknown));
		}
		return String("Not Defined Error: ") + (sl_uint32)error;
	}

	void Socket::clearError()
	{
		_setError(SocketError::None);
	}

	sl_bool Socket::isListening() const
	{
		return getOption_IsListening();
	}

}
