package slib.util;

public class NumberFormator {

	static char[] bigDigit = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	static char[] smallDigit = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

	public static String toHex(int hex, int cnt, boolean big) {
		String ret = "";
		char[] digit = big ? bigDigit : smallDigit;
		while (hex > 0 || cnt > 0) {
			ret = digit[hex % 16] + ret;
			hex >>= 4;
			cnt--;
		}
		return ret;
	}

	public static String toHex(int hex, boolean big) {
		String ret = "";
		char[] digit = big ? bigDigit : smallDigit;
		while (hex > 0) {
			ret = digit[hex % 16] + ret;
			hex >>= 4;
		}
		return ret;
	}

	public static String toHex(int hex) {
		return toHex(hex, true);
	}

	static public String formatInt(int src, int len) {
		String str = Integer.toString(src);
		int lenOrg = str.length();
		String dest = "";
		if (len > lenOrg) {
			for (int i = 0; i < len - lenOrg; i++) {
				dest += "0";
			}
			dest += str;
			return dest;
		} else {
			return str;
		}
	}

	static public String formatNumber(double value, int precision, boolean bFlagSkipZero) {
		String ret = formatNumber(value, precision);
		if (bFlagSkipZero) {
			int len = ret.length();
			int i = len - 1;
			for (i = len - 1; i >= 0; i--) {
				char ch = ret.charAt(i);
				if (ch == '.') {
					i--;
					break;
				}
				if (ch != '0') {
					break;
				}
			}
			ret = ret.substring(0, i + 1);
		}
		return ret;
	}

	static public String formatNumber(double value, int precision) {
		if (value == 0) {
			if (precision == 0)
				return "0";
			String ret = ".";
			for (int i = 0; i < precision; i++)
				ret += "0";
			return ret;
		}
		String str = String.format("%." + precision + "f", value);
		String orig = str;
		String prefix = "";
		if (str.charAt(0) == '-') {
			prefix = "-";
			str = str.substring(1);
		}
		char[] buf = str.toCharArray();
		int indexDot = str.indexOf(".");
		if (indexDot < 0)
			indexDot = buf.length;
		if (indexDot == buf.length - 1)
			return orig;
		String ret = "";
		for (int i = 0; i < indexDot; i++) {
			if (buf[i] < '0' || buf[i] > '9')
				return orig;
			if (i != 0 && (indexDot - i) % 3 == 0)
				ret += ',';
			ret += buf[i];
		}
		if (indexDot == buf.length)
			return prefix + str;
		ret += ".";
		for (int i = indexDot + 1; i < buf.length; i++) {
			if (buf[i] < '0' || buf[i] > '9')
				return orig;
			ret += buf[i];
		}
		return prefix + ret;
	}
	
	public static String formatZipCode(int zipCode) {
		if (zipCode == 0) {
			return "";
		} else {
			return formatInt(zipCode, 5);
		}
	}
}
