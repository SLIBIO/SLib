/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
		
		Json(const List< Map<String, Variant> >& list);
		
		Json(const AtomicList< Map<String, Variant> >& list);
		
		Json(const List<Json>& list);
		
		Json(const AtomicList<Json>& list);
		
		Json(const Map<String, Json>& map);
		
		Json(const AtomicMap<String, Json>& map);
		
		Json(const List< Map<String, Json> >& list);
		
		Json(const AtomicList< Map<String, Json> >& list);

		template <class T>
		Json(const List<T>& list);
		
		template <class T>
		Json(const AtomicList<T>& list);
		
		template <class KT, class VT>
		Json(const Map<KT, VT>& map);
		
		template <class KT, class VT>
		Json(const AtomicMap<KT, VT>& map);
		
#ifdef SLIB_SUPPORT_STD_TYPES
		Json(const std::initializer_list< Pair<String, Json> >& pairs);
		
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
		Json& operator=(const std::initializer_list< Pair<String, Json> >& pairs);
		
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
		
		List<Json> getJsonList() const;
		
		void setJsonList(const List<Json>& list);
		
		sl_bool isJsonMap() const;
		
		Map<String, Json> getJsonMap() const;
		
		void setJsonMap(const Map<String, Json>& map);
		
		sl_bool isJsonMapList() const;
		
		List< Map<String, Json> > getJsonMapList() const;
		
		void setJsonMapList(const List< Map<String, Json> >& list);

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
		
		void get(List< Map<String, Variant> >& _out) const;
		void set(const List< Map<String, Variant> >& _in);
		
		void get(AtomicList< Map<String, Variant> >& _out) const;
		void set(const AtomicList< Map<String, Variant> >& _in);
		
		void get(List<Json>& _out) const;
		void set(const List<Json>& _in);
		
		void get(AtomicList<Json>& _out) const;
		void set(const AtomicList<Json>& _in);
		
		void get(Map<String, Json>& _out) const;
		void set(const Map<String, Json>& _in);
		
		void get(AtomicMap<String, Json>& _out) const;
		void set(const AtomicMap<String, Json>& _in);
		
		void get(List< Map<String, Json> >& _out) const;
		void set(const List< Map<String, Json> >& _in);
		
		void get(AtomicList< Map<String, Json> >& _out) const;
		void set(const AtomicList< Map<String, Json> >& _in);
		
		template <class T>
		void get(List<T>& _out) const;
		template <class T>
		void set(const List<T>& _in);
		
		template <class T>
		void get(AtomicList<T>& _out) const;
		template <class T>
		void set(const AtomicList<T>& _in);
		
		template <class KT, class VT>
		void get(Map<KT, VT>& _out) const;
		template <class KT, class VT>
		void set(const Map<KT, VT>& _in);
		
		template <class KT, class VT>
		void get(AtomicMap<KT, VT>& _out) const;
		template <class KT, class VT>
		void set(const AtomicMap<KT, VT>& _in);
		
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
	
	SLIB_INLINE Pair<String, Json> operator<<=(const String& str, const Json& v)
	{
		return Pair<String, Json>(str, v);
	}
	
	SLIB_INLINE Pair<String, Json> operator>>=(const String& str, const Json& v)
	{
		return Pair<String, Json>(str, v);
	}
	
	typedef List<Json> JsonList;
	typedef AtomicList<Json> AtomicJsonList;
	typedef Map<String, Json> JsonMap;
	typedef AtomicMap<String, Json> AtomicJsonMap;
	typedef List< Map<String, Json> > JsonMapList;
	typedef AtomicList< Map<String, Json> > AtomicJsonMapList;
	
	typedef Pair<String, Json> JsonPair;

}

#include "detail/json.inc"

#ifdef SLIB_SUPPORT_STD_TYPES
#include "detail/json_std.inc"
#endif

#endif
