#ifndef CHECKHEADER_SLIB_CRYPTO_ZLIB
#define CHECKHEADER_SLIB_CRYPTO_ZLIB

#include "definition.h"

#include "../core/object.h"
#include "../core/memory.h"
#include "../core/string.h"

SLIB_CRYPTO_NAMESPACE_BEGIN

class SLIB_EXPORT GzipParam
{
public:
	String fileName;
	String comment;
};

class SLIB_EXPORT ZlibCompress : public Object
{
public:
	ZlibCompress();
	
	~ZlibCompress();

public:
	sl_bool isStarted();

	/*
		contains zlib wrapper
		level = 0 ~ 9
	*/
	sl_bool start(sl_int32 level = 6);

	/*
		raw deflate content
		level = 0 ~ 9
	*/
	sl_bool startRaw(sl_int32 level = 6);

	/*
		contains gzip header
		level = 0 ~ 9
	*/
	
	sl_bool startGzip(const GzipParam& param, sl_int32 level = 6);
	
	sl_bool startGzip(sl_int32 level = 6);

	/*
		returns
			<0: Error
			=0: Finished
			>0: Success
	*/
	sl_int32 compress(
		const void* input, sl_uint32 sizeInputAvailable, sl_uint32& sizeInputPassed,
		void* output, sl_uint32 sizeOutputAvailable, sl_uint32& sizeOutputUsed,
		sl_bool flagFinish);

	Memory compress(const void* data, sl_size size, sl_bool flagFinish);

	void abort();

private:
	sl_uint8 m_stream[128]; // bigger than sizeof(z_stream)
	
	sl_uint8 m_gzipHeader[128];
	String m_gzipFileName;
	String m_gzipComment;

	sl_bool m_flagStarted;
	
};

class SLIB_EXPORT ZlibDecompress : public Object
{
public:
	ZlibDecompress();
	
	~ZlibDecompress();
	
public:
	sl_bool isStarted();

	// zlib and gzip wrapper
	sl_bool start();

	// raw inflate
	sl_bool startRaw();

	/*
		returns
			<0: Error
			=0: Finished
			>0: Success
	*/
	sl_int32 decompress(
		const void* input, sl_uint32 sizeInputAvailable, sl_uint32& sizeInputPassed,
		void* output, sl_uint32 sizeOutputAvailable, sl_uint32& sizeOutputUsed);
	
	Memory decompress(const void* data, sl_size size);

	void abort();

private:
	sl_uint8 m_stream[128]; // bigger than sizeof(z_stream)
	sl_bool m_flagStarted;
	
};

class SLIB_EXPORT Zlib
{
public:
	static sl_uint32 adler32(sl_uint32 adler, const void* data, sl_size size);
	
	static sl_uint32 adler32(const void* data, sl_size size);
	
	static sl_uint32 adler32(sl_uint32 adler, const Memory& mem);
	
	static sl_uint32 adler32(const Memory& mem);

	/*
		zlib crc32 : CRC32B
	*/
	static sl_uint32 crc32(sl_uint32 crc, const void* data, sl_size size);
	
	static sl_uint32 crc32(const void* data, sl_size size);
	
	static sl_uint32 crc32(sl_uint32 crc, const Memory& mem);
	
	static sl_uint32 crc32(const Memory& mem);
	
	/*
		Compress
	*/
	static Memory compress(const void* data, sl_size size, sl_int32 level = 6);
	
	static Memory compressRaw(const void* data, sl_size size, sl_int32 level = 6);
	
	static Memory compressGzip(const GzipParam& param, const void* data, sl_size size, sl_int32 level = 6);
	
	static Memory compressGzip(const void* data, sl_size size, sl_int32 level = 6);

	/*
		Decompress
	*/

	// decompress gzip and zlib wrappers
	static Memory decompress(const void* data, sl_size size);

	static Memory decompressRaw(const void* data, sl_size size);

};

SLIB_CRYPTO_NAMESPACE_END

#endif
