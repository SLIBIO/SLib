/*
 *   Copyright (c) 2008-2019 SLIBIO <https://github.com/SLIBIO>
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

namespace slib
{

	template <class T>
	SLIB_INLINE CPromise<T>::CPromise(): m_state(PromiseState::Pending)
	{
	}
	
	template <class T>
	template <class... ARGS>
	SLIB_INLINE CPromise<T>::CPromise(PromiseState state, ARGS&&... args): m_state(state)
	{
		new ((T*)((void*)m_result)) T(Forward<ARGS>(args)...);
	}
	
	template <class T>
	SLIB_INLINE CPromise<T>::~CPromise()
	{
		if (m_state != PromiseState::Pending) {
			(*((T*)((void*)m_result))).T::~T();
		}
	}
	
	template <class T>
	PromiseState CPromise<T>::getState()
	{
		return m_state;
	}
	
	template <class T>
	template <class... ARGS>
	void CPromise<T>::resolve(ARGS&&... args)
	{
		SpinLocker lock(&m_lock);
		if (m_state != PromiseState::Pending) {
			return;
		}
		new ((T*)((void*)m_result)) T(Forward<ARGS>(args)...);
		auto callback = Move(m_callback);
		if (callback.isNotNull()) {
			m_state = PromiseState::Completed;
			lock.unlock();
			callback(*((T*)((void*)m_result)));
		} else {
			m_state = PromiseState::Resolved;
		}
	}
	
	template <class T>
	template <class CALLBACK>
	void CPromise<T>::then(CALLBACK&& callback)
	{
		SpinLocker lock(&m_lock);
		if (m_state == PromiseState::Resolved) {
			m_state = PromiseState::Completed;
			lock.unlock();
			callback(*((T*)((void*)m_result)));
		} else if (m_state == PromiseState::Pending) {
			m_callback = Forward<CALLBACK>(callback);
		}
	}
	
	
	template <class T>
	SLIB_INLINE Promise<T> Promise<T>::create()
	{
		return new CPromise<T>();
	}
	
	template <class T>
	template <class... ARGS>
	SLIB_INLINE Promise<T> Promise<T>::fromValue(ARGS&&... args)
	{
		return new CPromise<T>(PromiseState::Resolved, Forward<ARGS>(args)...);
	}
	
	template <class T>
	SLIB_INLINE void Promise<T>::initialize()
	{
		ref = new CPromise<T>();
	}
	
	template <class T>
	PromiseState Promise<T>::getState() const
	{
		CPromise<T>* object = ref._ptr;
		if (object) {
			object->getState();
		} else {
			return PromiseState::Resolved;
		}
	}
	
	template <class T>
	template <class... ARGS>
	SLIB_INLINE void Promise<T>::resolve(ARGS&&... args) const
	{
		CPromise<T>* object = ref._ptr;
		if (object) {
			object->resolve(Forward<ARGS>(args)...);
		}
	}
	
	template <class T>
	template <class CALLBACK>
	SLIB_INLINE void Promise<T>::then(CALLBACK&& callback) const
	{
		CPromise<T>* object = ref._ptr;
		if (object) {
			object->then(Forward<CALLBACK>(callback));
		} else {
			T result = NullValue<T>::get();
			callback(result);
		}
	}
	
	template <class T>
	template <class RET>
	Promise<RET> Promise<T>::thenReturn(const Function<RET(T& result)>& callback) const
	{
		Promise<RET> promise;
		promise.initialize();
		then([promise, callback](T& result) {
			promise.resolve(callback(result));
		});
		return promise;
	}
	
	template <class T>
	template <class RET>
	Promise<RET> Promise<T>::thenPromise(const Function<Promise<RET>(T& result)>& callback) const
	{
		Promise<RET> promise;
		promise.initialize();
		then([promise, callback](T& result) {
			callback(result).then([promise](RET& ret) {
				promise.resolve(Move(ret));
			});
		});
		return promise;
	}
	
	template <class T>
	sl_bool Promise<T>::wait(sl_int32 timeoutMilliseconds) const
	{
		Ref<Event> event = Event::create();
		then([&event](T& result) {
			event->set();
		});
		return event->wait(timeoutMilliseconds);
	}
	
	template <class T>
	sl_bool Promise<T>::wait(T* _output, sl_int32 timeoutMilliseconds) const
	{
		Ref<Event> event = Event::create();
		then([&event, _output](T& result) {
			if (_output) {
				*_output = Move(result);
			}
			event->set();
		});
		return event->wait(timeoutMilliseconds);
	}
	
	namespace priv
	{
		namespace promise
		{
			
			template <class T>
			struct PromiseAllContext : public Referable
			{
				SpinLock lock;
				List<T> results;
				sl_reg nCompleted;
			};
			
		}
	}
	
	template <class T>
	template <class... PROMISES>
	Promise< List<T> > Promise<T>::all(PROMISES&&... _promises)
	{
		Promise<T> promises[] = {Forward<PROMISES>(_promises)...};
		sl_size n = sizeof...(_promises);
		if (!n) {
			return sl_null;
		}
		Ref< priv::promise::PromiseAllContext<T> > refContext = new priv::promise::PromiseAllContext<T>;
		priv::promise::PromiseAllContext<T>* context = refContext.get();
		context->results = List<T>::create(n);
		if (context->results.isNull()) {
			return sl_null;
		}
		context->nCompleted = 0;
		Promise< List<T> > ret;
		ret.initialize();
		for (sl_size i = 0; i < n; i++) {
			Promise<T>& promise = promises[i];
			sl_size index = i;
			promise.then([ret, index, n, context, refContext](T& result) {
				context->results.setAt_NoLock(index, Move(result));
				if (Base::interlockedIncrement(&(context->nCompleted)) == n) {
					ret.resolve(context->results);
				}
			});
		}
		return ret;
	}
	
	template <class T>
	Promise< List<T> > Promise<T>::allList(const List< Promise<T> >& _promises)
	{
		sl_size n = _promises.getCount();
		if (!n) {
			return sl_null;
		}
		ListElements< Promise<T> > promises(_promises.duplicate());
		if (!(promises.count)) {
			return sl_null;
		}
		Ref< priv::promise::PromiseAllContext<T> > refContext = new priv::promise::PromiseAllContext<T>;
		priv::promise::PromiseAllContext<T>* context = refContext.get();
		context->results = List<T>::create(n);
		if (context->results.isNull()) {
			return sl_null;
		}
		context->nCompleted = 0;
		Promise< List<T> > ret;
		ret.initialize();
		for (sl_size i = 0; i < promises.count; i++) {
			Promise<T>& promise = promises[i];
			sl_size index = i;
			promise.then([ret, index, n, context, refContext](T& result) {
				context->results.setAt_NoLock(index, Move(result));
				if (Base::interlockedIncrement(&(context->nCompleted)) == n) {
					ret.resolve(context->results);
				}
			});
		}
		return ret;
	}
	
	template <class T>
	template <class... PROMISES>
	Promise<T> Promise<T>::race(PROMISES&&... _promises)
	{
		Promise<T> promises[] = {Forward<PROMISES>(_promises)...};
		sl_size n = sizeof...(_promises);
		if (!n) {
			return sl_null;
		}
		Promise<T> ret;
		ret.initialize();
		for (sl_size i = 0; i < n; i++) {
			Promise<T>& promise = promises[i];
			promise.then([ret](T& result) {
				ret.resolve(Move(result));
			});
		}
		return ret;
	}
	
	template <class T>
	Promise<T> Promise<T>::raceList(const List< Promise<T> >& _promises)
	{
		sl_size n = _promises.getCount();
		if (!n) {
			return sl_null;
		}
		ListElements< Promise<T> > promises(_promises.duplicate());
		if (!(promises.count)) {
			return sl_null;
		}
		Promise<T> ret;
		ret.initialize();
		for (sl_size i = 0; i < promises.count; i++) {
			Promise<T>& promise = promises[i];
			promise.then([ret](T& result) {
				ret.resolve(Move(result));
			});
		}
		return ret;
	}

}
