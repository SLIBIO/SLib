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
			struct ConstContainer
			{
				StringContainer* container;
				sl_int32 lock;
			};

			extern const ConstContainer g_null;
			extern const ConstContainer g_empty;

			extern const char* g_conv_radixPatternUpper;
			extern const char* g_conv_radixPatternLower;
			extern const sl_uint8* g_conv_radixInversePatternBig;
			extern const sl_uint8* g_conv_radixInversePatternSmall;

			template <class STRING>
			class PlusOperator
			{
			public:
				typedef STRING ReturnType;

			public:
				template <class CHAR2>
				SLIB_INLINE static STRING call(const STRING& s1, const CHAR2* sz2, sl_reg len2) noexcept
				{
					if (sz2 && len2) {
						sl_size n = s1.getLength();
						if (n) {
							return STRING::merge(s1.getData(), n, sz2, len2);
						} else {
							return STRING(sz2, len2);
						}
					} else {
						return s1.getNotNull();
					}
				}

				template <class CHAR2>
				SLIB_INLINE static STRING call(const CHAR2* sz1, sl_reg len1, const STRING& s2) noexcept
				{
					if (sz1 && len1) {
						sl_size n = s2.getLength();
						if (n) {
							return STRING::merge(sz1, len1, s2.getData(), n);
						} else {
							return STRING(sz1, len1);
						}
					} else {
						return s2.getNotNull();
					}
				}

			};
			
			template <class STRING>
			class EqualsOperator
			{
			public:
				typedef sl_bool ReturnType;

			public:
				template <class CHAR2>
				SLIB_INLINE static sl_bool call(const STRING& s1, const CHAR2* sz2, sl_reg len2) noexcept
				{
					return STRING::equals(s1.getData(), s1.getLength(), sz2, len2);
				}

			};
			
			template <class STRING>
			class CompareOperator
			{
			public:
				typedef sl_compare_result ReturnType;

			public:
				template <class CHAR2>
				SLIB_INLINE static sl_compare_result call(const STRING& s1, const CHAR2* sz2, sl_reg len2) noexcept
				{
					return STRING::compare(s1.getData(), s1.getLength(), sz2, len2);
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
				
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const std::u16string& a2) noexcept
				{
					return OP::call(a1, a2.c_str(), a2.length());
				}
				
				SLIB_INLINE static typename OP::ReturnType call(const std::string& a2, const STRING& a1) noexcept
				{
					return OP::call(a2.c_str(), a2.length(), a1);
				}

				SLIB_INLINE static typename OP::ReturnType call(const std::u16string& a2, const STRING& a1) noexcept
				{
					return OP::call(a2.c_str(), a2.length(), a1);
				}
#endif
				
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const StringParam& _a2) noexcept
				{
					StringData a2(_a2);
					return OP::call(a1, a2.data, a2.getLength());
				}
				
				SLIB_INLINE static typename OP::ReturnType call(const StringParam& _a2, const STRING& a1) noexcept
				{
					StringData a2(_a2);
					return OP::call(a2.data, a2.getLength(), a1);
				}
				
			};

			template <class STRING, class OP>
			class OperatorHelper
			{
			public:
				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, char (&a2)[N]) noexcept
				{
					return OP::call(a1, a2, -1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const char (&a2)[N]) noexcept
				{
					return OP::call(a1, a2, N - 1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, wchar_t (&a2)[N]) noexcept
				{
					return OP::call(a1, a2, -1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const wchar_t (&a2)[N]) noexcept
				{
					return OP::call(a1, a2, N - 1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, char16_t (&a2)[N]) noexcept
				{
					return OP::call(a1, a2, -1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const char16_t (&a2)[N]) noexcept
				{
					return OP::call(a1, a2, N - 1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, char32_t (&a2)[N]) noexcept
				{
					return OP::call(a1, a2, -1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const STRING& a1, const char32_t (&a2)[N]) noexcept
				{
					return OP::call(a1, a2, N - 1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(char (&a2)[N], const STRING& a1) noexcept
				{
					return OP::call(a2, -1, a1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const char (&a2)[N], const STRING& a1) noexcept
				{
					return OP::call(a2, N - 1, a1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(wchar_t (&a2)[N], const STRING& a1) noexcept
				{
					return OP::call(a2, -1, a1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const wchar_t (&a2)[N], const STRING& a1) noexcept
				{
					return OP::call(a2, N - 1, a1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(char16_t (&a2)[N], const STRING& a1) noexcept
				{
					return OP::call(a2, -1, a1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const char16_t (&a2)[N], const STRING& a1) noexcept
				{
					return OP::call(a2, N - 1, a1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(char32_t (&a2)[N], const STRING& a1) noexcept
				{
					return OP::call(a2, -1, a1);
				}

				template <sl_size N>
				SLIB_INLINE static typename OP::ReturnType call(const char32_t (&a2)[N], const STRING& a1) noexcept
				{
					return OP::call(a2, N - 1, a1);
				}

				template <class ARG1, class ARG2>
				SLIB_INLINE static typename OP::ReturnType call(const ARG1& a1, const ARG2& a2) noexcept
				{
					return OperatorInvoker<STRING, OP>::call(a1, a2);
				}

			};


		}
	}

#define PRIV_SLIB_DEFINE_STRING_OPERATOR1(STRING, RET, FUNC, OP, OP_STRING, STRING_REF) \
	template <class CHAR, sl_size N> \
	SLIB_INLINE RET STRING::FUNC(CHAR (&other)[N]) const noexcept \
	{ \
		STRING_REF s = *this; \
		return priv::string::OperatorHelper< OP_STRING, priv::string::OP<OP_STRING> >::call(s, other); \
	} \
	template <class ARG> \
	SLIB_INLINE RET STRING::FUNC(const ARG& other) const noexcept \
	{ \
		STRING_REF s = *this; \
		return priv::string::OperatorHelper< OP_STRING, priv::string::OP<OP_STRING> >::call(s, other); \
	}

#define PRIV_SLIB_DEFINE_STRING_OPERATOR2(STRING, RET, FUNC, OP, OP_STRING, STRING_REF) \
	template <class CHAR, sl_size N> \
	SLIB_INLINE RET STRING::FUNC(CHAR (&other)[N]) const noexcept \
	{ \
		STRING_REF s = *this; \
		return priv::string::OperatorHelper< OP_STRING, priv::string::OP<OP_STRING> >::call(s, other); \
	} \
	template <class ARG> \
	SLIB_INLINE RET STRING::FUNC(const ARG& other) const noexcept \
	{ \
		STRING_REF s = *this; \
		return priv::string::OperatorHelper< OP_STRING, priv::string::OP<OP_STRING> >::call(s, other); \
	} \
	template <class CHAR, sl_size N> \
	SLIB_INLINE RET FUNC(CHAR (&other)[N], const STRING& _s) noexcept \
	{ \
		STRING_REF s = _s; \
		return priv::string::OperatorHelper< OP_STRING, priv::string::OP<OP_STRING> >::call(other, s); \
	} \
	template <class ARG> \
	SLIB_INLINE RET FUNC(const ARG& other, const STRING& _s) noexcept \
	{ \
		STRING_REF s = _s; \
		return priv::string::OperatorHelper< OP_STRING, priv::string::OP<OP_STRING> >::call(other, s); \
	}

#define PRIV_SLIB_STRING_DEFINE_COMPARE_OPERATOR_TEMPLATE_SECTION(OP, STRING, BODY, BODY_FRIEND) \
	template <class CHAR, sl_size N> \
	SLIB_INLINE sl_bool STRING::operator OP(CHAR (&other)[N]) const noexcept \
	{ \
		return BODY; \
	} \
	template <class ARG> \
	SLIB_INLINE sl_bool STRING::operator OP(const ARG& other) const noexcept \
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
	PRIV_SLIB_STRING_DEFINE_COMPARE_OPERATOR_TEMPLATE_SECTION(OP, AtomicString16, BODY, BODY_FRIEND)

	PRIV_SLIB_STRING_DEFINE_COMPARE_OPERATOR_TEMPLATE(==, equals(other), s.equals(other))
	PRIV_SLIB_STRING_DEFINE_COMPARE_OPERATOR_TEMPLATE(!=, !(equals(other)), !(s.equals(other)))
	PRIV_SLIB_STRING_DEFINE_COMPARE_OPERATOR_TEMPLATE(>=, compare(other)>=0, s.compare(other)<=0)
	PRIV_SLIB_STRING_DEFINE_COMPARE_OPERATOR_TEMPLATE(<=, compare(other)<=0, s.compare(other)>=0)
	PRIV_SLIB_STRING_DEFINE_COMPARE_OPERATOR_TEMPLATE(>, compare(other)>0, s.compare(other)<0)
	PRIV_SLIB_STRING_DEFINE_COMPARE_OPERATOR_TEMPLATE(<, compare(other)<0, s.compare(other)>0)


	SLIB_INLINE String::String(String&& src) noexcept
	{
		m_container = src.m_container;
		src.m_container = sl_null;
	}
	
	SLIB_INLINE String::String(AtomicString&& src) noexcept
	{
		m_container = src.m_container;
		src.m_container = sl_null;
	}

	template <sl_size N>
	SLIB_INLINE String String::fromStatic(const sl_char8 (&str)[N]) noexcept
	{
		return fromStatic(str, N - 1);
	}

	SLIB_INLINE const String& String::null() noexcept
	{
		return *(reinterpret_cast<String const*>(&(priv::string::g_null)));
	}
	
	SLIB_INLINE const String& String::getEmpty() noexcept
	{
		return *(reinterpret_cast<String const*>(&(priv::string::g_empty)));
	}

	SLIB_INLINE const String& String::getNotNull() const noexcept
	{
		if (!m_container) {
			return *(reinterpret_cast<String const*>(&(priv::string::g_empty)));
		}
		return *this;
	}
	
	SLIB_INLINE sl_bool String::isNull() const noexcept
	{
		return m_container == sl_null;
	}

	SLIB_INLINE sl_bool String::isNotNull() const noexcept
	{
		return m_container != sl_null;
	}
	
	SLIB_INLINE sl_bool String::isEmpty() const noexcept
	{
		if (m_container) {
			return (m_container->len == 0);
		} else {
			return sl_true;
		}
	}
	
	SLIB_INLINE sl_bool String::isNotEmpty() const noexcept
	{
		if (m_container) {
			return (m_container->len != 0);
		} else {
			return sl_false;
		}
	}
	
	SLIB_INLINE sl_char8* String::getData() const noexcept
	{
		if (m_container) {
			return m_container->sz;
		} else {
			return (sl_char8*)((void*)(""));
		}
	}
	
	SLIB_INLINE sl_char8* String::getData(sl_size& outLength) const noexcept
	{
		if (m_container) {
			outLength = m_container->len;
			return m_container->sz;
		} else {
			outLength = 0;
			return (sl_char8*)((void*)(""));
		}
	}

	SLIB_INLINE sl_size String::getLength() const noexcept
	{
		if (m_container) {
			return m_container->len;
		} else {
			return 0;
		}
	}

	PRIV_SLIB_DEFINE_STRING_OPERATOR2(String, String, operator+, PlusOperator, String, const String&)

	template <class CHAR, sl_size N>
	SLIB_INLINE String& String::operator+=(CHAR (&other)[N]) noexcept
	{
		*this = priv::string::OperatorHelper< String, priv::string::PlusOperator<String> >::call(*this, other);
		return *this;
	}

	template <class ARG>
	SLIB_INLINE String& String::operator+=(const ARG& other) noexcept
	{
		*this = priv::string::OperatorHelper< String, priv::string::PlusOperator<String> >::call(*this, other);
		return *this;
	}

	PRIV_SLIB_DEFINE_STRING_OPERATOR1(String, sl_bool, equals, EqualsOperator, String, const String&)
	PRIV_SLIB_DEFINE_STRING_OPERATOR1(String, sl_compare_result, compare, CompareOperator, String, const String&)


	SLIB_INLINE AtomicString::Atomic(AtomicString&& src) noexcept
	{
		m_container = src.m_container;
		src.m_container = sl_null;
	}

	SLIB_INLINE AtomicString::Atomic(String&& src) noexcept
	{
		m_container = src.m_container;
		src.m_container = sl_null;
	}

	SLIB_INLINE const AtomicString& AtomicString::null() noexcept
	{
		return *(reinterpret_cast<AtomicString const*>(&(priv::string::g_null)));
	}
	
	SLIB_INLINE const AtomicString& AtomicString::getEmpty() noexcept
	{
		return *(reinterpret_cast<AtomicString const*>(&(priv::string::g_empty)));
	}

	SLIB_INLINE sl_bool AtomicString::isNull() const noexcept
	{
		return m_container == sl_null;
	}

	SLIB_INLINE sl_bool AtomicString::isNotNull() const noexcept
	{
		return m_container != sl_null;
	}

	PRIV_SLIB_DEFINE_STRING_OPERATOR2(AtomicString, String, operator+, PlusOperator, String, String)

	template <class CHAR, sl_size N>
	SLIB_INLINE AtomicString& AtomicString::operator+=(CHAR (&other)[N]) noexcept
	{
		String str(*this);
		*this = priv::string::OperatorHelper< String, priv::string::PlusOperator<String> >::call(str, other);
		return *this;
	}

	template <class ARG>
	SLIB_INLINE AtomicString& AtomicString::operator+=(const ARG& other) noexcept
	{
		String str(*this);
		*this = priv::string::OperatorHelper< String, priv::string::PlusOperator<String> >::call(str, other);
		return *this;
	}

	PRIV_SLIB_DEFINE_STRING_OPERATOR1(AtomicString, sl_bool, equals, EqualsOperator, String, String)
	PRIV_SLIB_DEFINE_STRING_OPERATOR1(AtomicString, sl_compare_result, compare, CompareOperator, String, String)

}
