#ifndef CHECKHEADER_SLIB_NETWORK_SOCKET
#define CHECKHEADER_SLIB_NETWORK_SOCKET

#include "definition.h"

#include "constants.h"
#include "socket_address.h"
#include "mac_address.h"

typedef int sl_socket;
#define SLIB_SOCKET_INVALID_HANDLE (-1)

namespace slib
{
	enum class L2PacketType
	{
		Host = 0,
		Broadcast = 1,
		Multicast = 2,
		OtherHost = 3,
		OutGoing = 4,
		Loopback = 5,
		FastRoute = 6
	};
	
	class SLIB_EXPORT L2PacketInfo
	{
	public:
		NetworkLinkProtocol protocol; // physical layer protocol
		sl_uint32 iface; // interface number
		
		L2PacketType type;
		sl_uint32 lenHardwareAddress;
		sl_uint8 hardwareAddress[8];
		
	public:
		void setMacAddress(const MacAddress& address);
		
		sl_bool getMacAddress(MacAddress* address = sl_null);
		
		void clearAddress();
		
	};
	
	enum class SocketType
	{
		None = 0,
		Tcp = 0x01,
		Udp = 0x02,
		Raw = 0x03,
		TcpIPv6 = 0x11,
		UdpIPv6 = 0x12,
		RawIPv6 = 0x13,
		PacketRaw = 0x80,
		PacketDatagram = 0x81
	};
	
	enum class SocketShutdownMode
	{
		Receive = 0,
		Send = 1,
		Both = 2
	};
	
	enum class SocketError
	{
		None = 0,
		WouldBlock = 1,
		NetworkDown = 2,
		NetworkReset = 3,
		ConnectionReset = 4,
		ConnectionAbort = 5,
		ConnectionRefused = 6,
		Timeout = 7,
		NotSocket = 8,
		AddressAlreadyInUse = 9,
		NoBufs = 10,
		NoMem = 11,
		InProgress = 12,
		DestinationAddressRequired = 13, /* sendTo */
		ProtocolFamilyNotSupported = 14,
		AddressFamilyNotSupported = 15,
		AddressNotAvailable = 16,
		NotConnected = 17,
		Shutdown = 18,
		Access = 19, /* ex. broadcast error */
		
		Closed = 101,
		BindInvalidAddress = 102,
		BindInvalidType = 103,
		ListenIsNotSupported = 104,
		AcceptIsNotSupported = 105,
		ConnectIsNotSupported = 106,
		ConnectToInvalidAddress = 107,
		SendIsNotSupported = 108,
		ReceiveIsNotSupported = 109,
		SendToIsNotSupported = 110,
		SendToInvalidAddress = 111,
		ReceiveFromIsNotSupported = 112,
		SendPacketIsNotSupported = 113,
		SendPacketInvalidAddress = 114,
		ReceivePacketIsNotSupported = 115,
		
		Unknown = 10000
		
	};
	
	
	class SLIB_EXPORT Socket : public Referable
	{
		SLIB_DECLARE_OBJECT
		
	private:
		Socket();
		
		~Socket();
		
	public:
		static Ref<Socket> open(SocketType type, sl_uint32 protocol = 0);
		
		static Ref<Socket> openTcp();
		
		static Ref<Socket> openUdp();
		
		static Ref<Socket> openRaw(NetworkInternetProtocol internetProtocol);
		
		static Ref<Socket> openTcp_IPv6();
		
		static Ref<Socket> openUdp_IPv6();
		
		static Ref<Socket> openRaw_IPv6(NetworkInternetProtocol internetProtocol);
		
		static Ref<Socket> openPacketRaw(NetworkLinkProtocol linkProtocol = NetworkLinkProtocol::All);
		
		static Ref<Socket> openPacketDatagram(NetworkLinkProtocol linkProtocol = NetworkLinkProtocol::All);
		
	public:
		void close();
		
		sl_bool isOpened() const;
		
		sl_socket getHandle() const;
		
		SocketType getType() const;
		
		String getTypeText() const;
		
		sl_bool isTcp() const;
		
		sl_bool isUdp() const;
		
		sl_bool isRaw() const;
		
		sl_bool isPacket() const;
		
		sl_bool isIPv4() const;
		
		sl_bool isIPv6() const;
		
		SocketError getLastError() const;
		
		String getLastErrorMessage() const;
		
		sl_bool shutdown(SocketShutdownMode shutMode);
		
		sl_bool bind(const SocketAddress& addr);
		
		sl_bool listen();
		
		sl_bool accept(Ref<Socket>& socket, SocketAddress& address);
		
		sl_bool connect(const SocketAddress& address);
		
		sl_bool connectAndWait(const SocketAddress& address, sl_int32 timeout = -1);
		
		sl_int32 send(const void* buf, sl_uint32 size);
		
		sl_int32 receive(void* buf, sl_uint32 size);
		
		sl_int32 sendTo(const SocketAddress& address, const void* buf, sl_uint32 size);
		
		sl_int32 receiveFrom(SocketAddress& address, void* buf, sl_uint32 size);
		
		sl_int32 sendPacket(const void* buf, sl_uint32 size, const L2PacketInfo& info);
		
		sl_int32 receivePacket(const void* buf, sl_uint32 size, L2PacketInfo& info);
		
		sl_bool setNonBlockingMode(sl_bool flagEnable);
		
		sl_bool setPromiscuousMode(const String& deviceName, sl_bool flagEnable);
		
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
		
		static String getErrorMessage(SocketError error);
		
		void clearError();
		
		sl_bool isListening() const;
		
	private:
		SocketError _setError(SocketError code);
		void _setClosedError();
		SocketError _checkError();
		
		sl_bool setOption(int level, int option, const void* buf, sl_uint32 bufSize);
		sl_bool getOption(int level, int option, void* buf, sl_uint32 bufSize) const;
		sl_bool setOption(int level, int option, sl_uint32 value);
		sl_uint32 getOption(int level, int option) const;
		
	protected:
		SocketType m_type;
		sl_socket m_socket;
		SocketError m_lastError;
		
	};	
}

#endif
