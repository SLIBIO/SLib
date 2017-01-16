#ifndef CHECKHEADER_SLIB_CORE_ASYNC
#define CHECKHEADER_SLIB_CORE_ASYNC

#include "definition.h"

#include "object.h"
#include "thread.h"
#include "file.h"
#include "variant.h"
#include "ptr.h"

SLIB_NAMESPACE_BEGIN

enum class AsyncIoMode
{
	None = 0,
	In = 1,
	Out = 2,
	InOut = 3
};

class AsyncLoop;
class AsyncTimer;
class AsyncIoLoop;
class AsyncIoInstance;
class AsyncIoObject;
class AsyncStreamInstance;
class AsyncStream;
class AsyncStreamRequest;


class SLIB_EXPORT Async
{
public:
	static sl_bool runTask(const Function<void()>& task, const Ref<AsyncLoop>& loop);
	
	static sl_bool runTask(const Function<void()>& task);
	
	
	static sl_bool setTimeout(const Function<void()>& task, sl_uint64 delay_ms, const Ref<AsyncLoop>& loop);
	
	static sl_bool setTimeout(const Function<void()>& task, sl_uint64 delay_ms);
	
	
	static Ref<AsyncTimer> addTimer(const Function<void()>& task, sl_uint64 interval_ms, const Ref<AsyncLoop>& loop);
	
	static Ref<AsyncTimer> addTimer(const Function<void()>& task, sl_uint64 interval_ms);
	
	static void removeTimer(const Ref<AsyncTimer>& timer, const Ref<AsyncLoop>& loop);
	
	static void removeTimer(const Ref<AsyncTimer>& timer);
	
};

class SLIB_EXPORT AsyncLoop : public Dispatcher
{
	SLIB_DECLARE_OBJECT
	
private:
	AsyncLoop();
	
	~AsyncLoop();

public:
	static Ref<AsyncLoop> getDefault();

	static void releaseDefault();

	static Ref<AsyncLoop> create(sl_bool flagAutoStart = sl_true);	

public:
	void release();

	void start();
	
	sl_bool isRunning();
	
	sl_bool addTask(const Function<void()>& task);

	sl_bool setTimeout(const Function<void()>& task, sl_uint64 delay_ms);

	Ref<AsyncTimer> addTimer(const Function<void()>& task, sl_uint64 interval_ms);
	
	sl_bool addTimer(const Ref<AsyncTimer>& timer);
	
	void removeTimer(const Ref<AsyncTimer>& timer);

	sl_uint64 getElapsedMilliseconds();
	
	// override
	sl_bool dispatch(const Function<void()>& callback);

protected:
	sl_bool m_flagInit;
	sl_bool m_flagRunning;
	Ref<Thread> m_thread;

	TimeCounter m_timeCounter;

	LinkedQueue<Function<void()>> m_queueTasks;

	struct TimeTask
	{
		sl_uint64 time;
		Function<void()> task;
	};
	BTree<sl_uint64, TimeTask> m_timeTasks;
	Mutex m_lockTimeTasks;

	class Timer
	{
	public:
		WeakRef<AsyncTimer> timer;
		
	public:
		sl_bool operator==(const Timer& other) const;
	};
	LinkedQueue<Timer> m_queueTimers;
	Mutex m_lockTimer;

protected:
	void _wake();
	sl_int32 _getTimeout();
	sl_int32 _getTimeout_TimeTasks();
	sl_int32 _getTimeout_Timer();
	void _runLoop();

};


class SLIB_EXPORT AsyncTimer : public Object
{
	SLIB_DECLARE_OBJECT
	
protected:
	AsyncTimer();
	
public:
	static Ref<AsyncTimer> create(const Function<void()>& task, sl_uint64 interval_ms, sl_bool flagStart = sl_true);
	
public:
	void start();
	
	void stop();
	
	sl_bool isStarted();
	
	Function<void()> getTask();
	
	sl_uint64 getInterval();
	
	void run();
	
	void stopAndWait();
	
public:
	SLIB_PROPERTY(sl_uint64, LastRunTime)
	
	SLIB_PROPERTY(sl_uint32, MaxConcurrentThread)
	
protected:
	sl_bool m_flagStarted;
	Function<void()> m_task;
	sl_uint64 m_interval;
	sl_int32 m_nCountRun;
	
};


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
	
	// override
	sl_bool dispatch(const Function<void()>& callback);
	
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
	static void* __createHandle();
	static void __closeHandle(void* handle);
	void __runLoop();
	sl_bool __attachInstance(AsyncIoInstance* instance, AsyncIoMode mode);
	void __detachInstance(AsyncIoInstance* instance);
	void __wake();
	
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

	
	Ref<Referable> getUserObject(const String& key);
	
	void setUserObject(const String& key, const Ref<Referable>& object);

	
	Ref<Referable> getUserObject(sl_uint64 key);
	
	void setUserObject(sl_uint64 key, const Ref<Referable>& object);

	
	Variant getUserData(const String& key);
	
	void setUserData(const String& key, const Variant& data);

	
	Variant getUserData(sl_uint64 key);
	
	void setUserData(sl_uint64 key, const Variant& object);

protected:
	void setIoLoop(const Ref<AsyncIoLoop>& loop);
	
	void setIoInstance(AsyncIoInstance* instance);

private:
	AtomicWeakRef<AsyncIoLoop> m_ioLoop;
	AtomicRef<AsyncIoInstance> m_ioInstance;

	HashMap< String, Ref<Referable> > m_mapUserObjects_s;
	HashMap< sl_uint64, Ref<Referable> > m_mapUserObjects_i;
	HashMap<String, Variant> m_mapUserData_s;
	HashMap<sl_uint64, Variant> m_mapUserData_i;
	
};


class SLIB_EXPORT IAsyncStreamListener
{
public:
	// data may be changed during the I/O operations
	virtual void onRead(AsyncStream* stream, void* data, sl_uint32 sizeRead, Referable* ref, sl_bool flagError);
	
	// data may be changed during the I/O operations
	virtual void onWrite(AsyncStream* stream, void* data, sl_uint32 sizeWritten, Referable* ref, sl_bool flagError);
	
};

class SLIB_EXPORT AsyncStreamRequest : public Referable
{
	SLIB_DECLARE_OBJECT
	
public:
	void* data;
	sl_uint32 size;
	Ref<Referable> refData;
	Ptr<IAsyncStreamListener> listener;
	sl_bool flagRead;
	
protected:
	AsyncStreamRequest(void* data, sl_uint32 size, Referable* refData, const Ptr<IAsyncStreamListener>& listener, sl_bool flagRead);

public:
	static Ref<AsyncStreamRequest> createRead(void* data, sl_uint32 size, Referable* refData, const Ptr<IAsyncStreamListener>& listener);
	
	static Ref<AsyncStreamRequest> createWrite(void* data, sl_uint32 size, Referable* refData, const Ptr<IAsyncStreamListener>& listener);

};


class SLIB_EXPORT AsyncStreamInstance : public AsyncIoInstance
{
	SLIB_DECLARE_OBJECT
	
public:
	AsyncStreamInstance();
	
public:
	virtual sl_bool read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref);
	
	virtual sl_bool write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref);

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
	static Ref<AsyncStream> create(AsyncStreamInstance* instance, AsyncIoMode mode, const Ref<AsyncIoLoop>& loop);
	
	static Ref<AsyncStream> create(AsyncStreamInstance* instance, AsyncIoMode mode);

public:
	virtual void close() = 0;
	
	virtual sl_bool isOpened() = 0;
	
	virtual sl_bool read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref = sl_null) = 0;
	
	virtual sl_bool write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref = sl_null) = 0;
	
	virtual sl_bool isSeekable();
	
	virtual sl_bool seek(sl_uint64 pos);
	
	virtual sl_uint64 getSize();
	
	sl_bool readToMemory(const Memory& mem, const Ptr<IAsyncStreamListener>& listener);

	sl_bool writeFromMemory(const Memory& mem, const Ptr<IAsyncStreamListener>& listener);
	
	virtual sl_bool addTask(const Function<void()>& callback) = 0;
	
};

class SLIB_EXPORT AsyncStreamBase : public AsyncStream
{
public:
	// override
	void close();
	
	// override
	sl_bool isOpened();

	// override
	sl_bool read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref = sl_null);
	
	// override
	sl_bool write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref = sl_null);
	
	// override
	sl_bool isSeekable();
	
	// override
	sl_bool seek(sl_uint64 pos);
	
	// override
	sl_uint64 getSize();
	
	// override
	sl_bool addTask(const Function<void()>& callback);
	
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

public:
	// override
	sl_bool read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref = sl_null);
	
	// override
	sl_bool write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref = sl_null);
	
	// override
	sl_bool addTask(const Function<void()>& callback);
	
protected:
	virtual void processRequest(AsyncStreamRequest* request) = 0;

protected:
	void init();
	
	void initWithDispatcher(const Ref<Dispatcher>& dispatcher);
	
protected:
	sl_bool _addRequest(AsyncStreamRequest* request);
	
	void _runProcessor();
	
private:
	LinkedQueue< Ref<AsyncStreamRequest> > m_requests;
	sl_bool m_flagProcessRequest;
	
	Ref<AsyncLoop> m_loop;
	WeakRef<Dispatcher> m_dispatcher;

};

class SLIB_EXPORT AsyncReader : public AsyncStreamSimulator
{
protected:
	AsyncReader();
	
public:
	static Ref<AsyncReader> create(const Ptr<IReader>& reader);
	
	static Ref<AsyncReader> create(const Ptr<IReader>& reader, const Ref<Dispatcher>& dispatcher);
	
public:
	// override
	void close();
	
	// override
	sl_bool isOpened();
	
	// override
	sl_bool write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref = sl_null);

public:
	Ptr<IReader> getReader();

protected:
	// override
	void processRequest(AsyncStreamRequest* request);

private:
	AtomicPtr<IReader> m_reader;
	
};

class SLIB_EXPORT AsyncWriter : public AsyncStreamSimulator
{
protected:
	AsyncWriter();
	
public:
	static Ref<AsyncWriter> create(const Ptr<IWriter>& writer);
	
	static Ref<AsyncWriter> create(const Ptr<IWriter>& writer, const Ref<Dispatcher>& dispatcher);
	

public:
	// override
	void close();
	
	// override
	sl_bool isOpened();
	
	// override
	sl_bool read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref = sl_null);

public:
	Ptr<IWriter> getWriter();

protected:
	// override
	void processRequest(AsyncStreamRequest* request);

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
	// override
	void close();
	
	// override
	sl_bool isOpened();
	
	// override
	sl_bool isSeekable();
	
	// override
	sl_bool seek(sl_uint64 pos);
	
	// override
	sl_uint64 getSize();

public:
	Ref<File> getFile();

protected:
	// override
	void processRequest(AsyncStreamRequest* request);

private:
	AtomicRef<File> m_file;
	
};


class AsyncCopy;

class SLIB_EXPORT IAsyncCopyListener
{
public:
	virtual Memory onAsyncCopyRead(AsyncCopy* task, const Memory& input);
	
	virtual void onAsyncCopyWrite(AsyncCopy* task);
	
	virtual void onAsyncCopyExit(AsyncCopy* task);
	
};

class SLIB_EXPORT AsyncCopy : public Object, public IAsyncStreamListener
{
private:
	AsyncCopy();
	
	~AsyncCopy();
	
public:
	static Ref<AsyncCopy> create(const Ref<AsyncStream>& source, const Ref<AsyncStream>& target
		, sl_uint64 sizeTotal, const Ptr<IAsyncCopyListener>& listener
		, sl_uint32 bufferSize = 0x10000, sl_uint32 bufferCount = 8
		, sl_bool flagStart = sl_true);

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

	const Ptr<IAsyncCopyListener>& getListener();

protected:
	// override
	void onRead(AsyncStream* stream, void* data, sl_uint32 sizeRead, Referable* ref, sl_bool flagError);
	
	// override
	void onWrite(AsyncStream* stream, void* data, sl_uint32 sizeWritten, Referable* ref, sl_bool flagError);

protected:
	void enqueue();

protected:
	AtomicRef<AsyncStream> m_source;
	AtomicRef<AsyncStream> m_target;
	Ptr<IAsyncCopyListener> m_listener;
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
public:
	AsyncOutputBuffer();

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

class SLIB_EXPORT IAsyncOutputListener
{
public:
	virtual void onAsyncOutputError(AsyncOutput* output);
	
	virtual void onAsyncOutputComplete(AsyncOutput* output);
	
};

class SLIB_EXPORT AsyncOutput : public AsyncOutputBuffer, public IAsyncCopyListener, public IAsyncStreamListener
{
protected:
	AsyncOutput();
	
	~AsyncOutput();
	
public:
	static Ref<AsyncOutput> create(const Ref<AsyncStream>& streamOutput, const Ptr<IAsyncOutputListener>& listener, sl_uint32 sizeBuffer = 0x10000);
	
	static Ref<AsyncOutput> create(const Ref<AsyncStream>& streamOutput, sl_uint32 sizeBuffer = 0x10000);

public:
	void mergeBuffer(AsyncOutputBuffer* buffer);
	
	void startWriting();
	
	void close();
	
protected:
	// override
	void onWrite(AsyncStream* stream, void* data, sl_uint32 sizeWritten, Referable* ref, sl_bool flagError);

	// override
	void onAsyncCopyExit(AsyncCopy* task);

protected:
	void _onError();
	void _onComplete();
	void _write(sl_bool flagCompleted);
	
protected:
	Ref<AsyncOutputBufferElement> m_elementWriting;
	Ref<AsyncStream> m_streamOutput;
	Ref<AsyncCopy> m_copy;
	Memory m_bufWrite;
	sl_uint32 m_sizeBuffer;
	sl_bool m_flagWriting;
	sl_bool m_flagClosed;
	
	Ptr<IAsyncOutputListener> m_listener;
};

#define SLIB_ASYNC_STREAM_FILTER_DEFAULT_BUFFER_SIZE 16384

class SLIB_EXPORT AsyncStreamFilter : public AsyncStream, public IAsyncStreamListener
{
protected:
	AsyncStreamFilter();
	
	~AsyncStreamFilter();

public:
	Ref<AsyncStream> getSourceStream() const;
	
	void setSourceStream(const Ref<AsyncStream>& stream);
	
	
	// override
	void close();
	
	// override
	sl_bool isOpened();
	
	// override
	virtual sl_bool read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref = sl_null);

	// override
	virtual sl_bool write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref = sl_null);

	// override
	sl_bool addTask(const Function<void()>& callback);
	
	
	void addReadData(void* data, sl_uint32 size, Referable* refData);
	
	void addReadData(const Memory& data);
	
	void addReadData(void* data, sl_uint32 size);
	
	void setReadingBufferSize(sl_uint32 sizeBuffer);

	
	sl_bool isReadingError();
	
	sl_bool isReadingEnded();

	
	sl_bool isWritingError();
	
	sl_bool isWritingEnded();

protected:
	virtual Memory filterRead(void* data, sl_uint32 size, Referable* refData);
	
	virtual Memory filterWrite(void* data, sl_uint32 size, Referable* refData);

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
	void _read();
	
	void _processRead(void* data, sl_uint32 size, Referable* refData, sl_bool flagError);
	
	void _processReadEmpty();

	void _processZeroWrite(void* data, sl_uint32 size, Ptr<IAsyncStreamListener> listener, const Ref<Referable>& ref);
	
	void _closeAllReadRequests();

protected:
	// override
	virtual void onRead(AsyncStream* stream, void* data, sl_uint32 sizeRead, Referable* ref, sl_bool flagError);

	// override
	virtual void onWrite(AsyncStream* stream, void* data, sl_uint32 sizeWritten, Referable* ref, sl_bool flagError);
};

SLIB_NAMESPACE_END

#endif
