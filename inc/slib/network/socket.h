#ifndef CHECKHEADER_SLIB_NETWORK_SOCKET
#define CHECKHEADER_SLIB_NETWORK_SOCKET

#include "definition.h"

#include "constants.h"
#include "socket_address.h"
#include "mac_address.h"

typedef int sl_socket;
#define SLIB_SOCKET_INVALID_HANDLE (-1)

SLIB_NETWORK_NAMESPACE_BEGIN

enum L2PacketType
{
    l2PacketType_Host = 0,
    l2PacketType_Broadcast = 1,
    l2PacketType_Multicast = 2,
    l2PacketType_OtherHost = 3,
    l2PacketType_OutGoing = 4,
    l2PacketType_Loopback = 5,
    l2PacketType_FastRoute = 6
};

class SLIB_EXPORT L2PacketInfo
{
public:
	sl_uint16 protocol; // NetworkLinkProtocol, physical layer protocol
	sl_uint32 iface; // interface number

	L2PacketType type;
	sl_uint32 lenHardwareAddress;
	sl_uint8 hardwareAddress[8];

public:
	SLIB_INLINE void setMacAddress(const MacAddress& address)
	{
		lenHardwareAddress = 6;
		Base::copyMemory(hardwareAddress, address.m, 6);
		hardwareAddress[6] = 0;
		hardwareAddress[7] = 0;
	}
	
	SLIB_INLINE sl_bool getMacAddress(MacAddress* address = sl_null)
	{
		if (lenHardwareAddress == 6) {
			if (address) {
				address->set(hardwareAddress);
			}
			return sl_true;
		} else {
			return sl_false;
		}
	}
	
	SLIB_INLINE void clearAddress()
	{
		lenHardwareAddress = 0;
		Base::zeroMemory(hardwareAddress, 8);
	}
	
};

enum SocketType
{
    socketType_None = 0,
    socketType_Tcp = 0x01,
    socketType_Udp = 0x02,
    socketType_Raw = 0x03,
    socketType_TcpIPv6 = 0x11,
    socketType_UdpIPv6 = 0x12,
    socketType_RawIPv6 = 0x13,
    socketType_PacketRaw = 0x80,
    socketType_PacketDatagram = 0x81
};

enum SocketShutdownMode
{
    socketShutdownMode_Receive = 0,
    socketShutdownMode_Send = 1,
    socketShutdownMode_Both = 2
};

enum SocketError
{
    socketError_None = 0,
    socketError_WouldBlock = 1,
    socketError_NetworkDown = 2,
    socketError_NetworkReset = 3,
    socketError_ConnectionReset = 4,
    socketError_ConnectionAbort = 5,
    socketError_ConnectionRefused = 6,
    socketError_Timeout = 7,
    socketError_NotSocket = 8,
    socketError_AddressAlreadyInUse = 9,
    socketError_NoBufs = 10,
    socketError_NoMem = 11,
    socketError_InProgress = 12,
    socketError_DestinationAddressRequired = 13, /* sendTo */
    socketError_ProtocolFamilyNotSupported = 14,
    socketError_AddressFamilyNotSupported = 15,
    socketError_AddressNotAvailable = 16,
    socketError_NotConnected = 17,
    socketError_Shutdown = 18,
    socketError_Access = 19, /* ex. broadcast error */

    socketError_Closed = 101,
    socketError_BindInvalidAddress = 102,
    socketError_BindInvalidType = 103,
    socketError_ListenIsNotSupported = 104,
    socketError_AcceptIsNotSupported = 105,
    socketError_ConnectIsNotSupported = 106,
    socketError_ConnectToInvalidAddress = 107,
    socketError_SendIsNotSupported = 108,
    socketError_ReceiveIsNotSupported = 109,
    socketError_SendToIsNotSupported = 110,
    socketError_SendToInvalidAddress = 111,
    socketError_ReceiveFromIsNotSupported = 112,
    socketError_SendPacketIsNotSupported = 113,
    socketError_SendPacketInvalidAddress = 114,
    socketError_ReceivePacketIsNotSupported = 115,

    socketError_Unknown = 10000
};


class SLIB_EXPORT Socket : public Referable
{
	SLIB_DECLARE_ROOT_OBJECT(Socket)
private:
	Socket();
public:
	~Socket();
	
public:
	static Ref<Socket> open(SocketType type, sl_uint32 protocol = 0);

	SLIB_INLINE static Ref<Socket> openTcp()
	{
		return open(socketType_Tcp);
	}

	SLIB_INLINE static Ref<Socket> openUdp()
	{
		return open(socketType_Udp);
	}

	// NetworkInternetProtocolType
	SLIB_INLINE static Ref<Socket> openRaw(sl_uint32 internetProtocol)
	{
		return open(socketType_Raw, internetProtocol);
	}

	SLIB_INLINE static Ref<Socket> openTcp_IPv6()
	{
		return open(socketType_TcpIPv6);
	}

	SLIB_INLINE static Ref<Socket> openUdp_IPv6()
	{
		return open(socketType_UdpIPv6);
	}

	// NetworkInternetProtocolType
	SLIB_INLINE static Ref<Socket> openRaw_IPv6(sl_uint32 internetProtocol)
	{
		return open(socketType_RawIPv6, internetProtocol);
	}

	// NetworLinkProtocolType
	SLIB_INLINE static Ref<Socket> openPacketRaw(sl_uint32 linkProtocol = networkLinkProtocol_All)
	{
		return open(socketType_PacketRaw, linkProtocol);
	}

	// NetworLinkProtocolType
	SLIB_INLINE static Ref<Socket> openPacketDatagram(sl_uint32 linkProtocol = networkLinkProtocol_All)
	{
		return open(socketType_PacketDatagram, linkProtocol);
	}

public:
	void close();

	SLIB_INLINE sl_bool isOpened() const
	{
		return m_socket != SLIB_SOCKET_INVALID_HANDLE;
	}

	SLIB_INLINE sl_socket getHandle() const
	{
		return m_socket;
	}

	SLIB_INLINE SocketType getType() const
	{
		return m_type;
	}

	String getTypeText() const;

	SLIB_INLINE sl_bool isTcp() const
	{
		return m_type == socketType_Tcp || m_type == socketType_TcpIPv6;
	}

	SLIB_INLINE sl_bool isUdp() const
	{
		return m_type == socketType_Udp || m_type == socketType_UdpIPv6;
	}

	SLIB_INLINE sl_bool isRaw() const
	{
		return m_type == socketType_Raw || m_type == socketType_RawIPv6;
	}

	SLIB_INLINE sl_bool isPacket() const
	{
		return m_type == socketType_PacketRaw || m_type == socketType_PacketDatagram;
	}

	SLIB_INLINE sl_bool isIPv4() const
	{
		return m_type == socketType_Tcp || m_type == socketType_Udp || m_type == socketType_Raw;
	}

	SLIB_INLINE sl_bool isIPv6() const
	{
		return m_type == socketType_TcpIPv6 || m_type == socketType_UdpIPv6 || m_type == socketType_RawIPv6;
	}

	SLIB_INLINE sl_uint32 getLastError() const
	{
		return m_lastError;
	}

public:
	sl_bool shutdown(SocketShutdownMode shutMode);
	
	sl_bool bind(const SocketAddress& addr);
	
	sl_bool listen();
	
	sl_bool accept(Ref<Socket>& socket, SocketAddress& address);
	
	sl_bool connect(const SocketAddress& address);
	
	sl_bool connectAndWait(const SocketAddress& address, sl_int32 timeout = -1);
	
	sl_int32 send(const void* buf, sl_uint32 size);
	
	sl_int32 receive(void* buf, sl_uint32 size);
	
	sl_int32 sendTo(const void* buf, sl_uint32 size, const SocketAddress& address);
	
	sl_int32 receiveFrom(void* buf, sl_uint32 size, SocketAddress& address);
	
	sl_int32 sendBroadcast(const void* buf, sl_int32 size, sl_uint32 port);

	sl_int32 sendPacket(const void* buf, sl_uint32 size, const L2PacketInfo& info);
	
	sl_int32 receivePacket(const void* buf, sl_uint32 size, L2PacketInfo& info);

public:
	sl_bool setNonBlockingMode(sl_bool flagEnable);

	
	sl_bool getLocalAddress(SocketAddress& _out);
	
	sl_bool getRemoteAddress(SocketAddress& _out);

	
	sl_uint32 getOption_Error() const;

	
	sl_bool setOption_Broadcast(sl_bool flagEnable);
	
	sl_bool getOption_Broadcast() const;

	
	sl_bool setOption_ReuseAddress(sl_bool flagEnable);
	
	sl_bool getOption_ReuseAddress() const;

	
	sl_bool setOption_SendBufferSize(sl_uint32 size);
	
	sl_uint32 getOption_SendBufferSize() const;

	
	sl_bool setOption_ReceiveBufferSize(sl_uint32 size);
	
	sl_uint32 getOption_ReceiveBufferSize() const;

	
	sl_bool setOption_SendTimeout(sl_uint32 size); // write-only

	sl_bool setOption_ReceiveTimeout(sl_uint32 size); // write-only

	
	sl_bool setOption_IPv6Only(sl_bool flagEnable);
	
	sl_bool getOption_IPv6Only() const;

	
	sl_bool setOption_TcpNoDelay(sl_bool flagEnable);
	
	sl_bool getOption_TcpNoDelay() const;

	
	sl_bool setOption_IpTTL(sl_uint32 ttl); // max - 255
	
	sl_uint32 getOption_IpTTL() const;

	
	sl_bool getOption_IsListening() const; // read-only

	
	sl_bool setOption_IncludeIpHeader(sl_bool flagEnable);
	
	sl_bool getOption_IncludeIpHeader() const;
	
	
	sl_bool setOption_bindToDevice(const String& ifname);

	/****** multicast ******/
	// interface address may be null
	sl_bool setOption_IpAddMembership(const IPv4Address& ipMulticast, const IPv4Address& ipInterface);
	
	sl_bool setOption_IpDropMembership(const IPv4Address& ipMulticast, const IPv4Address& ipInterface);
	
	sl_bool setOption_IpMulticastLoop(sl_bool flag);
	
	sl_bool getOption_IpMulticastLoop() const;
	
	sl_bool setOption_IpMulticastTTL(sl_uint32 ttl = 1);
	
	sl_uint32 getOption_IpMulticastTTL() const;

	static void initializeSocket();

public:
	static String getErrorMessage(sl_uint32 error);
	SLIB_INLINE void clearError()
	{
		_setError(socketError_None);
	}

	SLIB_INLINE sl_bool isListening() const
	{
		return getOption_IsListening();
	}

private:
	sl_uint32 _setError(sl_uint32 code);
	void _setClosedError();
	sl_uint32 _checkError();

	sl_bool setOption(int level, int option, const void* buf, sl_uint32 bufSize);
	sl_bool getOption(int level, int option, void* buf, sl_uint32 bufSize) const;
	sl_bool setOption(int level, int option, sl_uint32 value);
	sl_uint32 getOption(int level, int option) const;

protected:
	SocketType m_type;
	sl_socket m_socket;
	sl_uint32 m_lastError;
};
SLIB_NETWORK_NAMESPACE_END

#endif
