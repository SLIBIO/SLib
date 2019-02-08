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

#include "slib/core/memory.h"

namespace slib
{

	SLIB_DEFINE_OBJECT(CMemory, CArray<sl_uint8>)

	CMemory::CMemory()
	{
	}

	CMemory::CMemory(sl_size count) : CArray(count)
	{
	}

	CMemory::CMemory(const void* data, sl_size count) : CArray((sl_uint8 const*)data, count)
	{
	}

	CMemory::CMemory(const void* data, sl_size count, Referable* refer) : CArray((sl_uint8 const*)data, count, refer)
	{
	}

	CMemory::~CMemory()
	{
	}

	CMemory* CMemory::create(sl_size count)
	{
		if (count > 0) {
			CMemory* ret = new CMemory(count);
			if (ret) {
				if (ret->m_data) {
					return ret;
				}
				delete ret;
			}
		}
		return sl_null;
	}


	CMemory* CMemory::create(const void* data, sl_size count)
	{
		if (count > 0) {
			CMemory* ret = new CMemory(data, count);
			if (ret) {
				if (ret->m_data) {
					return ret;
				}
				delete ret;
			}
		}
		return sl_null;
	}

	CMemory* CMemory::createStatic(const void* data, sl_size count, Referable* refer)
	{
		if (data && count > 0) {
			return new CMemory(data, count, refer);
		}
		return sl_null;
	}

	CMemory* CMemory::sub(sl_size start, sl_size count)
	{
		sl_size countParent = m_count;
		if (start < countParent) {
			if (count > countParent - start) {
				count = countParent - start;
			}
			if (count > 0) {
				if (start == 0 && countParent == count) {
					return (CMemory*)this;
				}
				if (m_flagStatic) {
					return createStatic(m_data + start, count, m_refer._ptr);
				} else {
					return createStatic(m_data + start, count, this);
				}
			}
		}
		return sl_null;
	}

	CMemory* CMemory::duplicate() const
	{
		return create(m_data, m_count);
	}


	Memory Memory::create(sl_size count)
	{
		return CMemory::create(count);
	}

	Memory Memory::create(const void* buf, sl_size count)
	{
		return CMemory::create(buf, count);
	}

	Memory Memory::createStatic(const void* buf, sl_size count)
	{
		return CMemory::createStatic(buf, count, sl_null);
	}

	Memory Memory::createStatic(const void* buf, sl_size count, Referable* refer)
	{
		return CMemory::createStatic(buf, count, refer);
	}

	void* Memory::getData() const
	{
		CMemory* obj = ref._ptr;
		if (obj) {
			return obj->getData();
		}
		return sl_null;
	}

	sl_size Memory::getSize() const
	{
		CMemory* obj = ref._ptr;
		if (obj) {
			return obj->getCount();
		}
		return 0;
	}

	Memory Memory::sub(sl_size start, sl_size size) const
	{
		CMemory* obj = ref._ptr;
		if (obj) {
			return obj->sub(start, size);
		}
		return sl_null;
	}

	sl_size Memory::read(sl_size startSource, sl_size len, void* bufDst) const
	{
		CMemory* obj = ref._ptr;
		if (obj) {
			return obj->read(startSource, len, (sl_uint8*)bufDst);
		}
		return 0;
	}

	sl_size Memory::write(sl_size startTarget, sl_size len, const void* bufSrc) const
	{
		CMemory* obj = ref._ptr;
		if (obj) {
			return obj->write(startTarget, len, (const sl_uint8*)bufSrc);
		}
		return 0;
	}

	sl_size Memory::copy(sl_size startTarget, const Memory& source, sl_size startSource, sl_size len) const
	{
		CMemory* obj = ref._ptr;
		if (obj) {
			return obj->copy(startTarget, source.ref._ptr, startSource, len);
		}
		return 0;
	}

	sl_size Memory::copy(const Memory& source, sl_size start, sl_size len) const
	{
		return copy(0, source, start, len);
	}

	Memory Memory::duplicate() const
	{
		CMemory* obj = ref._ptr;
		if (obj) {
			return obj->duplicate();
		}
		return sl_null;
	}

	sl_bool Memory::getData(MemoryData& data) const
	{
		CMemory* obj = ref._ptr;
		if (obj) {
			data.data = obj->getData();
			data.size = obj->getCount();
			if (obj->isStatic()) {
				data.refer = obj->getRefer();
			} else {
				data.refer = obj;
			}
			return sl_true;
		} else {
			data.data = sl_null;
			data.size = 0;
			data.refer.setNull();
			return sl_false;
		}
	}


	sl_size Atomic<Memory>::getSize() const
	{
		Ref<CMemory> obj(ref);
		if (obj.isNotNull()) {
			return obj->getCount();
		}
		return 0;
	}

	Memory Atomic<Memory>::sub(sl_size start, sl_size count) const
	{
		Ref<CMemory> obj(ref);
		if (obj.isNotNull()) {
			return obj->sub(start, count);
		}
		return sl_null;
	}

	sl_size Atomic<Memory>::read(sl_size startSource, sl_size len, void* bufDst) const
	{
		Ref<CMemory> obj(ref);
		if (obj.isNotNull()) {
			return obj->read(startSource, len, (sl_uint8*)bufDst);
		}
		return 0;
	}

	sl_size Atomic<Memory>::write(sl_size startTarget, sl_size len, const void* bufSrc) const
	{
		Ref<CMemory> obj(ref);
		if (obj.isNotNull()) {
			return obj->write(startTarget, len, (const sl_uint8*)bufSrc);
		}
		return 0;
	}

	sl_size Atomic<Memory>::copy(sl_size startTarget, const Memory& source, sl_size startSource, sl_size len) const
	{
		Ref<CMemory> obj(ref);
		if (obj.isNotNull()) {
			return obj->copy(startTarget, source.ref._ptr, startSource, len);
		}
		return 0;
	}

	sl_size Atomic<Memory>::copy(const Memory& source, sl_size startSource, sl_size len) const
	{
		Ref<CMemory> obj(ref);
		if (obj.isNotNull()) {
			return obj->copy(0, source.ref._ptr, startSource, len);
		}
		return 0;
	}

	Memory Atomic<Memory>::duplicate() const
	{
		Ref<CMemory> obj(ref);
		if (obj.isNotNull()) {
			return obj->duplicate();
		}
		return sl_null;
	}

	sl_bool Atomic<Memory>::getData(MemoryData& data) const
	{
		Memory mem(*this);
		return mem.getData(data);
	}


/*******************************************
			MemoryData
*******************************************/

	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(MemoryData)
	
	MemoryData::MemoryData()
	 : data(sl_null), size(0)
	{
	}

	Memory MemoryData::getMemory() const
	{
		if (CMemory* mem = CastInstance<CMemory>(refer._ptr)) {
			if (mem->getData() == data && mem->getCount() == size) {
				return mem;
			}
		}
		return Memory::createStatic(data, size, refer._ptr);
	}

	Memory MemoryData::sub(sl_size start, sl_size sizeSub) const
	{
		if (start >= size) {
			return sl_null;
		}
		if (sizeSub > size - start) {
			sizeSub = size - start;
		}
		if (start == 0 && sizeSub == size) {
			return getMemory();
		}
		return Memory::createStatic((sl_uint8*)data + start, sizeSub, refer._ptr);
	}

/*******************************************
			MemoryBuffer
*******************************************/

	MemoryBuffer::MemoryBuffer()
	{
		m_size = 0;
	}

	MemoryBuffer::~MemoryBuffer()
	{
	}

	sl_size MemoryBuffer::getSize() const
	{
		return m_size;
	}

	sl_bool MemoryBuffer::add(const MemoryData& mem)
	{
		if (mem.size == 0) {
			return sl_true;
		}
		if (mem.data) {
			if (m_queue.push_NoLock(mem)) {
				m_size += mem.size;
				return sl_true;
			}
		}
		return sl_false;
	}
	
	sl_bool MemoryBuffer::add(const Memory& mem)
	{
		MemoryData data;
		if (mem.getData(data)) {
			return add(data);
		}
		return sl_true;
	}
	
	sl_bool MemoryBuffer::addStatic(const void* buf, sl_size size)
	{
		if (size == 0) {
			return sl_true;
		}
		if (buf) {
			MemoryData data;
			data.data = (void*)buf;
			data.size = size;
			return add(data);
		}
		return sl_false;
	}

	void MemoryBuffer::link(MemoryBuffer& buf)
	{
		m_size += buf.m_size;
		buf.m_size = 0;
		m_queue.merge_NoLock(&(buf.m_queue));
	}
	
	void MemoryBuffer::clear()
	{
		m_queue.removeAll_NoLock();
		m_size = 0;
	}

	
	Memory MemoryBuffer::merge() const
	{
		if (m_queue.getCount() == 0) {
			return sl_null;
		}
		Link<MemoryData>* front = m_queue.getFront();
		if (m_queue.getCount() == 1) {
			return front->value.getMemory();
		}
		sl_size total = m_size;
		Memory ret = Memory::create(total);
		if (ret.isNotNull()) {
			char* buf = (char*)(ret.getData());
			sl_size offset = 0;
			Link<MemoryData>* item = front;
			while (item) {
				MemoryData& m = item->value;
				sl_size t = m.size;
				Base::copyMemory(buf + offset, m.data, t);
				offset += t;
				item = item->next;
			}
		}
		return ret;
	}

/*******************************************
			MemoryQueue
*******************************************/

	MemoryQueue::MemoryQueue()
	{
		m_size = 0;
		m_memCurrent.data = sl_null;
		m_memCurrent.size = 0;
		m_posCurrent = 0;
	}

	MemoryQueue::~MemoryQueue()
	{
	}
	
	sl_size MemoryQueue::getSize() const
	{
		return m_size;
	}
	
	sl_bool MemoryQueue::add_NoLock(const MemoryData& mem)
	{
		if (mem.size == 0) {
			return sl_true;
		}
		if (mem.data) {
			if (m_queue.push_NoLock(mem)) {
				m_size += mem.size;
				return sl_true;
			}
		}
		return sl_false;
	}
	
	sl_bool MemoryQueue::add(const MemoryData& mem)
	{
		if (mem.size == 0) {
			return sl_true;
		}
		if (mem.data) {
			ObjectLocker lock(this);
			if (m_queue.push_NoLock(mem)) {
				m_size += mem.size;
				return sl_true;
			}
		}
		return sl_false;
	}
	
	sl_bool MemoryQueue::add_NoLock(const Memory& mem)
	{
		MemoryData data;
		if (mem.getData(data)) {
			return add_NoLock(data);
		}
		return sl_true;
	}
	
	sl_bool MemoryQueue::add(const Memory& mem)
	{
		MemoryData data;
		if (mem.getData(data)) {
			return add(data);
		}
		return sl_true;
	}
	
	sl_bool MemoryQueue::addStatic_NoLock(const void* buf, sl_size size)
	{
		if (size == 0) {
			return sl_true;
		}
		if (buf) {
			MemoryData data;
			data.data = (void*)buf;
			data.size = size;
			return add_NoLock(data);
		}
		return sl_false;
	}
	
	sl_bool MemoryQueue::addStatic(const void* buf, sl_size size)
	{
		if (size == 0) {
			return sl_true;
		}
		if (buf) {
			MemoryData data;
			data.data = (void*)buf;
			data.size = size;
			return add(data);
		}
		return sl_false;
	}
	
	void MemoryQueue::link_NoLock(MemoryQueue& buf)
	{
		m_size += buf.m_size;
		buf.m_size = 0;
		m_queue.merge_NoLock(&(buf.m_queue));
	}
	
	void MemoryQueue::link(MemoryQueue& buf)
	{
		MultipleObjectsLocker lock(this, &buf);
		m_size += buf.m_size;
		buf.m_size = 0;
		m_queue.merge_NoLock(&(buf.m_queue));
	}
	
	void MemoryQueue::clear_NoLock()
	{
		m_queue.removeAll_NoLock();
		m_size = 0;
	}
	
	void MemoryQueue::clear()
	{
		ObjectLocker lock(this);
		m_queue.removeAll_NoLock();
		m_size = 0;
	}
	
	sl_bool MemoryQueue::pop_NoLock(MemoryData& data)
	{
		MemoryData mem = m_memCurrent;
		if (mem.size > 0) {
			sl_size pos = m_posCurrent;
			m_memCurrent.size = 0;
			m_posCurrent = 0;
			if (pos < mem.size) {
				data.data = (sl_uint8*)(mem.data) + pos;
				data.size = mem.size - pos;
				data.refer = mem.refer;
				m_size -= data.size;
				return sl_true;
			}
		} else {
			if (m_queue.pop_NoLock(&data)) {
				if (data.size > 0) {
					m_size -= data.size;
					return sl_true;
				}
			}
		}
		return sl_false;
	}

	sl_bool MemoryQueue::pop(MemoryData& data)
	{
		ObjectLocker lock(this);
		return pop_NoLock(data);
	}
	
	sl_size MemoryQueue::pop_NoLock(void* _buf, sl_size size)
	{
		char* buf = (char*)_buf;
		sl_size nRead = 0;
		while (nRead < size) {
			MemoryData mem = m_memCurrent;
			sl_size pos = m_posCurrent;
			m_memCurrent.size = 0;
			m_posCurrent = 0;
			if (mem.size == 0) {
				m_queue.pop_NoLock(&mem);
				pos = 0;
			}
			if (mem.size == 0) {
				break;
			}
			sl_size n = size - nRead;
			sl_size m = mem.size;
			if (pos > m) {
				pos = m;
			}
			m -= pos;
			if (n >= m) {
				Base::copyMemory(buf + nRead, (char*)(mem.data) + pos, m);
				nRead += m;
			} else {
				Base::copyMemory(buf + nRead, (char*)(mem.data) + pos, n);
				nRead += n;
				m_posCurrent = pos + n;
				m_memCurrent = mem;
			}
		}
		m_size -= nRead;
		return nRead;
	}

	sl_size MemoryQueue::pop(void* buf, sl_size size)
	{
		ObjectLocker lock(this);
		return pop_NoLock(buf, size);
	}
	
	Memory MemoryQueue::merge_NoLock() const
	{
		if (m_queue.getCount() == 0) {
			return m_memCurrent.sub(m_posCurrent);
		}
		Link<MemoryData>* front = m_queue.getFront();
		if (m_queue.getCount() == 1 && m_memCurrent.size == 0) {
			return front->value.getMemory();
		}
		sl_size total = m_size;
		Memory ret = Memory::create(total);
		if (ret.isNotNull()) {
			char* buf = (char*)(ret.getData());
			sl_size offset = 0;
			if (m_memCurrent.size > 0) {
				sl_size p = m_posCurrent;
				sl_size s = m_memCurrent.size;
				if (p < s) {
					s -= p;
					Base::copyMemory(buf, (char*)(m_memCurrent.data) + p, s);
					offset = s;
				}
			}
			Link<MemoryData>* item = front;
			while (item) {
				MemoryData& m = item->value;
				sl_size t = m.size;
				Base::copyMemory(buf + offset, m.data, t);
				offset += t;
				item = item->next;
			}
		}
		return ret;
	}

	Memory MemoryQueue::merge() const
	{
		ObjectLocker lock(this);
		return merge_NoLock();
	}

}
