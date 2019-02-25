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
#include "../core/time.h"
#include "../core/string.h"
#include "../core/function.h"

namespace slib
{

	class SLIB_EXPORT NetCapturePacket
	{
	public:
		sl_uint8* data;
		sl_uint32 length;
		Time time;
		
	public:
		NetCapturePacket();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(NetCapturePacket)
		
	};
	
	class SLIB_EXPORT NetCaptureDeviceInfo
	{
	public:
		String name;
		String description;
		sl_bool flagLoopback;
		
		List<IPv4Address> ipv4Addresses;
		List<IPv6Address> ipv6Addresses;
		
	public:
		NetCaptureDeviceInfo();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(NetCaptureDeviceInfo)
		
	};
	
	class NetCapture;
	
	class SLIB_EXPORT NetCaptureParam
	{
	public:
		String deviceName; // <null> or <empty string> for any devices
		
		sl_bool flagPromiscuous; // ignored for "any devices" mode, used in pcap
		sl_uint32 timeoutRead; // read timeout, in milliseconds, used in pcap
		sl_bool flagImmediate; // immediate mode, used in pcap
		sl_uint32 sizeBuffer; // buffer size, used in pcap
		
		NetworkLinkDeviceType preferedLinkDeviceType; // NetworkLinkDeviceType, used in Packet Socket mode. now supported Ethernet and Raw
		
		sl_bool flagAutoStart; // default: true
		
		Function<void(NetCapture*, NetCapturePacket&)> onCapturePacket;
		
	public:
		NetCaptureParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(NetCaptureParam)
		
	};
	
	class SLIB_EXPORT NetCapture : public Object
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		NetCapture();
		
		~NetCapture();
		
	public:
		// libpcap capturing engine
		static Ref<NetCapture> createPcap(const NetCaptureParam& param);
		
		// linux packet datagram socket
		static Ref<NetCapture> createRawPacket(const NetCaptureParam& param);
		
		// raw socket
		static Ref<NetCapture> createRawIPv4(const NetCaptureParam& param);
		
	public:
		virtual void release() = 0;
		
		virtual void start() = 0;
		
		virtual sl_bool isRunning() = 0;
		
		virtual NetworkLinkDeviceType getLinkType() = 0;
		
		virtual sl_bool setLinkType(sl_uint32 type);
		
		// send a L2-packet
		virtual sl_bool sendPacket(const void* buf, sl_uint32 size) = 0;
		
		virtual String getLastErrorMessage();
		
		// Pcap Utiltities
		static List<NetCaptureDeviceInfo> getAllPcapDevices();
		
		static sl_bool findPcapDevice(const String& name, NetCaptureDeviceInfo& _out);
		
	protected:
		void _initWithParam(const NetCaptureParam& param);
		
		void _onCapturePacket(NetCapturePacket& packet);
		
	protected:
		Function<void(NetCapture*, NetCapturePacket&)> m_onCapturePacket;
		
	};
	
	
	enum class LinuxCookedPacketType
	{
		Host = 0,
		Broadcast = 1,
		Multicast = 2,
		OtherHost = 3,
		OutGoing = 4
	};
	
	class SLIB_EXPORT LinuxCookedFrame
	{
	public:
		enum
		{
			HeaderSize = 16
		};
		
	public:
		LinuxCookedPacketType getPacketType() const;
		
		void setPacketType(LinuxCookedPacketType type);
		
		NetworkLinkDeviceType getDeviceType() const;
		
		void setDeviceType(NetworkLinkDeviceType type);
		
		sl_uint16 getAddressLength() const;
		
		void setAddressLength(sl_uint16 len);
		
		const sl_uint8* getAddress() const;
		
		sl_uint8* getAddress();
		
		NetworkLinkProtocol getProtocolType() const;
		
		void setProtocolType(NetworkLinkProtocol type);
		
		const sl_uint8* getContent() const;
		
		sl_uint8* getContent();
		
	private:
		sl_uint8 m_packetType[2];
		sl_uint8 m_deviceType[2];
		sl_uint8 m_lenAddress[2];
		sl_uint8 m_address[8];
		sl_uint8 m_protocol[2];
		
	};

}

#endif
