#ifndef CHECKHEADER_SLIB_NETWORK_SOCKET
#define CHECKHEADER_SLIB_NETWORK_SOCKET

#include "definition.h"

#include "constants.h"
#include "socket_address.h"
#include "mac_address.h"

typedef int sl_socket;
#define SLIB_SOCKET_INVALID_HANDLE (-1)

SLIB_NETWORK_NAMESPACE_BEGIN

class SLIB_EXPORT L2PacketInfo
{
public:
	sl_uint16 protocol; // NetworkLinkProtocolType, physical layer protocol
	sl_uint32 iface; // interface number

	enum Type {
		typeHost = 0,
		typeBroadcast = 1,
		typeMulticast = 2,
		typeOtherHost = 3,
		typeOutGoing = 4,
		typeLoopback = 5,
		typeFastRoute = 6
	};
	Type type;
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

class SLIB_EXPORT Socket : public Object
{
	SLIB_DECLARE_OBJECT(Socket, Object)
public:
	enum Type {
		typeNone = 0,
		typeTcp_IPv4 = 0x01,
		typeUdp_IPv4 = 0x02,
		typeRaw_IPv4 = 0x03,
		typeTcp_IPv6 = 0x11,
		typeUdp_IPv6 = 0x12,
		typeRaw_IPv6 = 0x13,
		typePacketRaw = 0x80,
		typePacketDatagram = 0x81
	};
	
	enum ShutdownMode {
		shutReceive = 0,
		shutSend = 1,
		shutBoth = 2
	};
	
	enum ErrorCode
	{
		errorNone = 0,
		errorWouldBlock = 1,
		errorNetworkDown = 2,
		errorNetworkReset = 3,
		errorConnectionReset = 4,
		errorConnectionAbort = 5,
		errorConnectionRefused = 6,
		errorTimeout = 7,
		errorNotSocket = 8,
		errorAddressAlreadyInUse = 9,
		errorNoBufs = 10,
		errorNoMem = 11,
		errorInProgress = 12,
		errorDestinationAddressRequired = 13, /* sendTo */
		errorProtocolFamilyNotSupported = 14,
		errorAddressFamilyNotSupported = 15,
		errorAddressNotAvailable = 16,
		errorNotConnected = 17,
		errorShutdown = 18,
		errorAccess = 19, /* ex. broadcast error */

		errorClosed = 101,
		errorBindInvalidAddress = 102,
		errorBindInvalidType = 103,
		errorListenIsNotSupported = 104,
		errorAcceptIsNotSupported = 105,
		errorConnectIsNotSupported = 106,
		errorConnectToInvalidAddress = 107,
		errorSendIsNotSupported = 108,
		errorReceiveIsNotSupported = 109,
		errorSendToIsNotSupported = 110,
		errorSendToInvalidAddress = 111,
		errorReceiveFromIsNotSupported = 112,
		errorSendPacketIsNotSupported = 113,
		errorSendPacketInvalidAddress = 114,
		errorReceivePacketIsNotSupported = 115,

		errorUnknown = 10000
	};

private:
	Socket();
public:
	~Socket();
	
public:
	static Ref<Socket> open(Type type, sl_uint32 protocol = 0);

	SLIB_INLINE static Ref<Socket> openTcp_IPv4()
	{
		return open(typeTcp_IPv4);
	}

	SLIB_INLINE static Ref<Socket> openUdp_IPv4()
	{
		return open(typeUdp_IPv4);
	}

	// NetworkInternetProtocolType
	SLIB_INLINE static Ref<Socket> openRaw_IPv4(sl_uint32 internetProtocol)
	{
		return open(typeRaw_IPv4, internetProtocol);
	}

	SLIB_INLINE static Ref<Socket> openTcp_IPv6()
	{
		return open(typeTcp_IPv6);
	}

	SLIB_INLINE static Ref<Socket> openUdp_IPv6()
	{
		return open(typeUdp_IPv6);
	}

	// NetworkInternetProtocolType
	SLIB_INLINE static Ref<Socket> openRaw_IPv6(sl_uint32 internetProtocol)
	{
		return open(typeRaw_IPv6, internetProtocol);
	}

	// NetworLinkProtocolType
	SLIB_INLINE static Ref<Socket> openPacketRaw(sl_uint32 linkProtocol = networkLinkProtocol_All)
	{
		return open(typePacketRaw, linkProtocol);
	}

	// NetworLinkProtocolType
	SLIB_INLINE static Ref<Socket> openPacketDatagram(sl_uint32 linkProtocol = networkLinkProtocol_All)
	{
		return open(typePacketDatagram, linkProtocol);
	}

	void close();

	SLIB_INLINE sl_bool isOpened() const
	{
		return m_socket != SLIB_SOCKET_INVALID_HANDLE;
	}

	SLIB_INLINE sl_socket getHandle() const
	{
		return m_socket;
	}

	SLIB_INLINE Type getType() const
	{
		return m_type;
	}

	String getTypeText() const;

	SLIB_INLINE sl_bool isTcp() const
	{
		Type type = getType();
		return type == typeTcp_IPv4 || type == typeTcp_IPv6;
	}

	SLIB_INLINE sl_bool isUdp() const
	{
		Type type = getType();
		return type == typeUdp_IPv4 || type == typeUdp_IPv6;
	}

	SLIB_INLINE sl_bool isRaw() const
	{
		Type type = getType();
		return type == typeRaw_IPv4 || type == typeRaw_IPv6;
	}

	SLIB_INLINE sl_bool isPacket() const
	{
		Type type = getType();
		return type == typePacketRaw || type == typePacketDatagram;
	}

	SLIB_INLINE sl_bool isIPv4() const
	{
		Type type = getType();
		return type == typeTcp_IPv4 || type == typeUdp_IPv4 || type == typeRaw_IPv4;
	}

	SLIB_INLINE sl_bool isIPv6() const
	{
		Type type = getType();
		return type == typeTcp_IPv6 || type == typeUdp_IPv6 || type == typeRaw_IPv6;
	}

	SLIB_INLINE sl_uint32 getLastError() const
	{
		return m_lastError;
	}

public:
	sl_bool shutdown(ShutdownMode shutMode);
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

	sl_bool getLocalAddress(SocketAddress& out);
	sl_bool getRemoteAddress(SocketAddress& out);

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
		_setError(errorNone);
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
	Type m_type;
	sl_socket m_socket;
	sl_uint32 m_lastError;
};
SLIB_NETWORK_NAMESPACE_END

#endif
