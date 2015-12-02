#include "../../../inc/slib/core/io.h"
#include "../../../inc/slib/core/thread.h"
#include "../../../inc/slib/core/scoped_pointer.h"

/***************
	IReader
***************/
SLIB_NAMESPACE_BEGIN
sl_int32 IReader::read32(void* buf, sl_uint32 size)
{
	return (sl_int32)(read(buf, size));
}

sl_reg IReader::read(void* _buf, sl_size size)
{
	char* buf = (char*)_buf;
	if (size == 0) {
		return 0;
	}
	sl_size nRead = 0;
	while (nRead < size) {
		sl_size n = size - nRead;
		if (n > 0x40000000) {
			n = 0x40000000; // 1GB
		}
		sl_int32 m = read32(buf + nRead, (sl_uint32)n);
		if (m <= 0) {
			if (nRead) {
				return nRead;
			} else {
				return m;
			}
		}
		nRead += m;
		if (Thread::isStoppingCurrent()) {
			return nRead;
		}
	}
	return nRead;
}

sl_reg IReader::readFully(void* _buf, sl_size size)
{
	char* buf = (char*)_buf;
	if (size == 0) {
		return 0;
	}
	sl_size nRead = 0;
	while (nRead < size) {
		sl_size n = size - nRead;
		sl_reg m = read(buf + nRead, n);
		if (m < 0) {
			if (nRead) {
				return nRead;
			} else {
				return m;
			}
		}
		nRead += m;
		if (Thread::isStoppingCurrent()) {
			return nRead;
		}
		if (m == 0) {
			Thread::sleep(1);
			if (Thread::isStoppingCurrent()) {
				return nRead;
			}
		}
	}
	return nRead;
}

sl_bool IReader::readUint32CVLI(sl_uint32* output)
{
	sl_uint32 v = 0;
	int m = 0;
	while (1) {
		sl_uint8 n = 0;
		if (readFully(&n, 1) == 1) {
			v += (((sl_uint32)(n & 127)) << m);
			m += 7;
			if ((n & 128) == 0) {
				break;
			}
		} else {
			return sl_false;
		}
	}
	*output = v;
	return sl_true;
}

sl_bool IReader::readUint64CVLI(sl_uint64* output)
{
	sl_uint64 v = 0;
	int m = 0;
	while (1) {
		sl_uint8 n = 0;
		if (readFully(&n, 1) == 1) {
			v += (((sl_uint64)(n & 127)) << m);
			m += 7;
			if ((n & 128) == 0) {
				break;
			}
		} else {
			return sl_false;
		}
	}
	*output = v;
	return sl_true;
}

Memory IReader::readToMemory(sl_size size)
{
	Memory mem = Memory::create(size);
	if (mem.isNotEmpty()) {
		size = readFully(mem.getBuf(), size);
		if (size > 0) {
			mem = mem.sub(0, size);
		} else {
			mem.setNull();
		}
	}
	return mem;
}

sl_bool IReader::readSection(void* mem, sl_size& size)
{
	sl_size sizeBuf = size;
	if (readSizeCVLI(&size)) {
		if (size <= sizeBuf) {
			if (readFully(mem, size) == size) {
				return sl_true;
			}
		}
	} else {
		size = 0;
	}
	return sl_false;
}
sl_bool IReader::readSection(Memory* mem, sl_reg maxSize)
{
	sl_size size;
	if (readSizeCVLI(&size)) {
		if (maxSize >= 0 && size > (sl_size)maxSize) {
			return sl_false;
		}
		if (size == 0) {
			*mem = Memory::null();
			return sl_true;
		}
		Memory ret = Memory::create(size);
		if (ret.isNotNull()) {
			if (readFully(ret.getBuf(), size) == size) {
				*mem = ret;
				return sl_true;
			}
		}
	}
	return sl_false;
}

sl_bool IReader::readString(String* str, sl_int32 maxLen)
{
	Memory mem;
	if (readSection(&mem, maxLen)) {
		if (mem.isEmpty()) {
			*str = String::null();
			return sl_true;
		}
		sl_size len = mem.getSize();
		if (len >= SLIB_STR_MAX_LEN) {
			return sl_false;
		}
		String ret((char*)(mem.getBuf()), (sl_uint32)len);
		if (ret.isNotNull()) {
			*str = ret;
			return sl_true;
		}
	}
	return sl_false;
}

sl_bool IReader::readBigInt(BigInt* v, sl_int32 maxLen)
{
	Memory mem;
	if (readSection(&mem, maxLen)) {
		if (mem.isEmpty()) {
			*v = BigInt::null();
			return sl_true;
		}
		sl_size len = mem.getSize();
		if (len >= SLIB_STR_MAX_LEN) {
			return sl_false;
		}
		BigInt ret = BigInt::fromBytesLE(mem.getBuf(), (sl_uint32)len);
		if (ret.isNotNull()) {
			*v = ret;
			return sl_true;
		}
	}
	return sl_false;
}

sl_bool IReader::readTime(Time* v)
{
	sl_int64 m;
	if (readInt64(&m)) {
		*v = m;
		return sl_true;
	}
	return sl_false;
}

/***************
	IWriter
***************/
sl_int32 IWriter::write32(const void* buf, sl_uint32 size)
{
	return (sl_int32)(write(buf, size));
}

sl_reg IWriter::write(const void* _buf, sl_size size)
{
	char* buf = (char*)_buf;
	if (size == 0) {
		return 0;
	}
	sl_size nWrite = 0;
	while (nWrite < size) {
		sl_size n = size - nWrite;
		if (n > 0x40000000) {
			n = 0x40000000; // 1GB
		}
		sl_int32 m = write32(buf + nWrite, (sl_uint32)n);
		if (m <= 0) {
			break;
		}
		nWrite += m;
		if (Thread::isStoppingCurrent()) {
			return nWrite;
		}
	}
	return nWrite;
}

sl_reg IWriter::writeFully(const void* _buf, sl_size size)
{
	char* buf = (char*)_buf;
	if (size == 0) {
		return 0;
	}
	sl_size nWrite = 0;
	while (nWrite < size) {
		sl_size n = size - nWrite;
		sl_reg m = write(buf + nWrite, n);
		if (m < 0) {
			if (nWrite) {
				return nWrite;
			} else {
				return m;
			}
		}
		nWrite += m;
		if (Thread::isStoppingCurrent()) {
			return nWrite;
		}
		if (m == 0) {
			Thread::sleep(1);
			if (Thread::isStoppingCurrent()) {
				return nWrite;
			}
		}
	}
	return nWrite;
}

sl_bool IWriter::writeUint32CVLI(sl_uint32 value)
{
	sl_bool flagContinue = sl_true;
	do {
		sl_uint8 n = ((sl_uint8)value) & 127;
		value = value >> 7;
		if (value != 0) {
			n |= 128;
		} else {
			flagContinue = sl_false;
		}
		if (writeFully(&n, 1) != 1) {
			return sl_false;
		}
	} while (flagContinue);
	return sl_true;
}

sl_bool IWriter::writeUint64CVLI(sl_uint64 value)
{
	sl_bool flagContinue = sl_true;
	do {
		sl_uint8 n = ((sl_uint8)value) & 127;
		value = value >> 7;
		if (value != 0) {
			n |= 128;
		} else {
			flagContinue = sl_false;
		}
		if (writeFully(&n, 1) != 1) {
			return sl_false;
		}
	} while (flagContinue);
	return sl_true;
}

sl_bool IWriter::writeSection(const void* mem, sl_size size)
{
	if (writeSizeCVLI(size)) {
		if (writeFully(mem, size) == size) {
			return sl_true;
		}
	}
	return sl_false;
}

sl_bool IWriter::writeString(const String& _str, sl_int32 maxLen)
{
	String8 str = _str;
	return writeSection(str.getBuf(), str.getLength());
}

sl_bool IWriter::writeBigInt(const BigInt& _v, sl_int32 maxLen)
{
	BigInt v = _v;
	sl_uint32 n = v.getSizeInBytes();
	if (maxLen >= 0 && n > (sl_uint32)maxLen) {
		n = (sl_uint32)maxLen;
	}
	SLIB_SCOPED_BUFFER(char, 1024, buf, n);
	v.getBytesLE(buf, n);
	return writeSection(buf, n);
}

sl_bool IWriter::writeTime(const Time& t)
{
	return writeInt64(t.toInt());
}
SLIB_NAMESPACE_END

/****************************
	MemoryIO
****************************/
SLIB_NAMESPACE_BEGIN
MemoryIO::~MemoryIO()
{
	close();
}

void MemoryIO::init(const void* data, sl_size size, sl_bool flagResizable)
{
	MutexLocker lock(getLocker());
	close();

	if (size > 0) {
		m_buf = Base::createMemory(size);
		if (m_buf) {
			m_size = size;
			if (data) {
				Base::copyMemory(m_buf, data, size);
			}
		} else {
			m_size = 0;
		}
	} else {
		m_buf = sl_null;
		m_size = 0;
	}
	m_offset = 0;
	setAutoExpandable(flagResizable);
}

void MemoryIO::close()
{
	MutexLocker lock(getLocker());
	if (m_buf) {
		Base::freeMemory(m_buf);
		m_buf = sl_null;
	}
	m_size = 0;
	m_offset = 0;
}

sl_reg MemoryIO::read(void* buf, sl_size size)
{
	if (size == 0) {
		return 0;
	}
	MutexLocker lock(getLocker());
	if (m_offset >= m_size) {
		return -1;
	}
	sl_size limit = m_size - m_offset;
	if (size > limit) {
		size = limit;
	}
	if (size > 0) {
		Base::copyMemory(buf, (char*)m_buf + m_offset, size);
		m_offset += size;
	}
	return size;
}

sl_reg MemoryIO::write(const void* buf, sl_size size)
{
	if (size == 0) {
		return 0;
	}
	MutexLocker lock(getLocker());
	if (m_offset >= m_size) {
		return -1;
	}
	sl_size limit = m_size - m_offset;
	if (size > limit) {
		if (isAutoExpandable()) {
			sl_size n = m_size * 3 / 2;
			if (n < m_offset + size) {
				n = m_offset + size;
			}
			if (setSize(n)) {
				limit = m_size - m_offset;
			}
		}
		size = limit;
	}
	if (size > 0) {
		Base::copyMemory((char*)m_buf + m_offset, buf, size);
		m_offset += size;
	}
	return size;
}

sl_bool MemoryIO::seek(sl_int64 offset, Position pos)
{
	MutexLocker lock(getLocker());
	sl_uint64 p = m_offset;
	if (pos == positionBegin) {
		p = 0;
	} else if (pos == positionEnd) {
		p = m_size;
	}
	p += offset;
	if (p > m_size) {
		return sl_false;
	}
	m_offset = (sl_size)p;
	return sl_true;
}

sl_uint64 MemoryIO::getSize()
{
	return getLength();
}

sl_uint64 MemoryIO::getPosition()
{
	return getOffset();
}

sl_bool MemoryIO::setSize(sl_uint64 _size)
{
	sl_size size = (sl_size)_size;
	MutexLocker lock(getLocker());
	if (size == 0) {
		close();
		return sl_true;
	} else {
		void* data;
		if (m_buf) {
			data = Base::reallocMemory(m_buf, size);
		} else {
			data = Base::createMemory(size);
		}
		if (data) {
			m_buf = data;
			m_size = size;
			if (m_offset > size) {
				m_offset = size;
			}
			return sl_true;
		} else {
			return sl_false;
		}
	}
}
SLIB_NAMESPACE_END

/****************************
	MemoryReader
****************************/
SLIB_NAMESPACE_BEGIN
void MemoryReader::init(const Memory& _mem)
{
	MutexLocker lock(getLocker());
	Memory mem = _mem;
	m_mem = mem;
	m_buf = mem.getBuf();
	m_size = mem.getSize();
	m_offset = 0;
}

void MemoryReader::init(const void* buf, sl_size size)
{
	MutexLocker lock(getLocker());
	m_buf = buf;
	m_size = size;
	m_offset = 0;
	if (!buf) {
		m_size = 0;
	}
}

sl_reg MemoryReader::read(void* buf, sl_size size)
{
	if (size == 0) {
		return 0;
	}
	MutexLocker lock(getLocker());
	if (m_offset >= m_size) {
		return -1;
	}
	sl_size limit = m_size - m_offset;
	if (size > limit) {
		size = limit;
	}
	if (size > 0) {
		Base::copyMemory(buf, (char*)m_buf + m_offset, size);
		m_offset += size;
	}
	return size;
}

sl_bool MemoryReader::seek(sl_int64 offset, Position pos)
{
	MutexLocker lock(getLocker());
	sl_uint64 p = m_offset;
	if (pos == positionBegin) {
		p = 0;
	} else if (pos == positionEnd) {
		p = m_size;
	}
	p += offset;
	if (p > m_size) {
		return sl_false;
	}
	m_offset = (sl_size)p;
	return sl_true;
}

sl_uint64 MemoryReader::getSize()
{
	return getLength();
}

sl_uint64 MemoryReader::getPosition()
{
	return getOffset();
}
SLIB_NAMESPACE_END

/****************************
	MemoryWriter
****************************/
SLIB_NAMESPACE_BEGIN
void MemoryWriter::init()
{
	MutexLocker lock(getLocker());
	m_buf = sl_null;
	m_size = 0;
	m_offset = 0;
}

void MemoryWriter::init(const Memory& _mem)
{
	MutexLocker lock(getLocker());
	Memory mem = _mem;
	m_mem = mem;
	m_buf = mem.getBuf();
	m_size = mem.getSize();
	m_offset = 0;
}

void MemoryWriter::init(void* buf, sl_size size)
{
	MutexLocker lock(getLocker());
	m_buf = buf;
	m_size = size;
	m_offset = 0;
	if (!buf) {
		size = 0;
	}
}

sl_reg MemoryWriter::write(const void* buf, sl_size size)
{
	if (size == 0) {
		return 0;
	}
	MutexLocker lock(getLocker());
	if (m_buf) {
		if (m_offset >= m_size) {
			return -1;
		}
		sl_size limit = m_size - m_offset;
		if (size > limit) {
			size = limit;
		}
		if (size > 0) {
			Base::copyMemory((char*)m_buf + m_offset, buf, size);
			m_offset += size;
		}
	} else {
		m_buffer.add(Memory::create(buf, size));
	}
	return size;
}

sl_reg MemoryWriter::write(const Memory& _mem)
{
	Memory mem = _mem;
	if (mem.getSize() == 0) {
		return 0;
	}
	if (m_buf) {
		return write(mem.getBuf(), mem.getSize());
	} else {
		m_buffer.add(mem);
		return mem.getSize();
	}
}

sl_bool MemoryWriter::seek(sl_int64 offset, Position pos)
{
	MutexLocker lock(getLocker());
	if (m_buf) {
		sl_uint64 p = m_offset;
		if (pos == positionBegin) {
			p = 0;
		} else if (pos == positionEnd) {
			p = m_size;
		}
		p += offset;
		if (p > m_size) {
			return sl_false;
		}
		m_offset = (sl_size)p;
		return sl_true;
	} else {
		return sl_false;
	}
}

Memory MemoryWriter::getData()
{
	MutexLocker lock(getLocker());
	if (m_buf) {
		if (m_mem.isNotNull()) {
			return m_mem;
		} else {
			return Memory::createStatic(m_buf, m_size, this);
		}
	} else {
		return m_buffer.merge();
	}
}

sl_uint64 MemoryWriter::getSize()
{
	return getLength();
}

sl_uint64 MemoryWriter::getPosition()
{
	return getOffset();
}

/***********************************
	DatagramStream
***********************************/
DatagramStream::DatagramStream()
{
	setMaxDatagramSize(0);
	clear();
}

void DatagramStream::clear()
{
	MutexLocker lock(getLocker());
	m_lenCurrentDatagram = 0;
	m_posCurrentDatagram = 0;
	m_lenBufSize = 0;
	m_buf.clear();
}

sl_bool DatagramStream::parse(const void* _data, sl_size size, Queue<Memory>& datagrams)
{
	sl_uint32 maxDatagram = getMaxDatagramSize();
	MutexLocker lock(getLocker());
	const sl_uint8* data = (const sl_uint8*)_data;
	while (size > 0) {
		if (m_lenCurrentDatagram == 0) {
			sl_uint32 n = 4 - m_lenBufSize;
			if (n <= size) {
				Base::copyMemory(m_bufSize + m_lenBufSize, data, n);
				m_lenCurrentDatagram = MIO::readUint32LE(m_bufSize);
				data += n;
				size -= n;
				m_lenBufSize = 0;
				m_posCurrentDatagram = 0;
				if (m_lenCurrentDatagram == 0 || (maxDatagram > 0 && m_lenCurrentDatagram > maxDatagram)) {
					clear();
					return sl_false;
				}
			} else {
				Base::copyMemory(m_bufSize + m_lenBufSize, data, size);
				m_lenBufSize += (sl_uint32)size;
				return sl_true;
			}
		} else {
			sl_uint32 n = m_lenCurrentDatagram - m_posCurrentDatagram;
			if (n <= size) {
				if (m_buf.add(data, n)) {
					Memory mem = m_buf.merge();
					if (mem.isNotEmpty()) {
						if (!(datagrams.push(mem))) {
							clear();
							return sl_false;
						}
					} else {
						clear();
						return sl_false;
					}
				} else {
					clear();
					return sl_false;
				}
				m_buf.clear();
				data += n;
				size -= n;
				m_lenBufSize = 0;
				m_lenCurrentDatagram = 0;
				m_posCurrentDatagram = 0;
			} else {
				if (m_buf.add(data, size)) {
					m_posCurrentDatagram += (sl_uint32)size;
					return sl_true;
				} else {
					clear();
					return sl_false;
				}
			}
		}
	}
	return sl_true;
}

Memory DatagramStream::build(const void* datagram, sl_uint32 size)
{
	if (size == 0) {
		return Memory::null();
	}
	sl_uint32 sizeMax = getMaxDatagramSize();
	if (sizeMax > 0 && size > sizeMax) {
		return Memory::null();
	}
	Memory mem = Memory::create(size + 4);
	if (mem.isNotEmpty()) {
		sl_uint8* data = (sl_uint8*)(mem.getBuf());
		MIO::writeUint32LE(data, size);
		Base::copyMemory(data + 4, datagram, size);
	}
	return mem;
}

Memory DatagramStream::build(MemoryBuffer& input)
{
	sl_size size = input.getSize();
	if (size == 0 || size >= 0x80000000) {
		return Memory::null();
	}
	sl_uint32 sizeMax = getMaxDatagramSize();
	if (sizeMax > 0 && size > sizeMax) {
		return Memory::null();
	}
	MemoryWriter writer;
	writer.writeUint32((sl_uint32)size);
	writer.getMemoryBuffer().link(input);
	Memory mem = writer.getData();
	if (mem.getSize() <= 4 || (sizeMax > 0 && mem.getSize() > sizeMax + 4)) {
		mem.setNull();
	}
	return mem;
}
SLIB_NAMESPACE_END
