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

#ifndef CHECKHEADER_SLIB_CORE_IO
#define CHECKHEADER_SLIB_CORE_IO

#include "definition.h"

#include "object.h"
#include "memory.h"
#include "endian.h"
#include "time.h"

#include "../math/bigint.h"

namespace slib
{
	
	enum class SeekPosition
	{
		Current = 1,
		Begin = 2,
		End = 3
	};
	
	class SLIB_EXPORT IReader
	{
	public:
		IReader();

		virtual ~IReader();

	public:
		virtual sl_reg read(void* buf, sl_size size);

		virtual sl_int32 read32(void* buf, sl_uint32 size);
	
	public:
		sl_reg readFully(void* buf, sl_size size);
	
		sl_bool readInt8(sl_int8* output);

		sl_int8 readInt8(sl_int8 def = 0);

		sl_bool readUint8(sl_uint8* output);

		sl_uint8 readUint8(sl_uint8 def = 0);

		sl_bool readInt16(sl_int16* output, EndianType endian = Endian::Little);

		sl_int16 readInt16(sl_int16 def = 0, EndianType endian = Endian::Little);
	
		sl_bool readUint16(sl_uint16* output, EndianType endian = Endian::Little);

		sl_uint16 readUint16(sl_uint16 def = 0, EndianType endian = Endian::Little);
	
		sl_bool readInt32(sl_int32* output, EndianType endian = Endian::Little);

		sl_int32 readInt32(sl_int32 def = 0, EndianType endian = Endian::Little);

		sl_bool readUint32(sl_uint32* output, EndianType endian = Endian::Little);

		sl_uint32 readUint32(sl_uint32 def = 0, EndianType endian = Endian::Little);

		sl_bool readInt64(sl_int64* output, EndianType endian = Endian::Little);

		sl_int64 readInt64(sl_int64 def = 0, EndianType endian = Endian::Little);
	
		sl_bool readUint64(sl_uint64* output, EndianType endian = Endian::Little);
	
		sl_uint64 readUint64(sl_uint64 def = 0, EndianType endian = Endian::Little);

		sl_bool readFloat(float* output, EndianType endian = Endian::Little);

		float readFloat(float def = 0, EndianType endian = Endian::Little);
	
		sl_bool readDouble(double* output, EndianType endian = Endian::Little);

		double readDouble(double def = 0, EndianType endian = Endian::Little);

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
	
		sl_bool readSectionData(void* data, sl_size& size);

		sl_bool readSection(Memory* output, sl_size maxSize = SLIB_SIZE_MAX);

		Memory readSection(const Memory& def, sl_size maxSize = SLIB_SIZE_MAX);
	
		Memory readSection(sl_size maxLen = SLIB_SIZE_MAX);

		// maxLen means the maximum length of utf-8 string
		sl_bool readStringSection(String* output, sl_size maxLen = SLIB_SIZE_MAX);

		// maxLen means the maximum length of utf-8 string
		String readStringSection(const String& def, sl_size maxLen = SLIB_SIZE_MAX);

		String readStringSection(sl_size maxLen = SLIB_SIZE_MAX);
	
		sl_bool readBigInt(BigInt* v, sl_size maxLen = SLIB_SIZE_MAX);

		BigInt readBigInt(const BigInt& def, sl_size maxLen = SLIB_SIZE_MAX);

		BigInt readBigInt(sl_size maxLen = SLIB_SIZE_MAX);

		sl_bool readTime(Time* output);

		Time readTime();

		Time readTime(Time def);
	
		String readTextUTF8(sl_size size);

		String16 readTextUTF16(sl_size size, EndianType endian = Endian::Little);

		String readText(sl_size size, Charset* outCharset = sl_null);

		String16 readText16(sl_size size, Charset* outCharset = sl_null);
		
	};
	
	
	class SLIB_EXPORT IWriter
	{
	public:
		IWriter();

		virtual ~IWriter();

	public:
		virtual sl_reg write(const void* buf, sl_size size);

		virtual sl_int32 write32(const void* buf, sl_uint32 size);
	
	public:
		sl_reg writeFully(const void* buf, sl_size size);
	
		sl_bool writeInt8(sl_int8 value);

		sl_bool writeUint8(sl_uint8 value);

		sl_bool writeInt16(sl_int16 value, EndianType endian = Endian::Little);

		sl_bool writeUint16(sl_uint16 value, EndianType endian = Endian::Little);

		sl_bool writeInt32(sl_int32 value, EndianType endian = Endian::Little);

		sl_bool writeUint32(sl_uint32 value, EndianType endian = Endian::Little);

		sl_bool writeInt64(sl_int64 value, EndianType endian = Endian::Little);

		sl_bool writeUint64(sl_uint64 value, EndianType endian = Endian::Little);

		sl_bool writeFloat(float value, EndianType endian = Endian::Little);
	
		sl_bool writeDouble(double value, EndianType endian = Endian::Little);
	
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
		sl_bool writeStringSection(const String& str, sl_size maxLen = SLIB_SIZE_MAX);
	
		sl_bool writeBigInt(const BigInt& v, sl_size maxLen = SLIB_SIZE_MAX);
	
		sl_bool writeTime(const Time& t);
	
		sl_bool writeTextUTF8(const String& text, sl_bool flagWriteByteOrderMark = sl_false);

		sl_bool writeTextUTF16LE(const String16& text, sl_bool flagWriteByteOrderMark = sl_false);

		sl_bool writeTextUTF16BE(const String16& text, sl_bool flagWriteByteOrderMark = sl_false);

	};
	
	class SLIB_EXPORT IStream : public IReader, public IWriter
	{
	public:
		IStream();

		~IStream();

	};
	
	class SLIB_EXPORT ISeekable
	{
	public:
		ISeekable();

		virtual ~ISeekable();

	public:
		virtual sl_uint64 getPosition() = 0;

		virtual sl_uint64 getSize() = 0;

		virtual sl_bool seek(sl_int64 offset, SeekPosition pos) = 0;

	public:
		sl_bool isEOF();

		sl_bool seekToBegin();

		sl_bool seekToEnd();
	
	};
	
	class SLIB_EXPORT IResizable
	{
	public:
		IResizable();

		virtual ~IResizable();

	public:
		virtual sl_bool setSize(sl_uint64 size) = 0;
	};
	
	class SLIB_EXPORT IClosable
	{
	public:
		IClosable();

		virtual ~IClosable();

	public:
		virtual void close() = 0;
	};
	
	class SLIB_EXPORT IO : public Object, public IStream, public ISeekable, public IResizable, public IClosable
	{
		SLIB_DECLARE_OBJECT

	public:
		IO();

		~IO();

	public:
		String readLine();
	
		Memory readAllBytes(sl_size maxSize = SLIB_SIZE_MAX);
		
		String readAllTextUTF8(sl_size maxSize = SLIB_SIZE_MAX);
		
		String16 readAllTextUTF16(EndianType endian = Endian::Little, sl_size maxSize = SLIB_SIZE_MAX);
		
		String readAllText(Charset* outCharset = sl_null, sl_size maxSize = SLIB_SIZE_MAX);
		
		String16 readAllText16(Charset* outCharset = sl_null, sl_size maxSize = SLIB_SIZE_MAX);
		
	};
	
	class SLIB_EXPORT MemoryIO : public IO
	{
	public:
		MemoryIO(sl_size size = 0, sl_bool flagResizable = sl_true);
		
		MemoryIO(const void* data, sl_size size, sl_bool flagResizable = sl_true);

		MemoryIO(const Memory& mem);
	
		~MemoryIO();
	
	public:
		void initialize(const void* data, sl_size size, sl_bool flagResizable = sl_true);
	
		void initialize(sl_size size = 0, sl_bool flagResizable = sl_true);
		
		void initialize(const Memory& mem);

		sl_size getOffset();

		sl_size getLength();

		char* getBuffer();
	
		void close() override;
	
		sl_reg read(void* buf, sl_size size) override;
	
		sl_reg write(const void* buf, sl_size size) override;

		sl_uint64 getPosition() override;

		sl_uint64 getSize() override;

		sl_bool seek(sl_int64 offset, SeekPosition pos = SeekPosition::Current) override;
	
		sl_bool setSize(sl_uint64 size) override;
	
		sl_bool isResizable();
		
	protected:
		void _initialize(const void* data, sl_size size, sl_bool flagResizable);

		void _initialize(sl_size size, sl_bool flagResizable);
		
		void _initialize(const Memory& mem);
		
		void _free();
		
	protected:
		void* m_buf;
		sl_size m_size;
		sl_size m_offset;
		sl_bool m_flagResizable;
	
	};
	
	class SLIB_EXPORT MemoryReader : public Object, public IReader, public ISeekable
	{
	public:
		MemoryReader(const Memory& mem);
	
		MemoryReader(const void* buf, sl_size size);

		~MemoryReader();
	
	public:
		void initialize(const Memory& mem);

		void initialize(const void* buf, sl_size size);
	
		sl_size getOffset();

		sl_size getLength();

		char* getBuffer();

		sl_reg read(void* buf, sl_size size) override;

		sl_uint64 getPosition() override;

		sl_uint64 getSize() override;

		sl_bool seek(sl_int64 offset, SeekPosition pos) override;
		
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

		~MemoryWriter();
	
	public:
		void initialize();

		void initialize(const Memory& mem);

		void initialize(void* buf, sl_size size);

		sl_reg write(const void* buf, sl_size size) override;

		sl_reg write(const Memory& mem);
	
		sl_bool seek(sl_int64 offset, SeekPosition pos) override;
	
		Memory getData();
	
		MemoryBuffer& getMemoryBuffer();
	
		sl_size getOffset();
	
		sl_size getLength();
	
		char* getBuffer();
	
		sl_uint64 getPosition() override;

		sl_uint64 getSize() override;
		
	protected:
		void* m_buf;
		sl_size m_size;
		sl_size m_offset;
		Memory m_mem;
		MemoryBuffer m_buffer;
	
	};
	
}

#endif
