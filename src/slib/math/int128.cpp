#include "../../../inc/slib/math/int128.h"

/*************************************
	Uint128
**************************************/
SLIB_MATH_NAMESPACE_BEGIN

const sl_uint64 Uint128::_zero[2] = { 0, 0 };

sl_uint32 Uint128::getMostSignificantBits() const
{
	if (high != 0) {
		return 64 + Math::getMostSignificantBits(high);
	}
	if (low != 0) {
		return Math::getMostSignificantBits(low);
	}
	return 0;
}

sl_uint32 Uint128::getLeastSignificantBits() const
{
	if (low != 0) {
		return Math::getLeastSignificantBits(low);
	}
	if (high != 0) {
		return 64 + Math::getLeastSignificantBits(high);
	}
	return 0;
}

sl_bool Uint128::div(const Uint128& a, const Uint128& b, Uint128* quotient, Uint128* remainder)
{
	if (b.isZero()) {
		return sl_false;
	}
	if (a < b) {
		if (remainder) {
			*remainder = a;
		}
		if (quotient) {
			quotient->setZero();
		}
		return sl_true;
	}
	sl_uint32 nba = a.getMostSignificantBits();
	sl_uint32 nbb = b.getMostSignificantBits();
	if (nba < nbb) {
		if (remainder) {
			*remainder = a;
		}
		if (quotient) {
			quotient->setZero();
		}
		return sl_true;
	}
	sl_uint32 shift = nba - nbb;
	Uint128 r = a;
	Uint128 t = b;
	t <<= shift;
	Uint128 q;
	q.setZero();
	do {
		if (r >= t) {
			q |= 1;
			r -= t;
		}
		if (shift == 0) {
			break;
		}
		q.shiftLeft();
		t.shiftRight();
		shift--;
	} while (1);
	if (quotient) {
		*quotient = q;
	}
	if (remainder) {
		*remainder = r;
	}
	return sl_true;
}

template <class CT>
sl_int32 _Uint128_parseString(Uint128* out, const CT* sz, sl_uint32 posBegin, sl_uint32 len, sl_uint32 radix)
{
	if (radix < 2 || radix > 64) {
		return SLIB_PARSE_ERROR;
	}
	sl_uint32 pos = posBegin;
	Uint128 m;
	m.setZero();
	const sl_uint8* pattern = radix <= 36 ? _StringConv_radixInversePatternSmall : _StringConv_radixInversePatternBig;
	if (radix == 16) {
		for (; pos < len; pos++) {
			sl_uint32 c = (sl_uint8)(sz[pos]);
			sl_uint32 v = c < 128 ? pattern[c] : 255;
			if (v >= 16) {
				break;
			}
			m <<= 4;
			m |= v;
		}
	} else {
		for (; pos < len; pos++) {
			sl_uint32 c = (sl_uint8)(sz[pos]);
			sl_uint32 v = c < 128 ? pattern[c] : 255;
			if (v >= radix) {
				break;
			}
			m *= radix;
			m += v;
		}
	}
	if (pos == posBegin) {
		return SLIB_PARSE_ERROR;
	}
	if (out) {
		*out = m;
	}
	return pos;
}

sl_int32 Uint128::parseString(Uint128* out, const char* sz, sl_uint32 posBegin, sl_uint32 len, sl_uint32 radix)
{
	return _Uint128_parseString(out, sz, posBegin, len, radix);
}

sl_int32 Uint128::parseString(Uint128* out, const sl_char16* sz, sl_uint32 posBegin, sl_uint32 len, sl_uint32 radix)
{
	return _Uint128_parseString(out, sz, posBegin, len, radix);
}

sl_bool Uint128::parseString(const String& str, sl_uint32 radix)
{
	sl_uint32 n = str.getLength();
	if (n == 0) {
		return sl_false;
	}
	return _Uint128_parseString(this, str.getBuf(), 0, n, radix) == n;
}

Uint128 Uint128::fromString(const String& str, sl_uint32 radix)
{
	Uint128 ret;
	if (ret.parseString(str, radix)) {
		return ret;
	}
	ret.setZero();
	return ret;
}

String Uint128::toString(sl_uint32 radix) const
{
	if (radix < 2 || radix > 64) {
		return String::null();
	}
	Uint128 m = *this;
	if (m.isZero()) {
		SLIB_STATIC_STRING(s, "0");
		return s;
	}
	if (radix == 16) {
		char buf[33];
		buf[32] = 0;
		sl_uint32 posBuf = 32;
		while (m.isNotZero() && posBuf > 0) {
			posBuf--;
			sl_uint32 v = m.low & 15;
			if (v < 10) {
				buf[posBuf] = (sl_char8)(v + 0x30);
			} else {
				buf[posBuf] = (sl_char8)(v + 0x37);
			}
			m >>= 4;
		}
		return String(buf + posBuf, 32 - posBuf);
	} else {
		char buf[129];
		buf[128] = 0;
		sl_uint32 posBuf = 128;
		Uint128 _radix = radix;
		Uint128 r;
		while (m.isNotZero() && posBuf > 0) {
			posBuf--;
			if (div(m, _radix, &m, &r)) {
				sl_uint32 v = (sl_uint32)(r.low);
				if (v < radix) {
					buf[posBuf] = _StringConv_radixPatternUpper[v];
				} else {
					buf[posBuf] = '?';
				}
			} else {
				buf[posBuf] = '?';
			}			
		}
		return String(buf + posBuf, 128 - posBuf);
	}
}

SLIB_MATH_NAMESPACE_END
