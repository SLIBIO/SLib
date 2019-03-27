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

#ifndef CHECKHEADER_SLIB_CORE_ASYNC
#define CHECKHEADER_SLIB_CORE_ASYNC

#include "definition.h"

#include "dispatch_loop.h"
#include "file.h"
#include "variant.h"
#include "ptr.h"
#include "function.h"

namespace slib
{
	
	enum class AsyncIoMode
	{
		None = 0,
		In = 1,
		Out = 2,
		InOut = 3
	};

	class AsyncIoLoop;
	class AsyncIoInstance;
	class AsyncIoObject;
	class AsyncStreamInstance;
	class AsyncStream;
	class AsyncStreamRequest;
	
	class SLIB_EXPORT AsyncIoLoop : public Dispatcher
	{
		SLIB_DECLARE_OBJECT

	private:
		AsyncIoLoop();

		~AsyncIoLoop();

	public:
		static Ref<AsyncIoLoop> getDefault();
	
		static void releaseDefault();

		static Ref<AsyncIoLoop> create(sl_bool flagAutoStart = sl_true);
	
	public:
		void release();
	
		void start();

		sl_bool isRunning();


		sl_bool addTask(const Function<void()>& task);
	
		void wake();


		sl_bool attachInstance(AsyncIoInstance* instance, AsyncIoMode mode);

		void closeInstance(AsyncIoInstance* instance);

		void requestOrder(AsyncIoInstance* instance);

		sl_bool dispatch(const Function<void()>& callback, sl_uint64 delay_ms) override;

	protected:
		sl_bool m_flagInit;
		sl_bool m_flagRunning;
		void* m_handle;

		Ref<Thread> m_thread;

		LinkedQueue< Function<void()> > m_queueTasks;
	
		LinkedQueue< Ref<AsyncIoInstance> > m_queueInstancesOrder;
		LinkedQueue< Ref<AsyncIoInstance> > m_queueInstancesClosing;
		LinkedQueue< Ref<AsyncIoInstance> > m_queueInstancesClosed;

	protected:
		static void* _native_createHandle();
		static void _native_closeHandle(void* handle);
		void _native_runLoop();
		sl_bool _native_attachInstance(AsyncIoInstance* instance, AsyncIoMode mode);
		void _native_detachInstance(AsyncIoInstance* instance);
		void _native_wake();

	protected:
		void _stepBegin();
		void _stepEnd();
	
	};
	
	
	class AsyncIoObject;
	
	class SLIB_EXPORT AsyncIoInstance : public Object
	{
		SLIB_DECLARE_OBJECT

	public:
		AsyncIoInstance();

		~AsyncIoInstance();
	
	public:
		Ref<AsyncIoObject> getObject();

		void setObject(AsyncIoObject* object);
	
		Ref<AsyncIoLoop> getLoop();

		sl_file getHandle();

		sl_bool isOpened();
	
		AsyncIoMode getMode();

		sl_bool isClosing();

		void setClosing();

		void addToQueue(LinkedQueue< Ref<AsyncIoInstance> >& queue);

		void requestOrder();
	
		void processOrder();

	protected:
		void setMode(AsyncIoMode mode);

		void setHandle(sl_file handle);
	
	public:
		virtual void close() = 0;
	
	protected:
		virtual void onOrder() = 0;

		struct EventDesc
		{
#if defined(SLIB_PLATFORM_IS_WIN32)
			void* pOverlapped; // OVERLAPPED
#endif
#if defined(SLIB_PLATFORM_IS_UNIX)
			sl_bool flagIn;
			sl_bool flagOut;
			sl_bool flagError;
#endif
		};
		virtual void onEvent(EventDesc* pev) = 0;
	
	private:
		AtomicWeakRef<AsyncIoObject> m_object;
		sl_file m_handle;
		AsyncIoMode m_mode;
	
		sl_bool m_flagClosing;

		sl_bool m_flagOrdering;
		Mutex m_lockOrdering;

		friend class AsyncIoLoop;
	};
	
	
	class SLIB_EXPORT AsyncIoObject : public Object
	{
		SLIB_DECLARE_OBJECT

	protected:
		AsyncIoObject();

		~AsyncIoObject();
	
	public:
		Ref<AsyncIoLoop> getIoLoop();

		Ref<AsyncIoInstance> getIoInstance();

		void closeIoInstance();
	

		Variant getUserData();

		void setUserData(const Variant& data);
	
	protected:
		void setIoLoop(const Ref<AsyncIoLoop>& loop);

		void setIoInstance(AsyncIoInstance* instance);
	
	private:
		AtomicWeakRef<AsyncIoLoop> m_ioLoop;
		AtomicRef<AsyncIoInstance> m_ioInstance;
	
		AtomicVariant m_userData;
	
	};
	
	
	struct SLIB_EXPORT AsyncStreamResult
	{
		AsyncStream* stream;
		void* data;
		sl_uint32 size;
		sl_uint32 requestSize;
		Referable* userObject;
		sl_bool flagError;

	};
	
	class SLIB_EXPORT AsyncStreamRequest : public Referable
	{
		SLIB_DECLARE_OBJECT

	public:
		void* data;
		sl_uint32 size;
		Ref<Referable> userObject;
		Function<void(AsyncStreamResult&)> callback;
		sl_bool flagRead;

	protected:
		AsyncStreamRequest(const void* data, sl_uint32 size, Referable* userObject, const Function<void(AsyncStreamResult&)>& callback, sl_bool flagRead);
		
		~AsyncStreamRequest();
	
	public:
		static Ref<AsyncStreamRequest> createRead(void* data, sl_uint32 size, Referable* userObject, const Function<void(AsyncStreamResult&)>& callback);

		static Ref<AsyncStreamRequest> createWrite(const void* data, sl_uint32 size, Referable* userObject, const Function<void(AsyncStreamResult&)>& callback);

	public:
		void runCallback(AsyncStream* stream, sl_uint32 resultSize, sl_bool flagError);

	};
	
	
	class SLIB_EXPORT AsyncStreamInstance : public AsyncIoInstance
	{
		SLIB_DECLARE_OBJECT

	public:
		AsyncStreamInstance();

		~AsyncStreamInstance();

	public:
		virtual sl_bool read(void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& callback, Referable* userObject);

		virtual sl_bool write(const void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& callback, Referable* userObject);

		virtual sl_bool isSeekable();

		virtual sl_bool seek(sl_uint64 pos);

		virtual sl_uint64 getSize();

		sl_size getWaitingSizeForWrite();

	protected:
		sl_bool addReadRequest(const Ref<AsyncStreamRequest>& request);

		sl_bool popReadRequest(Ref<AsyncStreamRequest>& request);

		sl_size getReadRequestsCount();
	
		sl_bool addWriteRequest(const Ref<AsyncStreamRequest>& request);

		sl_bool popWriteRequest(Ref<AsyncStreamRequest>& request);

		sl_size getWriteRequestsCount();
	
	private:
		LinkedQueue< Ref<AsyncStreamRequest> > m_requestsRead;
		LinkedQueue< Ref<AsyncStreamRequest> > m_requestsWrite;
		sl_reg m_sizeWriteWaiting;

	};
	
	class SLIB_EXPORT AsyncStream : public AsyncIoObject
	{
		SLIB_DECLARE_OBJECT

	public:
		AsyncStream();

		~AsyncStream();

	public:
		static Ref<AsyncStream> create(AsyncStreamInstance* instance, AsyncIoMode mode, const Ref<AsyncIoLoop>& loop);

		static Ref<AsyncStream> create(AsyncStreamInstance* instance, AsyncIoMode mode);
	
	public:
		virtual void close() = 0;

		virtual sl_bool isOpened() = 0;

		virtual sl_bool read(void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& callback, Referable* userObject = sl_null) = 0;

		virtual sl_bool write(const void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& callback, Referable* userObject = sl_null) = 0;

		virtual sl_bool isSeekable();

		virtual sl_bool seek(sl_uint64 pos);

		virtual sl_uint64 getSize();

		sl_bool readToMemory(const Memory& mem, const Function<void(AsyncStreamResult&)>& callback);
	
		sl_bool writeFromMemory(const Memory& mem, const Function<void(AsyncStreamResult&)>& callback);

		virtual sl_bool addTask(const Function<void()>& callback) = 0;

	};
	
	class SLIB_EXPORT AsyncStreamBase : public AsyncStream
	{
		SLIB_DECLARE_OBJECT
		
	public:
		AsyncStreamBase();

		~AsyncStreamBase();

	public:
		void close() override;

		sl_bool isOpened() override;
	
		sl_bool read(void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& callback, Referable* userObject = sl_null) override;

		sl_bool write(const void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& callback, Referable* userObject = sl_null) override;

		sl_bool isSeekable() override;

		sl_bool seek(sl_uint64 pos) override;

		sl_uint64 getSize() override;

		sl_bool addTask(const Function<void()>& callback) override;

		sl_size getWaitingSizeForWrite();
	
	protected:
		Ref<AsyncStreamInstance> getIoInstance();

		sl_bool _initialize(AsyncStreamInstance* instance, AsyncIoMode mode, const Ref<AsyncIoLoop>& loop);
	
		friend class AsyncStream;

	};
	
	class SLIB_EXPORT AsyncStreamSimulator : public AsyncStream
	{
		SLIB_DECLARE_OBJECT

	protected:
		AsyncStreamSimulator();

		~AsyncStreamSimulator();
	
	public:
		sl_bool read(void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& callback, Referable* userObject = sl_null) override;

		sl_bool write(const void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& callback, Referable* userObject = sl_null) override;

		sl_bool addTask(const Function<void()>& callback) override;

	protected:
		virtual void processRequest(AsyncStreamRequest* request) = 0;
	
	protected:
		void initialize();

		void initialize(const Ref<Dispatcher>& dispatcher);

	protected:
		sl_bool _addRequest(AsyncStreamRequest* request);

		void _runProcessor();

	private:
		LinkedQueue< Ref<AsyncStreamRequest> > m_requests;
		sl_bool m_flagProcessRequest;

		Ref<DispatchLoop> m_dispatchLoop;
		WeakRef<Dispatcher> m_dispatcher;
	
	};
	
	class SLIB_EXPORT AsyncReader : public AsyncStreamSimulator
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		AsyncReader();

		~AsyncReader();

	public:
		static Ref<AsyncReader> create(const Ptr<IReader>& reader);

		static Ref<AsyncReader> create(const Ptr<IReader>& reader, const Ref<Dispatcher>& dispatcher);

	public:
		void close() override;

		sl_bool isOpened() override;

		sl_bool write(const void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& callback, Referable* userObject = sl_null) override;
	
	public:
		Ptr<IReader> getReader();
	
	protected:
		void processRequest(AsyncStreamRequest* request) override;
	
	private:
		AtomicPtr<IReader> m_reader;

	};
	
	class SLIB_EXPORT AsyncWriter : public AsyncStreamSimulator
	{
		SLIB_DECLARE_OBJECT
		
	protected:
		AsyncWriter();

		~AsyncWriter();

	public:
		static Ref<AsyncWriter> create(const Ptr<IWriter>& writer);

		static Ref<AsyncWriter> create(const Ptr<IWriter>& writer, const Ref<Dispatcher>& dispatcher);

	
	public:
		void close() override;

		sl_bool isOpened() override;

		sl_bool read(void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& callback, Referable* userObject = sl_null) override;
	
	public:
		Ptr<IWriter> getWriter();
	
	protected:
		void processRequest(AsyncStreamRequest* request) override;
	
	protected:
		AtomicPtr<IWriter> m_writer;

	};
	
	class SLIB_EXPORT AsyncFile : public AsyncStreamSimulator
	{
		SLIB_DECLARE_OBJECT

	protected:
		AsyncFile();

		~AsyncFile();

	public:
		static Ref<AsyncFile> create(const Ref<File>& file);

		static Ref<AsyncFile> create(const Ref<File>& file, const Ref<Dispatcher>& dispatcher);


		static Ref<AsyncFile> open(const String& path, FileMode mode);
	
		static Ref<AsyncFile> open(const String& path, FileMode mode, const Ref<Dispatcher>& dispatcher);


		static Ref<AsyncFile> openForRead(const String& path);

		static Ref<AsyncFile> openForRead(const String& path, const Ref<Dispatcher>& dispatcher);


		static Ref<AsyncFile> openForWrite(const String& path);

		static Ref<AsyncFile> openForWrite(const String& path, const Ref<Dispatcher>& dispatcher);


		static Ref<AsyncFile> openForAppend(const String& path);

		static Ref<AsyncFile> openForAppend(const String& path, const Ref<Dispatcher>& dispatcher);


#if defined(SLIB_PLATFORM_IS_WIN32)
		static Ref<AsyncStream> openIOCP(const String& path, FileMode mode, const Ref<AsyncIoLoop>& loop);

		static Ref<AsyncStream> openIOCP(const String& path, FileMode mode);
#endif
	
	public:
		void close() override;

		sl_bool isOpened() override;

		sl_bool isSeekable() override;

		sl_bool seek(sl_uint64 pos) override;

		sl_uint64 getSize() override;
	
	public:
		Ref<File> getFile();
	
	protected:
		void processRequest(AsyncStreamRequest* request) override;
	
	private:
		AtomicRef<File> m_file;

	};
	
	class AsyncCopy;
	
	class SLIB_EXPORT AsyncCopyParam
	{
	public:
		// required
		Ref<AsyncStream> source;
		Ref<AsyncStream> target;
	
		// optional
		sl_uint64 size; // default: Maximum
		sl_uint32 bufferSize; // default: 0x10000
		sl_uint32 bufferCount; // default: 8
		sl_bool flagAutoStart; // default: true

		Function<Memory(AsyncCopy*, const Memory& input)> onRead;
		Function<void(AsyncCopy*)> onWrite;
		Function<void(AsyncCopy*, sl_bool flagError)> onEnd;

	public:
		AsyncCopyParam();
		
		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(AsyncCopyParam)

	};
	
	class SLIB_EXPORT AsyncCopy : public Object
	{
		SLIB_DECLARE_OBJECT

	private:
		AsyncCopy();

		~AsyncCopy();

	public:
		static Ref<AsyncCopy> create(const AsyncCopyParam& param);
	
	public:
		sl_bool start();
	
		void close();
	
		sl_bool isRunning();
	
		Ref<AsyncStream> getSource();
	
		Ref<AsyncStream> getTarget();
	
		sl_uint64 getTotalSize();
	
		sl_uint64 getReadSize();
	
		sl_uint64 getWrittenSize();
	
		sl_bool isCompleted();
	
		sl_bool isErrorOccured();
	
		sl_bool isReadingErrorOccured();
	
		sl_bool isWritingErrorOccured();
	
		sl_bool isReading();
	
		sl_bool isWriting();
	
	private:
		void onReadStream(AsyncStreamResult& result);

		void onWriteStream(AsyncStreamResult& result);
	
		void enqueue();
		
		Memory dispatchRead(const Memory& input);
		
		void dispatchWrite();
		
		void dispatchEnd();
	
	private:
		Ref<AsyncStream> m_source;
		Ref<AsyncStream> m_target;
		Function<Memory(AsyncCopy*, const Memory& input)> m_onRead;
		Function<void(AsyncCopy*)> m_onWrite;
		Function<void(AsyncCopy*, sl_bool flagError)> m_onEnd;
		sl_uint64 m_sizeRead;
		sl_uint64 m_sizeWritten;
		sl_uint64 m_sizeTotal;
		sl_bool m_flagReadError;
		sl_bool m_flagWriteError;
		sl_bool m_flagStarted;
		sl_bool m_flagRunning;
		sl_bool m_flagEnqueue;
	
		class Buffer : public Referable
		{
		public:
			Memory mem;
			Memory memRead;
			Memory memWrite;
		};
		LinkedQueue< Ref<Buffer> > m_buffersRead;
		AtomicRef<Buffer> m_bufferReading;
		LinkedQueue< Ref<Buffer> > m_buffersWrite;
		AtomicRef<Buffer> m_bufferWriting;
	
	};
	
	class SLIB_EXPORT AsyncOutputBufferElement : public Referable
	{
	public:
		AsyncOutputBufferElement();

		AsyncOutputBufferElement(const Memory& header);

		AsyncOutputBufferElement(AsyncStream* stream, sl_uint64 size);
		
		~AsyncOutputBufferElement();

		SLIB_DELETE_CLASS_DEFAULT_MEMBERS(AsyncOutputBufferElement)
	
	public:
		sl_bool isEmpty() const;

		sl_bool isEmptyBody() const;
	

		sl_bool addHeader(const Memory& header);

		void setBody(AsyncStream* stream, sl_uint64 size);
	
		MemoryQueue& getHeader();
	
		Ref<AsyncStream> getBody();
	
		sl_uint64 getBodySize();
	
	protected:
		MemoryQueue m_header;
		sl_uint64 m_sizeBody;
		AtomicRef<AsyncStream> m_body;

	};
	
	class SLIB_EXPORT AsyncOutputBuffer: public Object
	{
		SLIB_DECLARE_OBJECT
		
	public:
		AsyncOutputBuffer();

		~AsyncOutputBuffer();
	
	public:
		void clearOutput();

		sl_bool write(const void* buf, sl_size size);

		sl_bool write(const Memory& mem);

		sl_bool copyFrom(AsyncStream* stream, sl_uint64 size);
	
		sl_bool copyFromFile(const String& path);

		sl_bool copyFromFile(const String& path, const Ref<Dispatcher>& dispatcher);

		sl_uint64 getOutputLength() const;
	
	protected:
		sl_uint64 m_lengthOutput;
		LinkedQueue< Ref<AsyncOutputBufferElement> > m_queueOutput;

		friend class AsyncOutput;
	};
	
	class AsyncOutput;
	
	class AsyncOutputParam
	{
	public:
		Ref<AsyncStream> stream;

		// optional
		sl_uint32 bufferSize;
		sl_uint32 bufferCount;

		Function<void(AsyncOutput*, sl_bool flagError)> onEnd;

	public:
		AsyncOutputParam();

		SLIB_DECLARE_CLASS_DEFAULT_MEMBERS(AsyncOutputParam)

	};
	
	class SLIB_EXPORT AsyncOutput : public AsyncOutputBuffer
	{
		SLIB_DECLARE_OBJECT

	protected:
		AsyncOutput();

		~AsyncOutput();

	public:
		static Ref<AsyncOutput> create(const AsyncOutputParam& param);
	
	public:
		void mergeBuffer(AsyncOutputBuffer* buffer);

		void startWriting();

		void close();

	private:
		void onAsyncCopyEnd(AsyncCopy* task, sl_bool flagError);
		
		void onWriteStream(AsyncStreamResult& result);

	protected:
		void _onError();

		void _onComplete();

		void _write(sl_bool flagCompleted);

	protected:
		Ref<AsyncStream> m_streamOutput;
		sl_uint32 m_bufferSize;
		sl_uint32 m_bufferCount;
	
		Function<void(AsyncOutput*, sl_bool)> m_onEnd;

		Ref<AsyncOutputBufferElement> m_elementWriting;
		Ref<AsyncCopy> m_copy;
		Memory m_bufWrite;
		sl_bool m_flagWriting;
		sl_bool m_flagClosed;

	};
	
	
#define SLIB_ASYNC_STREAM_FILTER_DEFAULT_BUFFER_SIZE 16384
	
	class SLIB_EXPORT AsyncStreamFilter : public AsyncStream
	{
		SLIB_DECLARE_OBJECT

	protected:
		AsyncStreamFilter();

		~AsyncStreamFilter();
	
	public:
		Ref<AsyncStream> getSourceStream() const;

		void setSourceStream(const Ref<AsyncStream>& stream);


		void close() override;

		sl_bool isOpened() override;

		sl_bool read(void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& callback, Referable* userObject = sl_null) override;
	
		sl_bool write(const void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& callback, Referable* userObject = sl_null) override;
	
		sl_bool addTask(const Function<void()>& callback) override;


		void addReadData(void* data, sl_uint32 size, Referable* userObject);

		void addReadData(const Memory& data);

		void addReadData(void* data, sl_uint32 size);

		void setReadingBufferSize(sl_uint32 sizeBuffer);
	

		sl_bool isReadingError();

		sl_bool isReadingEnded();
	

		sl_bool isWritingError();

		sl_bool isWritingEnded();
	
	protected:
		virtual Memory filterRead(void* data, sl_uint32 size, Referable* userObject);

		virtual Memory filterWrite(const void* data, sl_uint32 size, Referable* userObject);
	
	protected:
		void setReadingError();

		void setReadingEnded();
	
		void setWritingError();

		void setWritingEnded();
	
	protected:
		sl_bool m_flagOpened;
	
		AtomicRef<AsyncStream> m_stream;

		MemoryQueue m_bufReadConverted;
		LinkedQueue< Ref<AsyncStreamRequest> > m_requestsRead;
		Mutex m_lockReading;
		sl_bool m_flagReading;
		sl_bool m_flagReadingError;
		sl_bool m_flagReadingEnded;
		AtomicMemory m_memReading;
	
		Mutex m_lockWriting;
		sl_bool m_flagWritingError;
		sl_bool m_flagWritingEnded;
	
	protected:
		sl_bool _read();

		void _closeAllReadRequests();
	
	protected:
		virtual void onReadStream(AsyncStreamResult& result);
	
		virtual void onWriteStream(AsyncStreamResult& result);

	};

}

#endif
