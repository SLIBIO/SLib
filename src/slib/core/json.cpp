/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/core/json.h"
#include "slib/core/json_std.h"

#include "slib/core/list_std.h"
#include "slib/core/map_std.h"

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
	
	Json::Json(const std::initializer_list< Pair<String, Json> >& pairs): Variant(VariantMap(*(reinterpret_cast<const std::initializer_list< Pair<String, Variant> >*>(&pairs))))
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
	
	Json::Json(char value): Variant(value)
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
	
	Json::Json(const List< Map<String, Variant> >& list): Variant(list)
	{
	}
	
	Json::Json(const AtomicList< Map<String, Variant> >& list): Variant(list)
	{
	}
	
	Json::Json(const List<Json>& list): Variant(*(reinterpret_cast<VariantList const*>(&list)))
	{
	}
	
	Json::Json(const AtomicList<Json>& list): Variant(*(reinterpret_cast<AtomicVariantList const*>(&list)))
	{
	}
	
	Json::Json(const Map<String, Json>& map): Variant(*(reinterpret_cast<VariantMap const*>(&map)))
	{
	}
	
	Json::Json(const AtomicMap<String, Json>& map): Variant(*(reinterpret_cast<AtomicVariantMap const*>(&map)))
	{
	}
	
	Json::Json(const List< Map<String, Json> >& list): Variant(*(reinterpret_cast<VariantMapList const*>(&list)))
	{
	}
	
	Json::Json(const AtomicList< Map<String, Json> >& list): Variant(*(reinterpret_cast<AtomicVariantMapList const*>(&list)))
	{
	}

	Json Json::createList()
	{
		return Variant::createList();
	}
	
	Json Json::createMap()
	{
		return Variant::createMap();
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
	
	Json& Json::operator=(const std::initializer_list< Pair<String, Json> >& pairs)
	{
		return *this = Json(pairs);
	}
	
	Json& Json::operator=(const std::initializer_list<Json>& elements)
	{
		return *this = Json(elements);
	}
	
	Json Json::operator[](sl_size indexForVariantList) const
	{
		return (*(static_cast<const Variant*>(this)))[indexForVariantList];
	}
	
	Json Json::operator[](const String& keyForVariantMap) const
	{
		return (*(static_cast<const Variant*>(this)))[keyForVariantMap];
	}
	
	template <class ST, class CT>
	class _Json_Parser
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
		_Json_Parser();
		
	public:
		void escapeSpaceAndComments();
		
		Json parseJson();

		static Json parseJson(const CT* buf, sl_size len, JsonParseParam& param);
		
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
	Json _Json_Parser<ST, CT>::parseJson()
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
				return Variant::createList();
			}
			VariantList list = VariantList::create();
			while (pos < len) {
				CT ch = buf[pos];
				if (ch == ']' || ch == ',') {
					list.add_NoLock(Variant::null());
				} else {
					Variant item = parseJson();
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
			VariantMap map = VariantMap::createHash();
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
					map.put_NoLock(key, Variant::null());
				} else {
					Variant item = parseJson();
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
	Json _Json_Parser<ST, CT>::parseJson(const CT* buf, sl_size len, JsonParseParam& param)
	{
		param.flagError = sl_false;
		
		_Json_Parser<ST, CT> parser;
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
		return _Json_Parser<String, sl_char8>::parseJson(sz, len, param);
	}

	Json Json::parseJson(const sl_char8* sz, sl_size len)
	{
		JsonParseParam param;
		return parseJson(sz, len, param);
	}

	Json Json::parseJson(const String& json, JsonParseParam& param)
	{
		return _Json_Parser<String, sl_char8>::parseJson(json.getData(), json.getLength(), param);
	}

	Json Json::parseJson(const String& json)
	{
		JsonParseParam param;
		return parseJson(json, param);
	}


	Json Json::parseJson16(const sl_char16* sz, sl_size len, JsonParseParam& param)
	{
		return _Json_Parser<String16, sl_char16>::parseJson(sz, len, param);
	}

	Json Json::parseJson16(const sl_char16* sz, sl_size len)
	{
		JsonParseParam param;
		return Json::parseJson16(sz, len, param);
	}

	Json Json::parseJson16(const String16& json, JsonParseParam& param)
	{
		return _Json_Parser<String16, sl_char16>::parseJson(json.getData(), json.getLength(), param);
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
	
	List<Json> Json::getJsonList() const
	{
		Ref<Referable> obj(getObject());
		if (CList<Json>* p = CastInstance< CList<Json> >(obj._ptr)) {
			return p;
		}
		return sl_null;
	}
	
	void Json::setJsonList(const List<Json>& list)
	{
		setVariantList(*(reinterpret_cast<VariantList const*>(&list)));
	}
	
	sl_bool Json::isJsonMap() const
	{
		return isVariantMap();
	}
	
	Map<String, Json> Json::getJsonMap() const
	{
		Ref<Referable> obj(getObject());
		if (IMap<String, Json>* p = CastInstance< IMap<String, Json> >(obj._ptr)) {
			return p;
		}
		return sl_null;
	}
	
	void Json::setJsonMap(const Map<String, Json>& map)
	{
		setVariantMap(*(reinterpret_cast<VariantMap const*>(&map)));
	}
	
	sl_bool Json::isJsonMapList() const
	{
		return isVariantMapList();
	}
	
	List< Map<String, Json> > Json::getJsonMapList() const
	{
		Ref<Referable> obj(getObject());
		if (CList< Map<String, Json> >* p = CastInstance< CList< Map<String, Json> > >(obj._ptr)) {
			return p;
		}
		return sl_null;
	}
	
	void Json::setJsonMapList(const List< Map<String, Json> >& list)
	{
		setVariantMapList(*(reinterpret_cast<VariantMapList const*>(&list)));
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

	void Json::get(Json& _out) const
	{
		_out = *this;
	}
	
	void Json::set(const Json& _in)
	{
		*this = _in;
	}

	void Json::get(Variant& _out) const
	{
		_out = *this;
	}
	
	void Json::set(const Variant& _in)
	{
		*this = _in;
	}
	
	void Json::get(AtomicVariant& _out) const
	{
		_out = *this;
	}
		
	void Json::set(const AtomicVariant& _in)
	{
		*this = _in;
	}
	
	void Json::get(char& _out) const
	{
		_out = (char)(getInt32());
	}
	
	void Json::get(char& _out, char def) const
	{
		_out = (char)(getInt32((sl_int32)def));
	}
	
	void Json::set(char _in)
	{
		setInt32((sl_int32)_in);
	}
	
	void Json::get(unsigned char& _out) const
	{
		_out = (unsigned char)(getUint32());
	}
	
	void Json::get(unsigned char& _out, unsigned char def) const
	{
		_out = (unsigned char)(getUint32((sl_uint32)def));
	}
	
	void Json::set(unsigned char _in)
	{
		setUint32((sl_uint32)_in);
	}
	
	void Json::get(short& _out) const
	{
		_out = (short)(getInt32());
	}
	
	void Json::get(short& _out, short def) const
	{
		_out = (short)(getInt32((sl_int32)def));
	}
	
	void Json::set(short _in)
	{
		setInt32((sl_int32)_in);
	}
	
	void Json::get(unsigned short& _out) const
	{
		_out = (unsigned short)(getUint32());
	}
	
	void Json::get(unsigned short& _out, unsigned short def) const
	{
		_out = (unsigned short)(getUint32((sl_uint32)def));
	}
	
	void Json::set(unsigned short _in)
	{
		setUint32((sl_uint32)_in);
	}
	
	void Json::get(int& _out) const
	{
		_out = (int)(getInt32());
	}
	
	void Json::get(int& _out, int def) const
	{
		_out = (int)(getInt32((sl_int32)def));
	}
	
	void Json::set(int _in)
	{
		setInt32((sl_int32)_in);
	}
	
	void Json::get(unsigned int& _out) const
	{
		_out = (unsigned int)(getUint32());
	}
	
	void Json::get(unsigned int& _out, unsigned int def) const
	{
		_out = (unsigned int)(getUint32((sl_uint32)def));
	}
	
	void Json::set(unsigned int _in)
	{
		setUint32((sl_uint32)_in);
	}
	
	void Json::get(long& _out) const
	{
		_out = (long)(getInt32());
	}
	
	void Json::get(long& _out, long def) const
	{
		_out = (long)(getInt32((sl_int32)def));
	}
	
	void Json::set(long _in)
	{
		setInt32((sl_int32)_in);
	}
	
	void Json::get(unsigned long& _out) const
	{
		_out = (unsigned long)(getUint32());
	}
	
	void Json::get(unsigned long& _out, unsigned long def) const
	{
		_out = (unsigned long)(getUint32((sl_uint32)def));
	}
	
	void Json::set(unsigned long _in)
	{
		setUint32((sl_uint32)_in);
	}
	
	void Json::get(sl_int64& _out) const
	{
		_out = getInt64();
	}
	
	void Json::get(sl_int64& _out, sl_int64 def) const
	{
		_out = getInt64(def);
	}
	
	void Json::set(sl_int64 _in)
	{
		setInt64(_in);
	}
	
	void Json::get(sl_uint64& _out) const
	{
		_out = getUint64();
	}
	
	void Json::get(sl_uint64& _out, sl_uint64 def) const
	{
		_out = getUint64(def);
	}
	
	void Json::set(sl_uint64 _in)
	{
		setUint64(_in);
	}
	
	void Json::get(float& _out) const
	{
		_out = getFloat();
	}
	
	void Json::get(float& _out, float def) const
	{
		_out = getFloat(def);
	}
	
	void Json::set(float _in)
	{
		setFloat(_in);
	}
	
	void Json::get(double& _out) const
	{
		_out = getDouble();
	}
	
	void Json::get(double& _out, double def) const
	{
		_out = getDouble(def);
	}
	
	void Json::set(double _in)
	{
		setDouble(_in);
	}
	
	void Json::get(bool& _out) const
	{
		_out = getBoolean();
	}
	
	void Json::get(bool& _out, bool def) const
	{
		_out = getBoolean(def);
	}
	
	void Json::set(bool _in)
	{
		setBoolean(_in);
	}
	
	void Json::get(String& _out) const
	{
		_out = getString();
	}
	
	void Json::get(String& _out, const String& def) const
	{
		_out = getString(def);
	}
	
	void Json::set(const String& _in)
	{
		setString(_in);
	}
	
	void Json::get(AtomicString& _out) const
	{
		_out = getString();
	}
	
	void Json::get(AtomicString& _out, const String& def) const
	{
		_out = getString(def);
	}
	
	void Json::set(const AtomicString& _in)
	{
		setString(_in);
	}
	
	void Json::get(String16& _out) const
	{
		_out = getString16();
	}
	
	void Json::get(String16& _out, const String16& def) const
	{
		_out = getString16(def);
	}
	
	void Json::set(const String16& _in)
	{
		setString(_in);
	}
	
	void Json::get(AtomicString16& _out) const
	{
		_out = getString16();
	}
	
	void Json::get(AtomicString16& _out, const String16& def) const
	{
		_out = getString16(def);
	}
	
	void Json::set(const AtomicString16& _in)
	{
		setString(_in);
	}
	
	void Json::set(const sl_char8* _in)
	{
		setString(_in);
	}
	
	void Json::set(const sl_char16* _in)
	{
		setString(_in);
	}
	
	void Json::get(Time& _out) const
	{
		_out = getTime();
	}
	
	void Json::get(Time& _out, const Time& def) const
	{
		_out = getTime(def);
	}
	
	void Json::set(const Time& _in)
	{
		setTime(_in);
	}
	
	void Json::get(VariantList& _out) const
	{
		_out = getVariantList();
	}
	
	void Json::set(const VariantList& _in)
	{
		setVariantList(_in);
	}
	
	void Json::get(AtomicVariantList& _out) const
	{
		_out = getVariantList();
	}
	
	void Json::set(const AtomicVariantList& _in)
	{
		setVariantList(_in);
	}
	
	void Json::get(VariantMap& _out) const
	{
		_out = getVariantMap();
	}
	
	void Json::set(const VariantMap& _in)
	{
		setVariantMap(_in);
	}
	
	void Json::get(AtomicVariantMap& _out) const
	{
		_out = getVariantMap();
	}
	
	void Json::set(const AtomicVariantMap& _in)
	{
		setVariantMap(_in);
	}
	
	void Json::get(VariantMapList& _out) const
	{
		_out = getVariantMapList();
	}
	
	void Json::set(const VariantMapList& _in)
	{
		setVariantMapList(_in);
	}
	
	void Json::get(AtomicVariantMapList& _out) const
	{
		_out = getVariantMapList();
	}
	
	void Json::set(const AtomicVariantMapList& _in)
	{
		setVariantMapList(_in);
	}

	void Json::get(JsonList& _out) const
	{
		_out = getJsonList();
	}
	
	void Json::set(const JsonList& _in)
	{
		setJsonList(_in);
	}
	
	void Json::get(AtomicJsonList& _out) const
	{
		_out = getJsonList();
	}
	
	void Json::set(const AtomicJsonList& _in)
	{
		setJsonList(_in);
	}
	
	void Json::get(JsonMap& _out) const
	{
		_out = getJsonMap();
	}
	
	void Json::set(const JsonMap& _in)
	{
		setJsonMap(_in);
	}
	
	void Json::get(AtomicJsonMap& _out) const
	{
		_out = getJsonMap();
	}
	
	void Json::set(const AtomicJsonMap& _in)
	{
		setJsonMap(_in);
	}
	
	void Json::get(JsonMapList& _out) const
	{
		_out = getJsonMapList();
	}
	
	void Json::set(const JsonMapList& _in)
	{
		setJsonMapList(_in);
	}
	
	void Json::get(AtomicJsonMapList& _out) const
	{
		_out = getJsonMapList();
	}
	
	void Json::set(const AtomicJsonMapList& _in)
	{
		setJsonMapList(_in);
	}
	
}
