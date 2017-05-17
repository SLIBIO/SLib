/*
 *  Copyright (c) 2008-2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MATH_INT128
#define CHECKHEADER_SLIB_MATH_INT128

#include "definition.h"

#include "../core/compare.h"
#include "../core/hash.h"
#include "../core/string.h"
#include "../core/parse.h"

namespace slib
{
	
	class SLIB_EXPORT Uint128
	{
	public:
		sl_uint64 high;
		sl_uint64 low;
	
	public:
		constexpr Uint128(): high(0), low(0) {}

		constexpr Uint128(const Uint128& num): high(num.high), low(num.low) {}

		constexpr Uint128(sl_uint64 num): high(0), low(num) {}

	private:
		static const sl_uint64 _zero[2];
	
	public:
		static const Uint128& zero();

		constexpr sl_bool isZero() const
		{
			return high == 0 && low == 0;
		}

		constexpr sl_bool isNotZero() const
		{
			return high != 0 || low != 0;
		}

		void setZero();
	
		int compare(const Uint128& other) const;

		static sl_bool div(const Uint128& a, const Uint128& b, Uint128* quotient = sl_null, Uint128* remainder = sl_null);
	
		Uint128 div(const Uint128& other) const;

		Uint128 div(sl_uint64 num) const;

		Uint128 mod(const Uint128& other) const;

		sl_uint64 mod(sl_uint64 num) const;

		void shiftRight();

		void shiftLeft();

		void makeNegative();

		void makeBitwiseNot();
	
	public:
		Uint128& operator=(const Uint128& other);

		Uint128& operator=(sl_uint64 num);
	

		sl_bool operator==(const Uint128& other) const;

		sl_bool operator==(sl_uint64 num) const;

		friend sl_bool operator==(sl_uint64 num, const Uint128& v);

		sl_bool operator!=(const Uint128& other) const;

		sl_bool operator!=(sl_uint64 num) const;

		friend sl_bool operator!=(sl_uint64 num, const Uint128& v);

		sl_bool operator>=(const Uint128& other) const;

		sl_bool operator>=(sl_uint64 num) const;

		friend sl_bool operator>=(sl_uint64 num, const Uint128& v);
	
		sl_bool operator<=(const Uint128& other) const;

		sl_bool operator<=(sl_uint64 num) const;

		friend sl_bool operator<=(sl_uint64 num, const Uint128& v);

		sl_bool operator>(const Uint128& other) const;

		sl_bool operator>(sl_uint64 num) const;

		friend sl_bool operator>(sl_uint64 num, const Uint128& v);

		sl_bool operator<(const Uint128& other) const;

		sl_bool operator<(sl_uint64 num) const;

		friend sl_bool operator<(sl_uint64 num, const Uint128& v);
	

		Uint128 operator+(const Uint128& other) const;

		Uint128 operator+(sl_uint64 num) const;

		friend Uint128 operator+(sl_uint64 num, const Uint128& v);

		Uint128& operator+=(const Uint128& other);

		Uint128& operator+=(sl_uint32 num);
	
		Uint128& operator++();

		Uint128 operator++(int);


		Uint128 operator-(const Uint128& other) const;

		Uint128 operator-(sl_uint64 num) const;

		friend Uint128 operator-(sl_uint64 num, const Uint128& v);
	
		Uint128& operator-=(const Uint128& other);

		Uint128& operator-=(sl_uint64 num);

		Uint128& operator--();

		Uint128 operator--(int);


		Uint128 operator*(const Uint128& other) const;

		Uint128 operator*(sl_uint64 num) const;

		friend Uint128 operator*(sl_uint64 num, const Uint128& v);

		Uint128& operator*=(const Uint128& other);

		Uint128& operator*=(sl_uint64 num);


		Uint128 operator/(const Uint128& other) const;

		Uint128 operator/(sl_uint64 num) const;

		friend Uint128 operator/(sl_uint64 num, const Uint128& v);
	
		Uint128& operator/=(const Uint128& other);

		Uint128& operator/=(sl_uint64 num);
	

		Uint128 operator%(const Uint128& other) const;

		Uint128 operator%(sl_uint64 num) const;

		friend Uint128 operator%(sl_uint64 num, const Uint128& v);
	
		Uint128& operator%=(const Uint128& other);

		Uint128& operator%=(sl_uint64 num);
	

		Uint128 operator&(const Uint128& other) const;

		Uint128 operator&(sl_uint64 num) const;

		friend Uint128 operator&(sl_uint64 num, const Uint128& v);

		Uint128& operator&=(const Uint128& other);

		Uint128& operator&=(sl_uint32 num);
	

		Uint128 operator|(const Uint128& other) const;

		Uint128 operator|(sl_uint64 num) const;

		friend Uint128 operator|(sl_uint64 num, const Uint128& v);

		Uint128& operator|=(const Uint128& other);

		Uint128& operator|=(sl_uint32 num);
	

		Uint128 operator^(const Uint128& other) const;

		Uint128 operator^(sl_uint64 num) const;

		friend Uint128 operator^(sl_uint64 num, const Uint128& v);
	
		Uint128& operator^=(const Uint128& other);

		Uint128& operator^=(sl_uint32 num);
	

		Uint128 operator>>(sl_uint32 n) const;

		Uint128& operator>>=(sl_uint32 n);
	

		Uint128 operator<<(sl_uint32 n) const;

		Uint128& operator<<=(sl_uint32 n);
	

		Uint128 operator-() const;
	
		Uint128 operator~() const;
	
	public:
		sl_uint32 getMostSignificantBits() const;

		sl_uint32 getLeastSignificantBits() const;

		// 16 bytes
		void getBytesBE(void* buf);

		// 16 bytes
		void setBytesBE(const void* buf);

		// 16 bytes
		void getBytesLE(void* buf);
	
		// 16 bytes
		void setBytesLE(const void* buf);
	
		static Uint128 fromString(const String& str, sl_uint32 radix = 10);

		String toString(sl_uint32 radix = 10) const;
	
		static Uint128 fromHexString(const String& str);

		String toHexString() const;


		template <class ST>
		static sl_bool parse(const ST& str, Uint128* _out, sl_uint32 radix = 10)
		{
			return ParseInt(str, _out, radix);
		}

		template <class ST>
		sl_bool parse(const ST& str, sl_uint32 radix = 10)
		{
			return ParseInt(str, this, radix);
		}
	
	};
	
	template <>
	sl_reg IntParser<Uint128, sl_char8>::parse(Uint128* _out, sl_uint32 radix, const sl_char8 *sz, sl_size posBegin, sl_size posEnd);
	
	template <>
	sl_reg IntParser<Uint128, sl_char16>::parse(Uint128* _out, sl_uint32 radix, const sl_char16 *sz, sl_size posBegin, sl_size posEnd);
	
	
	template <>
	class Compare<Uint128>
	{
	public:
		int operator()(const Uint128& a, const Uint128& b) const;
	};
	
	template <>
	class Equals<Uint128>
	{
	public:
		sl_bool operator()(const Uint128& a, const Uint128& b) const;
	};
	
	template <>
	class Hash<Uint128>
	{
	public:
		sl_uint32 operator()(const Uint128& a) const;
	};

}

#include "detail/int128.inc"

#endif

