#include "../../../inc/slib/network/mac_address.h"

SLIB_NETWORK_NAMESPACE_BEGIN

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

SLIB_NETWORK_NAMESPACE_END
