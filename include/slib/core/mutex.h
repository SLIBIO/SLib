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

#ifndef CHECKHEADER_SLIB_CORE_MUTEX
#define CHECKHEADER_SLIB_CORE_MUTEX

#include "definition.h"

#include "spin_lock.h"

namespace slib
{
	
	class SLIB_EXPORT Mutex
	{
	public:
		Mutex() noexcept;

		Mutex(const Mutex& other) noexcept;
		
		Mutex(Mutex&& other) noexcept;

		~Mutex() noexcept;
	
	public:
		sl_bool tryLock() const noexcept;

		void lock() const noexcept;

		void unlock() const noexcept;
		
		SpinLock* getSpinLock() const noexcept;

	public:
		Mutex& operator=(const Mutex& other) noexcept;
	
		Mutex& operator=(Mutex&& other) noexcept;
		
	private:
		mutable void* m_object;
		SpinLock m_lock;

	private:
		void* _initObject() const noexcept;
		
		void* _getObject() const noexcept;

	};
	
	
	class SLIB_EXPORT MutexLocker
	{
	public:
		MutexLocker() noexcept;
		
		MutexLocker(const Mutex* mutex) noexcept;
		
		MutexLocker(const MutexLocker& other) = delete;
		
		MutexLocker(MutexLocker&& other) = delete;
		
		~MutexLocker() noexcept;
		
	public:
		void lock(const Mutex* mutex) noexcept;
		
		void unlock() noexcept;
		
	public:
		MutexLocker& operator=(const MutexLocker& other) = delete;
		
		MutexLocker& operator=(MutexLocker&& other) = delete;
		
	private:
		const Mutex* m_mutex;
		
	};
	
	
#define SLIB_MAX_LOCK_MUTEX 16
	
	class SLIB_EXPORT MultipleMutexLocker
	{
	public:
		MultipleMutexLocker() noexcept;
	
		MultipleMutexLocker(const Mutex* mutex) noexcept;
	
		MultipleMutexLocker(const Mutex* mutex1, const Mutex* mutex2) noexcept;

		MultipleMutexLocker(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3) noexcept;
	
		MultipleMutexLocker(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3, const Mutex* mutex4) noexcept;
	
		MultipleMutexLocker(Mutex const* const* mutex_array, sl_size count) noexcept;
		
		MultipleMutexLocker(const MultipleMutexLocker& other) = delete;
		
		MultipleMutexLocker(MultipleMutexLocker&& other) = delete;

		~MultipleMutexLocker() noexcept;

	public:
		void lock(const Mutex* mutex) noexcept;

		void lock(const Mutex* mutex1, const Mutex* mutex2) noexcept;

		void lock(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3) noexcept;

		void lock(const Mutex* mutex1, const Mutex* mutex2, const Mutex* mutex3, const Mutex* mutex4) noexcept;

		void lock(Mutex const* const* mutex_array, sl_size count) noexcept;
	
		void unlock() noexcept;
		
	public:
		MultipleMutexLocker& operator=(const MultipleMutexLocker& other) = delete;
		
		MultipleMutexLocker& operator=(MultipleMutexLocker&& other) = delete;

	private:
		sl_size m_count;
		const Mutex* m_mutex[SLIB_MAX_LOCK_MUTEX];

	};

}

#endif
