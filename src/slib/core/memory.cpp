#include "../../../inc/slib/core/memory.h"

SLIB_NAMESPACE_BEGIN

MemoryBuffer::MemoryBuffer()
{
	m_size = 0;
}

MemoryBuffer::~MemoryBuffer()
{
}

sl_bool MemoryBuffer::add(const Memory& mem)
{
	ObjectLocker lock(this);
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
	ObjectLocker lock(this);
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
	ObjectLocker lock(this);
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
	ObjectLocker lock(this);
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
