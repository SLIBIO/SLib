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

#include "slib/network/http_io.h"

namespace slib
{

/***********************************************************************
						HttpOutputBuffer
***********************************************************************/
	HttpOutputBuffer::HttpOutputBuffer()
	{
	}

	HttpOutputBuffer::~HttpOutputBuffer()
	{
	}

	void HttpOutputBuffer::clearOutput()
	{
		m_bufferOutput.clearOutput();
	}

	void HttpOutputBuffer::write(const void* buf, sl_size size)
	{
		m_bufferOutput.write(buf, size);
	}

	void HttpOutputBuffer::write(const String& str)
	{
		write(str.getData(), str.getLength());
	}

	void HttpOutputBuffer::write(const Memory& mem)
	{
		m_bufferOutput.write(mem);
	}

	void HttpOutputBuffer::copyFrom(AsyncStream* stream, sl_uint64 size)
	{
		m_bufferOutput.copyFrom(stream, size);
	}

	void HttpOutputBuffer::copyFromFile(const String& path)
	{
		m_bufferOutput.copyFromFile(path);
	}

	void HttpOutputBuffer::copyFromFile(const String& path, const Ref<Dispatcher>& dispatcher)
	{
		m_bufferOutput.copyFromFile(path, dispatcher);
	}

	sl_uint64 HttpOutputBuffer::getOutputLength() const
	{
		return m_bufferOutput.getOutputLength();
	}

/***********************************************************************
						HttpHeaderReader
***********************************************************************/
	HttpHeaderReader::HttpHeaderReader()
	{
		m_last[0] = 0;
		m_last[1] = 0;
		m_last[2] = 0;
	}

	HttpHeaderReader::~HttpHeaderReader()
	{
	}

	sl_bool HttpHeaderReader::add(const void* _buf, sl_size size, sl_size& posBody)
	{
		if (size == 0) {
			return sl_false;
		}
		sl_bool flagFound = sl_false;
		const sl_uint8* buf = (const sl_uint8*)(_buf);
		if (size > 0 && m_last[0] == '\r' && m_last[1] == '\n' && m_last[2] == '\r' && buf[0] == '\n') {
			posBody = 1;
			flagFound = sl_true;
		}
		if (size > 1 && m_last[1] == '\r' && m_last[2] == '\n' && buf[0] == '\r' && buf[1] == '\n') {
			posBody = 2;
			flagFound = sl_true;
		}
		if (size > 2 && m_last[2] == '\r' && buf[0] == '\n' && buf[1] == '\r' && buf[2] == '\n') {
			posBody = 3;
			flagFound = sl_true;
		}
		if (size > 3) {
			for (sl_size i = 0; i <= size - 4; i++) {
				if (buf[i] == '\r' && buf[i + 1] == '\n' && buf[i + 2] == '\r' && buf[i + 3] == '\n') {
					posBody = 4 + i;
					flagFound = sl_true;
					break;
				}
			}
		}
		if (flagFound) {
			m_buffer.add(Memory::create(buf, posBody));
			m_last[0] = 0;
			m_last[1] = 0;
			m_last[2] = 0;
		} else {
			m_buffer.add(Memory::create(buf, size));
			if (size < 3) {
				if (size == 1) {
					m_last[0] = m_last[1];
					m_last[1] = m_last[2];
					m_last[2] = buf[0];
				} else {
					m_last[0] = m_last[2];
					m_last[1] = buf[0];
					m_last[2] = buf[1];
				}
			} else {
				m_last[0] = buf[size - 3];
				m_last[1] = buf[size - 2];
				m_last[2] = buf[size - 1];
			}
		}
		return flagFound;
	}

	sl_size HttpHeaderReader::getHeaderSize()
	{
		return m_buffer.getSize();
	}

	Memory HttpHeaderReader::mergeHeader()
	{
		return m_buffer.merge();
	}

	void HttpHeaderReader::clear()
	{
		m_last[0] = 0;
		m_last[1] = 0;
		m_last[2] = 0;
		m_buffer.clear();
	}

/***********************************************************************
						HttpContentReader
***********************************************************************/

	IHttpContentReaderListener::IHttpContentReaderListener()
	{
	}

	IHttpContentReaderListener::~IHttpContentReaderListener()
	{
	}

	HttpContentReader::HttpContentReader()
	{
		m_flagDecompressing = sl_false;
	}

	HttpContentReader::~HttpContentReader()
	{
	}

	class _priv_HttpContentReader_Persistent : public HttpContentReader
	{
	public:
		sl_uint64 m_sizeTotal;
		sl_uint64 m_sizeRead;

	public:
		_priv_HttpContentReader_Persistent()
		{
			m_sizeRead = 0;
			m_sizeTotal = 0;
		}

		Memory filterRead(void* data, sl_uint32 size, Referable* refData)
		{
			sl_uint64 sizeRemain = m_sizeTotal - m_sizeRead;
			if (size < sizeRemain) {
				m_sizeRead += size;
				return decompressData(data, size, refData);
			} else {
				m_sizeRead = m_sizeTotal;
				sl_uint32 sizeRead = (sl_uint32)sizeRemain;
				Memory ret = decompressData(data, sizeRead, refData);
				setCompleted((char*)data + sizeRead, size - sizeRead);
				return ret;
			}
		}
	};

	Ref<HttpContentReader> HttpContentReader::createPersistent(const Ref<AsyncStream>& io,
															   const Ptr<IHttpContentReaderListener>& listener,
															   sl_uint64 contentLength,
															   sl_uint32 bufferSize,
															   sl_bool flagDecompress)
	{
		Ref<_priv_HttpContentReader_Persistent> ret = new _priv_HttpContentReader_Persistent;
		if (io.isNull()) {
			return ret;
		}
		if (contentLength == 0 || bufferSize == 0) {
			return ret;
		}
		if (ret.isNotNull()) {
			ret->m_sizeTotal = contentLength;
			ret->m_listener = listener;
			ret->setReadingBufferSize(bufferSize);
			ret->setSourceStream(io);
			if (flagDecompress) {
				if (!(ret->setDecompressing())) {
					ret.setNull();
				}
			}
		}
		return ret;
	}

/*

							Chunked Transfer Coding

	chunked-body   = *chunk
                      last-chunk
                      trailer-part
                      CRLF

     chunk          = chunk-size [ chunk-ext ] CRLF
                      chunk-data CRLF
     chunk-size     = 1*HEXDIG
     last-chunk     = 1*("0") [ chunk-ext ] CRLF

     chunk-data     = 1*OCTET ; a sequence of chunk-size octets

*/

	class _priv_HttpContentReader_Chunked : public HttpContentReader
	{
	public:
		sl_uint32 m_state;
		sl_uint64 m_sizeCurrentChunk;
		sl_uint64 m_sizeCurrentChunkRead;
		sl_uint32 m_sizeTrailerField;

	public:
		_priv_HttpContentReader_Chunked()
		{
			m_state = 0;
			m_sizeCurrentChunk = 0;
			m_sizeCurrentChunkRead = 0;
			m_sizeTrailerField = 0;
		}

		Memory filterRead(void* _data, sl_uint32 size, Referable* refData)
		{
			sl_uint8* data = (sl_uint8*)_data;
			sl_uint32 pos = 0;

			sl_uint8* output = data;
			sl_uint32 sizeOutput = 0;
			
			sl_uint32 v;

			while (pos < size) {
				sl_uint8 ch = data[pos];
				switch (m_state) {
				case 0: // chunk-size
					v = SLIB_CHAR_HEX_TO_INT(ch);
					if (v < 16) {
						m_sizeCurrentChunk = (m_sizeCurrentChunk << 4) | v;
						pos++;
					} else {
						m_state = 1;
					}
					break;
				case 1: // chunk-ext
					if (ch == '\r') {
						m_state = 2;
					}
					pos++;
					break;
				case 2: // CRLF
					if (ch == '\n') {
						if (m_sizeCurrentChunk > 0) {
							m_state = 3;
						} else {
							// last chunk
							m_state = 10;
							m_sizeTrailerField = 0;
						}
					} else {
						m_state = -1;
						setError();
						return sl_null;
					}
					pos++;
					break;
				case 3: // chunk-data
					if (m_sizeCurrentChunkRead < m_sizeCurrentChunk) {
						output[sizeOutput] = ch;
						m_sizeCurrentChunkRead++;
						sizeOutput++;
					} else {
						if (ch == '\r') {
							m_state = 4;
						} else {
							m_state = -1;
							setError();
							return sl_null;
						}
					}
					pos++;
					break;
				case 4: // CRLF
					if (ch == '\n') {
						m_sizeCurrentChunk = 0;
						m_sizeCurrentChunkRead = 0;
						m_state = 0;
					} else {
						m_state = -1;
						setError();
						return sl_null;
					}
					pos++;
					break;
				case 10: // trailer-part
					if (ch == '\r') {
						m_state = 11;
					} else {
						m_sizeTrailerField++;
					}
					pos++;
					break;
				case 11: // CRLF
					if (ch == '\n') {
						if (m_sizeTrailerField > 0) {
							m_state = 10;
							m_sizeTrailerField = 0;
						} else {
							pos++;
							m_state = -1;
							Memory mem = decompressData(output, sizeOutput, refData);
							setCompleted(data + pos, size - pos);
							return mem;
						}
					} else {
						m_state = -1;
						setError();
						return sl_null;
					}
					pos++;
					break;
				default:
					return sl_null;
				}
			}
			return decompressData(output, sizeOutput, refData);
		}
	};

	Ref<HttpContentReader> HttpContentReader::createChunked(const Ref<AsyncStream>& io,
															const Ptr<IHttpContentReaderListener>& listener,
															sl_uint32 bufferSize,
															sl_bool flagDecompress)
	{
		Ref<_priv_HttpContentReader_Chunked> ret = new _priv_HttpContentReader_Chunked;
		if (io.isNull()) {
			return ret;
		}
		if (bufferSize == 0) {
			return ret;
		}
		if (ret.isNotNull()) {
			ret->m_listener = listener;
			ret->setReadingBufferSize(bufferSize);
			ret->setSourceStream(io);
			if (flagDecompress) {
				if (!(ret->setDecompressing())) {
					ret.setNull();
				}
			}
		}
		return ret;
	}

	class _priv_HttpContentReader_TearDown : public HttpContentReader
	{
	public:
		Memory filterRead(void* data, sl_uint32 size, Referable* refData)
		{
			return decompressData(data, size, refData);
		}
	};

	Ref<HttpContentReader> HttpContentReader::createTearDown(const Ref<AsyncStream>& io,
															 const Ptr<IHttpContentReaderListener>& listener,
															 sl_uint32 bufferSize,
															 sl_bool flagDecompress)
	{
		Ref<_priv_HttpContentReader_TearDown> ret = new _priv_HttpContentReader_TearDown;
		if (io.isNull()) {
			return ret;
		}
		if (bufferSize == 0) {
			return ret;
		}
		if (ret.isNotNull()) {
			ret->m_listener = listener;
			ret->setReadingBufferSize(bufferSize);
			ret->setSourceStream(io);
			if (flagDecompress) {
				if (!(ret->setDecompressing())) {
					ret.setNull();
				}
			}
		}
		return ret;
	}

	sl_bool HttpContentReader::isDecompressing()
	{
		return m_flagDecompressing;
	}

	void HttpContentReader::onReadStream(AsyncStreamResult* result)
	{
		if (result->flagError) {
			setReadingEnded();
		}
		AsyncStreamFilter::onReadStream(result);
		if (result->flagError) {
			setError();
		}
	}

	void HttpContentReader::setCompleted(void* dataRemain, sl_uint32 size)
	{
		setReadingEnded();
		PtrLocker<IHttpContentReaderListener> listener(m_listener);
		if (listener.isNotNull()) {
			listener->onCompleteReadHttpContent(dataRemain, size, isReadingError());
		}
		setReadingError();
	}

	void HttpContentReader::setError()
	{
		setReadingEnded();
		PtrLocker<IHttpContentReaderListener> listener(m_listener);
		if (listener.isNotNull()) {
			listener->onCompleteReadHttpContent(sl_null, 0, sl_true);
		}
		setReadingError();
	}

	sl_bool HttpContentReader::write(const void* data, sl_uint32 size, const Function<void(AsyncStreamResult*)>& callback, Referable* ref)
	{
		return sl_false;
	}

	sl_bool HttpContentReader::setDecompressing()
	{
		if (m_zlib.start()) {
			m_flagDecompressing = sl_true;
			return sl_true;
		} else {
			m_flagDecompressing = sl_false;
			return sl_false;
		}
	}

	Memory HttpContentReader::decompressData(void* data, sl_uint32 size, Referable* refData)
	{
		if (m_flagDecompressing) {
			return m_zlib.decompress(data, size);
		} else {
			return Memory::createStatic(data, size, refData);
		}
	}

}