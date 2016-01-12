#ifndef CHECKHEADER_SLIB_CORE_IO
#define CHECKHEADER_SLIB_CORE_IO

#include "definition.h"

#include "mio.h"
#include "object.h"
#include "memory.h"
#include "time.h"
#include "../math/bigint.h"

SLIB_NAMESPACE_BEGIN

class SLIB_EXPORT IReader
{
public:
	virtual sl_reg read(void* buf, sl_size size);
	
	virtual sl_int32 read32(void* buf, sl_uint32 size);

public:
	sl_reg readFully(void* buf, sl_size size);

public:
	SLIB_INLINE sl_bool readInt8(sl_int8* output)
	{
		if (readFully(output, 1) == 1) {
			return sl_true;
		} else {
			return sl_false;
		}
	}
	
	SLIB_INLINE sl_int8 readInt8(sl_int8 def = 0)
	{
		sl_int8 ret;
		if (readInt8(&ret)) {
			return ret;
		} else {
			return def;
		}
	}
	
	SLIB_INLINE sl_bool readUint8(sl_uint8* output)
	{
		if (readFully(output, 1) == 1) {
			return sl_true;
		} else {
			return sl_false;
		}
	}
	
	SLIB_INLINE sl_uint8 readUint8(sl_uint8 def = 0)
	{
		sl_uint8 ret;
		if (readUint8(&ret)) {
			return ret;
		} else {
			return def;
		}
	}

	
	SLIB_INLINE sl_bool readInt16(sl_int16* output, sl_bool flagBigEndian = sl_false)
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
	
	SLIB_INLINE sl_int16 readInt16(sl_int16 def = 0, sl_bool flagBigEndian = sl_false)
	{
		sl_int16 ret;
		if (readInt16(&ret, flagBigEndian)) {
			return ret;
		} else {
			return def;
		}
	}
	
	SLIB_INLINE sl_bool readUint16(sl_uint16* output, sl_bool flagBigEndian = sl_false)
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
	
	SLIB_INLINE sl_uint16 readUint16(sl_uint16 def = 0, sl_bool flagBigEndian = sl_false)
	{
		sl_uint16 ret;
		if (readUint16(&ret, flagBigEndian)) {
			return ret;
		} else {
			return def;
		}
	}

	
	SLIB_INLINE sl_bool readInt32(sl_int32* output, sl_bool flagBigEndian = sl_false)
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
	
	SLIB_INLINE sl_int32 readInt32(sl_int32 def = 0, sl_bool flagBigEndian = sl_false)
	{
		sl_int32 ret;
		if (readInt32(&ret, flagBigEndian)) {
			return ret;
		} else {
			return def;
		}
	}
	
	SLIB_INLINE sl_bool readUint32(sl_uint32* output, sl_bool flagBigEndian = sl_false)
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
	
	SLIB_INLINE sl_uint32 readUint32(sl_uint32 def = 0, sl_bool flagBigEndian = sl_false)
	{
		sl_uint32 ret;
		if (readUint32(&ret, flagBigEndian)) {
			return ret;
		} else {
			return def;
		}
	}

	
	SLIB_INLINE sl_bool readInt64(sl_int64* output, sl_bool flagBigEndian = sl_false)
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
	
	SLIB_INLINE sl_int64 readInt64(sl_int64 def = 0, sl_bool flagBigEndian = sl_false)
	{
		sl_int64 ret;
		if (readInt64(&ret, flagBigEndian)) {
			return ret;
		} else {
			return def;
		}
	}
	
	SLIB_INLINE sl_bool readUint64(sl_uint64* output, sl_bool flagBigEndian = sl_false)
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
	
	SLIB_INLINE sl_uint64 readUint64(sl_uint64 def = 0, sl_bool flagBigEndian = sl_false)
	{
		sl_uint64 ret;
		if (readUint64(&ret, flagBigEndian)) {
			return ret;
		} else {
			return def;
		}
	}

	
	SLIB_INLINE sl_bool readFloat(float* output, sl_bool flagBigEndian = sl_false)
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
	
	SLIB_INLINE float readFloat(float def = 0, sl_bool flagBigEndian = sl_false)
	{
		float ret;
		if (readFloat(&ret, flagBigEndian)) {
			return ret;
		} else {
			return def;
		}
	}

	
	SLIB_INLINE sl_bool readDouble(double* output, sl_bool flagBigEndian = sl_false)
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
	
	SLIB_INLINE double readDouble(double def = 0, sl_bool flagBigEndian = sl_false)
	{
		double ret;
		if (readDouble(&ret, flagBigEndian)) {
			return ret;
		} else {
			return def;
		}
	}

	
	//  CVLI (Chain Variable Length Integer)
	sl_bool readUint32CVLI(sl_uint32* output);
	
	SLIB_INLINE sl_uint32 readUint32CVLI(sl_uint32 def = 0)
	{
		sl_uint32 ret;
		if (readUint32CVLI(&ret)) {
			return ret;
		} else {
			return def;
		}
	}
	
	SLIB_INLINE sl_bool readInt32CVLI(sl_int32* output)
	{
		return readUint32CVLI((sl_uint32*)output);
	}
	
	SLIB_INLINE sl_int32 readInt32CVLI(sl_int32 def = 0)
	{
		sl_int32 ret;
		if (readInt32CVLI(&ret)) {
			return ret;
		} else {
			return def;
		}
	}

	//  CVLI (Chain Variable Length Integer)
	sl_bool readUint64CVLI(sl_uint64* output);

	SLIB_INLINE sl_uint64 readUint64CVLI(sl_uint64 def = 0)
	{
		sl_uint64 ret;
		if (readUint64CVLI(&ret)) {
			return ret;
		} else {
			return def;
		}
	}
	
	SLIB_INLINE sl_bool readInt64CVLI(sl_int64* output)
	{
		return readUint64CVLI((sl_uint64*)output);
	}
	
	SLIB_INLINE sl_int64 readInt64CVLI(sl_int64 def = 0)
	{
		sl_int64 ret;
		if (readInt64CVLI(&ret)) {
			return ret;
		} else {
			return def;
		}
	}
	
	
	SLIB_INLINE sl_bool readSizeCVLI(sl_size* output)
	{
#ifdef SLIB_ARCH_IS_64BIT
		return readUint64CVLI(output);
#else
		return readUint32CVLI(output);
#endif
	}
	
	SLIB_INLINE sl_size readSizeCVLI(sl_size def = 0)
	{
#ifdef SLIB_ARCH_IS_64BIT
		return readUint64CVLI(def);
#else
		return readUint32CVLI(def);
#endif
	}
	
	SLIB_INLINE sl_bool readIntCVLI(sl_reg* output)
	{
#ifdef SLIB_ARCH_IS_64BIT
		return readInt64CVLI(output);
#else
		return readInt32CVLI(output);
#endif
	}
	
	SLIB_INLINE sl_reg readIntCVLI(sl_reg def = 0)
	{
#ifdef SLIB_ARCH_IS_64BIT
		return readInt64CVLI(def);
#else
		return readInt32CVLI(def);
#endif
	}

	
	Memory readToMemory(sl_size size);

	sl_bool readSection(void* mem, sl_size& size);
	
	sl_bool readSection(Memory* mem, sl_reg maxSize = -1);
	
	SLIB_INLINE Memory readSection(const Memory& def)
	{
		Memory ret;
		if (readSection(&ret)) {
			return ret;
		} else {
			return def;
		}
	}
	
	SLIB_INLINE Memory readSection(sl_reg maxSize = -1)
	{
		Memory ret;
		if (readSection(&ret, maxSize)) {
			return ret;
		} else {
			return Memory::null();
		}
	}

	// maxLen means the maximum length of utf-8 string
	sl_bool readString(String* str, sl_int32 maxLen = -1);
	
	SLIB_INLINE String readString(const String& def)
	{
		String ret;
		if (readString(&ret)) {
			return ret;
		} else {
			return def;
		}
	}
	
	SLIB_INLINE String readString(sl_int32 maxLen, const String& def)
	{
		String ret;
		if (readString(&ret, maxLen)) {
			return ret;
		} else {
			return def;
		}
	}
	
	SLIB_INLINE String readString(sl_int32 maxLen = -1)
	{
		String ret;
		if (readString(&ret, maxLen)) {
			return ret;
		} else {
			return String::null();
		}
	}

	
	sl_bool readBigInt(BigInt* v, sl_int32 maxLen = -1);
	
	SLIB_INLINE BigInt readBigInt(const BigInt& def)
	{
		BigInt ret;
		if (readBigInt(&ret)) {
			return ret;
		} else {
			return def;
		}
	}
	
	SLIB_INLINE BigInt readBigInt(sl_int32 maxLen, const BigInt& def)
	{
		BigInt ret;
		if (readBigInt(&ret, maxLen)) {
			return ret;
		} else {
			return def;
		}
	}
	
	SLIB_INLINE BigInt readBigInt(sl_int32 maxLen = -1)
	{
		BigInt ret;
		if (readBigInt(&ret, maxLen)) {
			return ret;
		} else {
			return BigInt::null();
		}
	}

	
	sl_bool readTime(Time* output);
	
	SLIB_INLINE Time readTime()
	{
		Time ret;
		if (readTime(&ret)) {
			return ret;
		} else {
			return Time::zero();
		}
	}
	
	SLIB_INLINE Time readTime(Time def)
	{
		Time ret;
		if (readTime(&ret)) {
			return ret;
		} else {
			return def;
		}
	}

};


class SLIB_EXPORT IWriter
{
public:
	virtual sl_reg write(const void* buf, sl_size size);
	
	virtual sl_int32 write32(const void* buf, sl_uint32 size);

public:
	sl_reg writeFully(const void* buf, sl_size size);

public:
	SLIB_INLINE sl_bool writeInt8(sl_int8 value)
	{
		return writeFully(&value, 1) == 1;
	}
	
	SLIB_INLINE sl_bool writeUint8(sl_uint8 value)
	{
		return writeInt8(value);
	}

	
	SLIB_INLINE sl_bool writeInt16(sl_int16 value, sl_bool flagBigEndian = sl_false)
	{
		if (flagBigEndian) {
			value = Endian::swap16LE(value);
		} else {
			value = Endian::swap16BE(value);
		}
		return writeFully(&value, 2) == 2;
	}
	
	SLIB_INLINE sl_bool writeUint16(sl_uint16 value, sl_bool flagBigEndian = sl_false)
	{
		return writeInt16(value, flagBigEndian);
	}

	
	SLIB_INLINE sl_bool writeInt32(sl_int32 value, sl_bool flagBigEndian = sl_false)
	{
		if (flagBigEndian) {
			value = Endian::swap32LE(value);
		} else {
			value = Endian::swap32BE(value);
		}
		return writeFully(&value, 4) == 4;
	}
	
	SLIB_INLINE sl_bool writeUint32(sl_uint32 value, sl_bool flagBigEndian = sl_false)
	{
		return writeInt32(value, flagBigEndian);
	}

	
	SLIB_INLINE sl_bool writeInt64(sl_int64 value, sl_bool flagBigEndian = sl_false)
	{
		if (flagBigEndian) {
			value = Endian::swap64LE(value);
		} else {
			value = Endian::swap64BE(value);
		}
		return writeFully(&value, 8) == 8;
	}
	
	SLIB_INLINE sl_bool writeUint64(sl_uint64 value, sl_bool flagBigEndian = sl_false)
	{
		return writeInt64(value, flagBigEndian);
	}

	
	SLIB_INLINE sl_bool writeFloat(float value, sl_bool flagBigEndian = sl_false)
	{
		if (flagBigEndian) {
			value = Endian::swapFloatLE(value);
		} else {
			value = Endian::swapFloatBE(value);
		}
		return writeFully(&value, 4) == 4;
	}

	
	SLIB_INLINE sl_bool writeDouble(double value, sl_bool flagBigEndian = sl_false)
	{
		if (flagBigEndian) {
			value = Endian::swapDoubleLE(value);
		} else {
			value = Endian::swapDoubleBE(value);
		}
		return writeFully(&value, 8) == 8;
	}

	
	//  CVLI (Chain Variable Length Integer)
	sl_bool writeUint32CVLI(sl_uint32 value);
	
	SLIB_INLINE sl_bool writeInt32CVLI(sl_int32 value)
	{
		return writeUint32CVLI((sl_uint32)value);
	}
	

	sl_bool writeUint64CVLI(sl_uint64 value);
	
	SLIB_INLINE sl_bool writeInt64CVLI(sl_int64 value)
	{
		return writeUint64CVLI((sl_uint64)value);
	}
	
	
	SLIB_INLINE sl_bool writeSizeCVLI(sl_size value)
	{
#ifdef SLIB_ARCH_IS_64BIT
		return writeUint64CVLI(value);
#else
		return writeUint32CVLI(value);
#endif
	}
	
	SLIB_INLINE sl_bool writeIntCVLI(sl_reg value)
	{
#ifdef SLIB_ARCH_IS_64BIT
		return writeInt64CVLI(value);
#else
		return writeInt32CVLI(value);
#endif
	}

	
	SLIB_INLINE sl_size writeFromMemory(const Memory& mem)
	{
		return writeFully(mem.getBuf(), mem.getSize());
	}

	
	sl_bool writeSection(const void* mem, sl_size size);
	
	SLIB_INLINE sl_bool writeSection(const Memory& mem)
	{
		return writeSection(mem.getBuf(), mem.getSize());
	}

	// maxLen means the maximum length of utf-8 string
	sl_bool writeString(const String& str, sl_int32 maxLen = -1);

	sl_bool writeBigInt(const BigInt& v, sl_int32 maxLen = -1);

	sl_bool writeTime(const Time& t);
};


class SLIB_EXPORT IStream : public IReader, public IWriter
{
};


class SLIB_EXPORT ISeekable
{
public:
	enum Position {
		positionCurrent = 1,
		positionBegin = 2,
		positionEnd = 3
	};

	virtual sl_uint64 getPosition() = 0;
	
	virtual sl_uint64 getSize() = 0;
	
	virtual sl_bool seek(sl_int64 offset, Position pos = positionCurrent) = 0;

public:
	sl_bool seekToBegin()
	{
		return seek(0, positionBegin);
	}
	
	sl_bool seekToEnd()
	{
		return seek(0, positionEnd);
	}
};

class SLIB_EXPORT IResizable
{
public:
	virtual sl_bool setSize(sl_uint64 size) = 0;
};

class SLIB_EXPORT IClosable
{
public:
	virtual void close() = 0;
};

class SLIB_EXPORT IO : public Object, public IStream, public ISeekable, public IResizable, public IClosable
{
	SLIB_DECLARE_OBJECT(IO, Object)
public:
	SLIB_INLINE IO() {}

};

class SLIB_EXPORT MemoryIO : public IO
{
public:
	SLIB_INLINE MemoryIO(sl_size size = 0, sl_bool flagResizable = sl_true)
	{
		init(size, flagResizable);
	}
	
	SLIB_INLINE MemoryIO(const Memory& mem, sl_bool flagResizable = sl_true)
	{
		init(mem);
	}
	
	SLIB_INLINE MemoryIO(const void* data, sl_size size, sl_bool flagResizable = sl_true)
	{
		init(data, size);
	}

	~MemoryIO();

public:
	SLIB_INLINE void init(Memory mem, sl_bool flagResizable = sl_true)
	{
		init(mem.getBuf(), mem.getSize(), flagResizable);
	}
	
	SLIB_INLINE void init(sl_size size = 0, sl_bool flagResizable = sl_true)
	{
		init(sl_null, size, flagResizable);
	}
	
	void init(const void* data, sl_size size, sl_bool flagResizable = sl_true);

	void close();

	
	sl_reg read(void* buf, sl_size size);

	sl_reg write(const void* buf, sl_size size);

	
	sl_uint64 getPosition();
	
	sl_uint64 getSize();
	
	sl_bool seek(sl_int64 offset, Position pos = positionCurrent);

	
	sl_bool setSize(sl_uint64 size);

	
	SLIB_INLINE sl_size getOffset()
	{
		return m_offset;
	}

	SLIB_INLINE sl_size getLength()
	{
		return m_size;
	}

	SLIB_INLINE char* getBuffer()
	{
		return (char*)m_buf;
	}

public:
	SLIB_BOOLEAN_PROPERTY_INLINE(AutoExpandable)

protected:
	void* m_buf;
	sl_size m_size;
	sl_size m_offset;
};

class SLIB_EXPORT MemoryReader : public Object, public IReader, public ISeekable
{
public:
	SLIB_INLINE MemoryReader(const Memory& mem)
	{
		init(mem);
	}

	SLIB_INLINE MemoryReader(const void* buf, sl_size size)
	{
		init(buf, size);
	}

public:
	void init(const Memory& mem);
	
	void init(const void* buf, sl_size size);

	
	sl_reg read(void* buf, sl_size size);

	
	sl_uint64 getPosition();
	
	sl_uint64 getSize();
	
	sl_bool seek(sl_int64 offset, Position pos = positionCurrent);

	
	SLIB_INLINE sl_size getOffset()
	{
		return m_offset;
	}

	SLIB_INLINE sl_size getLength()
	{
		return m_size;
	}

	SLIB_INLINE char* getBuffer()
	{
		return (char*)m_buf;
	}

protected:
	const void* m_buf;
	sl_size m_size;
	sl_size m_offset;
	Memory m_mem;
};

class SLIB_EXPORT MemoryWriter : public Object, public IWriter, public ISeekable
{
public:
	// write-only/appending memory
	SLIB_INLINE MemoryWriter()
	{
		init();
	}
	
	SLIB_INLINE MemoryWriter(const Memory& mem)
	{
		init(mem);
	}
	
	SLIB_INLINE MemoryWriter(void* buf, sl_size size)
	{
		init(buf, size);
	}

public:
	void init();
	
	void init(const Memory& mem);
	
	void init(void* buf, sl_size size);

	
	sl_reg write(const void* buf, sl_size size);
	
	sl_reg write(const Memory& mem);

	
	sl_uint64 getPosition();
	
	sl_uint64 getSize();
	
	sl_bool seek(sl_int64 offset, Position pos = positionCurrent);

	
	Memory getData();

	
	SLIB_INLINE MemoryBuffer& getMemoryBuffer()
	{
		return m_buffer;
	}

	SLIB_INLINE sl_size getOffset()
	{
		if (m_buf) {
			return m_offset;
		} else {
			return m_buffer.getSize();
		}
	}

	SLIB_INLINE sl_size getLength()
	{
		if (m_buf) {
			return m_size;
		} else {
			return m_buffer.getSize();
		}
	}

	SLIB_INLINE char* getBuffer()
	{
		return (char*)m_buf;
	}

protected:
	void* m_buf;
	sl_size m_size;
	sl_size m_offset;
	Memory m_mem;
	MemoryBuffer m_buffer;
};

/*
		DatagramStream

	Datagram1-Length (4 bytes, LE)
	Datagram1-Content
	Datagram2-Length (4 bytes, LE)
	Datagram2-Content
	...
*/

class SLIB_EXPORT DatagramStream : public Object
{
public:
	DatagramStream();

public:
	void clear();
	
	sl_bool parse(const void* data, sl_size size, Queue<Memory>& datagrams);
	
	Memory build(const void* datagram, sl_uint32 size);
	
	Memory build(MemoryBuffer& buf);

public:
	SLIB_PROPERTY_INLINE(sl_uint32, MaxDatagramSize)

protected:
	MemoryBuffer m_buf;
	sl_uint32 m_lenCurrentDatagram;
	sl_uint32 m_posCurrentDatagram;
	sl_uint8 m_bufSize[4];
	sl_uint32 m_lenBufSize;
};

SLIB_NAMESPACE_END

#endif
