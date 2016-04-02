#include "../../../inc/slib/core/json.h"
#include "../../../inc/slib/core/file.h"

SLIB_NAMESPACE_BEGIN

JsonParseParam::JsonParseParam()
{
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
	return String::null();
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
_Json_Parser<String8, sl_char8>::_Json_Parser()
{
	SLIB_STATIC_STRING8(_null, "null");
	strNull = _null;
	SLIB_STATIC_STRING8(_true, "true");
	strTrue = _true;
	SLIB_STATIC_STRING8(_false, "false");
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
		if (!flagEscape && ch != '\r' && ch != '\n' && ch != ' ' && ch != '\t') {
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
		return Variant::null();
	}
	
	CT first = buf[pos];
	
	// string
	if (first == '"' || first == '\'') {
		sl_size m = 0;
		sl_bool f = sl_false;
		ST str = ST::parseBackslashEscapes(buf + pos, len - pos, &m, &f);
		pos += m;
		if (f) {
			flagError = sl_true;
			errorMessage = "String: Missing character  \" or ' ";
			return Variant::null();
		}
		return str;
	}
	
	// array
	if (first == '[') {
		pos++;
		if (pos == len) {
			flagError = sl_true;
			errorMessage = "Array: Missing character ] ";
			return Variant::null();
		}
		Variant list = Variant::createVariantList();
		sl_bool flagFirst = sl_true;
		while (pos < len) {
			escapeSpaceAndComments();
			if (pos == len) {
				flagError = sl_true;
				errorMessage = "Array: Missing character ] ";
				return Variant::null();
			}
			CT ch = buf[pos];
			if (ch == ']') {
				pos++;
				return list;
			}
			if (! flagFirst) {
				if (ch == ',') {
					pos++;
				} else {
					flagError = sl_true;
					errorMessage = "Array: Missing character , ";
					return Variant::null();
				}
			}
			escapeSpaceAndComments();
			if (pos == len) {
				flagError = sl_true;
				errorMessage = "Array: Missing character ] ";
				return Variant::null();
			}
			if (buf[pos] == ']') {
				pos++;
				return list;
			}
			Variant item = parseJson();
			if (flagError) {
				return Variant::null();
			}
			list.addListItem(item);
			flagFirst = sl_false;
		}
		flagError = sl_true;
		errorMessage = "Array: Missing character ] ";
		return Variant::null();
	}
	
	// object
	if (first == '{') {
		pos++;
		if (pos == len) {
			flagError = sl_true;
			errorMessage = "Object: Missing character } ";
			return Variant::null();
		}
		Variant map = Variant::createVariantHashMap();
		sl_bool flagFirst = sl_true;
		while (pos < len) {
			escapeSpaceAndComments();
			if (pos == len) {
				flagError = sl_true;
				errorMessage = "Object: Missing character } ";
				return Variant::null();
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
					return Variant::null();
				}
			}
			escapeSpaceAndComments();
			if (pos == len) {
				flagError = sl_true;
				errorMessage = "Object: Missing character } ";
				return Variant::null();
			}
			ST key;
			ch = buf[pos];
			if (ch == '}') {
				pos++;
				return map;
			} else if (ch == '"' || ch == '\'') {
				sl_size m = 0;
				sl_bool f = sl_false;
				key = ST::parseBackslashEscapes(buf + pos, len - pos, &m, &f);
				pos += m;
				if (f) {
					flagError = sl_true;
					errorMessage = "Object Item Name: Missing terminating character \" or ' ";
					return Variant::null();
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
					return Variant::null();
				}
				key = ST(buf + s, pos - s);
			}
			escapeSpaceAndComments();
			if (pos == len) {
				flagError = sl_true;
				errorMessage = "Object: Missing character : ";
				return Variant::null();
			}
			if (buf[pos] == ':') {
				pos++;
			} else {
				flagError = sl_true;
				errorMessage = "Object: Missing character : ";
				return Variant::null();
			}
			escapeSpaceAndComments();
			if (pos == len) {
				flagError = sl_true;
				errorMessage = "Object: Missing Item value";
				return Variant::null();
			}
			Variant item = parseJson();
			if (flagError) {
				return Variant::null();
			}
			map.putField(key, item);
			flagFirst = sl_false;
		}
		flagError = sl_true;
		errorMessage = "Object: Missing character } ";
		return Variant::null();
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
			return Variant::null();
		}
		ST str(buf + s, pos - s);
		if (str == strNull) {
			return Variant::null();
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
	return Variant::null();
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
	param.errorLine = ST::countLineNumber(buf, parser.pos, &(param.errorColumn));
	
	return Variant::null();
	
}


Variant Json::parseJson(const sl_char8* sz, sl_size len, JsonParseParam& param)
{
	return _Json_Parser<String8, sl_char8>::parseJson(sz, len, param);
}

Variant Json::parseJson(const sl_char8* sz, sl_size len)
{
	JsonParseParam param;
	return parseJson(sz, len, param);
}

Variant Json::parseJson(const String& json, JsonParseParam& param)
{
	return _Json_Parser<String8, sl_char8>::parseJson(json.getData(), json.getLength(), param);
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


Variant Json::parseJsonFromUtf8TextFile(const String& filePath, JsonParseParam& param)
{
	String json = File::readUtf8Text(filePath);
	return parseJson(json, param);
}

Variant Json::parseJsonFromUtf8TextFile(const String& filePath)
{
	JsonParseParam param;
	return parseJsonFromUtf8TextFile(filePath, param);
}


String Json::toJson(const Variant& var)
{
	return var.toJson();
}

SLIB_NAMESPACE_END
