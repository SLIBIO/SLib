#include "../../../inc/slib/core/definition.h"

#include "../../../inc/slib/network/capture.h"
#include "../../../inc/slib/network/socket_address.h"

#include "../../../inc/slib/core/thread.h"
#include "../../../inc/slib/core/log.h"

#define TAG "NetCapture"

#if defined(SLIB_PLATFORM_IS_WIN32)
#include "../../../inc/thirdparty/winpcap/pcap/pcap.h"
#elif defined(SLIB_PLATFORM_IS_UNIX)
#include "../../../inc/thirdparty/pcap/pcap.h"
#include <sys/socket.h>
#endif

#define MAX_PACKET_SIZE 65535

SLIB_NETWORK_NAMESPACE_BEGIN

#if defined(SLIB_PLATFORM_IS_UNIX) || defined(SLIB_PLATFORM_IS_WIN32)
class _NetPcapCapture : public NetCapture
{
public:
	pcap_t* m_handle;
	
	Ref<Thread> m_thread;

	sl_bool m_flagRunning;
	
	_NetPcapCapture()
	{	
		m_flagRunning = sl_true;
	}
	
	~_NetPcapCapture()
	{
		release();
	}
	
	static Ref<_NetPcapCapture> create(const NetCaptureParam& param)
	{
		Ref<_NetPcapCapture> ret;

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
			szName = name8.getBuf();
		}

		char errBuf[PCAP_ERRBUF_SIZE] = { 0 };
		
#if defined(SLIB_PLATFORM_IS_WIN32)
		pcap_t* handle = pcap_open_live(szName, MAX_PACKET_SIZE, param.flagPromiscuous, param.timeoutRead, errBuf);
		if (handle) {
			if (pcap_setbuff(handle, param.sizeBuffer) == 0) {
				ret = new _NetPcapCapture;
				if (ret.isNotNull()) {
					ret->m_handle = handle;
					ret->setListener(param.listener);
					ret->m_thread = Thread::start(SLIB_CALLBACK_CLASS(_NetPcapCapture, _run, ret.get()));
					return ret;
				} 
			} else {
				SLIB_LOG_ERROR(TAG, "Set Buffer Size Failed");
			}
			if (ret.isNull()) {
				pcap_close(handle);
			}
		} else {
			SLIB_LOG_ERROR(TAG, errBuf);
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
								ret = new _NetPcapCapture;
								if (ret.isNotNull()) {
									ret->m_handle = handle;
									ret->setListener(param.listener);
									ret->m_thread = Thread::start(SLIB_CALLBACK_CLASS(_NetPcapCapture, _run, ret.get()));
									return ret;
								}
							} else {
								SLIB_LOG_ERROR(TAG, "Activate Failed");
							}
						} else {
							SLIB_LOG_ERROR(TAG, "Set Timeout Failed");
						}
					} else {
						SLIB_LOG_ERROR(TAG, "Set Promiscuous Mode Failed");
					}
				} else {
					SLIB_LOG_ERROR(TAG, "Set Buffer Size Failed");
				}
			} else {
				SLIB_LOG_ERROR(TAG, "Set Snaplen Failed");
			}
			if (ret.isNull()) {
				pcap_close(handle);
			}
		} else {
			SLIB_LOG_ERROR(TAG, errBuf);
		}
#endif
		return ret;
	}
	
	void release()
	{
		ObjectLocker lock(this);
		if (!m_flagRunning) {
			return;
		}
		m_flagRunning = sl_false;
		
		if (m_thread.isNotNull()) {
			m_thread->finishAndWait();
			m_thread.setNull();
		}
		
		pcap_close(m_handle);
	}
	
	sl_bool isRunning()
	{
		return m_flagRunning;
	}
	
	void _run()
	{
		NetCapturePacket packet;

		while (Thread::isNotStoppingCurrent() && m_flagRunning) {
			pcap_pkthdr* info = sl_null;
			const sl_uint8* data = sl_null;
			int ret = pcap_next_ex(m_handle, &info, &data);
			if (ret == 1) {
				packet.data = data;
				packet.length = info->len;
				if (packet.length > info->caplen) {
					packet.length = info->caplen;
				}
				sl_uint64 t = info->ts.tv_sec;
				t = t * 1000000 + info->ts.tv_usec;
				packet.time = t;
				_onCapturePacket(&packet);
			} else if (ret < 0) {
				break;
			}
		}
	}

	sl_uint32 getLinkType()
	{
		sl_uint32 dt = pcap_datalink(m_handle);
		return dt;
	}

	sl_bool setLinkType(sl_uint32 type)
	{
		int ret = pcap_set_datalink(m_handle, type);
		return ret == 0;
	}

	sl_bool sendPacket(const void* buf, sl_uint32 size)
	{
		if (m_flagRunning) {
			int ret = pcap_sendpacket(m_handle, (const sl_uint8*)buf, size);
			if (ret == 0) {
				return sl_true;
			}
		}
		return sl_false;
	}
	
	String getLastErrorMessage()
	{
		if (m_flagRunning) {
			char* err = pcap_geterr(m_handle);
			if (err) {
				return err;
			}
		}
		return String::null();
	}
};

Ref<NetCapture> NetCapture::createPcap(const NetCaptureParam& param)
{
	Ref<_NetPcapCapture> ret = _NetPcapCapture::create(param);
	return Ref<NetCapture>::from(ret);
}

static void _NetCapture_parseDeviceInfo(pcap_if_t* dev, NetCaptureDevice& out)
{
	out.name = String::fromUtf8(dev->name);
	out.description = String::fromUtf8(dev->description);
	out.flagLoopback = dev->flags & PCAP_IF_LOOPBACK;

	pcap_addr* addr = dev->addresses;
	SocketAddress sa;
	IPv6Address addrIp6;
	
	while (addr) {
		if (addr->addr) {
			switch (addr->addr->sa_family) {
			case AF_INET:
				sa.setSystemSocketAddress(addr->addr);
				out.ipv4Addresses.add(sa.ip.getIPv4());
				break;
			case AF_INET6:
				sa.setSystemSocketAddress(addr->addr);
				out.ipv6Addresses.add(sa.ip.getIPv6());
				break;
			}
		}
		addr = addr->next;
	}
}

List<NetCaptureDevice> NetCapture::getAllPcapDevices()
{
	List<NetCaptureDevice> list;
	char errBuf[PCAP_ERRBUF_SIZE] = { 0 };

	pcap_if_t* devs = NULL;
	int ret = pcap_findalldevs(&devs, errBuf);
	if (ret == 0 && devs) {
		pcap_if_t* dev = devs;
		while (dev) {
			NetCaptureDevice item;
			_NetCapture_parseDeviceInfo(dev, item);
			list.add(item);
			dev = dev->next;
		}
		pcap_freealldevs(devs);
	} else {
		SLIB_LOG_ERROR(TAG, errBuf);
	}
	return list;
}

sl_bool NetCapture::findPcapDevice(const String& name, NetCaptureDevice& out)
{
	char errBuf[PCAP_ERRBUF_SIZE] = { 0 };

	pcap_if_t* devs = NULL;
	int ret = pcap_findalldevs(&devs, errBuf);
	if (ret == 0 && devs) {
		pcap_if_t* dev = devs;
		while (dev) {
			if (name == dev->name || name == dev->description) {
				_NetCapture_parseDeviceInfo(dev, out);
				return sl_true;
			}
			dev = dev->next;
		}
		pcap_freealldevs(devs);
	} else {
		SLIB_LOG_ERROR(TAG, errBuf);
	}
	return sl_false;
}

#else

Ref<NetCapture> NetCapture::createPcap(const NetCaptureParam& param)
{
	return Ref<NetCapture>::null();
}

List<NetCaptureDevice> NetCapture::getAllDevices()
{
	List<NetCaptureDevice> list;
	return list;
}

#endif

SLIB_NETWORK_NAMESPACE_END
