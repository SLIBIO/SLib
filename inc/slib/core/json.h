#ifndef CHECKHEADER_SLIB_CORE_JSON
#define CHECKHEADER_SLIB_CORE_JSON

#include "definition.h"

#include "variant.h"

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
	
	template <> void Json::fromJson(const Variant& v, Variant& _out);
	template <> void Json::fromJson(const Variant& v, Variant& _out, const Variant& def);
	template <> Variant Json::toJson(const Variant& _in);
	
	template <> void Json::fromJson(const Variant& v, AtomicVariant& _out);
	template <> void Json::fromJson(const Variant& v, AtomicVariant& _out, const AtomicVariant& def);
	template <> Variant Json::toJson(const AtomicVariant& _in);
	
	template <> void Json::fromJson(const Variant& v, char& _out);
	template <> void Json::fromJson(const Variant& v, char& _out, const char& def);
	template <> Variant Json::toJson(const char& _in);
	
	template <> void Json::fromJson(const Variant& v, unsigned char& _out);
	template <> void Json::fromJson(const Variant& v, unsigned char& _out, const unsigned char& def);
	template <> Variant Json::toJson(const unsigned char& _in);
	
	template <> void Json::fromJson(const Variant& v, short& _out);
	template <> void Json::fromJson(const Variant& v, short& _out, const short& def);
	template <> Variant Json::toJson(const short& _in);
	
	template <> void Json::fromJson(const Variant& v, unsigned short& _out);
	template <> void Json::fromJson(const Variant& v, unsigned short& _out, const unsigned short& def);
	template <> Variant Json::toJson(const unsigned short& _in);

	template <> void Json::fromJson(const Variant& v, int& _out);
	template <> void Json::fromJson(const Variant& v, int& _out, const int& def);
	template <> Variant Json::toJson(const int& _in);
	
	template <> void Json::fromJson(const Variant& v, unsigned int& _out);
	template <> void Json::fromJson(const Variant& v, unsigned int& _out, const unsigned int& def);
	template <> Variant Json::toJson(const unsigned int& _in);
	
	template <> void Json::fromJson(const Variant& v, long& _out);
	template <> void Json::fromJson(const Variant& v, long& _out, const long& def);
	template <> Variant Json::toJson(const long& _in);
	
	template <> void Json::fromJson(const Variant& v, unsigned long& _out);
	template <> void Json::fromJson(const Variant& v, unsigned long& _out, const unsigned long& def);
	template <> Variant Json::toJson(const unsigned long& _in);

	template <> void Json::fromJson(const Variant& v, sl_int64& _out);
	template <> void Json::fromJson(const Variant& v, sl_int64& _out, const sl_int64& def);
	template <> Variant Json::toJson(const sl_int64& _in);
	
	template <> void Json::fromJson(const Variant& v, sl_uint64& _out);
	template <> void Json::fromJson(const Variant& v, sl_uint64& _out, const sl_uint64& def);
	template <> Variant Json::toJson(const sl_uint64& _in);
	
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
}

#include "detail/json.h"

#endif
