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

class NetCapture;
class SLIB_EXPORT INetCaptureListener
{
public:
	virtual void onCapturePacket(NetCapture* capture, NetCapturePacket* packet) = 0;
};

class SLIB_EXPORT NetCapture : public Object
{
	SLIB_DECLARE_OBJECT(NetCapture, Object)
protected:
	NetCapture();
public:
	~NetCapture();

public:
	struct NetCaptureParam
	{
	public:
		String deviceName; // <null> or <empty string> for any devices
		
		sl_bool flagPromiscuous; // ignored for "any devices" mode, used in pcap mode
		sl_uint32 timeoutRead; // read timeout, in milliseconds, used in pcap mode
		sl_uint32 sizeBuffer; // buffer size, used in pcap mode
		
		sl_uint32 preferedLinkDeviceType; // NetworkLinkDeviceType, used in Packet Socket mode. now supported Ethernet and Raw

		Ptr<INetCaptureListener> listener;

		NetCaptureParam()
		{
			flagPromiscuous = sl_false;
			timeoutRead = 100;
			sizeBuffer = 0x200000; // 2MB (16Mb)
			
			preferedLinkDeviceType = networkLinkDevice_Ethernet;
		}
	};

	struct NetCaptureDevice
	{
		String name;
		String description;
		sl_bool flagLoopback;

		List<IPv4Address> ipv4Addresses;
		List<IPv6Address> ipv6Addresses;
	};

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
	static List<NetCaptureDevice> getAllPcapDevices();
	static sl_bool findPcapDevice(const String& name, NetCaptureDevice& out);
	
protected:
	void _onCapturePacket(NetCapturePacket* packet);

	SLIB_PROPERTY_INLINE(Ptr<INetCaptureListener>, Listener);
};

typedef NetCapture::NetCaptureParam NetCaptureParam;
typedef NetCapture::NetCaptureDevice NetCaptureDevice;

#define SLIB_NETWORK_LINUX_COOKED_FRAME_HEADER_SIZE 16

class SLIB_EXPORT LinuxCookedFrameFormat
{
public:
	enum PacketType {
		typeHost = 0,
		typeBroadcast = 1,
		typeMulticast = 2,
		typeOtherHost = 3,
		typeOutGoing = 4
	};
	SLIB_INLINE PacketType getPacketType() const
	{
		return (PacketType)(MIO::readUint16BE(m_packetType));
	}
	SLIB_INLINE void setPacketType(PacketType type)
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

	// NetworkLinkProtocolType
	SLIB_INLINE sl_uint16 getProtocolType() const
	{
		return MIO::readUint16BE(m_protocol);
	}
	// NetworkLinkProtocolType
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
