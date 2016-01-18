#ifndef CHECKHEADER_SLIB_NETWORK_CAPTURE
#define CHECKHEADER_SLIB_NETWORK_CAPTURE

/*****************************************************************
	
	NetCapture is based on 
	
		libpcap (unix) and winpcap (win32)
		, raw sockets, packet sockets (linux)
		
*****************************************************************/

#include "definition.h"

#include "constants.h"
#include "ip_address.h"

#include "../core/object.h"
#include "../core/pointer.h"
#include "../core/time.h"
#include "../core/string.h"

SLIB_NETWORK_NAMESPACE_BEGIN

class SLIB_EXPORT NetCapturePacket
{
public:
	const sl_uint8* data;
	sl_uint32 length;
	Time time;
};

class SLIB_EXPORT NetCaptureDeviceInfo
{
public:
	String name;
	String description;
	sl_bool flagLoopback;
	
	List<IPv4Address> ipv4Addresses;
	List<IPv6Address> ipv6Addresses;
};

class NetCapture;

class SLIB_EXPORT INetCaptureListener
{
public:
	virtual void onCapturePacket(NetCapture* capture, NetCapturePacket* packet) = 0;
};

class SLIB_EXPORT NetCaptureParam
{
public:
	String deviceName; // <null> or <empty string> for any devices
	
	sl_bool flagPromiscuous; // ignored for "any devices" mode, used in pcap mode
	sl_uint32 timeoutRead; // read timeout, in milliseconds, used in pcap mode
	sl_uint32 sizeBuffer; // buffer size, used in pcap mode
	
	sl_uint32 preferedLinkDeviceType; // NetworkLinkDeviceType, used in Packet Socket mode. now supported Ethernet and Raw
	
	Ptr<INetCaptureListener> listener;
	
public:
	NetCaptureParam();
};

class SLIB_EXPORT NetCapture : public Object
{
	SLIB_DECLARE_OBJECT(NetCapture, Object)

public:
	virtual void release() = 0;
	
	virtual sl_bool isRunning() = 0;

	virtual sl_uint32 getLinkType() = 0;
	
	virtual sl_bool setLinkType(sl_uint32 type);

	// send a L2-packet
	virtual sl_bool sendPacket(const void* buf, sl_uint32 size) = 0;
	
	virtual String getLastErrorMessage();

public:
	// libpcap capturing engine
	static Ref<NetCapture> createPcap(const NetCaptureParam& param);
	
	// linux packet datagram socket
	static Ref<NetCapture> createRawPacket(const NetCaptureParam& param);
	
	// raw socket
	static Ref<NetCapture> createRawIPv4(const NetCaptureParam& param);

	// Pcap Utiltities
	static List<NetCaptureDeviceInfo> getAllPcapDevices();
	
	static sl_bool findPcapDevice(const String& name, NetCaptureDeviceInfo& _out);
	
protected:
	void _onCapturePacket(NetCapturePacket* packet);

protected:
	Ptr<INetCaptureListener> m_listener;
};


#define SLIB_NETWORK_LINUX_COOKED_FRAME_HEADER_SIZE 16

enum LinuxCookedPacketType
{
	linuxCookedPacketType_Host = 0,
	linuxCookedPacketType_Broadcast = 1,
	linuxCookedPacketType_Multicast = 2,
	linuxCookedPacketType_OtherHost = 3,
	linuxCookedPacketType_OutGoing = 4
};

class SLIB_EXPORT LinuxCookedFrameFormat
{
public:
	SLIB_INLINE LinuxCookedPacketType getPacketType() const
	{
		return (LinuxCookedPacketType)(MIO::readUint16BE(m_packetType));
	}
	
	SLIB_INLINE void setPacketType(LinuxCookedPacketType type)
	{
		MIO::writeUint16BE(m_packetType, type);
	}

	
	// NetworkLinkDeviceType
	SLIB_INLINE sl_uint16 getDeviceType() const
	{
		return MIO::readUint16BE(m_deviceType);
	}
	
	// NetworkLinkDeviceType
	SLIB_INLINE void setDeviceType(sl_uint16 type)
	{
		MIO::writeUint16BE(m_deviceType, type);
	}

	
	SLIB_INLINE sl_uint16 getAddressLength() const
	{
		return MIO::readUint16BE(m_lenAddress);
	}
	
	SLIB_INLINE void setAddressLength(sl_uint16 len)
	{
		MIO::writeUint16BE(m_lenAddress, len);
	}

	
	SLIB_INLINE const sl_uint8* getAddress() const
	{
		return m_address;
	}
	
	SLIB_INLINE sl_uint8* getAddress()
	{
		return m_address;
	}

	
	// NetworkLinkProtocol
	SLIB_INLINE sl_uint16 getProtocolType() const
	{
		return MIO::readUint16BE(m_protocol);
	}
	
	// NetworkLinkProtocol
	SLIB_INLINE void setProtocolType(sl_uint16 type)
	{
		MIO::writeUint16BE(m_protocol, type);
	}

	
	SLIB_INLINE static sl_uint32 getHeaderSize()
	{
		return SLIB_NETWORK_LINUX_COOKED_FRAME_HEADER_SIZE;
	}

	
	SLIB_INLINE const sl_uint8* getContent() const
	{
		return ((const sl_uint8*)this) + SLIB_NETWORK_LINUX_COOKED_FRAME_HEADER_SIZE;
	}
	
	SLIB_INLINE sl_uint8* getContent()
	{
		return ((sl_uint8*)this) + SLIB_NETWORK_LINUX_COOKED_FRAME_HEADER_SIZE;
	}

private:
	sl_uint8 m_packetType[2];
	sl_uint8 m_deviceType[2];
	sl_uint8 m_lenAddress[2];
	sl_uint8 m_address[8];
	sl_uint8 m_protocol[2];
};

SLIB_NETWORK_NAMESPACE_END

#endif
