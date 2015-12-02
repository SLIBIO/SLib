#ifndef CHECKHEADER_SLIB_CORE_ASYNC
#define CHECKHEADER_SLIB_CORE_ASYNC

#include "definition.h"
#include "object.h"
#include "thread_pool.h"
#include "file.h"
#include "variant.h"

SLIB_NAMESPACE_BEGIN
class AsyncInstance;
class AsyncTimer;
class AsyncLoop;

class SLIB_EXPORT Async
{
public:
	static sl_bool runTask(const Ref<Runnable>& task, const Ref<AsyncLoop>& loop, sl_bool flagRunByThreadPool = sl_true);
	static sl_bool runTask(const Ref<Runnable>& task, sl_bool flagRunByThreadPool = sl_true);
	
	static sl_bool setTimeout(const Ref<Runnable>& task, sl_uint64 delay_ms, const Ref<AsyncLoop>& loop, sl_bool flagRunByThreadPool = sl_true);
	static sl_bool setTimeout(const Ref<Runnable>& task, sl_uint64 delay_ms, sl_bool flagRunByThreadPool = sl_true);
	
	static Ref<AsyncTimer> addTimer(const Ref<Runnable>& task, sl_uint64 interval_ms, const Ref<AsyncLoop>& loop, sl_bool flagRunByThreadPool = sl_true);
	static Ref<AsyncTimer> addTimer(const Ref<Runnable>& task, sl_uint64 interval_ms, sl_bool flagRunByThreadPool = sl_true);
	static void removeTimer(const Ref<AsyncTimer>& timer, const Ref<AsyncLoop>& loop);
	static void removeTimer(const Ref<AsyncTimer>& timer);
};

class SLIB_EXPORT AsyncLoop : public Object
{
	SLIB_DECLARE_OBJECT(AsyncLoop, Object)
private:
	AsyncLoop();
public:
	~AsyncLoop();

public:
	static Ref<AsyncLoop> getDefault();
	static Ref<AsyncLoop> create();
	static Ref<AsyncLoop> createNonIO();

public:
	void release();
	sl_bool isRunning();

	void wake();

	sl_bool attachInstance(AsyncInstance* instance);
	void closeInstance(AsyncInstance* instance);
	void requestOrder(AsyncInstance* instance);
	
	sl_uint32 getMinimumThreadsCount();
	void setMinimumThreadCount(sl_uint32 n);
	sl_uint32 getMaximumThreadsCount();
	void setMaximumThreadsCount(sl_uint32 n);
	sl_uint32 getThreadStackSize();
	void setThreadStackSize(sl_uint32 n);
	Ref<ThreadPool> getThreadPool();
	
	sl_bool addTask(const Ref<Runnable>& task, sl_bool flagRunByThreadPool = sl_true);

	sl_bool setTimeout(const Ref<Runnable>& task, sl_uint64 delay_ms, sl_bool flagRunByThreadPool = sl_true);

	Ref<AsyncTimer> addTimer(const Ref<Runnable>& task, sl_uint64 interval_ms, sl_bool flagRunByThreadPool = sl_true);
	sl_bool addTimer(const Ref<AsyncTimer>& timer, sl_bool flagRunByThreadPool = sl_true);
	void removeTimer(const Ref<AsyncTimer>& timer);

	SLIB_INLINE sl_uint64 getEllapsedMilliseconds()
	{
		return m_timeCounter.getEllapsedMilliseconds();
	}

protected:
	sl_bool m_flagRunning;
	sl_bool m_flagNonIO;
	void* m_handle;
	Ref<Thread> m_thread;
	Ref<ThreadPool> m_threadPool;
	Queue< Ref<AsyncInstance>, sl_false > m_queueInstancesOrder;
	Queue< Ref<AsyncInstance> > m_queueInstancesClosing;
	Queue< Ref<AsyncInstance> > m_queueInstancesClosed;

	TimeCounter m_timeCounter;

	Queue< Ref<Runnable> > m_queueTasks;

	struct TimeTask
	{
		sl_uint64 time;
		Ref<Runnable> task;
		sl_bool flagRunByThreadPool;
	};
	BTree<sl_uint64, TimeTask> m_timeTasks;
	Mutex m_lockTimeTasks;

	class Timer
	{
	public:
		WeakRef<AsyncTimer> timer;
		sl_bool flagRunByThreadPool;
		
	public:
		sl_bool operator==(const Timer& other) const
		{
			return timer == other.timer;
		}
	};
	Queue<Timer> m_queueTimers;
	Mutex m_lockTimer;

protected:
	static void* __createHandle();
	static void __closeHandle(void* handle);
	void __runLoop();
	sl_bool __attachInstance(AsyncInstance* instance);
	void __detachInstance(AsyncInstance* instance);
	void __wake();

protected:
	sl_int32 _getTimeout();
	sl_int32 _getTimeout_TimeTasks();
	sl_int32 _getTimeout_Timer();
	void _stepBegin();
	void _stepEnd();
	void _runLoopNonIO();
};

class SLIB_EXPORT AsyncTimer : public Object
{
protected:
	AsyncTimer();
public:
	~AsyncTimer();

public:
	static Ref<AsyncTimer> create(const Ref<Runnable>& task, sl_uint64 interval_ms, sl_bool flagStart = sl_true);

	SLIB_INLINE void start()
	{
		m_flagStarted = sl_true;
	}

	SLIB_INLINE void stop()
	{
		m_flagStarted = sl_false;
	}

	SLIB_INLINE sl_bool isStarted()
	{
		return m_flagStarted;
	}

	SLIB_INLINE Ref<Runnable> getTask()
	{
		return m_task;
	}

	SLIB_INLINE sl_uint64 getInterval()
	{
		return m_interval;
	}

	void run();

	void stopAndWait();

public:
	SLIB_PROPERTY_INLINE(sl_uint64, LastRunTime)
	SLIB_PROPERTY_INLINE(sl_uint32, MaxConcurrentThread)

protected:
	sl_bool m_flagStarted;
	Ref<Runnable> m_task;
	sl_uint64 m_interval;
	sl_int32 m_nCountRun;
};

class AsyncObject;
class SLIB_EXPORT AsyncInstance : public Object
{
	SLIB_DECLARE_OBJECT(AsyncInstance, Object)
protected:
	AsyncInstance();
public:
	~AsyncInstance();

public:
	SLIB_INLINE sl_file getHandle()
	{
		return m_handle;
	}
	SLIB_INLINE void setHandle(sl_file handle)
	{
		m_handle = handle;
	}
	
	SLIB_INLINE sl_bool isClosing()
	{
		return m_flagClosing;
	}
	SLIB_INLINE void setClosing()
	{
		m_flagClosing = sl_true;
	}

public:
	SLIB_INLINE sl_bool isOpened()
	{
		return m_handle != SLIB_FILE_INVALID_HANDLE;
	}

	virtual void close() = 0;

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

	Ref<AsyncObject> getObject();
	void setObject(AsyncObject* object);
	
	Ref<AsyncLoop> getLoop();
	
	void requestOrder();

private:
	sl_bool m_flagOrdering;
	Mutex m_lockOrdering;
	
	sl_bool m_flagClosing;
	sl_file m_handle;
	WeakRef<AsyncObject> m_object;
	
	friend class AsyncLoop;
};

class SLIB_EXPORT AsyncObject : public Object
{
protected:
	AsyncObject();
public:
	~AsyncObject();

public:
	Ref<AsyncLoop> getLoop();
	const Ref<AsyncInstance>& getInstance();
	void closeInstance();

	SLIB_INLINE Ptr<Referable> getUserObject(const String& key)
	{
		return m_mapUserObjects_s.getValue(key, Ptr<Referable>::null());
	}
	SLIB_INLINE void setUserObject(const String& key, const Ptr<Referable>& object)
	{
		m_mapUserObjects_s.put(key, object);
	}

	SLIB_INLINE Ptr<Referable> getUserObject(sl_uint64 key)
	{
		return m_mapUserObjects_i.getValue(key, Ptr<Referable>::null());
	}
	SLIB_INLINE void setUserObject(sl_uint64 key, const Ptr<Referable>& object)
	{
		m_mapUserObjects_i.put(key, object);
	}

	SLIB_INLINE Variant getUserData(const String& key)
	{
		return m_mapUserData_s.getValue(key, Variant::null());
	}
	SLIB_INLINE void setUserData(const String& key, const Variant& data)
	{
		m_mapUserData_s.put(key, data);
	}

	SLIB_INLINE Variant getUserData(sl_uint64 key)
	{
		return m_mapUserData_i.getValue(key, Variant::null());
	}
	SLIB_INLINE void setUserData(sl_uint64 key, const Variant& object)
	{
		m_mapUserData_i.put(key, object);
	}

protected:
	void setLoop(const Ref<AsyncLoop>& loop);
	void setInstance(AsyncInstance* instance);

private:
	WeakRef<AsyncLoop> m_loop;
	Ref<AsyncInstance> m_instance;

	Map< String, Ptr<Referable> > m_mapUserObjects_s;
	Map< sl_uint64, Ptr<Referable> > m_mapUserObjects_i;
	Map<String, Variant> m_mapUserData_s;
	Map<sl_uint64, Variant> m_mapUserData_i;
};

class AsyncStream;
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
	SLIB_DECLARE_ROOT_OBJECT(AsyncStreamRequest)
protected:
	AsyncStreamRequest();
	AsyncStreamRequest(void* data, sl_uint32 size, const Ref<Referable>& refData, const Ptr<IAsyncStreamListener>& listener, sl_bool flagRead);

public:
	void* data;
	sl_uint32 size;
	Ref<Referable> refData;
	Ptr<IAsyncStreamListener> listener;
	sl_bool flagRead;

public:
	static Ref<AsyncStreamRequest> createRead(void* data, sl_uint32 size, const Ref<Referable>& refData, const Ptr<IAsyncStreamListener>& listener);
	static Ref<AsyncStreamRequest> createWrite(void* data, sl_uint32 size, const Ref<Referable>& refData, const Ptr<IAsyncStreamListener>& listener);
};

class SLIB_EXPORT AsyncStreamInstance : public AsyncInstance
{
protected:
	AsyncStreamInstance();
public:
	~AsyncStreamInstance();

public:
	virtual sl_bool read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref);
	virtual sl_bool write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref);

	virtual sl_bool isSeekable();
	virtual sl_bool seek(sl_uint64 pos);
	virtual sl_uint64 getSize();

protected:
	Queue< Ref<AsyncStreamRequest> > m_requestsRead;
	Queue< Ref<AsyncStreamRequest> > m_requestsWrite;
};

class SLIB_EXPORT AsyncStream : public AsyncObject
{
	SLIB_DECLARE_OBJECT(AsyncStream, Object)
protected:
	AsyncStream();
public:
	~AsyncStream();

public:
	virtual void close() = 0;
	virtual sl_bool isOpened() = 0;

	virtual sl_bool read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref = sl_null) = 0;
	SLIB_INLINE sl_bool read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Ref<Referable> ref)
	{
		return read(data, size, listener, ref.get());
	}

	virtual sl_bool write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref = sl_null) = 0;
	SLIB_INLINE sl_bool write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Ref<Referable> ref)
	{
		return write(data, size, listener, ref.get());
	}

	virtual sl_bool isSeekable();
	virtual sl_bool seek(sl_uint64 pos);
	virtual sl_uint64 getSize();

	SLIB_INLINE sl_bool readToMemory(const Memory& _mem, const Ptr<IAsyncStreamListener>& listener)
	{
		Memory mem = _mem;
		sl_size size = mem.getSize();
		if (size > 0x40000000) {
			size = 0x40000000;
		}
		return read(mem.getBuf(), (sl_uint32)(size), listener, mem.getReference());
	}

	SLIB_INLINE sl_bool writeFromMemory(const Memory& _mem, const Ptr<IAsyncStreamListener>& listener)
	{
		Memory mem = _mem;
		sl_size size = mem.getSize();
		if (size > 0x40000000) {
			size = 0x40000000;
		}
		return write(mem.getBuf(), (sl_uint32)(size), listener, mem.getReference());
	}

public:
	static Ref<AsyncStream> create(AsyncStreamInstance* instance, const Ref<AsyncLoop>& loop);
	static Ref<AsyncStream> create(AsyncStreamInstance* instance);
};

class SLIB_EXPORT AsyncStreamBase : public AsyncStream
{
protected:
	AsyncStreamBase();
public:
	~AsyncStreamBase();

public:
	virtual void close();
	virtual sl_bool isOpened();

	virtual sl_bool read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref = sl_null);
	SLIB_INLINE sl_bool read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Ref<Referable> ref)
	{
		return read(data, size, listener, ref.get());
	}
	
	virtual sl_bool write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref = sl_null);
	SLIB_INLINE sl_bool write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Ref<Referable> ref)
	{
		return write(data, size, listener, ref.get());
	}

	virtual sl_bool isSeekable();
	virtual sl_bool seek(sl_uint64 pos);
	virtual sl_uint64 getSize();

protected:
	SLIB_INLINE const Ref<AsyncStreamInstance>& getInstance()
	{
		return Ref<AsyncStreamInstance>::from(AsyncObject::getInstance());
	}

protected:
	sl_bool initialize(AsyncStreamInstance* instance, const Ref<AsyncLoop>& loop);

	friend class AsyncStream;
};

class SLIB_EXPORT AsyncStreamBaseIO : public AsyncStream
{
protected:
	AsyncStreamBaseIO();
public:
	~AsyncStreamBaseIO();

public:
	virtual sl_bool read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref = sl_null);
	SLIB_INLINE sl_bool read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Ref<Referable> ref)
	{
		return read(data, size, listener, ref.get());
	}
	
	virtual sl_bool write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref = sl_null);
	SLIB_INLINE sl_bool write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Ref<Referable> ref)
	{
		return write(data, size, listener, ref.get());
	}

protected:
	virtual void processRequest(AsyncStreamRequest* request);

private:
	Queue< Ref<AsyncStreamRequest> > m_requests;
	sl_bool m_flagProcessRequest;

private:
	sl_bool _addRequest(AsyncStreamRequest* request);
	void _runProcessor();
};

class SLIB_EXPORT AsyncReader : public AsyncStreamBaseIO
{
private:
	AsyncReader();
public:
	~AsyncReader();

public:
	virtual void close();
	virtual sl_bool isOpened();
	virtual sl_bool write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref = sl_null);

public:
	const Ptr<IReader>& getReader();

public:
	static Ref<AsyncReader> create(const Ptr<IReader>& reader, const Ref<AsyncLoop>& loop);
	static Ref<AsyncReader> create(const Ptr<IReader>& reader);

protected:
	virtual void processRequest(AsyncStreamRequest* request);

private:
	Ptr<IReader> m_reader;
};

class SLIB_EXPORT AsyncWriter : public AsyncStreamBaseIO
{
private:
	AsyncWriter();
public:
	~AsyncWriter();

public:
	virtual void close();
	virtual sl_bool isOpened();
	virtual sl_bool read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref = sl_null);

public:
	const Ptr<IWriter>& getWriter();

public:
	static Ref<AsyncWriter> create(const Ptr<IWriter>& writer, const Ref<AsyncLoop>& loop);
	static Ref<AsyncWriter> create(const Ptr<IWriter>& writer);

protected:
	virtual void processRequest(AsyncStreamRequest* request);

private:
	Ptr<IWriter> m_writer;
};

class SLIB_EXPORT AsyncFile : public AsyncStreamBaseIO
{
private:
	AsyncFile();
public:
	~AsyncFile();

public:
	Ref<File> getFile();

	virtual void close();
	virtual sl_bool isOpened();
	
	virtual sl_bool isSeekable();
	virtual sl_bool seek(sl_uint64 pos);
	virtual sl_uint64 getSize();

public:
	static Ref<AsyncFile> create(const Ref<File>& file, const Ref<AsyncLoop>& loop);
	static Ref<AsyncFile> create(const Ref<File>& file);

	static Ref<AsyncFile> open(const String& path, File::Mode mode, const Ref<AsyncLoop>& loop);
	static Ref<AsyncFile> open(const String& path, File::Mode mode);

	static Ref<AsyncFile> openForRead(const String& path, const Ref<AsyncLoop>& loop);
	static Ref<AsyncFile> openForRead(const String& path);

	static Ref<AsyncFile> openForWrite(const String& path, const Ref<AsyncLoop>& loop);
	static Ref<AsyncFile> openForWrite(const String& path);

	static Ref<AsyncFile> openForAppend(const String& path, const Ref<AsyncLoop>& loop);
	static Ref<AsyncFile> openForAppend(const String& path);

#if defined(SLIB_PLATFORM_IS_WIN32)
	static Ref<AsyncStream> openIOCP(const String& path, File::Mode mode, const Ref<AsyncLoop>& loop);
	static Ref<AsyncStream> openIOCP(const String& path, File::Mode mode);
#endif

protected:
	virtual void processRequest(AsyncStreamRequest* request);

private:
	Ref<File> m_file;
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
public:
	~AsyncCopy();
public:
	static Ref<AsyncCopy> create(const Ref<AsyncStream>& source, const Ref<AsyncStream>& target
		, sl_uint64 sizeTotal, const Ptr<IAsyncCopyListener>& listener
		, sl_uint32 bufferSize = 0x10000, sl_uint32 bufferCount = 8
		, sl_bool flagStart = sl_true);

	sl_bool start();

	void close();

	SLIB_INLINE sl_bool isRunning()
	{
		return m_flagRunning;
	}

	SLIB_INLINE Ref<AsyncStream> getSource()
	{
		return m_source;
	}

	SLIB_INLINE Ref<AsyncStream> getTarget()
	{
		return m_target;
	}

	SLIB_INLINE sl_uint64 getTotalSize()
	{
		return m_sizeTotal;
	}

	SLIB_INLINE sl_uint64 getReadSize()
	{
		return m_sizeRead;
	}

	SLIB_INLINE sl_uint64 getWrittenSize()
	{
		return m_sizeWritten;
	}

	SLIB_INLINE sl_bool isCompleted()
	{
		return m_sizeWritten == m_sizeTotal;
	}

	SLIB_INLINE sl_bool isErrorOccured()
	{
		return m_flagReadError || m_flagWriteError;
	}

	SLIB_INLINE sl_bool isReadingErrorOccured()
	{
		return m_flagReadError;
	}

	SLIB_INLINE sl_bool isWritingErrorOccured()
	{
		return m_flagWriteError;
	}

	SLIB_INLINE sl_bool isReading()
	{
		return m_bufferReading.isNotNull();
	}

	SLIB_INLINE sl_bool isWriting()
	{
		return m_bufferWriting.isNotNull();
	}

	SLIB_INLINE const Ptr<IAsyncCopyListener>& getListener()
	{
		return m_listener;
	}

protected:
	void onRead(AsyncStream* stream, void* data, sl_uint32 sizeRead, Referable* ref, sl_bool flagError);
	void onWrite(AsyncStream* stream, void* data, sl_uint32 sizeWritten, Referable* ref, sl_bool flagError);

protected:
	void enqueue();

protected:
	Ref<AsyncStream> m_source;
	Ref<AsyncStream> m_target;
	Ptr<IAsyncCopyListener> m_listener;
	sl_uint64 m_sizeRead;
	sl_uint64 m_sizeWritten;
	sl_uint64 m_sizeTotal;
	sl_bool m_flagReadError;
	sl_bool m_flagWriteError;
	sl_bool m_flagStarted;
	sl_bool m_flagRunning;

	class Buffer : public Referable
	{
	public:
		Memory mem;
		Memory memRead;
		Memory memWrite;
	};
	Queue< Ref<Buffer> > m_buffersRead;
	Ref<Buffer> m_bufferReading;
	Queue< Ref<Buffer> > m_buffersWrite;
	Ref<Buffer> m_bufferWriting;
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

	SLIB_INLINE MemoryQueue& getHeader()
	{
		return m_header;
	}

	SLIB_INLINE const Ref<AsyncStream>& getBody()
	{
		return m_body;
	}

	SLIB_INLINE sl_uint64 getBodySize()
	{
		return m_sizeBody;
	}

protected:
	MemoryQueue m_header;
	sl_uint64 m_sizeBody;
	Ref<AsyncStream> m_body;
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

	SLIB_INLINE sl_uint64 getOutputLength() const
	{
		return m_lengthOutput;
	}

protected:
	sl_uint64 m_lengthOutput;
	Queue< Ref<AsyncOutputBufferElement> > m_queueOutput;
	
	friend class AsyncOutput;
};

class AsyncOutput;
class SLIB_EXPORT IAsyncOutputListener
{
public:
	void onAsyncOutputError(AsyncOutput* output);
	void onAsyncOutputComplete(AsyncOutput* output);
};

class SLIB_EXPORT AsyncOutput : public AsyncOutputBuffer, public IAsyncCopyListener, public IAsyncStreamListener
{
protected:
	AsyncOutput();
public:
	~AsyncOutput();

public:
	void mergeBuffer(AsyncOutputBuffer* buffer);
	void startWriting();
	void close();

public:
	static Ref<AsyncOutput> create(const Ref<AsyncStream>& streamOutput, const Ptr<IAsyncOutputListener>& listener, sl_uint32 sizeBuffer = 0x10000);
	SLIB_INLINE static Ref<AsyncOutput> create(const Ref<AsyncStream>& streamOutput, sl_uint32 sizeBuffer = 0x10000)
	{
		return create(streamOutput, Ptr<IAsyncOutputListener>::null(), sizeBuffer);
	}

public:
	SLIB_PROPERTY_INLINE(Ptr<IAsyncOutputListener>, Listener)

protected:
	Ref<AsyncOutputBufferElement> m_elementWriting;
	Ref<AsyncStream> m_streamOutput;
	Ref<AsyncCopy> m_copy;
	Memory m_bufWrite;
	sl_uint32 m_sizeBuffer;
	sl_bool m_flagWriting;
	sl_bool m_flagClosed;

protected:
	void onAsyncCopyExit(AsyncCopy* task);
	void onWrite(AsyncStream* stream, void* data, sl_uint32 sizeWritten, Referable* ref, sl_bool flagError);
	void _onError();
	void _onComplete();
	void _write(sl_bool flagCompleted);
};

#define SLIB_ASYNC_STREAM_FILTER_DEFAULT_BUFFER_SIZE 16384

class SLIB_EXPORT AsyncStreamFilter : public AsyncStream, public IAsyncStreamListener
{
protected:
	AsyncStreamFilter();
public:
	~AsyncStreamFilter();

public:
	const Ref<AsyncStream>& getReadingStream() const;
	void setReadingStream(const Ref<AsyncStream>& stream); 
	
	const Ref<AsyncStream>& getWritingStream() const;
	void setWritingStream(const Ref<AsyncStream>& stream);

	virtual void close();
	virtual sl_bool isOpened();

	virtual sl_bool read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref = sl_null);
	SLIB_INLINE sl_bool read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Ref<Referable> ref)
	{
		return read(data, size, listener, ref.get());
	}

	virtual sl_bool write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Referable* ref = sl_null);
	SLIB_INLINE sl_bool write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, Ref<Referable> ref)
	{
		return write(data, size, listener, ref.get());
	}

	void addReadData(void* data, sl_uint32 size, const Ref<Referable>& refData);
	void addReadData(const Memory& data);
	void addReadData(void* data, sl_uint32 size);
	void setReadingBufferSize(sl_uint32 sizeBuffer);

	SLIB_INLINE sl_bool isReadingError()
	{
		return m_flagReadingError;
	}
	SLIB_INLINE sl_bool isReadingEnded()
	{
		return m_flagReadingEnded;
	}

	SLIB_INLINE sl_bool isWritingError()
	{
		return m_flagWritingError;
	}
	SLIB_INLINE sl_bool isWritingEnded()
	{
		return m_flagWritingEnded;
	}

protected:
	virtual Memory filterRead(void* data, sl_uint32 size, Referable* refData);
	virtual Memory filterWrite(void* data, sl_uint32 size, Referable* refData);

	void setReadingError();
	void setReadingEnded();

	void setWritingError();
	void setWritingEnded();

protected:
	sl_bool m_flagOpened;

	Ref<AsyncStream> m_streamReading;	
	MemoryQueue m_bufReadConverted;
	Queue< Ref<AsyncStreamRequest> > m_requestsRead;
	Mutex m_lockReading;
	sl_bool m_flagReading;
	sl_bool m_flagReadingError;
	sl_bool m_flagReadingEnded;
	Memory m_memReading;

	Ref<AsyncStream> m_streamWriting;
	Mutex m_lockWriting;
	sl_bool m_flagWritingError;
	sl_bool m_flagWritingEnded;

protected:
	void _read();
	virtual void onRead(AsyncStream* stream, void* data, sl_uint32 sizeRead, Referable* ref, sl_bool flagError);
	void _processRead(void* data, sl_uint32 size, Referable* refData, sl_bool flagError);
	void _processRead();

	virtual void onWrite(AsyncStream* stream, void* data, sl_uint32 sizeWritten, Referable* ref, sl_bool flagError);
	void _processZeroWrite(void* data, sl_uint32 size, Ptr<IAsyncStreamListener> listener, Ref<Referable> ref);
	void _closeAllReadRequests();

};
SLIB_NAMESPACE_END

#endif
