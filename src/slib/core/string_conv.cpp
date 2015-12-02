#include "../../../inc/slib/core/string.h"
#include "../../../inc/slib/core/math.h"

SLIB_NAMESPACE_BEGIN

#define MAX_NUMBER_STR_LEN 256
#define MAX_PRECISION 50

const char _string_conv_radix_pattern_lower[65] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ@_";
const char* _StringConv_radixPatternLower = _string_conv_radix_pattern_lower;
const char _string_conv_radix_pattern_upper[65] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz@_";
const char* _StringConv_radixPatternUpper = _string_conv_radix_pattern_upper;

const sl_uint8 _string_conv_radix_inverse_pattern_big[128] = {
	/*00*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*08*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*10*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*18*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*20*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*28*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*30*/ 0,   1,   2,   3,   4,   5,   6,   7,
	/*38*/ 8,   9,   255, 255, 255, 255, 255, 255,
	/*40*/ 62,  36,  37,  38,  39,  40,  41,  42,
	/*48*/ 43,  44,  45,  46,  47,  48,  49,  50,
	/*50*/ 51,  52,  53,  54,  55,  56,  57,  58,
	/*58*/ 59,  60,  61,  255, 255, 255, 255, 63,
	/*60*/ 255, 10,  11,  12,  13,  14,  15,  16,
	/*68*/ 17,  18,  19,  20,  21,  22,  23,  24,
	/*70*/ 25,  26,  27,  28,  29,  30,  31,  32,
	/*78*/ 33,  34,  35,  255, 255, 255, 255, 255
};
const sl_uint8* _StringConv_radixInversePatternBig = _string_conv_radix_inverse_pattern_big;

const sl_uint8 _string_conv_radix_inverse_pattern_small[128] = {
	/*00*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*08*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*10*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*18*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*20*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*28*/ 255, 255, 255, 255, 255, 255, 255, 255,
	/*30*/ 0,   1,   2,   3,   4,   5,   6,   7,
	/*38*/ 8,   9,   255, 255, 255, 255, 255, 255,
	/*40*/ 255, 10,  11,  12,  13,  14,  15,  16,
	/*48*/ 17,  18,  19,  20,  21,  22,  23,  24,
	/*50*/ 25,  26,  27,  28,  29,  30,  31,  32,
	/*58*/ 33,  34,  35,  255, 255, 255, 255, 255,
	/*60*/ 255, 10,  11,  12,  13,  14,  15,  16,
	/*68*/ 17,  18,  19,  20,  21,  22,  23,  24,
	/*70*/ 25,  26,  27,  28,  29,  30,  31,  32,
	/*78*/ 33,  34,  35,  255, 255, 255, 255, 255
};
const sl_uint8* _StringConv_radixInversePatternSmall = _string_conv_radix_inverse_pattern_small;

template <class IT, class ST, class CT>
SLIB_INLINE ST _String_fromInt(IT value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
	if (radix < 2 || radix > 64) {
		return ST::null();
	}
	const char* pattern = flagUpperCase && radix <= 36 ? _string_conv_radix_pattern_upper : _string_conv_radix_pattern_lower;
	CT buf[MAX_NUMBER_STR_LEN];
	sl_int32 pos = MAX_NUMBER_STR_LEN - 1;
	buf[pos] = 0;
	if (minWidth < 1) {
		minWidth = 1;
	}

	sl_bool flagMinus = sl_false;
	if (value < 0) {
		value = -value;
		flagMinus = sl_true;
	}
	while (value || minWidth > 0) {
		pos--;
		buf[pos] = pattern[value % radix];
		value = value / radix;
		minWidth--;
	}
	if (flagMinus) {
		pos--;
		buf[pos] = '-';
	}
	
	ST ret(buf + pos);
	return ret;
}

template <class IT, class ST, class CT>
SLIB_INLINE ST _String_fromUint(IT value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
	if (radix < 2 || radix > 64) {
		return ST::null();
	}
	const char* pattern = flagUpperCase && radix <= 36 ? _string_conv_radix_pattern_upper : _string_conv_radix_pattern_lower;
	CT buf[MAX_NUMBER_STR_LEN];
	sl_int32 pos = MAX_NUMBER_STR_LEN - 1;
	buf[pos] = 0;
	if (minWidth < 1) {
		minWidth = 1;
	}
	while (value || minWidth > 0) {
		pos--;
		buf[pos] = pattern[value % radix];
		value = value / radix;
		minWidth--;
	}
	ST ret(buf + pos);
	return ret;
}

String8 String8::fromInt32(sl_int32 value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
	return _String_fromInt<sl_int32, String8, sl_char8>(value, radix, minWidth, flagUpperCase);
}

String16 String16::fromInt32(sl_int32 value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
	return _String_fromInt<sl_int32, String16, sl_char16>(value, radix, minWidth, flagUpperCase);
}

String8 String8::fromUint32(sl_uint32 value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
	return _String_fromUint<sl_uint32, String8, sl_char8>(value, radix, minWidth, flagUpperCase);
}

String16 String16::fromUint32(sl_uint32 value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
	return _String_fromUint<sl_uint32, String16, sl_char16>(value, radix, minWidth, flagUpperCase);
}

String8 String8::fromInt64(sl_int64 value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
	return _String_fromInt<sl_int64, String8, sl_char8>(value, radix, minWidth, flagUpperCase);
}

String16 String16::fromInt64(sl_int64 value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
	return _String_fromInt<sl_int64, String16, sl_char16>(value, radix, minWidth, flagUpperCase);
}

String8 String8::fromUint64(sl_uint64 value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
	return _String_fromUint<sl_uint64, String8, sl_char8>(value, radix, minWidth, flagUpperCase);
}

String16 String16::fromUint64(sl_uint64 value, sl_int32 radix, sl_int32 minWidth, sl_bool flagUpperCase)
{
	return _String_fromUint<sl_uint64, String16, sl_char16>(value, radix, minWidth, flagUpperCase);
}

template <class FT, class ST, class CT>
SLIB_INLINE ST _String_fromFloat(FT value, sl_int32 precision, sl_bool flagZeroPadding, sl_int32 minWidthIntegral) {
	
	CT buf[MAX_NUMBER_STR_LEN];
	
	if (Math::isNaN(value)) {
		SLIB_SAFE_STATIC(ST, s, "NaN");
		return s;
	}
	if (Math::isInfinite(value)) {
		SLIB_SAFE_STATIC(ST, s, "Infinite");
		return s;
	}
	if (value == 0) {
		if (precision == 0) {
			if (minWidthIntegral > MAX_PRECISION - 1) {
				minWidthIntegral = MAX_PRECISION - 1;
			}
			for (sl_int32 i = 0; i < minWidthIntegral; i++) {
				buf[i] = '0';
			}
			buf[minWidthIntegral] = 0;
			return ST(buf);
		} else {
			if (minWidthIntegral > MAX_PRECISION - 1) {
				minWidthIntegral = MAX_PRECISION - 1;
			}
			sl_int32 i = 0;
			for (i = 0; i < minWidthIntegral; i++) {
				buf[i] = '0';
			}
			buf[i++] = '.';
			if (precision > 0 && flagZeroPadding) {
				if (precision > MAX_PRECISION) {
					precision = MAX_PRECISION;
				}
				for (sl_int32 k = 0; k < precision; k++) {
					buf[i++] = '0';
				}
			} else {
				buf[i++] = '0';
			}
			buf[i] = 0;
			return ST(buf);
		}
	}
	
	CT* str = buf;
	CT* str_last = buf + MAX_NUMBER_STR_LEN - 10;
	
	sl_int32 flagMinus;
	if (value < 0) {
		flagMinus = 1;
		value = -value;
	} else {
		flagMinus = 0;
	}
	
	FT min_value;
	if (precision < 0) {
		min_value = 0;
	} else {
		min_value = Math::pow((FT)10, (FT)(-precision));
	}
	value += min_value / 2;
	if (flagZeroPadding) {
		min_value = 0;
	}

	if (flagMinus) {
		*(str++) = '-';
	}
	sl_int32 nExp = 0;

	sl_int32 nInt = (sl_int32)(Math::log10(value));
	FT weight = Math::pow((FT)10, (FT)(nInt));
	if (nInt >= 15) { // use exp
		value = value / weight;
		nExp = nInt;
		nInt = 0;
		weight = 1;
	}
	if (nInt < -15) { // use exp
		value = value / weight * 10;
		nExp = nInt - 1;
		nInt = 0;
		weight = 1;
	}
	
	if (precision < 0) {
		precision = 15 - nInt;
		if (precision < 0) {
			precision = 0;
		}
		if (precision > 50) {
			precision = 50;
		}
		min_value = Math::pow((FT)10, (FT)(-precision));
		value += min_value / 3;
	}
	if (precision == 0) {
		flagZeroPadding = 0;
	}
	if (nInt < minWidthIntegral - 1) {
		nInt = minWidthIntegral - 1;
		weight = Math::pow((FT)10, (FT)nInt);
	}
	while (str < str_last && nInt >= -precision && (nInt >= 0 || value >= min_value)) {
		if (nInt == -1) {
			if (value >= min_value) {
				*(str++) = '.';
			} else {
				break;
			}
		}
		if (weight > 0 && !Math::isInfinite(weight)) {
			sl_int32 digit = (sl_int32)(value / weight);
			value -= (digit * weight);
			*(str++) = (CT)('0' + digit);
		}
		nInt--;
		weight /= 10;
	}
	
	if (nExp) {
		*(str++) = 'e';
		if (nExp > 0) {
			*(str++) = '+';
		} else {
			*(str++) = '-';
			nExp = -nExp;
		}
		CT* t1 = str;
		while (nExp > 0 && str < str_last) {
			*(str++) = (CT)('0' + (nExp % 10));
			nExp /= 10;
		}
		CT* t2 = str - 1;
		while (t1 < t2) {
			CT chTemp = *t1;
			*t1 = *t2;
			*t2 = chTemp;
			t1++;
			t2--;
		}
	}
	*str = 0;
	
	return buf;
}

String8 String8::fromDouble(double value, sl_int32 precision, sl_bool flagZeroPadding, sl_int32 minWidthIntegral) {
	return _String_fromFloat<double, String8, sl_char8>(value, precision, flagZeroPadding, minWidthIntegral);
}

String8 String8::fromFloat(float value, sl_int32 precision, sl_bool flagZeroPadding, sl_int32 minWidthIntegral)
{
	return _String_fromFloat<float, String8, sl_char8>(value, precision, flagZeroPadding, minWidthIntegral);
}

String16 String16::fromDouble(double value, sl_int32 precision, sl_bool flagZeroPadding, sl_int32 minWidthIntegral) {
	return _String_fromFloat<double, String16, sl_char16>(value, precision, flagZeroPadding, minWidthIntegral);
}

String16 String16::fromFloat(float value, sl_int32 precision, sl_bool flagZeroPadding, sl_int32 minWidthIntegral)
{
	return _String_fromFloat<float, String16, sl_char16>(value, precision, flagZeroPadding, minWidthIntegral);
}

template <class ST, class CT>
SLIB_INLINE ST _String_makeHexString(const void* buf, sl_int32 size)
{
	if (!buf || size <= 0) {
		return ST::null();
	}
	ST str = ST::memory(size * 2);
	if (str.isEmpty()) {
		return str;
	}
	CT* sz = (CT*)(str.getBuf());
	for (sl_int32 i = 0; i < size; i++) {
		sl_uint8 v = ((sl_uint8*)(buf))[i];
		sz[i << 1] = _string_conv_radix_pattern_lower[v >> 4];
		sz[(i << 1) + 1] = _string_conv_radix_pattern_lower[v & 15];
	}
	return str;
}

String8 String8::makeHexString(const void* buf, sl_int32 size)
{
	return _String_makeHexString<String8, sl_char8>(buf, size);
}

String16 String16::makeHexString(const void* buf, sl_int32 size)
{
	return _String_makeHexString<String16, sl_char16>(buf, size);
}

template <class IT, class CT>
SLIB_INLINE sl_int32 _String_parseInt(const CT* sz, sl_uint32 i, sl_uint32 n, IT* out, sl_int32 radix)
{
	if (i >= n) {
		return SLIB_PARSE_ERROR;
	}
	sl_int32 r = radix;

	const sl_uint8* pattern = r <= 36 ? _string_conv_radix_inverse_pattern_small : _string_conv_radix_inverse_pattern_big;
	IT v = 0;
	sl_bool bMinus = sl_false;
	sl_bool bEmpty = sl_true;
	if (sz[i] == '-') {
		i++;
		bMinus = sl_true;
	}
	for (; i < n; i++) {
		sl_int32 c = (sl_uint32)(sz[i]);
		if (c != '\t' && c != ' ') {
			break;
		}
	}
	for (; i < n; i++) {
		sl_int32 c = (sl_uint32)(sz[i]);
		sl_int32 m = c < 128 ? pattern[c] : 255;
		if (m < r) {
			v = v * r + m;
			bEmpty = sl_false;
		} else {
			break;
		}
	}
	if (bEmpty) {
		return SLIB_PARSE_ERROR;
	}
	if (bMinus) {
		v = -v;
	}
	if (out) {
		*out = v;
	}
	return i;
}

template <class IT, class CT>
SLIB_INLINE sl_int32 _String_parseUint(const CT* sz, sl_uint32 i, sl_uint32 n, IT* out, sl_int32 radix)
{
	if (i >= n) {
		return SLIB_PARSE_ERROR;
	}
	sl_uint32 r = radix;

	const sl_uint8* pattern = r <= 36 ? _string_conv_radix_inverse_pattern_small : _string_conv_radix_inverse_pattern_big;
	IT v = 0;
	for (; i < n; i++) {
		sl_int32 c = (sl_uint32)(sz[i]);
		sl_uint32 m = c < 128 ? pattern[c] : 255;
		if (m < r) {
			v = v * r + m;
		} else {
			break;
		}
	}
	if (out) {
		*out = v;
	}
	return i;
}

sl_int32 String8::parseInt32(sl_int32* out, const char* sz, sl_uint32 posBegin, sl_uint32 len, sl_int32 radix)
{
	return _String_parseInt(sz, posBegin, len, out, radix);
}

sl_int32 String8::parseInt32(sl_int32* out, const sl_char16* sz, sl_uint32 posBegin, sl_uint32 len, sl_int32 radix)
{
	return _String_parseInt(sz, posBegin, len, out, radix);
}

sl_bool String8::parseInt32(sl_int32* out, sl_int32 radix) const
{
	String8 s = *this;
	sl_uint32 n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseInt(s.getBuf(), 0, n, out, radix) == n;
}

sl_int32 String16::parseInt32(sl_int32* out, const char* sz, sl_uint32 posBegin, sl_uint32 len, sl_int32 radix)
{
	return _String_parseInt(sz, posBegin, len, out, radix);
}

sl_int32 String16::parseInt32(sl_int32* out, const sl_char16* sz, sl_uint32 posBegin, sl_uint32 len, sl_int32 radix)
{
	return _String_parseInt(sz, posBegin, len, out, radix);
}

sl_bool String16::parseInt32(sl_int32* out, sl_int32 radix) const
{
	String16 s = *this;
	sl_uint32 n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseInt(s.getBuf(), 0, n, out, radix) == n;
}

sl_int32 String8::parseUint32(sl_uint32* out, const char* sz, sl_uint32 posBegin, sl_uint32 len, sl_int32 radix)
{
	return _String_parseUint(sz, posBegin, len, out, radix);
}

sl_int32 String8::parseUint32(sl_uint32* out, const sl_char16* sz, sl_uint32 posBegin, sl_uint32 len, sl_int32 radix)
{
	return _String_parseUint(sz, posBegin, len, out, radix);
}

sl_bool String8::parseUint32(sl_uint32* out, sl_int32 radix) const
{
	String8 s = *this;
	sl_uint32 n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseUint(s.getBuf(), 0, n, out, radix) == n;
}

sl_int32 String16::parseUint32(sl_uint32* out, const char* sz, sl_uint32 posBegin, sl_uint32 n, sl_int32 radix)
{
	return _String_parseUint(sz, posBegin, n, out, radix);
}

sl_int32 String16::parseUint32(sl_uint32* out, const sl_char16* sz, sl_uint32 posBegin, sl_uint32 n, sl_int32 radix)
{
	return _String_parseUint(sz, posBegin, n, out, radix);
}

sl_bool String16::parseUint32(sl_uint32* out, sl_int32 radix) const
{
	String16 s = *this;
	sl_uint32 n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseUint(s.getBuf(), 0, n, out, radix) == n;
}

sl_int32 String8::parseInt64(sl_int64* out, const char* sz, sl_uint32 posBegin, sl_uint32 len, sl_int32 radix)
{
	return _String_parseInt(sz, posBegin, len, out, radix);
}

sl_int32 String8::parseInt64(sl_int64* out, const sl_char16* sz, sl_uint32 posBegin, sl_uint32 len, sl_int32 radix)
{
	return _String_parseInt(sz, posBegin, len, out, radix);
}

sl_bool String8::parseInt64(sl_int64* out, sl_int32 radix) const
{
	String8 s = *this;
	sl_uint32 n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseInt(s.getBuf(), 0, n, out, radix) == n;
}

sl_int32 String16::parseInt64(sl_int64* out, const char* sz, sl_uint32 posBegin, sl_uint32 len, sl_int32 radix)
{
	return _String_parseInt(sz, posBegin, len, out, radix);
}

sl_int32 String16::parseInt64(sl_int64* out, const sl_char16* sz, sl_uint32 posBegin, sl_uint32 len, sl_int32 radix)
{
	return _String_parseInt(sz, posBegin, len, out, radix);
}

sl_bool String16::parseInt64(sl_int64* out, sl_int32 radix) const
{
	String16 s = *this;
	sl_uint32 n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseInt(s.getBuf(), 0, n, out, radix) == n;
}

sl_int32 String8::parseUint64(sl_uint64* out, const char* sz, sl_uint32 posBegin, sl_uint32 len, sl_int32 radix)
{
	return _String_parseUint(sz, posBegin, len, out, radix);
}

sl_int32 String8::parseUint64(sl_uint64* out, const sl_char16* sz, sl_uint32 posBegin, sl_uint32 len, sl_int32 radix)
{
	return _String_parseUint(sz, posBegin, len, out, radix);
}

sl_bool String8::parseUint64(sl_uint64* out, sl_int32 radix) const
{
	String8 s = *this;
	sl_uint32 n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseUint(s.getBuf(), 0, n, out, radix) == n;
}

sl_int32 String16::parseUint64(sl_uint64* out, const char* sz, sl_uint32 posBegin, sl_uint32 len, sl_int32 radix)
{
	return _String_parseUint(sz, posBegin, len, out, radix);
}

sl_int32 String16::parseUint64(sl_uint64* out, const sl_char16* sz, sl_uint32 posBegin, sl_uint32 len, sl_int32 radix)
{
	return _String_parseUint(sz, posBegin, len, out, radix);
}

sl_bool String16::parseUint64(sl_uint64* out, sl_int32 radix) const
{
	String16 s = *this;
	sl_uint32 n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseUint(s.getBuf(), 0, n, out, radix) == n;
}

template <class FT, class CT>
SLIB_INLINE sl_int32 _String_parseFloat(const CT* sz, sl_uint32 i, sl_uint32 n, FT* out)
{
	if (i >= n) {
		return SLIB_PARSE_ERROR; // input string is empty
	}

	FT v = 0;
	sl_bool bMinus = sl_false;
	sl_bool bEmpty = sl_true;

	if (sz[i] == '-') {
		i++;
		bMinus = sl_true;
	}
	for (; i < n; i++) {
		sl_int32 c = (sl_uint32)(sz[i]);
		if (c != '\t' && c != ' ') {
			break;
		}
	}
	for (; i < n; i++) {
		sl_int32 c = (sl_uint32)sz[i];
		if (c >= '0' && c <= '9') {
			v = v * 10 + (c - '0');
			bEmpty = sl_false;
		} else {
			break;
		}
	}
	if (bEmpty) {
		return SLIB_PARSE_ERROR; // integral number is required
	}
	if (i < n) {
		if (sz[i] == '.') {
			i++;
			bEmpty = sl_true;
			FT weight = (FT)(0.1);
			for (; i < n; i++) {
				sl_int32 c = (sl_uint32)sz[i];
				if (c >= '0' && c <= '9') {
					v = v + (c - '0') * weight;
					weight /= 10;
					bEmpty = sl_false;
				} else {
					break;
				}
			}
			if (bEmpty) {
				return SLIB_PARSE_ERROR; // fraction number is required
			}
		}
		if (i < n) {
			if (sz[i] == 'e' || sz[i] == 'E') {
				i++;
				bEmpty = sl_true;
				sl_bool bMinuxExp = sl_false;
				FT exp = 0;
				if (i < n && (sz[i] == '+' || sz[i] == '-')) {
					if (sz[i] == '-') {
						bMinuxExp = sl_true;
					}
					i++;
				}
				for (; i < n; i++) {
					sl_int32 c = (sl_uint32)sz[i];
					if (c >= '0' && c <= '9') {
						exp = exp * 10 + (c - '0');
						bEmpty = sl_false;
					} else {
						break; // invalid character
					}
				}
				if (bEmpty) {
					return SLIB_PARSE_ERROR; // exponent number is required 
				}
				if (bMinuxExp) {
					exp = -exp;
				}
				v *= Math::pow((FT)(10.0), exp);
			}
		}
	}
	if (bMinus) {
		v = -v;
	}
	if (out) {
		*out = v;
	}
	return i;
}

sl_int32 String8::parseFloat(float* out, const char* sz, sl_uint32 posBegin, sl_uint32 len)
{
	return _String_parseFloat(sz, posBegin, len, out);
}

sl_int32 String8::parseFloat(float* out, const sl_char16* sz, sl_uint32 posBegin, sl_uint32 len)
{
	return _String_parseFloat(sz, posBegin, len, out);
}

sl_bool String8::parseFloat(float* out) const
{
	String8 s = *this;
	sl_uint32 n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseFloat(s.getBuf(), 0, n, out) == n;
}

sl_int32 String16::parseFloat(float* out, const char* sz, sl_uint32 posBegin, sl_uint32 len)
{
	return _String_parseFloat(sz, posBegin, len, out);
}

sl_int32 String16::parseFloat(float* out, const sl_char16* sz, sl_uint32 posBegin, sl_uint32 len)
{
	return _String_parseFloat(sz, posBegin, len, out);
}

sl_bool String16::parseFloat(float* out) const
{
	String16 s = *this;
	sl_uint32 n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseFloat(s.getBuf(), 0, n, out) == n;
}

sl_int32 String8::parseDouble(double* out, const char* sz, sl_uint32 posBegin, sl_uint32 n)
{
	return _String_parseFloat(sz, posBegin, n, out);
}

sl_int32 String8::parseDouble(double* out, const sl_char16* sz, sl_uint32 posBegin, sl_uint32 n)
{
	return _String_parseFloat(sz, posBegin, n, out);
}

sl_bool String8::parseDouble(double* out) const
{
	String8 s = *this;
	sl_uint32 n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseFloat(s.getBuf(), 0, n, out) == n;
}

sl_int32 String16::parseDouble(double* out, const char* sz, sl_uint32 posBegin, sl_uint32 n)
{
	return _String_parseFloat(sz, posBegin, n, out);
}

sl_int32 String16::parseDouble(double* out, const sl_char16* sz, sl_uint32 posBegin, sl_uint32 n)
{
	return _String_parseFloat(sz, posBegin, n, out);
}

sl_bool String16::parseDouble(double* out) const
{
	String16 s = *this;
	sl_uint32 n = s.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _String_parseFloat(s.getBuf(), 0, n, out) == n;
}

template <class CT>
SLIB_INLINE sl_int32 _String_parseHexString(const CT* sz, sl_uint32 i, sl_uint32 n, void* out)
{
	if (i >= n) {
		return SLIB_PARSE_ERROR;
	}
	sl_uint8* buf = (sl_uint8*)(out);
	sl_uint32 k = 0;
	for (; i < n; i += 2) {
		sl_uint32 v1, v2;
		{
			sl_uint32 ch = (sl_uint32)sz[i];
			if (ch >= '0' && ch <= '9') {
				v1 = ch - '0';
			} else if (ch >= 'A' && ch <= 'F') {
				v1 = ch - 'A' + 10;
			} else if (ch >= 'a' && ch <= 'f') {
				v1 = ch - 'a' + 10;
			} else {
				break;
			}
		}
		{
			sl_uint32 ch = (sl_uint32)sz[i + 1];
			if (ch >= '0' && ch <= '9') {
				v2 = ch - '0';
			} else if (ch >= 'A' && ch <= 'F') {
				v2 = ch - 'A' + 10;
			} else if (ch >= 'a' && ch <= 'f') {
				v2 = ch - 'a' + 10;
			} else {
				break;
			}
		}
		buf[k++] = (sl_uint8)((v1 << 4) | v2);
	}
	return i;
}

sl_int32 String8::parseHexString(void* out, const char* sz, sl_uint32 posBegin, sl_uint32 len)
{
	return _String_parseHexString(sz, posBegin, len, out);
}

sl_int32 String8::parseHexString(void* out, const sl_char16* sz, sl_uint32 posBegin, sl_uint32 len)
{
	return _String_parseHexString(sz, posBegin, len, out);
}

sl_bool String8::parseHexString(void* out) const
{
	String8 s = *this;
	sl_uint32 n = 0;
	if (n == 0) {
		return sl_false;
	}
	return _String_parseHexString(s.getBuf(), 0, n, out) == n;
}

sl_int32 String16::parseHexString(void* out, const char* sz, sl_uint32 posBegin, sl_uint32 len)
{
	return _String_parseHexString(sz, posBegin, len, out);
}

sl_int32 String16::parseHexString(void* out, const sl_char16* sz, sl_uint32 posBegin, sl_uint32 len)
{
	return _String_parseHexString(sz, posBegin, len, out);
}

sl_bool String16::parseHexString(void* out) const
{
	String16 s = *this;
	sl_uint32 n = 0;
	if (n == 0) {
		return sl_false;
	}
	return _String_parseHexString(s.getBuf(), 0, n, out) == n;
}

SLIB_NAMESPACE_END
