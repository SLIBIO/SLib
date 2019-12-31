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

namespace slib
{

	namespace priv
	{
		namespace string
		{

			template <class STRING>
			class PlusOperator
			{
			public:
				typedef STRING StringType;
				typedef STRING ReturnType;

			public:
				static STRING call(const STRING& s1, const STRING& s2) noexcept
				{
					sl_size n2 = s2.getLength();
					if (n2) {
						sl_size n1 = s1.getLength();
						if (n1) {
							return STRING::merge(s1.getData(), n1, s2.getData(), n2);
						} else {
							return s2.getNotNull();
						}
					} else {
						return s1.getNotNull();
					}
				}

				template <class STRING_ARG, class CHAR2>
				static STRING call(const STRING_ARG& s1, const CHAR2* sz2, sl_reg len2) noexcept
				{
					if (sz2 && len2) {
						sl_size n = s1.getLength();
						if (n) {
							return STRING::merge(s1.getData(), n, sz2, len2);
						} else {
							return STRING::create(sz2, len2);
						}
					} else {
						return s1;
					}
				}

				template <class CHAR2, class STRING_ARG>
				static STRING call(const CHAR2* sz1, sl_reg len1, const STRING_ARG& s2) noexcept
				{
					if (sz1 && len1) {
						sl_size n = s2.getLength();
						if (n) {
							return STRING::merge(sz1, len1, s2.getData(), n);
						} else {
							return STRING::create(sz1, len1);
						}
					} else {
						return s2;
					}
				}

				template <class STRING_ARG, class T>
				SLIB_INLINE static STRING call(const STRING_ARG& s1, const T& s2) noexcept
				{
					return s1 + STRING::from(s2);
				}

				template <class T, class STRING_ARG>
				SLIB_INLINE static STRING call_inverse(const T& s1, const STRING_ARG& s2) noexcept
				{
					return STRING::from(s1) + s2;
				}

			};
			
			template <class STRING>
			class EqualsOperator
			{
			public:
				typedef STRING StringType;
				typedef sl_bool ReturnType;

			public:
				SLIB_INLINE static sl_bool call(const STRING& s1, const STRING& s2) noexcept
				{
					return s1.equals(s2);
				}

				template <class STRING_ARG, class CHAR2>
				SLIB_INLINE static sl_bool call(const STRING_ARG& s1, const CHAR2* sz2, sl_reg len2) noexcept
				{
					if (sz2 && len2) {
						return STRING::equals(s1.getData(), s1.getLength(), sz2, len2);
					} else {
						return s1.isEmpty();
					}
				}

			};
			
			template <class STRING>
			class CompareOperator
			{
			public:
				typedef STRING StringType;
				typedef sl_compare_result ReturnType;

			public:
				SLIB_INLINE static sl_compare_result call(const STRING& s1, const STRING& s2) noexcept
				{
					return s1.compare(s2);
				}

				template <class STRING_ARG, class CHAR2>
				SLIB_INLINE static sl_compare_result call(const STRING_ARG& s1, const CHAR2* sz2, sl_reg len2) noexcept
				{
					if (sz2 && len2) {
						return STRING::compare(s1.getData(), s1.getLength(), sz2, len2);
					} else {
						return s1.isEmpty() ? 0 : 1;
					}
				}

			};
			
			template <class STRING, class OP>
			class OperatorInvoker
			{
			public:
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const String& a2) noexcept
				{
					return OP::call(a1, a2.getData(), a2.getLength());
				}

				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const AtomicString& _a2) noexcept
				{
					String a2(_a2);
					return OP::call(a1, a2.getData(), a2.getLength());
				}

				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const String16& a2) noexcept
				{
					return OP::call(a1, a2.getData(), a2.getLength());
				}

				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const AtomicString16& _a2) noexcept
				{
					String16 a2(_a2);
					return OP::call(a1, a2.getData(), a2.getLength());
				}

				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const StringView& a2) noexcept
				{
					return OP::call(a1, a2.getData(), a2.getLength());
				}

				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const StringView16& a2) noexcept
				{
					return OP::call(a1, a2.getData(), a2.getLength());
				}

				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const StringParam& _a2) noexcept
				{
					if (_a2.isNull()) {
						return OP::call(a1, (sl_char8*)sl_null, 0);
					} else if (_a2.is16()) {
						StringData16 a2(_a2);
						return OP::call(a1, a2.getData(), a2.getLength());
					} else {
						StringData a2(_a2);
						return OP::call(a1, a2.getData(), a2.getLength());
					}
				}

				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const char* a2) noexcept
				{
					return OP::call(a1, a2, -1);
				}

				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const wchar_t* a2) noexcept
				{
					return OP::call(a1, a2, -1);
				}

				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const char16_t* a2) noexcept
				{
					return OP::call(a1, a2, -1);
				}

				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const char32_t* a2) noexcept
				{
					return OP::call(a1, a2, -1);
				}

				SLIB_INLINE static typename OP::ReturnType call(const char* a2, const STRING& a1) noexcept
				{
					return OP::call(a2, -1, a1);
				}

				SLIB_INLINE static typename OP::ReturnType call(const wchar_t* a2, const STRING& a1) noexcept
				{
					return OP::call(a2, -1, a1);
				}

				SLIB_INLINE static typename OP::ReturnType call(const char16_t* a2, const STRING& a1) noexcept
				{
					return OP::call(a2, -1, a1);
				}

				SLIB_INLINE static typename OP::ReturnType call(const char32_t* a2, const STRING& a1) noexcept
				{
					return OP::call(a2, -1, a1);
				}

#ifdef SLIB_SUPPORT_STD_TYPES
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const std::string& a2) noexcept
				{
					return OP::call(a1, a2.c_str(), a2.length());
				}
				
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const std::wstring& a2) noexcept
				{
					return OP::call(a1, a2.c_str(), a2.length());
				}
				
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const std::u16string& a2) noexcept
				{
					return OP::call(a1, a2.c_str(), a2.length());
				}
				
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const std::u32string& a2) noexcept
				{
					return OP::call(a1, a2.c_str(), a2.length());
				}
				
				SLIB_INLINE static typename OP::ReturnType call(const std::string& a2, const STRING& a1) noexcept
				{
					return OP::call(a2.c_str(), a2.length(), a1);
				}

				SLIB_INLINE static typename OP::ReturnType call(const std::wstring& a2, const STRING& a1) noexcept
				{
					return OP::call(a2.c_str(), a2.length(), a1);
				}

				SLIB_INLINE static typename OP::ReturnType call(const std::u16string& a2, const STRING& a1) noexcept
				{
					return OP::call(a2.c_str(), a2.length(), a1);
				}
				
				SLIB_INLINE static typename OP::ReturnType call(const std::u32string& a2, const STRING& a1) noexcept
				{
					return OP::call(a2.c_str(), a2.length(), a1);
				}				
#endif
				
				template <class T>
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const T& a2) noexcept
				{
					return OP::call(a1, a2);
				}

				template <class T>
				SLIB_INLINE static typename OP::ReturnType call(const T& a2, const STRING& a1) noexcept
				{
					return OP::call_inverse(a2, a1);
				}

			};

			template <class STRING, class OP>
			class OperatorHelper
			{
			public:
				SLIB_INLINE static typename OP::ReturnType call(const typename OP::StringType& a1, const typename OP::StringType& a2) noexcept
				{
					return OP::call(a1, a2);
				}

				SLIB_INLINE static typename OP::ReturnType call(const typename OP::StringType& a1, const Atomic<typename OP::StringType>& a2) noexcept
				{
					return OP::call(a1, (typename OP::StringType)(a2));
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const char (&a2)[N]) noexcept
				{
					return OP::call(a1, a2, N - 1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const wchar_t (&a2)[N]) noexcept
				{
					return OP::call(a1, a2, N - 1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const char16_t (&a2)[N]) noexcept
				{
					return OP::call(a1, a2, N - 1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const char32_t (&a2)[N]) noexcept
				{
					return OP::call(a1, a2, N - 1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const char (&a2)[N], const STRING& a1) noexcept
				{
					return OP::call(a2, N - 1, a1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const wchar_t (&a2)[N], const STRING& a1) noexcept
				{
					return OP::call(a2, N - 1, a1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const char16_t (&a2)[N], const STRING& a1) noexcept
				{
					return OP::call(a2, N - 1, a1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const char32_t (&a2)[N], const STRING& a1) noexcept
				{
					return OP::call(a2, N - 1, a1);
				}
				
				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, char (&a2)[N]) noexcept
				{
					return OP::call(a1, a2, -1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, wchar_t (&a2)[N]) noexcept
				{
					return OP::call(a1, a2, -1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, char16_t (&a2)[N]) noexcept
				{
					return OP::call(a1, a2, -1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, char32_t (&a2)[N]) noexcept
				{
					return OP::call(a1, a2, -1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(char (&a2)[N], const STRING& a1) noexcept
				{
					return OP::call(a2, -1, a1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(wchar_t (&a2)[N], const STRING& a1) noexcept
				{
					return OP::call(a2, -1, a1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(char16_t (&a2)[N], const STRING& a1) noexcept
				{
					return OP::call(a2, -1, a1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(char32_t (&a2)[N], const STRING& a1) noexcept
				{
					return OP::call(a2, -1, a1);
				}

				template <class ARG1, class ARG2>
				SLIB_INLINE static typename OP::ReturnType call(const ARG1& a1, const ARG2& a2) noexcept
				{
					return OperatorInvoker<STRING, OP>::call(a1, a2);
				}

			};

		}
	}

#define PRIV_SLIB_DEFINE_STRING_CLASS_OP_TEMPLATE_SUB(STRING, RET, FUNC, OP, OP_STRING) \
	template <class CHAR, sl_size N> \
	SLIB_INLINE RET STRING::FUNC(CHAR (&other)[N]) const noexcept \
	{ \
		return priv::string::OperatorHelper< STRING, priv::string::OP<OP_STRING> >::call(*this, other); \
	} \
	template <class ARG> \
	SLIB_INLINE RET STRING::FUNC(const ARG& other) const noexcept \
	{ \
		return priv::string::OperatorHelper< STRING, priv::string::OP<OP_STRING> >::call(*this, other); \
	}
	
#define PRIV_SLIB_DEFINE_STRING_CLASS_OP_TEMPLATE_SUB_ATOMIC(STRING, RET, FUNC, OP, OP_STRING) \
	PRIV_SLIB_DEFINE_STRING_CLASS_OP_TEMPLATE_SUB(STRING, RET, FUNC, OP, OP_STRING) \
	template <class CHAR, sl_size N> \
	SLIB_INLINE RET Atomic<STRING>::FUNC(CHAR (&other)[N]) const noexcept \
	{ \
		STRING s = *this; \
		return priv::string::OperatorHelper< STRING, priv::string::OP<OP_STRING> >::call(s, other); \
	} \
	template <class ARG> \
	SLIB_INLINE RET Atomic<STRING>::FUNC(const ARG& other) const noexcept \
	{ \
		STRING s = *this; \
		return priv::string::OperatorHelper< STRING, priv::string::OP<OP_STRING> >::call(s, other); \
	}
	
#define PRIV_SLIB_DEFINE_STRING_CLASS_OP_TEMPLATE(RET, FUNC, OP) \
	PRIV_SLIB_DEFINE_STRING_CLASS_OP_TEMPLATE_SUB_ATOMIC(String, RET, FUNC, OP, String) \
	PRIV_SLIB_DEFINE_STRING_CLASS_OP_TEMPLATE_SUB_ATOMIC(String16, RET, FUNC, OP, String16) \
	PRIV_SLIB_DEFINE_STRING_CLASS_OP_TEMPLATE_SUB(StringView, RET, FUNC, OP, String) \
	PRIV_SLIB_DEFINE_STRING_CLASS_OP_TEMPLATE_SUB(StringView16, RET, FUNC, OP, String16) \
	template <class CHAR, sl_size N> \
	RET StringParam::FUNC(CHAR (&other)[N]) const noexcept \
	{ \
		if (isNull()) { \
			return priv::string::OperatorHelper< String, priv::string::OP<String> >::call(String::null(), other); \
		} else { \
			if (isString16()) { \
				return priv::string::OperatorHelper< String16, priv::string::OP<String16> >::call(toString16(), other); \
			} else if (isString8()) { \
				return priv::string::OperatorHelper< String, priv::string::OP<String> >::call(toString(), other); \
			} else { \
				if (isSz16()) { \
					StringData16 s(*this); \
					return priv::string::OperatorHelper< StringView16, priv::string::OP<String16> >::call((const StringView16&)s, other); \
				} else { \
					StringData s(*this); \
					return priv::string::OperatorHelper< StringView, priv::string::OP<String> >::call((const StringView&)s, other); \
				} \
			} \
		} \
	} \
	template <class ARG> \
	SLIB_INLINE RET StringParam::FUNC(const ARG& other) const noexcept \
	{ \
		if (isNull()) { \
			return priv::string::OperatorHelper< String, priv::string::OP<String> >::call(String::null(), other); \
		} else { \
			if (isString16()) { \
				return priv::string::OperatorHelper< String16, priv::string::OP<String16> >::call(toString16(), other); \
			} else if (isString8()) { \
				return priv::string::OperatorHelper< String, priv::string::OP<String> >::call(toString(), other); \
			} else { \
				if (isSz16()) { \
					StringData16 s(*this); \
					return priv::string::OperatorHelper< StringView16, priv::string::OP<String16> >::call((const StringView16&)s, other); \
				} else { \
					StringData s(*this); \
					return priv::string::OperatorHelper< StringView, priv::string::OP<String> >::call((const StringView&)s, other); \
				} \
			} \
		} \
	}

	PRIV_SLIB_DEFINE_STRING_CLASS_OP_TEMPLATE(sl_bool, equals, EqualsOperator)
	PRIV_SLIB_DEFINE_STRING_CLASS_OP_TEMPLATE(sl_compare_result, compare, CompareOperator)

#define PRIV_SLIB_DEFINE_STRING_OPERATOR_TEMPLATE_SUB(STRING, RET, FUNC, OP, OP_STRING) \
	template <class CHAR, sl_size N> \
	SLIB_INLINE RET FUNC(const STRING& s1, CHAR (&s2)[N]) noexcept \
	{ \
		return priv::string::OperatorHelper< STRING, priv::string::OP<OP_STRING> >::call(s1, s2); \
	} \
	template <class ARG> \
	SLIB_INLINE RET FUNC(const STRING& s1, const ARG& s2) noexcept \
	{ \
		return priv::string::OperatorHelper< STRING, priv::string::OP<OP_STRING> >::call(s1, s2); \
	} \
	template <class CHAR, sl_size N> \
	SLIB_INLINE RET FUNC(CHAR (&s1)[N], const STRING& s2) noexcept \
	{ \
		return priv::string::OperatorHelper< STRING, priv::string::OP<OP_STRING> >::call(s1, s2); \
	} \
	template <class ARG> \
	SLIB_INLINE RET FUNC(const ARG& s1, const STRING& s2) noexcept \
	{ \
		return priv::string::OperatorHelper< STRING, priv::string::OP<OP_STRING> >::call(s1, s2); \
	}

#define PRIV_SLIB_DEFINE_STRING_OPERATOR_TEMPLATE_SUB_ATOMIC(STRING, RET, FUNC, OP, OP_STRING) \
	PRIV_SLIB_DEFINE_STRING_OPERATOR_TEMPLATE_SUB(STRING, RET, FUNC, OP, OP_STRING) \
	template <class CHAR, sl_size N> \
	SLIB_INLINE RET FUNC(const Atomic<STRING>& _s1, CHAR (&s2)[N]) noexcept \
	{ \
		STRING s1(_s1); \
		return priv::string::OperatorHelper< STRING, priv::string::OP<OP_STRING> >::call(s1, s2); \
	} \
	template <class ARG> \
	SLIB_INLINE RET FUNC(const Atomic<STRING>& _s1, const ARG& s2) noexcept \
	{ \
		STRING s1(_s1); \
		return priv::string::OperatorHelper< STRING, priv::string::OP<OP_STRING> >::call(s1, s2); \
	} \
	template <class CHAR, sl_size N> \
	SLIB_INLINE RET FUNC(CHAR (&s1)[N], const Atomic<STRING>& _s2) noexcept \
	{ \
		STRING s2(_s2); \
		return priv::string::OperatorHelper< STRING, priv::string::OP<OP_STRING> >::call(s1, s2); \
	} \
	template <class ARG> \
	SLIB_INLINE RET FUNC(const ARG& s1, const Atomic<STRING>& _s2) noexcept \
	{ \
		STRING s2(_s2); \
		return priv::string::OperatorHelper< STRING, priv::string::OP<OP_STRING> >::call(s1, s2); \
	}

#define PRIV_SLIB_DEFINE_STRING8_OPERATOR_TEMPLATE(RET, FUNC, OP) \
	PRIV_SLIB_DEFINE_STRING_OPERATOR_TEMPLATE_SUB_ATOMIC(String, RET, FUNC, OP, String) \
	PRIV_SLIB_DEFINE_STRING_OPERATOR_TEMPLATE_SUB(StringView, RET, FUNC, OP, String)

#define PRIV_SLIB_DEFINE_STRING16_OPERATOR_TEMPLATE(RET, FUNC, OP) \
	PRIV_SLIB_DEFINE_STRING_OPERATOR_TEMPLATE_SUB_ATOMIC(String16, RET, FUNC, OP, String16) \
	PRIV_SLIB_DEFINE_STRING_OPERATOR_TEMPLATE_SUB(StringView16, RET, FUNC, OP, String16)

#define PRIV_SLIB_DEFINE_STRING_PARAM_OPERATOR_TEMPLATE(RET, FUNC, OP) \
	template <class CHAR, sl_size N> \
	RET FUNC(const StringParam& s1, CHAR (&s2)[N]) noexcept \
	{ \
		if (s1.isNull()) { \
			return priv::string::OperatorHelper< String, priv::string::OP<String> >::call(String::null(), s2); \
		} else { \
			if (s1.isString16()) { \
				return priv::string::OperatorHelper< String16, priv::string::OP<String16> >::call(s1.toString16(), s2); \
			} else if (s1.isString8()) { \
				return priv::string::OperatorHelper< String, priv::string::OP<String> >::call(s1.toString(), s2); \
			} else { \
				if (s1.isSz16()) { \
					StringData16 s(s1); \
					return priv::string::OperatorHelper< String16, priv::string::OP<String16> >::call((const StringView16&)s, s2); \
				} else { \
					StringData s(s1); \
					return priv::string::OperatorHelper< String, priv::string::OP<String> >::call((const StringView&)s, s2); \
				} \
			} \
		} \
	} \
	template <class CHAR, sl_size N> \
	RET FUNC(CHAR (&s1)[N], const StringParam& s2) noexcept \
	{ \
		if (s2.isNull()) { \
			return priv::string::OperatorHelper< String, priv::string::OP<String> >::call(s1, String::null()); \
		} else { \
			if (s2.isString16()) { \
				return priv::string::OperatorHelper< String16, priv::string::OP<String16> >::call(s1, s2.toString16()); \
			} else if (s2.isString8()) { \
				return priv::string::OperatorHelper< String, priv::string::OP<String> >::call(s1, s2.toString()); \
			} else { \
				if (s2.isSz16()) { \
					StringData16 s(s2); \
					return priv::string::OperatorHelper< StringView16, priv::string::OP<String16> >::call(s1, (const StringView16&)s); \
				} else { \
					StringData s(s2); \
					return priv::string::OperatorHelper< StringView, priv::string::OP<String> >::call(s1, (const StringView&)s); \
				} \
			} \
		} \
	} \
	template <class ARG> \
	SLIB_INLINE RET FUNC(const StringParam& s1, const ARG& s2) noexcept \
	{ \
		if (s1.isNull()) { \
			return priv::string::OperatorHelper< String, priv::string::OP<String> >::call(String::null(), s2); \
		} else { \
			if (s1.isString16()) { \
				return priv::string::OperatorHelper< String16, priv::string::OP<String16> >::call(s1.toString16(), s2); \
			} else if (s1.isString8()) { \
				return priv::string::OperatorHelper< String, priv::string::OP<String> >::call(s1.toString(), s2); \
			} else { \
				if (s1.isSz16()) { \
					StringData16 s(s1); \
					return priv::string::OperatorHelper< String16, priv::string::OP<String16> >::call((const StringView16&)s, s2); \
				} else { \
					StringData s(s1); \
					return priv::string::OperatorHelper< String, priv::string::OP<String> >::call((const StringView&)s, s2); \
				} \
			} \
		} \
	} \
	template <class ARG> \
	SLIB_INLINE RET FUNC(const ARG& s1, const StringParam& s2) noexcept \
	{ \
		if (s2.isNull()) { \
			return priv::string::OperatorHelper< String, priv::string::OP<String> >::call(s1, String::null()); \
		} else { \
			if (s2.isString16()) { \
				return priv::string::OperatorHelper< String16, priv::string::OP<String16> >::call(s1, s2.toString16()); \
			} else if (s2.isString8()) { \
				return priv::string::OperatorHelper< String, priv::string::OP<String> >::call(s1, s2.toString()); \
			} else { \
				if (s2.isSz16()) { \
					StringData16 s(s2); \
					return priv::string::OperatorHelper< StringView16, priv::string::OP<String16> >::call(s1, (const StringView16&)s); \
				} else { \
					StringData s(s2); \
					return priv::string::OperatorHelper< StringView, priv::string::OP<String> >::call(s1, (const StringView&)s); \
				} \
			} \
		} \
	}

	PRIV_SLIB_DEFINE_STRING8_OPERATOR_TEMPLATE(String, operator+, PlusOperator)
	PRIV_SLIB_DEFINE_STRING16_OPERATOR_TEMPLATE(String16, operator+, PlusOperator)
	PRIV_SLIB_DEFINE_STRING_PARAM_OPERATOR_TEMPLATE(StringParam, operator+, PlusOperator)

#define PRIV_SLIB_STRING_DEFINE_COMPARE_OPERATOR_TEMPLATE_SECTION(OP, STRING, BODY, BODY_FRIEND) \
	template <class CHAR, sl_size N> \
	SLIB_INLINE sl_bool operator OP(const STRING& s, CHAR (&other)[N]) noexcept \
	{ \
		return BODY; \
	} \
	template <class ARG> \
	SLIB_INLINE sl_bool operator OP(const STRING& s, const ARG& other) noexcept \
	{ \
		return BODY; \
	} \
	template <class CHAR, sl_size N> \
	SLIB_INLINE sl_bool operator OP(CHAR (&other)[N], const STRING& s) noexcept \
	{ \
		return BODY_FRIEND; \
	} \
	template <class ARG> \
	SLIB_INLINE sl_bool operator OP(const ARG& other, const STRING& s) noexcept \
	{ \
		return BODY_FRIEND; \
	}

#define PRIV_SLIB_STRING_DEFINE_COMPARE_OPERATOR_TEMPLATE(OP, BODY, BODY_FRIEND) \
	PRIV_SLIB_STRING_DEFINE_COMPARE_OPERATOR_TEMPLATE_SECTION(OP, String, BODY, BODY_FRIEND) \
	PRIV_SLIB_STRING_DEFINE_COMPARE_OPERATOR_TEMPLATE_SECTION(OP, String16, BODY, BODY_FRIEND) \
	PRIV_SLIB_STRING_DEFINE_COMPARE_OPERATOR_TEMPLATE_SECTION(OP, AtomicString, BODY, BODY_FRIEND) \
	PRIV_SLIB_STRING_DEFINE_COMPARE_OPERATOR_TEMPLATE_SECTION(OP, AtomicString16, BODY, BODY_FRIEND) \
	PRIV_SLIB_STRING_DEFINE_COMPARE_OPERATOR_TEMPLATE_SECTION(OP, StringView, BODY, BODY_FRIEND) \
	PRIV_SLIB_STRING_DEFINE_COMPARE_OPERATOR_TEMPLATE_SECTION(OP, StringView16, BODY, BODY_FRIEND) \
	PRIV_SLIB_STRING_DEFINE_COMPARE_OPERATOR_TEMPLATE_SECTION(OP, StringParam, BODY, BODY_FRIEND)

	PRIV_SLIB_STRING_DEFINE_COMPARE_OPERATOR_TEMPLATE(==, s.equals(other), s.equals(other))
	PRIV_SLIB_STRING_DEFINE_COMPARE_OPERATOR_TEMPLATE(!=, !(s.equals(other)), !(s.equals(other)))
	PRIV_SLIB_STRING_DEFINE_COMPARE_OPERATOR_TEMPLATE(>=, s.compare(other)>=0, s.compare(other)<=0)
	PRIV_SLIB_STRING_DEFINE_COMPARE_OPERATOR_TEMPLATE(<=, s.compare(other)<=0, s.compare(other)>=0)
	PRIV_SLIB_STRING_DEFINE_COMPARE_OPERATOR_TEMPLATE(>, s.compare(other)>0, s.compare(other)<0)
	PRIV_SLIB_STRING_DEFINE_COMPARE_OPERATOR_TEMPLATE(<, s.compare(other)<0, s.compare(other)>0)

	template <class T>
	SLIB_INLINE String& String::operator+=(T&& other) noexcept
	{
		return *this = *this + Forward<T>(other);
	}
	
	template <class T>
	SLIB_INLINE AtomicString& AtomicString::operator+=(T&& other) noexcept
	{
		return *this = String(*this) + Forward<T>(other);
	}

	template <class T>
	SLIB_INLINE String16& String16::operator+=(T&& other) noexcept
	{
		return *this = *this + Forward<T>(other);
	}
	
	template <class T>
	SLIB_INLINE AtomicString16& AtomicString16::operator+=(T&& other) noexcept
	{
		return *this = String16(*this) + Forward<T>(other);
	}

}
