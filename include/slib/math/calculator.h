/*
 *  Copyright (c) 2008-2018 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef CHECKHEADER_SLIB_MATH_CALCULATOR
#define CHECKHEADER_SLIB_MATH_CALCULATOR

#include "definition.h"

#include "../core/string.h"

namespace slib
{
	
	class SLIB_EXPORT Calculator
	{
	public:
		static sl_reg calculate(sl_int32* result, sl_bool* isDivisionByZero, const sl_char8* sz, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX) noexcept;
		
		static sl_reg calculate(sl_int32* result, sl_bool* isDivisionByZero, const sl_char16* sz, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX) noexcept;

		static sl_bool calculate(const sl_char8* sz, sl_int32* result, sl_bool* isDivisionByZero = sl_null) noexcept;

		static sl_bool calculate(const sl_char16* sz, sl_int32* result, sl_bool* isDivisionByZero = sl_null) noexcept;

		static sl_bool calculate(const String& str, sl_int32* result, sl_bool* isDivisionByZero = sl_null) noexcept;

		static sl_bool calculate(const AtomicString& str, sl_int32* result, sl_bool* isDivisionByZero = sl_null) noexcept;

		static sl_bool calculate(const String16& str, sl_int32* result, sl_bool* isDivisionByZero = sl_null) noexcept;
		
		static sl_bool calculate(const AtomicString16& str, sl_int32* result, sl_bool* isDivisionByZero = sl_null) noexcept;

		static sl_reg calculate(sl_int64* result, sl_bool* isDivisionByZero, const sl_char8* sz, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX) noexcept;
		
		static sl_reg calculate(sl_int64* result, sl_bool* isDivisionByZero, const sl_char16* sz, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX) noexcept;
		
		static sl_bool calculate(const sl_char8* sz, sl_int64* result, sl_bool* isDivisionByZero = sl_null) noexcept;
		
		static sl_bool calculate(const sl_char16* sz, sl_int64* result, sl_bool* isDivisionByZero = sl_null) noexcept;
		
		static sl_bool calculate(const String& str, sl_int64* result, sl_bool* isDivisionByZero = sl_null) noexcept;
		
		static sl_bool calculate(const AtomicString& str, sl_int64* result, sl_bool* isDivisionByZero = sl_null) noexcept;
		
		static sl_bool calculate(const String16& str, sl_int64* result, sl_bool* isDivisionByZero = sl_null) noexcept;
		
		static sl_bool calculate(const AtomicString16& str, sl_int64* result, sl_bool* isDivisionByZero = sl_null) noexcept;

		static sl_reg calculate(float* result, sl_bool* isDivisionByZero, const sl_char8* sz, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX) noexcept;
		
		static sl_reg calculate(float* result, sl_bool* isDivisionByZero, const sl_char16* sz, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX) noexcept;
		
		static sl_bool calculate(const sl_char8* sz, float* result, sl_bool* isDivisionByZero = sl_null) noexcept;
		
		static sl_bool calculate(const sl_char16* sz, float* result, sl_bool* isDivisionByZero = sl_null) noexcept;
		
		static sl_bool calculate(const String& str, float* result, sl_bool* isDivisionByZero = sl_null) noexcept;
		
		static sl_bool calculate(const AtomicString& str, float* result, sl_bool* isDivisionByZero = sl_null) noexcept;
		
		static sl_bool calculate(const String16& str, float* result, sl_bool* isDivisionByZero = sl_null) noexcept;
		
		static sl_bool calculate(const AtomicString16& str, float* result, sl_bool* isDivisionByZero = sl_null) noexcept;

		static sl_reg calculate(double* result, sl_bool* isDivisionByZero, const sl_char8* sz, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX) noexcept;
		
		static sl_reg calculate(double* result, sl_bool* isDivisionByZero, const sl_char16* sz, sl_size posBegin = 0, sl_size posEnd = SLIB_SIZE_MAX) noexcept;		
		
		static sl_bool calculate(const sl_char8* sz, double* result, sl_bool* isDivisionByZero = sl_null) noexcept;
		
		static sl_bool calculate(const sl_char16* sz, double* result, sl_bool* isDivisionByZero = sl_null) noexcept;
		
		static sl_bool calculate(const String& str, double* result, sl_bool* isDivisionByZero = sl_null) noexcept;
		
		static sl_bool calculate(const AtomicString& str, double* result, sl_bool* isDivisionByZero = sl_null) noexcept;
		
		static sl_bool calculate(const String16& str, double* result, sl_bool* isDivisionByZero = sl_null) noexcept;
		
		static sl_bool calculate(const AtomicString16& str, double* result, sl_bool* isDivisionByZero = sl_null) noexcept;

	};
	
}

#endif
