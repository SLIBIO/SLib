#ifndef CHECKHEADER_SLIB_CORE_PARSE
#define CHECKHEADER_SLIB_CORE_PARSE

#include "definition.h"

#include "string.h"

SLIB_NAMESPACE_BEGIN

#define SLIB_PARSE_ERROR (-1)

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
sl_bool Parse(const String& str, T* _out)
{
	sl_size n = str.getLength();
	if (n == 0) {
		return sl_false;
	}
	return Parser<T, sl_char8>::parse(_out, str.getData(), 0, n) == n;
}

template <class T>
sl_bool Parse(const String16& str, T* _out)
{
	sl_size n = str.getLength();
	if (n == 0) {
		return sl_false;
	}
	return Parser<T, sl_char16>::parse(_out, str.getData(), 0, n) == n;
}

template <class T>
sl_bool Parse(const AtomicString& _str, T* _out)
{
	String str(_str);
	sl_size n = str.getLength();
	if (n == 0) {
		return sl_false;
	}
	return Parser<T, sl_char8>::parse(_out, str.getData(), 0, n) == n;
}

template <class T>
sl_bool Parse(const AtomicString16& _str, T* _out)
{
	String16 str(_str);
	sl_size n = str.getLength();
	if (n == 0) {
		return sl_false;
	}
	return Parser<T, sl_char16>::parse(_out, str.getData(), 0, n) == n;
}

template <class T>
sl_bool Parse(const sl_char8* sz, T* _out)
{
	sl_reg ret = Parser<T, sl_char8>::parse(_out, sz, 0, SLIB_SIZE_MAX);
	if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) {
		return sl_true;
	}
	return sl_false;
}

template <class T>
sl_bool Parse(const sl_char16* sz, T* _out)
{
	sl_reg ret = Parser<T, sl_char16>::parse(_out, sz, 0, SLIB_SIZE_MAX);
	if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) {
		return sl_true;
	}
	return sl_false;
}


template <class T>
sl_bool ParseInt(const String& str, T* _out, sl_uint32 radix = 10)
{
	sl_size n = str.getLength();
	if (n == 0) {
		return sl_false;
	}
	return IntParser<T, sl_char8>::parse(_out, radix, str.getData(), 0, n) == n;
}

template <class T>
sl_bool ParseInt(const String16& str, T* _out, sl_uint32 radix = 10)
{
	sl_size n = str.getLength();
	if (n == 0) {
		return sl_false;
	}
	return IntParser<T, sl_char16>::parse(_out, radix, str.getData(), 0, n) == n;
}

template <class T>
sl_bool ParseInt(const AtomicString& _str, T* _out, sl_uint32 radix = 10)
{
	String str(_str);
	sl_size n = str.getLength();
	if (n == 0) {
		return sl_false;
	}
	return IntParser<T, sl_char8>::parse(_out, radix, str.getData(), 0, n) == n;
}

template <class T>
sl_bool ParseInt(const AtomicString16& _str, T* _out, sl_uint32 radix = 10)
{
	String16 str(_str);
	sl_size n = str.getLength();
	if (n == 0) {
		return sl_false;
	}
	return IntParser<T, sl_char16>::parse(_out, radix, str.getData(), 0, n) == n;
}

template <class T>
sl_bool ParseInt(const sl_char8* sz, T* _out, sl_uint32 radix = 10)
{
	sl_reg ret = IntParser<T, sl_char8>::parse(_out, radix, sz, 0, SLIB_SIZE_MAX);
	if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) {
		return sl_true;
	}
	return sl_false;
}

template <class T>
sl_bool ParseInt(const sl_char16* sz, T* _out, sl_uint32 radix = 10)
{
	sl_reg ret = IntParser<T, sl_char16>::parse(_out, radix, sz, 0, SLIB_SIZE_MAX);
	if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) {
		return sl_true;
	}
	return sl_false;
}


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

SLIB_NAMESPACE_END

#endif
