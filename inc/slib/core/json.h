#ifndef CHECKHEADER_SLIB_CORE_JSON
#define CHECKHEADER_SLIB_CORE_JSON

#include "definition.h"

#include "variant.h"

SLIB_NAMESPACE_BEGIN

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
	
public:
	String getErrorText();
	
};

class SLIB_EXPORT Json
{
public:
	static Variant parseJson(const sl_char8* sz, sl_size len, JsonParseParam& param);
	
	static Variant parseJson(const sl_char8* sz, sl_size len);
	
	static Variant parseJson(const String& json, JsonParseParam& param);
	
	static Variant parseJson(const String& json);
	
	static Variant parseJson16(const sl_char16* sz, sl_size len, JsonParseParam& param);
	
	static Variant parseJson16(const sl_char16* sz, sl_size len);
	
	static Variant parseJson16(const String16& json, JsonParseParam& param);
	
	static Variant parseJson16(const String16& json);
	
	static Variant parseJsonFromTextFile(const String& filePath, JsonParseParam& param);
	
	static Variant parseJsonFromTextFile(const String& filePath);
	
	static Variant parseJsonUtf8(const Memory& mem, JsonParseParam& param);
	
	static Variant parseJsonUtf8(const Memory& mem);
	
	static Variant parseJson16Utf8(const Memory& mem, JsonParseParam& param);
	
	static Variant parseJson16Utf8(const Memory& mem);

	static String toString(const Variant& var);
	
	
	static Variant createList();
	
	static Variant createMap();
	
	
	template <class T>
	static void fromJson(const Variant& v, List<T>& _out);
	
	template <class T>
	static void fromJson(const Variant& v, List<T>& _out, const List<T>& def);
	
	template <class T>
	static Variant toJson(const List<T>& _in);

	template <class T>
	static void fromJson(const Variant& v, AtomicList<T>& _out);
	
	template <class T>
	static void fromJson(const Variant& v, AtomicList<T>& _out, const AtomicList<T>& def);
	
	template <class T>
	static Variant toJson(const AtomicList<T>& _in);
	
	template <class T>
	static void fromJson(const Variant& v, Map<String, T>& _out);
	
	template <class T>
	static void fromJson(const Variant& v, Map<String, T>& _out, const Map<String, T>& def);
	
	template <class T>
	static Variant toJson(const Map<String, T>& _in);
	
	template <class T>
	static void fromJson(const Variant& v, AtomicMap<String, T>& _out);
	
	template <class T>
	static void fromJson(const Variant& v, AtomicMap<String, T>& _out, const AtomicMap<String, T>& def);
	
	template <class T>
	static Variant toJson(const AtomicMap<String, T>& _in);
	
	template <class T>
	static void fromJson(const Variant& v, Ref<T>& _out);
	
	template <class T>
	static void fromJson(const Variant& v, Ref<T>& _out, const Ref<T>& def);
	
	template <class T>
	static Variant toJson(const Ref<T>& _in);
	
	template <class T>
	static void fromJson(const Variant& v, AtomicRef<T>& _out);
	
	template <class T>
	static void fromJson(const Variant& v, AtomicRef<T>& _out, const AtomicRef<T>& def);
	
	template <class T>
	static Variant toJson(const AtomicRef<T>& _in);

	template <class T>
	static void fromJson(const Variant& v, WeakRef<T>& _out);
	
	template <class T>
	static void fromJson(const Variant& v, WeakRef<T>& _out, const WeakRef<T>& def);
	
	template <class T>
	static Variant toJson(const WeakRef<T>& _in);
	
	template <class T>
	static void fromJson(const Variant& v, AtomicWeakRef<T>& _out);
	
	template <class T>
	static void fromJson(const Variant& v, AtomicWeakRef<T>& _out, const AtomicWeakRef<T>& def);
	
	template <class T>
	static Variant toJson(const AtomicWeakRef<T>& _in);

	static Variant toJson(const sl_char8* _in);
	
	static Variant toJson(const sl_char16* _in);
	
	static Variant toJson(const sl_char32* _in);
	
	template <class T>
	static void fromJson(const Variant& v, T& _out);
	
	template <class T>
	static void fromJson(const Variant& v, T& _out, const T& def);
	
	template <class T>
	static Variant toJson(const T& _in);
	
	template <class T>
	static void get(const Variant& map, const String& fieldName, T& _out);
	
	template <class T>
	static void get(const Variant& map, const String& fieldName, T& _out, const T& def);
	
	template <class T>
	static void put(const Variant& map, const String& fieldName, const T& _in);
	
	template <class T>
	static void get(const Variant& list, sl_size index, T& _out);
	
	template <class T>
	static void get(const Variant& list, sl_size index, T& _out, const T& def);
	
	template <class T>
	static void add(const Variant& list, const T& _in);
	
};

template <class T>
SLIB_INLINE void Json::fromJson(const Variant& v, T& _out)
{
	_out.fromJson(v);
}

template <class T>
SLIB_INLINE void Json::fromJson(const Variant& v, T& _out, const T& _def)
{
	_out.fromJson(v);
}

template <class T>
SLIB_INLINE Variant Json::toJson(const T& _in)
{
	return _in.toJson();
}

template <class T>
SLIB_INLINE void Json::get(const Variant& v, const String& fieldName, T& _out)
{
	Map<String, Variant> map = v.getVariantMap();
	Variant item;
	if (map.get(fieldName, &item)) {
		Json::fromJson(item, _out);
	}
}

template <class T>
SLIB_INLINE void Json::put(const Variant& v, const String& fieldName, const T& _in)
{
	Map<String, Variant> map = v.getVariantMap();
	map.put(fieldName, Json::toJson(_in));
}

template <class T>
SLIB_INLINE void Json::get(const Variant& v, sl_size index, T& _out)
{
	List<Variant> list = v.getVariantList();
	Variant item;
	if (list.getAt(index, &item)) {
		Json::fromJson(item, _out);
	}
}

template <class T>
SLIB_INLINE void Json::add(const Variant& v, const T& _in)
{
	List<Variant> list = v.getVariantList();
	if (list.isNotNull()) {
		list.add(Json::toJson(_in));
	}
}

template <> void Json::fromJson(const Variant& v, Variant& _out);
template <> void Json::fromJson(const Variant& v, Variant& _out, const Variant& def);
template <> Variant Json::toJson(const Variant& _in);

template <> void Json::fromJson(const Variant& v, AtomicVariant& _out);
template <> void Json::fromJson(const Variant& v, AtomicVariant& _out, const AtomicVariant& def);
template <> Variant Json::toJson(const AtomicVariant& _in);

template <> void Json::fromJson(const Variant& v, sl_int32& _out);
template <> void Json::fromJson(const Variant& v, sl_int32& _out, const sl_int32& def);
template <> Variant Json::toJson(const sl_int32& _in);

template <> void Json::fromJson(const Variant& v, sl_uint32& _out);
template <> void Json::fromJson(const Variant& v, sl_uint32& _out, const sl_uint32& def);
template <> Variant Json::toJson(const sl_uint32& _in);

template <> void Json::fromJson(const Variant& v, sl_int64& _out);
template <> void Json::fromJson(const Variant& v, sl_int64& _out, const sl_int64& def);
template <> Variant Json::toJson(const sl_int64& _in);

template <> void Json::fromJson(const Variant& v, sl_uint64& _out);
template <> void Json::fromJson(const Variant& v, sl_uint64& _out, const sl_uint64& def);
template <> Variant Json::toJson(const sl_uint64& _in);

template <> void Json::fromJson(const Variant& v, sl_int8& _out);
template <> void Json::fromJson(const Variant& v, sl_int8& _out, const sl_int8& def);
template <> Variant Json::toJson(const sl_int8& _in);

template <> void Json::fromJson(const Variant& v, sl_uint8& _out);
template <> void Json::fromJson(const Variant& v, sl_uint8& _out, const sl_uint8& def);
template <> Variant Json::toJson(const sl_uint8& _in);

template <> void Json::fromJson(const Variant& v, sl_int16& _out);
template <> void Json::fromJson(const Variant& v, sl_int16& _out, const sl_int16& def);
template <> Variant Json::toJson(const sl_int16& _in);

template <> void Json::fromJson(const Variant& v, sl_uint16& _out);
template <> void Json::fromJson(const Variant& v, sl_uint16& _out, const sl_uint16& def);
template <> Variant Json::toJson(const sl_uint16& _in);

template <> void Json::fromJson(const Variant& v, float& _out);
template <> void Json::fromJson(const Variant& v, float& _out, const float& def);
template <> Variant Json::toJson(const float& _in);

template <> void Json::fromJson(const Variant& v, double& _out);
template <> void Json::fromJson(const Variant& v, double& _out, const double& def);
template <> Variant Json::toJson(const double& _in);

template <> void Json::fromJson(const Variant& v, bool& _out);
template <> void Json::fromJson(const Variant& v, bool& _out, const bool& def);
template <> Variant Json::toJson(const bool& _in);

template <> void Json::fromJson(const Variant& v, String& _out);
template <> void Json::fromJson(const Variant& v, String& _out, const String& def);
template <> Variant Json::toJson(const String& _in);

template <> void Json::fromJson(const Variant& v, AtomicString& _out);
template <> void Json::fromJson(const Variant& v, AtomicString& _out, const AtomicString& def);
template <> Variant Json::toJson(const AtomicString& _in);

template <> void Json::fromJson(const Variant& v, String16& _out);
template <> void Json::fromJson(const Variant& v, String16& _out, const String16& def);
template <> Variant Json::toJson(const String16& _in);

template <> void Json::fromJson(const Variant& v, AtomicString16& _out);
template <> void Json::fromJson(const Variant& v, AtomicString16& _out, const AtomicString16& def);
template <> Variant Json::toJson(const AtomicString16& _in);

template <> void Json::fromJson(const Variant& v, Time& _out);
template <> void Json::fromJson(const Variant& v, Time& _out, const Time& def);
template <> Variant Json::toJson(const Time& _in);

template <> void Json::fromJson(const Variant& v, List<Variant>& _out);
template <> void Json::fromJson(const Variant& v, List<Variant>& _out, const List<Variant>& def);
template <> Variant Json::toJson(const List<Variant>& _in);

template <> void Json::fromJson(const Variant& v, AtomicList<Variant>& _out);
template <> void Json::fromJson(const Variant& v, AtomicList<Variant>& _out, const AtomicList<Variant>& def);
template <> Variant Json::toJson(const AtomicList<Variant>& _in);

template <> void Json::fromJson(const Variant& v, Map<String, Variant>& _out);
template <> void Json::fromJson(const Variant& v, Map<String, Variant>& _out, const Map<String, Variant>& def);
template <> Variant Json::toJson(const Map<String, Variant>& _in);

template <> void Json::fromJson(const Variant& v, AtomicMap<String, Variant>& _out);
template <> void Json::fromJson(const Variant& v, AtomicMap<String, Variant>& _out, const AtomicMap<String, Variant>& def);
template <> Variant Json::toJson(const AtomicMap<String, Variant>& _in);

template <> void Json::fromJson(const Variant& v, List< Map<String, Variant> >& _out);
template <> void Json::fromJson(const Variant& v, List< Map<String, Variant> >& _out, const List< Map<String, Variant> >& def);
template <> Variant Json::toJson(const List< Map<String, Variant> >& _in);

template <> void Json::fromJson(const Variant& v, AtomicList< Map<String, Variant> >& _out);
template <> void Json::fromJson(const Variant& v, AtomicList< Map<String, Variant> >& _out, const AtomicList< Map<String, Variant> >& def);
template <> Variant Json::toJson(const AtomicList< Map<String, Variant> >& _in);

template <class T>
void Json::fromJson(const Variant& v, List<T>& _out)
{
	List<T> dst;
	Ref<Referable> obj(v.getObject());
	if (obj.isNotNull()) {
		if (CList<Variant>* _src = CastInstance< CList<Variant> >(obj.get())) {
			ListLocker<Variant> src(*_src);
			for (sl_size i = 0; i < src.count; i++) {
				Variant& v = src[i];
				T o;
				Json::fromJson(v, o);
				dst.add_NoLock(o);
			}
		} else if (CList< Map<String, Variant> >* _src = CastInstance< CList< Map<String, Variant> > >(obj.get())) {
			ListLocker< Map<String, Variant> > src(*_src);
			for (sl_size i = 0; i < src.count; i++) {
				Variant v(src[i]);
				T o;
				Json::fromJson(v, o);
				dst.add_NoLock(o);
			}
		}
	}
	_out = dst;
}

template <class T>
void Json::fromJson(const Variant& v, List<T>& _out, const List<T>& def)
{
	if (v.isNotNull()) {
		Json::fromJson(v, _out);
	} else {
		_out = def;
	}
}

template <class T>
Variant Json::toJson(const List<T>& _in)
{
	List<Variant> list;
	if (_in.isNotNull()) {
		ListLocker<T> src(_in);
		for (sl_size i = 0; i < src.count; i++) {
			T& o = src[i];
			list.add_NoLock(Json::toJson(o));
		}
		return list;
	}
	return sl_null;
}

template <class T>
void Json::fromJson(const Variant& v, AtomicList<T>& _out)
{
	List<T> __out;
	Json::fromJson(v, __out);
	_out = __out;
}

template <class T>
void Json::fromJson(const Variant& v, AtomicList<T>& _out, const AtomicList<T>& def)
{
	if (v.isNotNull()) {
		Json::fromJson(v, _out);
	} else {
		_out = def;
	}
}

template <class T>
Variant Json::toJson(const AtomicList<T>& _in)
{
	return Json::toJson(List<T>(_in));
}

template <class T>
void Json::fromJson(const Variant& v, Map<String, T>& _out)
{
	Iterator< Pair<String, Variant> > iterator = v.getVariantMap().toIterator();
	if (iterator.isNotNull()) {
		Map<String, T> map;
		map.initHash();
		Pair<String, Variant> pair;
		while (iterator.next(&pair)) {
			T o;
			Json::fromJson(pair.value, o);
			map.put_NoLock(pair.key, o);
		}
		_out = map;
	} else {
		_out.setNull();
	}
}

template <class T>
void Json::fromJson(const Variant& v, Map<String, T>& _out, const Map<String, T>& def)
{
	if (v.isNotNull()) {
		Json::fromJson(v, _out);
	} else {
		_out = def;
	}
}

template <class T>
Variant Json::toJson(const Map<String, T>& _in)
{
	Map<String, Variant> map;
	Iterator< Pair<String, T> > iterator = _in.toIterator();
	if (iterator.isNotNull()) {
		map.initList();
		Pair<String, T> pair;
		while (iterator.next(&pair)) {
			map.put_NoLock(pair.key, Json::toJson(pair.value), MapPutMode::AddAlways);
		}
	}
	return map;
}

template <class T>
void Json::fromJson(const Variant& v, AtomicMap<String, T>& _out)
{
	Map<String, T> __out;
	Json::fromJson(v, __out);
	_out = __out;
}

template <class T>
void Json::fromJson(const Variant& v, AtomicMap<String, T>& _out, const AtomicMap<String, T>& def)
{
	if (v.isNotNull()) {
		Json::fromJson(v, _out);
	} else {
		_out = def;
	}
}

template <class T>
Variant Json::toJson(const AtomicMap<String, T>& _in)
{
	return Json::toJson(Map<String, T>(_in));
}

template <class T>
void Json::fromJson(const Variant& v, Ref<T>& _out)
{
	if (v.isNotNull()) {
		Ref<T> o = new T;
		if (o.isNotNull()) {
			Json::fromJson(v, *o);
			_out = o;
			return;
		}
	}
	_out.setNull();
}

template <class T>
void Json::fromJson(const Variant& v, Ref<T>& _out, const Ref<T>& def)
{
	if (v.isNotNull()) {
		fromJson(v, _out);
	} else {
		_out = def;
	}
}

template <class T>
Variant Json::toJson(const Ref<T>& _in)
{
	if (_in.isNotNull()) {
		return toJson(*_in);
	}
	return sl_null;
}

template <class T>
void Json::fromJson(const Variant& v, AtomicRef<T>& _out)
{
	Ref<T> __out;
	fromJson(v, __out);
	_out = __out;
}

template <class T>
void Json::fromJson(const Variant& v, AtomicRef<T>& _out, const AtomicRef<T>& def)
{
	if (v.isNotNull()) {
		fromJson(v, _out);
	} else {
		_out = def;
	}
}

template <class T>
Variant Json::toJson(const AtomicRef<T>& _in)
{
	return toJson(Ref<T>(_in));
}

template <class T>
void Json::fromJson(const Variant& v, WeakRef<T>& _out)
{
	_out.setNull();
}

template <class T>
void Json::fromJson(const Variant& v, WeakRef<T>& _out, const WeakRef<T>& def)
{
	_out = def;
}

template <class T>
Variant Json::toJson(const WeakRef<T>& _in)
{
	return toJson(Ref<T>(_in));
}

template <class T>
void Json::fromJson(const Variant& v, AtomicWeakRef<T>& _out)
{
	_out.setNull();
}

template <class T>
void Json::fromJson(const Variant& v, AtomicWeakRef<T>& _out, const AtomicWeakRef<T>& def)
{
	_out = def;
}

template <class T>
Variant Json::toJson(const AtomicWeakRef<T>& _in)
{
	return toJson(Ref<T>(_in));
}

SLIB_NAMESPACE_END

#endif
