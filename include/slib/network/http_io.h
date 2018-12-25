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

#ifndef CHECKHEADER_SLIB_NETWORK_HTTP_IO
#define CHECKHEADER_SLIB_NETWORK_HTTP_IO

#include "definition.h"

#include "../core/string.h"
#include "../crypto/zlib.h"

#include "async.h"

namespace slib
{
	
	class SLIB_EXPORT HttpOutputBuffer
	{
	public:
		HttpOutputBuffer();
		
		~HttpOutputBuffer();
		
	public:
		void clearOutput();
		
		void write(const void* buf, sl_size size);
		
		void write(const String& str);
		
		void write(const Memory& mem);
		
		void copyFrom(AsyncStream* stream, sl_uint64 size);
		
		void copyFromFile(const String& path);
		
		void copyFromFile(const String& path, const Ref<Dispatcher>& dispatcher);
		
		sl_uint64 getOutputLength() const;
		
	protected:
		AsyncOutputBuffer m_bufferOutput;
		
	};
	
	class SLIB_EXPORT HttpHeaderReader
	{
	public:
		HttpHeaderReader();
		
		~HttpHeaderReader();
		
	public:
		// return sl_true when body section (\r\n\r\n) is detected
		sl_bool add(const void* buf, sl_size size, sl_size& posBody);
		
		Memory mergeHeader();
		
		sl_size getHeaderSize();
		
		void clear();
		
	protected:
		sl_char16 m_last[3];
		MemoryQueue m_buffer;
		
	};
	
	typedef Function<void(void* dataRemained, sl_uint32 sizeRemained, sl_bool flagError)> HttpContentReaderOnComplete;
	
	class SLIB_EXPORT HttpContentReader : public AsyncStreamFilter
	{
	protected:
		HttpContentReader();
		
		~HttpContentReader();
		
	public:
		static Ref<HttpContentReader> createPersistent(const Ref<AsyncStream>& io,
													   const HttpContentReaderOnComplete& onComplete,
													   sl_uint64 contentLength,
													   sl_uint32 bufferSize,
													   sl_bool flagDecompress);
		
		static Ref<HttpContentReader> createChunked(const Ref<AsyncStream>& io,
													const HttpContentReaderOnComplete& onComplete,
													sl_uint32 bufferSize,
													sl_bool flagDecompress);
		
		static Ref<HttpContentReader> createTearDown(const Ref<AsyncStream>& io,
													 const HttpContentReaderOnComplete& onComplete,
													 sl_uint32 bufferSize,
													 sl_bool flagDecompress);
	public:
		sl_bool isDecompressing();
		
	protected:
		sl_bool write(const void* data, sl_uint32 size, const Function<void(AsyncStreamResult*)>& callback, Referable* ref) override;
		
		void onReadStream(AsyncStreamResult* result) override;
		
	protected:
		void setError();
		
		void setCompleted(void* dataRemain, sl_uint32 size);
		
		sl_bool setDecompressing();
		
		Memory decompressData(void* data, sl_uint32 size, Referable* refData);
		
	protected:
		sl_bool m_flagDecompressing;
		ZlibDecompress m_zlib;
		HttpContentReaderOnComplete m_onComplete;

	};

}

#endif

