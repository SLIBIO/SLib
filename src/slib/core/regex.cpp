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

#include "slib/core/regex.h"

#include "slib/core/safe_static.h"

#include <regex>

namespace slib
{
	SLIB_DEFINE_OBJECT(CRegEx, Object)
	
	CRegEx::CRegEx() noexcept
	{
	}
	
	CRegEx::~CRegEx() noexcept
	{
		delete ((std::regex*)m_obj);
	}
	
	Ref<CRegEx> CRegEx::_create(const String& pattern, int _flags) noexcept
	{
		std::regex* obj = (std::regex*)(Base::createMemory(sizeof(std::regex)));
		if (obj) {
			int flags = 0;
			if (_flags & RegExFlags::Icase) {
				flags |= std::regex_constants::icase;
			}
			if (_flags & RegExFlags::Nosubs) {
				flags |= std::regex_constants::nosubs;
			}
			if (_flags & RegExFlags::Optimize) {
				flags |= std::regex_constants::optimize;
			}
			if (_flags & RegExFlags::Collate) {
				flags |= std::regex_constants::collate;
			}
			if (_flags & RegExFlags::ECMAScript) {
				flags |= std::regex_constants::ECMAScript;
			}
			if (_flags & RegExFlags::Basic) {
				flags |= std::regex_constants::basic;
			}
			if (_flags & RegExFlags::Extended) {
				flags |= std::regex_constants::extended;
			}
			if (_flags & RegExFlags::Awk) {
				flags |= std::regex_constants::awk;
			}
			if (_flags & RegExFlags::Grep) {
				flags |= std::regex_constants::grep;
			}
			if (_flags & RegExFlags::Egrep) {
				flags |= std::regex_constants::egrep;
			}
			try {
				if (flags) {
					new (obj) std::regex((char*)(pattern.getData()), (std::size_t)(pattern.getLength()), (std::regex_constants::syntax_option_type)flags);
				} else {
					new (obj) std::regex((char*)(pattern.getData()), (std::size_t)(pattern.getLength()));
				}
			} catch (std::regex_error&) {
				Base::freeMemory(obj);
				return sl_null;
			}
			Ref<CRegEx> ret = new CRegEx;
			if (ret.isNotNull()) {
				ret->m_obj = obj;
				return ret;
			}
			delete obj;
		}
		return sl_null;
	}
	
	Ref<CRegEx> CRegEx::create(const String& pattern) noexcept
	{
		return _create(pattern, 0);
	}
	
	Ref<CRegEx> CRegEx::create(const String& pattern, const RegExFlags& flags) noexcept
	{
		return _create(pattern, flags);
	}
	
	sl_bool CRegEx::match(const String& str, const RegExMatchFlags& _flags) noexcept
	{
		int flags = 0;
		int v = _flags.value;
		if (v) {
			if (v & RegExMatchFlags::NotBol) {
				flags |= std::regex_constants::match_not_bol;
			}
			if (v & RegExMatchFlags::NotEol) {
				flags |= std::regex_constants::match_not_eol;
			}
			if (v & RegExMatchFlags::NotBow) {
				flags |= std::regex_constants::match_not_bow;
			}
			if (v & RegExMatchFlags::NotEow) {
				flags |= std::regex_constants::match_not_eow;
			}
			if (v & RegExMatchFlags::Any) {
				flags |= std::regex_constants::match_any;
			}
			if (v & RegExMatchFlags::NotNull) {
				flags |= std::regex_constants::match_not_null;
			}
			if (v & RegExMatchFlags::Continuous) {
				flags |= std::regex_constants::match_continuous;
			}
			if (v & RegExMatchFlags::PrevAvail) {
				flags |= std::regex_constants::match_prev_avail;
			}
			if (v & RegExMatchFlags::FormatSed) {
				flags |= std::regex_constants::format_sed;
			}
			if (v & RegExMatchFlags::FormatNoCopy) {
				flags |= std::regex_constants::format_no_copy;
			}
			if (v & RegExMatchFlags::FormatFirstOnly) {
				flags |= std::regex_constants::format_first_only;
			}
		}
		std::regex* obj = (std::regex*)m_obj;
		const char* start = (char*)(str.getData());
		const char* end = start + str.getLength();
		return std::regex_match(start, end, *obj, (std::regex_constants::match_flag_type)flags);
	}
	
	RegEx::RegEx(const String& pattern) noexcept
	 : ref(CRegEx::create(pattern))
	{
	}

	RegEx::RegEx(const String& pattern, const RegExFlags& flags) noexcept
	 : ref(CRegEx::create(pattern, flags))
	{
	}
	
	sl_bool RegEx::match(const String& str, const RegExMatchFlags& flags) noexcept
	{
		if (ref.isNotNull()) {
			return ref->match(str, flags);
		}
		return sl_false;
	}
	
	Atomic<RegEx>::Atomic(const String& pattern) noexcept
	 : ref(CRegEx::create(pattern, 0))
	{
	}
	
	Atomic<RegEx>::Atomic(const String& pattern, const RegExFlags& flags) noexcept
	 : ref(CRegEx::create(pattern, flags))
	{
	}
	
	sl_bool Atomic<RegEx>::match(const String& str, const RegExMatchFlags& flags) noexcept
	{
		Ref<CRegEx> ref(this->ref);
		if (ref.isNotNull()) {
			return ref->match(str, flags);
		}
		return sl_false;
	}

	
	sl_bool RegEx::matchEmail(const String& str) noexcept
	{
		SLIB_SAFE_STATIC(RegEx, regex, "^[a-zA-Z0-9.!#$%&’*+/=?^_`{|}~-]+@[a-zA-Z0-9-]+(?:\\.[a-zA-Z0-9-]+)*$");
		if (SLIB_SAFE_STATIC_CHECK_FREED(regex)) {
			return sl_false;
		}
		return regex.match(str);
	}
	
}
