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

#ifndef CHECKHEADER_SLIB_CORE_REGEX
#define CHECKHEADER_SLIB_CORE_REGEX

#include "definition.h"

#include "object.h"
#include "string.h"

namespace slib
{

	class RegExFlags
	{
	public:
		int value;
		SLIB_MEMBERS_OF_FLAGS(RegExFlags, value)
		
		enum {
			Default = 0,
			Icase = 0x0001,
			Nosubs = 0x0002,
			Optimize = 0x0004,
			Collate = 0x0008,
			ECMAScript = 0x0010,
			Basic = 0x0020,
			Extended = 0x0040,
			Awk = 0x0080,
			Grep = 0x0100,
			Egrep = 0x0200
		};
	};
	
	class RegExMatchFlags
	{
	public:
		int value;
		SLIB_MEMBERS_OF_FLAGS(RegExMatchFlags, value)
		
		enum {
			Default = 0,
			NotBol = 0x0001,
			NotEol = 0x0002,
			NotBow = 0x0004,
			NotEow = 0x0008,
			Any = 0x0010,
			NotNull = 0x0020,
			Continuous = 0x0040,
			PrevAvail = 0x0080,
			FormatSed = 0x0100,
			FormatNoCopy = 0x0200,
			FormatFirstOnly = 0x0400
		};
	};
	
	class CRegEx : public Object
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		CRegEx() noexcept;
		
		~CRegEx() noexcept;
		
	public:
		static Ref<CRegEx> create(const String& pattern) noexcept;
		
		static Ref<CRegEx> create(const String& pattern, const RegExFlags& flags) noexcept;

	public:
		sl_bool match(const String& str, const RegExMatchFlags& flags = RegExMatchFlags::Default) noexcept;
		
	private:
		static Ref<CRegEx> _create(const String& pattern, int flags) noexcept;
		
	private:
		void* m_obj;
		
	};
	
	class RegEx;
	
	template <>
	class SLIB_EXPORT Atomic<RegEx>
	{
	public:
		SLIB_ATOMIC_REF_WRAPPER(CRegEx)
		
		Atomic(const String& pattern) noexcept;
		
		Atomic(const String& pattern, const RegExFlags& flags) noexcept;
		
	public:
		sl_bool match(const String& str, const RegExMatchFlags& flags = RegExMatchFlags::Default) noexcept;

	private:
		AtomicRef<CRegEx> ref;
		
	};

	typedef Atomic<RegEx> AtomicRegEx;

	
	class SLIB_EXPORT RegEx
	{
	public:
		SLIB_REF_WRAPPER(RegEx, CRegEx)
		
		RegEx(const String& pattern) noexcept;

		RegEx(const String& pattern, const RegExFlags& flags) noexcept;
				
	public:
		sl_bool match(const String& str, const RegExMatchFlags& flags = RegExMatchFlags::Default) noexcept;
		
	public:
		static sl_bool matchEmail(const String& str) noexcept;

	private:
		Ref<CRegEx> ref;
		
	};

}

#endif
