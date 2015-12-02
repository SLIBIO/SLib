#include "../../../inc/slib/core/memory.h"
#include "../../../inc/slib/core/base.h"

SLIB_NAMESPACE_BEGIN
MemoryObject::~MemoryObject()
{
	if (!m_flagStatic) {
		if (m_buf) {
			Base::freeMemory(m_buf);
		}
	}
}

MemoryObject* MemoryObject::create(const void* buf, sl_size size)
{
	if (size == 0) {
		return sl_null;
	}
	void* bufNew = Base::createMemory(size);
	if (bufNew == sl_null) {
		return sl_null;
	}
	MemoryObject* ret = new MemoryObject();
	if (ret) {
		if (buf) {
			Base::copyMemory(bufNew, buf, size);
		}
		ret->m_flagStatic = sl_false;
		ret->m_buf = bufNew;
		ret->m_size = size;
	} else {
		Base::freeMemory(bufNew);
	}
	return ret;
}

MemoryObject* MemoryObject::createStatic(const void* buf, sl_size size, const Ref<Referable>& refer)
{
	if (buf == sl_null || size == 0) {
		return sl_null;
	}
	MemoryObject* ret = new MemoryObject();
	if (ret) {
		ret->m_flagStatic = sl_true;
		ret->m_buf = (void*)buf;
		ret->m_size = size;
		ret->m_refer = refer;
	}
	return ret;
}

Memory Memory::sub(sl_size offset) const
{
	Memory ret;
	Memory mem = *this;
	sl_size sizeParent = mem.getSize();
	if (offset >= sizeParent) {
		return ret;
	}
	ret.m_object = MemoryObject::createStatic((char*)(mem.getBuf()) + offset, sizeParent - offset, mem.getReference());
	return ret;
}

Memory Memory::sub(sl_size offset, sl_size size) const
{
	Memory ret;
	if (size == 0) {
		return ret;
	}
	Memory mem = *this;
	sl_size sizeParent = mem.getSize();
	if (offset >= sizeParent) {
		return ret;
	}
	if (size > sizeParent - offset) {
		size = sizeParent - offset;
	}
	ret.m_object = MemoryObject::createStatic((char*)(mem.getBuf()) + offset, size, mem.getReference());
	return ret;
}

sl_size Memory::write(sl_size offset, sl_size lenSource, const void* source)
{
	Memory mem = *this;
	sl_uint8* pDst = (sl_uint8*)(mem.getBuf());
	sl_uint8* pSrc = (sl_uint8*)source;
	if (pDst == sl_null || pSrc == sl_null) {
		return 0;
	}
	sl_size size = mem.getSize();
	if (offset >= size) {
		return 0;
	}
	if (lenSource > size - offset) {
		lenSource = size - offset;
	}
	Base::copyMemory(pDst + offset, pSrc, lenSource);
	return lenSource;
}

sl_size Memory::read(sl_size offset, sl_size lenOutput, void* output)
{
	Memory mem = *this;
	sl_uint8* pSrc = (sl_uint8*)(mem.getBuf());
	sl_uint8* pDst = (sl_uint8*)output;
	if (pDst == sl_null || pSrc == sl_null) {
		return 0;
	}
	sl_size size = mem.getSize();
	if (offset >= size) {
		return 0;
	}
	if (lenOutput > size - offset) {
		lenOutput = size - offset;
	}
	Base::copyMemory(pDst, pSrc + offset, lenOutput);
	return lenOutput;
}

sl_size Memory::copy(sl_size offsetTarget, Memory source, sl_size offsetSource, sl_size len)
{
	Memory mem = *this;
	sl_uint8* pDst = (sl_uint8*)(mem.getBuf());
	sl_uint8* pSrc = (sl_uint8*)(source.getBuf());
	if (pDst == sl_null || pSrc == sl_null) {
		return 0;
	}
	sl_size sizeDst = mem.getSize();
	if (offsetTarget >= sizeDst) {
		return 0;
	}
	sl_size sizeSrc = source.getSize();
	if (offsetSource >= sizeSrc) {
		return 0;
	}
	sl_size lenDst = sizeDst - offsetTarget;
	sl_size lenSrc = sizeSrc - offsetSource;
	if (lenSrc > len) {
		lenSrc = len;
	}
	if (lenDst > lenSrc) {
		lenDst = lenSrc;
	}
	pDst += offsetTarget;
	pSrc += offsetSource;
	Base::copyMemory(pDst, pSrc, lenDst);
	return lenDst;
}

MemoryBuffer::MemoryBuffer()
{
	m_size = 0;
}

MemoryBuffer::~MemoryBuffer()
{
}

sl_bool MemoryBuffer::add(const Memory& mem)
{
	MutexLocker lock(getLocker());
	sl_size size = mem.getSize();
	if (m_queue.push(mem)) {
		m_size += size;
		return sl_true;
	} else {
		return sl_false;
	}
}

sl_bool MemoryBuffer::add(const void* buf, sl_size size)
{
	if (size == 0) {
		return 0;
	}
	return add(Memory::create(buf, size));
}

void MemoryBuffer::link(MemoryBuffer& buf)
{
	MutexLocker lock(getLocker(), buf.getLocker());
	m_size += buf.m_size;
	buf.m_size = 0;
	m_queue.merge(&(buf.m_queue));
}

void MemoryBuffer::clear()
{
	MutexLocker lock(getLocker());
	m_queue.clear();
	m_size = 0;
}

Memory MemoryBuffer::merge() const
{
	MutexLocker lock(getLocker());
	if (m_queue.count() == 0) {
		return Memory::null();
	}
	Link<Memory>* begin = m_queue.getBegin();
	if (m_queue.count() == 1) {
		return begin->value;
	}
	sl_size total = m_size;
	Memory ret = Memory::create(total);
	if (ret.isNotNull()) {
		char* buf = (char*)(ret.getBuf());
		sl_size offset = 0;
		Link<Memory>* item = begin;
		while (item) {
			Memory& m = item->value;
			sl_size t = m.getSize();
			Base::copyMemory(buf + offset, m.getBuf(), t);
			offset += t;
			item = item->next;
		}
	}
	return ret;
}

MemoryQueue::MemoryQueue()
{
	m_posCurrent = 0;
}

MemoryQueue::~MemoryQueue()
{
}

Memory MemoryQueue::pop()
{
	MutexLocker lock(getLocker());
	Memory mem = m_memCurrent;
	if (mem.isNotEmpty()) {
		sl_size pos = m_posCurrent;
		m_memCurrent.setNull();
		m_posCurrent = 0;
		sl_size size = mem.getSize();
		if (pos >= size) {
			return Memory::null();
		} else {
			m_size -= (size - pos);
			return mem.sub(pos);
		}
	} else {
		m_queue.pop(&mem);
		m_size -= mem.getSize();
		return mem;
	}
}

sl_size MemoryQueue::pop(void* _buf, sl_size size)
{
	MutexLocker lock(getLocker());
	char* buf = (char*)_buf;
	sl_size nRead = 0;
	while (nRead < size) {
		Memory mem = m_memCurrent;
		sl_size pos = m_posCurrent;
		m_memCurrent.setNull();
		m_posCurrent = 0;
		if (mem.isEmpty()) {
			m_queue.pop(&mem);
			pos = 0;
		}
		if (mem.isEmpty()) {
			break;
		}
		sl_size n = size - nRead;
		sl_size m = mem.getSize();
		if (pos > m) {
			pos = m;
		}
		m -= pos;
		if (n >= m) {
			Base::copyMemory(buf + nRead, (char*)(mem.getBuf()) + pos, m);
			nRead += m;
		} else {
			Base::copyMemory(buf + nRead, (char*)(mem.getBuf()) + pos, n);
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
	MutexLocker lock(getLocker());
	if (m_queue.count() == 0) {
		return m_memCurrent.sub(m_posCurrent);
	}
	Link<Memory>* begin = m_queue.getBegin();
	if (m_queue.count() == 1 && m_memCurrent.isEmpty()) {
		return begin->value;
	}
	sl_size total = m_size;
	Memory ret = Memory::create(total);
	if (ret.isNotEmpty()) {
		char* buf = (char*)(ret.getBuf());
		sl_size offset = 0;
		if (m_memCurrent.isNotEmpty()) {
			sl_size p = m_posCurrent;
			sl_size s = m_memCurrent.size();
			if (p < s) {
				s -= p;
				Base::copyMemory(buf, (char*)(m_memCurrent.getBuf()) + p, s);
				offset = s;
			}
		}
		Link<Memory>* item = begin;
		while (item) {
			Memory& m = item->value;
			sl_size t = m.getSize();
			Base::copyMemory(buf + offset, m.getBuf(), t);
			offset += t;
			item = item->next;
		}
	}
	return ret;
}

SLIB_NAMESPACE_END
