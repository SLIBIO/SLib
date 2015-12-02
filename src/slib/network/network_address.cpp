#include "../../../inc/slib/network/address.h"
#include "../../../inc/slib/network/socket.h"

#include "../../../inc/slib/core/log.h"
#include "../../../inc/slib/core/map.h"

SLIB_NETWORK_NAMESPACE_BEGIN

const sl_uint8 IPv4Address::_any[4] = {0, 0, 0, 0};
const sl_uint8 IPv4Address::_broadcast[4] = { 255, 255, 255, 255 };
const sl_uint8 IPv4Address::_loopback[4] = { 127, 0, 0, 1 };
const sl_uint8 IPv4Address::_multicast_begin[4] = { 224, 0, 0, 0 };
const sl_uint8 IPv4Address::_multicast_end[4] = { 239, 255, 255, 255 };

String IPv4Address::toString() const
{
	String ret = String::fromUint32(a()) + "." + String::fromUint32(b()) + "." + String::fromUint32(c()) + "." + String::fromUint32(d());
	return ret;
}

template <class CT>
SLIB_INLINE sl_int32 _IPv4Address_parse(IPv4Address* obj, const CT* sz, sl_uint32 i, sl_uint32 n)
{
	if (i >= n) {
		return SLIB_PARSE_ERROR;
	}
	int v[4];
	for (int k = 0; k < 4; k++) {
		int t = 0;
		int s = 0;
		for (; i < n; i++) {
			int h = sz[i];
			if (h >= '0' && h <= '9') {
				s = s * 10 + (h - '0');
				if (s > 255) {
					return SLIB_PARSE_ERROR;
				}
				t++;
			} else {
				break;
			}
		}
		if (t == 0) {
			return SLIB_PARSE_ERROR;
		}
		if (k < 3) {
			if (i >= n || sz[i] != '.') {
				return SLIB_PARSE_ERROR;
			}
			i++;
		}
		v[k] = s;
	}
	if (obj) {
		obj->m[0] = (sl_uint8)(v[0]);
		obj->m[1] = (sl_uint8)(v[1]);
		obj->m[2] = (sl_uint8)(v[2]);
		obj->m[3] = (sl_uint8)(v[3]);
	}
	return i;
}

sl_int32 IPv4Address::parse(IPv4Address* out, const char* sz, sl_uint32 posBegin, sl_uint32 n)
{
	return _IPv4Address_parse(out, sz, posBegin, n);
}

sl_int32 IPv4Address::parse(IPv4Address* out, const sl_char16* sz, sl_uint32 posBegin, sl_uint32 n)
{
	return _IPv4Address_parse(out, sz, posBegin, n);
}

sl_bool IPv4Address::parse(const String& _str)
{
	String s = _str;
	sl_uint32 n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _IPv4Address_parse(this, s.getBuf(), 0, n) == n;
}

void IPv4Address::makeNetworkMask(sl_uint32 networkPrefixLength)
{
	int p = networkPrefixLength;
	if (p >= 32) {
		m[0] = 255;
		m[1] = 255;
		m[2] = 255;
		m[3] = 255;
	} else if (p == 31) {
		m[0] = 255;
		m[1] = 255;
		m[2] = 255;
		m[3] = 254;
	} else if (p == 0) {
		m[0] = 0;
		m[1] = 0;
		m[2] = 0;
		m[3] = 0;
	} else {
		int t = ((1 << (p + 1)) - 1) << (32 - p);
		m[0] = (t >> 24) & 255;
		m[1] = (t >> 16) & 255;
		m[2] = (t >> 8) & 255;
		m[3] = t & 255;
	}
}

sl_uint32 IPv4Address::getNetworkPrefixLengthFromMask() const
{
	sl_uint32 t = toInt();
	if (t == 0) {
		return 0;
	}
	return 32 - Math::getLeastSignificantBits(t);
}

sl_bool IPv4Address::setHostName(const String& hostName)
{
	*this = NetworkAddress::getIPv4AddressFromHostName(hostName);
	return isNotAny();
}

const sl_uint8 IPv6Address::_any[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
const sl_uint8 IPv6Address::_loopback[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1 };

String IPv6Address::toString() const
{
	String ret;
	for (int i = 0; i < 8; i++) {
		if (i > 0) {
			ret += ":";
		}
		ret += String::fromUint32(get(i), 16, 4);
	}
	return ret;
}

template <class CT>
SLIB_INLINE sl_int32 _IPv6Address_parse(IPv6Address* obj, const CT* sz, sl_uint32 i, sl_uint32 n)
{
	if (i >= n) {
		return SLIB_PARSE_ERROR;
	}
	int k = 0;
	int v[8];
	int skip_START = -1;
	for (k = 0; k < 8;) {
		int t = 0;
		int s = 0;
		for (; i < n; i++) {
			int h = sz[i];
			int x = 0;
			if (h >= '0' && h <= '9') {
				x = h - '0';
			} else if (h >= 'A' && h <= 'F') {
				x = h - ('A' - 10);
			} else if (h >= 'a' && h <= 'f') {
				x = h - ('a' - 10);
			} else {
				break;
			}
			s = (s << 4) | x;
			if (s > 0x10000) {
				return SLIB_PARSE_ERROR;
			}
			t++;
		}
		if (i >= n || sz[i] != ':') {
			if (t == 0) {
				if (skip_START != k) {
					return SLIB_PARSE_ERROR;
				}
			} else {
				v[k] = s;
				k++;
			}
			break;
		}
		if (t == 0) {
			if (k == 0) {
				if (i < n - 1 && sz[i + 1] == ':') {
					skip_START = 0;
					i += 2;
				} else {
					return SLIB_PARSE_ERROR;
				}
			} else {
				if (skip_START >= 0) {
					return SLIB_PARSE_ERROR;
				}
				skip_START = k;
				i++;
			}
		} else {
			v[k] = s;
			k++;
			i++;
		}
	}
	if (k == 8) {
		if (skip_START >= 0) {
			return SLIB_PARSE_ERROR;
		} else {
			if (obj) {
				for (int q = 0; q < 8; q++) {
					obj->set(q, v[q]);
				}
			}
		}
	} else {
		if (skip_START < 0) {
			return SLIB_PARSE_ERROR;
		} else {
			if (obj) {
				int q;
				for (q = 0; q < skip_START; q++) {
					obj->set(q, v[q]);
				}
				int x = skip_START + 8 - k;
				for (; q < x; q++) {
					obj->set(q, 0);
				}
				for (; q < 8; q++) {
					obj->set(q, v[q - 8 + k]);
				}
			}
		}
	}
	return i;
}

sl_int32 IPv6Address::parse(IPv6Address* out, const char* sz, sl_uint32 posStart, sl_uint32 n)
{
	return _IPv6Address_parse(out, sz, posStart, n);
}

sl_int32 IPv6Address::parse(IPv6Address* out, const sl_char16* sz, sl_uint32 posStart, sl_uint32 n)
{
	return _IPv6Address_parse(out, sz, posStart, n);
}

sl_bool IPv6Address::parse(const String& _str)
{
	String s = _str;
	sl_uint32 n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _IPv6Address_parse(this, s.getBuf(), 0, n) == n;
}

sl_bool IPv6Address::setHostName(const String& hostName)
{
	*this = NetworkAddress::getIPv6AddressFromHostName(hostName);
	return isNotAny();
}

const IPAddress::_IPAddress IPAddress::_none = { IPAddress::typeNone, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } };

int IPAddress::compare(const IPAddress& other) const
{
	if (m_type < other.m_type) {
		return -1;
	}
	if (m_type > other.m_type) {
		return 1;
	}
	switch (m_type) {
	case typeNone:
		return 0;
	case typeIPv4:
		return ((IPv4Address*)(void*)(m_data))->compare(*((IPv4Address*)(void*)(other.m_data)));
	case typeIPv6:
		return ((IPv6Address*)(void*)(m_data))->compare(*((IPv6Address*)(void*)(other.m_data)));
	}
	return 0;
}

sl_uint32 IPAddress::hashCode() const
{
	switch (m_type) {
	case typeNone:
		return 0;
	case typeIPv4:
		return ((IPv4Address*)(void*)(m_data))->hashCode();
	case typeIPv6:
		return ((IPv6Address*)(void*)(m_data))->hashCode();
	}
	return 0;
}

String IPAddress::toString() const
{
	switch (m_type)
	{
	case typeIPv4:
		return (*((IPv4Address*)(void*)(m_data))).toString();
	case typeIPv6:
		return (*((IPv6Address*)(void*)(m_data))).toString();
	default:
		return String::null();
	}
}

template <class CT>
SLIB_INLINE sl_int32 _IPAddress_parse(IPAddress* obj, const CT* sz, sl_uint32 posStart, sl_uint32 len)
{
	if (posStart >= len) {
		return SLIB_PARSE_ERROR;
	}
	sl_int32 index;
	IPv4Address a4;
	index = IPv4Address::parse(&a4, sz, posStart, len);
	if (index != SLIB_PARSE_ERROR) {
		if (obj) {
			*obj = a4;
		}
		return index;
	}
	IPv6Address a6;
	index = IPv6Address::parse(&a6, sz, posStart, len);
	if (index != SLIB_PARSE_ERROR) {
		if (obj) {
			*obj = a6;
		}
		return index;
	}
	return SLIB_PARSE_ERROR;
}

sl_int32 IPAddress::parse(IPAddress* out, const char* sz, sl_uint32 posStart, sl_uint32 len)
{
	return _IPAddress_parse(out, sz, posStart, len);
}

sl_int32 IPAddress::parse(IPAddress* out, const sl_char16* sz, sl_uint32 posStart, sl_uint32 len)
{
	return _IPAddress_parse(out, sz, posStart, len);
}

sl_bool IPAddress::parse(const String& _str)
{
	String s = _str;
	sl_uint32 n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _IPAddress_parse(this, s.getBuf(), 0, n) == n;
}

sl_bool IPAddress::setHostName(const String& hostName)
{
	*this = NetworkAddress::getIPAddressFromHostName(hostName);
	return isNotNone();
}

const SocketAddress::_SocketAddress SocketAddress::_none = { { IPAddress::typeNone, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } }, 0 };

int SocketAddress::compare(const SocketAddress& other) const
{
	int c = ip.compare(other.ip);
	if (c == 0) {
		return Compare<sl_int32>::compare(port, other.port);
	}
	return c;
}

sl_uint32 SocketAddress::hashCode() const
{
	sl_uint64 t = ip.hashCode();
	t = t * 31 + port;
	return sl_hash(t);
}

String SocketAddress::toString() const
{
	if (ip.isIPv4()) {
		return ip.toString() + ":" + String::fromInt32(port);
	} else if (ip.isIPv6()) {
		return "[" + ip.toString() + "]:" + String::fromInt32(port);
	} else {
		return ":" + String::fromInt32(port);
	}
}

template <class CT>
SLIB_INLINE sl_int32 _SocketAddress_parse(SocketAddress* obj, const CT* sz, sl_uint32 pos, sl_uint32 len)
{
	if (pos >= len) {
		return SLIB_PARSE_ERROR;
	}
	IPAddress ip;
	if (sz[0] == '[') {
		IPv6Address addr;
		pos++;
		pos = IPv6Address::parse(&addr, sz, pos, len);
		if (pos == SLIB_PARSE_ERROR || pos >= len) {
			return SLIB_PARSE_ERROR;
		}
		if (sz[pos] != ']') {
			return SLIB_PARSE_ERROR;
		}
		pos++;
		ip = addr;
	} else {
		IPv4Address addr;
		pos = IPv4Address::parse(&addr, sz, pos, len);
		if (pos == SLIB_PARSE_ERROR) {
			return SLIB_PARSE_ERROR;
		}
		ip = addr;
	}
	if (pos >= len) {
		return SLIB_PARSE_ERROR;
	}
	if (sz[pos] != ':') {
		return SLIB_PARSE_ERROR;
	}
	pos++;
	sl_uint32 port;
	pos = String::parseUint32(&port, sz, pos, len);
	if (pos == SLIB_PARSE_ERROR) {
		return SLIB_PARSE_ERROR;
	}
	if (obj) {
		obj->ip = ip;
		obj->port = port;
	}
	return pos;
}

sl_int32 SocketAddress::parse(SocketAddress* out, const char* sz, sl_uint32 posBegin, sl_uint32 len)
{
	return _SocketAddress_parse(out, sz, posBegin, len);
}

sl_int32 SocketAddress::parse(SocketAddress* out, const sl_char16* sz, sl_uint32 posBegin, sl_uint32 len)
{
	return _SocketAddress_parse(out, sz, posBegin, len);
}

sl_bool SocketAddress::parse(const String& _str)
{
	String s = _str;
	sl_uint32 n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _SocketAddress_parse(this, s.getBuf(), 0, n) == n;
}

sl_bool SocketAddress::setHostAddress(const String& _address)
{
	String address = _address;
	sl_int32 index = address.lastIndexOf(':');
	if (index < 0) {
		port = 0;
		return ip.setHostName(address);
	} else {
		port = address.substring(index + 1).parseInt32();
		return ip.setHostName(address.substring(0, index));
	}
}

const sl_uint8 MacAddress::_zero[6] = { 0 };
const sl_uint8 MacAddress::_broadcast[6] = { 255, 255, 255, 255, 255, 255 };

String MacAddress::toString(sl_char8 sep) const
{
	String ret;
	for (int i = 0; i < 6; i++) {
		if (i) {
			ret += String(sep);
		}
		ret += String::fromUint32(m[i], 16, 2, sl_true);
	}
	return ret;
}

template <class CT>
SLIB_INLINE sl_int32 _MacAddress_parse(MacAddress* obj, const CT* sz, sl_uint32 i, sl_uint32 n, sl_char8 sep)
{
	sl_char8 sep1 = sep;
	sl_char8 sep2 = sep;
	if (sep == 0) {
		sep1 = '-';
		sep2 = ':';
	}
	int v[6];
	for (int k = 0; k < 6; k++) {
		int t = 0;
		int s = 0;
		for (; i < n; i++) {
			int h = sz[i];
			if (h >= '0' && h <= '9') {
				s = (s << 4) | (h - '0');
				if (s > 255) {
					return SLIB_PARSE_ERROR;
				}
				t++;
			} else if (h >= 'A' && h <= 'F') {
				s = (s << 4) | (h - 'A' + 10);
				if (s > 255) {
					return SLIB_PARSE_ERROR;
				}
				t++;
			} else if (h >= 'a' && h <= 'f') {
				s = (s << 4) | (h - 'a' + 10);
				if (s > 255) {
					return SLIB_PARSE_ERROR;
				}
				t++;
			} else {
				break;
			}
		}
		if (k < 5) {
			if (i >= n || (sz[i] != sep1 && sz[i] != sep2)) {
				return SLIB_PARSE_ERROR;
			}
			i++;
		}
		if (t == 0) {
			return SLIB_PARSE_ERROR;
		}
		v[k] = s;
	}
	if (obj) {
		obj->m[0] = (sl_uint8)(v[0]);
		obj->m[1] = (sl_uint8)(v[1]);
		obj->m[2] = (sl_uint8)(v[2]);
		obj->m[3] = (sl_uint8)(v[3]);
		obj->m[4] = (sl_uint8)(v[4]);
		obj->m[5] = (sl_uint8)(v[5]);
	}
	return i;
}

sl_int32 MacAddress::parse(MacAddress* out, const char* sz, sl_uint32 posBegin, sl_uint32 len, sl_char8 sep)
{
	return _MacAddress_parse(out, sz, posBegin, len, sep);
}

sl_int32 MacAddress::parse(MacAddress* out, const sl_char16* sz, sl_uint32 posBegin, sl_uint32 len, sl_char8 sep)
{
	return _MacAddress_parse(out, sz, posBegin, len, sep);
}

sl_bool MacAddress::parse(const String& _str, sl_char8 sep)
{
	String s = _str;
	sl_uint32 n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _MacAddress_parse(this, s.getBuf(), 0, n, sep) == n;
}


List<IPv4Address> NetworkAddress::findAllIPv4Addresses()
{
	List<IPv4Address> list;
	ListLocker<NetworkDevice> devices(NetworkDevice::findAllDevices());
	for (sl_size i = 0; i < devices.count(); i++) {
		ListLocker<IPv4AddressInfo> addrs(devices[i].addresses_IPv4);
		for (sl_size k = 0; k < addrs.count(); k++) {
			if (addrs[k].address.isHost()) {
				list.add(addrs[k].address);
			}
		}
	}
	return list;
}

List<IPv4AddressInfo> NetworkAddress::findAllIPv4AddressInfos()
{
	List<IPv4AddressInfo> list;
	ListLocker<NetworkDevice> devices(NetworkDevice::findAllDevices());
	for (sl_size i = 0; i < devices.count(); i++) {
		ListLocker<IPv4AddressInfo> addrs(devices[i].addresses_IPv4);
		for (sl_size k = 0; k < addrs.count(); k++) {
			if (addrs[k].address.isHost()) {
				list.add(addrs[k]);
			}
		}
	}
	return list;
}

List<MacAddress> NetworkAddress::findAllMacAddresses()
{
	List<MacAddress> list;
	ListLocker<NetworkDevice> devices(NetworkDevice::findAllDevices());
	for (sl_size i = 0; i < devices.count(); i++) {
		if (devices[i].macAddress.isNotZero()) {
			list.add(devices[i].macAddress);
		}
	}
	return list;
}


sl_bool NetworkDevice::findDevice(const String& _name)
{
	String name = _name;
	ListLocker<NetworkDevice> devices(NetworkDevice::findAllDevices());
	for (sl_size i = 0; i < devices.count(); i++) {
		if (devices[i].displayName == name) {
			*this = devices[i];
			return sl_true;
		}
	}
	return sl_false;
}
SLIB_NETWORK_NAMESPACE_END

#if defined(SLIB_PLATFORM_IS_WINDOWS)
#if defined(SLIB_PLATFORM_IS_DESKTOP)
#include <winsock2.h>
#include <ws2ipdef.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <netioapi.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

SLIB_NETWORK_NAMESPACE_BEGIN
template <>
int Compare<MIB_IPADDRROW, IPv4Address>::compare(const MIB_IPADDRROW& a, const IPv4Address& b)
{
	return Compare<sl_uint32>::compare(a.dwAddr, Endian::swap32LE(b.toInt()));
}
template <>
sl_bool Compare<MIB_IPADDRROW, IPv4Address>::equals(const MIB_IPADDRROW& a, const IPv4Address& b)
{
	return Compare<sl_uint32>::equals(a.dwAddr, Endian::swap32LE(b.toInt()));
}

List<NetworkDevice> NetworkDevice::findAllDevices()
{
	Socket::initializeSocket();

	List<NetworkDevice> ret;
	ULONG ulOutBufLen;

	MIB_IPADDRTABLE* iptable = 0;
	ulOutBufLen = 0;
	if (GetIpAddrTable(iptable, &ulOutBufLen, TRUE) == ERROR_INSUFFICIENT_BUFFER) {
		iptable = (MIB_IPADDRTABLE*)(Base::createMemory(ulOutBufLen));
	}
	if (iptable) {
		if (GetIpAddrTable(iptable, &ulOutBufLen, TRUE) == NO_ERROR) {
			IP_ADAPTER_ADDRESSES* pinfo = 0;
			ulOutBufLen = 0;
			if (GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pinfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
			{
				pinfo = (IP_ADAPTER_ADDRESSES*)(Base::createMemory(ulOutBufLen));
			}
			if (pinfo) {
				if (GetAdaptersAddresses(AF_UNSPEC, 0, NULL, pinfo, &ulOutBufLen) == NO_ERROR)
				{
					IP_ADAPTER_ADDRESSES* adapter = pinfo;
					while (adapter) {
						NetworkDevice device;
						device.name = adapter->AdapterName;
						device.displayName = adapter->FriendlyName;
						device.description = adapter->Description;
						
						IP_ADAPTER_UNICAST_ADDRESS* pip = adapter->FirstUnicastAddress;
						while (pip) {
							SocketAddress sa;
							sa.setSystemSocketAddress(pip->Address.lpSockaddr, pip->Address.iSockaddrLength);
							if (sa.ip.isIPv4()) {
								IPv4AddressInfo a4;
								a4.address = sa.ip.getIPv4();
								sl_uint32 networkPrefixLength = 0;
								if (pip->Length == sizeof(IP_ADAPTER_UNICAST_ADDRESS_LH)) {
									networkPrefixLength = ((IP_ADAPTER_UNICAST_ADDRESS_LH*)pip)->OnLinkPrefixLength;
								} else {
									sl_size indexTable = 0;
									if (BinarySearch<MIB_IPADDRROW, IPv4Address>::search(iptable->table, iptable->dwNumEntries, a4.address, &indexTable)) {
										networkPrefixLength = IPv4Address(Endian::swap32LE(iptable->table[indexTable].dwMask)).getNetworkPrefixLengthFromMask();
									}
								}
								a4.networkPrefixLength = networkPrefixLength;
								device.addresses_IPv4.add(a4);
							} else if (sa.ip.isIPv6()) {
								IPv6AddressInfo a6;
								a6.address = sa.ip.getIPv6();
								device.addresses_IPv6.add(a6);
							}
							pip = pip->Next;
						}
						if (adapter->PhysicalAddressLength == 6) {
							device.macAddress.set(adapter->PhysicalAddress);
						} else {
							device.macAddress.setZero();
						}
						ret.add(device);
						adapter = adapter->Next;
					}
				}
				Base::freeMemory(pinfo);
			}
		}

		Base::freeMemory(iptable);
	}
	return ret;
}

SLIB_NETWORK_NAMESPACE_END

#endif

#elif defined(SLIB_PLATFORM_IS_ANDROID)

#include "../../../inc/slib/core/platform_android.h"

SLIB_NETWORK_NAMESPACE_BEGIN

SLIB_JNI_BEGIN_CLASS(_NetworkDevice, "slib/platform/android/network/NetworkDevice")
	SLIB_JNI_STRING_FIELD(name);
	SLIB_JNI_STRING_FIELD(macAddress);
	SLIB_JNI_OBJECT_FIELD(addresses_IPv4, "[Ljava/lang/String;");
	SLIB_JNI_OBJECT_FIELD(addresses_IPv6, "[Ljava/lang/String;");
SLIB_JNI_END_CLASS

SLIB_JNI_BEGIN_CLASS(_NetworkAddress, "slib/platform/android/network/Network")
	SLIB_JNI_STATIC_METHOD(getAllDevices, "getAllDevices", "()[Lslib/platform/android/network/NetworkDevice;");
SLIB_JNI_END_CLASS

List<NetworkDevice> NetworkDevice::findAllDevices()
{
	List<NetworkDevice> ret;
	if (_NetworkAddress::get().isNotNull() && _NetworkDevice::get().isNotNull()) {
		JniLocal<jobjectArray> jarr = (jobjectArray)(_NetworkAddress::getAllDevices.callObject(sl_null));
		if (jarr.isNotNull()) {
			sl_uint32 n = Jni::getArrayLength(jarr);
			for (sl_uint32 i = 0; i < n; i++) {
				JniLocal<jobject> jdev = Jni::getObjectArrayElement(jarr, i);
				if (jdev.isNotNull()) {
					NetworkDevice dev;
					dev.name = _NetworkDevice::name.get(jdev);
					dev.displayName = dev.name;
					dev.macAddress.setZero();
					dev.macAddress.parse(_NetworkDevice::macAddress.get(jdev));
					JniLocal<jobjectArray> jarrIPv4 = (jobjectArray)(_NetworkDevice::addresses_IPv4.get(jdev));
					if (jarrIPv4.isNotNull()) {
						sl_uint32 nAddr = Jni::getArrayLength(jarrIPv4);
						for (sl_uint32 k = 0; k < nAddr; k++) {
							String saddr = Jni::getStringArrayElement(jarrIPv4, k);
							sl_int32 indexPrefix = saddr.indexOf("/");
							if (indexPrefix > 0) {
								IPv4AddressInfo ip;
								if (ip.address.parse(saddr.substring(0, indexPrefix))) {
									ip.networkPrefixLength = saddr.substring(indexPrefix+1).parseUint32();
									dev.addresses_IPv4.add(ip);
								}
							}
						}

					}
					JniLocal<jobjectArray> jarrIPv6 = (jobjectArray)(_NetworkDevice::addresses_IPv6.get(jdev));
					if (jarrIPv6.isNotNull()) {
						sl_uint32 nAddr = Jni::getArrayLength(jarrIPv6);
						for (sl_uint32 k = 0; k < nAddr; k++) {
							String saddr = Jni::getStringArrayElement(jarrIPv6, k);
							sl_int32 indexPrefix = saddr.indexOf("/");
							if (indexPrefix > 0) {
								IPv6AddressInfo ip;
								if (ip.address.parse(saddr.substring(0, indexPrefix))) {
									dev.addresses_IPv6.add(ip);
								}
							}
						}
					}
					ret.add(dev);
				}
			}
		}
	}
	return ret;
}
SLIB_NETWORK_NAMESPACE_END

#elif defined(SLIB_PLATFORM_IS_UNIX)

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ifaddrs.h>
#include <net/if.h>

#if defined(SLIB_PLATFORM_IS_APPLE)
#define	IFT_ETHER	0x6		/* Ethernet CSMACD */
#include <net/if_dl.h>
#endif
#if defined(SLIB_PLATFORM_IS_LINUX)
#include <linux/if_packet.h>
#endif

SLIB_NETWORK_NAMESPACE_BEGIN
List<NetworkDevice> NetworkDevice::findAllDevices()
{
	Map<String, NetworkDevice> ret;

    struct ifaddrs * adapters = 0;
    getifaddrs(&adapters);

	if (adapters) {
		
		struct ifaddrs* adapter = adapters;
		
		while (adapter) {
			
			String name = adapter->ifa_name;
			
			NetworkDevice* pdev = ret.getItemPtr(name);
			if (!pdev) {
				NetworkDevice dev;
				dev.name = name;
				dev.displayName = name;
				dev.macAddress.setZero();
				ret.put(name, dev);
				pdev = ret.getItemPtr(name);
			}
			if (pdev && adapter->ifa_addr) {
				if (adapter->ifa_addr->sa_family == AF_INET) {
					// ipv4 address
					sockaddr_in* addr = (sockaddr_in*)(adapter->ifa_addr);
					sockaddr_in* mask = (sockaddr_in*)(adapter->ifa_netmask);
					IPv4AddressInfo ip;
					ip.address = IPv4Address(Endian::swap32LE(addr->sin_addr.s_addr));
					ip.networkPrefixLength = IPv4Address(Endian::swap32LE(mask->sin_addr.s_addr)).getNetworkPrefixLengthFromMask();
					pdev->addresses_IPv4.add(ip);
				} else if (adapter->ifa_addr->sa_family == AF_INET6) {
					// ipv6 address
					SocketAddress s;
					s.setSystemSocketAddress(adapter->ifa_addr);
					IPv6AddressInfo ip;
					ip.address = s.ip.getIPv6();
					pdev->addresses_IPv6.add(ip);
				}
#if defined(SLIB_PLATFORM_IS_APPLE)
				else if (adapter->ifa_addr->sa_family == AF_LINK) {
					sockaddr_dl* addr = (sockaddr_dl*)(adapter->ifa_addr);
					if (addr->sdl_type == IFT_ETHER) {
						sl_uint8* base = (sl_uint8*)(addr->sdl_data + addr->sdl_nlen);
						pdev->macAddress.set(base);
					}
				}
#endif
#if defined(SLIB_PLATFORM_IS_LINUX)
				else if (adapter->ifa_addr->sa_family == AF_PACKET) {
					sockaddr_ll* addr = (sockaddr_ll*)(adapter->ifa_addr);
					if (addr->sll_halen == 6) {
						pdev->macAddress.set((sl_uint8*)(addr->sll_addr));
					}
				}
#endif
			}
			adapter = adapter->ifa_next;
		}
		freeifaddrs(adapters);
	}

    return ret.values();
}
SLIB_NETWORK_NAMESPACE_END

#endif

#if defined(SLIB_PLATFORM_IS_UNIX)
#include <netdb.h>
#include <net/if.h>
#endif

SLIB_NETWORK_NAMESPACE_BEGIN
sl_uint32 NetworkDevice::getInterfaceIndexFromName(const String& _name)
{
#if defined(SLIB_PLATFORM_IS_WINDOWS)
	Socket::initializeSocket();
#endif
	String8 name = _name;
	sl_uint32 n = if_nametoindex(name.getBuf());
	return n;
}

String NetworkDevice::getInterfaceNameFromIndex(sl_uint32 index)
{
#if defined(SLIB_PLATFORM_IS_WINDOWS)
	Socket::initializeSocket();
#endif
	char buf[256];
	char* s = if_indextoname(index, buf);
	if (s) {
		return String(s);
	} else {
		return String::null();
	}
}

List<IPAddress> NetworkAddress::getIPAddressesFromHostName(const String& _hostName)
{
	Socket::initializeSocket();

	List<IPAddress> ret;

	String8 hostName = _hostName;
	addrinfo *addrs = sl_null;

	SocketAddress sa;
	int iRet = getaddrinfo(hostName.getBuf(), sl_null, sl_null, &addrs);
	if (iRet == 0) {
		addrinfo* addr = addrs;
		while (addr) {
			sl_int32 lenAddr = (sl_int32)(addr->ai_addrlen);
			if (lenAddr > 0) {
				sa.ip.setNone();
				sa.setSystemSocketAddress(addr->ai_addr, lenAddr);
				if (sa.ip.isNotNone()) {
					ret.add(sa.ip);
				}
			}
			addr = addr->ai_next;
		}
		if (addrs) {
			freeaddrinfo(addrs);
		}
	}
	return ret;
}

IPAddress NetworkAddress::getIPAddressFromHostName(const String& hostName)
{
	ListLocker<IPAddress> list(getIPAddressesFromHostName(hostName));
	sl_size i;
	for (i = 0; i < list.count(); i++) {
		if (list[i].isIPv4()) {
			return list[i];
		}
	}
	for (i = 0; i < list.count(); i++) {
		if (list[i].isIPv6()) {
			return list[i];
		}
	}
	return IPAddress::none();
}

IPv4Address NetworkAddress::getIPv4AddressFromHostName(const String& hostName)
{
	ListLocker<IPAddress> list(getIPAddressesFromHostName(hostName));
	for (sl_size i = 0; i < list.count(); i++) {
		if (list[i].isIPv4()) {
			return list[i].getIPv4();
		}
	}
	return IPv4Address::any();
}

IPv6Address NetworkAddress::getIPv6AddressFromHostName(const String& hostName)
{
	ListLocker<IPAddress> list(getIPAddressesFromHostName(hostName));
	for (sl_size i = 0; i < list.count(); i++) {
		if (list[i].isIPv6()) {
			return list[i].getIPv6();
		}
	}
	return IPv6Address::any();
}
SLIB_NETWORK_NAMESPACE_END
