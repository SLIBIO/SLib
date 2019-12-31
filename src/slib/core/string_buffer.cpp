/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

#include "slib/core/string_buffer.h"

namespace slib
{
	
	StringBuffer::StringBuffer() noexcept
	{
		m_len = 0;
	}
	
	StringBuffer::~StringBuffer() noexcept
	{
	}

	StringBuffer16::StringBuffer16() noexcept
	{
		m_len = 0;
	}
	
	StringBuffer16::~StringBuffer16() noexcept
	{
	}

	sl_size StringBuffer::getLength() const noexcept
	{
		return m_len;
	}

	sl_size StringBuffer16::getLength() const noexcept
	{
		return m_len;
	}
	
	sl_bool StringBuffer::add(const String& str) noexcept
	{
		sl_size len = str.getLength();
		if (len == 0) {
			return sl_true;
		}
		StringStorage data;
		data.data8 = str.getData();
		data.length = len;
		data.string8 = str;
		if (m_queue.push_NoLock(data)) {
			m_len += len;
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool StringBuffer16::add(const String16& str) noexcept
	{
		sl_size len = str.getLength();
		if (len == 0) {
			return sl_true;
		}
		StringStorage data;
		data.data16 = str.getData();
		data.length = len;
		data.string16 = str;
		if (m_queue.push_NoLock(data)) {
			m_len += len;
			return sl_true;
		}
		return sl_false;
	}
	
	sl_bool StringBuffer::add(const StringStorage& data) noexcept
	{
		sl_size len = data.length;
		if (len == 0) {
			return sl_true;
		}
		if (data.data8) {
			if (m_queue.push_NoLock(data)) {
				m_len += len;
				return sl_true;
			}
		}
		return sl_false;
	}
	
	sl_bool StringBuffer16::add(const StringStorage& data) noexcept
	{
		sl_size len = data.length;
		if (len == 0) {
			return sl_true;
		}
		if (data.data16) {
			if (m_queue.push_NoLock(data)) {
				m_len += len;
				return sl_true;
			}
		}
		return sl_false;
	}
	
	sl_bool StringBuffer::addStatic(const sl_char8* buf, sl_size length) noexcept
	{
		if (!length) {
			return sl_true;
		}
		StringStorage data;
		data.data8 = buf;
		data.length = length;
		return add(data);
	}

	sl_bool StringBuffer16::addStatic(const sl_char16* buf, sl_size length) noexcept
	{
		if (!length) {
			return sl_true;
		}
		StringStorage data;
		data.data16 = buf;
		data.length = length;
		return add(data);
	}

	void StringBuffer::link(StringBuffer& buf) noexcept
	{
		m_len += buf.m_len;
		buf.m_len = 0;
		m_queue.merge_NoLock(&(buf.m_queue));
	}
	
	void StringBuffer16::link(StringBuffer16& buf) noexcept
	{
		m_len += buf.m_len;
		buf.m_len = 0;
		m_queue.merge_NoLock(&(buf.m_queue));
	}
	
	void StringBuffer::clear() noexcept
	{
		m_queue.removeAll_NoLock();
		m_len = 0;
	}

	void StringBuffer16::clear() noexcept
	{
		m_queue.removeAll_NoLock();
		m_len = 0;
	}

	String StringBuffer::merge() const noexcept
	{
		if (m_queue.getCount() == 0) {
			return String::getEmpty();
		}
		Link<StringStorage>* front = m_queue.getFront();
		sl_size total = m_len;
		String ret = String::allocate(total);
		if (ret.isNotEmpty()) {
			sl_char8* buf = (sl_char8*)(ret.getData());
			sl_size offset = 0;
			Link<StringStorage>* item = front;
			while (item) {
				StringStorage& s = item->value;
				sl_size t = s.length;
				if (offset + t > total) {
					Base::copyMemory(buf + offset, s.data8, total - offset);
					return ret;
				}
				Base::copyMemory(buf + offset, s.data8, t);
				offset += t;
				item = item->next;
			}
		}
		return ret;
	}
	
	String16 StringBuffer16::merge() const noexcept
	{
		if (m_queue.getCount() == 0) {
			return String16::getEmpty();
		}
		Link<StringStorage>* front = m_queue.getFront();
		sl_size total = m_len;
		String16 ret = String16::allocate(total);
		if (ret.isNotEmpty()) {
			sl_char16* buf = (sl_char16*)(ret.getData());
			sl_size offset = 0;
			Link<StringStorage>* item = front;
			while (item) {
				StringStorage& s = item->value;
				sl_size t = s.length;
				if (offset + t > total) {
					Base::copyMemory(buf + offset, s.data16, (total - offset) << 1);
					return ret;
				}
				Base::copyMemory(buf + offset, s.data16, t << 1);
				offset += t;
				item = item->next;
			}
		}
		return ret;
	}
	
	Memory StringBuffer::mergeToMemory() const noexcept
	{
		if (m_queue.getCount() == 0) {
			return sl_null;
		}
		Link<StringStorage>* front = m_queue.getFront();
		sl_size total = m_len;
		Memory ret = Memory::create(total);
		if (ret.isNotNull()) {
			sl_char8* buf = (sl_char8*)(ret.getData());
			sl_size offset = 0;
			Link<StringStorage>* item = front;
			while (item) {
				StringStorage& s = item->value;
				sl_size t = s.length;
				Base::copyMemory(buf + offset, s.data8, t);
				offset += t;
				item = item->next;
			}
		}
		return ret;
	}
	
	Memory StringBuffer16::mergeToMemory() const noexcept
	{
		if (m_queue.getCount() == 0) {
			return sl_null;
		}
		Link<StringStorage>* front = m_queue.getFront();
		sl_size total = m_len;
		Memory ret = Memory::create(total * 2);
		if (ret.isNotNull()) {
			sl_char16* buf = (sl_char16*)(ret.getData());
			sl_size offset = 0;
			Link<StringStorage>* item = front;
			while (item) {
				StringStorage& s = item->value;
				sl_size t = s.length;
				Base::copyMemory(buf + offset, s.data16, t * 2);
				offset += t;
				item = item->next;
			}
		}
		return ret;
	}
	
}
