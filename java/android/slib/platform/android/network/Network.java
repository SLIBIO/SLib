package slib.platform.android.network;

import java.net.Inet4Address;
import java.net.Inet6Address;
import java.net.InetAddress;
import java.net.InterfaceAddress;
import java.net.NetworkInterface;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.List;

import android.util.Log;

public class Network {

	public static NetworkDevice[] getAllDevices() {
		ArrayList<NetworkDevice> devices = new ArrayList<NetworkDevice>();
		try {
			Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces();
			for (; en.hasMoreElements();) {
				NetworkInterface intf = en.nextElement();
				if (! intf.isLoopback() && ! intf.isVirtual()) {
					NetworkDevice device = new NetworkDevice();
					ArrayList<String> addr4 = new ArrayList<String>();
					ArrayList<String> addr6 = new ArrayList<String>();
					List<InterfaceAddress> list = intf.getInterfaceAddresses();
					for (int i = 0; i < list.size(); i++) {
						InterfaceAddress addr = list.get(i);
						InetAddress inetAddress = addr.getAddress();
						if (inetAddress != null) {
							if (!inetAddress.isLoopbackAddress()) {
								if (inetAddress instanceof Inet4Address) {
									int p = addr.getNetworkPrefixLength();
									String s = inetAddress.getHostAddress() + "/" + p;
									addr4.add(s);
								} else if (inetAddress instanceof Inet6Address) {
									int p = addr.getNetworkPrefixLength();
									String s = inetAddress.getHostAddress();
									int indexScope = s.indexOf('%');
									if (indexScope > 0) {
										s = s.substring(0, indexScope);										
									}
									s += "/" + p;
									addr6.add(s);
								}
							}
						}
					}
					device.name = intf.getName();
					byte[] macAddr = intf.getHardwareAddress();
					if (macAddr != null) {
						StringBuilder sb = new StringBuilder();
						for (int i = 0; i < macAddr.length; i++) {
							sb.append(String.format("%02X%s", macAddr[i], (i < macAddr.length - 1) ? "-" : ""));		
						}
						device.macAddress = sb.toString();
					} else {
						device.macAddress = "";
					}
					device.addresses_IPv4 = addr4.toArray(new String[] {});
					device.addresses_IPv6 = addr6.toArray(new String[] {});
					devices.add(device);
				}
			}
		} catch (Throwable ex) {
			Log.e("Network", ex.toString());
		}
		return devices.toArray(new NetworkDevice[0]);
	}
	
}
