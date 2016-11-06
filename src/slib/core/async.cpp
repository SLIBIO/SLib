#include "../../../inc/slib/core/async.h"
#include "../../../inc/slib/core/system.h"

SLIB_NAMESPACE_BEGIN

/*************************************
	Async
*************************************/
sl_bool Async::runTask(const Ref<Runnable>& task, const Ref<AsyncLoop>& loop)
{
	if (loop.isNotNull()) {
		return loop->addTask(task);
	}
	return sl_false;
}

sl_bool Async::runTask(const Ref<Runnable>& task)
{
	return Async::runTask(task, AsyncLoop::getDefault());
}

sl_bool Async::setTimeout(const Ref<Runnable>& task, sl_uint64 delay_ms, const Ref<AsyncLoop>& loop)
{
	if (loop.isNotNull()) {
		return loop->setTimeout(task, delay_ms);
	}
	return sl_false;
}

sl_bool Async::setTimeout(const Ref<Runnable>& task, sl_uint64 delay_ms)
{
	return Async::setTimeout(task, delay_ms, AsyncLoop::getDefault());
}

Ref<AsyncTimer> Async::addTimer(const Ref<Runnable>& task, sl_uint64 interval_ms, const Ref<AsyncLoop>& loop)
{
	if (loop.isNotNull()) {
		return loop->addTimer(task, interval_ms);
	}
	return Ref<AsyncTimer>::null();
}

Ref<AsyncTimer> Async::addTimer(const Ref<Runnable>& task, sl_uint64 interval_ms)
{
	return Async::addTimer(task, interval_ms, AsyncLoop::getDefault());
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

SLIB_DEFINE_OBJECT(AsyncLoop, Object)

AsyncLoop::AsyncLoop()
{
	m_flagInit = sl_false;
	m_flagRunning = sl_false;
}

AsyncLoop::~AsyncLoop()
{
	release();
}

Ref<AsyncLoop> AsyncLoop::getDefault()
{
	SLIB_SAFE_STATIC(Ref<AsyncLoop>, ret, create())
	if (SLIB_SAFE_STATIC_CHECK_FREED(ret)) {
		return Ref<AsyncLoop>::null();
	}
	return ret;
}

void AsyncLoop::releaseDefault()
{
	Ref<AsyncLoop> loop = getDefault();
	if (loop.isNotNull()) {
		loop->release();
	}
}

Ref<AsyncLoop> AsyncLoop::create(sl_bool flagAutoStart)
{
	Ref<AsyncLoop> ret = new AsyncLoop;
	if (ret.isNotNull()) {
		ret->m_thread = Thread::create(SLIB_CALLBACK_CLASS(AsyncLoop, _runLoop, ret.ptr));
		if (ret->m_thread.isNotNull()) {
			ret->m_flagInit = sl_true;
			if (flagAutoStart) {
				ret->start();
			}
			return ret;
		}
	}
	return Ref<AsyncLoop>::null();
}

void AsyncLoop::release()
{
	ObjectLocker lock(this);
	if (!m_flagInit) {
		return;
	}
	m_flagInit = sl_false;

	if (m_flagRunning) {
		m_flagRunning = sl_false;
		lock.unlock();
		m_thread->finishAndWait();
	}

	m_queueTasks.removeAll();
	
	MutexLocker lockTime(&m_lockTimeTasks);
	m_timeTasks.removeAll();
}

void AsyncLoop::start()
{
	ObjectLocker lock(this);
	if (!m_flagInit) {
		return;
	}
	if (m_flagRunning) {
		return;
	}
	m_flagRunning = sl_true;
	if (!(m_thread->start())) {
		m_flagRunning = sl_false;
	}
}

sl_bool AsyncLoop::isRunning()
{
	return m_flagRunning;
}

void AsyncLoop::_wake()
{
	ObjectLocker lock(this);
	if (!m_flagRunning) {
		return;
	}
	m_thread->wake();
}

sl_int32 AsyncLoop::_getTimeout()
{
	m_timeCounter.update();
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

sl_bool AsyncLoop::addTask(const Ref<Runnable>& task)
{
	if (task.isNull()) {
		return sl_false;
	}
	if (m_queueTasks.push(task)) {
		_wake();
		return sl_true;
	}
	return sl_false;
}

sl_int32 AsyncLoop::_getTimeout_TimeTasks()
{
	MutexLocker lock(&m_lockTimeTasks);
	sl_uint64 rel = getEllapsedMilliseconds();
	if (m_timeTasks.getCount() == 0) {
		return -1;
	}
	sl_int32 timeout = -1;
	TreePosition pos;
	TimeTask timeTask;
	Queue< Ref<Runnable> > tasks;
	sl_uint64 t;
	while (m_timeTasks.getFirstPosition(pos, &t, &timeTask)) {
		if (rel >= t) {
			tasks.push(timeTask.task);
			m_timeTasks.removeAt(pos);
		} else {
			t = t - rel;
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

sl_bool AsyncLoop::setTimeout(const Ref<Runnable>& task, sl_uint64 ms)
{
	if (task.isNull()) {
		return sl_false;
	}
	MutexLocker lock(&m_lockTimeTasks);
	TimeTask tt;
	tt.time = getEllapsedMilliseconds() + ms;
	tt.task = task;
	if (m_timeTasks.put(tt.time, tt, MapPutMode::AddAlways)) {
		_wake();
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
					tasks.push(timer);
					timer->setLastRunTime(rel);
					t = timer->getInterval();
				} else {
					t = timer->getInterval() - t;
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

sl_bool AsyncLoop::Timer::operator==(const AsyncLoop::Timer& other) const
{
	return timer == other.timer;
}

sl_bool AsyncLoop::addTimer(const Ref<AsyncTimer>& timer)
{
	if (timer.isNull()) {
		return sl_false;
	}
	Timer t;
	t.timer = timer;
	MutexLocker lock(&m_lockTimer);
	if (m_queueTimers.push(t) != sl_null) {
		_wake();
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

Ref<AsyncTimer> AsyncLoop::addTimer(const Ref<Runnable>& task, sl_uint64 interval_ms)
{
	Ref<AsyncTimer> timer = AsyncTimer::create(task, interval_ms);
	if (timer.isNotNull()) {
		if (addTimer(timer)) {
			return timer;
		}
	}
	return Ref<AsyncTimer>::null();
}

sl_uint64 AsyncLoop::getEllapsedMilliseconds()
{
	return m_timeCounter.getEllapsedMilliseconds();
}

void AsyncLoop::_runLoop()
{
	while (Thread::isNotStoppingCurrent()) {

		// Async Tasks
		{
			Queue< Ref<Runnable> > tasks;
			tasks.merge(&m_queueTasks);
			Ref<Runnable> task;
			while (tasks.pop(&task)) {
				if (task.isNotNull()) {
					task->run();
				}
			}
		}
		
		sl_int32 _t = _getTimeout();
		if (_t < 0) {
			_t = 10000;
		}
		if (_t > 10000) {
			_t = 10000;
		}
		Thread::sleep(_t);

	}
}

/*************************************
	AsyncTimer
**************************************/

SLIB_DEFINE_OBJECT(AsyncTimer, Object)

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

void AsyncTimer::start()
{
	m_flagStarted = sl_true;
}

void AsyncTimer::stop()
{
	m_flagStarted = sl_false;
}

sl_bool AsyncTimer::isStarted()
{
	return m_flagStarted;
}

Ref<Runnable> AsyncTimer::getTask()
{
	return m_task;
}

sl_uint64 AsyncTimer::getInterval()
{
	return m_interval;
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
	AsyncLoop
 *************************************/

SLIB_DEFINE_OBJECT(AsyncIoLoop, Object)

AsyncIoLoop::AsyncIoLoop()
{
	m_flagInit = sl_false;
	m_flagRunning = sl_false;
	m_handle = sl_null;
}

AsyncIoLoop::~AsyncIoLoop()
{
	release();
}

Ref<AsyncIoLoop> AsyncIoLoop::getDefault()
{
	SLIB_SAFE_STATIC(Ref<AsyncIoLoop>, ret, create())
	if (SLIB_SAFE_STATIC_CHECK_FREED(ret)) {
		return Ref<AsyncIoLoop>::null();
	}
	return ret;
}

void AsyncIoLoop::releaseDefault()
{
	Ref<AsyncIoLoop> loop = getDefault();
	if (loop.isNotNull()) {
		loop->release();
	}
}

Ref<AsyncIoLoop> AsyncIoLoop::create(sl_bool flagAutoStart)
{
	void* handle = __createHandle();
	if (handle) {
		Ref<AsyncIoLoop> ret = new AsyncIoLoop;
		if (ret.isNotNull()) {
			ret->m_handle = handle;
			ret->m_thread = Thread::create(SLIB_CALLBACK_CLASS(AsyncIoLoop, __runLoop, ret.ptr));
			if (ret->m_thread.isNotNull()) {
				ret->m_flagInit = sl_true;
				if (flagAutoStart) {
					ret->start();
				}
				return ret;
			}
		}
		__closeHandle(handle);
	}
	return Ref<AsyncIoLoop>::null();
}

void AsyncIoLoop::release()
{
	ObjectLocker lock(this);
	if (!m_flagInit) {
		return;
	}
	m_flagInit = sl_false;

	if (m_flagRunning) {
		m_flagRunning = sl_false;
		m_thread->finish();
		__wake();
		lock.unlock();
		m_thread->finishAndWait();
	}
	
	__closeHandle(m_handle);
	
	m_queueInstancesOrder.removeAll();
	m_queueInstancesClosing.removeAll();
	m_queueInstancesClosed.removeAll();
	
}

void AsyncIoLoop::start()
{
	ObjectLocker lock(this);
	if (!m_flagInit) {
		return;
	}
	if (m_flagRunning) {
		return;
	}
	m_flagRunning = sl_true;
	if (!(m_thread->start())) {
		m_flagRunning = sl_false;
	}
}

sl_bool AsyncIoLoop::isRunning()
{
	return m_flagRunning;
}

sl_bool AsyncIoLoop::addTask(const Ref<Runnable>& task)
{
	if (task.isNull()) {
		return sl_false;
	}
	if (m_queueTasks.push(task)) {
		wake();
		return sl_true;
	}
	return sl_false;
}

void AsyncIoLoop::wake()
{
	ObjectLocker lock(this);
	if (!m_flagRunning) {
		return;
	}
	__wake();
}

sl_bool AsyncIoLoop::attachInstance(AsyncIoInstance* instance, AsyncIoMode mode)
{
	if (m_handle) {
		if (instance && instance->isOpened()) {
			ObjectLocker lock(this);
			return __attachInstance(instance, mode);
		}
	}
	return sl_false;
}

void AsyncIoLoop::closeInstance(AsyncIoInstance* instance)
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

void AsyncIoLoop::requestOrder(AsyncIoInstance* instance)
{
	if (m_handle) {
		if (instance && instance->isOpened()) {
			instance->addToQueue(m_queueInstancesOrder);
			wake();
		}
	}
}

void AsyncIoLoop::_stepBegin()
{
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
	
	// Request Orders
	{
		Queue< Ref<AsyncIoInstance> > instances;
		instances.merge(&m_queueInstancesOrder);
		Ref<AsyncIoInstance> instance;
		while (instances.pop(&instance)) {
			if (instance.isNotNull() && instance->isOpened()) {
				instance->processOrder();
			}
		}
	}
}

void AsyncIoLoop::_stepEnd()
{
	Ref<AsyncIoInstance> instance;
	while (m_queueInstancesClosing.pop(&instance)) {
		if (instance.isNotNull() && instance->isOpened()) {
			__detachInstance(instance.ptr);
			instance->close();
			m_queueInstancesClosed.push(instance);
		}
	}
}

/*************************************
	AsyncIoInstance
**************************************/

SLIB_DEFINE_OBJECT(AsyncIoInstance, Object)

AsyncIoInstance::AsyncIoInstance()
{
	m_handle = 0;
	m_flagClosing = sl_false;
	m_flagOrdering = sl_false;
	m_mode = AsyncIoMode::InOut;
}

Ref<AsyncIoObject> AsyncIoInstance::getObject()
{
	return m_object;
}

void AsyncIoInstance::setObject(AsyncIoObject* object)
{
	m_object = object;
}

Ref<AsyncIoLoop> AsyncIoInstance::getLoop()
{
	Ref<AsyncIoObject> object(m_object);
	if (object.isNotNull()) {
		return object->getIoLoop();
	}
	return Ref<AsyncIoLoop>::null();
}

sl_file AsyncIoInstance::getHandle()
{
	return m_handle;
}

void AsyncIoInstance::setHandle(sl_file handle)
{
	m_handle = handle;
}

sl_bool AsyncIoInstance::isOpened()
{
	return m_handle != SLIB_FILE_INVALID_HANDLE;
}

AsyncIoMode AsyncIoInstance::getMode()
{
	return m_mode;
}

void AsyncIoInstance::setMode(AsyncIoMode mode)
{
	m_mode = mode;
}

sl_bool AsyncIoInstance::isClosing()
{
	return m_flagClosing;
}

void AsyncIoInstance::setClosing()
{
	m_flagClosing = sl_true;
}

void AsyncIoInstance::addToQueue(Queue< Ref<AsyncIoInstance> >& queue)
{
	MutexLocker lock(&m_lockOrdering);
	if (!m_flagOrdering) {
		m_flagOrdering = sl_true;
		queue.push(this);
	}
}

void AsyncIoInstance::requestOrder()
{
	Ref<AsyncIoLoop> loop(getLoop());
	if (loop.isNotNull()) {
		loop->requestOrder(this);
	}
}

void AsyncIoInstance::processOrder()
{
	MutexLocker lock(&m_lockOrdering);
	m_flagOrdering = sl_false;
	lock.unlock();
	onOrder();
}

/*************************************
	AsyncIoObject
**************************************/

SLIB_DEFINE_OBJECT(AsyncIoObject, Object)

AsyncIoObject::AsyncIoObject()
{
}

AsyncIoObject::~AsyncIoObject()
{
	closeIoInstance();
}

Ref<AsyncIoLoop> AsyncIoObject::getIoLoop()
{
	Ref<AsyncIoLoop> loop = m_ioLoop;
	if (loop.isNull()) {
		loop = AsyncIoLoop::getDefault();
	}
	return loop;
}

Ref<AsyncIoInstance> AsyncIoObject::getIoInstance()
{
	return m_ioInstance;
}

void AsyncIoObject::closeIoInstance()
{
	ObjectLocker lock(this);
	Ref<AsyncIoInstance> instance(m_ioInstance);
	if (instance.isNotNull()) {
		Ref<AsyncIoLoop> loop = getIoLoop();
		if (loop.isNotNull()) {
			loop->closeInstance(instance.ptr);
		}
		m_ioInstance.setNull();
	}
}

Ref<Referable> AsyncIoObject::getUserObject(const String& key)
{
	return m_mapUserObjects_s.getValue(key, Ref<Referable>::null());
}

void AsyncIoObject::setUserObject(const String& key, const Ref<Referable>& object)
{
	m_mapUserObjects_s.put(key, object);
}

Ref<Referable> AsyncIoObject::getUserObject(sl_uint64 key)
{
	return m_mapUserObjects_i.getValue(key, Ref<Referable>::null());
}

void AsyncIoObject::setUserObject(sl_uint64 key, const Ref<Referable>& object)
{
	m_mapUserObjects_i.put(key, object);
}

Variant AsyncIoObject::getUserData(const String& key)
{
	return m_mapUserData_s.getValue(key, Variant::null());
}

void AsyncIoObject::setUserData(const String& key, const Variant& data)
{
	m_mapUserData_s.put(key, data);
}

Variant AsyncIoObject::getUserData(sl_uint64 key)
{
	return m_mapUserData_i.getValue(key, Variant::null());
}

void AsyncIoObject::setUserData(sl_uint64 key, const Variant& object)
{
	m_mapUserData_i.put(key, object);
}

void AsyncIoObject::setIoLoop(const Ref<AsyncIoLoop>& loop)
{
	m_ioLoop = loop;
}

void AsyncIoObject::setIoInstance(AsyncIoInstance* instance)
{
	m_ioInstance = instance;
}

/*************************************
	AsyncStreamInstance
**************************************/

SLIB_DEFINE_ROOT_OBJECT(AsyncStreamRequest)

AsyncStreamRequest::AsyncStreamRequest(
	void* _data,
	sl_uint32 _size,
	const Referable* _refData,
	const Ptr<IAsyncStreamListener>& _listener,
	sl_bool _flagRead)
: data(_data), size(_size), refData(_refData), listener(_listener), flagRead(_flagRead)
{
}

Ref<AsyncStreamRequest> AsyncStreamRequest::createRead(
	void* data,
	sl_uint32 size,
	const Referable* refData,
	const Ptr<IAsyncStreamListener>& listener)
{
	return new AsyncStreamRequest(data, size, refData, listener, sl_true);
}

Ref<AsyncStreamRequest> AsyncStreamRequest::createWrite(
	void* data,
	sl_uint32 size,
	const Referable* refData,
	const Ptr<IAsyncStreamListener>& listener)
{
	return new AsyncStreamRequest(data, size, refData, listener, sl_false);
}

void IAsyncStreamListener::onRead(AsyncStream* stream, void* data, sl_uint32 sizeRead, const Referable* ref, sl_bool flagError)
{
}

void IAsyncStreamListener::onWrite(AsyncStream* stream, void* data, sl_uint32 sizeWritten, const Referable* ref, sl_bool flagError)
{
}


SLIB_DEFINE_OBJECT(AsyncStreamInstance, AsyncIoInstance)

AsyncStreamInstance::AsyncStreamInstance()
{
	m_sizeWriteWaiting = 0;
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

sl_size AsyncStreamInstance::getWaitingSizeForWrite()
{
	return m_sizeWriteWaiting;
}

sl_bool AsyncStreamInstance::addReadRequest(const Ref<AsyncStreamRequest>& request)
{
	return m_requestsRead.push(request) != sl_null;
}

sl_bool AsyncStreamInstance::popReadRequest(Ref<AsyncStreamRequest>& request)
{
	return m_requestsRead.pop(&request);
}

sl_size AsyncStreamInstance::getReadRequestsCount()
{
	return m_requestsRead.getCount();
}

sl_bool AsyncStreamInstance::addWriteRequest(const Ref<AsyncStreamRequest>& request)
{
	if (m_requestsWrite.push(request)) {
		Base::interlockedAdd(&m_sizeWriteWaiting, request->size);
		return sl_true;
	}
	return sl_false;
}

sl_bool AsyncStreamInstance::popWriteRequest(Ref<AsyncStreamRequest>& request)
{
	if (m_requestsWrite.pop(&request)) {
		Base::interlockedAdd(&m_sizeWriteWaiting, -((sl_reg)(request->size)));
		return sl_true;
	}
	return sl_false;
}

sl_size AsyncStreamInstance::getWriteRequestsCount()
{
	return m_requestsWrite.getCount();
}

/*************************************
	AsyncStream
**************************************/

SLIB_DEFINE_OBJECT(AsyncStream, Object)

Ref<AsyncStream> AsyncStream::create(AsyncStreamInstance* instance, AsyncIoMode mode, const Ref<AsyncIoLoop>& loop)
{
	Ref<AsyncStreamBase> ret;
	if (instance) {
		ret = new AsyncStreamBase;
		if (ret.isNotNull()) {
			if (!(ret->_initialize(instance, mode, loop))) {
				ret.setNull();
			}
		}
	}
	return ret;
}

Ref<AsyncStream> AsyncStream::create(AsyncStreamInstance* instance, AsyncIoMode mode)
{
	if (instance) {
		return create(instance, mode, AsyncIoLoop::getDefault());
	}
	return Ref<AsyncStream>::null();
}

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

sl_bool AsyncStream::readToMemory(const Memory& mem, const Ptr<IAsyncStreamListener>& listener)
{
	sl_size size = mem.getSize();
	if (size > 0x40000000) {
		size = 0x40000000;
	}
	return read(mem.getData(), (sl_uint32)(size), listener, mem.ref.ptr);
}

sl_bool AsyncStream::writeFromMemory(const Memory& mem, const Ptr<IAsyncStreamListener>& listener)
{
	sl_size size = mem.getSize();
	if (size > 0x40000000) {
		size = 0x40000000;
	}
	return write(mem.getData(), (sl_uint32)(size), listener, mem.ref.ptr);
}

/*************************************
	AsyncStreamBase
**************************************/
Ref<AsyncStreamInstance> AsyncStreamBase::getIoInstance()
{
	return Ref<AsyncStreamInstance>::from(AsyncIoObject::getIoInstance());
}

sl_bool AsyncStreamBase::_initialize(AsyncStreamInstance* instance, AsyncIoMode mode, const Ref<AsyncIoLoop>& _loop)
{
	if (!instance) {
		return sl_false;
	}
	Ref<AsyncIoLoop> loop = _loop;
	if (loop.isNull()) {
		loop = AsyncIoLoop::getDefault();
		if (loop.isNull()) {
			return sl_false;
		}
	}
	instance->setObject(this);
	setIoInstance(instance);
	setIoLoop(loop);
	if (loop->attachInstance(instance, mode)) {
		return sl_true;
	}
	return sl_false;
}

void AsyncStreamBase::close()
{
	closeIoInstance();
}

sl_bool AsyncStreamBase::isOpened()
{
	return getIoInstance().isNotNull();
}

sl_bool AsyncStreamBase::read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, const Referable* refData)
{
	Ref<AsyncIoLoop> loop = getIoLoop();
	if (loop.isNull()) {
		return sl_false;
	}
	Ref<AsyncStreamInstance> instance = getIoInstance();
	if (instance.isNotNull()) {
		if (instance->read(data, size, listener, refData)) {
			loop->requestOrder(instance.ptr);
			return sl_true;
		}
	}
	return sl_false;
}

sl_bool AsyncStreamBase::write(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, const Referable* refData)
{
	Ref<AsyncIoLoop> loop = getIoLoop();
	if (loop.isNull()) {
		return sl_false;
	}
	Ref<AsyncStreamInstance> instance = getIoInstance();
	if (instance.isNotNull()) {
		if (instance->write(data, size, listener, refData)) {
			loop->requestOrder(instance.ptr);
			return sl_true;
		}
	}
	return sl_false;
}

sl_bool AsyncStreamBase::isSeekable()
{
	Ref<AsyncStreamInstance> instance = getIoInstance();
	if (instance.isNotNull()) {
		return instance->isSeekable();
	}
	return sl_false;
}

sl_bool AsyncStreamBase::seek(sl_uint64 pos)
{
	Ref<AsyncStreamInstance> instance = getIoInstance();
	if (instance.isNotNull()) {
		instance->seek(pos);
		return sl_true;
	}
	return sl_false;
}

sl_uint64 AsyncStreamBase::getSize()
{
	Ref<AsyncStreamInstance> instance = getIoInstance();
	if (instance.isNotNull()) {
		return instance->getSize();
	}
	return sl_false;
}

sl_bool AsyncStreamBase::addTask(const Ref<Runnable>& callback)
{
	Ref<AsyncIoLoop> loop = getIoLoop();
	if (loop.isNotNull()) {
		return loop->addTask(callback);
	}
	return sl_false;
}

sl_size AsyncStreamBase::getWaitingSizeForWrite()
{
	Ref<AsyncStreamInstance> instance = getIoInstance();
	if (instance.isNotNull()) {
		return instance->getWaitingSizeForWrite();
	}
	return 0;
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
				return _addRequest(req.ptr);
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
				return _addRequest(req.ptr);
			}
		}
	}
	return sl_false;
}

sl_bool AsyncStreamBaseIO::addTask(const Ref<Runnable>& callback)
{
	Ref<AsyncLoop> loop = getLoop();
	if (loop.isNotNull()) {
		return loop->addTask(callback);
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
				processRequest(req.ptr);
			}
		}
	}
}

Ref<AsyncLoop> AsyncStreamBaseIO::getLoop()
{
	Ref<AsyncLoop> loop = m_loop;
	if (loop.isNull()) {
		loop = AsyncLoop::getDefault();
	}
	return loop;
}

void AsyncStreamBaseIO::setLoop(const Ref<AsyncLoop>& loop)
{
	m_loop = loop;
}


/*************************************
	AsyncReader
**************************************/

AsyncReader::AsyncReader()
{
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
		sl_reg size = reader->read(request->data, request->size);
		if (size <= 0) {
			flagError = sl_true;
		}
		PtrLocker<IAsyncStreamListener> listener(request->listener);
		if (listener.isNotNull()) {
			listener->onRead(this, request->data, (sl_uint32)size, request->refData.ptr, flagError);
		}
	}
}


/*************************************
	AsyncWriter
**************************************/

AsyncWriter::AsyncWriter()
{
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
		sl_reg size = reader->write(request->data, request->size);
		if (size <= 0) {
			flagError = sl_true;
		}
		PtrLocker<IAsyncStreamListener> listener(request->listener);
		if (listener.isNotNull()) {
			listener->onWrite(this, request->data, (sl_uint32)size, request->refData.ptr, flagError);
		}
	}
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
	return AsyncFile::open(path, FileMode::Read, loop);
}

Ref<AsyncFile> AsyncFile::openForRead(const String& path)
{
	return AsyncFile::openForRead(path, AsyncLoop::getDefault());
}

Ref<AsyncFile> AsyncFile::openForWrite(const String& path, const Ref<AsyncLoop>& loop)
{
	return AsyncFile::open(path, FileMode::Write, loop);
}

Ref<AsyncFile> AsyncFile::openForWrite(const String& path)
{
	return AsyncFile::openForWrite(path, AsyncLoop::getDefault());
}

Ref<AsyncFile> AsyncFile::openForAppend(const String& path, const Ref<AsyncLoop>& loop)
{
	return AsyncFile::open(path, FileMode::Append, loop);
}

Ref<AsyncFile> AsyncFile::openForAppend(const String& path)
{
	return AsyncFile::openForAppend(path, AsyncLoop::getDefault());
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
		if (request->flagRead) {
			size = file->read(request->data, request->size);
		} else {
			size = file->write(request->data, request->size);
		}
		if (size <= 0) {
			flagError = sl_true;
			size = 0;
		}
		PtrLocker<IAsyncStreamListener> listener(request->listener);
		if (listener.isNotNull()) {
			if (request->flagRead) {
				listener->onRead(this, request->data, (sl_uint32)size, request->refData.ptr, flagError);
			} else {
				listener->onWrite(this, request->data, (sl_uint32)size, request->refData.ptr, flagError);
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
		return file->seek(pos, SeekPosition::Begin);
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

sl_bool AsyncCopy::isRunning()
{
	return m_flagRunning;
}

Ref<AsyncStream> AsyncCopy::getSource()
{
	return m_source;
}

Ref<AsyncStream> AsyncCopy::getTarget()
{
	return m_target;
}

sl_uint64 AsyncCopy::getTotalSize()
{
	return m_sizeTotal;
}

sl_uint64 AsyncCopy::getReadSize()
{
	return m_sizeRead;
}

sl_uint64 AsyncCopy::getWrittenSize()
{
	return m_sizeWritten;
}

sl_bool AsyncCopy::isCompleted()
{
	return m_sizeWritten == m_sizeTotal;
}

sl_bool AsyncCopy::isErrorOccured()
{
	return m_flagReadError || m_flagWriteError;
}

sl_bool AsyncCopy::isReadingErrorOccured()
{
	return m_flagReadError;
}

sl_bool AsyncCopy::isWritingErrorOccured()
{
	return m_flagWriteError;
}

sl_bool AsyncCopy::isReading()
{
	return m_bufferReading.isNotNull();
}

sl_bool AsyncCopy::isWriting()
{
	return m_bufferWriting.isNotNull();
}

const Ptr<IAsyncCopyListener>& AsyncCopy::getListener()
{
	return m_listener;
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
	m_header.add(header);
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
	return m_header.add(header);
}

void AsyncOutputBufferElement::setBody(AsyncStream* stream, sl_uint64 size)
{
	m_body = stream;
	m_sizeBody = size;
}

MemoryQueue& AsyncOutputBufferElement::getHeader()
{
	return m_header;
}

Ref<AsyncStream> AsyncOutputBufferElement::getBody()
{
	return m_body;
}

sl_uint64 AsyncOutputBufferElement::getBodySize()
{
	return m_sizeBody;
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

sl_bool AsyncOutputBuffer::copyFromFile(const String& path, const Ref<AsyncLoop>& loop)
{
	if (!(File::exists(path))) {
		return sl_false;
	}
	sl_uint64 size = File::getSize(path);
	if (size > 0) {
		Ref<AsyncFile> file = AsyncFile::openForRead(path, loop);
		if (file.isNotNull()) {
			return copyFrom(file.ptr, size);
		} else {
			return sl_false;
		}
	}
	return sl_true;
}

sl_bool AsyncOutputBuffer::copyFromFile(const String& path)
{
	return copyFromFile(path, AsyncLoop::getDefault());
}

sl_uint64 AsyncOutputBuffer::getOutputLength() const
{
	return m_lengthOutput;
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

Ref<AsyncOutput> AsyncOutput::create(const Ref<AsyncStream>& streamOutput, sl_uint32 sizeBuffer)
{
	return create(streamOutput, Ptr<IAsyncOutputListener>::null(), sizeBuffer);
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
		sl_uint32 size = (sl_uint32)(header.pop(m_bufWrite.getData(), m_bufWrite.getSize()));
		if (size > 0) {
			m_flagWriting = sl_true;
			if (!(m_streamOutput->write(m_bufWrite.getData(), size, (WeakRef<AsyncOutput>)this, m_bufWrite.ref.ptr))) {
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

Ref<AsyncStream> AsyncStreamFilter::getSourceStream() const
{
	return m_stream;
}

void AsyncStreamFilter::setSourceStream(const Ref<AsyncStream>& stream)
{
	m_stream = stream;
}

void AsyncStreamFilter::close()
{
	MutexLocker lock(&m_lockReading, &m_lockWriting);
	if (m_flagOpened) {
		m_flagOpened = sl_false;
	}
	setReadingEnded();
	setWritingEnded();
	m_stream.setNull();
}

sl_bool AsyncStreamFilter::isOpened()
{
	return m_flagOpened;
}

sl_bool AsyncStreamFilter::read(void* data, sl_uint32 size, const Ptr<IAsyncStreamListener>& listener, const Referable* ref)
{
	Ref<AsyncStreamRequest> request = AsyncStreamRequest::createRead(data, size, ref, listener);
	if (request.isNotNull()) {
		MutexLocker lock(&m_lockReading);
		if (!m_flagOpened) {
			return sl_false;
		}
		if (m_requestsRead.push(request)) {
			if (m_bufReadConverted.getSize() > 0) {
				if (!(addTask(SLIB_CALLBACK_WEAKREF(AsyncStreamFilter, _processRead, this)))) {
					return sl_false;
				}
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
		addReadData(data.getData(), (sl_uint32)(size), data.ref.ptr);
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
	Ref<AsyncStream> stream = m_stream;
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
				sl_size _m = m_bufReadConverted.pop(req->data, req->size);
				sl_uint32 m = (sl_uint32)(_m);
				if (m_bufReadConverted.getSize() == 0) {
					PtrLocker<IAsyncStreamListener> listener(req->listener);
					if (listener.isNotNull()) {
						listener->onRead(this, req->data, m, req->refData.ptr, m_flagReadingError);
					}
					break;
				} else {
					PtrLocker<IAsyncStreamListener> listener(req->listener);
					if (listener.isNotNull()) {
						listener->onRead(this, req->data, m, req->refData.ptr, sl_false);
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
	Ref<AsyncStream> stream = m_stream;
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
				return stream->write(memConv.getData(), (sl_uint32)(memConv.getSize()), WeakRef<AsyncStreamFilter>(this), req.ptr);
			}
		} else {
			if (listener.isNull()) {
				return sl_true;
			}
			if (addTask(SLIB_CALLBACK_WEAKREF(AsyncStreamFilter, _processZeroWrite, this, data, size, listener, Ref<Referable>(ref)))) {
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
			listener->onWrite(this, req->data, req->size, req->refData.ptr, m_flagWritingError);
		}
	}
}

void AsyncStreamFilter::_processZeroWrite(void* data, sl_uint32 size, Ptr<IAsyncStreamListener> _listener, Ref<Referable> ref)
{
	PtrLocker<IAsyncStreamListener> listener(_listener);
	if (listener.isNotNull()) {
		listener->onWrite(this, data, size, ref.ptr, m_flagWritingError);
	}
}

sl_bool AsyncStreamFilter::addTask(const Ref<Runnable>& callback)
{
	Ref<AsyncStream> stream = m_stream;
	if (stream.isNotNull()) {
		return stream->addTask(callback);
	}
	return sl_false;
}

Memory AsyncStreamFilter::filterRead(void* data, sl_uint32 size, const Referable* refData)
{
	return Memory::createStatic(data, size, refData);
}

Memory AsyncStreamFilter::filterWrite(void* data, sl_uint32 size, const Referable* refData)
{
	return Memory::createStatic(data, size, refData);
}

sl_bool AsyncStreamFilter::isReadingError()
{
	return m_flagReadingError;
}

void AsyncStreamFilter::setReadingError()
{
	m_flagReadingError = sl_true;
}

sl_bool AsyncStreamFilter::isReadingEnded()
{
	return m_flagReadingEnded;
}

void AsyncStreamFilter::setReadingEnded()
{
	m_flagReadingEnded = sl_true;
}

sl_bool AsyncStreamFilter::isWritingError()
{
	return m_flagWritingError;
}

void AsyncStreamFilter::setWritingError()
{
	m_flagWritingError = sl_true;
}

sl_bool AsyncStreamFilter::isWritingEnded()
{
	return m_flagWritingEnded;
}

void AsyncStreamFilter::setWritingEnded()
{
	m_flagWritingEnded = sl_true;
}

void AsyncStreamFilter::_closeAllReadRequests()
{
	Ref<AsyncStreamRequest> req;
	while (m_requestsRead.pop(&req)) {
		if (req.isNotNull()) {
			PtrLocker<IAsyncStreamListener> listener(req->listener);
			if (listener.isNotNull()) {
				listener->onRead(this, req->data, 0, req->refData.ptr, sl_true);
			}
		}
	}
}

SLIB_NAMESPACE_END
