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

#include "slib/core/async.h"

#include "slib/core/safe_static.h"

namespace slib
{

/*************************************
			AsyncIoLoop
*************************************/

	SLIB_DEFINE_OBJECT(AsyncIoLoop, Dispatcher)

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
			return sl_null;
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
		void* handle = _native_createHandle();
		if (handle) {
			Ref<AsyncIoLoop> ret = new AsyncIoLoop;
			if (ret.isNotNull()) {
				ret->m_handle = handle;
				ret->m_thread = Thread::create(SLIB_FUNCTION_MEMBER(AsyncIoLoop, _native_runLoop, ret.get()));
				if (ret->m_thread.isNotNull()) {
					ret->m_flagInit = sl_true;
					if (flagAutoStart) {
						ret->start();
					}
					return ret;
				}
			}
			_native_closeHandle(handle);
		}
		return sl_null;
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
			_native_wake();
			lock.unlock();
			m_thread->finishAndWait();
		}
		
		_native_closeHandle(m_handle);
		
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

	sl_bool AsyncIoLoop::addTask(const Function<void()>& task)
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

	sl_bool AsyncIoLoop::dispatch(const Function<void()>& callback, sl_uint64 delay_ms)
	{
		return addTask(callback);
	}

	void AsyncIoLoop::wake()
	{
		ObjectLocker lock(this);
		if (!m_flagRunning) {
			return;
		}
		_native_wake();
	}

	sl_bool AsyncIoLoop::attachInstance(AsyncIoInstance* instance, AsyncIoMode mode)
	{
		if (m_handle) {
			if (instance && instance->isOpened()) {
				ObjectLocker lock(this);
				return _native_attachInstance(instance, mode);
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
			LinkedQueue< Function<void()> > tasks;
			tasks.merge(&m_queueTasks);
			Function<void()> task;
			while (tasks.pop(&task)) {
				task();
			}
		}
		
		// Request Orders
		{
			LinkedQueue< Ref<AsyncIoInstance> > instances;
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
				_native_detachInstance(instance.get());
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

	AsyncIoInstance::~AsyncIoInstance()
	{
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
		return sl_null;
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

	void AsyncIoInstance::addToQueue(LinkedQueue< Ref<AsyncIoInstance> >& queue)
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
				loop->closeInstance(instance.get());
			}
			m_ioInstance.setNull();
		}
	}

	Variant AsyncIoObject::getUserData()
	{
		return m_userData;
	}

	void AsyncIoObject::setUserData(const Variant& data)
	{
		m_userData = data;
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
		const void* _data,
		sl_uint32 _size,
		Referable* _userObject,
		const Function<void(AsyncStreamResult&)>& _callback,
		sl_bool _flagRead)
	 : data((void*)_data), size(_size), userObject(_userObject), callback(_callback), flagRead(_flagRead)
	{
	}
	
	AsyncStreamRequest::~AsyncStreamRequest()
	{
	}

	Ref<AsyncStreamRequest> AsyncStreamRequest::createRead(
		void* data,
		sl_uint32 size,
		Referable* userObject,
		const Function<void(AsyncStreamResult&)>& callback)
	{
		if (!size) {
			return sl_null;
		}
		return new AsyncStreamRequest(data, size, userObject, callback, sl_true);
	}

	Ref<AsyncStreamRequest> AsyncStreamRequest::createWrite(
		const void* data,
		sl_uint32 size,
		Referable* userObject,
		const Function<void(AsyncStreamResult&)>& callback)
	{
		if (!size) {
			return sl_null;
		}
		return new AsyncStreamRequest(data, size, userObject, callback, sl_false);
	}

	void AsyncStreamRequest::runCallback(AsyncStream* stream, sl_uint32 resultSize, sl_bool flagError)
	{
		if (callback.isNotNull()) {
			AsyncStreamResult result;
			result.stream = stream;
			result.data = data;
			result.size = resultSize;
			result.requestSize = size;
			result.userObject = userObject.get();
			result.flagError = flagError;
			callback(result);
		}
	}

	SLIB_DEFINE_OBJECT(AsyncStreamInstance, AsyncIoInstance)

	AsyncStreamInstance::AsyncStreamInstance()
	{
		m_sizeWriteWaiting = 0;
	}

	AsyncStreamInstance::~AsyncStreamInstance()
	{
	}

	sl_bool AsyncStreamInstance::read(void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& callback, Referable* userObject)
	{
		Ref<AsyncStreamRequest> req = AsyncStreamRequest::createRead(data, size, userObject, callback);
		if (req.isNotNull()) {
			m_requestsRead.push(req);
			return sl_true;
		}
		return sl_false;
	}

	sl_bool AsyncStreamInstance::write(const void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& callback, Referable* userObject)
	{
		Ref<AsyncStreamRequest> req = AsyncStreamRequest::createWrite(data, size, userObject, callback);
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
		return m_requestsRead.push(request);
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

	AsyncStream::AsyncStream()
	{
	}

	AsyncStream::~AsyncStream()
	{
	}

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
		return sl_null;
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

	sl_bool AsyncStream::readToMemory(const Memory& mem, const Function<void(AsyncStreamResult&)>& callback)
	{
		sl_size size = mem.getSize();
		if (size > 0x40000000) {
			size = 0x40000000;
		}
		return read(mem.getData(), (sl_uint32)(size), callback, mem.ref.get());
	}

	sl_bool AsyncStream::writeFromMemory(const Memory& mem, const Function<void(AsyncStreamResult&)>& callback)
	{
		sl_size size = mem.getSize();
		if (size > 0x40000000) {
			size = 0x40000000;
		}
		return write(mem.getData(), (sl_uint32)(size), callback, mem.ref.get());
	}

/*************************************
		AsyncStreamBase
**************************************/

	SLIB_DEFINE_OBJECT(AsyncStreamBase, AsyncStream)

	AsyncStreamBase::AsyncStreamBase()
	{
	}

	AsyncStreamBase::~AsyncStreamBase()
	{
	}

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

	sl_bool AsyncStreamBase::read(void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& callback, Referable* userObject)
	{
		Ref<AsyncIoLoop> loop = getIoLoop();
		if (loop.isNull()) {
			return sl_false;
		}
		Ref<AsyncStreamInstance> instance = getIoInstance();
		if (instance.isNotNull()) {
			if (instance->read(data, size, callback, userObject)) {
				loop->requestOrder(instance.get());
				return sl_true;
			}
		}
		return sl_false;
	}

	sl_bool AsyncStreamBase::write(const void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& callback, Referable* userObject)
	{
		Ref<AsyncIoLoop> loop = getIoLoop();
		if (loop.isNull()) {
			return sl_false;
		}
		Ref<AsyncStreamInstance> instance = getIoInstance();
		if (instance.isNotNull()) {
			if (instance->write(data, size, callback, userObject)) {
				loop->requestOrder(instance.get());
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

	sl_bool AsyncStreamBase::addTask(const Function<void()>& callback)
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
		AsyncStreamSimulator
**************************************/

	SLIB_DEFINE_OBJECT(AsyncStreamSimulator, AsyncStream)

	AsyncStreamSimulator::AsyncStreamSimulator()
	{
		m_flagProcessRequest = sl_false;
	}

	AsyncStreamSimulator::~AsyncStreamSimulator()
	{
	}

	sl_bool AsyncStreamSimulator::read(void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& callback, Referable* userObject)
	{
		if (isOpened()) {
			Ref<AsyncStreamRequest> req = AsyncStreamRequest::createRead(data, size, userObject, callback);
			if (req.isNotNull()) {
				return _addRequest(req.get());
			}
		}
		return sl_false;
	}

	sl_bool AsyncStreamSimulator::write(const void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& callback, Referable* userObject)
	{
		if (isOpened()) {
			Ref<AsyncStreamRequest> req = AsyncStreamRequest::createWrite(data, size, userObject, callback);
			if (req.isNotNull()) {
				return _addRequest(req.get());
			}
		}
		return sl_false;
	}

	sl_bool AsyncStreamSimulator::addTask(const Function<void()>& callback)
	{
		Ref<Dispatcher> dispatcher(m_dispatcher);
		if (dispatcher.isNotNull()) {
			return dispatcher->dispatch(callback);
		}
		return sl_false;
	}

	void AsyncStreamSimulator::initialize()
	{
		m_dispatchLoop = DispatchLoop::create();
		m_dispatcher = m_dispatchLoop;
	}

	void AsyncStreamSimulator::initialize(const Ref<Dispatcher>& dispatcher)
	{
		if (dispatcher.isNotNull()) {
			m_dispatcher = dispatcher;
		} else {
			initialize();
		}
	}

	sl_bool AsyncStreamSimulator::_addRequest(AsyncStreamRequest* req)
	{
		Ref<Dispatcher> dispatcher(m_dispatcher);
		if (dispatcher.isNotNull()) {
			ObjectLocker lock(this);
			m_requests.push_NoLock(req);
			if (!m_flagProcessRequest) {
				m_flagProcessRequest = sl_true;
				lock.unlock();
				dispatcher->dispatch(SLIB_FUNCTION_WEAKREF(AsyncStreamSimulator, _runProcessor, this));
			}
			return sl_true;
		}
		return sl_false;
	}

	void AsyncStreamSimulator::_runProcessor()
	{
		if (isOpened()) {
			Ref<Thread> thread = Thread::getCurrent();
			while (thread.isNull() || thread->isNotStopping()) {
				Ref<AsyncStreamRequest> req;
				{
					ObjectLocker lock(this);
					if (!(m_requests.pop_NoLock(&req))) {
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

	SLIB_DEFINE_OBJECT(AsyncReader, AsyncStreamSimulator)
	
	AsyncReader::AsyncReader()
	{
	}

	AsyncReader::~AsyncReader()
	{
	}

	Ref<AsyncReader> AsyncReader::create(const Ptr<IReader>& reader)
	{
		if (reader.isNotNull()) {
			Ref<AsyncReader> ret = new AsyncReader;
			if (ret.isNotNull()) {
				ret->m_reader = reader;
				ret->initialize();
				return ret;
			}
		}
		return sl_null;
	}

	Ref<AsyncReader> AsyncReader::create(const Ptr<IReader>& reader, const Ref<Dispatcher>& dispatcher)
	{
		if (reader.isNotNull()) {
			Ref<AsyncReader> ret = new AsyncReader;
			if (ret.isNotNull()) {
				ret->m_reader = reader;
				ret->initialize(dispatcher);
				return ret;
			}
		}
		return sl_null;
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

	sl_bool AsyncReader::write(const void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& callback, Referable* userObject)
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
			if (request->data && request->size) {
				sl_bool flagError = sl_false;
				sl_reg size = reader->read(request->data, request->size);
				if (size <= 0) {
					flagError = sl_true;
				}
				request->runCallback(this, (sl_uint32)size, flagError);
			} else {
				request->runCallback(this, request->size, sl_false);
			}
		}
	}


/*************************************
		AsyncWriter
**************************************/

	SLIB_DEFINE_OBJECT(AsyncWriter, AsyncStreamSimulator)
	
	AsyncWriter::AsyncWriter()
	{
	}

	AsyncWriter::~AsyncWriter()
	{
	}

	Ref<AsyncWriter> AsyncWriter::create(const Ptr<IWriter>& writer)
	{
		if (writer.isNotNull()) {
			Ref<AsyncWriter> ret = new AsyncWriter;
			if (ret.isNotNull()) {
				ret->m_writer = writer;
				ret->initialize();
				return ret;
			}
		}
		return sl_null;
	}

	Ref<AsyncWriter> AsyncWriter::create(const Ptr<IWriter>& writer, const Ref<Dispatcher>& dispatcher)
	{
		if (writer.isNotNull()) {
			Ref<AsyncWriter> ret = new AsyncWriter;
			if (ret.isNotNull()) {
				ret->m_writer = writer;
				ret->initialize(dispatcher);
				return ret;
			}
		}
		return sl_null;
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

	sl_bool AsyncWriter::read(void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& listener, Referable* userObject)
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
			if (request->data && request->size) {
				sl_bool flagError = sl_false;
				sl_reg size = reader->write(request->data, request->size);
				if (size <= 0) {
					flagError = sl_true;
				}
				request->runCallback(this, (sl_uint32)size, flagError);
			} else {
				request->runCallback(this, request->size, sl_false);
			}
		}
	}


/*************************************
		AsyncFile
**************************************/

	SLIB_DEFINE_OBJECT(AsyncFile, AsyncStreamSimulator)

	AsyncFile::AsyncFile()
	{
	}

	AsyncFile::~AsyncFile()
	{
		close();
	}

	Ref<AsyncFile> AsyncFile::create(const Ref<File>& file)
	{
		if (file.isNotNull()) {
			Ref<AsyncFile> ret = new AsyncFile;
			if (ret.isNotNull()) {
				ret->m_file = file;
				ret->initialize();
				return ret;
			}
		}
		return sl_null;
	}

	Ref<AsyncFile> AsyncFile::create(const Ref<File>& file, const Ref<Dispatcher>& dispatcher)
	{
		if (file.isNotNull()) {
			Ref<AsyncFile> ret = new AsyncFile;
			if (ret.isNotNull()) {
				ret->m_file = file;
				ret->initialize(dispatcher);
				return ret;
			}
		}
		return sl_null;
	}

	Ref<AsyncFile> AsyncFile::open(const StringParam& path, FileMode mode)
	{
		Ref<File> file = File::open(path, mode);
		if (file.isNotNull()) {
			return AsyncFile::create(file);
		}
		return sl_null;
	}

	Ref<AsyncFile> AsyncFile::open(const StringParam& path, FileMode mode, const Ref<Dispatcher>& dispatcher)
	{
		Ref<File> file = File::open(path, mode);
		if (file.isNotNull()) {
			return AsyncFile::create(file, dispatcher);
		}
		return sl_null;
	}

	Ref<AsyncFile> AsyncFile::openForRead(const StringParam& path)
	{
		return AsyncFile::open(path, FileMode::Read);
	}

	Ref<AsyncFile> AsyncFile::openForRead(const StringParam& path, const Ref<Dispatcher>& dispatcher)
	{
		return AsyncFile::open(path, FileMode::Read, dispatcher);
	}

	Ref<AsyncFile> AsyncFile::openForWrite(const StringParam& path)
	{
		return AsyncFile::open(path, FileMode::Write);
	}

	Ref<AsyncFile> AsyncFile::openForWrite(const StringParam& path, const Ref<Dispatcher>& dispatcher)
	{
		return AsyncFile::open(path, FileMode::Write, dispatcher);
	}

	Ref<AsyncFile> AsyncFile::openForAppend(const StringParam& path)
	{
		return AsyncFile::open(path, FileMode::Append);
	}

	Ref<AsyncFile> AsyncFile::openForAppend(const StringParam& path, const Ref<Dispatcher>& dispatcher)
	{
		return AsyncFile::open(path, FileMode::Append, dispatcher);
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
			if (request->data && request->size) {
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
				request->runCallback(this, (sl_uint32)size, flagError);
			} else {
				request->runCallback(this, request->size, sl_false);
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
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(AsyncCopyParam)

	AsyncCopyParam::AsyncCopyParam()
	{
		size = SLIB_UINT64_MAX;
		bufferSize = 0x10000;
		bufferCount = 8;
		flagAutoStart = sl_true;
	}

	SLIB_DEFINE_OBJECT(AsyncCopy, Object)

	AsyncCopy::AsyncCopy()
	{
		m_sizeRead = 0;
		m_sizeWritten = 0;
		m_flagReadError = sl_false;
		m_flagWriteError = sl_false;
		m_flagRunning = sl_true;
		m_flagStarted = sl_false;
		m_flagEnqueue = sl_false;

		m_sizeTotal = 0;
	}

	AsyncCopy::~AsyncCopy()
	{
		close();
	}

	Ref<AsyncCopy> AsyncCopy::create(const AsyncCopyParam& param)
	{
		if (param.target.isNull()) {
			return sl_null;
		}
		if (param.source.isNull()) {
			return sl_null;
		}
		if (param.size == 0) {
			return sl_null;
		}
		if (param.bufferSize == 0 || param.bufferCount == 0) {
			return sl_null;
		}
		Ref<AsyncCopy> ret = new AsyncCopy();
		if (ret.isNotNull()) {
			ret->m_source = param.source;
			ret->m_target = param.target;
			ret->m_onRead = param.onRead;
			ret->m_onWrite = param.onWrite;
			ret->m_onEnd = param.onEnd;
			ret->m_sizeTotal = param.size;
			for (sl_uint32 i = 0; i < param.bufferCount; i++) {
				Memory mem = Memory::create(param.bufferSize);
				if (mem.isNotNull()) {
					Ref<Buffer> buf = new Buffer;
					if (buf.isNotNull()) {
						buf->mem = mem;
						ret->m_buffersRead.push(buf);
					}
				} else {
					return sl_null;
				}
			}
			if (param.flagAutoStart) {
				if (ret->start()) {
					return ret;
				}
			} else {
				return ret;
			}
		}
		return sl_null;
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
			dispatchEnd();
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

	void AsyncCopy::onReadStream(AsyncStreamResult& result)
	{
		ObjectLocker lock(this);
		if (!m_flagRunning) {
			return;
		}

		if (result.flagError) {
			m_flagReadError = sl_true;
		}

		Ref<Buffer> bufferReading = m_bufferReading;
		m_bufferReading.setNull();

		if (bufferReading.isNotNull()) {
			m_sizeRead += result.size;
			Memory memWrite = bufferReading->mem.sub(0, result.size);
			if (memWrite.isNull()) {
				m_flagReadError = sl_true;
			} else {
				memWrite = dispatchRead(memWrite);
				if (memWrite.isNotNull()) {
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

	void AsyncCopy::onWriteStream(AsyncStreamResult& result)
	{
		ObjectLocker lock(this);
		if (!m_flagRunning) {
			return;
		}
		if (result.flagError) {
			m_flagWriteError = sl_true;
		}

		Ref<Buffer> bufferWriting = m_bufferWriting;
		m_bufferWriting.setNull();

		if (bufferWriting.isNotNull()) {
			m_sizeWritten += result.size;
			bufferWriting->memWrite.setNull();
			m_buffersRead.pushBack(bufferWriting);
			dispatchWrite();
		}

		enqueue();
	}
	
	void AsyncCopy::enqueue()
	{
		if (!m_flagRunning) {
			return;
		}

		if (m_flagEnqueue) {
			return;
		}
		m_flagEnqueue = sl_true;

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
				m_bufferReading = buffer;
				if (buffer->memRead.isNotNull()) {
					Ref<AsyncStream> source = m_source;
					if (source.isNotNull()) {
						bRet = source->readToMemory(buffer->memRead, SLIB_FUNCTION_WEAKREF(AsyncCopy, onReadStream, this));
					}
				}
				if (!bRet) {
					m_bufferReading.setNull();
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
				m_bufferWriting = buffer;
				sl_bool bRet = sl_false;
				Ref<AsyncStream> target = m_target;
				if (target.isNotNull()) {
					bRet = target->writeFromMemory(buffer->memWrite, SLIB_FUNCTION_WEAKREF(AsyncCopy, onWriteStream, this));
				}
				if (!bRet) {
					m_bufferWriting.setNull();
					m_flagWriteError = sl_true;
				}
			}
		} while (0);

		if (m_bufferReading.isNull() && m_bufferWriting.isNull()) {
			close();
		}

		m_flagEnqueue = sl_false;

	}
	
	Memory AsyncCopy::dispatchRead(const Memory& input)
	{
		if (m_onRead.isNotNull()) {
			return m_onRead(this, input);
		} else {
			return input;
		}
	}
	
	void AsyncCopy::dispatchWrite()
	{
		m_onWrite(this);
	}
	
	void AsyncCopy::dispatchEnd()
	{
		m_onEnd(this, isErrorOccured());
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
	
	AsyncOutputBufferElement::~AsyncOutputBufferElement()
	{
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

	SLIB_DEFINE_OBJECT(AsyncOutputBuffer, Object)
	
	AsyncOutputBuffer::AsyncOutputBuffer()
	{
		m_lengthOutput = 0;
	}

	AsyncOutputBuffer::~AsyncOutputBuffer()
	{
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
		if (mem.isNull()) {
			return sl_false;
		}
		ObjectLocker lock(this);
		Link< Ref<AsyncOutputBufferElement> >* link = m_queueOutput.getBack();
		if (link && link->value->isEmptyBody()) {
			if (link->value->addHeader(mem)) {
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
		Link< Ref<AsyncOutputBufferElement> >* link = m_queueOutput.getBack();
		if (link && link->value->isEmptyBody()) {
			link->value->setBody(stream, size);
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
#if defined(SLIB_PLATFORM_IS_WIN32)
		if (File::exists(path)) {
			sl_uint64 size = File::getSize(path);
			if (size > 0) {
				Ref<AsyncStream> file = AsyncFile::openIOCP(path, FileMode::Read);
				if (file.isNotNull()) {
					return copyFrom(file.get(), size);
				}
			}
		}
		return sl_false;
#else
		return copyFromFile(path, Ref<Dispatcher>::null());
#endif
	}

	sl_bool AsyncOutputBuffer::copyFromFile(const String& path, const Ref<Dispatcher>& dispatcher)
	{
		if (!(File::exists(path))) {
			return sl_false;
		}
		sl_uint64 size = File::getSize(path);
		if (size > 0) {
			Ref<AsyncFile> file = AsyncFile::openForRead(path, dispatcher);
			if (file.isNotNull()) {
				return copyFrom(file.get(), size);
			} else {
				return sl_false;
			}
		}
		return sl_true;
	}

	sl_uint64 AsyncOutputBuffer::getOutputLength() const
	{
		return m_lengthOutput;
	}

/**********************************************
				AsyncOutput
**********************************************/
	
	SLIB_DEFINE_CLASS_DEFAULT_MEMBERS(AsyncOutputParam)
	
	AsyncOutputParam::AsyncOutputParam()
	{
		bufferSize = 0x10000;
		bufferCount = 3;
	}

	SLIB_DEFINE_OBJECT(AsyncOutput, AsyncOutputBuffer)

	AsyncOutput::AsyncOutput()
	{
		m_flagClosed = sl_false;
		m_flagWriting = sl_false;

		m_bufferCount = 1;
		m_bufferSize = 0x10000;
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

	Ref<AsyncOutput> AsyncOutput::create(const AsyncOutputParam& param)
	{
		if (param.stream.isNull()) {
			return sl_null;
		}
		Memory buffer = Memory::create(param.bufferSize);
		if (buffer.isNull()) {
			return sl_null;
		}
		Ref<AsyncOutput> ret = new AsyncOutput;
		if (ret.isNotNull()) {
			ret->m_streamOutput = param.stream;
			ret->m_bufferSize = param.bufferSize;
			ret->m_bufferCount = param.bufferCount;
			ret->m_onEnd = param.onEnd;
			ret->m_bufWrite = buffer;
			return ret;
		}
		return sl_null;
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
				if (!(m_streamOutput->write(m_bufWrite.getData(), size, SLIB_FUNCTION_WEAKREF(AsyncOutput, onWriteStream, this), m_bufWrite.ref.get()))) {
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
				AsyncCopyParam param;
				param.source = body;
				param.target = m_streamOutput;
				param.size = sizeBody;
				param.bufferSize = m_bufferSize;
				param.bufferCount = m_bufferCount;
				param.onEnd = SLIB_FUNCTION_WEAKREF(AsyncOutput, onAsyncCopyEnd, this);
				Ref<AsyncCopy> copy = AsyncCopy::create(param);
				if (copy.isNotNull()) {
					m_copy = copy;
				} else {
					m_flagWriting = sl_false;
					_onError();
				}
			}
		}
	}

	void AsyncOutput::onAsyncCopyEnd(AsyncCopy* task, sl_bool flagError)
	{
		m_flagWriting = sl_false;
		if (flagError || !(task->isCompleted())) {
			_onError();
		} else {
			_write(sl_true);
		}
	}

	void AsyncOutput::onWriteStream(AsyncStreamResult& result)
	{
		m_flagWriting = sl_false;
		if (result.flagError) {
			_onError();
			return;
		}
		_write(sl_true);
	}

	void AsyncOutput::_onError()
	{
		m_onEnd(this, sl_true);
	}

	void AsyncOutput::_onComplete()
	{
		m_onEnd(this, sl_false);
	}


/**********************************************
		AsyncStreamFilter
**********************************************/

	SLIB_DEFINE_OBJECT(AsyncStreamFilter, AsyncStream)

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
		MultipleMutexLocker lock(&m_lockReading, &m_lockWriting);
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

	sl_bool AsyncStreamFilter::read(void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& callback, Referable* userObject)
	{
		Ref<AsyncStreamRequest> request = AsyncStreamRequest::createRead(data, size, userObject, callback);
		if (request.isNotNull()) {
			MutexLocker lock(&m_lockReading);
			if (!m_flagOpened) {
				return sl_false;
			}
			if (m_requestsRead.push(request)) {
				if (m_flagReadingEnded) {
					return sl_false;
				}
				return _read();
			}
		}
		return sl_false;
	}

	void AsyncStreamFilter::addReadData(void* data, sl_uint32 size, Referable* userObject)
	{
		if (size > 0) {
			MutexLocker lock(&m_lockReading);
			Memory memConv = filterRead(data, size, userObject);
			if (memConv.isNotNull()) {
				m_bufReadConverted.add(memConv);
			}
		}
	}

	void AsyncStreamFilter::addReadData(const Memory& data)
	{
		if (data.isNotNull()) {
			sl_size size = data.getSize();
			if (size >= 0x80000000) {
				return;
			}
			addReadData(data.getData(), (sl_uint32)(size), data.ref.get());
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

	sl_bool AsyncStreamFilter::_read()
	{
		Ref<AsyncStream> stream = m_stream;
		if (stream.isNull()) {
			return sl_false;
		}
		if (m_flagReading) {
			return sl_false;
		}
		if (m_flagReadingEnded) {
			return sl_false;
		}
		if (m_flagReadingError) {
			return sl_false;
		}
		do {
			Function<void(AsyncStreamResult&)> callback = SLIB_FUNCTION_WEAKREF(AsyncStreamFilter, onReadStream, this);
			if (m_bufReadConverted.getSize() > 0) {
				if (!(stream->read(sl_null, 0, callback))) {
					break;
				}
			}
			Memory mem = m_memReading;
			if (mem.isNull()) {
				mem = Memory::create(SLIB_ASYNC_STREAM_FILTER_DEFAULT_BUFFER_SIZE);
				if (mem.isNull()) {
					break;
				}
				m_memReading = mem;
			}
			if (stream->readToMemory(m_memReading, callback)) {
				m_flagReading = sl_true;
				return sl_true;
			}
		} while (0);
		m_flagReadingError = sl_true;
		_closeAllReadRequests();
		return sl_false;
	}

	void AsyncStreamFilter::onReadStream(AsyncStreamResult& result)
	{
		MutexLocker lock(&m_lockReading);
		m_flagReading = sl_false;
		if (!m_flagOpened) {
			return;
		}
		if (result.size > 0) {
			addReadData(result.data, result.size, result.userObject);
		}
		if (result.flagError) {
			m_flagReadingError = sl_true;
		}
		if (m_bufReadConverted.getSize() > 0) {
			for (;;) {
				Ref<AsyncStreamRequest> req;
				if (!(m_requestsRead.pop(&req))) {
					return;
				}
				if (req.isNotNull()) {
					sl_uint32 m = 0;
					if (req->data && req->size) {
						sl_size _m = m_bufReadConverted.pop(req->data, req->size);
						m = (sl_uint32)(_m);
						if (m_bufReadConverted.getSize() == 0) {
							req->runCallback(this, m, m_flagReadingError);
							break;
						} else {
							req->runCallback(this, m, sl_false);
						}
					} else {
						req->runCallback(this, req->size, sl_false);
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

	namespace priv
	{
		namespace async
		{
			class AsyncStreamFilter_WriteRequest : public AsyncStreamRequest
			{
			public:
				Memory memConverted;
				
			public:
				AsyncStreamFilter_WriteRequest(const Memory& _memConv, const void* data, sl_uint32 size, Referable* userObject, const Function<void(AsyncStreamResult&)>& callback)
				: AsyncStreamRequest(data, size, userObject, callback, sl_false), memConverted(_memConv)
				{
				}
				
			};
		}
	}

	sl_bool AsyncStreamFilter::write(const void* data, sl_uint32 size, const Function<void(AsyncStreamResult&)>& callback, Referable* userObject)
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
		if (data && size) {
			Memory memConv = filterWrite(data, size, userObject);
			Ref<priv::async::AsyncStreamFilter_WriteRequest> req = new priv::async::AsyncStreamFilter_WriteRequest(memConv, data, size, userObject, callback);
			if (req.isNotNull()) {
				return stream->write(memConv.getData(), (sl_uint32)(memConv.getSize()), SLIB_FUNCTION_WEAKREF(AsyncStreamFilter, onWriteStream, this), req.get());
			}
		} else {
			return stream->write(data, size, callback, userObject);
		}
		return sl_false;
	}

	void AsyncStreamFilter::onWriteStream(AsyncStreamResult& result)
	{
		if (result.flagError) {
			m_flagWritingError = sl_true;
		}
		priv::async::AsyncStreamFilter_WriteRequest* req = (priv::async::AsyncStreamFilter_WriteRequest*)(result.userObject);
		if (req) {
			req->runCallback(this, req->size, m_flagWritingError);
		}
	}

	sl_bool AsyncStreamFilter::addTask(const Function<void()>& callback)
	{
		Ref<AsyncStream> stream = m_stream;
		if (stream.isNotNull()) {
			return stream->addTask(callback);
		}
		return sl_false;
	}

	Memory AsyncStreamFilter::filterRead(void* data, sl_uint32 size, Referable* userObject)
	{
		return Memory::createStatic(data, size, userObject);
	}

	Memory AsyncStreamFilter::filterWrite(const void* data, sl_uint32 size, Referable* userObject)
	{
		return Memory::createStatic(data, size, userObject);
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
				req->runCallback(this, 0, sl_true);
			}
		}
	}

}
