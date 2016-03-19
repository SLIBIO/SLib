#ifndef CHECKHEADER_SLIB_CORE_IO
#define CHECKHEADER_SLIB_CORE_IO

#include "definition.h"

#include "object.h"
#include "memory.h"
#include "time.h"
#include "../math/bigint.h"

SLIB_NAMESPACE_BEGIN

enum SeekPosition
{
	Current = 1,
	Begin = 2,
	End = 3
};

class SLIB_EXPORT IReader
{
public:
	virtual sl_reg read(void* buf, sl_size size);
	
	virtual sl_int32 read32(void* buf, sl_uint32 size);

public:
	sl_reg readFully(void* buf, sl_size size);

	sl_bool readInt8(sl_int8* output);
	
	sl_int8 readInt8(sl_int8 def = 0);
	
	sl_bool readUint8(sl_uint8* output);
	
	sl_uint8 readUint8(sl_uint8 def = 0);
	
	sl_bool readInt16(sl_int16* output, sl_bool flagBigEndian = sl_false);
	
	sl_int16 readInt16(sl_int16 def = 0, sl_bool flagBigEndian = sl_false);

	sl_bool readUint16(sl_uint16* output, sl_bool flagBigEndian = sl_false);
	
	sl_uint16 readUint16(sl_uint16 def = 0, sl_bool flagBigEndian = sl_false);

	sl_bool readInt32(sl_int32* output, sl_bool flagBigEndian = sl_false);
	
	sl_int32 readInt32(sl_int32 def = 0, sl_bool flagBigEndian = sl_false);
	
	sl_bool readUint32(sl_uint32* output, sl_bool flagBigEndian = sl_false);
	
	sl_uint32 readUint32(sl_uint32 def = 0, sl_bool flagBigEndian = sl_false);
	
	sl_bool readInt64(sl_int64* output, sl_bool flagBigEndian = sl_false);
	
	sl_int64 readInt64(sl_int64 def = 0, sl_bool flagBigEndian = sl_false);

	sl_bool readUint64(sl_uint64* output, sl_bool flagBigEndian = sl_false);

	sl_uint64 readUint64(sl_uint64 def = 0, sl_bool flagBigEndian = sl_false);
	
	sl_bool readFloat(float* output, sl_bool flagBigEndian = sl_false);
	
	float readFloat(float def = 0, sl_bool flagBigEndian = sl_false);

	sl_bool readDouble(double* output, sl_bool flagBigEndian = sl_false);
	
	double readDouble(double def = 0, sl_bool flagBigEndian = sl_false);
	
	//  CVLI (Chain Variable Length Integer)
	sl_bool readUint32CVLI(sl_uint32* output);
	
	sl_uint32 readUint32CVLI(sl_uint32 def = 0);
	
	sl_bool readInt32CVLI(sl_int32* output);
	
	sl_int32 readInt32CVLI(sl_int32 def = 0);

	//  CVLI (Chain Variable Length Integer)
	sl_bool readUint64CVLI(sl_uint64* output);

	sl_uint64 readUint64CVLI(sl_uint64 def = 0);
	
	sl_bool readInt64CVLI(sl_int64* output);
	
	sl_int64 readInt64CVLI(sl_int64 def = 0);
	
	sl_bool readSizeCVLI(sl_size* output);
	
	sl_size readSizeCVLI(sl_size def = 0);
	
	sl_bool readIntCVLI(sl_reg* output);
	
	sl_reg readIntCVLI(sl_reg def = 0);
	
	Memory readToMemory(sl_size size);

	sl_bool readSection(void* mem, sl_size& size);
	
	sl_bool readSection(Memory* mem, sl_reg maxSize = -1);
	
	Memory readSection(const Memory& def);
	
	Memory readSection(sl_reg maxSize = -1);

	// maxLen means the maximum length of utf-8 string
	sl_bool readString(String* str, sl_int32 maxLen = -1);
	
	String readString(const String& def);
	
	String readString(sl_int32 maxLen, const String& def);
	
	String readString(sl_int32 maxLen = -1);

	sl_bool readBigInt(BigInt* v, sl_int32 maxLen = -1);
	
	BigInt readBigInt(const BigInt& def);
	
	BigInt readBigInt(sl_int32 maxLen, const BigInt& def);
	
	BigInt readBigInt(sl_int32 maxLen = -1);
	
	sl_bool readTime(Time* output);
	
	Time readTime();
	
	Time readTime(Time def);

};


class SLIB_EXPORT IWriter
{
public:
	virtual sl_reg write(const void* buf, sl_size size);
	
	virtual sl_int32 write32(const void* buf, sl_uint32 size);

public:
	sl_reg writeFully(const void* buf, sl_size size);

	sl_bool writeInt8(sl_int8 value);
	
	sl_bool writeUint8(sl_uint8 value);
	
	sl_bool writeInt16(sl_int16 value, sl_bool flagBigEndian = sl_false);
	
	sl_bool writeUint16(sl_uint16 value, sl_bool flagBigEndian = sl_false);
	
	sl_bool writeInt32(sl_int32 value, sl_bool flagBigEndian = sl_false);
	
	sl_bool writeUint32(sl_uint32 value, sl_bool flagBigEndian = sl_false);
	
	sl_bool writeInt64(sl_int64 value, sl_bool flagBigEndian = sl_false);
	
	sl_bool writeUint64(sl_uint64 value, sl_bool flagBigEndian = sl_false);
	
	sl_bool writeFloat(float value, sl_bool flagBigEndian = sl_false);

	sl_bool writeDouble(double value, sl_bool flagBigEndian = sl_false);

	//  CVLI (Chain Variable Length Integer)
	sl_bool writeUint32CVLI(sl_uint32 value);
	
	sl_bool writeInt32CVLI(sl_int32 value);

	sl_bool writeUint64CVLI(sl_uint64 value);
	
	sl_bool writeInt64CVLI(sl_int64 value);
	
	sl_bool writeSizeCVLI(sl_size value);
	
	sl_bool writeIntCVLI(sl_reg value);
	
	sl_size writeFromMemory(const Memory& mem);
	
	sl_bool writeSection(const void* mem, sl_size size);
	
	sl_bool writeSection(const Memory& mem);

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
	virtual sl_uint64 getPosition() = 0;
	
	virtual sl_uint64 getSize() = 0;
	
	virtual sl_bool seek(sl_int64 offset, SeekPosition pos) = 0;

public:
	sl_bool seekToBegin();
	
	sl_bool seekToEnd();

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
	SLIB_DECLARE_OBJECT
};

class SLIB_EXPORT MemoryIO : public IO
{
public:
	MemoryIO(sl_size size = 0, sl_bool flagResizable = sl_true);
	
	MemoryIO(const Memory& mem, sl_bool flagResizable = sl_true);
	
	MemoryIO(const void* data, sl_size size, sl_bool flagResizable = sl_true);

	~MemoryIO();

public:
	void init(const void* data, sl_size size, sl_bool flagResizable = sl_true);

	void init(const Memory& mem, sl_bool flagResizable = sl_true);
	
	void init(sl_size size = 0, sl_bool flagResizable = sl_true);
	
	sl_size getOffset();
	
	sl_size getLength();
	
	char* getBuffer();

	// override
	void close();

	// override
	sl_reg read(void* buf, sl_size size);

	// override
	sl_reg write(const void* buf, sl_size size);
	
	// override
	sl_uint64 getPosition();
	
	// override
	sl_uint64 getSize();
	
	// override
	sl_bool seek(sl_int64 offset, SeekPosition pos = SeekPosition::Current);

	// override
	sl_bool setSize(sl_uint64 size);

public:
	SLIB_BOOLEAN_PROPERTY(AutoExpandable)

protected:
	void* m_buf;
	sl_size m_size;
	sl_size m_offset;

};

class SLIB_EXPORT MemoryReader : public Object, public IReader, public ISeekable
{
public:
	MemoryReader(const Memory& mem);

	MemoryReader(const void* buf, sl_size size);

public:
	void init(const Memory& mem);
	
	void init(const void* buf, sl_size size);

	sl_size getOffset();
	
	sl_size getLength();
	
	char* getBuffer();
	
	// override
	sl_reg read(void* buf, sl_size size);
	
	// override
	sl_uint64 getPosition();
	
	// override
	sl_uint64 getSize();
	
	// override
	sl_bool seek(sl_int64 offset, SeekPosition pos);
	
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
	MemoryWriter();
	
	MemoryWriter(const Memory& mem);
	
	MemoryWriter(void* buf, sl_size size);

public:
	void init();
	
	void init(const Memory& mem);
	
	void init(void* buf, sl_size size);
	
	// override
	sl_reg write(const void* buf, sl_size size);
	
	sl_reg write(const Memory& mem);

	// override
	sl_bool seek(sl_int64 offset, SeekPosition pos);

	Memory getData();

	MemoryBuffer& getMemoryBuffer();

	sl_size getOffset();

	sl_size getLength();

	char* getBuffer();

	// override
	sl_uint64 getPosition();
	
	// override
	sl_uint64 getSize();

	
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
	SLIB_PROPERTY(sl_uint32, MaxDatagramSize)

protected:
	MemoryBuffer m_buf;
	sl_uint32 m_lenCurrentDatagram;
	sl_uint32 m_posCurrentDatagram;
	sl_uint8 m_bufSize[4];
	sl_uint32 m_lenBufSize;
};

SLIB_NAMESPACE_END

#endif
