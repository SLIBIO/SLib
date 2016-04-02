#include "../../../inc/slib/core/io.h"
#include "../../../inc/slib/core/mio.h"
#include "../../../inc/slib/core/thread.h"
#include "../../../inc/slib/core/scoped_pointer.h"

SLIB_NAMESPACE_BEGIN

/***************
	IReader
 ***************/
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

sl_bool IReader::readInt8(sl_int8* output)
{
	if (readFully(output, 1) == 1) {
		return sl_true;
	} else {
		return sl_false;
	}
}

sl_int8 IReader::readInt8(sl_int8 def)
{
	sl_int8 ret;
	if (readInt8(&ret)) {
		return ret;
	} else {
		return def;
	}
}

sl_bool IReader::readUint8(sl_uint8* output)
{
	if (readFully(output, 1) == 1) {
		return sl_true;
	} else {
		return sl_false;
	}
}

sl_uint8 IReader::readUint8(sl_uint8 def)
{
	sl_uint8 ret;
	if (readUint8(&ret)) {
		return ret;
	} else {
		return def;
	}
}

sl_bool IReader::readInt16(sl_int16* output, sl_bool flagBigEndian)
{
	if (readFully(output, 2) == 2) {
		if (output) {
			if (flagBigEndian) {
				*output = Endian::swap16LE(*output);
			} else {
				*output = Endian::swap16BE(*output);
			}
		}
		return sl_true;
	} else {
		return sl_false;
	}
}

sl_int16 IReader::readInt16(sl_int16 def, sl_bool flagBigEndian)
{
	sl_int16 ret;
	if (readInt16(&ret, flagBigEndian)) {
		return ret;
	} else {
		return def;
	}
}

sl_bool IReader::readUint16(sl_uint16* output, sl_bool flagBigEndian)
{
	if (readFully(output, 2) == 2) {
		if (output) {
			if (flagBigEndian) {
				*output = Endian::swap16LE(*output);
			} else {
				*output = Endian::swap16BE(*output);
			}
		}
		return sl_true;
	} else {
		return sl_false;
	}
}

sl_uint16 IReader::readUint16(sl_uint16 def, sl_bool flagBigEndian)
{
	sl_uint16 ret;
	if (readUint16(&ret, flagBigEndian)) {
		return ret;
	} else {
		return def;
	}
}

sl_bool IReader::readInt32(sl_int32* output, sl_bool flagBigEndian)
{
	if (readFully(output, 4) == 4) {
		if (output) {
			if (flagBigEndian) {
				*output = Endian::swap32LE(*output);
			} else {
				*output = Endian::swap32BE(*output);
			}
		}
		return sl_true;
	} else {
		return sl_false;
	}
}

sl_int32 IReader::readInt32(sl_int32 def, sl_bool flagBigEndian)
{
	sl_int32 ret;
	if (readInt32(&ret, flagBigEndian)) {
		return ret;
	} else {
		return def;
	}
}

sl_bool IReader::readUint32(sl_uint32* output, sl_bool flagBigEndian)
{
	if (readFully(output, 4) == 4) {
		if (output) {
			if (flagBigEndian) {
				*output = Endian::swap32LE(*output);
			} else {
				*output = Endian::swap32BE(*output);
			}
		}
		return sl_true;
	} else {
		return sl_false;
	}
}

sl_uint32 IReader::readUint32(sl_uint32 def, sl_bool flagBigEndian)
{
	sl_uint32 ret;
	if (readUint32(&ret, flagBigEndian)) {
		return ret;
	} else {
		return def;
	}
}

sl_bool IReader::readInt64(sl_int64* output, sl_bool flagBigEndian)
{
	if (readFully(output, 8) == 8) {
		if (output) {
			if (flagBigEndian) {
				*output = Endian::swap64LE(*output);
			} else {
				*output = Endian::swap64BE(*output);
			}
		}
		return sl_true;
	} else {
		return sl_false;
	}
}

sl_int64 IReader::readInt64(sl_int64 def, sl_bool flagBigEndian)
{
	sl_int64 ret;
	if (readInt64(&ret, flagBigEndian)) {
		return ret;
	} else {
		return def;
	}
}

sl_bool IReader::readUint64(sl_uint64* output, sl_bool flagBigEndian)
{
	if (readFully(output, 8) == 8) {
		if (output) {
			if (flagBigEndian) {
				*output = Endian::swap64LE(*output);
			} else {
				*output = Endian::swap64BE(*output);
			}
		}
		return sl_true;
	} else {
		return sl_false;
	}
}

sl_uint64 IReader::readUint64(sl_uint64 def, sl_bool flagBigEndian)
{
	sl_uint64 ret;
	if (readUint64(&ret, flagBigEndian)) {
		return ret;
	} else {
		return def;
	}
}

sl_bool IReader::readFloat(float* output, sl_bool flagBigEndian)
{
	if (readFully(output, 4) == 4) {
		if (output) {
			if (flagBigEndian) {
				*output = Endian::swapFloatLE(*output);
			} else {
				*output = Endian::swapFloatBE(*output);
			}
		}
		return sl_true;
	} else {
		return sl_false;
	}
}

float IReader::readFloat(float def, sl_bool flagBigEndian)
{
	float ret;
	if (readFloat(&ret, flagBigEndian)) {
		return ret;
	} else {
		return def;
	}
}

sl_bool IReader::readDouble(double* output, sl_bool flagBigEndian)
{
	if (readFully(output, 8) == 8) {
		if (output) {
			if (flagBigEndian) {
				*output = Endian::swapDoubleLE(*output);
			} else {
				*output = Endian::swapDoubleBE(*output);
			}
		}
		return sl_true;
	} else {
		return sl_false;
	}
}

double IReader::readDouble(double def, sl_bool flagBigEndian)
{
	double ret;
	if (readDouble(&ret, flagBigEndian)) {
		return ret;
	} else {
		return def;
	}
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

sl_uint32 IReader::readUint32CVLI(sl_uint32 def)
{
	sl_uint32 ret;
	if (readUint32CVLI(&ret)) {
		return ret;
	} else {
		return def;
	}
}

sl_bool IReader::readInt32CVLI(sl_int32* output)
{
	return readUint32CVLI((sl_uint32*)output);
}

sl_int32 IReader::readInt32CVLI(sl_int32 def)
{
	sl_int32 ret;
	if (readInt32CVLI(&ret)) {
		return ret;
	} else {
		return def;
	}
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

sl_uint64 IReader::readUint64CVLI(sl_uint64 def)
{
	sl_uint64 ret;
	if (readUint64CVLI(&ret)) {
		return ret;
	} else {
		return def;
	}
}

sl_bool IReader::readInt64CVLI(sl_int64* output)
{
	return readUint64CVLI((sl_uint64*)output);
}

sl_int64 IReader::readInt64CVLI(sl_int64 def)
{
	sl_int64 ret;
	if (readInt64CVLI(&ret)) {
		return ret;
	} else {
		return def;
	}
}

sl_bool IReader::readSizeCVLI(sl_size* output)
{
#ifdef SLIB_ARCH_IS_64BIT
	return readUint64CVLI(output);
#else
	return readUint32CVLI(output);
#endif
}

sl_size IReader::readSizeCVLI(sl_size def)
{
#ifdef SLIB_ARCH_IS_64BIT
	return readUint64CVLI(def);
#else
	return readUint32CVLI(def);
#endif
}

sl_bool IReader::readIntCVLI(sl_reg* output)
{
#ifdef SLIB_ARCH_IS_64BIT
	return readInt64CVLI(output);
#else
	return readInt32CVLI(output);
#endif
}

sl_reg IReader::readIntCVLI(sl_reg def)
{
#ifdef SLIB_ARCH_IS_64BIT
	return readInt64CVLI(def);
#else
	return readInt32CVLI(def);
#endif
}

Memory IReader::readToMemory(sl_size size)
{
	Memory mem = Memory::create(size);
	if (mem.isNotEmpty()) {
		size = readFully(mem.getData(), size);
		if (size > 0) {
			mem = mem.sub(0, size);
		} else {
			mem.setNull();
		}
	}
	return mem;
}

sl_bool IReader::readSectionData(void* mem, sl_size& size)
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

sl_bool IReader::readSection(Memory* mem, sl_size maxSize)
{
	sl_size size;
	if (readSizeCVLI(&size)) {
		if (size > maxSize) {
			return sl_false;
		}
		if (size == 0) {
			*mem = Memory::null();
			return sl_true;
		}
		Memory ret = Memory::create(size);
		if (ret.isNotNull()) {
			if (readFully(ret.getData(), size) == size) {
				*mem = ret;
				return sl_true;
			}
		}
	}
	return sl_false;
}

Memory IReader::readSection(const Memory& def, sl_size maxSize)
{
	Memory ret;
	if (readSection(&ret, maxSize)) {
		return ret;
	}
	return def;
}

Memory IReader::readSection(sl_size maxSize)
{
	Memory ret;
	if (readSection(&ret, maxSize)) {
		return ret;
	}
	return Memory::null();
}

sl_bool IReader::readString(String* str, sl_size maxLen)
{
	Memory mem;
	if (readSection(&mem, maxLen)) {
		if (mem.isEmpty()) {
			*str = String::null();
			return sl_true;
		}
		sl_size len = mem.getSize();
		String ret((char*)(mem.getData()), len);
		if (ret.isNotNull()) {
			*str = ret;
			return sl_true;
		}
	}
	return sl_false;
}

String IReader::readString(const String& def, sl_size maxLen)
{
	String ret;
	if (readString(&ret, maxLen)) {
		return ret;
	} else {
		return def;
	}
}

String IReader::readString(sl_size maxLen)
{
	String ret;
	if (readString(&ret, maxLen)) {
		return ret;
	} else {
		return String::null();
	}
}

sl_bool IReader::readBigInt(BigInt* v, sl_size maxLen)
{
	Memory mem;
	if (readSection(&mem, maxLen)) {
		if (mem.isEmpty()) {
			*v = BigInt::null();
			return sl_true;
		}
		sl_size len = mem.getSize();
		BigInt ret = BigInt::fromBytesLE(mem.getData(), len);
		if (ret.isNotNull()) {
			*v = ret;
			return sl_true;
		}
	}
	return sl_false;
}

BigInt IReader::readBigInt(const BigInt& def, sl_size maxLen)
{
	BigInt ret;
	if (readBigInt(&ret, maxLen)) {
		return ret;
	} else {
		return def;
	}
}

BigInt IReader::readBigInt(sl_size maxLen)
{
	BigInt ret;
	if (readBigInt(&ret, maxLen)) {
		return ret;
	} else {
		return BigInt::null();
	}
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

Time IReader::readTime()
{
	Time ret;
	if (readTime(&ret)) {
		return ret;
	} else {
		return Time::zero();
	}
}

Time IReader::readTime(Time def)
{
	Time ret;
	if (readTime(&ret)) {
		return ret;
	} else {
		return def;
	}
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

sl_bool IWriter::writeInt8(sl_int8 value)
{
	return writeFully(&value, 1) == 1;
}

sl_bool IWriter::writeUint8(sl_uint8 value)
{
	return writeInt8(value);
}

sl_bool IWriter::writeInt16(sl_int16 value, sl_bool flagBigEndian)
{
	if (flagBigEndian) {
		value = Endian::swap16LE(value);
	} else {
		value = Endian::swap16BE(value);
	}
	return writeFully(&value, 2) == 2;
}

sl_bool IWriter::writeUint16(sl_uint16 value, sl_bool flagBigEndian)
{
	return writeInt16(value, flagBigEndian);
}

sl_bool IWriter::writeInt32(sl_int32 value, sl_bool flagBigEndian)
{
	if (flagBigEndian) {
		value = Endian::swap32LE(value);
	} else {
		value = Endian::swap32BE(value);
	}
	return writeFully(&value, 4) == 4;
}

sl_bool IWriter::writeUint32(sl_uint32 value, sl_bool flagBigEndian)
{
	return writeInt32(value, flagBigEndian);
}

sl_bool IWriter::writeInt64(sl_int64 value, sl_bool flagBigEndian)
{
	if (flagBigEndian) {
		value = Endian::swap64LE(value);
	} else {
		value = Endian::swap64BE(value);
	}
	return writeFully(&value, 8) == 8;
}

sl_bool IWriter::writeUint64(sl_uint64 value, sl_bool flagBigEndian)
{
	return writeInt64(value, flagBigEndian);
}

sl_bool IWriter::writeFloat(float value, sl_bool flagBigEndian)
{
	if (flagBigEndian) {
		value = Endian::swapFloatLE(value);
	} else {
		value = Endian::swapFloatBE(value);
	}
	return writeFully(&value, 4) == 4;
}

sl_bool IWriter::writeDouble(double value, sl_bool flagBigEndian)
{
	if (flagBigEndian) {
		value = Endian::swapDoubleLE(value);
	} else {
		value = Endian::swapDoubleBE(value);
	}
	return writeFully(&value, 8) == 8;
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

sl_bool IWriter::writeInt32CVLI(sl_int32 value)
{
	return writeUint32CVLI((sl_uint32)value);
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

sl_bool IWriter::writeInt64CVLI(sl_int64 value)
{
	return writeUint64CVLI((sl_uint64)value);
}

sl_bool IWriter::writeSizeCVLI(sl_size value)
{
#ifdef SLIB_ARCH_IS_64BIT
	return writeUint64CVLI(value);
#else
	return writeUint32CVLI(value);
#endif
}

sl_bool IWriter::writeIntCVLI(sl_reg value)
{
#ifdef SLIB_ARCH_IS_64BIT
	return writeInt64CVLI(value);
#else
	return writeInt32CVLI(value);
#endif
}

sl_size IWriter::writeFromMemory(const Memory& mem)
{
	return writeFully(mem.getData(), mem.getSize());
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

sl_bool IWriter::writeSection(const Memory& mem)
{
	return writeSection(mem.getData(), mem.getSize());
}

sl_bool IWriter::writeString(const String& str, sl_size maxLen)
{
	return writeSection(str.getData(), str.getLength());
}

sl_bool IWriter::writeBigInt(const BigInt& v, sl_size maxLen)
{
	sl_size n = v.getMostSignificantBytes();
	if (n > maxLen) {
		n = maxLen;
	}
	SLIB_SCOPED_BUFFER(char, 1024, buf, n);
	v.getBytesLE(buf, n);
	return writeSection(buf, n);
}

sl_bool IWriter::writeTime(const Time& t)
{
	return writeInt64(t.toInt());
}

sl_bool ISeekable::seekToBegin()
{
	return seek(0, SeekPosition::Begin);
}

sl_bool ISeekable::seekToEnd()
{
	return seek(0, SeekPosition::End);
}

/****************************
	IO
 ****************************/
SLIB_DEFINE_OBJECT(IO, Object)

/****************************
	MemoryIO
****************************/
MemoryIO::MemoryIO(sl_size size, sl_bool flagResizable)
{
	init(size, flagResizable);
}

MemoryIO::MemoryIO(const Memory& mem, sl_bool flagResizable)
{
	init(mem);
}

MemoryIO::MemoryIO(const void* data, sl_size size, sl_bool flagResizable)
{
	init(data, size);
}

MemoryIO::~MemoryIO()
{
	close();
}

void MemoryIO::init(const void* data, sl_size size, sl_bool flagResizable)
{
	ObjectLocker lock(this);
	
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

void MemoryIO::init(const Memory& mem, sl_bool flagResizable)
{
	init(mem.getData(), mem.getSize(), flagResizable);
}

void MemoryIO::init(sl_size size, sl_bool flagResizable)
{
	init(sl_null, size, flagResizable);
}

sl_size MemoryIO::getOffset()
{
	return m_offset;
}

sl_size MemoryIO::getLength()
{
	return m_size;
}

char* MemoryIO::getBuffer()
{
	return (char*)m_buf;
}

void MemoryIO::close()
{
	ObjectLocker lock(this);
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
	ObjectLocker lock(this);
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
	ObjectLocker lock(this);
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

sl_bool MemoryIO::seek(sl_int64 offset, SeekPosition pos)
{
	ObjectLocker lock(this);
	sl_uint64 p = m_offset;
	if (pos == SeekPosition::Begin) {
		p = 0;
	} else if (pos == SeekPosition::End) {
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
	ObjectLocker lock(this);
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

/****************************
	MemoryReader
****************************/
MemoryReader::MemoryReader(const Memory& mem)
{
	init(mem);
}

MemoryReader::MemoryReader(const void* buf, sl_size size)
{
	init(buf, size);
}

void MemoryReader::init(const Memory& mem)
{
	ObjectLocker lock(this);
	m_mem = mem;
	m_buf = mem.getData();
	m_size = mem.getSize();
	m_offset = 0;
}

void MemoryReader::init(const void* buf, sl_size size)
{
	ObjectLocker lock(this);
	m_buf = buf;
	m_size = size;
	m_offset = 0;
	if (!buf) {
		m_size = 0;
	}
}

sl_size MemoryReader::getOffset()
{
	return m_offset;
}

sl_size MemoryReader::getLength()
{
	return m_size;
}

char* MemoryReader::getBuffer()
{
	return (char*)m_buf;
}

sl_reg MemoryReader::read(void* buf, sl_size size)
{
	if (size == 0) {
		return 0;
	}
	ObjectLocker lock(this);
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

sl_bool MemoryReader::seek(sl_int64 offset, SeekPosition pos)
{
	ObjectLocker lock(this);
	sl_uint64 p = m_offset;
	if (pos == SeekPosition::Begin) {
		p = 0;
	} else if (pos == SeekPosition::End) {
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

/****************************
	MemoryWriter
****************************/
MemoryWriter::MemoryWriter()
{
	init();
}

MemoryWriter::MemoryWriter(const Memory& mem)
{
	init(mem);
}

MemoryWriter::MemoryWriter(void* buf, sl_size size)
{
	init(buf, size);
}

void MemoryWriter::init()
{
	ObjectLocker lock(this);
	m_buf = sl_null;
	m_size = 0;
	m_offset = 0;
}

void MemoryWriter::init(const Memory& mem)
{
	ObjectLocker lock(this);
	m_mem = mem;
	m_buf = mem.getData();
	m_size = mem.getSize();
	m_offset = 0;
}

void MemoryWriter::init(void* buf, sl_size size)
{
	ObjectLocker lock(this);
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
	ObjectLocker lock(this);
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

sl_reg MemoryWriter::write(const Memory& mem)
{
	if (mem.getSize() == 0) {
		return 0;
	}
	if (m_buf) {
		return write(mem.getData(), mem.getSize());
	} else {
		m_buffer.add(mem);
		return mem.getSize();
	}
}

sl_bool MemoryWriter::seek(sl_int64 offset, SeekPosition pos)
{
	ObjectLocker lock(this);
	if (m_buf) {
		sl_uint64 p = m_offset;
		if (pos == SeekPosition::Begin) {
			p = 0;
		} else if (pos == SeekPosition::End) {
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
	ObjectLocker lock(this);
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

MemoryBuffer& MemoryWriter::getMemoryBuffer()
{
	return m_buffer;
}

sl_size MemoryWriter::getOffset()
{
	if (m_buf) {
		return m_offset;
	} else {
		return m_buffer.getSize();
	}
}

sl_size MemoryWriter::getLength()
{
	if (m_buf) {
		return m_size;
	} else {
		return m_buffer.getSize();
	}
}

char* MemoryWriter::getBuffer()
{
	return (char*)m_buf;
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
	ObjectLocker lock(this);
	m_lenCurrentDatagram = 0;
	m_posCurrentDatagram = 0;
	m_lenBufSize = 0;
	m_buf.clear();
}

sl_bool DatagramStream::parse(const void* _data, sl_size size, Queue<Memory>& datagrams)
{
	sl_uint32 maxDatagram = getMaxDatagramSize();
	ObjectLocker lock(this);
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
				if (m_buf.add(Memory::create(data, n))) {
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
				if (m_buf.add(Memory::create(data, size))) {
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
		sl_uint8* data = (sl_uint8*)(mem.getData());
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
