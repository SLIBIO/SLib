/*
 *   Copyright (c) 2008-2018 SLIBIO <https://github.com/SLIBIO>
 *
 *   Permission is hereby granted, free of charge, to any person obtaining a copy
 *   of this software and associated documentation files (the "Software"), to deal
 *   in the Software without restriction, including without limitation the rights
 *   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *   copies of the Software, and to permit persons to whom the Software is
 *   furnished to do so, subject to the following conditions:
 *
 *   The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *   THE SOFTWARE.
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
