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

#ifndef CHECKHEADER_SLIB_NETWORK_OS
#define CHECKHEADER_SLIB_NETWORK_OS

#include "definition.h"

#include "ip_address.h"
#include "mac_address.h"
#include "../core/string.h"

namespace slib
{

	class SLIB_EXPORT NetworkInterfaceInfo
	{
	public:
		String name;
		String displayName;
		String description;
		MacAddress macAddress;
		List<IPv4AddressInfo> addresses_IPv4;
		List<IPv6Address> addresses_IPv6;
		
	public:
		NetworkInterfaceInfo();

		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(NetworkInterfaceInfo)
		
	};
	
	class SLIB_EXPORT Network
	{
	public:
		static sl_bool findInterface(const String& nameOrDisplayName, NetworkInterfaceInfo* pInfo);
		
		static List<NetworkInterfaceInfo> findAllInterfaces();
		
		
		// used in L2PacketInfo::iface, and 0 is returned on error (wrapper of if_nametoindex call)
		static sl_uint32 getInterfaceIndexFromName(const String& name);
		
		// wrapper of if_indextoname
		static String getInterfaceNameFromIndex(sl_uint32 index);
		
		
		static List<IPv4Address> findAllIPv4Addresses();
		
		static List<IPv4AddressInfo> findAllIPv4AddressInfos();
		
		static List<IPv6Address> findAllIPv6Addresses();
		
		static List<MacAddress> findAllMacAddresses();
		
		
		static List<IPAddress> getIPAddressesFromHostName(const String& hostName);
		
		static IPAddress getIPAddressFromHostName(const String& hostName);
		
		static IPv4Address getIPv4AddressFromHostName(const String& hostName);
		
		static IPv6Address getIPv6AddressFromHostName(const String& hostName);
		
	};

}

#endif
