/*
 *  Copyright (c) 2008-2018 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "slib/math/calculator.h"

#include "slib/core/math.h"
#include "slib/core/parse.h"

namespace slib
{
	
	template <class CT>
	SLIB_INLINE sl_reg _priv_Calculator_parseNumber(sl_int32& result, const CT* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return StringTypeFromCharType<CT>::Type::parseInt32(10, &result, sz, posBegin, posEnd);
	}
	
	template <class CT>
	SLIB_INLINE sl_reg _priv_Calculator_parseNumber(sl_int64& result, const CT* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return StringTypeFromCharType<CT>::Type::parseInt64(10, &result, sz, posBegin, posEnd);
	}
	
	template <class CT>
	SLIB_INLINE sl_reg _priv_Calculator_parseNumber(float& result, const CT* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return StringTypeFromCharType<CT>::Type::parseFloat(&result, sz, posBegin, posEnd);
	}
	
	template <class CT>
	SLIB_INLINE sl_reg _priv_Calculator_parseNumber(double& result, const CT* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return StringTypeFromCharType<CT>::Type::parseDouble(&result, sz, posBegin, posEnd);
	}

	template <class RT, class CT>
	static sl_reg _priv_Calculator_calculate(RT* result, sl_bool* isDivisionByZero, const CT* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		sl_bool flagDivisionByZero = sl_false;
		RT accum1 = 0;
		RT accum2 = 0;
		RT value = 0;
		CT opAccum1 = 0;
		CT opAccum2 = 0;
		sl_size pos = posBegin;
		if (pos >= posEnd) {
			return SLIB_PARSE_ERROR;
		}
		for (;;) {
			while (SLIB_CHAR_IS_WHITE_SPACE(sz[pos])) {
				pos++;
				if (pos >= posEnd) {
					return SLIB_PARSE_ERROR;
				}
			}
			sl_bool flagMinusParentheses = sl_false;
			if (sz[pos] == '-') {
				sl_size pos2 = pos + 1;
				if (pos2 >= posEnd) {
					return SLIB_PARSE_ERROR;
				}
				while (SLIB_CHAR_IS_WHITE_SPACE(sz[pos2])) {
					pos2++;
					if (pos2 >= posEnd) {
						return SLIB_PARSE_ERROR;
					}
				}
				if (sz[pos2] == '(') {
					pos = pos2;
					flagMinusParentheses = sl_true;
				}
			}
			if (sz[pos] == '(') {
				pos++;
				sl_bool flagDivisionByZeroInner;
				pos = _priv_Calculator_calculate(&value, &flagDivisionByZeroInner, sz, pos, posEnd);
				if (pos == SLIB_PARSE_ERROR) {
					return SLIB_PARSE_ERROR;
				}
				if (flagDivisionByZeroInner) {
					flagDivisionByZero = sl_true;
					value = 0;
				}
				if (pos >= posEnd) {
					return SLIB_PARSE_ERROR;
				}
				while (SLIB_CHAR_IS_WHITE_SPACE(sz[pos])) {
					pos++;
					if (pos >= posEnd) {
						return SLIB_PARSE_ERROR;
					}
				}
				if (sz[pos] != ')') {
					return SLIB_PARSE_ERROR;
				}
				if (flagMinusParentheses) {
					value = -value;
				}
				pos++;
			} else {
				pos = _priv_Calculator_parseNumber(value, sz, pos, posEnd);
				if (pos == SLIB_PARSE_ERROR) {
					return SLIB_PARSE_ERROR;
				}
			}
			if (opAccum2) {
				if (opAccum2 == '*') {
					accum2 *= value;
				} else if (opAccum2 == '/') {
					if (Math::isAlmostZero(value)) {
						accum2 = 0;
						flagDivisionByZero = sl_true;
					} else {
						accum2 /= value;
					}
				} else {
					return SLIB_PARSE_ERROR;
				}
			} else {
				accum2 = value;
			}
			if (pos < posEnd) {
				while (SLIB_CHAR_IS_WHITE_SPACE(sz[pos])) {
					pos++;
					if (pos >= posEnd) {
						break;
					}
				}
			}
			if (pos < posEnd && (sz[pos] == '*' || sz[pos] == '/')) {
				opAccum2 = sz[pos];
				pos++;
			} else {
				if (opAccum1) {
					if (opAccum1 == '+') {
						accum1 += accum2;
					} else if (opAccum1 == '-') {
						accum1 -= accum2;
					} else {
						return SLIB_PARSE_ERROR;
					}
				} else {
					accum1 = accum2;
				}
				if (pos < posEnd && (sz[pos] == '+' || sz[pos] == '-')) {
					opAccum2 = 0;
					opAccum1 = sz[pos];
					pos++;
				} else {
					if (flagDivisionByZero) {
						accum1 = 0;
					}
					if (isDivisionByZero) {
						*isDivisionByZero = flagDivisionByZero;
					}
					if (result) {
						*result = accum1;
					}
					return pos;
				}
			}
		}
		return SLIB_PARSE_ERROR;
	}

	sl_reg Calculator::calculate(sl_int32* result, sl_bool* isDivisionByZero, const sl_char8* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_Calculator_calculate<sl_int32, sl_char8>(result, isDivisionByZero, sz, posBegin, posEnd);
	}

	sl_reg Calculator::calculate(sl_int32* result, sl_bool* isDivisionByZero, const sl_char16* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_Calculator_calculate<sl_int32, sl_char16>(result, isDivisionByZero, sz, posBegin, posEnd);
	}
	
	sl_bool Calculator::calculate(const sl_char8* sz, sl_int32* result, sl_bool* isDivisionByZero) noexcept
	{
		sl_reg ret = _priv_Calculator_calculate<sl_int32, sl_char8>(result, isDivisionByZero, sz, 0, SLIB_SIZE_MAX);
		if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) {
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool Calculator::calculate(const sl_char16* sz, sl_int32* result, sl_bool* isDivisionByZero) noexcept
	{
		sl_reg ret = _priv_Calculator_calculate<sl_int32, sl_char16>(result, isDivisionByZero, sz, 0, SLIB_SIZE_MAX);
		if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) {
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool Calculator::calculate(const String& str, sl_int32* result, sl_bool* isDivisionByZero) noexcept
	{
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_Calculator_calculate<sl_int32, sl_char8>(result, isDivisionByZero, str.getData(), 0, n) == (sl_reg)n;
	}
	
	sl_bool Calculator::calculate(const AtomicString& _str, sl_int32* result, sl_bool* isDivisionByZero) noexcept
	{
		String str(_str);
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_Calculator_calculate<sl_int32, sl_char8>(result, isDivisionByZero, str.getData(), 0, n) == (sl_reg)n;
	}
	
	sl_bool Calculator::calculate(const String16& str, sl_int32* result, sl_bool* isDivisionByZero) noexcept
	{
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_Calculator_calculate<sl_int32, sl_char16>(result, isDivisionByZero, str.getData(), 0, n) == (sl_reg)n;
	}
	
	sl_bool Calculator::calculate(const AtomicString16& _str, sl_int32* result, sl_bool* isDivisionByZero) noexcept
	{
		String16 str(_str);
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_Calculator_calculate<sl_int32, sl_char16>(result, isDivisionByZero, str.getData(), 0, n) == (sl_reg)n;
	}
	
	sl_reg Calculator::calculate(sl_int64* result, sl_bool* isDivisionByZero, const sl_char8* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_Calculator_calculate<sl_int64, sl_char8>(result, isDivisionByZero, sz, posBegin, posEnd);
	}
	
	sl_reg Calculator::calculate(sl_int64* result, sl_bool* isDivisionByZero, const sl_char16* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_Calculator_calculate<sl_int64, sl_char16>(result, isDivisionByZero, sz, posBegin, posEnd);
	}
	
	sl_bool Calculator::calculate(const sl_char8* sz, sl_int64* result, sl_bool* isDivisionByZero) noexcept
	{
		sl_reg ret = _priv_Calculator_calculate<sl_int64, sl_char8>(result, isDivisionByZero, sz, 0, SLIB_SIZE_MAX);
		if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) {
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool Calculator::calculate(const sl_char16* sz, sl_int64* result, sl_bool* isDivisionByZero) noexcept
	{
		sl_reg ret = _priv_Calculator_calculate<sl_int64, sl_char16>(result, isDivisionByZero, sz, 0, SLIB_SIZE_MAX);
		if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) {
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool Calculator::calculate(const String& str, sl_int64* result, sl_bool* isDivisionByZero) noexcept
	{
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_Calculator_calculate<sl_int64, sl_char8>(result, isDivisionByZero, str.getData(), 0, n) == (sl_reg)n;
	}
	
	sl_bool Calculator::calculate(const AtomicString& _str, sl_int64* result, sl_bool* isDivisionByZero) noexcept
	{
		String str(_str);
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_Calculator_calculate<sl_int64, sl_char8>(result, isDivisionByZero, str.getData(), 0, n) == (sl_reg)n;
	}
	
	sl_bool Calculator::calculate(const String16& str, sl_int64* result, sl_bool* isDivisionByZero) noexcept
	{
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_Calculator_calculate<sl_int64, sl_char16>(result, isDivisionByZero, str.getData(), 0, n) == (sl_reg)n;
	}
	
	sl_bool Calculator::calculate(const AtomicString16& _str, sl_int64* result, sl_bool* isDivisionByZero) noexcept
	{
		String16 str(_str);
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_Calculator_calculate<sl_int64, sl_char16>(result, isDivisionByZero, str.getData(), 0, n) == (sl_reg)n;
	}
	
	sl_reg Calculator::calculate(float* result, sl_bool* isDivisionByZero, const sl_char8* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_Calculator_calculate<float, sl_char8>(result, isDivisionByZero, sz, posBegin, posEnd);
	}
	
	sl_reg Calculator::calculate(float* result, sl_bool* isDivisionByZero, const sl_char16* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_Calculator_calculate<float, sl_char16>(result, isDivisionByZero, sz, posBegin, posEnd);
	}
	
	sl_bool Calculator::calculate(const sl_char8* sz, float* result, sl_bool* isDivisionByZero) noexcept
	{
		sl_reg ret = _priv_Calculator_calculate<float, sl_char8>(result, isDivisionByZero, sz, 0, SLIB_SIZE_MAX);
		if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) {
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool Calculator::calculate(const sl_char16* sz, float* result, sl_bool* isDivisionByZero) noexcept
	{
		sl_reg ret = _priv_Calculator_calculate<float, sl_char16>(result, isDivisionByZero, sz, 0, SLIB_SIZE_MAX);
		if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) {
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool Calculator::calculate(const String& str, float* result, sl_bool* isDivisionByZero) noexcept
	{
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_Calculator_calculate<float, sl_char8>(result, isDivisionByZero, str.getData(), 0, n) == (sl_reg)n;
	}
	
	sl_bool Calculator::calculate(const AtomicString& _str, float* result, sl_bool* isDivisionByZero) noexcept
	{
		String str(_str);
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_Calculator_calculate<float, sl_char8>(result, isDivisionByZero, str.getData(), 0, n) == (sl_reg)n;
	}
	
	sl_bool Calculator::calculate(const String16& str, float* result, sl_bool* isDivisionByZero) noexcept
	{
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_Calculator_calculate<float, sl_char16>(result, isDivisionByZero, str.getData(), 0, n) == (sl_reg)n;
	}
	
	sl_bool Calculator::calculate(const AtomicString16& _str, float* result, sl_bool* isDivisionByZero) noexcept
	{
		String16 str(_str);
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_Calculator_calculate<float, sl_char16>(result, isDivisionByZero, str.getData(), 0, n) == (sl_reg)n;
	}
	
	sl_reg Calculator::calculate(double* result, sl_bool* isDivisionByZero, const sl_char8* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_Calculator_calculate<double, sl_char8>(result, isDivisionByZero, sz, posBegin, posEnd);
	}
	
	sl_reg Calculator::calculate(double* result, sl_bool* isDivisionByZero, const sl_char16* sz, sl_size posBegin, sl_size posEnd) noexcept
	{
		return _priv_Calculator_calculate<double, sl_char16>(result, isDivisionByZero, sz, posBegin, posEnd);
	}

	sl_bool Calculator::calculate(const sl_char8* sz, double* result, sl_bool* isDivisionByZero) noexcept
	{
		sl_reg ret = _priv_Calculator_calculate<double, sl_char8>(result, isDivisionByZero, sz, 0, SLIB_SIZE_MAX);
		if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) {
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool Calculator::calculate(const sl_char16* sz, double* result, sl_bool* isDivisionByZero) noexcept
	{
		sl_reg ret = _priv_Calculator_calculate<double, sl_char16>(result, isDivisionByZero, sz, 0, SLIB_SIZE_MAX);
		if (ret != SLIB_PARSE_ERROR && sz[ret] == 0) {
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool Calculator::calculate(const String& str, double* result, sl_bool* isDivisionByZero) noexcept
	{
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_Calculator_calculate<double, sl_char8>(result, isDivisionByZero, str.getData(), 0, n) == (sl_reg)n;
	}
	
	sl_bool Calculator::calculate(const AtomicString& _str, double* result, sl_bool* isDivisionByZero) noexcept
	{
		String str(_str);
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_Calculator_calculate<double, sl_char8>(result, isDivisionByZero, str.getData(), 0, n) == (sl_reg)n;
	}
	
	sl_bool Calculator::calculate(const String16& str, double* result, sl_bool* isDivisionByZero) noexcept
	{
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_Calculator_calculate<double, sl_char16>(result, isDivisionByZero, str.getData(), 0, n) == (sl_reg)n;
	}
	
	sl_bool Calculator::calculate(const AtomicString16& _str, double* result, sl_bool* isDivisionByZero) noexcept
	{
		String16 str(_str);
		sl_size n = str.getLength();
		if (n == 0) {
			return sl_false;
		}
		return _priv_Calculator_calculate<double, sl_char16>(result, isDivisionByZero, str.getData(), 0, n) == (sl_reg)n;
	}
	
}
