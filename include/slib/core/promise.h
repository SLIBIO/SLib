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

#ifndef CHECKHEADER_SLIB_CORE_PROMISE
#define CHECKHEADER_SLIB_CORE_PROMISE

#include "definition.h"

#include "function.h"
#include "event.h"

// Javascript-style Promise

namespace slib
{
	
	template <class T>
	class CPromise;
	
	template <class T>
	class Promise;
	
	template <class T>
	using AtomicPromise = Atomic< Promise<T> >;
	
	enum class PromiseState
	{
		Pending = 0,
		Resolved = 1,
		Completed = 2
	};
	
	class SLIB_EXPORT CPromiseBase : public Referable
	{
		SLIB_DECLARE_OBJECT
		
	public:
		CPromiseBase() noexcept;
		
		~CPromiseBase() noexcept;
		
	};
	
	template <class T>
	class SLIB_EXPORT CPromise : public CPromiseBase
	{
	private:
		PromiseState m_state;
		Function<void(T& result)> m_callback;
		SpinLock m_lock;
		sl_uint8 m_result[sizeof(T)];

	public:
		CPromise();
		
		template <class... ARGS>
		CPromise(PromiseState state, ARGS&&... args);
		
		~CPromise();
		
	public:
		PromiseState getState();
		
		template <class... ARGS>
		void resolve(ARGS&&... args);
		
		template <class CALLBACK>
		void then(CALLBACK&& callback);
		
	};
	
	template <class T>
	class SLIB_EXPORT Promise
	{
	public:
		Ref< CPromise<T> > ref;
		SLIB_REF_WRAPPER(Promise, CPromise<T>)

	public:
		static Promise<T> create();
		
		template <class... ARGS>
		static Promise<T> fromValue(ARGS&&... args);
		
		void initialize();
		
		PromiseState getState() const;
		
		template <class... ARGS>
		void resolve(ARGS&&... args) const;
		
		template <class CALLBACK>
		void then(CALLBACK&& callback) const;

		template <class RET>
		Promise<RET> thenReturn(const Function<RET(T& result)>& callback) const;
		
		template <class RET>
		Promise<RET> thenPromise(const Function<Promise<RET>(T& result)>& callback) const;
		
		// don't use mixed with `then()`, timeoutMilliseconds: negative means infinite timeout
		sl_bool wait(sl_int32 timeoutMilliseconds = -1) const;
		
		// don't use mixed with `then()`, timeoutMilliseconds: negative means infinite timeout
		sl_bool wait(T* _output, sl_int32 timeoutMilliseconds = -1) const;
		
		static Promise< List<T> > all(const List< Promise<T> >& promises);

		static Promise<T> race(const List< Promise<T> >& promises);
		
	};
	
	template <class T>
	class SLIB_EXPORT Atomic< Promise<T> >
	{
	public:
		AtomicRef< CPromise<T> > ref;
		SLIB_ATOMIC_REF_WRAPPER(CPromise<T>)
	};
	
}

#include "detail/promise.inc"

#endif
