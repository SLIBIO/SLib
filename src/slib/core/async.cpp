#include "../../../inc/slib/core/async.h"
#include "../../../inc/slib/core/system.h"

SLIB_NAMESPACE_BEGIN

/*************************************
	Async
*************************************/
sl_bool Async::runTask(const Ref<Runnable>& task, const Ref<AsyncLoop>& loop, sl_bool flagRunByThreadPool)
{
	if (loop.isNotNull()) {
		return loop->addTask(task, flagRunByThreadPool);
	}
	return sl_false;
}

sl_bool Async::runTask(const Ref<Runnable>& task, sl_bool flagRunByThreadPool)
{
	return Async::runTask(task, AsyncLoop::getDefault(), flagRunByThreadPool);
}

sl_bool Async::setTimeout(const Ref<Runnable>& task, sl_uint64 delay_ms, const Ref<AsyncLoop>& loop, sl_bool flagRunByThreadPool)
{
	if (loop.isNotNull()) {
		return loop->setTimeout(task, delay_ms, flagRunByThreadPool);
	}
	return sl_false;
}

sl_bool Async::setTimeout(const Ref<Runnable>& task, sl_uint64 delay_ms, sl_bool flagRunByThreadPool)
{
	return Async::setTimeout(task, delay_ms, AsyncLoop::getDefault(), flagRunByThreadPool);
}

Ref<AsyncTimer> Async::addTimer(const Ref<Runnable>& task, sl_uint64 interval_ms, const Ref<AsyncLoop>& loop, sl_bool flagRunByThreadPool)
{
	if (loop.isNotNull()) {
		return loop->addTimer(task, interval_ms, flagRunByThreadPool);
	}
	return Ref<AsyncTimer>::null();
}

Ref<AsyncTimer> Async::addTimer(const Ref<Runnable>& task, sl_uint64 interval_ms, sl_bool flagRunByThreadPool)
{
	return Async::addTimer(task, interval_ms, AsyncLoop::getDefault(), flagRunByThreadPool);
}

void Async::removeTimer(const Ref<AsyncTimer>& timer, const Ref<AsyncLoop>& loop)
{
	if (loop.isNotNull()) {
		return loop->removeTimer(timer);
	}
}

void Async::removeTimer(const Ref<AsyncTimer>& timer)
{
	Async::removeTimer(timer, Ref<AsyncLoop>::null());
}

/*************************************
	AsyncLoop
*************************************/
AsyncLoop::AsyncLoop()
{
	m_flagRunning = sl_false;
	m_handle = sl_null;
	m_flagNonIO = sl_false;
}

AsyncLoop::~AsyncLoop()
{
	release();
}

Ref<AsyncLoop> AsyncLoop::create()
{
	Ref<AsyncLoop> ret;
	void* handle = __createHandle();
	if (handle) {
		Ref<ThreadPool> threadPool = ThreadPool::create(0, 16);
		if (threadPool.isNotNull()) {
			ret = new AsyncLoop;
			if (ret.isNotNull()) {
				ret->m_flagNonIO = sl_false;
				ret->m_handle = handle;
				ret->m_threadPool = threadPool;
				ret->m_thread = Thread::start(SLIB_CALLBACK_CLASS(AsyncLoop, __runLoop, ret.get()));
				if (ret->m_thread.isNotNull()) {
					ret->m_flagRunning = sl_true;
					return ret;
				}
				ret.setNull();
			}
		}
		__closeHandle(handle);
	}
	return ret;
}

Ref<AsyncLoop> AsyncLoop::createNonIO()
{
	Ref<AsyncLoop> ret;
	Ref<ThreadPool> threadPool = ThreadPool::create(0, 16);
	if (threadPool.isNotNull()) {
		ret = new AsyncLoop;
		if (ret.isNotNull()) {
			ret->m_flagNonIO = sl_true;
			ret->m_threadPool = threadPool;
			ret->m_thread = Thread::start(SLIB_CALLBACK_CLASS(AsyncLoop, _runLoopNonIO, ret.get()));
			if (ret->m_thread.isNotNull()) {
				ret->m_flagRunning = sl_true;
				return ret;
			}
			ret.setNull();
		}
	}
	return ret;
}

void AsyncLoop::release()
{
	ObjectLocker lock(this);
	if (!m_flagRunning) {
		return;
	}
	m_flagRunning = sl_false;
	
	m_threadPool->release();
	
	m_thread->finish();
	if (!m_flagNonIO) {
		__wake();
	}
	m_thread->finishAndWait();

	__closeHandle(m_handle);

	m_queueInstancesOrder.removeAll();
	m_queueInstancesClosing.removeAll();
	m_queueInstancesClosed.removeAll();

	m_queueTasks.removeAll();
	
	MutexLocker lockTime(&m_lockTimeTasks);
	m_timeTasks.removeAll();
}

sl_bool AsyncLoop::isRunning()
{
	return m_flagRunning;
}

void AsyncLoop::wake()
{
	ObjectLocker lock(this);
	if (!m_flagRunning) {
		return;
	}
	if (m_flagNonIO) {
		m_thread->wake();
	} else {
		__wake();
	}
}

sl_bool AsyncLoop::attachInstance(AsyncInstance* instance)
{
	if (m_flagNonIO) {
		return sl_false;
	}
	if (m_handle) {
		if (instance && instance->isOpened()) {
			ObjectLocker lock(this);
			return __attachInstance(instance);
		}
	}
	return sl_false;
}

void AsyncLoop::closeInstance(AsyncInstance* instance)
{
	if (m_handle) {
		if (instance && instance->isOpened()) {
			if (!(instance->isClosing())) {
				instance->setClosing();
				m_queueInstancesClosing.push(instance);
				wake();
			}
		}
	}
}

void AsyncLoop::requestOrder(AsyncInstance* instance)
{
	if (m_handle) {
		if (instance && instance->isOpened()) {				
			instance->addToQueue(m_queueInstancesOrder);
			wake();
		}
	}
}

sl_int32 AsyncLoop::_getTimeout()
{
	m_timeCounter.update();
	if (m_queueInstancesOrder.isNotEmpty()) {
		return 0;
	}
	if (m_queueTasks.isNotEmpty()) {
		return 0;
	}
	sl_int32 t1 = _getTimeout_TimeTasks();
	sl_int32 t2 = _getTimeout_Timer();
	if (t1 < 0) {
		return t2;
	} else {
		if (t2 < 0) {
			return t1;
		} else {
			return SLIB_MIN(t1, t2);
		}
	}
}

void AsyncLoop::_stepBegin()
{
	// Orders
	{
		Queue< Ref<AsyncInstance> > instances;
		instances.merge(&m_queueInstancesOrder);
		Ref<AsyncInstance> instance;
		while (instances.pop(&instance)) {
			if (instance.isNotNull() && instance->isOpened()) {
				instance->processOrder();
			}
		}
	}

	// Async Tasks
	{
		Queue< Ref<Runnable> > tasks;
		tasks.merge(&m_queueTasks);
		Ref<Runnable> task;
		while (m_queueTasks.pop(&task)) {
			if (task.isNotNull()) {
				task->run();
			}
		}
	}
}

void AsyncLoop::_stepEnd()
{
	Ref<AsyncInstance> instance;
	while (m_queueInstancesClosing.pop(&instance)) {
		if (instance.isNotNull() && instance->isOpened()) {
			if (!m_flagNonIO) {
				__detachInstance(instance.get());
			}
			instance->close();
			m_queueInstancesClosed.push(instance);
		}
	}
}

sl_uint32 AsyncLoop::getMinimumThreadsCount()
{
	return m_threadPool->getMinimumThreadsCount();
}

void AsyncLoop::setMinimumThreadCount(sl_uint32 n)
{
	m_threadPool->setMinimumThreadsCount(n);
}

sl_uint32 AsyncLoop::getMaximumThreadsCount()
{
	return m_threadPool->getMaximumThreadsCount();
}

void AsyncLoop::setMaximumThreadsCount(sl_uint32 n)
{
	m_threadPool->setMaximumThreadsCount(n);
}

sl_uint32 AsyncLoop::getThreadStackSize()
{
	return m_threadPool->getThreadStackSize();
}

void AsyncLoop::setThreadStackSize(sl_uint32 n)
{
	m_threadPool->setThreadStackSize(n);
}

sl_bool AsyncLoop::addTask(const Ref<Runnable>& task, sl_bool flagRunByThreadPool)
{
	if (task.isNull()) {
		return sl_false;
	}
	if (flagRunByThreadPool) {
		return m_threadPool->addTask(task);
	} else {
		if (m_queueTasks.push(task)) {
			wake();
			return sl_true;
		}
		return sl_false;
	}
}

sl_int32 AsyncLoop::_getTimeout_TimeTasks()
{
	MutexLocker lock(&m_lockTimeTasks);
	sl_uint64 rel = getEllapsedMilliseconds();
	if (m_timeTasks.count() == 0) {
		return -1;
	}
	sl_int32 timeout = -1;
	TreePosition pos;
	TimeTask timeTask;
	Queue< Ref<Runnable> > tasks;
	sl_uint64 t;
	while (m_timeTasks.getFirstPosition(pos, &t, &timeTask)) {
		if (rel >= t) {
			if (timeTask.flagRunByThreadPool) {
				m_threadPool->addTask(timeTask.task);
			} else {
				tasks.push(timeTask.task);
			}
			m_timeTasks.removeAt(pos);
		} else {
			t = t - rel;
			if (t > 1000) {
				t = 1000;
			}
			timeout = (sl_int32)t;
			break;
		}
	}
	lock.unlock();

	Ref<Runnable> task;
	while (tasks.pop(&task)) {
		if (task.isNotNull()) {
			task->run();
		}
	}
	return timeout;
}

sl_bool AsyncLoop::setTimeout(const Ref<Runnable>& task, sl_uint64 ms, sl_bool flagRunByThreadPool)
{
	if (task.isNull()) {
		return sl_false;
	}
	MutexLocker lock(&m_lockTimeTasks);
	TimeTask tt;
	tt.time = getEllapsedMilliseconds() + ms;
	tt.flagRunByThreadPool = flagRunByThreadPool;
	tt.task = task;
	if (m_timeTasks.insert(tt.time, tt, sl_false)) {
		wake();
		return sl_true;
	}
	return sl_false;
}

sl_int32 AsyncLoop::_getTimeout_Timer()
{
	MutexLocker lock(&m_lockTimer);

	sl_int32 timeout = -1;
	Queue< Ref<AsyncTimer> > tasks;

	sl_uint64 rel = getEllapsedMilliseconds();
	Link<Timer>* link = m_queueTimers.getBegin();
	while (link) {
		Ref<AsyncTimer> timer(link->value.timer);
		if (timer.isNotNull()) {
			if (timer->isStarted()) {
				sl_uint64 t = rel - timer->getLastRunTime();
				if (t >= timer->getInterval()) {
					if (link->value.flagRunByThreadPool) {
						m_threadPool->addTask(SLIB_CALLBACK_WEAKREF(AsyncTimer, run, timer));
					} else {
						tasks.push(timer);
					}
					timer->setLastRunTime(rel);
					t = timer->getInterval();
				} else {
					t = timer->getInterval() - t;
				}
				if (t > 1000) {
					t = 1000;
				}
				if (timeout < 0 || t < timeout) {
					timeout = (sl_uint32)t;
				}
			}
			link = link->next;
		} else {
			Link<Timer>* linkRemove = link;
			link = link->next;
			m_queueTimers.removeItem(linkRemove);
		}
	}

	lock.unlock();

	Ref<AsyncTimer> task;
	while (tasks.pop(&task)) {
		if (task.isNotNull()) {
			task->run();
		}
	}
	return timeout;
}

sl_bool AsyncLoop::addTimer(const Ref<AsyncTimer>& timer, sl_bool flagRunByThreadPool)
{
	if (timer.isNull()) {
		return sl_false;
	}
	Timer t;
	t.timer = timer;
	t.flagRunByThreadPool = flagRunByThreadPool;
	MutexLocker lock(&m_lockTimer);
	if (m_queueTimers.push(t) != sl_null) {
		wake();
		return sl_true;
	} else {
		return sl_false;
	}
}

void AsyncLoop::removeTimer(const Ref<AsyncTimer>& timer)
{
	MutexLocker lock(&m_lockTimer);
	Timer t;
	t.timer = timer;
	m_queueTimers.removeValue(t);
}

Ref<AsyncTimer> AsyncLoop::addTimer(const Ref<Runnable>& task, sl_uint64 interval_ms, sl_bool flagRunByThreadPool)
{
	Ref<AsyncTimer> timer = AsyncTimer::create(task, interval_ms);
	if (timer.isNotNull()) {
		if (addTimer(timer, flagRunByThreadPool)) {
			return timer;
		}
	}
	return Ref<AsyncTimer>::null();
}

void AsyncLoop::_runLoopNonIO()
{
	while (Thread::isNotStoppingCurrent()) {

		_stepBegin();

		m_queueInstancesClosed.removeAll();
		
		sl_int32 _t = _getTimeout();
		if (_t < 0) {
			_t = 100000;
		}
		Thread::sleep(_t);

		if (m_flagRunning) {
			_stepEnd();
		}
	}
}

Ref<AsyncLoop> AsyncLoop::getDefault()
{
	SLIB_SAFE_STATIC(Ref<AsyncLoop>, ret, create());
	return ret;
}


/*************************************
	AsyncTimer
**************************************/
AsyncTimer::AsyncTimer()
{
	m_flagStarted = sl_false;
	m_nCountRun = 0;
	setLastRunTime(0);
	setMaxConcurrentThread(1);
}

Ref<AsyncTimer> AsyncTimer::create(const Ref<Runnable>& task, sl_uint64 interval_ms, sl_bool flagStart)
{
	Ref<AsyncTimer> ret;
	if (task.isNull()) {
		return ret;
	}
	ret = new AsyncTimer();
	if (ret.isNotNull()) {
		ret->m_flagStarted = flagStart;
		ret->m_task = task;
		ret->m_interval = interval_ms;
	}
	return ret;
}


void AsyncTimer::run()
{
	ObjectLocker lock(this);
	if (m_flagStarted) {
		sl_int32 n = Base::interlockedIncrement32(&m_nCountRun);
		if (n <= (sl_int32)(getMaxConcurrentThread())) {
			lock.unlock();
			m_task->run();
		}
		Base::interlockedDecrement32(&m_nCountRun);
	}
}

void AsyncTimer::stopAndWait()
{
	ObjectLocker lock(this);
	stop();
	sl_uint32 n = 0;
	while (m_nCountRun > 0) {
		System::yield(++n);
	}
}

/*************************************
	AsyncInstance
**************************************/
AsyncInstance::AsyncInstance()
{
	m_handle = sl_null;
	m_flagClosing = sl_false;
	m_flagOrdering = sl_false;
}

Ref<AsyncObject> AsyncInstance::getObject()
{
	return m_object;
}

void AsyncInstance::setObject(AsyncObject* object)
{
	m_object = object;
}

Ref<AsyncLoop> AsyncInstance::getLoop()
{
	Ref<AsyncObject> object(m_object);
	if (object.isNotNull()) {
		return object->getLoop();
	}
	return Ref<AsyncLoop>::null();
}

void AsyncInstance::requestOrder()
{
	Ref<AsyncLoop> loop(getLoop());
	if (loop.isNotNull()) {
		loop->requestOrder(this);
	}
}

void AsyncInstance::addToQueue(Queue< Ref<AsyncInstance> >& queue)
{
	MutexLocker lock(&m_lockOrdering);
	if (!m_flagOrdering) {
		m_flagOrdering = sl_true;
		queue.push(this);
	}
}

void AsyncInstance::processOrder()
{
	MutexLocker lock(&m_lockOrdering);
	m_flagOrdering = sl_false;
	lock.unlock();
	onOrder();
}

/*************************************
	AsyncObject
**************************************/
AsyncObject::AsyncObject()
{
}

AsyncObject::~AsyncObject()
{
	closeInstance();
}

Ref<AsyncLoop> AsyncObject::getLoop()
{
	Ref<AsyncLoop> loop = m_loop;
	if (loop.isNull()) {
		loop = AsyncLoop::getDefault();
	}
	return loop;
}

void AsyncObject::setLoop(const Ref<AsyncLoop>& loop)
{
	m_loop = loop;
}

Ref<AsyncInstance> AsyncObject::getInstance()
{
	return m_instance;
}

void AsyncObject::setInstance(AsyncInstance* instance)
{
	m_instance = instance;
}

void AsyncObject::closeInstance()
{
	ObjectLocker lock(this);
	Ref<AsyncInstance> instance(m_instance);
	if (instance.isNotNull()) {
		Ref<AsyncLoop> loop = getLoop();
		if (loop.isNotNull()) {
			loop->closeInstance(instance.get());
		}
		m_instance.setNull();
	}
}

/*************************************
	AsyncStreamInstance
**************************************/
AsyncStreamRequest::AsyncStreamRequest(void* data, sl_uint32 size, const Referable* refData, const Ptr<IAsyncStreamListener>& listener, sl_bool flagRead)
: m_data(data), m_size(size), m_refData(refData), m_listener(listener), m_flagRead(flagRead)
{
}

Ref<AsyncStreamRequest> AsyncStreamRequest::createRead(void* data, sl_uint32 size, const Referable* refData, const Ptr<IAsyncStreamListener>& listener)
{
	return new AsyncStreamRequest(data, size, refData, listener, sl_true);
}

Ref<AsyncStreamRequest> AsyncStreamRequest::createWrite(void* data, sl_uint32 size, const Referable* refData, const Ptr<IAsyncStreamListener>& listener)
{
	return new AsyncStreamRequest(data, size, refData, listener, sl_false);
}

void IAsyncStreamListener::onRead(AsyncStream* stream, void* data, sl_uint32 sizeRead, const Referable* ref, sl_bool flagError)
{
}

void IAsyncStreamListener::onWrite(AsyncStream* stream, void* data, sl_uint32 sizeWritten, const Referable* ref, sl_bool flagError)
{
}


sl_bool AsyncStreamInstance::read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, const Referable* refData)
{
	if (size == 0) {
		return sl_false;
	}
	Ref<AsyncStreamRequest> req = AsyncStreamRequest::createRead(data, size, refData, listener);
	if (req.isNotNull()) {
		m_requestsRead.push(req);
		return sl_true;
	}
	return sl_false;
}

sl_bool AsyncStreamInstance::write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, const Referable* refData)
{
	if (size == 0) {
		return sl_false;
	}
	Ref<AsyncStreamRequest> req = AsyncStreamRequest::createWrite(data, size, refData, listener);
	if (req.isNotNull()) {
		m_requestsWrite.push(req);
		return sl_true;
	}
	return sl_false;
}

sl_bool AsyncStreamInstance::isSeekable()
{
	return sl_false;
}

sl_bool AsyncStreamInstance::seek(sl_uint64 pos)
{
	return sl_false;
}

sl_uint64 AsyncStreamInstance::getSize()
{
	return 0;
}

/*************************************
	AsyncStream
**************************************/
sl_bool AsyncStream::isSeekable()
{
	return sl_false;
}

sl_bool AsyncStream::seek(sl_uint64 pos)
{
	return sl_false;
}

sl_uint64 AsyncStream::getSize()
{
	return 0;
}

sl_bool AsyncStreamBase::_initialize(AsyncStreamInstance* instance, const Ref<AsyncLoop>& _loop)
{
	if (!instance) {
		return sl_false;
	}
	Ref<AsyncLoop> loop = _loop;
	if (loop.isNull()) {
		loop = AsyncLoop::getDefault();
		if (loop.isNull()) {
			return sl_false;
		}
	}
	instance->setObject(this);
	setInstance(instance);
	setLoop(loop);
	if (loop->attachInstance(instance)) {
		return sl_true;
	}
	return sl_false;
}

void AsyncStreamBase::close()
{
	closeInstance();
}

sl_bool AsyncStreamBase::isOpened()
{
	return getInstance().isNotNull();
}

sl_bool AsyncStreamBase::read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, const Referable* refData)
{
	Ref<AsyncLoop> loop = getLoop();
	if (loop.isNull()) {
		return sl_false;
	}
	Ref<AsyncStreamInstance> instance = getInstance();
	if (instance.isNotNull()) {
		if (instance->read(data, size, listener, refData)) {
			loop->requestOrder(instance.get());
			return sl_true;
		}
	}
	return sl_false;
}

sl_bool AsyncStreamBase::write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, const Referable* refData)
{
	Ref<AsyncLoop> loop = getLoop();
	if (loop.isNull()) {
		return sl_false;
	}
	Ref<AsyncStreamInstance> instance = getInstance();
	if (instance.isNotNull()) {
		if (instance->write(data, size, listener, refData)) {
			loop->requestOrder(instance.get());
			return sl_true;
		}
	}
	return sl_false;
}

sl_bool AsyncStreamBase::isSeekable()
{
	Ref<AsyncStreamInstance> instance = getInstance();
	if (instance.isNotNull()) {
		return instance->isSeekable();
	}
	return sl_false;
}

sl_bool AsyncStreamBase::seek(sl_uint64 pos)
{
	Ref<AsyncStreamInstance> instance = getInstance();
	if (instance.isNotNull()) {
		instance->seek(pos);
		return sl_true;
	}
	return sl_false;
}

sl_uint64 AsyncStreamBase::getSize()
{
	Ref<AsyncStreamInstance> instance = getInstance();
	if (instance.isNotNull()) {
		return instance->getSize();
	}
	return sl_false;
}

Ref<AsyncStream> AsyncStream::create(AsyncStreamInstance* instance, const Ref<AsyncLoop>& loop)
{
	Ref<AsyncStreamBase> ret;
	if (instance) {
		ret = new AsyncStreamBase;
		if (ret.isNotNull()) {
			if (!(ret->_initialize(instance, loop))) {
				ret.setNull();
			}
		}
	}
	return ret;
}

Ref<AsyncStream> AsyncStream::create(AsyncStreamInstance* instance)
{
	if (instance) {
		return AsyncStream::create(instance, AsyncLoop::getDefault());
	}
	return Ref<AsyncStream>::null();
}

/*************************************
	AsyncStreamBaseIO
**************************************/
AsyncStreamBaseIO::AsyncStreamBaseIO()
{
	m_flagProcessRequest = sl_false;
}

sl_bool AsyncStreamBaseIO::read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, const Referable* refData)
{
	if (data && size > 0) {
		if (isOpened()) {
			Ref<AsyncStreamRequest> req = AsyncStreamRequest::createRead(data, size, refData, listener);
			if (req.isNotNull()) {
				return _addRequest(req.get());
			}
		}
	}
	return sl_false;
}

sl_bool AsyncStreamBaseIO::write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, const Referable* refData)
{
	if (data && size > 0) {
		if (isOpened()) {
			Ref<AsyncStreamRequest> req = AsyncStreamRequest::createWrite(data, size, refData, listener);
			if (req.isNotNull()) {
				return _addRequest(req.get());
			}
		}
	}
	return sl_false;
}

sl_bool AsyncStreamBaseIO::_addRequest(AsyncStreamRequest* req)
{
	Ref<AsyncLoop> loop = getLoop();
	if (loop.isNotNull()) {
		ObjectLocker lock(this);
		m_requests.push(req);
		if (!m_flagProcessRequest) {
			m_flagProcessRequest = sl_true;
			lock.unlock();
			loop->addTask(SLIB_CALLBACK_WEAKREF(AsyncStreamBaseIO, _runProcessor, this));
		}
		return sl_true;
	}
	return sl_false;
}

void AsyncStreamBaseIO::_runProcessor()
{
	if (isOpened()) {
		while (Thread::isNotStoppingCurrent()) {
			Ref<AsyncStreamRequest> req;
			{
				ObjectLocker lock(this);
				if (!(m_requests.pop(&req))) {
					m_flagProcessRequest = sl_false;
					break;
				}
			}
			if (req.isNotNull()) {
				processRequest(req.get());
			}
		}
	}
}

/*************************************
	AsyncReader
**************************************/
sl_bool AsyncReader::isOpened()
{
	Ptr<IReader> o = m_reader.lock();
	return o.isNotNull();
}

void AsyncReader::close()
{
	m_reader.setNull();
}

sl_bool AsyncReader::write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, const Referable* refData)
{
	return sl_false;
}

Ptr<IReader> AsyncReader::getReader()
{
	return m_reader;
}

void AsyncReader::processRequest(AsyncStreamRequest* request)
{
	PtrLocker<IReader> reader(m_reader);
	if (reader.isNotNull()) {
		sl_bool flagError = sl_false;
		sl_reg size = reader->read(request->data(), request->size());
		if (size <= 0) {
			flagError = sl_true;
		}
		PtrLocker<IAsyncStreamListener> listener(request->getListener());
		if (listener.isNotNull()) {
			listener->onRead(this, request->data(), (sl_uint32)size, request->getDataReference(), flagError);
		}
	}
}

Ref<AsyncReader> AsyncReader::create(const Ptr<IReader>& reader, const Ref<AsyncLoop>& loop)
{
	Ref<AsyncReader> ret;
	if (reader.isNotNull()) {
		ret = new AsyncReader;
		if (ret.isNotNull()) {
			ret->m_reader = reader;
			ret->setLoop(loop);
		}
	}
	return ret;
}

Ref<AsyncReader> AsyncReader::create(const Ptr<IReader>& reader)
{
	return AsyncReader::create(reader, AsyncLoop::getDefault());
}

/*************************************
	AsyncWriter
**************************************/
sl_bool AsyncWriter::isOpened()
{
	Ptr<IWriter> o = m_writer.lock();
	return o.isNotNull();
}

void AsyncWriter::close()
{
	m_writer.setNull();
}

sl_bool AsyncWriter::read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, const Referable* refData)
{
	return sl_false;
}

Ptr<IWriter> AsyncWriter::getWriter()
{
	return m_writer;
}

void AsyncWriter::processRequest(AsyncStreamRequest* request)
{
	PtrLocker<IWriter> reader(m_writer);
	if (reader.isNotNull()) {
		sl_bool flagError = sl_false;
		sl_reg size = reader->write(request->data(), request->size());
		if (size <= 0) {
			flagError = sl_true;
		}
		PtrLocker<IAsyncStreamListener> listener(request->getListener());
		if (listener.isNotNull()) {
			listener->onWrite(this, request->data(), (sl_uint32)size, request->getDataReference(), flagError);
		}
	}
}

Ref<AsyncWriter> AsyncWriter::create(const Ptr<IWriter>& writer, const Ref<AsyncLoop>& loop)
{
	Ref<AsyncWriter> ret;
	if (writer.isNotNull()) {
		ret = new AsyncWriter;
		if (ret.isNotNull()) {
			ret->m_writer = writer;
			ret->setLoop(loop);
		}
	}
	return ret;
}

Ref<AsyncWriter> AsyncWriter::create(const Ptr<IWriter>& writer)
{
	return AsyncWriter::create(writer, AsyncLoop::getDefault());
}

/*************************************
	AsyncFile
**************************************/
AsyncFile::AsyncFile()
{
}

AsyncFile::~AsyncFile()
{
	close();
}

Ref<File> AsyncFile::getFile()
{
	return m_file;
}

void AsyncFile::close()
{
	m_file.setNull();
}

sl_bool AsyncFile::isOpened()
{
	return m_file.isNotNull();
}

void AsyncFile::processRequest(AsyncStreamRequest* request)
{
	Ref<File> file = m_file;
	if (file.isNotNull()) {
		sl_reg size;
		sl_bool flagError = sl_false;
		if (request->isRead()) {
			size = file->read(request->data(), request->size());
		} else {
			size = file->write(request->data(), request->size());
		}
		if (size <= 0) {
			flagError = sl_true;
			size = 0;
		}
		PtrLocker<IAsyncStreamListener> listener(request->getListener());
		if (listener.isNotNull()) {
			if (request->isRead()) {
				listener->onRead(this, request->data(), (sl_uint32)size, request->getDataReference(), flagError);
			} else {
				listener->onWrite(this, request->data(), (sl_uint32)size, request->getDataReference(), flagError);
			}
		}
	}
}

sl_bool AsyncFile::isSeekable()
{
	return sl_true;
}

sl_bool AsyncFile::seek(sl_uint64 pos)
{
	Ref<File> file = m_file;
	if (file.isNotNull()) {
		return file->seek(pos, seekPosition_Begin);
	}
	return sl_false;
}

sl_uint64 AsyncFile::getSize()
{
	Ref<File> file = m_file;
	if (file.isNotNull()) {
		return file->getSize();
	}
	return 0;
}

Ref<AsyncFile> AsyncFile::create(const Ref<File>& file, const Ref<AsyncLoop>& loop)
{
	Ref<AsyncFile> ret;
	if (file.isNotNull()) {
		ret = new AsyncFile;
		if (ret.isNotNull()) {
			ret->m_file = file;
			ret->setLoop(loop);
		}
	}
	return ret;
}

Ref<AsyncFile> AsyncFile::create(const Ref<File>& file)
{
	return AsyncFile::create(file, AsyncLoop::getDefault());
}

Ref<AsyncFile> AsyncFile::open(const String& path, FileMode mode, const Ref<AsyncLoop>& loop)
{
	Ref<File> file = File::open(path, mode);
	if (file.isNotNull()) {
		return AsyncFile::create(file, loop);
	}
	return Ref<AsyncFile>::null();
}

Ref<AsyncFile> AsyncFile::open(const String& path, FileMode mode)
{
	return AsyncFile::open(path, mode, AsyncLoop::getDefault());
}

Ref<AsyncFile> AsyncFile::openForRead(const String& path, const Ref<AsyncLoop>& loop)
{
	return AsyncFile::open(path, fileMode_Read, loop);
}

Ref<AsyncFile> AsyncFile::openForRead(const String& path)
{
	return AsyncFile::openForRead(path, AsyncLoop::getDefault());
}

Ref<AsyncFile> AsyncFile::openForWrite(const String& path, const Ref<AsyncLoop>& loop)
{
	return AsyncFile::open(path, fileMode_Write, loop);
}

Ref<AsyncFile> AsyncFile::openForWrite(const String& path)
{
	return AsyncFile::openForWrite(path, AsyncLoop::getDefault());
}

Ref<AsyncFile> AsyncFile::openForAppend(const String& path, const Ref<AsyncLoop>& loop)
{
	return AsyncFile::open(path, fileMode_Append, loop);
}

Ref<AsyncFile> AsyncFile::openForAppend(const String& path)
{
	return AsyncFile::openForAppend(path, AsyncLoop::getDefault());
}

/*************************************
	AsyncCopy
**************************************/
Memory IAsyncCopyListener::onAsyncCopyRead(AsyncCopy* task, const Memory& input)
{
	return input;
}

void IAsyncCopyListener::onAsyncCopyWrite(AsyncCopy* task)
{
}

void IAsyncCopyListener::onAsyncCopyExit(AsyncCopy* task)
{
}

AsyncCopy::AsyncCopy()
{
	m_sizeRead = 0;
	m_sizeWritten = 0;
	m_flagReadError = sl_false;
	m_flagWriteError = sl_false;
	m_flagRunning = sl_true;
	m_flagStarted = sl_false;
}

AsyncCopy::~AsyncCopy()
{
	close();
}

Ref<AsyncCopy> AsyncCopy::create(const Ref<AsyncStream>& source, const Ref<AsyncStream>& target
	, sl_uint64 sizeTotal, const Ptr<IAsyncCopyListener>& listener
	, sl_uint32 bufferSize, sl_uint32 bufferCount
	, sl_bool flagStart)
{
	if (target.isNull()) {
		return Ref<AsyncCopy>::null();
	}
	if (source.isNull()) {
		return Ref<AsyncCopy>::null();
	}
	if (sizeTotal == 0) {
		return Ref<AsyncCopy>::null();
	}
	if (bufferSize == 0 || bufferCount == 0) {
		return Ref<AsyncCopy>::null();
	}
	Ref<AsyncCopy> ret = new AsyncCopy();
	if (ret.isNotNull()) {
		ret->m_source = source;
		ret->m_target = target;
		ret->m_listener = listener;
		ret->m_sizeTotal = sizeTotal;
		for (sl_uint32 i = 0; i < bufferCount; i++) {
			Memory mem = Memory::create(bufferSize);
			if (mem.isNotEmpty()) {
				Ref<Buffer> buf = new Buffer;
				if (buf.isNotNull()) {
					buf->mem = mem;
					ret->m_buffersRead.push(buf);
				}
			} else {
				return Ref<AsyncCopy>::null();
			}
		}
		if (flagStart) {
			if (ret->start()) {
				return ret;
			}
		} else {
			return ret;
		}
	}
	return Ref<AsyncCopy>::null();
}

sl_bool AsyncCopy::start()
{
	ObjectLocker lock(this);
	if (!m_flagStarted) {
		m_flagStarted = sl_true;
		enqueue();
		return sl_true;
	}
	return sl_false;
}

void AsyncCopy::close()
{
	ObjectLocker lock(this);
	if (m_flagRunning) {
		m_flagRunning = sl_false;
		PtrLocker<IAsyncCopyListener> listener(m_listener);
		if (listener.isNotNull()) {
			listener->onAsyncCopyExit(this);
		}
		m_source.setNull();
		m_target.setNull();
		m_bufferReading.setNull();
		m_buffersRead.removeAll();
		m_bufferWriting.setNull();
		m_buffersWrite.removeAll();
	}
}

void AsyncCopy::onRead(AsyncStream* stream, void* data, sl_uint32 sizeRead, const Referable* ref, sl_bool flagError)
{
	ObjectLocker lock(this);
	if (!m_flagRunning) {
		return;
	}
	if (flagError) {
		m_flagReadError = sl_true;
	}

	Ref<Buffer> bufferReading = m_bufferReading;
	m_bufferReading.setNull();

	if (bufferReading.isNotNull()) {
		m_sizeRead += sizeRead;
		Memory memWrite = bufferReading->mem.sub(0, sizeRead);
		if (memWrite.isEmpty()) {
			m_flagReadError = sl_true;
		} else {
			PtrLocker<IAsyncCopyListener> listener(m_listener);
			if (listener.isNotNull()) {
				memWrite = listener->onAsyncCopyRead(this, memWrite);
			}
			if (memWrite.isNotEmpty()) {
				bufferReading->memWrite = memWrite;
				m_buffersWrite.pushBack(bufferReading);
			} else {
				bufferReading->memWrite.setNull();
				m_buffersRead.pushBack(bufferReading);
			}
		}
	}

	enqueue();
}

void AsyncCopy::onWrite(AsyncStream* stream, void* data, sl_uint32 sizeWritten, const Referable* ref, sl_bool flagError)
{
	ObjectLocker lock(this);
	if (!m_flagRunning) {
		return;
	}
	if (flagError) {
		m_flagWriteError = sl_true;
	}

	Ref<Buffer> bufferWriting = m_bufferWriting;
	m_bufferWriting.setNull();

	if (bufferWriting.isNotNull()) {
		m_sizeWritten += sizeWritten;
		bufferWriting->memWrite.setNull();
		m_buffersRead.pushBack(bufferWriting);
		PtrLocker<IAsyncCopyListener> listener(m_listener);
		if (listener.isNotNull()) {
			listener->onAsyncCopyWrite(this);
		}
	}

	enqueue();
}

void AsyncCopy::enqueue()
{
	if (!m_flagRunning) {
		return;
	}
	// read
	do {
		if (m_flagReadError) {
			break;
		}
		if (m_sizeRead >= m_sizeTotal) {
			break;
		}
		if (m_bufferReading.isNotNull()) {
			break;
		}
		Ref<Buffer> buffer;
		if (m_buffersRead.popFront(&buffer)) {
			sl_uint32 size = (sl_uint32)(buffer->mem.getSize());
			sl_uint64 remain = m_sizeTotal - m_sizeRead;
			if (size > remain) {
				size = (sl_uint32)remain;
			}
			buffer->memRead = buffer->mem.sub(0, size);
			sl_bool bRet = sl_false;
			if (buffer->memRead.isNotEmpty()) {
				Ref<AsyncStream> source = m_source;
				if (source.isNotNull()) {
					bRet = source->readToMemory(buffer->memRead, (WeakRef<AsyncCopy>)this);
				}
			}
			if (bRet) {
				m_bufferReading = buffer;
			} else {
				m_flagReadError = sl_true;
			}
		}
	} while (0);
	// write
	do {
		if (m_flagWriteError) {
			break;
		}
		if (m_bufferWriting.isNotNull()) {
			break;
		}
		Ref<Buffer> buffer;
		if (m_buffersWrite.popFront(&buffer)) {
			sl_bool bRet = sl_false;
			Ref<AsyncStream> target = m_target;
			if (target.isNotNull()) {
				bRet = target->writeFromMemory(buffer->memWrite, (WeakRef<AsyncCopy>)this);
			}
			if (bRet) {
				m_bufferWriting = buffer;
			} else {
				m_flagWriteError = sl_true;
			}
		}
	} while (0);
	if (m_bufferReading.isNull() && m_bufferWriting.isNull()) {
		close();
	}
}


/**********************************************
	AsyncOutputBufferElement
**********************************************/
AsyncOutputBufferElement::AsyncOutputBufferElement()
{
	m_sizeBody = 0;
}

AsyncOutputBufferElement::AsyncOutputBufferElement(const Memory& header)
{
	m_header.push(header);
	m_sizeBody = 0;
}

AsyncOutputBufferElement::AsyncOutputBufferElement(AsyncStream* stream, sl_uint64 size)
{
	m_body = stream;
	m_sizeBody = size;
}

sl_bool AsyncOutputBufferElement::isEmpty() const
{
	if (m_header.getSize() == 0 && (m_sizeBody == 0 || m_body.isNull())) {
		return sl_true;
	}
	return sl_false;
}

sl_bool AsyncOutputBufferElement::isEmptyBody() const
{
	if (m_sizeBody == 0 || m_body.isNull()) {
		return sl_true;
	}
	return sl_false;
}

sl_bool AsyncOutputBufferElement::addHeader(const Memory& header)
{
	return m_header.push(header);
}

void AsyncOutputBufferElement::setBody(AsyncStream* stream, sl_uint64 size)
{
	m_body = stream;
	m_sizeBody = size;
}

/**********************************************
	AsyncOutputBuffer
**********************************************/
AsyncOutputBuffer::AsyncOutputBuffer()
{
	m_lengthOutput = 0;
}

void AsyncOutputBuffer::clearOutput()
{
	m_lengthOutput = 0;
	m_queueOutput.removeAll();
}

sl_bool AsyncOutputBuffer::write(const void* buf, sl_size size)
{
	return write(Memory::create(buf, size));
}

sl_bool AsyncOutputBuffer::write(const Memory& mem)
{
	if (mem.isEmpty()) {
		return sl_false;
	}
	ObjectLocker lock(this);
	Link< Ref<AsyncOutputBufferElement> >* end = m_queueOutput.getEnd();
	if (end && end->value->isEmptyBody()) {
		if (end->value->addHeader(mem)) {
			m_lengthOutput += mem.getSize();
		} else {
			return sl_false;
		}
	} else {
		Ref<AsyncOutputBufferElement> data = new AsyncOutputBufferElement(mem);
		if (data.isNotNull()) {
			m_queueOutput.push(data);
			m_lengthOutput += mem.getSize();
		} else {
			return sl_false;
		}
	}
	return sl_true;
}

sl_bool AsyncOutputBuffer::copyFrom(AsyncStream* stream, sl_uint64 size)
{
	if (size == 0) {
		return sl_true;
	}
	if (!stream) {
		return sl_false;
	}
	ObjectLocker lock(this);
	Link< Ref<AsyncOutputBufferElement> >* end = m_queueOutput.getEnd();
	if (end && end->value->isEmptyBody()) {
		end->value->setBody(stream, size);
		m_lengthOutput += size;
	} else {
		Ref<AsyncOutputBufferElement> data = new AsyncOutputBufferElement(stream, size);
		if (data.isNotNull()) {
			if (m_queueOutput.push(data)) {
				m_lengthOutput += size;
			} else {
				return sl_false;
			}
		} else {
			return sl_false;
		}
	}
	return sl_true;
}

sl_bool AsyncOutputBuffer::copyFromFile(const String& path)
{
	if (!(File::exists(path))) {
		return sl_false;
	}
	sl_uint64 size = File::getSize(path);
	if (size > 0) {
		Ref<AsyncFile> file = AsyncFile::openForRead(path);
		if (file.isNotNull()) {
			return copyFrom(file.get(), size);
		} else {
			return sl_false;
		}
	}
	return sl_true;
}

/**********************************************
	AsyncOutput
**********************************************/
void IAsyncOutputListener::onAsyncOutputError(AsyncOutput* output)
{
}

void IAsyncOutputListener::onAsyncOutputComplete(AsyncOutput* output)
{
}

AsyncOutput::AsyncOutput()
{
	m_flagClosed = sl_false;
	m_flagWriting = sl_false;
}

AsyncOutput::~AsyncOutput()
{
	close();
}

void AsyncOutput::close()
{
	ObjectLocker lock(this);
	if (m_flagClosed) {
		return;
	}
	m_flagClosed = sl_true;
	Ref<AsyncCopy> copy = m_copy;
	if (copy.isNotNull()) {
		copy->close();
	}
	m_copy.setNull();
	m_streamOutput.setNull();
}

Ref<AsyncOutput> AsyncOutput::create(const Ref<AsyncStream>& streamOutput, const Ptr<IAsyncOutputListener>& listener, sl_uint32 sizeBuffer)
{
	if (streamOutput.isNull()) {
		return Ref<AsyncOutput>::null();
	}
	Memory buffer = Memory::create(sizeBuffer);
	if (buffer.isEmpty()) {
		return Ref<AsyncOutput>::null();
	}
	Ref<AsyncOutput> ret = new AsyncOutput;
	if (ret.isNotNull()) {
		ret->m_streamOutput = streamOutput;
		ret->m_bufWrite = buffer;
		ret->m_sizeBuffer = sizeBuffer;
		ret->m_listener = listener;
	}
	return ret;
}

void AsyncOutput::mergeBuffer(AsyncOutputBuffer* buffer)
{
	ObjectLocker lock(this);
	m_queueOutput.merge(&(buffer->m_queueOutput));
	m_lengthOutput += buffer->m_lengthOutput;
}

void AsyncOutput::startWriting()
{
	_write(sl_false);
}

void AsyncOutput::_write(sl_bool flagCompleted)
{
	ObjectLocker lock(this);
	if (m_flagClosed) {
		return;
	}
	if (m_flagWriting) {
		return;
	}
	while (1) {
		if (m_elementWriting.isNotNull()) {
			if (m_elementWriting->isEmpty()) {
				m_elementWriting.setNull();
			} else {
				break;
			}
		}
		if (!(m_queueOutput.pop(&m_elementWriting))) {
			if (flagCompleted) {
				_onComplete();
			}
			return;
		}
	}
	MemoryQueue& header = m_elementWriting->getHeader();
	if (header.getSize() > 0) {
		sl_uint32 size = (sl_uint32)(header.pop(m_bufWrite.getBuf(), m_bufWrite.getSize()));
		if (size > 0) {
			m_flagWriting = sl_true;
			if (!(m_streamOutput->write(m_bufWrite.getBuf(), size, (WeakRef<AsyncOutput>)this, m_bufWrite.getObject()))) {
				m_flagWriting = sl_false;
				_onError();
			}
		}
	} else {
		sl_uint64 sizeBody = m_elementWriting->getBodySize();
		Ref<AsyncStream> body = m_elementWriting->getBody();
		if (sizeBody != 0 && body.isNotNull()) {
			m_flagWriting = sl_true;
			m_elementWriting.setNull();
			Ref<AsyncCopy> copy = AsyncCopy::create(body, m_streamOutput, sizeBody, (WeakRef<AsyncOutput>)this, 0x10000, 3);
			if (copy.isNotNull()) {
				m_copy = copy;
			} else {
				m_flagWriting = sl_false;
				_onError();
			}
		}
	}
}

void AsyncOutput::onAsyncCopyExit(AsyncCopy* task)
{
	m_flagWriting = sl_false;
	if (task->isErrorOccured()) {
		_onError();
		return;
	}
	if (task->isCompleted()) {
		_write(sl_true);
	} else {
		_onError();
	}
}

void AsyncOutput::onWrite(AsyncStream* stream, void* data, sl_uint32 sizeWritten, const Referable* ref, sl_bool flagError)
{
	m_flagWriting = sl_false;
	if (flagError) {
		_onError();
		return;
	}
	_write(sl_true);
}

void AsyncOutput::_onError()
{
	PtrLocker<IAsyncOutputListener> listener(m_listener);
	if (listener.isNotNull()) {
		listener->onAsyncOutputError(this);
	}
}

void AsyncOutput::_onComplete()
{
	PtrLocker<IAsyncOutputListener> listener(m_listener);
	if (listener.isNotNull()) {
		listener->onAsyncOutputComplete(this);
	}
}


/**********************************************
	AsyncStreamFilter
**********************************************/
AsyncStreamFilter::AsyncStreamFilter()
{
	m_flagOpened = sl_true;

	m_flagReading = sl_false;
	m_flagReadingError = sl_false;
	m_flagReadingEnded = sl_false;

	m_flagWritingError = sl_false;
	m_flagWritingEnded = sl_false;
}

AsyncStreamFilter::~AsyncStreamFilter()
{
	close();
}

Ref<AsyncStream> AsyncStreamFilter::getReadingStream() const
{
	return m_streamReading;
}

void AsyncStreamFilter::setReadingStream(const Ref<AsyncStream>& stream)
{
	m_streamReading = stream;
}

Ref<AsyncStream> AsyncStreamFilter::getWritingStream() const
{
	return m_streamWriting;
}

void AsyncStreamFilter::setWritingStream(const Ref<AsyncStream>& stream)
{
	m_streamWriting = stream;
}

void AsyncStreamFilter::close()
{
	MutexLocker lock(&m_lockReading, &m_lockWriting);
	if (m_flagOpened) {
		m_flagOpened = sl_false;
	}
	setReadingEnded();
	setWritingEnded();
}

sl_bool AsyncStreamFilter::isOpened()
{
	return m_flagOpened;
}

sl_bool AsyncStreamFilter::read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, const Referable* ref)
{
	Ref<AsyncLoop> loop = getLoop();
	if (loop.isNull()) {
		return sl_false;
	}
	Ref<AsyncStreamRequest> request = AsyncStreamRequest::createRead(data, size, ref, listener);
	if (request.isNotNull()) {
		MutexLocker lock(&m_lockReading);
		if (!m_flagOpened) {
			return sl_false;
		}
		if (m_requestsRead.push(request)) {
			if (m_bufReadConverted.getSize() > 0) {
				loop->addTask(SLIB_CALLBACK_WEAKREF(AsyncStreamFilter, _processRead, this), sl_false);
			}
			if (m_flagReadingEnded) {
				return sl_false;
			}
			_read();
			return sl_true;
		}
	}
	return sl_false;
}

void AsyncStreamFilter::addReadData(void* data, sl_uint32 size, const Referable* refData)
{
	if (size > 0) {
		MutexLocker lock(&m_lockReading);
		Memory memConv = filterRead(data, size, refData);
		if (memConv.isNotEmpty()) {
			m_bufReadConverted.add(memConv);
		}
	}
}

void AsyncStreamFilter::addReadData(const Memory& data)
{
	if (data.isNotEmpty()) {
		sl_size size = data.getSize();
		if (size >= 0x80000000) {
			return;
		}
		addReadData(data.getBuf(), (sl_uint32)(size), data.getObject());
	}
}

void AsyncStreamFilter::addReadData(void* data, sl_uint32 size)
{
	addReadData(Memory::create(data, size));
}

void AsyncStreamFilter::setReadingBufferSize(sl_uint32 sizeBuffer)
{
	if (sizeBuffer > 0) {
		m_memReading = Memory::create(sizeBuffer);
	}
}

void AsyncStreamFilter::_read()
{
	Ref<AsyncStream> stream = m_streamReading;
	if (stream.isNull()) {
		return;
	}
	if (m_flagReading) {
		return;
	}
	if (m_flagReadingEnded) {
		return;
	}
	if (m_flagReadingError) {
		return;
	}
	Memory mem = m_memReading;
	if (mem.isEmpty()) {
		mem = Memory::create(SLIB_ASYNC_STREAM_FILTER_DEFAULT_BUFFER_SIZE);
		if (mem.isNull()) {
			return;
		}
		m_memReading = mem;
	}
	if (stream->readToMemory(m_memReading, WeakRef<AsyncStreamFilter>(this))) {
		m_flagReading = sl_true;
	}
}

void AsyncStreamFilter::onRead(AsyncStream* stream, void* data, sl_uint32 sizeRead, const Referable* ref, sl_bool flagError)
{
	MutexLocker lock(&m_lockReading);
	m_flagReading = sl_false;
	_processRead(data, sizeRead, ref, flagError);
}

void AsyncStreamFilter::_processRead(void* data, sl_uint32 size, const Referable* refData, sl_bool flagError)
{
	MutexLocker lock(&m_lockReading);
	if (!m_flagOpened) {
		return;
	}
	if (size > 0) {
		addReadData(data, size, refData);
	}
	if (flagError) {
		m_flagReadingError = sl_true;
	}
	if (m_bufReadConverted.getSize() > 0) {
		for (;;) {
			Ref<AsyncStreamRequest> req;
			if (!(m_requestsRead.pop(&req))) {
				return;
			}
			if (req.isNotNull()) {
				sl_size _m = m_bufReadConverted.pop(req->data(), req->size());
				sl_uint32 m = (sl_uint32)(_m);
				if (m_bufReadConverted.getSize() == 0) {
					PtrLocker<IAsyncStreamListener> listener(req->getListener());
					if (listener.isNotNull()) {
						listener->onRead(this, req->data(), m, req->getDataReference(), m_flagReadingError);
					}
					break;
				} else {
					PtrLocker<IAsyncStreamListener> listener(req->getListener());
					if (listener.isNotNull()) {
						listener->onRead(this, req->data(), m, req->getDataReference(), sl_false);
					}
				}
			}
		}
	}
	if (m_flagReadingError) {
		_closeAllReadRequests();
		return;
	}
	if (m_requestsRead.isNotEmpty()) {
		_read();
	}
}

void AsyncStreamFilter::_processRead()
{
	_processRead(sl_null, 0, sl_null, sl_false);
}

class _AsyncStreamFilter_WriteRequest : public Referable
{
public:
	Memory memConverted;
	void* data;
	sl_uint32 size;
	Ref<Referable> refData;
	Ptr<IAsyncStreamListener> listener;
};

sl_bool AsyncStreamFilter::write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, const Referable* ref)
{
	MutexLocker lock(&m_lockWriting);
	Ref<AsyncStream> stream = m_streamWriting;
	if (stream.isNull()) {
		return sl_false;
	}
	if (m_flagWritingError) {
		return sl_false;
	}
	if (m_flagWritingEnded) {
		return sl_false;
	}
	if (size > 0) {
		Memory memConv = filterWrite(data, size, ref);
		if (memConv.isNotEmpty()) {
			Ref<_AsyncStreamFilter_WriteRequest> req = new _AsyncStreamFilter_WriteRequest;
			if (req.isNotNull()) {
				req->memConverted = memConv;
				req->data = data;
				req->size = size;
				req->refData = ref;
				req->listener = listener;
				return stream->write(memConv.getBuf(), (sl_uint32)(memConv.getSize()), WeakRef<AsyncStreamFilter>(this), req.get());
			}
		} else {
			if (listener.isNull()) {
				return sl_true;
			}
			Ref<AsyncLoop> loop = getLoop();
			if (loop.isNotNull()) {
				loop->addTask(SLIB_CALLBACK_WEAKREF(AsyncStreamFilter, _processZeroWrite, this, data, size, listener, Ref<Referable>(ref)), sl_false);
				return sl_true;
			}
		}
	}
	return sl_false;
}

void AsyncStreamFilter::onWrite(AsyncStream* stream, void* data, sl_uint32 sizeWritten, const Referable* ref, sl_bool flagError)
{
	if (flagError) {
		m_flagWritingError = sl_true;
	}
	_AsyncStreamFilter_WriteRequest* req = (_AsyncStreamFilter_WriteRequest*)(ref);
	if (req) {
		PtrLocker<IAsyncStreamListener> listener(req->listener);
		if (listener.isNotNull()) {
			listener->onWrite(this, req->data, req->size, req->refData.get(), m_flagWritingError);
		}
	}
}

void AsyncStreamFilter::_processZeroWrite(void* data, sl_uint32 size, Ptr<IAsyncStreamListener> _listener, Ref<Referable> ref)
{
	PtrLocker<IAsyncStreamListener> listener(_listener);
	if (listener.isNotNull()) {
		listener->onWrite(this, data, size, ref.get(), m_flagWritingError);
	}
}

Memory AsyncStreamFilter::filterRead(void* data, sl_uint32 size, const Referable* refData)
{
	return Memory::createStatic(data, size, refData);
}

Memory AsyncStreamFilter::filterWrite(void* data, sl_uint32 size, const Referable* refData)
{
	return Memory::createStatic(data, size, refData);
}

void AsyncStreamFilter::setReadingError()
{
	m_flagReadingError = sl_true;
}

void AsyncStreamFilter::setReadingEnded()
{
	m_flagReadingEnded = sl_true;
	m_streamReading.setNull();
}


void AsyncStreamFilter::setWritingError()
{
	m_flagWritingError = sl_true;
}

void AsyncStreamFilter::setWritingEnded()
{
	m_flagWritingEnded = sl_true;
	m_streamWriting.setNull();
}

void AsyncStreamFilter::_closeAllReadRequests()
{
	Ref<AsyncStreamRequest> req;
	while (m_requestsRead.pop(&req)) {
		if (req.isNotNull()) {
			PtrLocker<IAsyncStreamListener> listener(req->getListener());
			if (listener.isNotNull()) {
				listener->onRead(this, req->data(), 0, req->getDataReference(), sl_true);
			}
		}
	}
}
SLIB_NAMESPACE_END
