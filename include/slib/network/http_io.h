/*
 *  Copyright (c) 2017 SLIBIO. All Rights Reserved.
 *
 *  This file is part of the SLib.io project.
 *
 *  This Source Code Form is subject to the terms of the Mozilla Public
 *  License, v. 2.0. If a copy of the MPL was not distributed with this
 *  file, You can obtain one at http://mozilla.org/MPL/2.0/.
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
	
	class SLIB_EXPORT IHttpContentReaderListener
	{
	public:
		IHttpContentReaderListener();

		virtual ~IHttpContentReaderListener();

	public:
		virtual void onCompleteReadHttpContent(void* dataRemained, sl_uint32 sizeRemained, sl_bool flagError) = 0;
		
	};
	
	class SLIB_EXPORT HttpContentReader : public AsyncStreamFilter
	{
	protected:
		HttpContentReader();
		
		~HttpContentReader();
		
	public:
		static Ref<HttpContentReader> createPersistent(const Ref<AsyncStream>& io,
													   const Ptr<IHttpContentReaderListener>& listener,
													   sl_uint64 contentLength,
													   sl_uint32 bufferSize,
													   sl_bool flagDecompress);
		
		static Ref<HttpContentReader> createChunked(const Ref<AsyncStream>& io,
													const Ptr<IHttpContentReaderListener>& listener,
													sl_uint32 bufferSize,
													sl_bool flagDecompress);
		
		static Ref<HttpContentReader> createTearDown(const Ref<AsyncStream>& io,
													 const Ptr<IHttpContentReaderListener>& listener,
													 sl_uint32 bufferSize,
													 sl_bool flagDecompress);
	public:
		sl_bool isDecompressing();
		
	protected:
		sl_bool write(void* data, sl_uint32 size, const Function<void(AsyncStreamResult*)>& callback, Referable* ref) override;
		
		void onReadStream(AsyncStreamResult* result) override;
		
	protected:
		void setError();
		
		void setCompleted(void* dataRemain, sl_uint32 size);
		
		sl_bool setDecompressing();
		
		Memory decompressData(void* data, sl_uint32 size, Referable* refData);
		
	protected:
		sl_bool m_flagDecompressing;
		ZlibDecompress m_zlib;
		Ptr<IHttpContentReaderListener> m_listener;
		
	};

}

#endif

