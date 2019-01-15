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

#define SLIB_SUPPORT_STD_TYPES

#include "slib/core/json.h"

#include "slib/core/list.h"
#include "slib/core/map.h"

#include "slib/core/file.h"
#include "slib/core/log.h"

namespace slib
{
	
	JsonParseParam::JsonParseParam()
	{
		flagLogError = sl_true;
		flagSupportComments = sl_true;
		
		flagError = sl_false;
		errorLine = 0;
		errorColumn = 0;
		errorPosition = 0;
	}

	JsonParseParam::~JsonParseParam()
	{
	}

	String JsonParseParam::getErrorText()
	{
		if (flagError) {
			return "(" + String::fromSize(errorLine) + ":" + String::fromSize(errorColumn) + ") " + errorMessage;
		}
		return sl_null;
	}
	
	
	Json::Json()
	{
	}
	
	Json::Json(const Json& other) : Variant(other)
	{
	}
	
	Json::Json(Json&& other) : Variant(Move(*(static_cast<Variant*>(&other))))
	{
	}
	
	Json::Json(const Variant& variant) : Variant(variant)
	{
	}
	
	Json::Json(const AtomicVariant& variant) : Variant(variant)
	{
	}
	
	Json::Json(const std::initializer_list<JsonItem>& pairs): Variant(VariantHashMap(*(reinterpret_cast<const std::initializer_list< Pair<String, Variant> >*>(&pairs))))
	{
	}
	
	Json::Json(const std::initializer_list<Json>& elements): Variant(VariantList(*(reinterpret_cast<const std::initializer_list<Variant>*>(&elements))))
	{
	}
	
	Json::~Json()
	{
	}

	Json::Json(sl_null_t)
	{
	}
	
	Json::Json(signed char value): Variant(value)
	{
	}
	
	Json::Json(unsigned char value): Variant(value)
	{
	}
	
	Json::Json(short value): Variant(value)
	{
	}
	
	Json::Json(unsigned short value): Variant(value)
	{
	}
	
	Json::Json(int value): Variant(value)
	{
	}
	
	Json::Json(unsigned int value): Variant(value)
	{
	}
	
	Json::Json(long value): Variant(value)
	{
	}
	
	Json::Json(unsigned long value): Variant(value)
	{
	}
	
	Json::Json(sl_int64 value): Variant(value)
	{
	}
	
	Json::Json(sl_uint64 value): Variant(value)
	{
	}
	
	Json::Json(float value): Variant(value)
	{
	}
	
	Json::Json(double value): Variant(value)
	{
	}
	
	Json::Json(sl_bool value): Variant(value)
	{
	}
	
	Json::Json(const String& value): Variant(value)
	{
	}
	
	Json::Json(const String16& value): Variant(value)
	{
	}
	
	Json::Json(const AtomicString& value): Variant(value)
	{
	}
	
	Json::Json(const AtomicString16& value): Variant(value)
	{
	}
	
	Json::Json(const sl_char8* sz8): Variant(sz8)
	{
	}
	
	Json::Json(const sl_char16* sz16): Variant(sz16)
	{
	}
	
	Json::Json(const std::string& str): Variant(str)
	{
	}
	
	Json::Json(const std::u16string& str): Variant(str)
	{
	}

	Json::Json(const Time& value): Variant(value)
	{
	}
	
	Json::Json(const List<Variant>& list): Variant(list)
	{
	}
	
	Json::Json(const AtomicList<Variant>& list): Variant(list)
	{
	}
	
	Json::Json(const Map<String, Variant>& map): Variant(map)
	{
	}
	
	Json::Json(const AtomicMap<String, Variant>& map): Variant(map)
	{
	}
	
	Json::Json(const HashMap<String, Variant>& map): Variant(map)
	{
	}
	
	Json::Json(const AtomicHashMap<String, Variant>& map): Variant(map)
	{
	}
	
	Json::Json(const List< Map<String, Variant> >& list): Variant(list)
	{
	}
	
	Json::Json(const AtomicList< Map<String, Variant> >& list): Variant(list)
	{
	}
	
	Json::Json(const List< HashMap<String, Variant> >& list): Variant(list)
	{
	}
	
	Json::Json(const AtomicList< HashMap<String, Variant> >& list): Variant(list)
	{
	}
	
	Json::Json(const JsonList& list): Variant(*(reinterpret_cast<VariantList const*>(&list)))
	{
	}
	
	Json::Json(const AtomicJsonList& list): Variant(*(reinterpret_cast<AtomicVariantList const*>(&list)))
	{
	}
	
	Json::Json(const JsonMap& map): Variant(*(reinterpret_cast<VariantHashMap const*>(&map)))
	{
	}
	
	Json::Json(const AtomicJsonMap& map): Variant(*(reinterpret_cast<AtomicVariantHashMap const*>(&map)))
	{
	}
	
	Json::Json(const JsonMapList& list): Variant(*(reinterpret_cast<VariantHashMapList const*>(&list)))
	{
	}
	
	Json::Json(const AtomicJsonMapList& list): Variant(*(reinterpret_cast<AtomicVariantHashMapList const*>(&list)))
	{
	}

	Json Json::createList()
	{
		return Variant::createList();
	}
	
	Json Json::createMap()
	{
		return Variant::createHashMap();
	}

	Json& Json::operator=(const Json& json)
	{
		*(static_cast<Variant*>(this)) = json;
		return *this;
	}
	
	Json& Json::operator=(Json&& json)
	{
		*(static_cast<Variant*>(this)) = Move(*(static_cast<Variant*>(&json)));
		return *this;
	}
	
	Json& Json::operator=(const Variant& variant)
	{
		*(static_cast<Variant*>(this)) = variant;
		return *this;
	}
	
	Json& Json::operator=(const AtomicVariant& variant)
	{
		*(static_cast<Variant*>(this)) = variant;
		return *this;
	}
	
	Json& Json::operator=(sl_null_t)
	{
		setNull();
		return *this;
	}
	
	Json& Json::operator=(const std::initializer_list<JsonItem>& pairs)
	{
		return *this = Json(pairs);
	}
	
	Json& Json::operator=(const std::initializer_list<Json>& elements)
	{
		return *this = Json(elements);
	}
	
	template <class ST, class CT>
	class _priv_Json_Parser
	{
	public:
		const CT* buf = sl_null;
		sl_size len = 0;
		sl_bool flagSupportComments = sl_false;
		
		sl_size pos = 0;
		
		sl_bool flagError = sl_false;
		String errorMessage;
		
		ST strNull;
		ST strTrue;
		ST strFalse;
		
	public:
		_priv_Json_Parser();
		
	public:
		void escapeSpaceAndComments();
		
		Json parseJson();

		static Json parseJson(const CT* buf, sl_size len, JsonParseParam& param);
		
	};

	template <>
	_priv_Json_Parser<String, sl_char8>::_priv_Json_Parser()
	{
		SLIB_STATIC_STRING(_null, "null");
		strNull = _null;
		SLIB_STATIC_STRING(_true, "true");
		strTrue = _true;
		SLIB_STATIC_STRING(_false, "false");
		strFalse = _false;
	}

	template <>
	_priv_Json_Parser<String16, sl_char16>::_priv_Json_Parser()
	{
		SLIB_STATIC_STRING16_BY_ARRAY(_null, 'n', 'u', 'l', 'l');
		strNull = _null;
		SLIB_STATIC_STRING16_BY_ARRAY(_true, 't', 'r', 'u', 'e');
		strTrue = _true;
		SLIB_STATIC_STRING16_BY_ARRAY(_false, 'f', 'a', 'l', 's', 'e');
		strFalse = _false;
	}

	template <class ST, class CT>
	void _priv_Json_Parser<ST, CT>::escapeSpaceAndComments()
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
						if (pos + 2 <= len && ch == '/') {
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
	Json _priv_Json_Parser<ST, CT>::parseJson()
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
				return Json::createList();
			}
			JsonList list = JsonList::create();
			while (pos < len) {
				CT ch = buf[pos];
				if (ch == ']' || ch == ',') {
					list.add_NoLock(Json::null());
				} else {
					Json item = parseJson();
					if (flagError) {
						return sl_null;
					}
					list.add_NoLock(item);
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
			JsonMap map = JsonMap::create();
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
					map.put_NoLock(key, Json::null());
				} else {
					Json item = parseJson();
					if (flagError) {
						return sl_null;
					}
					map.put_NoLock(key, item);
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
				return Json::fromBoolean(sl_true);
			}
			if (str == strFalse) {
				return Json::fromBoolean(sl_false);
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
	Json _priv_Json_Parser<ST, CT>::parseJson(const CT* buf, sl_size len, JsonParseParam& param)
	{
		param.flagError = sl_false;
		
		_priv_Json_Parser<ST, CT> parser;
		parser.buf = buf;
		parser.len = len;
		parser.flagSupportComments = param.flagSupportComments;
		
		parser.pos = 0;
		parser.flagError = sl_false;
		
		Json var = parser.parseJson();
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


	Json Json::parseJson(const sl_char8* sz, sl_size len, JsonParseParam& param)
	{
		return _priv_Json_Parser<String, sl_char8>::parseJson(sz, len, param);
	}

	Json Json::parseJson(const sl_char8* sz, sl_size len)
	{
		JsonParseParam param;
		return parseJson(sz, len, param);
	}

	Json Json::parseJson(const String& json, JsonParseParam& param)
	{
		return _priv_Json_Parser<String, sl_char8>::parseJson(json.getData(), json.getLength(), param);
	}

	Json Json::parseJson(const String& json)
	{
		JsonParseParam param;
		return parseJson(json, param);
	}


	Json Json::parseJson16(const sl_char16* sz, sl_size len, JsonParseParam& param)
	{
		return _priv_Json_Parser<String16, sl_char16>::parseJson(sz, len, param);
	}

	Json Json::parseJson16(const sl_char16* sz, sl_size len)
	{
		JsonParseParam param;
		return Json::parseJson16(sz, len, param);
	}

	Json Json::parseJson16(const String16& json, JsonParseParam& param)
	{
		return _priv_Json_Parser<String16, sl_char16>::parseJson(json.getData(), json.getLength(), param);
	}

	Json Json::parseJson16(const String16& json)
	{
		JsonParseParam param;
		return Json::parseJson16(json, param);
	}


	Json Json::parseJsonFromTextFile(const String& filePath, JsonParseParam& param)
	{
		String16 json = File::readAllText16(filePath);
		return parseJson16(json, param);
	}

	Json Json::parseJsonFromTextFile(const String& filePath)
	{
		JsonParseParam param;
		return parseJsonFromTextFile(filePath, param);
	}

	Json Json::parseJsonUtf8(const Memory& mem, JsonParseParam& param)
	{
		return parseJson((sl_char8*)(mem.getData()), mem.getSize(), param);
	}

	Json Json::parseJsonUtf8(const Memory& mem)
	{
		return parseJson((sl_char8*)(mem.getData()), mem.getSize());
	}

	Json Json::parseJson16Utf8(const Memory& mem, JsonParseParam& param)
	{
		return parseJson(String16((sl_char8*)(mem.getData()), mem.getSize()), param);
	}

	Json Json::parseJson16Utf8(const Memory& mem)
	{
		return parseJson(String16((sl_char8*)(mem.getData()), mem.getSize()));
	}


	String Json::toString() const
	{
		return Variant::toString();
	}

	sl_bool Json::isJsonList() const
	{
		return isVariantList();
	}
	
	JsonList Json::getJsonList() const
	{
		Ref<Referable> obj(getObject());
		if (CList<Json>* p = CastInstance< CList<Json> >(obj._ptr)) {
			return p;
		}
		return sl_null;
	}
	
	void Json::setJsonList(const JsonList& list)
	{
		setVariantList(*(reinterpret_cast<VariantList const*>(&list)));
	}
	
	sl_bool Json::isJsonMap() const
	{
		return isVariantHashMap();
	}
	
	JsonMap Json::getJsonMap() const
	{
		Ref<Referable> obj(getObject());
		if (CHashMap<String, Json>* p = CastInstance< CHashMap<String, Json> >(obj._ptr)) {
			return p;
		}
		return sl_null;
	}
	
	void Json::setJsonMap(const JsonMap& map)
	{
		setVariantHashMap(*(reinterpret_cast<VariantHashMap const*>(&map)));
	}
	
	sl_bool Json::isJsonMapList() const
	{
		return isVariantHashMapList();
	}
	
	JsonMapList Json::getJsonMapList() const
	{
		Ref<Referable> obj(getObject());
		if (CList< HashMap<String, Json> >* p = CastInstance< CList< HashMap<String, Json> > >(obj._ptr)) {
			return p;
		}
		return sl_null;
	}
	
	void Json::setJsonMapList(const JsonMapList& list)
	{
		setVariantHashMapList(*(reinterpret_cast<VariantHashMapList const*>(&list)));
	}

	
	Json Json::getElement(sl_size index) const
	{
		return Variant::getElement(index);
	}
	
	sl_bool Json::setElement(sl_size index, const Json& value)
	{
		return Variant::setElement(index, value);
	}
	
	sl_bool Json::addElement(const Json& value)
	{
		return Variant::addElement(value);
	}
	
	Json Json::getItem(const String& key) const
	{
		return Variant::getItem(key);
	}
	
	sl_bool Json::putItem(const String& key, const Json& value)
	{
		return Variant::putItem(key, value);
	}
	
	sl_bool Json::removeItem(const String& key)
	{
		return Variant::removeItem(key);
	}
	
	Json Json::operator[](sl_size list_index) const
	{
		return (*(static_cast<const Variant*>(this)))[list_index];
	}
	
	Json Json::operator[](const String& map_key) const
	{
		return (*(static_cast<const Variant*>(this)))[map_key];
	}
	
	
	void FromJson(const Json& json, Json& _out)
	{
		_out = json;
	}
	
	void ToJson(Json& json, const Json& _in)
	{
		json = _in;
	}

	void FromJson(const Json& json, Variant& _out)
	{
		_out = json;
	}
	
	void ToJson(Json& json, const Variant& _in)
	{
		json = _in;
	}
	
	void FromJson(const Json& json, AtomicVariant& _out)
	{
		_out = json;
	}
		
	void ToJson(Json& json, const AtomicVariant& _in)
	{
		json = _in;
	}
	
	void FromJson(const Json& json, signed char& _out)
	{
		_out = (char)(json.getInt32());
	}
	
	void FromJson(const Json& json, signed char& _out, signed char def)
	{
		_out = (char)(json.getInt32((sl_int32)def));
	}
	
	void ToJson(Json& json, signed char _in)
	{
		json.setInt32((sl_int32)_in);
	}
	
	void FromJson(const Json& json, unsigned char& _out)
	{
		_out = (unsigned char)(json.getUint32());
	}
	
	void FromJson(const Json& json, unsigned char& _out, unsigned char def)
	{
		_out = (unsigned char)(json.getUint32((sl_uint32)def));
	}
	
	void ToJson(Json& json, unsigned char _in)
	{
		json.setUint32((sl_uint32)_in);
	}
	
	void FromJson(const Json& json, short& _out)
	{
		_out = (short)(json.getInt32());
	}
	
	void FromJson(const Json& json, short& _out, short def)
	{
		_out = (short)(json.getInt32((sl_int32)def));
	}
	
	void ToJson(Json& json, short _in)
	{
		json.setInt32((sl_int32)_in);
	}
	
	void FromJson(const Json& json, unsigned short& _out)
	{
		_out = (unsigned short)(json.getUint32());
	}
	
	void FromJson(const Json& json, unsigned short& _out, unsigned short def)
	{
		_out = (unsigned short)(json.getUint32((sl_uint32)def));
	}
	
	void ToJson(Json& json, unsigned short _in)
	{
		json.setUint32((sl_uint32)_in);
	}
	
	void FromJson(const Json& json, int& _out)
	{
		_out = (int)(json.getInt32());
	}
	
	void FromJson(const Json& json, int& _out, int def)
	{
		_out = (int)(json.getInt32((sl_int32)def));
	}
	
	void ToJson(Json& json, int _in)
	{
		json.setInt32((sl_int32)_in);
	}
	
	void FromJson(const Json& json, unsigned int& _out)
	{
		_out = (unsigned int)(json.getUint32());
	}
	
	void FromJson(const Json& json, unsigned int& _out, unsigned int def)
	{
		_out = (unsigned int)(json.getUint32((sl_uint32)def));
	}
	
	void ToJson(Json& json, unsigned int _in)
	{
		json.setUint32((sl_uint32)_in);
	}
	
	void FromJson(const Json& json, long& _out)
	{
		_out = (long)(json.getInt32());
	}
	
	void FromJson(const Json& json, long& _out, long def)
	{
		_out = (long)(json.getInt32((sl_int32)def));
	}
	
	void ToJson(Json& json, long _in)
	{
		json.setInt32((sl_int32)_in);
	}
	
	void FromJson(const Json& json, unsigned long& _out)
	{
		_out = (unsigned long)(json.getUint32());
	}
	
	void FromJson(const Json& json, unsigned long& _out, unsigned long def)
	{
		_out = (unsigned long)(json.getUint32((sl_uint32)def));
	}
	
	void ToJson(Json& json, unsigned long _in)
	{
		json.setUint32((sl_uint32)_in);
	}
	
	void FromJson(const Json& json, sl_int64& _out)
	{
		_out = json.getInt64();
	}
	
	void FromJson(const Json& json, sl_int64& _out, sl_int64 def)
	{
		_out = json.getInt64(def);
	}
	
	void ToJson(Json& json, sl_int64 _in)
	{
		json.setInt64(_in);
	}
	
	void FromJson(const Json& json, sl_uint64& _out)
	{
		_out = json.getUint64();
	}
	
	void FromJson(const Json& json, sl_uint64& _out, sl_uint64 def)
	{
		_out = json.getUint64(def);
	}
	
	void ToJson(Json& json, sl_uint64 _in)
	{
		json.setUint64(_in);
	}
	
	void FromJson(const Json& json, float& _out)
	{
		_out = json.getFloat();
	}
	
	void FromJson(const Json& json, float& _out, float def)
	{
		_out = json.getFloat(def);
	}
	
	void ToJson(Json& json, float _in)
	{
		json.setFloat(_in);
	}
	
	void FromJson(const Json& json, double& _out)
	{
		_out = json.getDouble();
	}
	
	void FromJson(const Json& json, double& _out, double def)
	{
		_out = json.getDouble(def);
	}
	
	void ToJson(Json& json, double _in)
	{
		json.setDouble(_in);
	}
	
	void FromJson(const Json& json, bool& _out)
	{
		_out = json.getBoolean();
	}
	
	void FromJson(const Json& json, bool& _out, bool def)
	{
		_out = json.getBoolean(def);
	}
	
	void ToJson(Json& json, bool _in)
	{
		json.setBoolean(_in);
	}
	
	void FromJson(const Json& json, String& _out)
	{
		_out = json.getString();
	}
	
	void FromJson(const Json& json, String& _out, const String& def)
	{
		_out = json.getString(def);
	}
	
	void ToJson(Json& json, const String& _in)
	{
		json.setString(_in);
	}
	
	void FromJson(const Json& json, AtomicString& _out)
	{
		_out = json.getString();
	}
	
	void FromJson(const Json& json, AtomicString& _out, const String& def)
	{
		_out = json.getString(def);
	}
	
	void ToJson(Json& json, const AtomicString& _in)
	{
		json.setString(_in);
	}
	
	void FromJson(const Json& json, String16& _out)
	{
		_out = json.getString16();
	}
	
	void FromJson(const Json& json, String16& _out, const String16& def)
	{
		_out = json.getString16(def);
	}
	
	void ToJson(Json& json, const String16& _in)
	{
		json.setString(_in);
	}
	
	void FromJson(const Json& json, AtomicString16& _out)
	{
		_out = json.getString16();
	}
	
	void FromJson(const Json& json, AtomicString16& _out, const String16& def)
	{
		_out = json.getString16(def);
	}
	
	void ToJson(Json& json, const AtomicString16& _in)
	{
		json.setString(_in);
	}
	
	void ToJson(Json& json, const sl_char8* _in)
	{
		json.setString(_in);
	}
	
	void ToJson(Json& json, const sl_char16* _in)
	{
		json.setString(_in);
	}
	
	void FromJson(const Json& json, Time& _out)
	{
		_out = json.getTime();
	}
	
	void FromJson(const Json& json, Time& _out, const Time& def)
	{
		_out = json.getTime(def);
	}
	
	void ToJson(Json& json, const Time& _in)
	{
		json.setTime(_in);
	}
	
	void FromJson(const Json& json, VariantList& _out)
	{
		_out = json.getVariantList();
	}
	
	void ToJson(Json& json, const VariantList& _in)
	{
		json.setVariantList(_in);
	}
	
	void FromJson(const Json& json, AtomicVariantList& _out)
	{
		_out = json.getVariantList();
	}
	
	void ToJson(Json& json, const AtomicVariantList& _in)
	{
		json.setVariantList(_in);
	}
	
	void FromJson(const Json& json, VariantMap& _out)
	{
		_out = json.getVariantMap();
	}
	
	void ToJson(Json& json, const VariantMap& _in)
	{
		json.setVariantMap(_in);
	}
	
	void FromJson(const Json& json, AtomicVariantMap& _out)
	{
		_out = json.getVariantMap();
	}
	
	void ToJson(Json& json, const AtomicVariantMap& _in)
	{
		json.setVariantMap(_in);
	}
	
	void FromJson(const Json& json, VariantHashMap& _out)
	{
		_out = json.getVariantHashMap();
	}
	
	void ToJson(Json& json, const VariantHashMap& _in)
	{
		json.setVariantHashMap(_in);
	}
	
	void FromJson(const Json& json, AtomicVariantHashMap& _out)
	{
		_out = json.getVariantHashMap();
	}
	
	void ToJson(Json& json, const AtomicVariantHashMap& _in)
	{
		json.setVariantHashMap(_in);
	}
	
	void FromJson(const Json& json, VariantMapList& _out)
	{
		_out = json.getVariantMapList();
	}
	
	void ToJson(Json& json, const VariantMapList& _in)
	{
		json.setVariantMapList(_in);
	}
	
	void FromJson(const Json& json, AtomicVariantMapList& _out)
	{
		_out = json.getVariantMapList();
	}
	
	void ToJson(Json& json, const AtomicVariantMapList& _in)
	{
		json.setVariantMapList(_in);
	}

	void FromJson(const Json& json, VariantHashMapList& _out)
	{
		_out = json.getVariantHashMapList();
	}
	
	void ToJson(Json& json, const VariantHashMapList& _in)
	{
		json.setVariantHashMapList(_in);
	}
	
	void FromJson(const Json& json, AtomicVariantHashMapList& _out)
	{
		_out = json.getVariantHashMapList();
	}
	
	void ToJson(Json& json, const AtomicVariantHashMapList& _in)
	{
		json.setVariantHashMapList(_in);
	}
	
	void FromJson(const Json& json, JsonList& _out)
	{
		_out = json.getJsonList();
	}
	
	void ToJson(Json& json, const JsonList& _in)
	{
		json.setJsonList(_in);
	}
	
	void FromJson(const Json& json, AtomicJsonList& _out)
	{
		_out = json.getJsonList();
	}
	
	void ToJson(Json& json, const AtomicJsonList& _in)
	{
		json.setJsonList(_in);
	}
	
	void FromJson(const Json& json, JsonMap& _out)
	{
		_out = json.getJsonMap();
	}
	
	void ToJson(Json& json, const JsonMap& _in)
	{
		json.setJsonMap(_in);
	}
	
	void FromJson(const Json& json, AtomicJsonMap& _out)
	{
		_out = json.getJsonMap();
	}
	
	void ToJson(Json& json, const AtomicJsonMap& _in)
	{
		json.setJsonMap(_in);
	}
	
	void FromJson(const Json& json, JsonMapList& _out)
	{
		_out = json.getJsonMapList();
	}
	
	void ToJson(Json& json, const JsonMapList& _in)
	{
		json.setJsonMapList(_in);
	}
	
	void FromJson(const Json& json, AtomicJsonMapList& _out)
	{
		_out = json.getJsonMapList();
	}
	
	void ToJson(Json& json, const AtomicJsonMapList& _in)
	{
		json.setJsonMapList(_in);
	}
	
}
