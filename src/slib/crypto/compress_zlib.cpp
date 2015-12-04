#include "../../../inc/slib/crypto/zlib.h"
#include "../../../inc/thirdparty/zlib/zlib.h"

#define STREAM ((z_stream*)(this->m_stream))
#define GZIP_HEADER ((gz_header*)(this->m_gzipHeader))

SLIB_CRYPTO_NAMESPACE_BEGIN
ZlibCompress::ZlibCompress()
{
	m_flagStarted = sl_false;
}

ZlibCompress::~ZlibCompress()
{
	abort();
}

sl_bool ZlibCompress::isStarted()
{
	return m_flagStarted;
}

sl_bool ZlibCompress::start(sl_int32 level)
{
	if (m_flagStarted) {
		abort();
	}
	Base::zeroMemory(STREAM, sizeof(z_stream));
	int iRet = deflateInit2(STREAM, level, Z_DEFLATED, 15, 8, Z_DEFAULT_STRATEGY);
	if (iRet == Z_OK) {
		m_flagStarted = sl_true;
		return sl_true;
	}
	return sl_false;
}

sl_bool ZlibCompress::startRaw(sl_int32 level)
{
	if (m_flagStarted) {
		abort();
	}
	Base::zeroMemory(STREAM, sizeof(z_stream));
	int iRet = deflateInit2(STREAM, level, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY);
	if (iRet == Z_OK) {
		m_flagStarted = sl_true;
		return sl_true;
	}
	return sl_false;
}

sl_bool ZlibCompress::startGzip(const GzipParam& param, sl_int32 level)
{
	if (m_flagStarted) {
		abort();
	}
	Base::zeroMemory(STREAM, sizeof(z_stream));
	int iRet = deflateInit2(STREAM, level, Z_DEFLATED, 31, 8, Z_DEFAULT_STRATEGY);
	if (iRet == Z_OK) {
		Base::zeroMemory(GZIP_HEADER, sizeof(gz_header));
		m_gzipFileName = param.fileName;
		if (m_gzipFileName.isNotEmpty()) {
			GZIP_HEADER->name = (Bytef*)(m_gzipFileName.getBuf());
		}
		m_gzipComment = param.comment;
		if (m_gzipComment.isNotEmpty()) {
			GZIP_HEADER->comment = (Bytef*)(m_gzipComment.getBuf());
		}
		GZIP_HEADER->os = 255;
		iRet = deflateSetHeader(STREAM, GZIP_HEADER);
		if (iRet == Z_OK) {
			m_flagStarted = sl_true;
			return sl_true;
		} else {
			deflateEnd(STREAM);
		}
	}
	return sl_false;
}

sl_int32 ZlibCompress::compress(
	const void* input, sl_uint32 sizeInputAvailable, sl_uint32& sizeInputPassed
	, void* output, sl_uint32 sizeOutputAvailable, sl_uint32& sizeOutputUsed
	, sl_bool flagFinish)
{
	if (!m_flagStarted) {
		return Z_STREAM_ERROR;
	}
	z_stream* stream = STREAM;
	sizeInputPassed = 0;
	sizeOutputUsed = 0;
	stream->next_in = (Bytef*)input;
	stream->avail_in = sizeInputAvailable;
	stream->next_out = (Bytef*)output;
	stream->avail_out = sizeOutputAvailable;
	int iRet = deflate(stream, flagFinish ? Z_FINISH : Z_NO_FLUSH);
	if (iRet < 0) {
		abort();
		return iRet;
	}
	sizeInputPassed = sizeInputAvailable - stream->avail_in;
	sizeOutputUsed = sizeOutputAvailable - stream->avail_out;
	if (iRet == Z_STREAM_END) {
		abort();
		return 0;
	}
	return 1;
}

Memory ZlibCompress::compress(const void* _data, sl_size size, sl_bool flagFinish)
{
	Memory ret;
	sl_uint8* data = (sl_uint8*)_data;
	sl_uint32 sizeChunk;
	if (size > 16384) {
		sizeChunk = 262144;
	} else {
		sizeChunk = 4096;
	}
	Memory memChunk = Memory::create(sizeChunk);
	if (memChunk.isEmpty()) {
		return ret;
	}
	sl_uint8* chunk = (sl_uint8*)(memChunk.getBuf());

	MemoryBuffer buffer;
	while (1) {
		sl_uint32 sizeInput = (sl_uint32)(SLIB_MIN(size, sizeChunk));
		sl_uint32 sizeInputPassed = 0, sizeOutputUsed = 0;
		sl_int32 iRet = compress(data, sizeInput, sizeInputPassed, chunk, sizeChunk, sizeOutputUsed, flagFinish && sizeInput < sizeChunk);
		if (iRet < 0) {
			return ret;
		}
		if (sizeOutputUsed > 0) {
			buffer.add(chunk, sizeOutputUsed);
		}
		data += sizeInputPassed;
		size -= sizeInputPassed;
		if (iRet == 0) {
			break;
		}
		if (size == 0 && sizeOutputUsed == 0) {
			break;
		}
	}
	ret = buffer.merge();
	return ret;
}

void ZlibCompress::abort()
{
	if (m_flagStarted) {
		deflateEnd(STREAM);
		m_flagStarted = sl_false;
	}
}

ZlibDecompress::ZlibDecompress()
{
	m_flagStarted = sl_false;
}

ZlibDecompress::~ZlibDecompress()
{
	abort();
}

sl_bool ZlibDecompress::isStarted()
{
	return m_flagStarted;
}

sl_bool ZlibDecompress::start()
{
	if (m_flagStarted) {
		abort();
	}
	Base::zeroMemory(STREAM, sizeof(z_stream));
	int iRet = inflateInit2(STREAM, 47);
	if (iRet == Z_OK) {
		m_flagStarted = sl_true;
		return sl_true;
	}
	return sl_false;
}

sl_bool ZlibDecompress::startRaw()
{
	if (m_flagStarted) {
		abort();
	}
	Base::zeroMemory(STREAM, sizeof(z_stream));
	int iRet = inflateInit2(STREAM, -15);
	if (iRet == Z_OK) {
		m_flagStarted = sl_true;
		return sl_true;
	}
	return sl_false;
}

sl_int32 ZlibDecompress::decompress(
	const void* input, sl_uint32 sizeInputAvailable, sl_uint32& sizeInputPassed
	, void* output, sl_uint32 sizeOutputAvailable, sl_uint32& sizeOutputUsed)
{
	if (!m_flagStarted) {
		return Z_STREAM_ERROR;
	}
	z_stream* stream = STREAM;
	sizeInputPassed = 0;
	sizeOutputUsed = 0;
	stream->next_in = (Bytef*)input;
	stream->avail_in = sizeInputAvailable;
	stream->next_out = (Bytef*)output;
	stream->avail_out = sizeOutputAvailable;
	int iRet = inflate(stream, Z_NO_FLUSH);
	if (iRet == Z_NEED_DICT) {
		iRet = Z_DATA_ERROR;
	}
	if (iRet < 0) {
		abort();
		return iRet;
	}
	sizeInputPassed = sizeInputAvailable - stream->avail_in;
	sizeOutputUsed = sizeOutputAvailable - stream->avail_out;
	if (iRet == Z_STREAM_END) {
		abort();
		return 0;
	}
	return 1;
}

Memory ZlibDecompress::decompress(const void* _data, sl_size size)
{
	Memory ret;
	sl_uint8* data = (sl_uint8*)_data;
	sl_uint32 sizeChunk;
	if (size > 16384) {
		sizeChunk = 262144;
	} else {
		sizeChunk = 4096;
	}
	Memory memChunk = Memory::create(sizeChunk);
	if (memChunk.isEmpty()) {
		return ret;
	}
	sl_uint8* chunk = (sl_uint8*)(memChunk.getBuf());

	MemoryBuffer buffer;
	while (1) {
		sl_uint32 sizeInput = (sl_uint32)(SLIB_MIN(size, sizeChunk));
		sl_uint32 sizeInputPassed = 0, sizeOutputUsed = 0;
		sl_int32 iRet = decompress(data, sizeInput, sizeInputPassed, chunk, sizeChunk, sizeOutputUsed);
		if (iRet < 0) {
			return ret;
		}
		if (sizeOutputUsed > 0) {
			buffer.add(chunk, sizeOutputUsed);
		}
		data += sizeInputPassed;
		size -= sizeInputPassed;
		if (iRet == 0) {
			break;
		}
		if (size == 0 && sizeOutputUsed == 0) {
			break;
		}
	}
	ret = buffer.merge();
	return ret;
}

void ZlibDecompress::abort()
{
	if (m_flagStarted) {
		inflateEnd(STREAM);
		m_flagStarted = sl_false;
	}
}
sl_uint32 Zlib::adler32(sl_uint32 adler, const void* _data, sl_size size)
{
	const char* data = (const char*)_data;
	while (size > 0) {
		sl_uint32 n = 0x10000000;
		if (size < n) {
			n = (sl_uint32)size;
		}
		adler = (sl_uint32)(::adler32(adler, (Bytef*)data, n));
		size -= n;
		data += n;
	}
	return adler;
}

sl_uint32 Zlib::crc32(sl_uint32 crc, const void* _data, sl_size size)
{
	const char* data = (const char*)_data;
	while (size > 0) {
		sl_uint32 n = 3;
		if (size < n) {
			n = (sl_uint32)size;
		}
		crc = (sl_uint32)(::crc32(crc, (Bytef*)data, n));
		size -= n;
		data += n;
	}
	return crc;
}

Memory Zlib::compress(const void* data, sl_size size, sl_int32 level)
{
	ZlibCompress zlib;
	if (zlib.start(level)) {
		return zlib.compress(data, size, sl_true);
	}
	return Memory::null();
}

Memory Zlib::compressRaw(const void* data, sl_size size, sl_int32 level)
{
	ZlibCompress zlib;
	if (zlib.startRaw(level)) {
		return zlib.compress(data, size, sl_true);
	}
	return Memory::null();
}

Memory Zlib::compressGzip(const GzipParam& param, const void* data, sl_size size, sl_int32 level)
{
	ZlibCompress zlib;
	if (zlib.startGzip(param, level)) {
		return zlib.compress(data, size, sl_true);
	}
	return Memory::null();
}

Memory Zlib::decompress(const void* data, sl_size size)
{
	ZlibDecompress zlib;
	if (zlib.start()) {
		return zlib.decompress(data, size);
	}
	return Memory::null();
}

Memory Zlib::decompressRaw(const void* data, sl_size size)
{
	ZlibDecompress zlib;
	if (zlib.startRaw()) {
		return zlib.decompress(data, size);
	}
	return Memory::null();
}
SLIB_CRYPTO_NAMESPACE_END

