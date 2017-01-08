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
sl_bool Parse(const String8& str, T* _out)
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
sl_bool Parse(const AtomicString8& _str, T* _out)
{
	String8 str(_str);
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
sl_bool ParseInt(const String8& str, T* _out, sl_uint32 radix = 10)
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
sl_bool ParseInt(const AtomicString8& _str, T* _out, sl_uint32 radix = 10)
{
	String8 str(_str);
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

SLIB_NAMESPACE_END

#endif
