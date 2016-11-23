#include "../../../inc/slib/core/memory.h"

SLIB_NAMESPACE_BEGIN

SLIB_DEFINE_OBJECT(CMemory, CArray<sl_uint8>)

SLIB_DEFINE_EXPLICIT_INSTANTIATIONS_FOR_LIST(Memory)
SLIB_DEFINE_EXPLICIT_INSTANTIATIONS_FOR_QUEUE(Memory)

CMemory::CMemory()
{
}

CMemory::CMemory(sl_size count) : CArray(count)
{
}

CMemory::CMemory(const sl_uint8* data, sl_size count) : CArray(data, count)
{
}

CMemory::CMemory(const sl_uint8* data, sl_size count, const Referable* refer) : CArray(data, count, refer)
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


CMemory* CMemory::create(const sl_uint8* data, sl_size count)
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

CMemory* CMemory::createStatic(const sl_uint8* data, sl_size count, const Referable* refer)
{
	if (data && count > 0) {
		return new CMemory(data, count, refer);
	}
	return sl_null;
}

CMemory* CMemory::sub(sl_size start, sl_size count) const
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
				return createStatic(m_data + start, count, m_refer.ptr);
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

SLIB_DEFINE_REF_WRAPPER(Memory, SafeMemory, CMemory, ref)

Memory::Memory(sl_size size) : ref(CMemory::create(size))
{
}

Memory::Memory(const void* buf, sl_size size) : ref(CMemory::create((sl_uint8*)buf, size))
{
}

Memory::Memory(const void* buf, sl_size size, const Referable* refer) : ref(CMemory::createStatic((const sl_uint8*)buf, size, refer))
{
}

Memory Memory::create(sl_size count)
{
	return Memory(count);
}

Memory Memory::create(const void* buf, sl_size count)
{
	return Memory(buf, count);
}

Memory Memory::createStatic(const void* buf, sl_size count)
{
	return Memory(buf, count, sl_null);
}

Memory Memory::createStatic(const void* buf, sl_size count, const Referable* refer)
{
	return Memory(buf, count, refer);
}

void* Memory::getData() const
{
	CMemory* obj = ref.ptr;
	if (obj) {
		return obj->getData();
	}
	return sl_null;
}

sl_size Memory::getSize() const
{
	CMemory* obj = ref.ptr;
	if (obj) {
		return obj->getCount();
	}
	return 0;
}

sl_bool Memory::isEmpty() const
{
	return isNull();
}

sl_bool Memory::isNotEmpty() const
{
	return isNotNull();
}

Memory Memory::sub(sl_size start, sl_size size) const
{
	CMemory* obj = ref.ptr;
	if (obj) {
		return obj->sub(start, size);
	}
	return Memory::null();
}

sl_size Memory::read(sl_size startSource, sl_size len, void* bufDst) const
{
	CMemory* obj = ref.ptr;
	if (obj) {
		return obj->read(startSource, len, (sl_uint8*)bufDst);
	}
	return 0;
}

sl_size Memory::write(sl_size startTarget, sl_size len, const void* bufSrc) const
{
	CMemory* obj = ref.ptr;
	if (obj) {
		return obj->write(startTarget, len, (const sl_uint8*)bufSrc);
	}
	return 0;
}

sl_size Memory::copy(sl_size startTarget, const Memory& source, sl_size startSource, sl_size len) const
{
	CMemory* obj = ref.ptr;
	if (obj) {
		return obj->copy(startTarget, source.ref.ptr, startSource, len);
	}
	return 0;
}

sl_size Memory::copy(const Memory& source, sl_size start, sl_size len) const
{
	return copy(0, source, start, len);
}

Memory Memory::duplicate() const
{
	CMemory* obj = ref.ptr;
	if (obj) {
		return obj->duplicate();
	}
	return Memory::null();
}

template <>
int Compare<Memory>::compare(const Memory& a, const Memory& b)
{
	void* p1 = a.ref.ptr;
	void* p2 = b.ref.ptr;
	return (p1 < p2) ? -1 : (p1 > p2);
}

template <>
sl_bool Compare<Memory>::equals(const Memory& a, const Memory& b)
{
	return a.ref.ptr == b.ref.ptr;
}

sl_bool Memory::getData(MemoryData& data) const
{
	CMemory* obj = ref.ptr;
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

SLIB_DEFINE_REF_WRAPPER(SafeMemory, Memory, CMemory, ref)

SafeMemory::SafeMemory(sl_size size) : ref(CMemory::create(size))
{
}

SafeMemory::SafeMemory(const void* buf, sl_size size) : ref(CMemory::create((const sl_uint8*)buf, size))
{
}

SafeMemory::SafeMemory(const void* buf, sl_size size, const Referable* refer) : ref(CMemory::createStatic((const sl_uint8*)buf, size, refer))
{
}

sl_size SafeMemory::getSize() const
{
	Ref<CMemory> obj(ref);
	if (obj.isNotNull()) {
		return obj->getCount();
	}
	return 0;
}

sl_bool SafeMemory::isEmpty() const
{
	Ref<CMemory> obj(ref);
	if (obj.isNotNull()) {
		return obj->getCount() == 0;
	}
	return sl_true;
}

sl_bool SafeMemory::isNotEmpty() const
{
	Ref<CMemory> obj(ref);
	if (obj.isNotNull()) {
		return obj->getCount() != 0;
	}
	return sl_false;
}

Memory SafeMemory::sub(sl_size start, sl_size count) const
{
	Ref<CMemory> obj(ref);
	if (obj.isNotNull()) {
		return obj->sub(start, count);
	}
	return Memory::null();
}

sl_size SafeMemory::read(sl_size startSource, sl_size len, void* bufDst) const
{
	Ref<CMemory> obj(ref);
	if (obj.isNotNull()) {
		return obj->read(startSource, len, (sl_uint8*)bufDst);
	}
	return 0;
}

sl_size SafeMemory::write(sl_size startTarget, sl_size len, const void* bufSrc) const
{
	Ref<CMemory> obj(ref);
	if (obj.isNotNull()) {
		return obj->write(startTarget, len, (const sl_uint8*)bufSrc);
	}
	return 0;
}

sl_size SafeMemory::copy(sl_size startTarget, const Memory& source, sl_size startSource, sl_size len) const
{
	Ref<CMemory> obj(ref);
	if (obj.isNotNull()) {
		return obj->copy(startTarget, source.ref.ptr, startSource, len);
	}
	return 0;
}

sl_size SafeMemory::copy(const Memory& source, sl_size startSource, sl_size len) const
{
	Ref<CMemory> obj(ref);
	if (obj.isNotNull()) {
		return obj->copy(0, source.ref.ptr, startSource, len);
	}
	return 0;
}

Memory SafeMemory::duplicate() const
{
	Ref<CMemory> obj(ref);
	if (obj.isNotNull()) {
		return obj->duplicate();
	}
	return Memory::null();
}

sl_bool SafeMemory::getData(MemoryData& data) const
{
	Memory mem = *this;
	return mem.getData(data);
}


/*******************************************
			MemoryData
*******************************************/

Memory MemoryData::getMemory() const
{
	if (CMemory::checkInstance(refer.ptr)) {
		CMemory* mem = (CMemory*)(refer.ptr);
		if (mem->getData() == data && mem->getCount() == size) {
			return mem;
		}
	}
	return Memory::createStatic(data, size, refer.ptr);
}

Memory MemoryData::sub(sl_size start, sl_size sizeSub) const
{
	if (start >= size) {
		return Memory::null();
	}
	if (sizeSub > size - start) {
		sizeSub = size - start;
	}
	if (start == 0 && sizeSub == size) {
		return getMemory();
	}
	return Memory::createStatic((sl_uint8*)data + start, sizeSub, refer.ptr);
}

/*******************************************
			MemoryBuffer
*******************************************/

MemoryBuffer::MemoryBuffer()
{
	m_size = 0;
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
		ObjectLocker lock(this);
		if (m_queue.push(mem)) {
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
	ObjectLocker lock(this, &buf);
	m_size += buf.m_size;
	buf.m_size = 0;
	m_queue.merge(&(buf.m_queue));
}

void MemoryBuffer::clear()
{
	ObjectLocker lock(this);
	m_queue.removeAll();
	m_size = 0;
}

Memory MemoryBuffer::merge() const
{
	ObjectLocker lock(this);
	if (m_queue.getCount() == 0) {
		return Memory::null();
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
			MemoryBuffer
*******************************************/

MemoryQueue::MemoryQueue()
{
	m_memCurrent.data = sl_null;
	m_memCurrent.size = 0;
	m_posCurrent = 0;
}

sl_bool MemoryQueue::pop(MemoryData& data)
{
	ObjectLocker lock(this);
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
		if (m_queue.pop(&data)) {
			if (data.size > 0) {
				m_size -= data.size;
				return sl_true;
			}
		}
	}
	return sl_false;
}

sl_size MemoryQueue::pop(void* _buf, sl_size size)
{
	ObjectLocker lock(this);
	char* buf = (char*)_buf;
	sl_size nRead = 0;
	while (nRead < size) {
		MemoryData mem = m_memCurrent;
		sl_size pos = m_posCurrent;
		m_memCurrent.size = 0;
		m_posCurrent = 0;
		if (mem.size == 0) {
			m_queue.pop(&mem);
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

Memory MemoryQueue::merge() const
{
	ObjectLocker lock(this);
	if (m_queue.getCount() == 0) {
		return m_memCurrent.sub(m_posCurrent);
	}
	Link<MemoryData>* front = m_queue.getFront();
	if (m_queue.getCount() == 1 && m_memCurrent.size == 0) {
		return front->value.getMemory();
	}
	sl_size total = m_size;
	Memory ret = Memory::create(total);
	if (ret.isNotEmpty()) {
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

SLIB_NAMESPACE_END
