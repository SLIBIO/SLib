#include "../../../inc/slib/core/json.h"
#include "../../../inc/slib/core/file.h"

SLIB_NAMESPACE_BEGIN

template <class ST, class CT>
static void _Json_escapeSpaceAndComments(const CT* buf, sl_uint32 len, sl_uint32& pos, sl_bool flagSupportComments)
{
	sl_bool flagLineComment = sl_false;
	sl_bool flagBlockComment = sl_false;
	while (pos < len) {
		sl_bool flagEscape = sl_false;
		CT ch = buf[pos];
		if (flagSupportComments) {
			if (flagLineComment) {
				flagEscape = sl_true;
				if (ch == '\r' || ch == '\n') {
					flagLineComment = sl_false;
				}
			} else {
				if (flagBlockComment) {
					flagEscape = sl_true;
					if (pos >= 2 && ch == '/' && buf[pos-1] == '*') {
						flagBlockComment = sl_false;
					}
				} else {
					if (pos <= len - 2 && ch == '/') {
						if (buf[pos + 1] == '/') {
							flagLineComment = sl_true;
							flagEscape = sl_true;
							pos++;
						} else if (buf[pos + 1] == '*') {
							flagBlockComment = sl_true;
							flagEscape = sl_true;
							pos++;
						}
					}
				}
			}
		}
		if (!flagEscape && ch != '\r' && ch != '\n' && ch != ' ' && ch != '\t') {
			break;
		}
		pos++;
	}
}

template <class ST, class CT>
static Variant _Json_parseJSON(const CT* buf, sl_uint32 len, sl_uint32& pos, sl_bool& flagError, sl_bool flagSupportComments)
{
	pos = 0;
	flagError = sl_true;
	_Json_escapeSpaceAndComments<ST, CT>(buf, len, pos, flagSupportComments);
	if (pos == len) {
		flagError = sl_false;
		return Variant::null();
	}
	CT first = buf[pos];
	if (first == '"' || first == '\'') {
		sl_int32 m = 0;
		sl_bool f = sl_false;
		ST str = ST::parseBackslashEscapes(buf + pos, len - pos, &m, &f);
		pos += m;
		if (f) {
			return Variant::null();
		} else {
			flagError = sl_false;
			return str;
		}
	}
	if (first == '[') {
		pos++;
		if (pos == len) {
			return Variant::null();
		}
		Variant list = Variant::createVariantList();
		sl_bool flagFirst = sl_true;
		while (pos < len) {
			_Json_escapeSpaceAndComments<ST, CT>(buf, len, pos, flagSupportComments);
			if (pos == len) {
				return Variant::null();
			}
			CT ch = buf[pos];
			if (ch == ']') {
				pos++;
				flagError = sl_false;
				return list;
			}
			if (! flagFirst) {
				if (ch == ',') {
					pos++;
				} else {
					return Variant::null();
				}
			}
			_Json_escapeSpaceAndComments<ST, CT>(buf, len, pos, flagSupportComments);
			if (pos == len) {
				return Variant::null();
			}
			if (buf[pos] == ']') {
				pos++;
				flagError = sl_false;
				return list;
			}
			sl_uint32 m = 0;
			sl_bool f = sl_false;
			Variant item = _Json_parseJSON<ST, CT>(buf + pos, len - pos, m, f, flagSupportComments);
			pos += m;
			if (f) {
				return Variant::null();
			}
			list.addListItem(item);
			flagFirst = sl_false;
		}
		return Variant::null();
	}
	if (first == '{') {
		pos++;
		if (pos == len) {
			return Variant::null();
		}
		Variant map = Variant::createVariantHashMap();
		sl_bool flagFirst = sl_true;
		while (pos < len) {
			_Json_escapeSpaceAndComments<ST, CT>(buf, len, pos, flagSupportComments);
			if (pos == len) {
				return Variant::null();
			}
			CT ch = buf[pos];
			if (ch == '}') {
				pos++;
				flagError = sl_false;
				return map;
			}
			if (!flagFirst) {
				if (ch == ',') {
					pos++;
				} else {
					return Variant::null();
				}
			}
			_Json_escapeSpaceAndComments<ST, CT>(buf, len, pos, flagSupportComments);
			if (pos == len) {
				return Variant::null();
			}
			ST key;
			ch = buf[pos];
			if (ch == '}') {
				pos++;
				flagError = sl_false;
				return map;
			} else if (ch == '"' || ch == '\'') {
				sl_int32 m = 0;
				sl_bool f = sl_false;
				key = ST::parseBackslashEscapes(buf + pos, len - pos, &m, &f);
				pos += m;
				if (f) {
					return Variant::null();
				}
			} else {
				sl_uint32 s = pos;
				while (pos < len) {
					CT ch = buf[pos];
					if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '_' || (pos != s && ch >= '0' && ch <= '9')) {
						pos++;
					} else {
						break;
					}
				}
				if (pos == len) {
					return Variant::null();
				}
				key = ST(buf + s, pos - s);
			}
			_Json_escapeSpaceAndComments<ST, CT>(buf, len, pos, flagSupportComments);
			if (pos == len) {
				return Variant::null();
			}
			if (buf[pos] == ':') {
				pos++;
			} else {
				return Variant::null();
			}
			sl_uint32 m = 0;
			sl_bool f = sl_false;
			Variant item = _Json_parseJSON<ST, CT>(buf + pos, len - pos, m, f, flagSupportComments);
			pos += m;
			if (f) {
				return Variant::null();
			}
			map.putField(key, item);
			flagFirst = sl_false;
		}
		return Variant::null();
	}
	{
		sl_uint32 s = pos;
		while (pos < len) {
			CT ch = buf[pos];
			if (ch == '\r' || ch == '\n' || ch == ' ' || ch == '\t' || ch == '/' || ch == ']' || ch == '}' || ch == ',') {
				break;
			} else {
				pos++;
			}
		}
		if (pos == s) {
			flagError = sl_false;
			return Variant::null();
		}
		ST str(buf + s, pos - s);
		SLIB_SAFE_STATIC(ST, _null, "null");
		if (str == _null) {
			flagError = sl_false;
			return Variant::null();
		}
		SLIB_SAFE_STATIC(ST, _true, "true");
		if (str == _true) {
			flagError = sl_false;
			return Variant::fromBoolean(sl_true);
		}
		SLIB_SAFE_STATIC(ST, _false, "false");
		if (str == _false) {
			flagError = sl_false;
			return Variant::fromBoolean(sl_false);
		}
		sl_int64 vi64;
		if (str.parseInt64(&vi64)) {
			flagError = sl_false;
			if (vi64 >= SLIB_INT64(-0x80000000) && vi64 < SLIB_INT64(0x7fffffff)) {
				return (sl_int32)vi64;
			} else {
				return vi64;
			}
		}
		double vf;
		if (str.parseDouble(&vf)) {
			flagError = sl_false;
			return vf;
		}
	}
	return Variant::null();
}

Variant Json::parseJSON(const String& json, sl_bool flagSupportComments)
{
	const sl_char8* buf = json.getBuf();
	sl_uint32 len = json.getLength();
	while (len > 0) {
		sl_char8 ch = buf[len - 1];
		if (ch == '\r' || ch == '\n' || ch == ' ' || ch == '\t') {
			len--;
		} else {
			break;
		}
	}
	if (len == 0) {
		return Variant::null();
	}
	sl_uint32 parsed = 0;
	sl_bool f = sl_false;
	Variant var = _Json_parseJSON<String8, sl_char8>(buf, len, parsed, f, flagSupportComments);
	if (!f) {
		if (parsed == len) {
			return var;
		}
	}
	return Variant::null();
}

Variant Json::parseJSON16(const String16& json, sl_bool flagSupportComments)
{
	const sl_char16* buf = json.getBuf();
	sl_uint32 len = json.getLength();
	while (len > 0) {
		sl_char16 ch = buf[len - 1];
		if (ch == '\r' || ch == '\n' || ch == ' ' || ch == '\t') {
			len--;
		} else {
			break;
		}
	}
	if (len == 0) {
		return Variant::null();
	}
	sl_uint32 parsed = 0;
	sl_bool f = sl_false;
	Variant var = _Json_parseJSON<String16, sl_char16>(buf, len, parsed, f, flagSupportComments);
	if (!f) {
		if (parsed == len) {
			return var;
		}
	}
	return Variant::null();
}

Variant Json::parseJSONFromUtf8TextFile(const String& filePath, sl_bool flagSupportComments)
{
	String json = File::readUtf8Text(filePath);
	return parseJSON(json, flagSupportComments);
}

String Json::toJSON(const Variant& var)
{
	return var.toJSON();
}

SLIB_NAMESPACE_END
