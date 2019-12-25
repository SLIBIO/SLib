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

#ifndef CHECKHEADER_SLIB_CORE_JSON
#define CHECKHEADER_SLIB_CORE_JSON

#include "definition.h"

#include "variant.h"
#include "cast.h"

#include "../math/bigint.h"

#ifdef SLIB_SUPPORT_STD_TYPES
#include <initializer_list>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#endif

namespace slib
{
	
	class SLIB_EXPORT JsonParseParam
	{
	public:
		// in
		sl_bool flagSupportComments;
		// in
		sl_bool flagLogError;

		// out
		sl_bool flagError;
		// out
		sl_size errorPosition;
		// out
		sl_size errorLine;
		// out
		sl_size errorColumn;
		// out
		String errorMessage;

	public:
		JsonParseParam();

		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(JsonParseParam)

	public:
		String getErrorText();

	};
	
	class Json;
	
	typedef List<Json> JsonList;
	typedef AtomicList<Json> AtomicJsonList;
	typedef HashMap<String, Json> JsonMap;
	typedef AtomicHashMap<String, Json> AtomicJsonMap;
	typedef List< HashMap<String, Json> > JsonMapList;
	typedef AtomicList< HashMap<String, Json> > AtomicJsonMapList;
	typedef Pair<String, Json> JsonItem;
	
	class SLIB_EXPORT Json : public Variant
	{
	public:
		Json();
		
		Json(const Json& other);
		
		Json(Json&& other);
		
		Json(const Variant& variant);
		
		Json(const AtomicVariant& variant);
		
		~Json();
		
	public:
		Json(sl_null_t);
		
		Json(signed char value);
		
		Json(unsigned char value);
		
		Json(short value);
		
		Json(unsigned short value);
		
		Json(int value);
		
		Json(unsigned int value);
		
		Json(long value);
		
		Json(unsigned long value);
		
		Json(sl_int64 value);
		
		Json(sl_uint64 value);
		
		Json(float value);
		
		Json(double value);
		
		Json(sl_bool value);
		
		Json(const String& value);
		
		Json(const String16& value);
		
		Json(const AtomicString& value);
		
		Json(const AtomicString16& value);
		
		Json(const sl_char8* sz8);
		
		Json(const sl_char16* sz16);
		
		Json(const StringParam& param);
		
#ifdef SLIB_SUPPORT_STD_TYPES
		Json(const std::string& value);

		Json(const std::u16string& value);
#endif
		
		Json(const Time& value);
		
		template <class T>
		Json(const Nullable<T>& value);
		
		template <class T>
		Json(const Ref<T>& ref);
		
		template <class T>
		Json(const AtomicRef<T>& ref);
		
		template <class T>
		Json(const WeakRef<T>& weak);
		
		template <class T>
		Json(const AtomicWeakRef<T>& weak);
		
		Json(const List<Variant>& list);
		
		Json(const AtomicList<Variant>& list);
		
		Json(const Map<String, Variant>& map);
		
		Json(const AtomicMap<String, Variant>& map);
		
		Json(const HashMap<String, Variant>& map);
		
		Json(const AtomicHashMap<String, Variant>& map);
		
		Json(const List< Map<String, Variant> >& list);
		
		Json(const AtomicList< Map<String, Variant> >& list);
		
		Json(const List< HashMap<String, Variant> >& list);
		
		Json(const AtomicList< HashMap<String, Variant> >& list);
		
		Json(const JsonList& list);
		
		Json(const AtomicJsonList& list);
		
		Json(const JsonMap& map);
		
		Json(const AtomicJsonMap& map);
		
		Json(const JsonMapList& list);
		
		Json(const AtomicJsonMapList& list);
		
		template <class T>
		Json(const List<T>& list);
		
		template <class T>
		Json(const AtomicList<T>& list);
		
		template <class T>
		Json(const ListParam<T>& list);
		
		template <class KT, class VT, class KEY_COMPARE>
		Json(const Map<KT, VT, KEY_COMPARE>& map);
		
		template <class KT, class VT, class KEY_COMPARE>
		Json(const AtomicMap<KT, VT, KEY_COMPARE>& map);
		
		template <class KT, class VT, class HASH, class KEY_COMPARE>
		Json(const HashMap<KT, VT, HASH, KEY_COMPARE>& map);
		
		template <class KT, class VT, class HASH, class KEY_COMPARE>
		Json(const AtomicHashMap<KT, VT, HASH, KEY_COMPARE>& map);
		
#ifdef SLIB_SUPPORT_STD_TYPES
		Json(const std::initializer_list<JsonItem>& pairs);
		
		Json(const std::initializer_list<Json>& elements);
#endif

	public:
		static const Json& undefined();

		static const Json& null();
		
		static Json createList();
		
		static Json createMap();
		
	public:
		Json& operator=(const Json& json);
		
		Json& operator=(Json&& json);
		
		Json& operator=(const Variant& variant);
		
		Json& operator=(const AtomicVariant& variant);
		
		Json& operator=(sl_null_t);
		
#ifdef SLIB_SUPPORT_STD_TYPES
		Json& operator=(const std::initializer_list<JsonItem>& pairs);
		
		Json& operator=(const std::initializer_list<Json>& elements);
#endif
		
	public:
		static Json parseJson(const sl_char8* sz, sl_size len, JsonParseParam& param);

		static Json parseJson(const sl_char8* sz, sl_size len);

		static Json parseJson(const String& json, JsonParseParam& param);

		static Json parseJson(const String& json);

		static Json parseJson16(const sl_char16* sz, sl_size len, JsonParseParam& param);

		static Json parseJson16(const sl_char16* sz, sl_size len);

		static Json parseJson16(const String16& json, JsonParseParam& param);

		static Json parseJson16(const String16& json);

		static Json parseJsonFromTextFile(const String& filePath, JsonParseParam& param);

		static Json parseJsonFromTextFile(const String& filePath);

		static Json parseJsonUtf8(const Memory& mem, JsonParseParam& param);

		static Json parseJsonUtf8(const Memory& mem);

		static Json parseJson16Utf8(const Memory& mem, JsonParseParam& param);

		static Json parseJson16Utf8(const Memory& mem);
		
	public:
		sl_bool isJsonList() const;
		
		JsonList getJsonList() const;
		
		void setJsonList(const JsonList& list);
		
		sl_bool isJsonMap() const;
		
		JsonMap getJsonMap() const;
		
		void setJsonMap(const JsonMap& map);
		
		sl_bool isJsonMapList() const;
		
		JsonMapList getJsonMapList() const;
		
		void setJsonMapList(const JsonMapList& list);

		Json getElement(sl_size index) const;

		template <class T>
		void getElement(sl_size index, T& _out) const;
		
		sl_bool setElement(sl_size index, const Json& value);
		
		sl_bool addElement(const Json& value);
		
		Json getItem(const String& key) const;
		
		template <class T>
		void getItem(const String& key, T& _out) const;

		sl_bool putItem(const String& key, const Json& value);
		
		sl_bool removeItem(const String& key);
		
		void merge(const Json& other);
		
	protected:
		String toString() const;
		
	public:
		Json operator[](sl_size list_index) const;

		Json operator[](const String& map_key) const;

	public:
		template <class T>
		Json(const T& value);
		
		template <class T>
		Json& operator=(const T& value);
		
		template <class T>
		void get(T& value) const;
		
		template <class T>
		void get(T& value, const T& defaultValue) const;
		
		template <class T>
		void set(const T& value);

	};
	
	SLIB_INLINE JsonItem operator<<=(const String& str, const Json& v)
	{
		return JsonItem(str, v);
	}
	
	SLIB_INLINE JsonItem operator>>=(const String& str, const Json& v)
	{
		return JsonItem(str, v);
	}

	void FromJson(const Json& json, Json& _out);
	void ToJson(Json& json, const Json& _in);
	
	void FromJson(const Json& json, Variant& _out);
	void ToJson(Json& json, const Variant& _in);
	
	void FromJson(const Json& json, AtomicVariant& _out);
	void ToJson(Json& json, const AtomicVariant& _in);
	
	void FromJson(const Json& json, signed char& _out);
	void FromJson(const Json& json, signed char& _out, signed char def);
	void ToJson(Json& json, signed char _in);
	
	void FromJson(const Json& json, unsigned char& _out);
	void FromJson(const Json& json, unsigned char& _out, unsigned char def);
	void ToJson(Json& json, unsigned char _in);
	
	void FromJson(const Json& json, short& _out);
	void FromJson(const Json& json, short& _out, short def);
	void ToJson(Json& json, short _in);
	
	void FromJson(const Json& json, unsigned short& _out);
	void FromJson(const Json& json, unsigned short& _out, unsigned short def);
	void ToJson(Json& json, unsigned short _in);
	
	void FromJson(const Json& json, int& _out);
	void FromJson(const Json& json, int& _out, int def);
	void ToJson(Json& json, int _in);
	
	void FromJson(const Json& json, unsigned int& _out);
	void FromJson(const Json& json, unsigned int& _out, unsigned int def);
	void ToJson(Json& json, unsigned int _in);
	
	void FromJson(const Json& json, long& _out);
	void FromJson(const Json& json, long& _out, long def);
	void ToJson(Json& json, long _in);
	
	void FromJson(const Json& json, unsigned long& _out);
	void FromJson(const Json& json, unsigned long& _out, unsigned long def);
	void ToJson(Json& json, unsigned long _in);
	
	void FromJson(const Json& json, sl_int64& _out);
	void FromJson(const Json& json, sl_int64& _out, sl_int64 def);
	void ToJson(Json& json, sl_int64 _in);
	
	void FromJson(const Json& json, sl_uint64& _out);
	void FromJson(const Json& json, sl_uint64& _out, sl_uint64 def);
	void ToJson(Json& json, sl_uint64 _in);
	
	void FromJson(const Json& json, float& _out);
	void FromJson(const Json& json, float& _out, float def);
	void ToJson(Json& json, float _in);
	
	void FromJson(const Json& json, double& _out);
	void FromJson(const Json& json, double& _out, double def);
	void ToJson(Json& json, double _in);
	
	void FromJson(const Json& json, bool& _out);
	void FromJson(const Json& json, bool& _out, bool def);
	void ToJson(Json& json, bool _in);
	
	void FromJson(const Json& json, String& _out);
	void FromJson(const Json& json, String& _out, const String& def);
	void ToJson(Json& json, const String& _in);
	
	void FromJson(const Json& json, AtomicString& _out);
	void FromJson(const Json& json, AtomicString& _out, const String& def);
	void ToJson(Json& json, const AtomicString& _in);
	
	void FromJson(const Json& json, String16& _out);
	void FromJson(const Json& json, String16& _out, const String16& def);
	void ToJson(Json& json, const String16& _in);
	
	void FromJson(const Json& json, AtomicString16& _out);
	void FromJson(const Json& json, AtomicString16& _out, const String16& def);
	void ToJson(Json& json, const AtomicString16& _in);
	
	void ToJson(Json& json, const sl_char8* sz8);
	void ToJson(Json& json, const sl_char16* sz16);
	
	void FromJson(const Json& json, StringParam& _out);
	void ToJson(Json& json, const StringParam& _in);

#ifdef SLIB_SUPPORT_STD_TYPES
	void FromJson(const Json& json, std::string& _out);
	void FromJson(const Json& json, std::string& _out, const std::string& def);
	void ToJson(Json& json, const std::string& _in);
	
	void FromJson(const Json& json, std::u16string& _out);
	void FromJson(const Json& json, std::u16string& _out, const std::u16string& def);
	void ToJson(Json& json, const std::u16string& _in);
#endif
	
	void FromJson(const Json& json, Time& _out);
	void FromJson(const Json& json, Time& _out, const Time& def);
	void ToJson(Json& json, const Time& _in);
	
	void FromJson(const Json& json, Memory& _out);
	void ToJson(Json& json, const Memory& _in);
	
	void FromJson(const Json& json, BigInt& _out);
	void ToJson(Json& json, const BigInt& _in);
	
	template <class T>
	void FromJson(const Json& json, Nullable<T>& _out);
	template <class T>
	void ToJson(Json& json, const Nullable<T>& ref);
	
	template <class T>
	void FromJson(const Json& json, Ref<T>& _out);
	template <class T>
	void ToJson(Json& json, const Ref<T>& ref);
	
	template <class T>
	void FromJson(const Json& json, AtomicRef<T>& _out);
	template <class T>
	void ToJson(Json& json, const AtomicRef<T>& _in);
	
	template <class T>
	void ToJson(Json& json, const WeakRef<T>& _in);
	template <class T>
	void ToJson(Json& json, const AtomicWeakRef<T>& _in);
	
	void FromJson(const Json& json, List<Variant>& _out);
	void ToJson(Json& json, const List<Variant>& _in);
	
	void FromJson(const Json& json, AtomicList<Variant>& _out);
	void ToJson(Json& json, const AtomicList<Variant>& _in);
	
	void FromJson(const Json& json, Map<String, Variant>& _out);
	void ToJson(Json& json, const Map<String, Variant>& _in);
	
	void FromJson(const Json& json, AtomicMap<String, Variant>& _out);
	void ToJson(Json& json, const AtomicMap<String, Variant>& _in);
	
	void FromJson(const Json& json, HashMap<String, Variant>& _out);
	void ToJson(Json& json, const HashMap<String, Variant>& _in);
	
	void FromJson(const Json& json, AtomicHashMap<String, Variant>& _out);
	void ToJson(Json& json, const AtomicHashMap<String, Variant>& _in);
	
	void FromJson(const Json& json, List< Map<String, Variant> >& _out);
	void ToJson(Json& json, const List< Map<String, Variant> >& _in);
	
	void FromJson(const Json& json, AtomicList< Map<String, Variant> >& _out);
	void ToJson(Json& json, const AtomicList< Map<String, Variant> >& _in);
	
	void FromJson(const Json& json, List< HashMap<String, Variant> >& _out);
	void ToJson(Json& json, const List< HashMap<String, Variant> >& _in);
	
	void FromJson(const Json& json, AtomicList< HashMap<String, Variant> >& _out);
	void ToJson(Json& json, const AtomicList< HashMap<String, Variant> >& _in);
	
	void FromJson(const Json& json, JsonList& _out);
	void ToJson(Json& json, const JsonList& _in);
	
	void FromJson(const Json& json, AtomicJsonList& _out);
	void ToJson(Json& json, const AtomicJsonList& _in);
	
	void FromJson(const Json& json, JsonMap& _out);
	void ToJson(Json& json, const JsonMap& _in);
	
	void FromJson(const Json& json, AtomicJsonMap& _out);
	void ToJson(Json& json, const AtomicJsonMap& _in);
	
	void FromJson(const Json& json, JsonMapList& _out);
	void ToJson(Json& json, const JsonMapList& _in);
	
	void FromJson(const Json& json, AtomicJsonMapList& _out);
	void ToJson(Json& json, const AtomicJsonMapList& _in);
	
	template <class T>
	void FromJson(const Json& json, List<T>& _out);
	template <class T>
	void ToJson(Json& json, const List<T>& _in);
	
	template <class T>
	void FromJson(const Json& json, AtomicList<T>& _out);
	template <class T>
	void ToJson(Json& json, const AtomicList<T>& _in);
	
	template <class T>
	void ToJson(Json& json, const ListParam<T>& _in);
	
	template <class KT, class VT, class KEY_COMPARE>
	void FromJson(const Json& json, Map<KT, VT, KEY_COMPARE>& _out);
	template <class KT, class VT, class KEY_COMPARE>
	void ToJson(Json& json, const Map<KT, VT, KEY_COMPARE>& _in);
	
	template <class KT, class VT, class KEY_COMPARE>
	void FromJson(const Json& json, AtomicMap<KT, VT, KEY_COMPARE>& _out);
	template <class KT, class VT, class KEY_COMPARE>
	void ToJson(Json& json, const AtomicMap<KT, VT, KEY_COMPARE>& _in);
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	void FromJson(const Json& json, HashMap<KT, VT, HASH, KEY_COMPARE>& _out);
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	void ToJson(Json& json, const HashMap<KT, VT, HASH, KEY_COMPARE>& _in);
	
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	void FromJson(const Json& json, AtomicHashMap<KT, VT, HASH, KEY_COMPARE>& _out);
	template <class KT, class VT, class HASH, class KEY_COMPARE>
	void ToJson(Json& json, const AtomicHashMap<KT, VT, HASH, KEY_COMPARE>& _in);
	
#ifdef SLIB_SUPPORT_STD_TYPES
	template <class T>
	void FromJson(const Json& json, std::vector<T>& _out);
	template <class T>
	void ToJson(Json& json, const std::vector<T>& _in);
	
	template <class KT, class VT, class COMPARE, class ALLOC>
	void FromJson(const Json& json, std::map<KT, VT, COMPARE, ALLOC>& _out);
	template <class KT, class VT, class COMPARE, class ALLOC>
	void ToJson(Json& json, const std::map<KT, VT, COMPARE, ALLOC>& _in);
	
	template <class KT, class VT, class HASH, class PRED, class ALLOC>
	void FromJson(const Json& json, std::unordered_map<KT, VT, HASH, PRED, ALLOC>& _out);
	template <class KT, class VT, class HASH, class PRED, class ALLOC>
	void ToJson(Json& json, const std::unordered_map<KT, VT, HASH, PRED, ALLOC>& _in);
#endif

	template <class T>
	void FromJson(const Json& json, T& _out);
	
	template <class T>
	void ToJson(Json& json, const T& _in);

}


#define SLIB_JSON \
public: \
	slib::Json toJson() const \
	{ \
		slib::Json json = slib::Json::createMap(); \
		slib::RemoveConstPointerVariable(this)->doJson(json, sl_false); \
		return json; \
	} \
	void fromJson(const slib::Json& json) \
	{ \
		if (json.isUndefined()) { \
			return; \
		} \
		doJson(*((slib::Json*)&json), sl_true); \
	} \
	void doJson(slib::Json& json, sl_bool isFromJson)

#define SLIB_JSON_ADD_MEMBER(MEMBER_NAME, JSON_NAME) \
	{ \
		static sl_char8 _strJsonField_buf[] = JSON_NAME; \
		static slib::priv::json::JsonFieldContainer _strJsonField_container(_strJsonField_buf, sizeof(_strJsonField_buf)-1); \
		static slib::StringContainer* _strJsonField_str = &_strJsonField_container; \
		static const slib::String& _strJsonField = *(reinterpret_cast<slib::String*>(&_strJsonField_str)); \
		if (isFromJson) { \
			slib::FromJson(json.getItem(_strJsonField), MEMBER_NAME); \
		} else { \
			json.putItem(_strJsonField, MEMBER_NAME); \
		} \
	}

#define SLIB_JSON_ADD_MEMBER_FROM(MEMBER_NAME, JSON_NAME) \
	{ \
		if (isFromJson) { \
			static sl_char8 _strJsonField_buf[] = JSON_NAME; \
			static slib::priv::json::JsonFieldContainer _strJsonField_container(_strJsonField_buf, sizeof(_strJsonField_buf)-1); \
			static slib::StringContainer* _strJsonField_str = &_strJsonField_container; \
			static const slib::String& _strJsonField = *(reinterpret_cast<slib::String*>(&_strJsonField_str)); \
			slib::FromJson(json.getItem(_strJsonField), MEMBER_NAME); \
		} \
	}

#define SLIB_JSON_ADD_MEMBER_TO(MEMBER_NAME, JSON_NAME) \
	{ \
		if (!isFromJson) { \
			static sl_char8 _strJsonField_buf[] = JSON_NAME; \
			static slib::priv::json::JsonFieldContainer _strJsonField_container(_strJsonField_buf, sizeof(_strJsonField_buf)-1); \
			static slib::StringContainer* _strJsonField_str = &_strJsonField_container; \
			static const slib::String& _strJsonField = *(reinterpret_cast<slib::String*>(&_strJsonField_str)); \
			json.putItem(_strJsonField, MEMBER_NAME); \
		} \
	}

#define PRIV_SLIB_JSON_ADD_MEMBERS0
#define PRIV_SLIB_JSON_ADD_MEMBERS1(NAME) SLIB_JSON_ADD_MEMBER(NAME, #NAME)
#define PRIV_SLIB_JSON_ADD_MEMBERS2(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS1(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS3(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS2(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS4(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS3(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS5(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS4(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS6(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS5(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS7(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS6(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS8(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS7(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS9(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS8(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS10(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS9(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS11(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS10(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS12(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS11(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS13(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS12(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS14(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS13(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS15(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS14(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS16(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS15(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS17(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS16(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS18(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS17(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS19(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS18(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS20(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS19(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS21(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS20(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS22(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS21(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS23(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS22(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS24(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS23(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS25(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS24(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS26(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS25(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS27(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS26(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS28(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS27(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS29(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS28(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS30(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS29(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS31(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS30(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS32(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS31(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS33(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS32(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS34(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS33(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS35(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS34(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS36(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS35(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS37(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS36(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS38(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS37(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS39(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS38(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS40(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS39(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS41(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS40(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS42(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS41(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS43(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS42(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS44(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS43(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS45(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS44(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS46(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS45(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS47(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS46(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS48(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS47(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS49(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS48(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS50(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS49(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS51(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS50(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS52(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS51(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS53(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS52(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS54(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS53(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS55(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS54(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS56(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS55(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS57(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS56(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS58(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS57(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS59(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS58(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS60(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS59(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS61(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS60(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS62(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS61(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS63(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS62(__VA_ARGS__),)
#define PRIV_SLIB_JSON_ADD_MEMBERS64(NAME, ...) SLIB_JSON_ADD_MEMBER(NAME, #NAME) SLIB_MACRO_CONCAT(PRIV_SLIB_JSON_ADD_MEMBERS63(__VA_ARGS__),)

#define SLIB_JSON_ADD_MEMBERS(...) SLIB_MACRO_CONCAT(SLIB_MACRO_OVERLOAD(PRIV_SLIB_JSON_ADD_MEMBERS, __VA_ARGS__)(__VA_ARGS__),)

#define SLIB_JSON_MEMBERS(...) \
	SLIB_JSON \
	{ \
		SLIB_JSON_ADD_MEMBERS(__VA_ARGS__) \
	}


#define SLIB_DECLARE_JSON \
public: \
	slib::Json toJson() const; \
	void fromJson(const slib::Json& json); \
	void doJson(slib::Json& json, sl_bool isFromJson);

#define SLIB_DEFINE_JSON(CLASS) \
	slib::Json CLASS::toJson() const \
	{ \
		slib::Json json = slib::Json::createMap(); \
		slib::RemoveConstPointerVariable(this)->doJson(json, sl_false); \
		return json; \
	} \
	void CLASS::fromJson(const slib::Json& json) \
	{ \
		if (json.isUndefined()) { \
			return; \
		} \
		doJson(*((slib::Json*)&json), sl_true); \
	} \
	void CLASS::doJson(slib::Json& json, sl_bool isFromJson)

#define SLIB_DEFINE_JSON_MEMBERS(CLASS, ...) \
	SLIB_DEFINE_JSON(CLASS) \
	{ \
		SLIB_JSON_ADD_MEMBERS(__VA_ARGS__) \
	}

#include "detail/json.inc"

#ifdef SLIB_SUPPORT_STD_TYPES
#include "detail/json_std.inc"
#endif

#endif
