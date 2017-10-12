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
		SLIB_INLINE constexpr Uint128() noexcept: high(0), low(0) {}

		SLIB_INLINE constexpr Uint128(const Uint128& other) noexcept: high(other.high), low(other.low) {}

		SLIB_INLINE constexpr Uint128(sl_uint64 num) noexcept: high(0), low(num) {}

	private:
		static const sl_uint64 _zero[2];
	
	public:
		static const Uint128& zero() noexcept;

		SLIB_INLINE constexpr sl_bool isZero() const noexcept
		{
			return high == 0 && low == 0;
		}

		SLIB_INLINE constexpr sl_bool isNotZero() const noexcept
		{
			return high != 0 || low != 0;
		}

		void setZero() noexcept;
	
		int compare(const Uint128& other) const noexcept;

		static sl_bool div(const Uint128& a, const Uint128& b, Uint128* quotient = sl_null, Uint128* remainder = sl_null) noexcept;
	
		Uint128 div(const Uint128& other) const noexcept;

		Uint128 div(sl_uint64 num) const noexcept;

		Uint128 mod(const Uint128& other) const noexcept;

		sl_uint64 mod(sl_uint64 num) const noexcept;

		void shiftRight() noexcept;

		void shiftLeft() noexcept;

		void makeNegative() noexcept;

		void makeBitwiseNot() noexcept;
	
	public:
		Uint128& operator=(const Uint128& other) noexcept;

		Uint128& operator=(sl_uint64 num) noexcept;
	

		sl_bool operator==(const Uint128& other) const noexcept;

		sl_bool operator==(sl_uint64 num) const noexcept;

		friend sl_bool operator==(sl_uint64 num, const Uint128& v) noexcept;

		sl_bool operator!=(const Uint128& other) const noexcept;

		sl_bool operator!=(sl_uint64 num) const noexcept;

		friend sl_bool operator!=(sl_uint64 num, const Uint128& v) noexcept;

		sl_bool operator>=(const Uint128& other) const noexcept;

		sl_bool operator>=(sl_uint64 num) const noexcept;

		friend sl_bool operator>=(sl_uint64 num, const Uint128& v) noexcept;
	
		sl_bool operator<=(const Uint128& other) const noexcept;

		sl_bool operator<=(sl_uint64 num) const noexcept;

		friend sl_bool operator<=(sl_uint64 num, const Uint128& v) noexcept;

		sl_bool operator>(const Uint128& other) const noexcept;

		sl_bool operator>(sl_uint64 num) const noexcept;

		friend sl_bool operator>(sl_uint64 num, const Uint128& v) noexcept;

		sl_bool operator<(const Uint128& other) const noexcept;

		sl_bool operator<(sl_uint64 num) const noexcept;

		friend sl_bool operator<(sl_uint64 num, const Uint128& v) noexcept;
	

		Uint128 operator+(const Uint128& other) const noexcept;

		Uint128 operator+(sl_uint64 num) const noexcept;

		friend Uint128 operator+(sl_uint64 num, const Uint128& v) noexcept;

		Uint128& operator+=(const Uint128& other) noexcept;

		Uint128& operator+=(sl_uint32 num) noexcept;
	
		Uint128& operator++() noexcept;

		Uint128 operator++(int) noexcept;


		Uint128 operator-(const Uint128& other) const noexcept;

		Uint128 operator-(sl_uint64 num) const noexcept;

		friend Uint128 operator-(sl_uint64 num, const Uint128& v) noexcept;
	
		Uint128& operator-=(const Uint128& other) noexcept;

		Uint128& operator-=(sl_uint64 num) noexcept;

		Uint128& operator--() noexcept;

		Uint128 operator--(int) noexcept;


		Uint128 operator*(const Uint128& other) const noexcept;

		Uint128 operator*(sl_uint64 num) const noexcept;

		friend Uint128 operator*(sl_uint64 num, const Uint128& v) noexcept;

		Uint128& operator*=(const Uint128& other) noexcept;

		Uint128& operator*=(sl_uint64 num) noexcept;


		Uint128 operator/(const Uint128& other) const noexcept;

		Uint128 operator/(sl_uint64 num) const noexcept;

		friend Uint128 operator/(sl_uint64 num, const Uint128& v) noexcept;
	
		Uint128& operator/=(const Uint128& other) noexcept;

		Uint128& operator/=(sl_uint64 num) noexcept;
	

		Uint128 operator%(const Uint128& other) const noexcept;

		Uint128 operator%(sl_uint64 num) const noexcept;

		friend Uint128 operator%(sl_uint64 num, const Uint128& v) noexcept;
	
		Uint128& operator%=(const Uint128& other) noexcept;

		Uint128& operator%=(sl_uint64 num) noexcept;
	

		Uint128 operator&(const Uint128& other) const noexcept;

		Uint128 operator&(sl_uint64 num) const noexcept;

		friend Uint128 operator&(sl_uint64 num, const Uint128& v) noexcept;

		Uint128& operator&=(const Uint128& other) noexcept;

		Uint128& operator&=(sl_uint32 num) noexcept;
	

		Uint128 operator|(const Uint128& other) const noexcept;

		Uint128 operator|(sl_uint64 num) const noexcept;

		friend Uint128 operator|(sl_uint64 num, const Uint128& v) noexcept;

		Uint128& operator|=(const Uint128& other) noexcept;

		Uint128& operator|=(sl_uint32 num) noexcept;
	

		Uint128 operator^(const Uint128& other) const noexcept;

		Uint128 operator^(sl_uint64 num) const noexcept;

		friend Uint128 operator^(sl_uint64 num, const Uint128& v) noexcept;
	
		Uint128& operator^=(const Uint128& other) noexcept;

		Uint128& operator^=(sl_uint32 num) noexcept;
	

		Uint128 operator>>(sl_uint32 n) const noexcept;

		Uint128& operator>>=(sl_uint32 n) noexcept;
	

		Uint128 operator<<(sl_uint32 n) const noexcept;

		Uint128& operator<<=(sl_uint32 n) noexcept;
	

		Uint128 operator-() const noexcept;
	
		Uint128 operator~() const noexcept;
	
	public:
		sl_uint32 getMostSignificantBits() const noexcept;

		sl_uint32 getLeastSignificantBits() const noexcept;

		// 16 bytes
		void getBytesBE(void* buf) noexcept;

		// 16 bytes
		void setBytesBE(const void* buf) noexcept;

		// 16 bytes
		void getBytesLE(void* buf) noexcept;
	
		// 16 bytes
		void setBytesLE(const void* buf) noexcept;
	
		static Uint128 fromString(const String& str, sl_uint32 radix = 10) noexcept;

		String toString(sl_uint32 radix = 10) const noexcept;
	
		static Uint128 fromHexString(const String& str) noexcept;

		String toHexString() const noexcept;


		template <class ST>
		static sl_bool parse(const ST& str, Uint128* _out, sl_uint32 radix = 10) noexcept
		{
			return ParseInt(str, _out, radix);
		}

		template <class ST>
		sl_bool parse(const ST& str, sl_uint32 radix = 10) noexcept
		{
			return ParseInt(str, this, radix);
		}
	
	};
	
	template <>
	sl_reg IntParser<Uint128, sl_char8>::parse(Uint128* _out, sl_uint32 radix, const sl_char8 *sz, sl_size posBegin, sl_size posEnd) noexcept;
	
	template <>
	sl_reg IntParser<Uint128, sl_char16>::parse(Uint128* _out, sl_uint32 radix, const sl_char16 *sz, sl_size posBegin, sl_size posEnd) noexcept;
	
	
	template <>
	class Compare<Uint128>
	{
	public:
		int operator()(const Uint128& a, const Uint128& b) const noexcept;
	};
	
	template <>
	class Equals<Uint128>
	{
	public:
		sl_bool operator()(const Uint128& a, const Uint128& b) const noexcept;
	};
	
	template <>
	class Hash<Uint128>
	{
	public:
		sl_size operator()(const Uint128& a) const noexcept;
	};

}

#include "detail/int128.inc"

#endif

