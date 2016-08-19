#ifndef CHECKHEADER_SLIB_CORE_PARSE
#define CHECKHEADER_SLIB_CORE_PARSE

#include "definition.h"

#include "string.h"

SLIB_NAMESPACE_BEGIN

#define SLIB_PARSE_ERROR (sl_reg)(-1)

#define SLIB_DECLARE_PARSE_FUNCTIONS(CLASS) \
public: \
	static sl_reg parse(CLASS* _out, const sl_char8* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX); \
	static sl_reg parse(CLASS* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX); \
	static sl_bool parse(const String8& str, CLASS* _out); \
	static sl_bool parse(const String16& str, CLASS* _out); \
	static sl_bool parse(const SafeString8& str, CLASS* _out); \
	static sl_bool parse(const SafeString16& str, CLASS* _out); \
	static sl_bool parse(const sl_char8* sz, CLASS* _out); \
	static sl_bool parse(const sl_char16* sz, CLASS* _out); \
	sl_bool parse(const String8& str); \
	sl_bool parse(const String16& str); \
	sl_bool parse(const SafeString8& str); \
	sl_bool parse(const SafeString16& str); \
	sl_bool parse(const sl_char8* sz); \
	sl_bool parse(const sl_char16* sz);


#define SLIB_DECLARE_PARSE_FUNCTIONS_ARG(CLASS, ARG_TYPE, ARG_NAME, ARG_DEF) \
public: \
	static sl_reg parse(CLASS* _out, const sl_char8* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX); \
	static sl_reg parse(CLASS* _out, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX); \
	static sl_reg parse(CLASS* _out, ARG_TYPE ARG_NAME, const sl_char8* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX); \
	static sl_reg parse(CLASS* _out, ARG_TYPE ARG_NAME, const sl_char16* sz, sl_size posBegin = 0, sl_size len = SLIB_SIZE_MAX); \
	static sl_bool parse(const String8& str, CLASS* _out, ARG_TYPE ARG_NAME=ARG_DEF); \
	static sl_bool parse(const String16& str, CLASS* _out, ARG_TYPE ARG_NAME=ARG_DEF); \
	static sl_bool parse(const SafeString8& str, CLASS* _out, ARG_TYPE ARG_NAME=ARG_DEF); \
	static sl_bool parse(const SafeString16& str, CLASS* _out, ARG_TYPE ARG_NAME=ARG_DEF); \
	static sl_bool parse(const sl_char8* sz, CLASS* _out, ARG_TYPE ARG_NAME=ARG_DEF); \
	static sl_bool parse(const sl_char16* sz, CLASS* _out, ARG_TYPE ARG_NAME=ARG_DEF); \
	sl_bool parse(const String8& str, ARG_TYPE ARG_NAME=ARG_DEF); \
	sl_bool parse(const String16& str, ARG_TYPE ARG_NAME=ARG_DEF); \
	sl_bool parse(const SafeString8& str, ARG_TYPE ARG_NAME=ARG_DEF); \
	sl_bool parse(const SafeString16& str, ARG_TYPE ARG_NAME=ARG_DEF); \
	sl_bool parse(const sl_char8* sz, ARG_TYPE ARG_NAME=ARG_DEF); \
	sl_bool parse(const sl_char16* sz, ARG_TYPE ARG_NAME=ARG_DEF);

#define SLIB_DEFINE_PARSE_FUNCTIONS(CLASS, TEMPLATE_FUNCTION) \
sl_reg CLASS::parse(CLASS* _out, const sl_char8* sz, sl_size posBegin, sl_size n) \
{ \
	return TEMPLATE_FUNCTION<sl_char8, String8>(_out, sz, posBegin, n); \
} \
sl_reg CLASS::parse(CLASS* _out, const sl_char16* sz, sl_size posBegin, sl_size n) \
{ \
	return TEMPLATE_FUNCTION<sl_char16, String16>(_out, sz, posBegin, n); \
} \
sl_bool CLASS::parse(const String8& s, CLASS* _out) \
{ \
	sl_size n = s.getLength(); \
	if (n == 0) { \
		return sl_false; \
	} \
	return TEMPLATE_FUNCTION<sl_char8, String8>(_out, s.getData(), 0, n) == n; \
} \
sl_bool CLASS::parse(const String16& s, CLASS* _out) \
{ \
	sl_size n = s.getLength(); \
	if (n == 0) { \
		return sl_false; \
	} \
	return TEMPLATE_FUNCTION<sl_char16, String16>(_out, s.getData(), 0, n) == n; \
} \
sl_bool CLASS::parse(const SafeString8& _s, CLASS* _out) \
{ \
	String8 s(_s); \
	sl_size n = s.getLength(); \
	if (n == 0) { \
		return sl_false; \
	} \
	return TEMPLATE_FUNCTION<sl_char8, String8>(_out, s.getData(), 0, n) == n; \
} \
sl_bool CLASS::parse(const SafeString16& _s, CLASS* _out) \
{ \
	String16 s(_s); \
	sl_size n = s.getLength(); \
	if (n == 0) { \
		return sl_false; \
	} \
	return TEMPLATE_FUNCTION<sl_char16, String16>(_out, s.getData(), 0, n) == n; \
} \
sl_bool CLASS::parse(const sl_char8* sz, CLASS* _out) \
{ \
	sl_reg ret = TEMPLATE_FUNCTION<sl_char8, String8>(_out, sz, 0, SLIB_SIZE_MAX); \
	if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) { \
		return sl_true; \
	} \
	return sl_false; \
} \
sl_bool CLASS::parse(const sl_char16* sz, CLASS* _out) \
{ \
	sl_reg ret = TEMPLATE_FUNCTION<sl_char16, String16>(_out, sz, 0, SLIB_SIZE_MAX); \
	if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) { \
		return sl_true; \
	} \
	return sl_false; \
} \
sl_bool CLASS::parse(const String8& s) \
{ \
	sl_size n = s.getLength(); \
	if (n == 0) { \
		return sl_false; \
	} \
	return TEMPLATE_FUNCTION<sl_char8, String8>(this, s.getData(), 0, n) == n; \
} \
sl_bool CLASS::parse(const String16& s) \
{ \
	sl_size n = s.getLength(); \
	if (n == 0) { \
		return sl_false; \
	} \
	return TEMPLATE_FUNCTION<sl_char16, String16>(this, s.getData(), 0, n) == n; \
} \
sl_bool CLASS::parse(const SafeString8& _s) \
{ \
	String8 s(_s); \
	sl_size n = s.getLength(); \
	if (n == 0) { \
		return sl_false; \
	} \
	return TEMPLATE_FUNCTION<sl_char8, String8>(this, s.getData(), 0, n) == n; \
} \
sl_bool CLASS::parse(const SafeString16& _s) \
{ \
	String16 s(_s); \
	sl_size n = s.getLength(); \
	if (n == 0) { \
		return sl_false; \
	} \
	return TEMPLATE_FUNCTION<sl_char16, String16>(this, s.getData(), 0, n) == n; \
} \
sl_bool CLASS::parse(const sl_char8* sz) \
{ \
	sl_reg ret = TEMPLATE_FUNCTION<sl_char8, String8>(this, sz, 0, SLIB_SIZE_MAX); \
	if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) { \
		return sl_true; \
	} \
	return sl_false; \
} \
sl_bool CLASS::parse(const sl_char16* sz) \
{ \
	sl_reg ret = TEMPLATE_FUNCTION<sl_char16, String16>(this, sz, 0, SLIB_SIZE_MAX); \
	if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) { \
		return sl_true; \
	} \
	return sl_false; \
}


#define SLIB_DEFINE_PARSE_FUNCTIONS_ARG(CLASS, TEMPLATE_FUNCTION, ARG_TYPE, ARG_NAME, ARG_DEF) \
sl_reg CLASS::parse(CLASS* _out, const sl_char8* sz, sl_size posBegin, sl_size n) \
{ \
	return TEMPLATE_FUNCTION<sl_char8, String8>(_out, sz, posBegin, n, ARG_DEF); \
} \
sl_reg CLASS::parse(CLASS* _out, const sl_char16* sz, sl_size posBegin, sl_size n) \
{ \
	return TEMPLATE_FUNCTION<sl_char16, String16>(_out, sz, posBegin, n, ARG_DEF); \
} \
sl_reg CLASS::parse(CLASS* _out, ARG_TYPE ARG_NAME, const sl_char8* sz, sl_size posBegin, sl_size n) \
{ \
	return TEMPLATE_FUNCTION<sl_char8, String8>(_out, sz, posBegin, n, ARG_NAME); \
} \
sl_reg CLASS::parse(CLASS* _out, ARG_TYPE ARG_NAME, const sl_char16* sz, sl_size posBegin, sl_size n) \
{ \
	return TEMPLATE_FUNCTION<sl_char16, String16>(_out, sz, posBegin, n, ARG_NAME); \
} \
sl_bool CLASS::parse(const String8& s, CLASS* _out, ARG_TYPE ARG_NAME) \
{ \
	sl_size n = s.getLength(); \
	if (n == 0) { \
		return sl_false; \
	} \
	return TEMPLATE_FUNCTION<sl_char8, String8>(_out, s.getData(), 0, n, ARG_NAME) == n; \
} \
sl_bool CLASS::parse(const String16& s, CLASS* _out, ARG_TYPE ARG_NAME) \
{ \
	sl_size n = s.getLength(); \
	if (n == 0) { \
		return sl_false; \
	} \
	return TEMPLATE_FUNCTION<sl_char16, String16>(_out, s.getData(), 0, n, ARG_NAME) == n; \
} \
sl_bool CLASS::parse(const SafeString8& _s, CLASS* _out, ARG_TYPE ARG_NAME) \
{ \
	String8 s(_s); \
	sl_size n = s.getLength(); \
	if (n == 0) { \
		return sl_false; \
	} \
	return TEMPLATE_FUNCTION<sl_char8, String8>(_out, s.getData(), 0, n, ARG_NAME) == n; \
} \
sl_bool CLASS::parse(const SafeString16& _s, CLASS* _out, ARG_TYPE ARG_NAME) \
{ \
	String16 s(_s); \
	sl_size n = s.getLength(); \
	if (n == 0) { \
		return sl_false; \
	} \
	return TEMPLATE_FUNCTION<sl_char16, String16>(_out, s.getData(), 0, n, ARG_NAME) == n; \
} \
sl_bool CLASS::parse(const sl_char8* sz, CLASS* _out, ARG_TYPE ARG_NAME) \
{ \
	sl_reg ret = TEMPLATE_FUNCTION<sl_char8, String8>(_out, sz, 0, SLIB_SIZE_MAX, ARG_NAME); \
	if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) { \
		return sl_true; \
	} \
	return sl_false; \
} \
sl_bool CLASS::parse(const sl_char16* sz, CLASS* _out, ARG_TYPE ARG_NAME) \
{ \
	sl_reg ret = TEMPLATE_FUNCTION<sl_char16, String16>(_out, sz, 0, SLIB_SIZE_MAX, ARG_NAME); \
	if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) { \
		return sl_true; \
	} \
	return sl_false; \
} \
sl_bool CLASS::parse(const String8& s, ARG_TYPE ARG_NAME) \
{ \
	sl_size n = s.getLength(); \
	if (n == 0) { \
		return sl_false; \
	} \
	return TEMPLATE_FUNCTION<sl_char8, String8>(this, s.getData(), 0, n, ARG_NAME) == n; \
} \
sl_bool CLASS::parse(const String16& s, ARG_TYPE ARG_NAME) \
{ \
	sl_size n = s.getLength(); \
	if (n == 0) { \
		return sl_false; \
	} \
	return TEMPLATE_FUNCTION<sl_char16, String16>(this, s.getData(), 0, n, ARG_NAME) == n; \
} \
sl_bool CLASS::parse(const SafeString8& _s, ARG_TYPE ARG_NAME) \
{ \
	String8 s(_s); \
	sl_size n = s.getLength(); \
	if (n == 0) { \
		return sl_false; \
	} \
	return TEMPLATE_FUNCTION<sl_char8, String8>(this, s.getData(), 0, n, ARG_NAME) == n; \
} \
sl_bool CLASS::parse(const SafeString16& _s, ARG_TYPE ARG_NAME) \
{ \
	String16 s(_s); \
	sl_size n = s.getLength(); \
	if (n == 0) { \
		return sl_false; \
	} \
	return TEMPLATE_FUNCTION<sl_char16, String16>(this, s.getData(), 0, n, ARG_NAME) == n; \
} \
sl_bool CLASS::parse(const sl_char8* sz, ARG_TYPE ARG_NAME) \
{ \
	sl_reg ret = TEMPLATE_FUNCTION<sl_char8, String8>(this, sz, 0, SLIB_SIZE_MAX, ARG_NAME); \
	if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) { \
		return sl_true; \
	} \
	return sl_false; \
} \
sl_bool CLASS::parse(const sl_char16* sz, ARG_TYPE ARG_NAME) \
{ \
	sl_reg ret = TEMPLATE_FUNCTION<sl_char16, String16>(this, sz, 0, SLIB_SIZE_MAX, ARG_NAME); \
	if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) { \
		return sl_true; \
	} \
	return sl_false; \
}


SLIB_NAMESPACE_END

#endif
