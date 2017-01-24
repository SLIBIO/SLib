#include "../../../inc/slib/core/json.h"

#include "../../../inc/slib/core/file.h"
#include "../../../inc/slib/core/log.h"

SLIB_NAMESPACE_BEGIN

JsonParseParam::JsonParseParam()
{
	flagLogError = sl_true;
	flagSupportComments = sl_true;
	
	flagError = sl_false;
	errorLine = 0;
	errorColumn = 0;
	errorPosition = 0;
}

String JsonParseParam::getErrorText()
{
	if (flagError) {
		return "(" + String::fromSize(errorLine) + ":" + String::fromSize(errorColumn) + ") " + errorMessage;
	}
	return sl_null;
}

template <class ST, class CT>
class _Json_Parser
{
public:
	const CT* buf;
	sl_size len;
	sl_bool flagSupportComments;
	
	sl_size pos;
	
	sl_bool flagError;
	String errorMessage;
	
	ST strNull;
	ST strTrue;
	ST strFalse;
	
public:
	_Json_Parser();
	
public:
	void escapeSpaceAndComments();
	
	Variant parseJson();

	static Variant parseJson(const CT* buf, sl_size len, JsonParseParam& param);
	
};

template <>
_Json_Parser<String, sl_char8>::_Json_Parser()
{
	SLIB_STATIC_STRING(_null, "null");
	strNull = _null;
	SLIB_STATIC_STRING(_true, "true");
	strTrue = _true;
	SLIB_STATIC_STRING(_false, "false");
	strFalse = _false;
}

template <>
_Json_Parser<String16, sl_char16>::_Json_Parser()
{
	SLIB_STATIC_STRING16_BY_ARRAY(_null, 'n', 'u', 'l', 'l');
	strNull = _null;
	SLIB_STATIC_STRING16_BY_ARRAY(_true, 't', 'r', 'u', 'e');
	strTrue = _true;
	SLIB_STATIC_STRING16_BY_ARRAY(_false, 'f', 'a', 'l', 's', 'e');
	strFalse = _false;
}

template <class ST, class CT>
void _Json_Parser<ST, CT>::escapeSpaceAndComments()
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
		if (!flagEscape && !(SLIB_CHAR_IS_WHITE_SPACE(ch))) {
			break;
		}
		pos++;
	}
}

template <class ST, class CT>
Variant _Json_Parser<ST, CT>::parseJson()
{
	escapeSpaceAndComments();
	if (pos == len) {
		return sl_null;
	}
	
	CT first = buf[pos];
	
	// string
	if (first == '"' || first == '\'') {
		sl_size m = 0;
		sl_bool f = sl_false;
		ST str = ParseUtil::parseBackslashEscapes(buf + pos, len - pos, &m, &f);
		pos += m;
		if (f) {
			flagError = sl_true;
			errorMessage = "String: Missing character  \" or ' ";
			return sl_null;
		}
		return str;
	}
	
	// array
	if (first == '[') {
		pos++;
		escapeSpaceAndComments();
		if (pos == len) {
			flagError = sl_true;
			errorMessage = "Array: Missing character ] ";
			return sl_null;
		}
		if (buf[pos] == ']') {
			pos++;
			return Variant::createVariantList();
		}
		Variant list = Variant::createVariantList();
		while (pos < len) {
			CT ch = buf[pos];
			if (ch == ']' || ch == ',') {
				list.addListElement(Variant::null());
			} else {
				Variant item = parseJson();
				if (flagError) {
					return sl_null;
				}
				list.addListElement(item);
                escapeSpaceAndComments();
                if (pos == len) {
                    flagError = sl_true;
                    errorMessage = "Array: Missing character ] ";
                    return sl_null;
                }
                ch = buf[pos];
			}
			if (ch == ']') {
				pos++;
				return list;
			} else if (ch == ',') {
				pos++;
            } else {
                flagError = sl_true;
                errorMessage = "Array: Missing character ] ";
                return sl_null;
            }
			escapeSpaceAndComments();
			if (pos == len) {
				flagError = sl_true;
				errorMessage = "Array: Missing character ] ";
				return sl_null;
			}
		}
		flagError = sl_true;
		errorMessage = "Array: Missing character ] ";
		return sl_null;
	}
	
	// object
	if (first == '{') {
		pos++;
		if (pos == len) {
			flagError = sl_true;
			errorMessage = "Object: Missing character } ";
			return sl_null;
		}
		Variant map = Variant::createVariantHashMap();
		sl_bool flagFirst = sl_true;
		while (pos < len) {
			escapeSpaceAndComments();
			if (pos == len) {
				flagError = sl_true;
				errorMessage = "Object: Missing character } ";
				return sl_null;
			}
			CT ch = buf[pos];
			if (ch == '}') {
				pos++;
				return map;
			}
			if (!flagFirst) {
				if (ch == ',') {
					pos++;
				} else {
					flagError = sl_true;
					errorMessage = "Object: Missing character , ";
					return sl_null;
				}
			}
			escapeSpaceAndComments();
			if (pos == len) {
				flagError = sl_true;
				errorMessage = "Object: Missing character } ";
				return sl_null;
			}
			ST key;
			ch = buf[pos];
			if (ch == '}') {
				pos++;
				return map;
			} else if (ch == '"' || ch == '\'') {
				sl_size m = 0;
				sl_bool f = sl_false;
				key = ParseUtil::parseBackslashEscapes(buf + pos, len - pos, &m, &f);
				pos += m;
				if (f) {
					flagError = sl_true;
					errorMessage = "Object Item Name: Missing terminating character \" or ' ";
					return sl_null;
				}
			} else {
				sl_size s = pos;
				while (pos < len) {
					CT ch = buf[pos];
					if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == '_' || (pos != s && ch >= '0' && ch <= '9')) {
						pos++;
					} else {
						break;
					}
				}
				if (pos == len) {
					flagError = sl_true;
					errorMessage = "Object: Missing character : ";
					return sl_null;
				}
				key = ST(buf + s, pos - s);
			}
			escapeSpaceAndComments();
			if (pos == len) {
				flagError = sl_true;
				errorMessage = "Object: Missing character : ";
				return sl_null;
			}
			if (buf[pos] == ':') {
				pos++;
			} else {
				flagError = sl_true;
				errorMessage = "Object: Missing character : ";
				return sl_null;
			}
			escapeSpaceAndComments();
			if (pos == len) {
				flagError = sl_true;
				errorMessage = "Object: Missing Item value";
				return sl_null;
			}
			if (buf[pos] == '}' || buf[pos] == ',') {
				map.putField(key, Variant::null());
			} else {
				Variant item = parseJson();
				if (flagError) {
					return sl_null;
				}
				map.putField(key, item);
			}
			flagFirst = sl_false;
		}
		flagError = sl_true;
		errorMessage = "Object: Missing character } ";
		return sl_null;
	}
	{
		sl_size s = pos;
		while (pos < len) {
			CT ch = buf[pos];
			if (ch == '\r' || ch == '\n' || ch == ' ' || ch == '\t' || ch == '/' || ch == ']' || ch == '}' || ch == ',') {
				break;
			} else {
				pos++;
			}
		}
		if (pos == s) {
			flagError = sl_true;
			errorMessage = "Invalid token";
			return sl_null;
		}
		ST str(buf + s, pos - s);
		if (str == strNull) {
			return sl_null;
		}
		if (str == strTrue) {
			return Variant::fromBoolean(sl_true);
		}
		if (str == strFalse) {
			return Variant::fromBoolean(sl_false);
		}
		sl_int64 vi64;
		if (str.parseInt64(10, &vi64)) {
			if (vi64 >= SLIB_INT64(-0x80000000) && vi64 < SLIB_INT64(0x7fffffff)) {
				return (sl_int32)vi64;
			} else {
				return vi64;
			}
		}
		double vf;
		if (str.parseDouble(&vf)) {
			return vf;
		}
	}
	flagError = sl_true;
	errorMessage = "Invalid token";
	return sl_null;
}

template <class ST, class CT>
Variant _Json_Parser<ST, CT>::parseJson(const CT* buf, sl_size len, JsonParseParam& param)
{
	param.flagError = sl_false;
	
	_Json_Parser<ST, CT> parser;
	parser.buf = buf;
	parser.len = len;
	parser.flagSupportComments = param.flagSupportComments;
	
	parser.pos = 0;
	parser.flagError = sl_false;
	
	Variant var = parser.parseJson();
	if (!(parser.flagError)) {
		parser.escapeSpaceAndComments();
		if (parser.pos != len) {
			parser.flagError = sl_true;
			parser.errorMessage = "Invalid token";
		}
		if (!(parser.flagError)) {
			return var;
		}
	}
	
	param.flagError = sl_true;
	param.errorPosition = parser.pos;
	param.errorMessage = parser.errorMessage;
	param.errorLine = ParseUtil::countLineNumber(buf, parser.pos, &(param.errorColumn));
	
	if (param.flagLogError) {
		LogError("Json", param.getErrorText());
	}
	
	return sl_null;
	
}


Variant Json::parseJson(const sl_char8* sz, sl_size len, JsonParseParam& param)
{
	return _Json_Parser<String, sl_char8>::parseJson(sz, len, param);
}

Variant Json::parseJson(const sl_char8* sz, sl_size len)
{
	JsonParseParam param;
	return parseJson(sz, len, param);
}

Variant Json::parseJson(const String& json, JsonParseParam& param)
{
	return _Json_Parser<String, sl_char8>::parseJson(json.getData(), json.getLength(), param);
}

Variant Json::parseJson(const String& json)
{
	JsonParseParam param;
	return parseJson(json, param);
}


Variant Json::parseJson16(const sl_char16* sz, sl_size len, JsonParseParam& param)
{
	return _Json_Parser<String16, sl_char16>::parseJson(sz, len, param);
}

Variant Json::parseJson16(const sl_char16* sz, sl_size len)
{
	JsonParseParam param;
	return Json::parseJson16(sz, len, param);
}

Variant Json::parseJson16(const String16& json, JsonParseParam& param)
{
	return _Json_Parser<String16, sl_char16>::parseJson(json.getData(), json.getLength(), param);
}

Variant Json::parseJson16(const String16& json)
{
	JsonParseParam param;
	return Json::parseJson16(json, param);
}


Variant Json::parseJsonFromTextFile(const String& filePath, JsonParseParam& param)
{
	String16 json = File::readAllText16(filePath);
	return parseJson16(json, param);
}

Variant Json::parseJsonFromTextFile(const String& filePath)
{
	JsonParseParam param;
	return parseJsonFromTextFile(filePath, param);
}

Variant Json::parseJsonUtf8(const Memory& mem, JsonParseParam& param)
{
	return parseJson((sl_char8*)(mem.getData()), mem.getSize(), param);
}

Variant Json::parseJsonUtf8(const Memory& mem)
{
	return parseJson((sl_char8*)(mem.getData()), mem.getSize());
}

Variant Json::parseJson16Utf8(const Memory& mem, JsonParseParam& param)
{
	return parseJson(String16((sl_char8*)(mem.getData()), mem.getSize()), param);
}

Variant Json::parseJson16Utf8(const Memory& mem)
{
	return parseJson(String16((sl_char8*)(mem.getData()), mem.getSize()));
}


String Json::toString(const Variant& var)
{
	return var.toJsonString();
}

Variant Json::createList()
{
	return Variant::createVariantList();
}

Variant Json::createMap()
{
	return Variant::createVariantListMap();
}

Variant Json::toJson(const sl_char8* _in)
{
	return Variant(_in);
}

Variant Json::toJson(const sl_char16* _in)
{
	return Variant(_in);
}

Variant Json::toJson(const sl_char32* _in)
{
	return Variant(_in);
}

template <> void Json::fromJson(const Variant& v, Variant& _out)
{
	_out = v;
}

template <> void Json::fromJson(const Variant& v, Variant& _out, const Variant& def)
{
	if (v.isNotNull()) {
		_out = v;
	} else {
		_out = def;
	}
}

template <> Variant Json::toJson(const Variant& _in)
{
	return _in;
}

template <> void Json::fromJson(const Variant& v, AtomicVariant& _out)
{
	_out = v;
}

template <> void Json::fromJson(const Variant& v, AtomicVariant& _out, const AtomicVariant& def)
{
	if (v.isNotNull()) {
		_out = v;
	} else {
		_out = def;
	}
}

template <> Variant Json::toJson(const AtomicVariant& _in)
{
	return _in;
}

template <> void Json::fromJson(const Variant& v, sl_int32& _out)
{
	_out = v.getInt32();
}

template <> void Json::fromJson(const Variant& v, sl_int32& _out, const sl_int32& def)
{
	_out = v.getInt32(def);
}

template <> Variant Json::toJson(const sl_int32& _in)
{
	return Variant(_in);
}

template <> void Json::fromJson(const Variant& v, sl_uint32& _out)
{
	_out = v.getUint32();
}

template <> void Json::fromJson(const Variant& v, sl_uint32& _out, const sl_uint32& def)
{
	_out = v.getUint32(def);
}

template <> Variant Json::toJson(const sl_uint32& _in)
{
	return Variant(_in);
}

template <> void Json::fromJson(const Variant& v, sl_int64& _out)
{
	_out = v.getInt64();
}

template <> void Json::fromJson(const Variant& v, sl_int64& _out, const sl_int64& def)
{
	_out = v.getInt64(def);
}

template <> Variant Json::toJson(const sl_int64& _in)
{
	return Variant(_in);
}

template <> void Json::fromJson(const Variant& v, sl_uint64& _out)
{
	_out = v.getUint64();
}

template <> void Json::fromJson(const Variant& v, sl_uint64& _out, const sl_uint64& def)
{
	_out = v.getUint64(def);
}

template <> Variant Json::toJson(const sl_uint64& _in)
{
	return Variant(_in);
}

template <> void Json::fromJson(const Variant& v, sl_int8& _out)
{
	_out = (sl_int8)(v.getInt32());
}

template <> void Json::fromJson(const Variant& v, sl_int8& _out, const sl_int8& def)
{
	_out = (sl_int8)(v.getInt32(def));
}

template <> Variant Json::toJson(const sl_int8& _in)
{
	return Variant((sl_int32)_in);
}

template <> void Json::fromJson(const Variant& v, sl_uint8& _out)
{
	_out = (sl_uint8)(v.getUint32());
}

template <> void Json::fromJson(const Variant& v, sl_uint8& _out, const sl_uint8& def)
{
	_out = (sl_uint8)(v.getUint32(def));
}

template <> Variant Json::toJson(const sl_uint8& _in)
{
	return Variant((sl_uint32)_in);
}

template <> void Json::fromJson(const Variant& v, sl_int16& _out)
{
	_out = (sl_int16)(v.getInt32());
}

template <> void Json::fromJson(const Variant& v, sl_int16& _out, const sl_int16& def)
{
	_out = (sl_int16)(v.getInt32(def));
}

template <> Variant Json::toJson(const sl_int16& _in)
{
	return Variant((sl_int32)_in);
}

template <> void Json::fromJson(const Variant& v, sl_uint16& _out)
{
	_out = (sl_uint16)(v.getUint32());
}

template <> void Json::fromJson(const Variant& v, sl_uint16& _out, const sl_uint16& def)
{
	_out = (sl_uint16)(v.getUint32(def));
}

template <> Variant Json::toJson(const sl_uint16& _in)
{
	return Variant((sl_uint32)_in);
}

template <> void Json::fromJson(const Variant& v, float& _out)
{
	_out = v.getFloat();
}

template <> void Json::fromJson(const Variant& v, float& _out, const float& def)
{
	_out = v.getFloat(def);
}

template <> Variant Json::toJson(const float& _in)
{
	return Variant(_in);
}

template <> void Json::fromJson(const Variant& v, double& _out)
{
	_out = v.getDouble();
}

template <> void Json::fromJson(const Variant& v, double& _out, const double& def)
{
	_out = v.getDouble(def);
}

template <> Variant Json::toJson(const double& _in)
{
	return Variant(_in);
}

template <> void Json::fromJson(const Variant& v, bool& _out)
{
	_out = v.getBoolean();
}

template <> void Json::fromJson(const Variant& v, bool& _out, const bool& def)
{
	_out = v.getBoolean(def);
}

template <> Variant Json::toJson(const bool& _in)
{
	return Variant((sl_bool)_in);
}

template <> void Json::fromJson(const Variant& v, String& _out)
{
	_out = v.getString();
}

template <> void Json::fromJson(const Variant& v, String& _out, const String& def)
{
	_out = v.getString(def);
}

template <> Variant Json::toJson(const String& _in)
{
	return Variant(_in);
}

template <> void Json::fromJson(const Variant& v, AtomicString& _out)
{
	_out = v.getString();
}

template <> void Json::fromJson(const Variant& v, AtomicString& _out, const AtomicString& def)
{
	_out = v.getString(def);
}

template <> Variant Json::toJson(const AtomicString& _in)
{
	return Variant(_in);
}

template <> void Json::fromJson(const Variant& v, String16& _out)
{
	_out = v.getString16();
}

template <> void Json::fromJson(const Variant& v, String16& _out, const String16& def)
{
	_out = v.getString16(def);
}

template <> Variant Json::toJson(const String16& _in)
{
	return Variant(_in);
}

template <> void Json::fromJson(const Variant& v, AtomicString16& _out)
{
	_out = v.getString16();
}

template <> void Json::fromJson(const Variant& v, AtomicString16& _out, const AtomicString16& def)
{
	_out = v.getString16(def);
}

template <> Variant Json::toJson(const AtomicString16& _in)
{
	return Variant(_in);
}

template <> void Json::fromJson(const Variant& v, Time& _out)
{
	_out = v.getTime();
}

template <> void Json::fromJson(const Variant& v, Time& _out, const Time& def)
{
	_out = v.getTime(def);
}

template <> Variant Json::toJson(const Time& _in)
{
	return Variant(_in);
}

template <> void Json::fromJson(const Variant& v, List<Variant>& _out)
{
	_out = v.getVariantList();
}

template <> void Json::fromJson(const Variant& v, List<Variant>& _out, const List<Variant>& def)
{
	if (v.isNotNull()) {
		_out = v.getVariantList();
	} else {
		_out = def;
	}
}

template <> Variant Json::toJson(const List<Variant>& _in)
{
	return _in;
}

template <> void Json::fromJson(const Variant& v, AtomicList<Variant>& _out)
{
	_out = v.getVariantList();
}

template <> void Json::fromJson(const Variant& v, AtomicList<Variant>& _out, const AtomicList<Variant>& def)
{
	if (v.isNotNull()) {
		_out = v.getVariantList();
	} else {
		_out = def;
	}
}

template <> Variant Json::toJson(const AtomicList<Variant>& _in)
{
	return _in;
}

template <> void Json::fromJson(const Variant& v, Map<String, Variant>& _out)
{
	_out = v.getVariantMap();
}

template <> void Json::fromJson(const Variant& v, Map<String, Variant>& _out, const Map<String, Variant>& def)
{
	if (v.isNotNull()) {
		_out = v.getVariantMap();
	} else {
		_out = def;
	}
}

template <> Variant Json::toJson(const Map<String, Variant>& _in)
{
	return _in;
}

template <> void Json::fromJson(const Variant& v, AtomicMap<String, Variant>& _out)
{
	_out = v.getVariantMap();
}

template <> void Json::fromJson(const Variant& v, AtomicMap<String, Variant>& _out, const AtomicMap<String, Variant>& def)
{
	if (v.isNotNull()) {
		_out = v.getVariantMap();
	} else {
		_out = def;
	}
}

template <> Variant Json::toJson(const AtomicMap<String, Variant>& _in)
{
	return _in;
}

template <> void Json::fromJson(const Variant& v, List< Map<String, Variant> >& _out)
{
	_out = v.getVariantMapList();
}

template <> void Json::fromJson(const Variant& v, List< Map<String, Variant> >& _out, const List< Map<String, Variant> >& def)
{
	if (v.isNotNull()) {
		_out = v.getVariantMapList();
	} else {
		_out = def;
	}
}

template <> Variant Json::toJson(const List< Map<String, Variant> >& _in)
{
	return _in;
}

template <> void Json::fromJson(const Variant& v, AtomicList< Map<String, Variant> >& _out)
{
	_out = v.getVariantMapList();
}

template <> void Json::fromJson(const Variant& v, AtomicList< Map<String, Variant> >& _out, const AtomicList< Map<String, Variant> >& def)
{
	if (v.isNotNull()) {
		_out = v.getVariantMapList();
	} else {
		_out = def;
	}
}

template <> Variant Json::toJson(const AtomicList< Map<String, Variant> >& _in)
{
	return _in;
}

SLIB_NAMESPACE_END
