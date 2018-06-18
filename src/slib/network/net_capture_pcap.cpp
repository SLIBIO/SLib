/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/network/capture.h"

#if defined(SLIB_PLATFORM_IS_UNIX) || defined(SLIB_PLATFORM_IS_WIN32)

#include "slib/network/socket_address.h"

#include "slib/core/thread.h"
#include "slib/core/log.h"

#define TAG "NetCapture"

#if defined(SLIB_PLATFORM_IS_WIN32)
#include "winpcap/pcap/pcap.h"
#elif defined(SLIB_PLATFORM_IS_UNIX)
#include "pcap/pcap.h"
#include <sys/socket.h>
#endif

#define MAX_PACKET_SIZE 65535

namespace slib
{

	class _priv_NetPcapCapture : public NetCapture
	{
	public:
		pcap_t* m_handle;
		
		Ref<Thread> m_thread;

		sl_bool m_flagInit;
		sl_bool m_flagRunning;
		
	public:
		_priv_NetPcapCapture()
		{
			m_flagInit = sl_false;
			m_flagRunning = sl_false;
		}
		
		~_priv_NetPcapCapture()
		{
			release();
		}
		
	public:
		static Ref<_priv_NetPcapCapture> create(const NetCaptureParam& param)
		{
			String name = param.deviceName;		
			String name8;
			const char* szName;
			if (name.isEmpty()) {
				szName = "any";
			} else {
#if defined(SLIB_PLATFORM_IS_WIN32)
				if (name.startsWith('{')) {
					name = "\\Device\\NPF_" + name;
				}
#endif
				name8 = name;
				szName = name8.getData();
			}

			char errBuf[PCAP_ERRBUF_SIZE] = { 0 };
		
#if defined(SLIB_PLATFORM_IS_WIN32)
			pcap_t* handle = pcap_open_live(szName, MAX_PACKET_SIZE, param.flagPromiscuous, param.timeoutRead, errBuf);
			if (handle) {
				if (pcap_setbuff(handle, param.sizeBuffer) == 0) {
					Ref<_priv_NetPcapCapture> ret = new _priv_NetPcapCapture;
					if (ret.isNotNull()) {
						ret->_initWithParam(param);
						ret->m_handle = handle;
						ret->m_thread = Thread::create(SLIB_FUNCTION_CLASS(_priv_NetPcapCapture, _run, ret.get()));
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
				} else {
					LogError(TAG, "Set Buffer Size Failed");
				}
				pcap_close(handle);
			} else {
				LogError(TAG, errBuf);
			}
#else
			pcap_t* handle = pcap_create(szName, errBuf);
			if (handle) {
				if (pcap_set_snaplen(handle, MAX_PACKET_SIZE) == 0) {
					if (pcap_set_buffer_size(handle, param.sizeBuffer) == 0) {
						if (pcap_set_promisc(handle, param.flagPromiscuous) == 0) {
							if (pcap_set_timeout(handle, param.timeoutRead) == 0) {
								int iRet = pcap_activate(handle);
								if (iRet == 0 || iRet == PCAP_WARNING || iRet == PCAP_WARNING_PROMISC_NOTSUP || iRet == PCAP_WARNING_TSTAMP_TYPE_NOTSUP) {
									Ref<_priv_NetPcapCapture> ret = new _priv_NetPcapCapture;
									if (ret.isNotNull()) {
										ret->_initWithParam(param);
										ret->m_handle = handle;
										ret->m_thread = Thread::create(SLIB_FUNCTION_CLASS(_priv_NetPcapCapture, _run, ret.get()));
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
								} else {
									LogError(TAG, "Activate Failed");
								}
							} else {
								LogError(TAG, "Set Timeout Failed");
							}
						} else {
							LogError(TAG, "Set Promiscuous Mode Failed");
						}
					} else {
						LogError(TAG, "Set Buffer Size Failed");
					}
				} else {
					LogError(TAG, "Set Snaplen Failed");
				}
				pcap_close(handle);
			} else {
				LogError(TAG, errBuf);
			}
#endif
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
			
			pcap_close(m_handle);
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

			sl_uint8 buf[MAX_PACKET_SIZE];

			while (Thread::isNotStoppingCurrent() && m_flagRunning) {
				pcap_pkthdr* info = sl_null;
				const sl_uint8* data = sl_null;
				int ret = pcap_next_ex(m_handle, &info, &data);
				if (ret == 1) {
					packet.data = buf;
					packet.length = info->len;
					if (packet.length > info->caplen) {
						packet.length = info->caplen;
					}
					Base::copyMemory(buf, data, packet.length);
					sl_uint64 t = info->ts.tv_sec;
					t = t * 1000000 + info->ts.tv_usec;
					packet.time = t;
					_onCapturePacket(&packet);
				} else if (ret < 0) {
					break;
				}
			}
		}

		NetworkLinkDeviceType getLinkType()
		{
			sl_uint32 dt = pcap_datalink(m_handle);
			return (NetworkLinkDeviceType)dt;
		}

		sl_bool setLinkType(sl_uint32 type)
		{
			int ret = pcap_set_datalink(m_handle, type);
			return ret == 0;
		}

		sl_bool sendPacket(const void* buf, sl_uint32 size)
		{
			if (m_flagInit) {
				int ret = pcap_sendpacket(m_handle, (const sl_uint8*)buf, size);
				if (ret == 0) {
					return sl_true;
				}
			}
			return sl_false;
		}
		
		String getLastErrorMessage()
		{
			if (m_flagInit) {
				char* err = pcap_geterr(m_handle);
				if (err) {
					return err;
				}
			}
			return sl_null;
		}
	};

	Ref<NetCapture> NetCapture::createPcap(const NetCaptureParam& param)
	{
		return _priv_NetPcapCapture::create(param);
	}

	static void _priv_NetCapture_parseDeviceInfo(pcap_if_t* dev, NetCaptureDeviceInfo& _out)
	{
		_out.name = String::fromUtf8(dev->name);
		_out.description = String::fromUtf8(dev->description);
		_out.flagLoopback = dev->flags & PCAP_IF_LOOPBACK;

		pcap_addr* addr = dev->addresses;
		SocketAddress sa;
		
		List<IPv4Address> addrs4;
		List<IPv6Address> addrs6;
		while (addr) {
			if (addr->addr) {
				switch (addr->addr->sa_family) {
				case AF_INET:
					sa.setSystemSocketAddress(addr->addr);
					addrs4.add_NoLock(sa.ip.getIPv4());
					break;
				case AF_INET6:
					sa.setSystemSocketAddress(addr->addr);
					addrs6.add_NoLock(sa.ip.getIPv6());
					break;
				}
			}
			addr = addr->next;
		}
		_out.ipv4Addresses = addrs4;
		_out.ipv6Addresses = addrs6;
	}

	List<NetCaptureDeviceInfo> NetCapture::getAllPcapDevices()
	{
		List<NetCaptureDeviceInfo> list;
		char errBuf[PCAP_ERRBUF_SIZE] = { 0 };

		pcap_if_t* devs = NULL;
		int ret = pcap_findalldevs(&devs, errBuf);
		if (ret == 0 && devs) {
			pcap_if_t* dev = devs;
			while (dev) {
				NetCaptureDeviceInfo item;
				_priv_NetCapture_parseDeviceInfo(dev, item);
				list.add_NoLock(item);
				dev = dev->next;
			}
			pcap_freealldevs(devs);
		} else {
			LogError(TAG, errBuf);
		}
		return list;
	}

	sl_bool NetCapture::findPcapDevice(const String& name, NetCaptureDeviceInfo& _out)
	{
		char errBuf[PCAP_ERRBUF_SIZE] = { 0 };

		pcap_if_t* devs = NULL;
		int ret = pcap_findalldevs(&devs, errBuf);
		if (ret == 0 && devs) {
			pcap_if_t* dev = devs;
			while (dev) {
				if (name == dev->name || name == dev->description) {
					_priv_NetCapture_parseDeviceInfo(dev, _out);
					return sl_true;
				}
				dev = dev->next;
			}
			pcap_freealldevs(devs);
		} else {
			LogError(TAG, errBuf);
		}
		return sl_false;
	}

}

#else

namespace slib
{

	Ref<NetCapture> NetCapture::createPcap(const NetCaptureParam& param)
	{
		return sl_null;
	}

	List<NetCaptureDevice> NetCapture::getAllDevices()
	{
		List<NetCaptureDevice> list;
		return list;
	}
	
}

#endif
