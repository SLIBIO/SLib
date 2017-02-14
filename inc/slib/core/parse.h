#ifndef CHECKHEADER_SLIB_CORE_PARSE
#define CHECKHEADER_SLIB_CORE_PARSE

#include "definition.h"

#include "string.h"

#define SLIB_PARSE_ERROR (-1)

namespace slib
{

	template <class ObjectType, class CharType>
	class Parser
	{
	public:
		static sl_reg parse(ObjectType* _out, CharType const* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);

	};

	template <class ObjectType, class CharType>
	class IntParser
	{
	public:
		static sl_reg parse(ObjectType* _out, sl_uint32 radix, CharType const* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX);

	};


	template <class T>
	sl_bool Parse(const String& str, T* _out);

	template <class T>
	sl_bool Parse(const String16& str, T* _out);
	
	template <class T>
	sl_bool Parse(const AtomicString& _str, T* _out);

	template <class T>
	sl_bool Parse(const AtomicString16& _str, T* _out);
	
	template <class T>
	sl_bool Parse(const sl_char8* sz, T* _out);

	template <class T>
	sl_bool Parse(const sl_char16* sz, T* _out);


	template <class T>
	sl_bool ParseInt(const String& str, T* _out, sl_uint32 radix = 10);

	template <class T>
	sl_bool ParseInt(const String16& str, T* _out, sl_uint32 radix = 10);

	template <class T>
	sl_bool ParseInt(const AtomicString& _str, T* _out, sl_uint32 radix = 10);

	template <class T>
	sl_bool ParseInt(const AtomicString16& _str, T* _out, sl_uint32 radix = 10);

	template <class T>
	sl_bool ParseInt(const sl_char8* sz, T* _out, sl_uint32 radix = 10);

	template <class T>
	sl_bool ParseInt(const sl_char16* sz, T* _out, sl_uint32 radix = 10);


	class ParseUtil
	{
	public:
		static String applyBackslashEscapes(const String& str, sl_bool flagDoubleQuote = sl_true, sl_bool flagAddQuote = sl_true, sl_bool flagEscapeNonAscii = sl_false);

		static String16 applyBackslashEscapes(const String16& str, sl_bool flagDoubleQuote = sl_true, sl_bool flagAddQuote = sl_true, sl_bool flagEscapeNonAscii = sl_false);

		static String applyBackslashEscapes(const AtomicString& str, sl_bool flagDoubleQuote = sl_true, sl_bool flagAddQuote = sl_true, sl_bool flagEscapeNonAscii = sl_false);

		static String16 applyBackslashEscapes(const AtomicString16& str, sl_bool flagDoubleQuote = sl_true, sl_bool flagAddQuote = sl_true, sl_bool flagEscapeNonAscii = sl_false);

		static String parseBackslashEscapes(const sl_char8* input, sl_size len, sl_size* lengthParsed = sl_null, sl_bool* flagError = sl_null);

		static String16 parseBackslashEscapes(const sl_char16* input, sl_size len, sl_size* lengthParsed = sl_null, sl_bool* flagError = sl_null);

		static String parseBackslashEscapes(const String& str, sl_size* lengthParsed = sl_null, sl_bool* flagError = sl_null);

		static String16 parseBackslashEscapes(const String16& str, sl_size* lengthParsed = sl_null, sl_bool* flagError = sl_null);

		static String parseBackslashEscapes(const AtomicString& str, sl_size* lengthParsed = sl_null, sl_bool* flagError = sl_null);

		static String16 parseBackslashEscapes(const AtomicString16& str, sl_size* lengthParsed = sl_null, sl_bool* flagError = sl_null);

		static sl_size countLineNumber(const sl_char8* input, sl_size len, sl_size* columnLast = sl_null);

		static sl_size countLineNumber(const sl_char16* input, sl_size len, sl_size* columnLast = sl_null);

		static sl_size countLineNumber(const String& str, sl_size pos, sl_size* column = sl_null);

		static sl_size countLineNumber(const String16& str, sl_size pos, sl_size* column = sl_null);

		static sl_size countLineNumber(const AtomicString& str, sl_size pos, sl_size* column = sl_null);

		static sl_size countLineNumber(const AtomicString16& str, sl_size pos, sl_size* column = sl_null);

	};

}

#include "detail/parse.h"

#endif
