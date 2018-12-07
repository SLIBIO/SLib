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

		~JsonParseParam();

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
		
#ifdef SLIB_SUPPORT_STD_TYPES
		Json(const std::string& value);

		Json(const std::u16string& value);
#endif
		
		Json(const Time& value);
		
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

		template <class T>
		Json(const T& value);
		
	public:
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
		
		template <class T>
		Json& operator=(const T& value);

		Json operator[](sl_size indexForVariantList) const;
		
		Json operator[](const String& keyForVariantMap) const;
		
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
		
	public:
		void get(Json& _out) const;
		void set(const Json& _in);
		
		void get(Variant& _out) const;
		void set(const Variant& _in);
		
		void get(AtomicVariant& _out) const;
		void set(const AtomicVariant& _in);
		
		void get(signed char& _out) const;
		void get(signed char& _out, signed char def) const;
		void set(signed char _in);
		
		void get(unsigned char& _out) const;
		void get(unsigned char& _out, unsigned char def) const;
		void set(unsigned char _in);
		
		void get(short& _out) const;
		void get(short& _out, short def) const;
		void set(short _in);
		
		void get(unsigned short& _out) const;
		void get(unsigned short& _out, unsigned short def) const;
		void set(unsigned short _in);
		
		void get(int& _out) const;
		void get(int& _out, int def) const;
		void set(int _in);
		
		void get(unsigned int& _out) const;
		void get(unsigned int& _out, unsigned int def) const;
		void set(unsigned int _in);
		
		void get(long& _out) const;
		void get(long& _out, long def) const;
		void set(long _in);
		
		void get(unsigned long& _out) const;
		void get(unsigned long& _out, unsigned long def) const;
		void set(unsigned long _in);
		
		void get(sl_int64& _out) const;
		void get(sl_int64& _out, sl_int64 def) const;
		void set(sl_int64 _in);
		
		void get(sl_uint64& _out) const;
		void get(sl_uint64& _out, sl_uint64 def) const;
		void set(sl_uint64 _in);
		
		void get(float& _out) const;
		void get(float& _out, float def) const;
		void set(float _in);
		
		void get(double& _out) const;
		void get(double& _out, double def) const;
		void set(double _in);
		
		void get(bool& _out) const;
		void get(bool& _out, bool def) const;
		void set(bool _in);
		
		void get(String& _out) const;
		void get(String& _out, const String& def) const;
		void set(const String& _in);
		
		void get(AtomicString& _out) const;
		void get(AtomicString& _out, const String& def) const;
		void set(const AtomicString& _in);
		
		void get(String16& _out) const;
		void get(String16& _out, const String16& def) const;
		void set(const String16& _in);
		
		void get(AtomicString16& _out) const;
		void get(AtomicString16& _out, const String16& def) const;
		void set(const AtomicString16& _in);
		
		void set(const sl_char8* sz8);
		void set(const sl_char16* sz16);
		
#ifdef SLIB_SUPPORT_STD_TYPES
		void get(std::string& _out) const;
		void get(std::string& _out, const std::string& def) const;
		void set(const std::string& _in);

		void get(std::u16string& _out) const;
		void get(std::u16string& _out, const std::u16string& def) const;
		void set(const std::u16string& _in);
#endif

		void get(Time& _out) const;
		void get(Time& _out, const Time& def) const;
		void set(const Time& _in);
		
		template <class T>
		void get(Ref<T>& _out) const;
		template <class T>
		void set(const Ref<T>& ref);
		
		template <class T>
		void get(AtomicRef<T>& _out) const;
		template <class T>
		void set(const AtomicRef<T>& _in);
		
		template <class T>
		void set(const WeakRef<T>& _in);
		template <class T>
		void set(const AtomicWeakRef<T>& _in);
		
		void get(List<Variant>& _out) const;
		void set(const List<Variant>& _in);
		
		void get(AtomicList<Variant>& _out) const;
		void set(const AtomicList<Variant>& _in);
		
		void get(Map<String, Variant>& _out) const;
		void set(const Map<String, Variant>& _in);
		
		void get(AtomicMap<String, Variant>& _out) const;
		void set(const AtomicMap<String, Variant>& _in);
		
		void get(HashMap<String, Variant>& _out) const;
		void set(const HashMap<String, Variant>& _in);
		
		void get(AtomicHashMap<String, Variant>& _out) const;
		void set(const AtomicHashMap<String, Variant>& _in);
		
		void get(List< Map<String, Variant> >& _out) const;
		void set(const List< Map<String, Variant> >& _in);
		
		void get(AtomicList< Map<String, Variant> >& _out) const;
		void set(const AtomicList< Map<String, Variant> >& _in);
		
		void get(List< HashMap<String, Variant> >& _out) const;
		void set(const List< HashMap<String, Variant> >& _in);
		
		void get(AtomicList< HashMap<String, Variant> >& _out) const;
		void set(const AtomicList< HashMap<String, Variant> >& _in);
		
		void get(JsonList& _out) const;
		void set(const JsonList& _in);
		
		void get(AtomicJsonList& _out) const;
		void set(const AtomicJsonList& _in);
		
		void get(JsonMap& _out) const;
		void set(const JsonMap& _in);
		
		void get(AtomicJsonMap& _out) const;
		void set(const AtomicJsonMap& _in);
		
		void get(JsonMapList& _out) const;
		void set(const JsonMapList& _in);
		
		void get(AtomicJsonMapList& _out) const;
		void set(const AtomicJsonMapList& _in);
		
		template <class T>
		void get(List<T>& _out) const;
		template <class T>
		void set(const List<T>& _in);
		
		template <class T>
		void get(AtomicList<T>& _out) const;
		template <class T>
		void set(const AtomicList<T>& _in);
		
		template <class KT, class VT, class KEY_COMPARE>
		void get(Map<KT, VT, KEY_COMPARE>& _out) const;
		template <class KT, class VT, class KEY_COMPARE>
		void set(const Map<KT, VT, KEY_COMPARE>& _in);
		
		template <class KT, class VT, class KEY_COMPARE>
		void get(AtomicMap<KT, VT, KEY_COMPARE>& _out) const;
		template <class KT, class VT, class KEY_COMPARE>
		void set(const AtomicMap<KT, VT, KEY_COMPARE>& _in);
		
		template <class KT, class VT, class HASH, class KEY_COMPARE>
		void get(HashMap<KT, VT, HASH, KEY_COMPARE>& _out) const;
		template <class KT, class VT, class HASH, class KEY_COMPARE>
		void set(const HashMap<KT, VT, HASH, KEY_COMPARE>& _in);
		
		template <class KT, class VT, class HASH, class KEY_COMPARE>
		void get(AtomicHashMap<KT, VT, HASH, KEY_COMPARE>& _out) const;
		template <class KT, class VT, class HASH, class KEY_COMPARE>
		void set(const AtomicHashMap<KT, VT, HASH, KEY_COMPARE>& _in);
		
#ifdef SLIB_SUPPORT_STD_TYPES
		template <class T>
		void get(std::vector<T>& _out) const;
		template <class T>
		void set(const std::vector<T>& _in);
		
		template <class KT, class VT, class COMPARE, class ALLOC>
		void get(std::map<KT, VT, COMPARE, ALLOC>& _out) const;
		template <class KT, class VT, class COMPARE, class ALLOC>
		void set(const std::map<KT, VT, COMPARE, ALLOC>& _in);
		
		template <class KT, class VT, class HASH, class PRED, class ALLOC>
		void get(std::unordered_map<KT, VT, HASH, PRED, ALLOC>& _out) const;
		template <class KT, class VT, class HASH, class PRED, class ALLOC>
		void set(const std::unordered_map<KT, VT, HASH, PRED, ALLOC>& _in);
#endif

		template <class T>
		void get(T& _out) const;
		template <class T>
		void set(const T& _in);

	protected:
		String toString() const;
		
	};
	
	SLIB_INLINE JsonItem operator<<=(const String& str, const Json& v)
	{
		return JsonItem(str, v);
	}
	
	SLIB_INLINE JsonItem operator>>=(const String& str, const Json& v)
	{
		return JsonItem(str, v);
	}

}

#include "detail/json.inc"

#ifdef SLIB_SUPPORT_STD_TYPES
#include "detail/json_std.inc"
#endif

#endif
