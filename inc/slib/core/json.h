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
	
	static String toString(const Variant& var);
	
	
	static Variant createList();
	
	static Variant createMap();
	
	
	template <class T, class COMPARE>
	static void fromJson(const Variant& v, List<T, COMPARE>& _out);
	
	template <class T, class COMPARE>
	static void fromJson(const Variant& v, List<T, COMPARE>& _out, const List<T, COMPARE>& def);
	
	template <class T, class COMPARE>
	static Variant toJson(const List<T, COMPARE>& _in);

	template <class T, class COMPARE>
	static void fromJson(const Variant& v, SafeList<T, COMPARE>& _out);
	
	template <class T, class COMPARE>
	static void fromJson(const Variant& v, SafeList<T, COMPARE>& _out, const SafeList<T, COMPARE>& def);
	
	template <class T, class COMPARE>
	static Variant toJson(const SafeList<T, COMPARE>& _in);
	
	template <class T>
	static void fromJson(const Variant& v, Map<String, T>& _out);
	
	template <class T>
	static void fromJson(const Variant& v, Map<String, T>& _out, const Map<String, T>& def);
	
	template <class T>
	static Variant toJson(const Map<String, T>& _in);
	
	template <class T>
	static void fromJson(const Variant& v, SafeMap<String, T>& _out);
	
	template <class T>
	static void fromJson(const Variant& v, SafeMap<String, T>& _out, const SafeMap<String, T>& def);
	
	template <class T>
	static Variant toJson(const SafeMap<String, T>& _in);
	
	template <class T>
	static void fromJson(const Variant& v, Ref<T>& _out);
	
	template <class T>
	static void fromJson(const Variant& v, Ref<T>& _out, const Ref<T>& def);
	
	template <class T>
	static Variant toJson(const Ref<T>& _in);
	
	template <class T>
	static void fromJson(const Variant& v, SafeRef<T>& _out);
	
	template <class T>
	static void fromJson(const Variant& v, SafeRef<T>& _out, const SafeRef<T>& def);
	
	template <class T>
	static Variant toJson(const SafeRef<T>& _in);

	template <class T>
	static void fromJson(const Variant& v, WeakRef<T>& _out);
	
	template <class T>
	static void fromJson(const Variant& v, WeakRef<T>& _out, const WeakRef<T>& def);
	
	template <class T>
	static Variant toJson(const WeakRef<T>& _in);
	
	template <class T>
	static void fromJson(const Variant& v, SafeWeakRef<T>& _out);
	
	template <class T>
	static void fromJson(const Variant& v, SafeWeakRef<T>& _out, const SafeWeakRef<T>& def);
	
	template <class T>
	static Variant toJson(const SafeWeakRef<T>& _in);

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
	if (list.getItem(index, &item)) {
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

template <> void Json::fromJson(const Variant& v, SafeVariant& _out);
template <> void Json::fromJson(const Variant& v, SafeVariant& _out, const SafeVariant& def);
template <> Variant Json::toJson(const SafeVariant& _in);

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

template <> void Json::fromJson(const Variant& v, String8& _out);
template <> void Json::fromJson(const Variant& v, String8& _out, const String8& def);
template <> Variant Json::toJson(const String8& _in);

template <> void Json::fromJson(const Variant& v, SafeString8& _out);
template <> void Json::fromJson(const Variant& v, SafeString8& _out, const SafeString8& def);
template <> Variant Json::toJson(const SafeString8& _in);

template <> void Json::fromJson(const Variant& v, String16& _out);
template <> void Json::fromJson(const Variant& v, String16& _out, const String16& def);
template <> Variant Json::toJson(const String16& _in);

template <> void Json::fromJson(const Variant& v, SafeString16& _out);
template <> void Json::fromJson(const Variant& v, SafeString16& _out, const SafeString16& def);
template <> Variant Json::toJson(const SafeString16& _in);

template <> void Json::fromJson(const Variant& v, List<Variant>& _out);
template <> void Json::fromJson(const Variant& v, List<Variant>& _out, const List<Variant>& def);
template <> Variant Json::toJson(const List<Variant>& _in);

template <> void Json::fromJson(const Variant& v, SafeList<Variant>& _out);
template <> void Json::fromJson(const Variant& v, SafeList<Variant>& _out, const SafeList<Variant>& def);
template <> Variant Json::toJson(const SafeList<Variant>& _in);

template <> void Json::fromJson(const Variant& v, Map<String, Variant>& _out);
template <> void Json::fromJson(const Variant& v, Map<String, Variant>& _out, const Map<String, Variant>& def);
template <> Variant Json::toJson(const Map<String, Variant>& _in);

template <> void Json::fromJson(const Variant& v, SafeMap<String, Variant>& _out);
template <> void Json::fromJson(const Variant& v, SafeMap<String, Variant>& _out, const SafeMap<String, Variant>& def);
template <> Variant Json::toJson(const SafeMap<String, Variant>& _in);

template <> void Json::fromJson(const Variant& v, List< Map<String, Variant> >& _out);
template <> void Json::fromJson(const Variant& v, List< Map<String, Variant> >& _out, const List< Map<String, Variant> >& def);
template <> Variant Json::toJson(const List< Map<String, Variant> >& _in);

template <> void Json::fromJson(const Variant& v, SafeList< Map<String, Variant> >& _out);
template <> void Json::fromJson(const Variant& v, SafeList< Map<String, Variant> >& _out, const SafeList< Map<String, Variant> >& def);
template <> Variant Json::toJson(const SafeList< Map<String, Variant> >& _in);

template <class T, class COMPARE>
void Json::fromJson(const Variant& v, List<T, COMPARE>& _out)
{
	List<T, COMPARE> dst;
	Ref<Referable> obj(v.getObject());
	if (obj.isNotNull()) {
		if (CList<Variant>::checkInstance(obj.ptr)) {
			ListLocker<Variant> src(*((CList<Variant>*)(obj.ptr)));
			for (sl_size i = 0; i < src.count; i++) {
				Variant& v = src[i];
				T o;
				Json::fromJson(v, o);
				dst.add_NoLock(o);
			}
		} else if (CList< Map<String, Variant> >::checkInstance(obj.ptr)) {
			ListLocker< Map<String, Variant> > src(*((CList< Map<String, Variant> >*)(obj.ptr)));
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

template <class T, class COMPARE>
void Json::fromJson(const Variant& v, List<T, COMPARE>& _out, const List<T, COMPARE>& def)
{
	if (v.isNotNull()) {
		Json::fromJson(v, _out);
	} else {
		_out = def;
	}
}

template <class T, class COMPARE>
Variant Json::toJson(const List<T, COMPARE>& _in)
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
	return Variant::null();
}

template <class T, class COMPARE>
void Json::fromJson(const Variant& v, SafeList<T, COMPARE>& _out)
{
	List<T, COMPARE> __out;
	Json::fromJson(v, __out);
	_out = __out;
}

template <class T, class COMPARE>
void Json::fromJson(const Variant& v, SafeList<T, COMPARE>& _out, const SafeList<T, COMPARE>& def)
{
	if (v.isNotNull()) {
		Json::fromJson(v, _out);
	} else {
		_out = def;
	}
}

template <class T, class COMPARE>
Variant Json::toJson(const SafeList<T, COMPARE>& _in)
{
	return Json::toJson(List<T, COMPARE>(_in));
}

template <class T>
void Json::fromJson(const Variant& v, Map<String, T>& _out)
{
	Iterator< Pair<String, Variant> > iterator = v.getVariantMap().iterator();
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
		_out = Map<String, T>::null();
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
	Iterator< Pair<String, T> > iterator = _in.iterator();
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
void Json::fromJson(const Variant& v, SafeMap<String, T>& _out)
{
	Map<String, T> __out;
	Json::fromJson(v, __out);
	_out = __out;
}

template <class T>
void Json::fromJson(const Variant& v, SafeMap<String, T>& _out, const SafeMap<String, T>& def)
{
	if (v.isNotNull()) {
		Json::fromJson(v, _out);
	} else {
		_out = def;
	}
}

template <class T>
Variant Json::toJson(const SafeMap<String, T>& _in)
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
	_out = Ref<T>::null();
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
	return Variant::null();
}

template <class T>
void Json::fromJson(const Variant& v, SafeRef<T>& _out)
{
	Ref<T> __out;
	fromJson(v, __out);
	_out = __out;
}

template <class T>
void Json::fromJson(const Variant& v, SafeRef<T>& _out, const SafeRef<T>& def)
{
	if (v.isNotNull()) {
		fromJson(v, _out);
	} else {
		_out = def;
	}
}

template <class T>
Variant Json::toJson(const SafeRef<T>& _in)
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
void Json::fromJson(const Variant& v, SafeWeakRef<T>& _out)
{
	_out.setNull();
}

template <class T>
void Json::fromJson(const Variant& v, SafeWeakRef<T>& _out, const SafeWeakRef<T>& def)
{
	_out = def;
}

template <class T>
Variant Json::toJson(const SafeWeakRef<T>& _in)
{
	return toJson(Ref<T>(_in));
}

SLIB_NAMESPACE_END

#endif
