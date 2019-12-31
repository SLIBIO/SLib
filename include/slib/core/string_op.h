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

#ifndef CHECKHEADER_SLIB_CORE_STRING_OP
#define CHECKHEADER_SLIB_CORE_STRING_OP

#include "definition.h"

#include "atomic.h"

#ifdef SLIB_SUPPORT_STD_TYPES
#include <string>
#endif

namespace slib
{
	class String;
	class String16;
	class StringView;
	class StringView16;
	class StringParam;

#define PRIV_SLIB_DECLARE_STRING_CLASS_OP_TEMPLATE(RET, FUNC) \
	template <class CHAR, sl_size N> RET FUNC(CHAR (&other)[N]) const noexcept; \
	template <class ARG> RET FUNC(const ARG& other) const noexcept;

#define PRIV_SLIB_DECLARE_STRING_CLASS_OP(STRING, RET, FUNC) \
	RET FUNC(const STRING& other) const noexcept; \
	RET FUNC(const Atomic<STRING>& other) const noexcept; \
	PRIV_SLIB_DECLARE_STRING_CLASS_OP_TEMPLATE(RET, FUNC)

#define PRIV_SLIB_DECLARE_STRING_OP_SUB2(STRING1, STRING2, RET, OP) \
	RET OP(const STRING1& a1, const STRING2& a2) noexcept;
	
#define PRIV_SLIB_DECLARE_STRING_OP_SUB(STRING, RET, OP) \
	PRIV_SLIB_DECLARE_STRING_OP_SUB2(STRING, String, RET, OP) \
	PRIV_SLIB_DECLARE_STRING_OP_SUB2(STRING, Atomic<String>, RET, OP) \
	PRIV_SLIB_DECLARE_STRING_OP_SUB2(STRING, String16, RET, OP) \
	PRIV_SLIB_DECLARE_STRING_OP_SUB2(STRING, Atomic<String16>, RET, OP) \
	PRIV_SLIB_DECLARE_STRING_OP_SUB2(STRING, StringView, RET, OP) \
	PRIV_SLIB_DECLARE_STRING_OP_SUB2(STRING, StringView16, RET, OP) \
	PRIV_SLIB_DECLARE_STRING_OP_SUB2(STRING, StringParam, RET, OP) \
	template <class CHAR, sl_size N> RET OP(const STRING& a1, CHAR (&a2)[N]) noexcept; \
	template <class ARG> RET OP(const STRING& a1, const ARG& a2) noexcept; \
	template <class CHAR, sl_size N> RET OP(CHAR (&a2)[N], const STRING& a1) noexcept; \
	template <class ARG> RET OP(const ARG& a2, const STRING& a1) noexcept;

#define PRIV_SLIB_DECLARE_STRING8_OP(RET, OP) \
	PRIV_SLIB_DECLARE_STRING_OP_SUB(String, RET, OP) \
	PRIV_SLIB_DECLARE_STRING_OP_SUB(Atomic<String>, RET, OP) \
	PRIV_SLIB_DECLARE_STRING_OP_SUB(StringView, RET, OP) \

#define PRIV_SLIB_DECLARE_STRING16_OP(RET, OP) \
	PRIV_SLIB_DECLARE_STRING_OP_SUB(String16, RET, OP) \
	PRIV_SLIB_DECLARE_STRING_OP_SUB(Atomic<String16>, RET, OP) \
	PRIV_SLIB_DECLARE_STRING_OP_SUB(StringView16, RET, OP) \

#define PRIV_SLIB_DECLARE_STRING_OP(RET, OP) \
	PRIV_SLIB_DECLARE_STRING8_OP(RET, OP) \
	PRIV_SLIB_DECLARE_STRING16_OP(RET, OP) \
	PRIV_SLIB_DECLARE_STRING_OP_SUB(StringParam, RET, OP)

	PRIV_SLIB_DECLARE_STRING8_OP(String, operator+)
	PRIV_SLIB_DECLARE_STRING16_OP(String16, operator+)
	PRIV_SLIB_DECLARE_STRING_OP_SUB(StringParam, StringParam, operator+)
	PRIV_SLIB_DECLARE_STRING_OP(sl_bool, operator==)
	PRIV_SLIB_DECLARE_STRING_OP(sl_bool, operator!=)
	PRIV_SLIB_DECLARE_STRING_OP(sl_bool, operator>=)
	PRIV_SLIB_DECLARE_STRING_OP(sl_bool, operator<=)
	PRIV_SLIB_DECLARE_STRING_OP(sl_bool, operator>)
	PRIV_SLIB_DECLARE_STRING_OP(sl_bool, operator<)

}

#endif
